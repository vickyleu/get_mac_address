#include "include/get_mac_address/get_mac_address_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "GetMacAddressPlugin.h"

void GetMacAddressPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  get_mac_address::GetMacAddressPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
