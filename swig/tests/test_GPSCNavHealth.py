#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestGPSCNavHealth(unittest.TestCase):
    def test_constructor(self):
        uut = gpstk.GPSCNavHealth()
        self.assertEqual(True, uut.health)

    def test_getUserTime(self):
        uut = gpstk.GPSCNavHealth()
        expL2 = gpstk.GPSWeekSecond(2100,147.0).toCommonTime()
        expL5 = gpstk.GPSWeekSecond(2100,141.0).toCommonTime()
        uut.timeStamp = gpstk.GPSWeekSecond(2100,135.0).toCommonTime()
        uut.signal = gpstk.NavMessageID(
            gpstk.NavSatelliteID(1, 1, gpstk.SatelliteSystem.GPS,
                                 gpstk.CarrierBand.L5, gpstk.TrackingCode.L5I,
                                 gpstk.NavType.GPSCNAVL5),
            gpstk.NavMessageType.Health)
        self.assertEqual(expL5, uut.getUserTime())
        uut.signal = gpstk.NavMessageID(
            gpstk.NavSatelliteID(1, 1, gpstk.SatelliteSystem.GPS,
                                 gpstk.CarrierBand.L2, gpstk.TrackingCode.L2CM,
                                 gpstk.NavType.GPSCNAVL2),
            gpstk.NavMessageType.Health)
        self.assertEqual(expL2, uut.getUserTime())

    def test_getHealth(self):
        uut = gpstk.GPSCNavHealth()
        self.assertEqual(gpstk.SVHealth.Unhealthy, uut.getHealth())
        uut.health = False
        self.assertEqual(gpstk.SVHealth.Healthy, uut.getHealth())

if __name__ == '__main__':
    run_unit_tests()
