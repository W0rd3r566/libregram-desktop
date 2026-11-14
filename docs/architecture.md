# LibreGram Desktop architecture

This document records the intended direction for LibreGram. It is a design guide, not a claim that planned modules already exist.

## 1. Foundation

- LibreGram has its own product identity, UI direction and feature roadmap.
- The current implementation uses an inherited Telegram Desktop-derived C++ / Qt codebase.
- Prefer incremental changes over replacing core client infrastructure.
- Isolate LibreGram-specific work so upstream updates can be reviewed and integrated deliberately.
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

### Multi-account and profile workspaces

Do not raise the inherited `Main::Domain::kMaxAccounts` limit to 500. LibreGram stores up to 500 separate workspace profiles in a metadata registry and gives every profile its own working directory. Profiles are launched as separate LibreGram instances through the existing `-workdir` argument. The registry contains only UUIDs, names and timestamps, never auth keys or API credentials. Each instance continues using the inherited account/session model and limits. See [MULTI_ACCOUNT.md](MULTI_ACCOUNT.md).

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

- [Kotatogram Desktop source project](https://github.com/kotatogram/kotatogram-desktop) — codebase origin and upstream reference.
- [Telegram Desktop](https://github.com/telegramdesktop/tdesktop)
- [Telegram X for Android](https://github.com/TGX-Android/Telegram-X)
- [AyuGram Desktop](https://github.com/AyuGram/AyuGramDesktop)
- [tg-ws-proxy](https://github.com/Flowseal/tg-ws-proxy)


## 6. Proxy terminology

LibreGram's planned **LibreProxy** integration with Flowseal/tg-ws-proxy is distinct from the inherited Telegram Desktop `WEB` proxy implementation documented in [web-proxy-plan.md](web-proxy-plan.md) and [web-proxy-test-plan.md](web-proxy-test-plan.md).

Do not merge their settings, transports, assumptions or documentation under one name. Before implementation, audit both mechanisms and decide whether they should coexist, share any management UI, or remain entirely independent. Preserve the inherited WEB proxy implementation unless a separately reviewed change explicitly targets it.

## 7. Product specification

The feature scope, user-facing behavior, non-goals and open product questions are tracked in [PRODUCT.md](PRODUCT.md). Development milestones are tracked in [ROADMAP.md](ROADMAP.md). Those documents are the product-level source of truth; this file describes technical direction only.
