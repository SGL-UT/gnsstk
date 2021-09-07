#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

class TestGPSLNavHealth(unittest.TestCase):
    def test_constructor(self):
        uut = gnsstk.GPSLNavHealth()
        self.assertEqual(0x80, uut.svHealth)

    def test_getUserTime(self):
        uut = gnsstk.GPSLNavHealth()
        uut.timeStamp = gnsstk.GPSWeekSecond(2100,135.0).toCommonTime()
        exp = gnsstk.GPSWeekSecond(2100,141.0).toCommonTime()
        self.assertEqual(exp, uut.getUserTime())

    def test_getHealth(self):
        uut = gnsstk.GPSLNavHealth()
        self.assertEqual(gnsstk.SVHealth.Unhealthy, uut.getHealth())
        uut.svHealth = 0
        self.assertEqual(gnsstk.SVHealth.Healthy, uut.getHealth())
        uut.svHealth = 1
        self.assertEqual(gnsstk.SVHealth.Unhealthy, uut.getHealth())

if __name__ == '__main__':
    run_unit_tests()
