import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'get_mac_address_platform_interface.dart';

/// An implementation of [GetMacAddressPlatform] that uses method channels.
class MethodChannelGetMacAddress extends GetMacAddressPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('get_mac_address');

  @override
  Future<String?> getMacAddress() async {
    final version = await methodChannel.invokeMethod<String>('getMacAddress');
    return version;
  }
}
