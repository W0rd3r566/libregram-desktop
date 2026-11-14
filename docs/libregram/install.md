# Install LibreGram Desktop

Download assets from [GitHub Releases](https://github.com/W0rd3r566/libregram-desktop/releases). Packages are uploaded after their platform checks pass.

| Platform | Package |
| --- | --- |
| Arch Linux, CachyOS | `.pkg.tar.zst` for x86-64 |
| Ubuntu 24.04 and 26.04, Debian 12 and 13 | `.deb` for amd64 |
| Other glibc Linux distributions | Portable `.tar.xz` for x86-64. The host provides graphics, GLib, Cairo, Pango, font and audio libraries. |
| Windows ARM64, x64 and x86 | Portable ZIP and installer EXE, uploaded after the architecture passes its checks |
| NixOS | Nix derivation wrapping the Linux archive |

## NixOS с flakes

Установите пакет из ветки `main`. Flake использует версию nixpkgs из `flake.lock`.

```sh
nix profile install github:W0rd3r566/libregram-desktop/main#libregram-desktop
```

## NixOS без flakes

Если у вас настроен канал `nixpkgs`, выполните:

```sh
git clone --depth 1 --branch main https://github.com/W0rd3r566/libregram-desktop.git
cd libregram-desktop
nix-env -if ./default.nix
```

Можно также скачать архив рецепта NixOS из Releases. Распакуйте его и из этой папки выполните `nix profile install .#libregram-desktop` для варианта с flakes или `nix-env -if ./default.nix` для варианта без flakes.

Оба варианта скачивают один и тот же архив из Releases и проверяют его SHA-256. Версия без flakes использует ваш `<nixpkgs>`.
