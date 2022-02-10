#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

class TestGPSCNav2Health(unittest.TestCase):
    def test_constructor(self):
        uut = gnsstk.GPSCNav2Health()
        self.assertEqual(True, uut.health)

    def test_getUserTime(self):
        uut = gnsstk.GPSCNav2Health()
        uut.timeStamp = gnsstk.GPSWeekSecond(2100,135.0).toCommonTime()
        expEph = uut.timeStamp + 12.0
        expAlm = uut.timeStamp + 5.48
        uut.setEph(True)
        self.assertEqual(expEph, uut.getUserTime())
        uut.setEph(False)
        self.assertEqual(expAlm, uut.getUserTime())

    def test_getHealth(self):
        uut = gnsstk.GPSCNav2Health()
        self.assertEqual(gnsstk.SVHealth.Unhealthy, uut.getHealth())
        uut.health = False
        self.assertEqual(gnsstk.SVHealth.Healthy, uut.getHealth())

if __name__ == '__main__':
    run_unit_tests()
