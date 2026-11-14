#!/usr/bin/env bash
set -euo pipefail
repo=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)
binary=${1:?Path to LibreGram}
helper=${2:?Path to libregram-ws-proxy}
out=${3:?Destination directory}
version=${4:?Package version}
[[ "$version" =~ ^[0-9][0-9A-Za-z.+~-]*$ ]]
mkdir -p "$out"
out=$(cd "$out" && pwd)
stage=$(mktemp -d)
trap 'rm -rf -- "$stage"' EXIT

app_id=io.github.w0rd3r566.libregram
name="LibreGram-${version//\~/\-}-linux-x86_64"
bundle="$stage/$name"
install -Dm755 "$binary" "$bundle/LibreGram"
install -Dm755 "$helper" "$bundle/libregram-ws-proxy"
install -m644 "$repo/"{LICENSE,LEGAL} "$bundle/"
install -m644 "$repo/Telegram/build/ws-proxy/LICENSE" "$bundle/tg-ws-proxy-rs.LICENSE"
install -m644 "$repo/Telegram/Resources/art/icon256.png" "$bundle/$app_id.png"
install -m644 "$repo/lib/xdg/$app_id.desktop" "$bundle/$app_id.desktop"
tar -C "$stage" -cJf "$out/$name.tar.xz" "$name"

deb="$stage/deb"
install -Dm755 "$bundle/LibreGram" "$deb/usr/lib/libregram/LibreGram"
install -Dm755 "$bundle/libregram-ws-proxy" "$deb/usr/lib/libregram/libregram-ws-proxy"
install -d "$deb/usr/bin" "$deb/DEBIAN" "$deb/usr/share/doc/libregram-desktop"
ln -s ../lib/libregram/LibreGram "$deb/usr/bin/LibreGram"
install -Dm644 "$bundle/$app_id.png" "$deb/usr/share/icons/hicolor/256x256/apps/$app_id.png"
install -Dm644 "$bundle/$app_id.desktop" "$deb/usr/share/applications/$app_id.desktop"
sed -i 's/^DBusActivatable=true$/DBusActivatable=false/' "$deb/usr/share/applications/$app_id.desktop"
install -m644 "$bundle/"{LICENSE,LEGAL,tg-ws-proxy-rs.LICENSE} "$deb/usr/share/doc/libregram-desktop/"
cat > "$deb/DEBIAN/control" <<CONTROL
Package: libregram-desktop
Version: $version
Section: net
Priority: optional
Architecture: amd64
Maintainer: LibreGram contributors <LibreGram@users.noreply.github.com>
Depends: libc6 (>= 2.34), libgcc-s1, libcairo2, libfontconfig1, libfreetype6, libglib2.0-0t64 | libglib2.0-0, libpango-1.0-0, libpangocairo-1.0-0, libpangoft2-1.0-0, libgl1, libegl1, libpulse0, libasound2t64 | libasound2, libdbus-1-3, libxcb1, libx11-6, libx11-xcb1
Recommends: fonts-dejavu-core
Homepage: https://github.com/W0rd3r566/libregram-desktop
Description: LibreGram Desktop messenger
 An independent Telegram client with local profiles, a Graphite theme
 and a built-in launcher for the WS Proxy helper.
CONTROL
dpkg-deb --root-owner-group --build "$deb" "$out/libregram-desktop_${version}_amd64.deb"
