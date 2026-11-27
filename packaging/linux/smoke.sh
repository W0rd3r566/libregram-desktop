#!/usr/bin/env bash
set -euo pipefail
binary=$(realpath "${1:?LibreGram executable required}")
helper=$(realpath "${2:?WS Proxy helper required}")
"$helper" --help >/dev/null
profile=$(mktemp -d)
trap 'rm -rf -- "$profile"' EXIT
export XDG_RUNTIME_DIR="$profile/runtime"
export XDG_CONFIG_HOME="$profile/config"
export XDG_DATA_HOME="$profile/data"
export XDG_CACHE_HOME="$profile/cache"
install -d -m700 "$XDG_RUNTIME_DIR"
export QT_QPA_PLATFORM=xcb LIBGL_ALWAYS_SOFTWARE=1
unset DISPLAY XAUTHORITY WAYLAND_DISPLAY
xvfb-run -a bash -euc '
  app=$1
  root=$2
  for phase in early normal; do
    profile="$root/$phase"
    mkdir -p "$profile"
    "$app" -workdir "$profile" -noupdate >"$profile/stdout.log" 2>&1 &
    pid=$!
    trap '\''kill "$pid" 2>/dev/null || true'\'' EXIT
    pattern="LibreGram"
    if [[ "$phase" == normal ]]; then
      pattern="LibreGram.*[1-9][0-9]{2,}x[1-9][0-9]{2,}[+]"
    fi
    ready=false
    for attempt in {1..600}; do
      if ! kill -0 "$pid" 2>/dev/null; then
        cat "$profile/stdout.log" >&2
        exit 1
      fi
      if xwininfo -root -tree | grep -E "$pattern" >/dev/null; then
        ready=true
        break
      fi
      sleep 0.05
    done
    if [[ "$ready" != true ]]; then
      cat "$profile/stdout.log" >&2
      exit 1
    fi
    timeout 15 "$app" -workdir "$profile" -noupdate -quit
    if ! timeout 15 tail --pid="$pid" -f /dev/null; then
      cat "$profile/stdout.log" >&2
      exit 1
    fi
    if ! wait "$pid"; then
      cat "$profile/stdout.log" >&2
      exit 1
    fi
    trap - EXIT
    printf "%s startup and quit passed\n" "$phase"
  done
' bash "$binary" "$profile"
