# LibreGram Desktop architecture

This document records the intended direction for LibreGram. It is a design guide, not a claim that planned modules already exist.

## 1. Foundation

- Keep the Kotatogram / Telegram Desktop Qt and C++ foundation.
- Prefer incremental changes over replacing core client infrastructure.
- Keep upstream synchronization possible: isolate LibreGram-specific work and avoid unnecessary edits to inherited code.
- Treat Telegram X as a source of interaction ideas, not code that can be directly merged into this Qt desktop application.

## 2. Planned modules

### LibreTheme

A Qt-native theme and customization layer.

Planned scope:
- palette, typography, spacing and component appearance;
- chat backgrounds and media backgrounds;
- icon and animation preferences;
- import/export of theme presets;
- a CSS-inspired text format translated into supported Qt/client styling properties.

A browser CSS engine is not assumed. Unsupported selectors or arbitrary layout replacement must not be advertised.

### Multi-account workspace

A dedicated account switcher with lazy activation. The long-term design target is up to 500 saved profiles, subject to Telegram API limits, platform constraints and secure session storage. This is not a promise of 500 simultaneous connections.

### LibreProxy

An optional integration based on [Flowseal/tg-ws-proxy](https://github.com/Flowseal/tg-ws-proxy).

Initial design:
- run the proxy as a separately managed helper process;
- manage lifecycle and diagnostics from the Qt application;
- make local endpoint and status visible;
- provide explicit enable/disable and failure handling;
- preserve the upstream project's MIT license and notices if its code is reused.

The proxy is not a VPN and must not be described as a guarantee of anonymity, access or improved speed.

### Local enhancements

Client-side convenience and appearance features only. They must not impersonate Telegram Premium or claim server-side entitlements.

## 3. Engineering rules

- Keep secrets, API credentials, account sessions and personal data out of Git.
- Do not disable TLS or certificate validation as a workaround.
- Avoid silently changing proxy routes or account behavior.
- Add tests or a reproducible manual test plan for each non-trivial feature.
- Document platform-specific limitations.
- Review license compatibility before copying code or assets from another project.

## 4. Suggested implementation order

1. Verify that the inherited project builds unchanged.
2. Establish LibreGram branding without changing protocol or internal identifiers blindly.
3. Add the smallest native theme customization slice.
4. Add account-switching improvements with secure storage review.
5. Integrate LibreProxy behind an explicit opt-in setting.
6. Expand local enhancements after regression testing.

## 5. Upstream references

- [Kotatogram Desktop](https://github.com/kotatogram/kotatogram-desktop)
- [Telegram Desktop](https://github.com/telegramdesktop/tdesktop)
- [Telegram X for Android](https://github.com/TGX-Android/Telegram-X)
- [AyuGram Desktop](https://github.com/AyuGram/AyuGramDesktop)
- [tg-ws-proxy](https://github.com/Flowseal/tg-ws-proxy)
