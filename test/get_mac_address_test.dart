import 'package:flutter_test/flutter_test.dart';
import 'package:get_mac_address/get_mac_address.dart';
import 'package:get_mac_address/get_mac_address_platform_interface.dart';
import 'package:get_mac_address/get_mac_address_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockGetMacAddressPlatform 
    with MockPlatformInterfaceMixin
    implements GetMacAddressPlatform {

  @override
  Future<String?> getMacAddress() => Future.value('42');
}

void main() {
  final GetMacAddressPlatform initialPlatform = GetMacAddressPlatform.instance;

  test('$MethodChannelGetMacAddress is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelGetMacAddress>());
  });

  test('getMacAddress', () async {
    GetMacAddress getMacAddressPlugin = GetMacAddress();
    MockGetMacAddressPlatform fakePlatform = MockGetMacAddressPlatform();
    GetMacAddressPlatform.instance = fakePlatform;
  
    expect(await getMacAddressPlugin.getMacAddress(), '42');
  });
}
