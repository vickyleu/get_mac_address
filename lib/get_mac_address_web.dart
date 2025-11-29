// ignore: avoid_web_libraries_in_flutter
import 'dart:html' as html show window;
import 'dart:math';
import 'dart:typed_data';

import 'package:flutter_web_plugins/flutter_web_plugins.dart';

import 'get_mac_address_platform_interface.dart';

/// A web implementation that generates a stable pseudo-MAC persisted in localStorage.
class GetMacAddressWeb extends GetMacAddressPlatform {
  GetMacAddressWeb();

  static const _storageKey = 'get_mac_address::pseudo_mac';
  static const _placeholder = '00:00:00:00:00:00';

  static void registerWith(Registrar registrar) {
    GetMacAddressPlatform.instance = GetMacAddressWeb();
  }

  @override
  Future<String?> getMacAddress() async {
    final storage = html.window.localStorage;
    final cached = storage[_storageKey];
    if (cached != null && cached.isNotEmpty) {
      return cached;
    }
    final generated = _generatePseudoMac();
    storage[_storageKey] = generated;
    return generated;
  }

  String _generatePseudoMac() {
    final bytes = Uint8List(6);
    try {
      final crypto = html.window.crypto;
      if (crypto != null) {
        crypto.getRandomValues(bytes);
        return _format(bytes);
      }
    } catch (_) {
      // Fall through to Random-based generation.
    }
    final random = Random.secure();
    for (var i = 0; i < bytes.length; i++) {
      bytes[i] = random.nextInt(256);
    }
    return _format(bytes);
  }

  String _format(List<int> bytes) {
    if (bytes.isEmpty) {
      return _placeholder;
    }
    final parts = bytes.map((b) => b.toRadixString(16).padLeft(2, '0').toUpperCase());
    return parts.join(':');
  }
}
