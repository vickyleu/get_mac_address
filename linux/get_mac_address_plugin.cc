#include "include/get_mac_address/get_mac_address_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>

#define GET_MAC_ADDRESS_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), get_mac_address_plugin_get_type(), \
                              GetMacAddressPlugin))

struct _GetMacAddressPlugin {
  GObject parent_instance;
};

G_DEFINE_TYPE(GetMacAddressPlugin, get_mac_address_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void get_mac_address_plugin_handle_method_call(
    GetMacAddressPlugin* self,
    FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);

  if (strcmp(method, "getMacAddress") == 0) {
      struct ifreq s;
      int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
      strcpy(s.ifr_name, "eth0");
      if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) {
        int i;
        //g_autofree gchar *version = g_strdup_printf("Linux %s", uname_data.version);
        g_autofree gchar *mac_addr = (char*)malloc(18);
        sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
                (unsigned char) s.ifr_addr.sa_data[0], (unsigned char) s.ifr_addr.sa_data[1],
                (unsigned char) s.ifr_addr.sa_data[2], (unsigned char) s.ifr_addr.sa_data[3],
                (unsigned char) s.ifr_addr.sa_data[4], (unsigned char) s.ifr_addr.sa_data[5]);
        g_autoptr(FlValue) result = fl_value_new_string(g_strdup_printf("%s", mac_addr));
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
      }else{
        g_autoptr(FlValue) result = fl_value_new_string("mac address get failure");
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
      }
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void get_mac_address_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(get_mac_address_plugin_parent_class)->dispose(object);
}

static void get_mac_address_plugin_class_init(GetMacAddressPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = get_mac_address_plugin_dispose;
}

static void get_mac_address_plugin_init(GetMacAddressPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  GetMacAddressPlugin* plugin = GET_MAC_ADDRESS_PLUGIN(user_data);
  get_mac_address_plugin_handle_method_call(plugin, method_call);
}

void get_mac_address_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  GetMacAddressPlugin* plugin = GET_MAC_ADDRESS_PLUGIN(
      g_object_new(get_mac_address_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "get_mac_address",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}
