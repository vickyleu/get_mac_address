package com.example.get_mac_address

import android.Manifest
import android.annotation.SuppressLint
import android.content.Context
import android.content.pm.PackageManager
import android.net.wifi.WifiManager
import android.os.Build
import android.provider.Settings
import android.util.Log
import androidx.annotation.NonNull
import androidx.core.content.ContextCompat
import io.flutter.embedding.engine.plugins.FlutterPlugin
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import io.flutter.plugin.common.MethodChannel.MethodCallHandler
import io.flutter.plugin.common.MethodChannel.Result
import java.net.NetworkInterface
import java.security.MessageDigest
import java.util.Collections
import java.util.Locale

/** GetMacAddressPlugin */
class GetMacAddressPlugin : FlutterPlugin, MethodCallHandler {
    private lateinit var channel: MethodChannel
    private lateinit var applicationContext: Context

    override fun onAttachedToEngine(@NonNull flutterPluginBinding: FlutterPlugin.FlutterPluginBinding) {
        applicationContext = flutterPluginBinding.applicationContext
        channel = MethodChannel(flutterPluginBinding.binaryMessenger, "get_mac_address")
        channel.setMethodCallHandler(this)
    }

    override fun onMethodCall(@NonNull call: MethodCall, @NonNull result: Result) {
        if (call.method == "getMacAddress") {
            result.success(resolveMacAddress())
        } else {
            result.notImplemented()
        }
    }

    override fun onDetachedFromEngine(@NonNull binding: FlutterPlugin.FlutterPluginBinding) {
        channel.setMethodCallHandler(null)
    }

    private fun resolveMacAddress(): String {
        networkInterfaceAddress()?.let { return it }
        wifiMacAddress()?.let { return it }
        pseudoMacFromAndroidId()?.let { return it }
        return PLACEHOLDER_MAC
    }

    private fun networkInterfaceAddress(): String? {
        return try {
            val interfaces = Collections.list(NetworkInterface.getNetworkInterfaces())
            for (nif in interfaces) {
                if (!nif.isUp || nif.isLoopback) continue
                val address = nif.hardwareAddress ?: continue
                val candidate = formatMac(address)
                if (candidate != null) {
                    return candidate
                }
            }
            null
        } catch (ex: Exception) {
            Log.w(TAG, "Failed to enumerate network interfaces", ex)
            null
        }
    }

    @SuppressLint("HardwareIds", "MissingPermission")
    private fun wifiMacAddress(): String? {
        val wifiManager = applicationContext.getSystemService(Context.WIFI_SERVICE) as? WifiManager
            ?: return null
        if (!hasWifiPermissions()) {
            return null
        }
        val info = wifiManager.connectionInfo ?: return null
        val mac = info.macAddress
        if (mac != null && isValidMac(mac)) {
            return mac.uppercase(Locale.US)
        }
        val bssid = info.bssid
        if (bssid != null && isValidMac(bssid)) {
            return bssid.uppercase(Locale.US)
        }
        return null
    }

    private fun hasWifiPermissions(): Boolean {
        val hasFineLocation =
            ContextCompat.checkSelfPermission(applicationContext, Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED
        val hasCoarseLocation =
            ContextCompat.checkSelfPermission(applicationContext, Manifest.permission.ACCESS_COARSE_LOCATION) == PackageManager.PERMISSION_GRANTED
        val hasWifiState =
            ContextCompat.checkSelfPermission(applicationContext, Manifest.permission.ACCESS_WIFI_STATE) == PackageManager.PERMISSION_GRANTED
        val hasNearby =
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                ContextCompat.checkSelfPermission(applicationContext, Manifest.permission.NEARBY_WIFI_DEVICES) == PackageManager.PERMISSION_GRANTED
            } else {
                true
            }
        return hasWifiState && (hasFineLocation || hasCoarseLocation || hasNearby)
    }

    private fun pseudoMacFromAndroidId(): String? {
        return try {
            val androidId = Settings.Secure.getString(applicationContext.contentResolver, Settings.Secure.ANDROID_ID)
                ?: return null
            val digest = MessageDigest.getInstance("SHA-256")
            val hash = digest.digest(androidId.toByteArray(Charsets.UTF_8))
            formatMac(hash.copyOfRange(0, 6))
        } catch (ex: Exception) {
            Log.w(TAG, "Failed to derive pseudo MAC", ex)
            null
        }
    }

    private fun formatMac(bytes: ByteArray): String? {
        if (bytes.isEmpty()) return null
        val builder = StringBuilder()
        bytes.forEach { b ->
            builder.append(String.format(Locale.US, "%02X:", b))
        }
        if (builder.isEmpty()) return null
        builder.deleteCharAt(builder.length - 1)
        val candidate = builder.toString()
        return if (isValidMac(candidate)) candidate else null
    }

    private fun isValidMac(value: String): Boolean {
        return MAC_REGEX.matches(value.uppercase(Locale.US)) && value != PLACEHOLDER_MAC
    }

    companion object {
        private const val PLACEHOLDER_MAC = "00:00:00:00:00:00"
        private const val TAG = "GetMacAddressPlugin"
        private val MAC_REGEX = Regex("^([0-9A-F]{2}:){5}[0-9A-F]{2}$")
    }
}
