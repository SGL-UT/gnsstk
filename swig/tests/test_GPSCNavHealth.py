#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

class TestGPSCNavHealth(unittest.TestCase):
    def test_constructor(self):
        uut = gnsstk.GPSCNavHealth()
        self.assertEqual(True, uut.health)

    def test_getUserTime(self):
        uut = gnsstk.GPSCNavHealth()
        expL2 = gnsstk.GPSWeekSecond(2100,147.0).toCommonTime()
        expL5 = gnsstk.GPSWeekSecond(2100,141.0).toCommonTime()
        uut.timeStamp = gnsstk.GPSWeekSecond(2100,135.0).toCommonTime()
        uut.signal = gnsstk.NavMessageID(
            gnsstk.NavSatelliteID(1, 1, gnsstk.SatelliteSystem.GPS,
                                 gnsstk.CarrierBand.L5, gnsstk.TrackingCode.L5I,
                                 gnsstk.NavType.GPSCNAVL5),
            gnsstk.NavMessageType.Health)
        self.assertEqual(expL5, uut.getUserTime())
        uut.signal = gnsstk.NavMessageID(
            gnsstk.NavSatelliteID(1, 1, gnsstk.SatelliteSystem.GPS,
                                 gnsstk.CarrierBand.L2, gnsstk.TrackingCode.L2CM,
                                 gnsstk.NavType.GPSCNAVL2),
            gnsstk.NavMessageType.Health)
        self.assertEqual(expL2, uut.getUserTime())

    def test_getHealth(self):
        uut = gnsstk.GPSCNavHealth()
        self.assertEqual(gnsstk.SVHealth.Unhealthy, uut.getHealth())
        uut.health = False
        self.assertEqual(gnsstk.SVHealth.Healthy, uut.getHealth())

if __name__ == '__main__':
    run_unit_tests()
