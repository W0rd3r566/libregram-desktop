#!/usr/bin/env bash
set -euo pipefail
binary=$(realpath "${1:?LibreGram executable required}")
helper=$(realpath "${2:?WS Proxy helper required}")
"$helper" --help >/dev/null
profile=$(mktemp -d)
trap 'rm -rf -- "$profile"' EXIT
export XDG_RUNTIME_DIR="$profile/runtime"
install -d -m700 "$XDG_RUNTIME_DIR"
export QT_QPA_PLATFORM=xcb LIBGL_ALWAYS_SOFTWARE=1
export HOME="$profile"
xvfb-run -a bash -euc '
  app=$1
  profile=$2
  "$app" -workdir "$profile" -noupdate >"$profile/stdout.log" 2>&1 &
  pid=$!
  trap '\''kill "$pid" 2>/dev/null || true'\'' EXIT
  for attempt in {1..60}; do
    if ! kill -0 "$pid" 2>/dev/null; then
      cat "$profile/stdout.log" >&2
      exit 1
    fi
    if xwininfo -root -tree | grep -i LibreGram >/dev/null; then
      "$app" -workdir "$profile" -noupdate -quit
      wait "$pid"
      trap - EXIT
      exit 0
    fi
    sleep 0.5
  done
  cat "$profile/stdout.log" >&2
  exit 1
' bash "$binary" "$profile"
