#!/usr/bin/env python3
"""Fast, dependency-free consistency checks for LibreGram Desktop."""

from __future__ import annotations

import json
import sys
import xml.etree.ElementTree as ET
from pathlib import Path
from typing import Any


ROOT = Path(__file__).resolve().parents[2]


def fail(message: str) -> None:
    raise SystemExit(f"[FAIL] {message}")


def read(relative: str) -> str:
    path = ROOT / relative
    if not path.is_file():
        fail(f"Required file is missing: {relative}")
    return path.read_text(encoding="utf-8")


def require(condition: bool, message: str) -> None:
    if not condition:
        fail(message)


def reject_duplicate_keys(pairs: list[tuple[str, Any]]) -> dict[str, Any]:
    result: dict[str, Any] = {}
    for key, value in pairs:
        if key in result:
            raise ValueError(f"duplicate JSON key: {key}")
        result[key] = value
    return result


def validate_json_resources() -> int:
    files = sorted((ROOT / "Telegram/Resources/langs/rewrites").glob("*.json"))
    require(bool(files), "No language rewrite JSON files were found.")
    parsed: dict[str, dict[str, Any]] = {}

    for path in files:
        try:
            parsed[path.stem] = json.loads(
                path.read_text(encoding="utf-8"),
                object_pairs_hook=reject_duplicate_keys,
            )
        except (json.JSONDecodeError, ValueError) as error:
            fail(f"Invalid language resource {path.relative_to(ROOT)}: {error}")

    ru = parsed.get("ru")
    require(ru is not None, "Russian language rewrite is missing.")
    require(
        "LibreGram" in ru.get("ktg_settings_kotato", ""),
        "Russian settings section is not branded as LibreGram.",
    )
    profile_keys = {
        "ktg_profiles_menu",
        "ktg_profiles_title",
        "ktg_profiles_description",
        "ktg_profiles_empty",
        "ktg_profiles_create",
        "ktg_profiles_prompt",
    }
    for language, translations in parsed.items():
        missing = profile_keys.difference(translations)
        require(
            not missing,
            f"Language resource {language} is missing profile strings: {', '.join(sorted(missing))}",
        )
    return len(files)


def validate_application_identity() -> None:
    root_cmake = read("CMakeLists.txt")
    app_cmake = read("Telegram/CMakeLists.txt")
    settings_source = read("Telegram/SourceFiles/kotato/kotato_settings.cpp")
    profile_header = read("Telegram/SourceFiles/libregram/profiles/libregram_profiles.h")
    profile_source = read("Telegram/SourceFiles/libregram/profiles/libregram_profiles.cpp")
    settings_menu = read("Telegram/SourceFiles/kotato/kotato_settings_menu.cpp")
    profile_docs = read("docs/MULTI_ACCOUNT.md")
    desktop = read("lib/xdg/io.github.w0rd3r566.libregram.desktop")
    metainfo = read("lib/xdg/io.github.w0rd3r566.libregram.metainfo.xml")
    snap = read("snap/snapcraft.yaml")

    require(
        'DESCRIPTION "LibreGram Desktop messaging client"' in root_cmake,
        "CMake project description must identify LibreGram.",
    )
    require(
        "DESKTOP_APP_DISABLE_AUTOUPDATE ON CACHE BOOL" in root_cmake,
        "Auto-updates must remain disabled until LibreGram has its own signed update channel.",
    )
    require('set(output_name "LibreGram")' in app_cmake, "Executable name must be LibreGram.")
    require(
        'set(bundle_identifier "io.github.w0rd3r566.libregram")' in app_cmake,
        "Bundle identifier must use the LibreGram application ID.",
    )
    require(
        'tdata/libregram-settings-custom.json' in settings_source,
        "Custom settings must use the LibreGram settings path.",
    )
    require(
        'QFile::copy(legacyPath, path)' in settings_source,
        "Legacy custom settings must be migrated without deleting the original.",
    )
    require("kMaxSavedProfiles = 500" in profile_header, "The profile catalog limit must be 500.")
    require('u"-workdir"_q' in profile_source, "Profiles must launch through the existing -workdir argument.")
    require("QProcess::startDetached" in profile_source, "Profile launch must use a detached LibreGram process.")
    require("#include <QtCore/QLockFile>" in profile_source, "Profile registry writes must be protected by a lock.")
    require("EnsurePrivateDirectory" in profile_source, "Profile data directories must have owner-only permissions.")
    require("ktg_profiles_menu" in settings_menu, "The profile manager must be accessible from the existing settings menu.")
    require("Each profile" in profile_docs, "Profile workspace documentation is missing its architecture explanation.")
    require("Name=LibreGram" in desktop, "Linux desktop entry name is incorrect.")
    require("Exec=LibreGram -- %U" in desktop, "Linux desktop entry launches the wrong binary.")
    require(
        "io.github.w0rd3r566.libregram" in metainfo,
        "AppStream metadata contains the wrong application ID.",
    )
    ET.fromstring(metainfo)
    require("name: libregram-desktop" in snap, "Snap package name is incorrect.")
    require(
        "usr/bin/libregram-desktop" in snap,
        "Snap launcher does not point to the packaged LibreGram binary.",
    )
    require((ROOT / "LICENSE").is_file(), "The project license file is missing.")


def validate_ci_artifacts() -> None:
    checks = {
        ".github/workflows/linux.yml": ("out/Debug/LibreGram", "mv LibreGram artifact/"),
        ".github/workflows/mac.yml": ("mv LibreGram.app artifact/",),
        ".github/workflows/mac_packaged.yml": ("build/LibreGram.app", "mv LibreGram.app artifact/"),
        ".github/workflows/win.yml": ("LibreGram.exe",),
    }
    for relative, expected in checks.items():
        workflow = read(relative)
        for token in expected:
            require(token in workflow, f"{relative} is missing expected artifact reference: {token}")
        require(
            "Kotatogram" not in workflow,
            f"{relative} still uses the old product name in its build pipeline.",
        )


def main() -> int:
    count = validate_json_resources()
    validate_application_identity()
    validate_ci_artifacts()
    print(f"[OK] Parsed {count} language rewrite JSON files (duplicate keys rejected).")
    print("[OK] CMake, Linux desktop entry, AppStream, Snap and license metadata are consistent.")
    print("[OK] Linux, macOS and Windows CI artifact names match LibreGram.")
    print("[OK] LibreGram validation passed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
