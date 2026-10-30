<div align="center">

# LibreGram Desktop

**Telegram Desktop, with room to make it yours.**

A community-driven desktop client built on the Kotatogram codebase, focused on deep customization, privacy controls and a polished desktop experience.

[![Base](https://img.shields.io/badge/base-Kotatogram-7965d8?style=flat-square)](https://github.com/kotatogram/kotatogram-desktop)
[![Status](https://img.shields.io/badge/status-early%20development-orange?style=flat-square)](#project-status)
[![License](https://img.shields.io/badge/license-GPL--3.0-blue?style=flat-square)](LICENSE)

</div>

> [!WARNING]
> **LibreGram is in early development.** It is not yet a ready-to-use replacement for Telegram Desktop. Planned features below are goals, not implemented functionality.

## ✨ Vision

- **LibreTheme** — a theme system for colors, typography, spacing, backgrounds and interface elements. A CSS-inspired format is planned, translated into supported Qt styling properties rather than treating the client as a browser.
- **Local enhancements** — client-side conveniences and appearance features. “Local Premium” means local-only enhancements, not Telegram Premium status or server-side benefits.
- **Multi-account workspace** — a fast account switcher designed for a large collection of profiles. The long-term target is up to 500 saved accounts, not 500 simultaneous active connections.
- **LibreProxy** — optional integration with [tg-ws-proxy](https://github.com/Flowseal/tg-ws-proxy), initially as a separately managed helper process.
- **Privacy and quality-of-life tools** — carefully selected features inspired by desktop and mobile Telegram clients.

## Project status

| Area | Status |
|---|---|
| Kotatogram codebase | Base selected |
| LibreGram development branch | Created |
| Rebranding and documentation | In progress |
| LibreTheme | Planned |
| Multi-account workspace | Planned |
| LibreProxy integration | Planned |
| Local enhancements | Planned |

The project prioritizes a stable, buildable client before adding large feature sets.

## Development

The current upstream base is [Kotatogram Desktop](https://github.com/kotatogram/kotatogram-desktop), an experimental Telegram Desktop fork. Telegram X is an Android project, so its interaction ideas may inspire desktop UX, but its Android code is not directly merged into this Qt application.

- [Linux build instructions](docs/building-linux.md)
- [Project architecture and development principles](docs/architecture.md)
- [Kotatogram upstream repository](https://github.com/kotatogram/kotatogram-desktop)

LibreGram-specific changes are developed on the `libregram/dev` branch. The upstream project remains the reference for inherited build requirements and behavior.

## Principles

1. **Local means local.** Client-side enhancements must not claim to grant Telegram server-side privileges.
2. **Security before convenience.** Handle account session data carefully; never commit credentials, API secrets or session files.
3. **Small, reviewable changes.** Each feature should be isolated, testable and documented.
4. **Respect upstream.** Preserve required notices and comply with upstream licenses and Telegram API terms.
5. **No fake promises.** Planned features remain clearly labeled until implemented and tested.

## License and attribution

LibreGram is based on Kotatogram Desktop and Telegram Desktop. This repository is distributed under the **GNU General Public License v3.0**; see [LICENSE](LICENSE) and upstream notices. Contributions and distributions must preserve applicable copyright and license notices and comply with GPL-3.0 requirements.

LibreGram is an independent community project and is not affiliated with, endorsed by, or an official product of Telegram.
