#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestGPSLNavAlm(unittest.TestCase):
    def test_constructor(self):
        uut = gpstk.GPSLNavAlm()
        self.assertEqual(255, uut.healthBits)
        self.assertEqual(0.0, uut.deltai)
        self.assertEqual(0.0, uut.toa)

    def test_validate(self):
        uut = gpstk.GPSLNavAlm()
        self.assertEqual(True, uut.validate())

    def test_getUserTime(self):
        uut = gpstk.GPSLNavAlm()
        exp = gpstk.GPSWeekSecond(2100,141.0).toCommonTime()
        uut.timeStamp = gpstk.GPSWeekSecond(2100,135.0).toCommonTime()
        self.assertEqual(exp, uut.getUserTime())

    def test_fixFit(self):
        uut = gpstk.GPSLNavAlm()
        toa = gpstk.GPSWeekSecond(2100,135.0).toCommonTime()
        expBegin = toa - (70.0 * 3600)
        expEnd = toa + (74.0 * 3600.0)
        uut.Toe = toa
        uut.fixFit()
        self.assertEqual(expBegin, uut.beginFit)
        self.assertEqual(expEnd, uut.endFit)

if __name__ == '__main__':
    run_unit_tests()
