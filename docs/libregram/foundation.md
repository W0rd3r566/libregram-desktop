# LibreGram foundation

## Audit, 12 November 2026

Desktop baseline: `dev`, `f073d4ffe13f1bc9168cf2bc19df4fe1e05b9157`.
Android reference: `main`, `9feade2905b489b3be339e68663caa5aa3181a9b`.

The Android README, ROADMAP and PROJECT_STATUS distinguish goals from implementation. Searching its Java sources found no LibreGram-specific feature implementation. `gradle.properties` still uses `com.exteraless.app`; `res/values/strings.xml` still names the app exteraless. The supplied identity asset is `assets/logo.png`. Desktop now includes that file unchanged as `Resources/art/libregram.png`; original upstream icons remain available. Neither Android nor Desktop packaging identity is considered fully migrated by this iteration.

| Android capability | Evidence | Classification |
| --- | --- | --- |
| Ghost Mode controls | `tw/nekomimi/nekogram/settings/GhostModeActivity.java`, `NekoConfig.java` | Existing upstream code; not a LibreGram Status Center |
| Read/online request filtering | `com/radolyn/ayugram/utils/AyuGhostUtils.java`, `AyuState.java` | Existing implementation; runtime not tested here |
| Themes and appearance settings | `app/exteraless/settings/OpenExteraAppearanceActivity.java`, `assets/*.attheme` | Existing upstream functionality |
| Plugins | `app/exteraless/plugins/PluginsController.java`, `com/exteragram/messenger/plugins/PythonPluginsEngine.java` | Android Python/Java runtime; cannot load directly into C++ Desktop |
| Online / Idle / DND / Invisible profiles, schedules, allowlists | `docs/ROADMAP.md`, phase 2 | Planned |
| LibreGram launcher collection and package migration | `docs/PROJECT_STATUS.md`, `docs/ROADMAP.md` | Planned; logo asset is present |

The graphite palette follows the requested Desktop direction. Android has no implemented LibreGram graphite theme to copy. The provided logo is retained, not presented as a newly designed or already shipped Android identity.

## Implemented in this iteration

`Settings::LibreGramId()` is a native section reached from the main settings screen. It registers with `Settings::Builder::BuildHelper`, including search, navigation, control highlighting and language producers. The UI uses existing `SettingsButton`, subsection titles, divider descriptions, scaling and animation behavior. No new rendering framework or parallel navigation tree is introduced.

The local `AddToggle` adapter handles the shared toggle behavior: index-only construction has no widget or write, changes filter out equal values, and subscriptions end with the button lifetime. It is deliberately private until another section needs it.

| Control | Actual owner and persistence |
| --- | --- |
| Graphite preview | `Window::Theme::Apply`; existing keep/revert warning and timeout; existing theme storage |
| Compact chat list | `Kotato::JsonSettings`, existing `chat_list_lines` key and change events |
| Reduce interface animations | Existing `PowerSaving::kAnimations` flag; preserves other flags; existing local settings writer |
| Hide phone in settings | Active `Main::SessionSettings::phoneNumberHidden`; session save, account scoped |
| Customize theme/chats | Existing `Settings::ChatId()` |
| Animations/power saving | Existing `PowerSavingBox`, including system battery behavior |
| Notifications | Existing `Settings::NotificationsId()` |
| Kotatogram features | Existing `Settings::Kotato::Id()`; also retains its original main-menu entry |

App title, intro heading, main-menu brand/link, About and default runtime window icon use LibreGram. About retains the upstream credit chain and upstream community links. Existing installer IDs, executable names, profiles, update infrastructure and alternate icons are not migrated here. A distributable LibreGram release needs a separate packaging/update audit. No licenses or upstream functionality are removed.

## Boundaries for the next iterations

These are architectural decisions, not claims that the modules below are implemented. No empty plugin loader or nonfunctional status controls are shipped.

### Status Center and notification rules

A future session-owned `LibreGram::StatusController` should own a local profile, expiry and previous profile. Use a versioned session preference record with namespaced keys (`libregram.status.*`), not new fields in the middle of the sequential settings stream. Preferences with unrecognized profile values fall back to Online. Session teardown owns timers and reactive subscriptions.

A separate pure notification policy should take a profile, rule set and event context (account, peer, chat kind, mention/reply, timestamp). It returns allow/suppress and a reason; it does not send MTProto requests or mutate messages. Apply it in `Window::Notifications::System` before notification scheduling and re-evaluate queued notifications before delivery. Suppression must cover native/default banners, sound and flash, not just one visual backend. Calls and badge counters require explicit separate product decisions.

Priority: explicit temporary override > active schedule > chosen persistent profile. Expired overrides restore the previous valid profile. Schedules store a timezone and need DST and suspend/resume tests. Allowlists must include account identity as well as peer identity. Existing server mute settings remain authoritative; a local rule must not unmute a server-muted chat implicitly.

Online means ordinary client behavior. Idle is a local notification policy; DND suppresses local notifications. Neither is a Telegram presence state. Invisible requires a separately audited presence adapter at `Api::Updates::updateOnline` and related request sites; do not scatter request filters through widgets. The schema `mtproto/scheme/api.tl` defines `account.updateStatus offline:Bool`, not arbitrary Idle/DND states. Other sessions and server activity can reveal presence. Do not claim complete invisibility or quietly change read receipts, message retention or server privacy settings.

Before exposing profiles: tests must cover account isolation, restart/expiry, queued notifications, switches with pending sound, allowlist precedence, and restoration after Invisible. A profile UI without those integrations is not an implemented Status Center.

### LibreTheme

Use the existing palette/theme parser, editor, accent selection, per-chat themes and keep/revert storage. Graphite is an additive preset, not a forced replacement for user themes. Future presets belong in resources and the existing embedded theme registry. Keep semantic error/success colors and validate contrast; avoid a second JSON theme renderer. Theme import/export should use the established format and size limits.

### Account manager

`Main::Domain::accounts`, `activate` and the existing main-menu account list already own account/session lifecycle. A future manager should present and reorder those accounts, not duplicate authentication or copy session secrets. Appearance is installation-wide; phone privacy and future profiles are account-scoped. Switching accounts must detach status subscriptions and must not apply the previous account's notification rules.

### Plugins

Keep extension points behind explicit versioned capabilities (settings contributions, theme presets, notification policy inputs). Start with built-in contributions using the native settings builder. Third-party execution needs an out-of-process host, an IPC schema, permission grants, lifecycle/revocation and compatibility checks before a plugin-install UI is added. Do not load arbitrary shared libraries into the MTProto process. Extensions must not receive auth keys, raw session storage or unrestricted API access. Android's Python/Java hooks are a reference for user-facing capabilities, not a compatible desktop ABI.

## Required runtime checks before release

Build and link the complete Debug target with its declared dependency set. Open Settings → LibreGram Settings; exercise search and back navigation at 100%, 150% and 200% scale. Preview Graphite, let its timeout revert, then keep it and restart. Test a pre-existing custom theme and a separate night theme. Toggle compact layout and verify updates and persistence. Toggle reduced motion with battery saving on/off and verify other power flags are unchanged. Hide the phone on one account, switch accounts, restart and verify isolation. Check app/window/tray icons, light/dark themes, screen-reader labels and keyboard focus. These checks require the full application, not merely successful generator or object compilation.
