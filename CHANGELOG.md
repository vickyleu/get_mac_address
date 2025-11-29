## 0.0.4

- Android now attempts multiple strategies (network interfaces → Wi-Fi info → hashed `ANDROID_ID`) and only returns the placeholder when all fail. Added the necessary permission declarations.[^issue4][^issue5][^issue7]
- iOS returns a deterministic pseudo-MAC derived from `identifierForVendor` instead of a hardcoded value.
- Web generates a stable pseudo-MAC and keeps it in `localStorage`.
- Updated documentation to explain the fallback hierarchy and required permissions.

## 0.0.3

- Fixed the Windows desktop implementation so it always returns valid UTF-8 strings and falls back to `00:00:00:00:00:00` instead of crashing with `FormatException`.[^issue9]
- Verified the Linux and Windows packages ship with their `CMakeLists.txt` files to address missing-file build failures.[^issue2][^issue8]
- Documented platform limitations for Web and Android/iOS privacy restrictions.[^issue1][^issue4][^issue5][^issue7]

## 0.0.2

- Added desktop targets (Windows, Linux, macOS) and a web stub implementation.

## 0.0.1

- Initial Android/iOS implementation scaffold.

[^issue1]: https://github.com/vickyleu/get_mac_address/issues/1
[^issue2]: https://github.com/vickyleu/get_mac_address/issues/2
[^issue4]: https://github.com/vickyleu/get_mac_address/issues/4
[^issue5]: https://github.com/vickyleu/get_mac_address/issues/5
[^issue7]: https://github.com/vickyleu/get_mac_address/issues/7
[^issue8]: https://github.com/vickyleu/get_mac_address/issues/8
[^issue9]: https://github.com/vickyleu/get_mac_address/issues/9
