//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <get_mac_address/get_mac_address_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) get_mac_address_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "GetMacAddressPlugin");
  get_mac_address_plugin_register_with_registrar(get_mac_address_registrar);
}
