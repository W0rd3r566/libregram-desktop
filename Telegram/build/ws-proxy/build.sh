#!/usr/bin/env bash
set -euo pipefail
here=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
mkdir -p -- "${1:?Usage: build.sh OUTPUT_DIRECTORY [SOURCE_DIRECTORY]}"
destination=$(cd -- "$1" && pwd)
revision=53f392280a370199ffe5c6ef0ac88808ed17c575
source_dir=${2:-$(mktemp -d)}
if [[ $# -lt 2 ]]; then
    trap 'rm -rf -- "$source_dir"' EXIT
    git -C "$source_dir" init -q
    git -C "$source_dir" fetch --depth=1 https://github.com/valnesfjord/tg-ws-proxy-rs.git "$revision"
    git -C "$source_dir" checkout --detach FETCH_HEAD
fi
test "$(git -C "$source_dir" rev-parse HEAD)" = "$revision"
mkdir -p "$source_dir/src/bin" "$destination"
cp "$here/libregram-ws-proxy.rs" "$source_dir/src/bin/"
target_args=()
build_dir="${CARGO_TARGET_DIR:-$source_dir/target}"
if [[ -n ${LIBREGRAM_WS_TARGET:-} ]]; then
    target_args=(--target "$LIBREGRAM_WS_TARGET")
    build_dir="$build_dir/$LIBREGRAM_WS_TARGET"
fi
case "${LIBREGRAM_WS_TARGET:-$(uname -s)}" in
    *windows*|MINGW*|MSYS*|CYGWIN*) suffix=.exe ;;
    *) suffix= ;;
esac
cargo build --manifest-path "$source_dir/Cargo.toml" --locked --release \
    --bin libregram-ws-proxy "${target_args[@]}"
install -m 755 "$build_dir/release/libregram-ws-proxy$suffix" "$destination/"
install -m 644 "$here/LICENSE" "$destination/tg-ws-proxy-rs.LICENSE"
