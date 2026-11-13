#!/usr/bin/env python3
"""Validate the shipped palette's references, contrast and resource mapping."""
from pathlib import Path
import re
import unittest
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[3]
RESOURCES = ROOT / 'Telegram' / 'Resources'


class GraphiteResources(unittest.TestCase):
    def setUp(self):
        text = (RESOURCES / 'libregram-graphite.tdesktop-theme').read_text()
        self.entries = re.findall(r'^(\w+):\s*([^;]+);', text, re.M)
        self.colors = dict(self.entries)

    def resolve(self, key, seen=None):
        seen = set() if seen is None else seen
        self.assertNotIn(key, seen, f'Palette alias cycle at {key}')
        seen.add(key)
        self.assertIn(key, self.colors, f'Unknown palette alias: {key}')
        value = self.colors[key]
        if value.startswith('#'):
            self.assertRegex(value, r'^#[0-9a-fA-F]{6}([0-9a-fA-F]{2})?$')
            return value
        return self.resolve(value, seen)

    def test_palette_aliases(self):
        self.assertEqual(len(self.entries), len(self.colors), 'Duplicate keys')
        for key in self.colors:
            self.resolve(key)

    def test_primary_text_contrast(self):
        def luminance(key):
            value = self.resolve(key)
            channels = [int(value[i:i + 2], 16) / 255 for i in (1, 3, 5)]
            linear = [v / 12.92 if v <= 0.04045
                      else ((v + 0.055) / 1.055) ** 2.4 for v in channels]
            return sum(v * w for v, w in zip(linear, (0.2126, 0.7152, 0.0722)))

        for foreground, background in [
            ('windowFg', 'windowBg'),
            ('windowSubTextFg', 'windowBg'),
            ('windowFgActive', 'windowBgActive'),
            ('activeButtonFg', 'activeButtonBg'),
        ]:
            with self.subTest(foreground=foreground, background=background):
                low, high = sorted((luminance(foreground), luminance(background)))
                self.assertGreaterEqual((high + 0.05) / (low + 0.05), 4.5)

    def test_resources_are_embedded(self):
        qrc = RESOURCES / 'qrc/telegram/telegram.qrc'
        entries = ET.parse(qrc).findall("qresource[@prefix='/gui']/file")
        files = {item.get('alias'): (qrc.parent / item.text).resolve()
                 for item in entries}
        for alias in ('art/libregram.png', 'libregram-graphite.tdesktop-theme'):
            self.assertIn(alias, files)
            self.assertTrue(files[alias].is_file())
        logo = files['art/libregram.png'].read_bytes()
        self.assertTrue(logo.startswith(b'\x89PNG\r\n\x1a\n'))
        self.assertLess(files['libregram-graphite.tdesktop-theme'].stat().st_size,
                        1024 * 1024)


if __name__ == '__main__':
    unittest.main()
