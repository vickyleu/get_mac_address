import Flutter
import UIKit

public class SwiftGetMacAddressPlugin: NSObject, FlutterPlugin {
  private let placeholder = "00:00:00:00:00:00"

  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "get_mac_address", binaryMessenger: registrar.messenger())
    let instance = SwiftGetMacAddressPlugin()
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    switch call.method {
    case "getMacAddress":
      result(resolveMacAddress())
    default:
      result(FlutterMethodNotImplemented)
    }
  }

  private func resolveMacAddress() -> String {
    if let pseudo = pseudoMacFromIdentifier() {
      return pseudo
    }
    return placeholder
  }

  private func pseudoMacFromIdentifier() -> String? {
    guard var uuid = UIDevice.current.identifierForVendor?.uuid else {
      return nil
    }
    return withUnsafeBytes(of: &uuid) { rawBuffer -> String? in
      let bytes = Array(rawBuffer.prefix(6))
      if bytes.count < 6 {
        return nil
      }
      return bytes
        .map { String(format: "%02X", $0) }
        .joined(separator: ":")
    }
  }
}