<div align="center">

<img src="Telegram/Resources/art/libregram.png" alt="Логотип LibreGram" width="128">

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

## Скачать

[**Packages — все сборки для скачивания**](https://github.com/W0rd3r566/libregram-desktop/releases/tag/v0.1.0-beta.2)

| Дистрибутив или ОС | Скачать |
| --- | --- |
| Arch Linux, CachyOS · x86-64 | [Пакет для pacman](https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/libregram-0.1.0beta2-1-x86_64.pkg.tar.zst) |
| Ubuntu 24.04 и 26.04, Debian 12 и 13 · amd64 | [Пакет .deb](https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/libregram-desktop_0.1.0.beta.2_amd64.deb) |
| NixOS x86-64 с flakes · beta 1 | [Архив .tar.gz](https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/LibreGram-NixOS-x86_64-flakes.tar.gz) · [Установка](docs/libregram/install.md#nixos-с-flakes) |
| NixOS x86-64 без flakes · beta 1 | [Архив .tar.gz](https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/LibreGram-NixOS-x86_64-no-flakes.tar.gz) · [Установка](docs/libregram/install.md#nixos-без-flakes) |
| Другие Linux с glibc · x86-64 | [TAR.GZ](https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/LibreGram-0.1.0-beta.2-linux-x86_64.tar.gz) · [TAR.XZ](https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/LibreGram-0.1.0-beta.2-linux-x86_64.tar.xz) |
| Windows x64 | [Установщик EXE](https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/LibreGram-Windows-x86_64.exe) · [Portable ZIP](https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/LibreGram-Windows-x86_64-portable.zip) |
| Windows x86 (32 бит) | [Установщик EXE](https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/LibreGram-Windows-x86.exe) · [Portable ZIP](https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/LibreGram-Windows-x86-portable.zip) |

Windows, DEB, Arch и переносной Linux — **beta 2**. Для NixOS пока доступны рецепты **beta 1**. NixOS beta 2 и Windows ARM64 добавятся после исправления проверок запуска и завершения.

### Команды скачивания

Команды сохраняют файл в текущей папке.

**Arch Linux и CachyOS**

```bash
curl -fLO 'https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/libregram-0.1.0beta2-1-x86_64.pkg.tar.zst'
```

**Ubuntu 24.04 и 26.04, Debian 12 и 13**

```bash
curl -fLO 'https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/libregram-desktop_0.1.0.beta.2_amd64.deb'
```

**NixOS с flakes**

```bash
curl -fLO 'https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/LibreGram-NixOS-x86_64-flakes.tar.gz'
```

**NixOS без flakes**

```bash
curl -fLO 'https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/LibreGram-NixOS-x86_64-no-flakes.tar.gz'
```

**Другие Linux с glibc**

```bash
curl -fLO 'https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/LibreGram-0.1.0-beta.2-linux-x86_64.tar.gz'
```

**Windows x64**

```powershell
Invoke-WebRequest 'https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/LibreGram-Windows-x86_64.exe' -OutFile '.\LibreGram-Windows-x86_64.exe'
```

**Windows x86 (32 бит)**

```powershell
Invoke-WebRequest 'https://github.com/W0rd3r566/libregram-desktop/releases/download/v0.1.0-beta.2/LibreGram-Windows-x86.exe' -OutFile '.\LibreGram-Windows-x86.exe'
```

## Быстрый старт

Выполняйте команды установки из папки, куда скачали пакет.

**Windows**

Запустите скачанный установщик EXE. Для portable-версии распакуйте ZIP и откройте `LibreGram.exe` из этой папки.

**Arch Linux и CachyOS**

```bash
sudo pacman -U ./libregram-0.1.0beta2-1-x86_64.pkg.tar.zst
```

**Ubuntu и Debian**

```bash
sudo apt install ./libregram-desktop_0.1.0.beta.2_amd64.deb
```

После установки откройте LibreGram из меню приложений.

**NixOS**

Выберите инструкцию [с flakes](docs/libregram/install.md#nixos-с-flakes) или [без flakes](docs/libregram/install.md#nixos-без-flakes). После установки запустите `LibreGram`.

**Переносной архив Linux**

Распакуйте архив и запустите `./LibreGram` из распакованной папки. Оставьте `libregram-ws-proxy` рядом с клиентом.

### Как включить WS Proxy

1. Откройте главное меню и включите **WS Proxy**. До входа в аккаунт его можно включить из списка прокси на экране входа.
2. Для автозапуска откройте **Настройки → LibreGram Settings → WS Proxy** и включите **Start WS Proxy when LibreGram opens**. [Подробнее о WS Proxy](docs/libregram/ws-proxy.md)

![Настройки WS Proxy в реальном окне LibreGram Desktop](docs/libregram/screenshots/ws-proxy.png)

## Обратная связь

Сообщайте об ошибках в [GitHub Issues](https://github.com/W0rd3r566/libregram-desktop/issues). Укажите версию LibreGram, ОС и архитектуру, а также шаги, после которых возникает ошибка.

Новости проекта и релизов публикуются в [Telegram-канале LibreGram](https://t.me/libregramm).

## What the current codebase brings

The inherited Kotatogram feature set includes:

- Local chat folders
- Forwarding to multiple chats
- Compact chat list
- Custom fonts and text replacements
- Adjustable chat bubbles

These describe capabilities from the upstream base. They are not a promise that every feature has been redesigned or independently maintained by LibreGram.

## Build from source

Build instructions currently focus on Linux and Windows:

| Platform | Guide |
|:--|:--|
| Linux | [Build with Docker](docs/building-linux.md) |
| Windows | [Build with Visual Studio](docs/building-win.md) |

You need your own Telegram API credentials (**api_id** and **api_hash**) to build the application. See [how to obtain them](docs/api_credentials.md).

**Keep credentials private.** Do not commit API values, local secrets, or credential-filled configuration files.

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
