<div align="center">

# LibreGram Desktop

**An experimental Telegram Desktop fork with a familiar Kotatogram-inspired experience.**

[![Arch package workflow](https://img.shields.io/github/actions/workflow/status/W0rd3r566/libregram-desktop/arch-package-build.yml?branch=dev&style=flat-square&label=Arch%20package)](https://github.com/W0rd3r566/libregram-desktop/actions/workflows/arch-package-build.yml)
[![Development branch](https://img.shields.io/badge/development-dev-20242B?style=flat-square)](https://github.com/W0rd3r566/libregram-desktop/tree/dev)
[![License](https://img.shields.io/badge/license-see%20LICENSE-20242B?style=flat-square)](LICENSE)

</div>

> **Project status:** experimental. LibreGram is under active development. Interfaces, build steps, and project structure may change.

LibreGram Desktop is an independent project based on the Telegram Desktop codebase and the Kotatogram fork. The goal is to build on the familiar desktop experience while developing LibreGram-specific improvements.

LibreGram is **not affiliated with Telegram** or Telegram FZ-LLC.

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

## Downloads

There are currently no public binary releases. Prebuilt packages will be linked here when they are ready.

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
