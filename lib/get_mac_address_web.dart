// In order to *not* need this ignore, consider extracting the "web" version
// of your plugin as a separate package, instead of inlining it in the same
// package as the core of your plugin.
// ignore: avoid_web_libraries_in_flutter
import 'dart:html' as html show window;

import 'package:flutter_web_plugins/flutter_web_plugins.dart';

import 'get_mac_address_platform_interface.dart';

/// A web implementation of the GetMacAddressPlatform of the GetMacAddress plugin.
class GetMacAddressWeb extends GetMacAddressPlatform {
  /// Constructs a GetMacAddressWeb
  GetMacAddressWeb();

  static void registerWith(Registrar registrar) {
    GetMacAddressPlatform.instance = GetMacAddressWeb();
  }

  /// Returns a [String] containing the version of the platform.
  @override
  Future<String?> getMacAddress() async {
    const staticMacAddress = "02:00:00:00:00:00";
    return staticMacAddress;
  }
}
