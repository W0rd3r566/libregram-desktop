/*
This file is part of Kotatogram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/kotatogram/kotatogram-desktop/blob/dev/LEGAL
*/
#pragma once

// Keep the inherited numeric marker so existing local data migrations remain compatible.
constexpr auto AppKotatoVersion = 1004009;

#ifdef LIBREGRAM_GIT_VERSION
constexpr auto AppKotatoVersionStr = LIBREGRAM_GIT_VERSION;
#else
constexpr auto AppKotatoVersionStr = "0.1.0-dev";
#endif

// LibreGram is not an upstream Kotatogram beta release.
constexpr auto AppKotatoBetaVersion = false;

//#define KTGDESKTOP_IS_TEST_VERSION
constexpr auto AppKotatoTestBranch = "dev";
constexpr auto AppKotatoTestVersion = 0;

#ifdef KTGDESKTOP_IS_TEST_VERSION
constexpr auto AppKotatoTestVersionFull = (1000ULL * AppKotatoVersion + AppKotatoTestVersion);
#else // KTGDESKTOP_IS_TEST_VERSION
constexpr auto AppKotatoTestVersionFull = (0ULL);
#endif // KTGDESKTOP_IS_TEST_VERSION
