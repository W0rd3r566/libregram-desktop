#!/usr/bin/env bash
set -euo pipefail
tag=v0.1.0-beta.2
: "${GITHUB_REPOSITORY:?}" "${GITHUB_SHA:?}"
(( $# > 0 ))
head=$(gh api "repos/$GITHUB_REPOSITORY/git/ref/heads/main" --jq '.object.sha')
if [[ "$head" != "$GITHUB_SHA" ]]; then
    printf 'A newer main commit supersedes this package.\n'
    exit 0
fi

release_info() {
    gh release view "$tag" --repo "$GITHUB_REPOSITORY" --json isDraft,targetCommitish,assets
}

if ! release=$(release_info 2>/dev/null); then
    gh release create "$tag" --repo "$GITHUB_REPOSITORY" \
        --target "$GITHUB_SHA" --draft --prerelease --title "LibreGram Desktop 0.1.0 beta 2" \
        --notes-file "$(dirname "${BASH_SOURCE[0]}")/beta-notes.md" || true
    release=$(release_info)
fi
if [[ $(jq -r .isDraft <<<"$release") == true ]]; then
    target=$(jq -er .targetCommitish <<<"$release")
else
    target=$(gh api "repos/$GITHUB_REPOSITORY/commits/$tag" --jq .sha)
fi
if [[ "$target" != "$GITHUB_SHA" ]]; then
    printf 'Release %s points to another source revision.\n' "$tag" >&2
    exit 1
fi

gh release upload "$tag" "$@" --repo "$GITHUB_REPOSITORY" --clobber
release=$(release_info)
if [[ $(jq -r .isDraft <<<"$release") != true ]]; then
    exit 0
fi

if jq -e '
    [.assets[].name] as $uploaded
    | [
        "LibreGram-Windows-x86_64.exe",
        "LibreGram-Windows-x86_64-portable.zip",
        "SHA256SUMS-windows-x86_64",
        "LibreGram-0.1.0-beta.2-linux-x86_64.tar.xz",
        "libregram-desktop_0.1.0.beta.2_amd64.deb",
        "SHA256SUMS-linux",
        "libregram-0.1.0beta2-1-x86_64.pkg.tar.zst",
        "libregram-0.1.0beta2-1-x86_64.pkg.tar.zst.sha256",
        "LibreGram-NixOS-x86_64-flakes.tar.gz",
        "LibreGram-NixOS-x86_64-no-flakes.tar.gz",
        "SHA256SUMS-nix",
        "libregram-desktop-v0.1.0-beta.2-full.tar.gz"
      ] | all(. as $name | $uploaded | index($name) != null)
  ' <<<"$release" >/dev/null; then
    gh release edit "$tag" --repo "$GITHUB_REPOSITORY" --draft=false
else
    printf 'Staged %s assets. GitHub will publish after the remaining packages pass.\n' "$tag"
fi
