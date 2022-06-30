package com.example.get_mac_address

import androidx.annotation.NonNull

import io.flutter.embedding.engine.plugins.FlutterPlugin
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import io.flutter.plugin.common.MethodChannel.MethodCallHandler
import io.flutter.plugin.common.MethodChannel.Result

/** GetMacAddressPlugin */
class GetMacAddressPlugin: FlutterPlugin, MethodCallHandler {
  /// The MethodChannel that will the communication between Flutter and native Android
  ///
  /// This local reference serves to register the plugin with the Flutter Engine and unregister it
  /// when the Flutter Engine is detached from the Activity
  private lateinit var channel : MethodChannel

  override fun onAttachedToEngine(@NonNull flutterPluginBinding: FlutterPlugin.FlutterPluginBinding) {
    channel = MethodChannel(flutterPluginBinding.binaryMessenger, "get_mac_address")
    channel.setMethodCallHandler(this)
  }

  override fun onMethodCall(@NonNull call: MethodCall, @NonNull result: Result) {
    if (call.method == "getMacAddress") {
      result.success(getMacAddress())
    } else {
      result.notImplemented()
    }
  }

  override fun onDetachedFromEngine(@NonNull binding: FlutterPlugin.FlutterPluginBinding) {
    channel.setMethodCallHandler(null)
  }

  @TargetApi(Build.VERSION_CODES.GINGERBREAD)
    private fun getMacAddress(): String? {
      try {
        val all: List<NetworkInterface> = Collections.list(NetworkInterface.getNetworkInterfaces())
        for (nif in all) {
          if (!nif.getName().equalsIgnoreCase("wlan0")) continue
          val macBytes: ByteArray = nif.getHardwareAddress() ?: return ""
          val res1 = StringBuilder()
          for (b in macBytes) {
            res1.append(String.format("%02X:", b))
          }
          if (res1.length() > 0) {
            res1.deleteCharAt(res1.length() - 1)
          }
          return res1.toString()
        }
      } catch (ex: Exception) {
      }
      return "02:00:00:00:00:00"
    }

}
