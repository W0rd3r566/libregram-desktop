# Install LibreGram Desktop

Download assets from [GitHub Releases](https://github.com/W0rd3r566/libregram-desktop/releases). Packages are uploaded after their platform checks pass.

| Platform | Package |
| --- | --- |
| Arch Linux, CachyOS | `.pkg.tar.zst` for x86-64 |
| Ubuntu 24.04 and 26.04, Debian 12 and 13 | `.deb` for amd64 |
| Other glibc Linux distributions | Portable `.tar.xz` for x86-64. The host provides graphics, GLib, Cairo, Pango, font and audio libraries. |
| Windows | Portable ZIP for x86-64 |
| NixOS | Nix derivation wrapping the Linux archive |

## NixOS

Download the portable Linux archive, then clone and check out the source tag from the same release. From the source directory, run:

```sh
archive=$(realpath /path/to/LibreGram-0.1.0-beta.1-linux-x86_64.tar.xz)
nix profile install --impure --expr \
  "(builtins.getFlake (toString ./.)).lib.mkLibreGram { archive = $archive; version = \"0.1.0-beta.1\"; }"
```

This requires Nix with the `nix-command` and `flakes` features enabled. The derivation uses the pinned nixpkgs revision in `flake.lock`.
