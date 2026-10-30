# LibreGram application branding

## Source of truth

The master artwork is `Telegram/Resources/art/libregram-logo.svg`. Keep the README preview and application resources pointed at this file instead of maintaining separate hand-edited variants.

## Runtime and packaging assets

- **Qt application icon:** `Telegram/Resources/art/logo_256.png` is the 256px raster render used by the inherited runtime icon pipeline. Linux also copies this resource into the user's hicolor icon directory for the active application icon.
- **Windows executable:** `Telegram/Resources/art/icon256.ico` is referenced by `Telegram/Resources/winrc/Kotatogram.rc`. It contains the common Windows icon sizes.
- **Linux desktop integration:** the desktop entry uses the icon name `io.github.w0rd3r566.libregram`. CMake installs the SVG into hicolor's scalable app-icon directory and the 256px PNG as a fallback.

The SVG is also embedded in the Qt resource collection at `Telegram/Resources/qrc/telegram/telegram.qrc` so future UI surfaces can reuse the master artwork directly.

## Scope

This changes application identity assets only. It does not redesign Kotatogram's chat list, settings, title bar, widgets or other interface elements. macOS remains outside LibreGram's supported platform and release targets.

## Verification status

The resource paths and packaging references have been updated. A full Linux and Windows build is still required to verify the generated executable and installed desktop entry end to end.
