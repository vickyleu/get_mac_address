import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:get_mac_address/get_mac_address_method_channel.dart';

void main() {
  MethodChannelGetMacAddress platform = MethodChannelGetMacAddress();
  const MethodChannel channel = MethodChannel('get_mac_address');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getMacAddress', () async {
    expect(await platform.getMacAddress(), '42');
  });
}
