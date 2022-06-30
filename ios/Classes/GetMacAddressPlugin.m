#import "GetMacAddressPlugin.h"
#if __has_include(<get_mac_address/get_mac_address-Swift.h>)
#import <get_mac_address/get_mac_address-Swift.h>
#else
// Support project import fallback if the generated compatibility header
// is not copied when this plugin is created as a library.
// https://forums.swift.org/t/swift-static-libraries-dont-copy-generated-objective-c-header/19816
#import "get_mac_address-Swift.h"
#endif

@implementation GetMacAddressPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftGetMacAddressPlugin registerWithRegistrar:registrar];
}
@end
