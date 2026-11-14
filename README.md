<div align="center">

# LibreGram Desktop

**An experimental Telegram Desktop fork with a familiar Kotatogram-inspired experience.**

[![Development branch](https://img.shields.io/badge/development-main-20242B?style=flat-square)](https://github.com/W0rd3r566/libregram-desktop/tree/main)
[![License](https://img.shields.io/badge/license-see%20LICENSE-20242B?style=flat-square)](LICENSE)

[LibreGram on Telegram](https://t.me/libregramm)

</div>

> **Project status:** experimental. LibreGram is under active development. Interfaces, build steps, and project structure may change.

LibreGram Desktop is an independent project based on the Telegram Desktop codebase and the Kotatogram fork. The goal is to build on the familiar desktop experience while developing LibreGram-specific improvements.

LibreGram is **not affiliated with Telegram** or Telegram FZ-LLC.

LibreGram adds native settings, the Graphite theme, and a [WS Proxy](docs/libregram/ws-proxy.md) switch with optional auto-start. The [profile manager](docs/MULTI_ACCOUNT.md) opens independent workspaces with their own data.

## What the current codebase brings

The inherited Kotatogram feature set includes:

- Local chat folders
- Forwarding to multiple chats
- Compact chat list
- Custom fonts and text replacements
- Adjustable sticker size
- Adaptive chat bubbles

These describe capabilities from the upstream base. They are not a promise that every feature has been redesigned or independently maintained by LibreGram.

## Build from source

Build instructions currently focus on Linux and Windows:

| Platform | Guide |
|:--|:--|
| Linux | [Build with Docker](docs/building-linux.md) |
| Windows | [Build with Visual Studio](docs/building-win.md) |

You need your own Telegram API credentials (**api_id** and **api_hash**) to build the application. See [how to obtain them](docs/api_credentials.md).

**Keep credentials private.** Do not commit API values, local secrets, or credential-filled configuration files.

## Скачать

[LibreGram Desktop 0.1.0 beta 1 · GitHub Releases](https://github.com/W0rd3r566/libregram-desktop/releases/tag/v0.1.0-beta.1)

| Дистрибутив или ОС | Пакет для x86-64 |
| --- | --- |
| Arch Linux, CachyOS | [Пакет для pacman](https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.1/libregram-0.1.0beta1-1-x86_64.pkg.tar.zst) |
| Ubuntu 24.04 и 26.04, Debian 12 и 13 | [Пакет .deb](https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.1/libregram-desktop_0.1.0.beta.1_amd64.deb) |
| NixOS с flakes | [Установка с flakes](docs/libregram/install.md#nixos-с-flakes) |
| NixOS без flakes | [Установка без flakes](docs/libregram/install.md#nixos-без-flakes) |
| Другие Linux с glibc | [Переносной архив .tar.xz](https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.1/LibreGram-0.1.0-beta.1-linux-x86_64.tar.xz) |
| Windows x64 | Установщик `.exe` будет в [Releases](https://github.com/W0rd3r566/libregram-desktop/releases/tag/v0.1.0-beta.1) после успешной сборки. |
| macOS | Пакета этой беты пока нет. |

## Project structure

- `Telegram/` — application source and build system
- `docs/` — build notes and project documentation
- `.github/workflows/` — automation and package workflows

## Contributing

Issues and focused pull requests are welcome. Before proposing a change, check the existing source and documentation so work stays aligned with the upstream codebase and LibreGram's current direction.

## Credits

LibreGram builds on the work of the [Telegram Desktop](https://github.com/telegramdesktop/tdesktop) and [Kotatogram](https://github.com/kotatogram/kotatogram-desktop) projects. Please see [LICENSE](LICENSE) and the upstream project notices for license and attribution details.

---

<div align="center">
  <sub>LibreGram Desktop · Built in the open by W0rd3r</sub>
</div>
