# LibreGram for CachyOS / Arch Linux

The first packaging target is **x86_64 Arch Linux**, including CachyOS. The package uses the native CMake build and system Qt/libraries, then installs through pacman as a normal `.pkg.tar.zst` package.

> [!WARNING]
> Build LibreGram with API credentials dedicated to the LibreGram Telegram application. CMake stores them in its build cache, and the client binary contains them. Anyone who receives a prebuilt binary may be able to recover its `api_hash`. Never commit credentials or upload the build directory/cache. For a public binary release, use a dedicated LibreGram app pair, not credentials reused by another project.

## Requirements

- CachyOS or another up-to-date Arch-based x86_64 system.
- `base-devel`, Git and network access for the source submodules.
- A Telegram API application with its own `api_id` and `api_hash`.
- Enough free disk space for a full C++/Qt build.

The PKGBUILD declares the remaining build and runtime dependencies. `makepkg --syncdeps` installs missing package dependencies.

## Build

Clone the development branch, then enter the packaging directory:

```bash
git clone --branch libregram/dev https://github.com/W0rd3r566/libregram-desktop.git
cd libregram-desktop/packaging/arch
```

Install the basic packaging tools:

```bash
sudo pacman -S --needed base-devel git
```

Read the credentials without echoing the hash into the terminal, then export them for this build:

```bash
read -rp "Telegram API ID: " LIBREGRAM_API_ID
read -rsp "Telegram API hash: " LIBREGRAM_API_HASH
printf '\n'
export LIBREGRAM_API_ID LIBREGRAM_API_HASH
```

Build the package. The default parallelism is limited to two jobs to keep memory use reasonable; override it when your machine has enough RAM:

```bash
CMAKE_BUILD_PARALLEL_LEVEL=2 makepkg --syncdeps --clean --cleanbuild
```

Install the resulting package:

```bash
sudo pacman -U libregram-0.1.0-1-x86_64.pkg.tar.zst
```

Launch LibreGram from the application menu or run:

```bash
LibreGram
```

## Before calling this a release

The recipe is an initial packaging target, not a claim that a tested binary has already shipped. The first CachyOS release is ready only after a clean build succeeds, the package passes `namcap`, the application launches on both Wayland and X11, Telegram authorization works, and profile creation/archive/restore are regression-tested.

Automatic updates remain disabled. Updates will be delivered through a new signed package or a user-built package revision, not through inherited Kotatogram update infrastructure.

## Troubleshooting

- If CMake reports a missing dependency, install the named Arch package and rebuild.
- If compilation fails, preserve the first relevant error and the CMake/Ninja output. Do not replace system Qt or disable TLS checks as a workaround.
- If a previous failed build left credentials in its cache, remove the temporary build directory before rebuilding:

  ```bash
  rm -rf src/libregram-build
  ```

- Do not use another user's `tdata` or profile directories as a test fixture. Test with a disposable profile instead.
