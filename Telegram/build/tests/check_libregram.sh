#!/usr/bin/env bash
# Host checks only: this does not link or launch the Telegram target.
set -euo pipefail
repo=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../.." && pwd)
check_dir=${1:-$(mktemp -d /tmp/libregram-checks.XXXXXX)}
mkdir -p "$check_dir"
check_dir=$(cd -- "$check_dir" && pwd)
cd -- "$repo"
cxx=${CXX:-c++}
export QT_QPA_PLATFORM=offscreen
mkdir -p "$check_dir/gen"
common=(-std=c++20 -g -O0 -fPIC -I Telegram/codegen -I Telegram/lib_base -I Telegram/ThirdParty/GSL/include)
qt=($(pkg-config --cflags --libs Qt6Core Qt6Gui))
"$cxx" "${common[@]}" Telegram/codegen/codegen/common/*.cpp Telegram/codegen/codegen/lang/*.cpp "${qt[@]}" -o "$check_dir/codegen_lang"
"$cxx" "${common[@]}" Telegram/codegen/codegen/common/*.cpp Telegram/codegen/codegen/style/*.cpp Telegram/lib_base/base/crc32hash.cpp $(pkg-config --cflags --libs Qt6Core Qt6Gui Qt6Svg) -o "$check_dir/codegen_style"
"$cxx" "${common[@]}" Telegram/codegen/codegen/common/*.cpp Telegram/codegen/codegen/emoji/*.cpp "${qt[@]}" -o "$check_dir/codegen_emoji"
"$check_dir/codegen_lang" -o"$check_dir/gen" Telegram/Resources/langs/lang.strings
"$check_dir/codegen_emoji" -o"$check_dir/gen" Telegram/lib_ui/emoji.txt Telegram/lib_ui/emoji_suggestions/emoji_autocomplete.json
python3 Telegram/SourceFiles/codegen/scheme/codegen_scheme.py -o"$check_dir/gen/scheme" Telegram/SourceFiles/mtproto/scheme/api.tl Telegram/SourceFiles/mtproto/scheme/mtproto.tl
"$check_dir/codegen_style" -ITelegram/SourceFiles -ITelegram/lib_ui -ITelegram/Resources -o"$check_dir/gen/styles" -t"$check_dir/gen/styles/base" Telegram/lib_ui/ui/colors.palette
mapfile -t styles < <(rg --files Telegram/lib_ui Telegram/SourceFiles -g '*.style')
"$check_dir/codegen_style" -ITelegram/SourceFiles -ITelegram/lib_ui -ITelegram/Resources -o"$check_dir/gen/styles" -t"$check_dir/gen/styles/all" "${styles[@]}"
cp Telegram/Resources/libregram-graphite.tdesktop-theme "$check_dir/graphite.palette"
"$check_dir/codegen_style" -o"$check_dir/theme-gen" -t"$check_dir/theme-gen/check" "$check_dir/graphite.palette"
python3 Telegram/build/tests/check_libregram_resources.py
qt_libexec=$(pkg-config --variable=libexecdir Qt6Core)
"$qt_libexec/rcc" --binary Telegram/Resources/qrc/telegram/telegram.qrc -o "$check_dir/telegram.rcc"
"$cxx" -std=c++20 -g -O0 -fPIC Telegram/build/tests/check_libregram_resources.cpp "${qt[@]}" -o "$check_dir/resources"
"$check_dir/resources" "$check_dir/telegram.rcc"
includes=(-I "$check_dir/gen" -I Telegram/SourceFiles)
for module in base rpl crl ui tl storage lottie qr spellcheck webview webrtc; do
    includes+=(-I "Telegram/lib_$module")
done
includes+=(-I Telegram/ThirdParty/GSL/include -I Telegram/ThirdParty/range-v3/include -I Telegram/ThirdParty/expected/include)
"$cxx" -std=c++20 -g -O0 -fPIC -c -DDESKTOP_APP_USE_PACKAGED -DCRL_USE_QT -DQT_NO_KEYWORDS -DQT_NO_FOREACH -include Telegram/SourceFiles/stdafx.h "${includes[@]}" $(pkg-config --cflags Qt6Core Qt6Gui Qt6Widgets Qt6OpenGLWidgets Qt6Network) Telegram/SourceFiles/settings/sections/settings_libregram.cpp -o "$check_dir/settings_libregram.o"
printf 'Host checks passed. Full application build/runtime still required. Results: %s\n' "$check_dir"
