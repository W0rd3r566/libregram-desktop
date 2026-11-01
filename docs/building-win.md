# Build LibreGram Desktop on Windows

The build is performed with **Visual Studio 2026** and the **10.0.26100.0 SDK**.

## Prepare the build folder

Choose an empty directory for the build, for example `D:\\TBuild`. It will be called `BuildPath` below.

Create these directories:

- `BuildPath\\ThirdParty`
- `BuildPath\\Libraries`

The default Visual Studio 2026 toolset (`v145`) does not support Windows 7. The inherited Telegram Desktop build configuration uses `-vcvars_ver=14.44` (`v144.4`, based on `v143`).

## Obtain API credentials

You need your own `api_id` and `api_hash` to access the Telegram API. Follow the [official instructions](https://core.telegram.org/api/obtaining_api_id).

Keep these values private. Do not commit them or share them in public logs and screenshots.

## Install required tools

- [Python 3.10](https://www.python.org/downloads/) (enable **Add Python to PATH**)
- [Git for Windows](https://git-scm.com/download/win)
- Visual Studio 2026 with the required C++ desktop development components and Windows SDK

## Initialize the developer command prompt

Open a command prompt initialized for your target architecture.

For 32-bit builds:

```bat
%comspec% /k "C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\VC\\Auxiliary\\Build\\vcvars32.bat" -vcvars_ver=14.44
```

For 64-bit builds:

```bat
%comspec% /k "C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat" -vcvars_ver=14.44
```

Use the initialized terminal for the remaining steps.

## Clone LibreGram and prepare dependencies

Go to `BuildPath` and run:

```bat
git clone --recursive https://github.com/W0rd3r566/libregram-desktop.git
cd libregram-desktop
Telegram\\build\\prepare\\win.bat
```

## Configure the build

Go to `BuildPath\\libregram-desktop\\Telegram`.

For 32-bit:

```bat
configure.bat -D TDESKTOP_API_ID=YOUR_API_ID -D TDESKTOP_API_HASH=YOUR_API_HASH
```

For 64-bit:

```bat
configure.bat x64 -D TDESKTOP_API_ID=YOUR_API_ID -D TDESKTOP_API_HASH=YOUR_API_HASH
```

Replace the placeholders with your own API credentials.

## Build

Open `BuildPath\\libregram-desktop\\out\\Telegram.slnx` in Visual Studio.

Select the **Telegram** project and build it using the Debug or Release configuration. The executable is generated in the corresponding configuration directory under `out`.

## Qt Visual Studio Tools (optional)

For additional Qt debugging support:

1. Open **Extensions → Manage Extensions** in Visual Studio.
2. Search for **Qt Visual Studio Tools**.
3. Install the extension and follow its setup instructions.

## Notes

These instructions follow the inherited Telegram Desktop build system. LibreGram-specific build details may change during development. If a step becomes outdated, please open an issue with the error message and relevant build environment details.
