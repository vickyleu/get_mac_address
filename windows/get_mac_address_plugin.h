#ifndef FLUTTER_PLUGIN_GET_MAC_ADDRESS_PLUGIN_H_
#define FLUTTER_PLUGIN_GET_MAC_ADDRESS_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace get_mac_address {

class GetMacAddressPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  GetMacAddressPlugin();

  virtual ~GetMacAddressPlugin();

  // Disallow copy and assign.
  GetMacAddressPlugin(const GetMacAddressPlugin&) = delete;
  GetMacAddressPlugin& operator=(const GetMacAddressPlugin&) = delete;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  char* getMAC();

};

}  // namespace get_mac_address

#endif  // FLUTTER_PLUGIN_GET_MAC_ADDRESS_PLUGIN_H_
