#include "get_mac_address_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>
#include <iptypes.h>
#include <iphlpapi.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>

#pragma comment(lib, "iphlpapi.lib")

namespace get_mac_address {

// static
void GetMacAddressPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  const auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "get_mac_address",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<GetMacAddressPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

GetMacAddressPlugin::GetMacAddressPlugin() = default;

GetMacAddressPlugin::~GetMacAddressPlugin() = default;

void GetMacAddressPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) const
{
      printf("HandleMethodCall\n");
  if (method_call.method_name() == "getMacAddress") {
    char* pMac = getMAC();
    if (pMac == nullptr) {
      result->Success(flutter::EncodableValue("00:00:00:00:00:00"));
      return;
    }
    const std::string macAddress = std::string(pMac);
    free(pMac);
    result->Success(flutter::EncodableValue(macAddress));
  } else {
    result->NotImplemented();
  }
}


char* GetMacAddressPlugin::getMAC() const
{
  DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
  auto mac_addr = static_cast<char*>(malloc(18));
  if (mac_addr == nullptr) {
    return nullptr;
  }
  mac_addr[0] = '\0';
  auto AdapterInfo = static_cast<IP_ADAPTER_INFO*>(malloc(sizeof(IP_ADAPTER_INFO)));
    if (AdapterInfo== nullptr){
        printf("Error allocating memory needed to call GetAdaptersinfo\n");
        free(mac_addr);
        return nullptr; // it is safe to call free(NULL)
    }
  // Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
  if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
    free(AdapterInfo);
    AdapterInfo = static_cast<IP_ADAPTER_INFO*>(malloc(dwBufLen));
    if (AdapterInfo == nullptr) {
      printf("Error allocating memory needed to call GetAdaptersinfo\n");
      free(mac_addr);
      return nullptr;
    }
  }

  if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
    // Contains pointer to current adapter info
    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    do {
      if (pAdapterInfo->AddressLength >= 6) {
        sprintf_s(mac_addr, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
          pAdapterInfo->Address[0], pAdapterInfo->Address[1],
          pAdapterInfo->Address[2], pAdapterInfo->Address[3],
          pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
        break;
      }
      pAdapterInfo = pAdapterInfo->Next;
    } while(pAdapterInfo);
  }
  free(AdapterInfo);
  if (mac_addr[0] == '\0') {
    sprintf_s(mac_addr, 18, "%s", "00:00:00:00:00:00");
  }
  return mac_addr; // caller must free.
}



}  // namespace get_mac_address
