import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'get_mac_address_method_channel.dart';

abstract class GetMacAddressPlatform extends PlatformInterface {
  /// Constructs a GetMacAddressPlatform.
  GetMacAddressPlatform() : super(token: _token);

  static final Object _token = Object();

  static GetMacAddressPlatform _instance = MethodChannelGetMacAddress();

  /// The default instance of [GetMacAddressPlatform] to use.
  ///
  /// Defaults to [MethodChannelGetMacAddress].
  static GetMacAddressPlatform get instance => _instance;
  
  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [GetMacAddressPlatform] when
  /// they register themselves.
  static set instance(GetMacAddressPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getMacAddress() {
    throw UnimplementedError('getMacAddress() has not been implemented.');
  }
}
