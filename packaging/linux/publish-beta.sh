#!/usr/bin/env bash
set -euo pipefail
tag=v0.1.0-beta.1
: "${GITHUB_REPOSITORY:?}" "${GITHUB_SHA:?}"
(( $# > 0 ))
head=$(gh api "repos/$GITHUB_REPOSITORY/git/ref/heads/main" --jq '.object.sha')
if [[ "$head" != "$GITHUB_SHA" ]]; then
    printf 'A newer main commit supersedes this package.\n'
    exit 0
fi
if ! gh release view "$tag" --repo "$GITHUB_REPOSITORY" >/dev/null 2>&1; then
    if gh release create "$tag" "$@" --repo "$GITHUB_REPOSITORY" \
        --target "$GITHUB_SHA" --prerelease --title "LibreGram Desktop 0.1.0 beta 1" \
        --notes-file "$(dirname "${BASH_SOURCE[0]}")/beta-notes.md"; then
        exit 0
    fi
fi
target=$(gh api "repos/$GITHUB_REPOSITORY/commits/$tag" --jq .sha)
if [[ "$target" != "$GITHUB_SHA" ]]; then
    printf 'Release %s points to another source revision.\n' "$tag" >&2
    exit 1
fi
gh release upload "$tag" "$@" --repo "$GITHUB_REPOSITORY" --clobber
