#include "get_mac_address_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>
#include <iptypes.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>

namespace get_mac_address {

// static
void GetMacAddressPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
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

GetMacAddressPlugin::GetMacAddressPlugin() {}

GetMacAddressPlugin::~GetMacAddressPlugin() {}

void GetMacAddressPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("getMacAddress") == 0) {
    char* pMac = get_mac_address::GetMacAddressPlugin::getMAC();
    std::string macAddress = std::string(pMac);
    free(pMac);
    result->Success(flutter::EncodableValue(macAddress));
  } else {
    result->NotImplemented();
  }
}


char* GetMacAddressPlugin::getMAC() {
    PIP_ADAPTER_INFO AdapterInfo;
    DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
    char *mac_addr = (char*)malloc(18);
    AdapterInfo=(IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
    if (AdapterInfo==NULL){
    printf("Error allocating memory needed to call GetAdaptersinfo\n");
    free(mac_addr);
    return NULL; // it is safe to call free(NULL)
  }

  // Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
  if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
    free(AdapterInfo);
    AdapterInfo = (IP_ADAPTER_INFO *) malloc(dwBufLen);
    if (AdapterInfo == NULL) {
      printf("Error allocating memory needed to call GetAdaptersinfo\n");
      free(mac_addr);
      return NULL;
    }
  }

  if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
    // Contains pointer to current adapter info
    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    do {
      // technically should look at pAdapterInfo->AddressLength
      //   and not assume it is 6.
      sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
        pAdapterInfo->Address[0], pAdapterInfo->Address[1],
        pAdapterInfo->Address[2], pAdapterInfo->Address[3],
        pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
      printf("Address: %s, mac: %s\n", pAdapterInfo->IpAddressList.IpAddress.String, mac_addr);
      // print them all, return the last one.
      // return mac_addr;

      printf("\n");
      pAdapterInfo = pAdapterInfo->Next;
    } while(pAdapterInfo);
  }
  free(AdapterInfo);
  return mac_addr; // caller must free.
}



}  // namespace get_mac_address
