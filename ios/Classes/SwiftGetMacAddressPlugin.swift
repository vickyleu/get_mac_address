import Flutter
import UIKit

public class SwiftGetMacAddressPlugin: NSObject, FlutterPlugin {
  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "get_mac_address", binaryMessenger: registrar.messenger())
    let instance = SwiftGetMacAddressPlugin()
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    let staticMacAddress = "02:00:00:00:00:00"
    result(staticMacAddress)
//     result(FlutterMethodNotImplemented)
  }
}