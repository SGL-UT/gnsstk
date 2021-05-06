#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestGPSLNavHealth(unittest.TestCase):
    def test_constructor(self):
        uut = gpstk.GPSLNavHealth()
        self.assertEqual(0x80, uut.svHealth)

    def test_getUserTime(self):
        uut = gpstk.GPSLNavHealth()
        uut.timeStamp = gpstk.GPSWeekSecond(2100,135.0).toCommonTime()
        exp = gpstk.GPSWeekSecond(2100,141.0).toCommonTime()
        self.assertEqual(exp, uut.getUserTime())

    def test_getHealth(self):
        uut = gpstk.GPSLNavHealth()
        self.assertEqual(gpstk.SVHealth.Unhealthy, uut.getHealth())
        uut.svHealth = 0
        self.assertEqual(gpstk.SVHealth.Healthy, uut.getHealth())
        uut.svHealth = 1
        self.assertEqual(gpstk.SVHealth.Unhealthy, uut.getHealth())

if __name__ == '__main__':
    run_unit_tests()
