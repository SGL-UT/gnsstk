#!/usr/env python

import unittest, sys, os

sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import run_unit_tests
import gnsstk

class TestGPSNavConfig(unittest.TestCase):
    def test_constructor(self):
        uut = gnsstk.GPSNavConfig()
        self.assertFalse(uut.antispoofOn)
        self.assertEqual(0, uut.svConfig)

    def test_getUserTime(self):
        uut = gnsstk.GPSNavConfig()
        uut.timeStamp = gnsstk.GPSWeekSecond(2100,135.0).toCommonTime()
        exp = gnsstk.GPSWeekSecond(2100,141.0).toCommonTime()
        self.assertEqual(exp, uut.getUserTime())

    def test_getSVConfig(self):
        uut = gnsstk.GPSNavConfig()
        uut.svConfig = 0
        self.assertEqual(gnsstk.NewNav.GPSSVConfig.noInfo, uut.getSVConfig())
        uut.svConfig = 1
        self.assertEqual(gnsstk.NewNav.GPSSVConfig.blockIIR, uut.getSVConfig())
        uut.svConfig = 2
        self.assertEqual(gnsstk.NewNav.GPSSVConfig.blockIIRM, uut.getSVConfig())
        uut.svConfig = 3
        self.assertEqual(gnsstk.NewNav.GPSSVConfig.blockIIF, uut.getSVConfig())
        uut.svConfig = 4
        self.assertEqual(gnsstk.NewNav.GPSSVConfig.blockIII, uut.getSVConfig())
        uut.svConfig = 5
        self.assertEqual(gnsstk.NewNav.GPSSVConfig.blockIIIF, uut.getSVConfig())
        uut.svConfig = 6
        self.assertEqual(gnsstk.NewNav.GPSSVConfig.noInfo, uut.getSVConfig())

if __name__ == '__main__':
    run_unit_tests()
