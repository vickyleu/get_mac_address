# get_mac_address

Flutter plugin that exposes a single `GetMacAddress().getMacAddress()` API for retrieving the primary network interface's MAC address on desktop platforms, while providing graceful fallbacks on mobile and web targets.

## Platform support

| Platform | Support | Notes |
| --- | --- | --- |
| **Windows** | ✅ Real MAC via `GetAdaptersInfo` | Requires Visual Studio + CMake 3.14+. `v0.0.3` fixes corrupted UTF-8 responses that triggered `FormatException` reports.[^issue9] |
| **Linux** | ✅ Real MAC via `ioctl(SIOCGIFHWADDR)` | Works once the Flutter project is created with `--platforms=linux` and system packages `build-essential` `cmake` `pkg-config` are installed. Missing `CMakeLists.txt` errors from issue reporters are resolved after republishing `v0.0.3`.[^issue2] |
| **macOS** | ✅ Real MAC via IOKit |
| **Android** | ⚠️ Restricted | Attempts real hardware interfaces first, then Wi‑Fi info (requires `ACCESS_WIFI_STATE` + location permissions), and finally hashes `ANDROID_ID` into a deterministic pseudo-MAC before falling back to the placeholder.[^issue4][^issue5][^issue7] |
| **iOS** | ⚠️ Restricted | Derives a deterministic pseudo-MAC from `identifierForVendor`; returns the placeholder only when unavailable. |
| **Web** | ⚠️ Not possible | Generates a random pseudo-MAC once and persists it via `localStorage` so it stays stable per browser profile.[^issue1] |

[^issue1]: [Need solution for web · Issue #1](https://github.com/vickyleu/get_mac_address/issues/1)
[^issue2]: [linux编译报错 · Issue #2](https://github.com/vickyleu/get_mac_address/issues/2)
[^issue4]: [not working on android 13 · Issue #4](https://github.com/vickyleu/get_mac_address/issues/4)
[^issue5]: [Transform's input file does not exist android 11 · Issue #5](https://github.com/vickyleu/get_mac_address/issues/5)
[^issue7]: [Don't work on android · Issue #7](https://github.com/vickyleu/get_mac_address/issues/7)
[^issue8]: [Cmake Error Windows · Issue #8](https://github.com/vickyleu/get_mac_address/issues/8)
[^issue9]: [FormatException: Missing extension byte (at offset 1) · Issue #9](https://github.com/vickyleu/get_mac_address/issues/9)

## Installing

```bash
flutter pub add get_mac_address
```

Use the API from Dart:

```dart
final plugin = GetMacAddress();
final mac = await plugin.getMacAddress() ?? 'Unknown';
if (mac == '02:00:00:00:00:00') {
  // Platform privacy restrictions prevented access.
}
```

## Desktop prerequisites

- **Linux:** `sudo apt install build-essential cmake pkg-config libgtk-3-dev`.
- **Windows:** Install the Desktop workload in Visual Studio 2019+ and enable `flutter config --enable-windows-desktop`.

If you still see `does not contain a CMakeLists.txt file`, wipe `linux/flutter/ephemeral` (or `windows/flutter/ephemeral`) so Flutter regenerates its plugin symlinks, or upgrade to `v0.0.3` where the files ship with the published package.[^issue2][^issue8]

## Mobile/Web permissions & fallbacks

- **Android:** Declare `ACCESS_WIFI_STATE`, `ACCESS_NETWORK_STATE`, `ACCESS_COARSE_LOCATION`, `ACCESS_FINE_LOCATION`, and on Android 13+ `NEARBY_WIFI_DEVICES`. The plugin uses whatever permissions are granted to attempt `NetworkInterface` → `WifiManager` → hashed `ANDROID_ID` before falling back to `00:00:00:00:00:00`.
- **iOS:** No extra entitlement is required. The plugin hashes `UIDevice.current.identifierForVendor` to offer a stable surrogate identifier.
- **Web:** No browser exposes the NIC MAC. We mint a random ID (6 bytes) and cache it inside `localStorage` so the result remains stable per profile.

## Known limitations

- Android and iOS may still return the placeholder when every fallback source (hardware interface, Wi‑Fi info, vendor/device identifiers) is blocked by system policy or permissions.[^issue4][^issue5][^issue7]
- Flutter web has no API for low-level network identifiers.[^issue1]

## Example

Run the bundled example on a supported desktop target:

```bash
cd example
flutter run -d linux
```

If you work on Windows, run `flutter doctor --android-licenses` and `flutter config --enable-windows-desktop` first, as missing toolchains caused the build failures described in the open issues.[^issue8]

