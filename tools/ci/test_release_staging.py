#!/usr/bin/env python3
"""Exercise release staging without uploading or publishing real packages."""

import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
REVISION = "a" * 40
OLD_REVISION = "b" * 40
PACKAGES = [
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
    "libregram-desktop-v0.1.0-beta.2-full.tar.gz",
]
FAKE_GH = r'''#!/usr/bin/env python3
import json, os, sys
from pathlib import Path
path = Path(os.environ["TEST_RELEASE_STATE"])
state = json.loads(path.read_text())
args = sys.argv[1:]
release = state.get("release")
if args[0] == "api":
    print(state["head"] if "/git/ref/" in args[1] else release["targetCommitish"])
    sys.exit(0)
if args[:2] == ["release", "view"]:
    if release is None:
        sys.exit(1)
    print(json.dumps(release))
    sys.exit(0)
state["actions"].append(args)
if args[:2] == ["release", "create"]:
    state["release"] = dict(isDraft=True, targetCommitish=args[args.index("--target") + 1], assets=[])
elif args[:2] == ["release", "edit"]:
    if "--target" in args:
        release["targetCommitish"] = args[args.index("--target") + 1]
    if "--draft=false" in args:
        release["isDraft"] = False
elif args[:2] == ["release", "upload"]:
    for value in args[3:args.index("--repo")]:
        file = Path(value.split("#", 1)[0])
        assert file.is_file(), file
        if file.name.startswith("BUILD-REVISION-"):
            assert file.read_text().strip() == os.environ["GITHUB_SHA"]
        if not any(asset["name"] == file.name for asset in release["assets"]):
            release["assets"].append(dict(name=file.name))
else:
    raise AssertionError(args)
path.write_text(json.dumps(state))
'''


def receipt(platform, revision=REVISION):
    return f"BUILD-REVISION-{revision}-{platform}.txt"


class ReleaseStagingTests(unittest.TestCase):
    def run_staging(self, *, assets=(), target=REVISION, draft=True, head=REVISION, exists=True):
        with tempfile.TemporaryDirectory() as directory:
            directory = Path(directory)
            gh = directory / "gh"
            gh.write_text(FAKE_GH)
            gh.chmod(0o755)
            payload = directory / "LibreGram-Windows-x86_64.exe"
            payload.write_bytes(b"test artifact")
            state = directory / "state.json"
            state.write_text(json.dumps({
                "head": head,
                "actions": [],
                "release": dict(isDraft=draft, targetCommitish=target,
                                assets=[dict(name=name) for name in assets]) if exists else None,
            }))
            env = dict(os.environ, PATH=f"{directory}:{os.environ['PATH']}",
                       TEST_RELEASE_STATE=str(state), GITHUB_REPOSITORY="example/libregram",
                       GITHUB_SHA=REVISION, LIBREGRAM_PACKAGE_PLATFORM="windows-x86_64")
            result = subprocess.run(["bash", str(ROOT / "packaging/linux/publish-beta.sh"), str(payload)],
                                    env=env, text=True, capture_output=True)
            return result, json.loads(state.read_text())

    def test_creates_a_draft_for_the_current_revision(self):
        result, state = self.run_staging(exists=False)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertTrue(state["release"]["isDraft"])
        self.assertEqual(state["release"]["targetCommitish"], REVISION)
        self.assertIn(receipt("windows-x86_64"), [a["name"] for a in state["release"]["assets"]])

    def test_old_successful_builds_cannot_publish_a_new_revision(self):
        assets = PACKAGES + [receipt(p, OLD_REVISION) for p in ("linux", "arch", "windows-x86_64")]
        result, state = self.run_staging(assets=assets, target=OLD_REVISION)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(state["release"]["targetCommitish"], REVISION)
        self.assertTrue(state["release"]["isDraft"])

    def test_publishes_after_every_required_platform_passes(self):
        result, state = self.run_staging(assets=PACKAGES + [receipt("linux"), receipt("arch")])
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertFalse(state["release"]["isDraft"])

    def test_a_missing_package_keeps_the_release_in_draft(self):
        result, state = self.run_staging(assets=PACKAGES[2:] + [receipt("linux"), receipt("arch")])
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertTrue(state["release"]["isDraft"])

    def test_published_releases_cannot_be_retargeted(self):
        result, state = self.run_staging(target=OLD_REVISION, draft=False)
        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(state["actions"], [])
        self.assertEqual(state["release"]["targetCommitish"], OLD_REVISION)

    def test_superseded_jobs_do_not_upload_anything(self):
        result, state = self.run_staging(head=OLD_REVISION)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(state["actions"], [])


if __name__ == "__main__":
    unittest.main()
