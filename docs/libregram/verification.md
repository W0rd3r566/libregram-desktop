# Verification record

Environment: Linux (CachyOS), system Qt 6.12.0, C++20. Only Debug / `-g -O0` compilation was used. No personal API credentials were supplied, no account was opened and no public release was produced.

## Full client

The Linux Debug build of the `Telegram` target completed successfully on CachyOS with Qt 6.12.0. Boost, WebRTC, tde2e and the missing generators were supplied from a workspace-local prefix. The build used the repository's `TDESKTOP_API_TEST` option.

The resulting executable rendered the LibreGram Desktop welcome screen under Xvfb with a fresh profile. The welcome illustration is still inherited from Kotatogram. Startup logged a network error for a missing HTTP `Date` header. Account sign-in, messaging, settings persistence, account switching and interactive theme preview were not tested.

## Completed host checks

Reproduce from the repository root:

```sh
Telegram/build/tests/check_libregram.sh /tmp/libregram-checks
```

Requires initialized submodules, C++20 compiler, Python 3, ripgrep, pkg-config and Qt 6 Core/Gui/Widgets/OpenGLWidgets/Network/Svg development tools.

The script builds the actual upstream language/style/emoji generators; generates language keys, styles, emoji and MTProto types; parses Graphite with the upstream palette generator; runs three palette/resource tests; compiles the real Qt resource collection; builds and runs a Qt resource smoke check; and compiles `settings_libregram.cpp` into an object file against repository headers and generated files. This is a host sanity check, not the application's CMake target or an integration test.

The resource tests check palette alias cycles/missing references/duplicate keys, main text and button contrast of at least 4.5:1, the theme size limit, QRC paths and PNG signature. The Qt smoke check loads the actual RCC bundle, decodes the supplied logo, scales it to typical runtime icon sizes and reads the Graphite resource.

Additionally, C++ syntax checks passed for the changed `settings_main.cpp`, `about_box.cpp`, `intro_start.cpp`, `window_main_menu.cpp` and `main_window.cpp` translation units with generated repository headers and system Qt (KCoreAddons include path was supplied for `main_window.cpp`). `git diff --check` passed.

Observed warnings are upstream deprecated implicit `this` captures and Qt deprecations (`QImage::mirrored`, hash-combiner construction). No production workarounds for those warnings were introduced.

The manual runtime matrix is in `foundation.md`. New LibreGram language keys currently have English fallback strings; translated cloud packs do not yet contain these new keys. Installer names/IDs, executable names and update infrastructure remain inherited and require a separate release migration.
