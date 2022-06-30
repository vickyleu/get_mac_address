
import 'get_mac_address_platform_interface.dart';

class GetMacAddress {
  Future<String?> getMacAddress() {
    return GetMacAddressPlatform.instance.getMacAddress();
  }
}
