#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestGPSCNav2Alm(unittest.TestCase):
    def test_constructor(self):
        uut = gpstk.GPSCNav2Alm()
        self.assertEqual(True, uut.healthL1)
        self.assertEqual(True, uut.healthL2)
        self.assertEqual(True, uut.healthL5)
        self.assertEqual(0, uut.deltai)
        self.assertEqual(0, uut.wna)
        self.assertEqual(0, uut.toa)

    def test_validate(self):
        uut = gpstk.GPSCNav2Alm()
        self.assertEqual(True, uut.validate())

    def test_getUserTime(self):
        uut = gpstk.GPSCNav2Alm()
        uut.timeStamp = gpstk.GPSWeekSecond(2100,135.0).toCommonTime()
        exp = uut.timeStamp + 5.48
        uut.signal = gpstk.NavMessageID(
            gpstk.NavSatelliteID(1, 1, gpstk.SatelliteSystem.GPS,
                                 gpstk.CarrierBand.L1, gpstk.TrackingCode.L1CD,
                                 gpstk.NavType.GPSCNAV2),
            gpstk.NavMessageType.Almanac)
        self.assertEqual(exp, uut.getUserTime())

    def test_fixFit(self):
        uut = gpstk.GPSCNav2Alm()
        toa = gpstk.GPSWeekSecond(2100,135.0).toCommonTime()
        xmit = gpstk.GPSWeekSecond(2099,604000.0).toCommonTime()
        expBegin = xmit
        expEnd = gpstk.CommonTime.END_OF_TIME
        uut.Toe = toa
        uut.xmitTime = xmit
        uut.fixFit()
        self.assertEqual(expBegin, uut.beginFit)
        self.assertEqual(expEnd, uut.endFit)

if __name__ == '__main__':
    run_unit_tests()
