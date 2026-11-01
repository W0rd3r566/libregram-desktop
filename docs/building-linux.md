# Build LibreGram Desktop on Linux

This guide builds the **development branch** of this repository inside the included Docker build environment.

## Requirements

- Git
- Docker
- [Poetry](https://python-poetry.org/)
- Your own Telegram API credentials: `api_id` and `api_hash`

See [API credentials](api_credentials.md) for the official instructions. Keep these values private and never commit them.

## 1. Clone LibreGram

From the directory where you keep your projects, run:

```bash
git clone --recursive https://github.com/W0rd3r566/libregram-desktop.git
cd libregram-desktop
```

The repository's default branch is `dev`.

## 2. Prepare the Docker build environment

From the repository root:

```bash
./Telegram/build/prepare/linux.sh
```

The script installs the Python build dependencies with Poetry and builds the Docker image named `tdesktop:centos_env`.

## 3. Build a release version

Run this from the **LibreGram repository root**. Replace the placeholders with your own API credentials:

```bash
docker run --rm -it \
  -u "$(id -u)" \
  -v "$PWD:/usr/src/tdesktop" \
  tdesktop:centos_env \
  /usr/src/tdesktop/Telegram/build/docker/centos_env/build.sh \
  -D TDESKTOP_API_ID=YOUR_API_ID \
  -D TDESKTOP_API_HASH=YOUR_API_HASH
```

## 4. Build a debug version

Use the same command with `CONFIG=Debug` added to the container environment:

```bash
docker run --rm -it \
  -u "$(id -u)" \
  -e CONFIG=Debug \
  -v "$PWD:/usr/src/tdesktop" \
  tdesktop:centos_env \
  /usr/src/tdesktop/Telegram/build/docker/centos_env/build.sh \
  -D TDESKTOP_API_ID=YOUR_API_ID \
  -D TDESKTOP_API_HASH=YOUR_API_HASH
```

Build output is written to the repository's `out` directory.

## API credentials and secrets

- Obtain your own credentials using the [official Telegram instructions](https://core.telegram.org/api/obtaining_api_id).
- Do not paste real values into public issues, pull requests, screenshots, or committed files.
- If you use an IDE configuration file for local credentials, make sure it stays untracked.

## Notes

The build scripts and Docker environment are inherited from the Telegram Desktop/Kotatogram build system. LibreGram-specific packaging and build steps may evolve; please report outdated instructions through the repository issue tracker.
