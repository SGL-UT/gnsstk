#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestGPSCNavAlm(unittest.TestCase):
    def test_constructor(self):
        uut = gpstk.GPSCNavAlm()
        self.assertEqual(True, uut.healthL1)
        self.assertEqual(True, uut.healthL2)
        self.assertEqual(True, uut.healthL5)
        self.assertEqual(0, uut.deltai)
        self.assertEqual(0, uut.wna)
        self.assertEqual(0, uut.toa)

    def test_validate(self):
        uut = gpstk.GPSCNavAlm()
        self.assertEqual(True, uut.validate())

    def test_getUserTime(self):
        uut = gpstk.GPSCNavAlm()
        expL2 = gpstk.GPSWeekSecond(2100,147.0).toCommonTime()
        expL5 = gpstk.GPSWeekSecond(2100,141.0).toCommonTime()
        uut.timeStamp = gpstk.GPSWeekSecond(2100,135.0).toCommonTime()
        uut.signal = gpstk.NavMessageID(
            gpstk.NavSatelliteID(1, 1, gpstk.SatelliteSystem.GPS,
                                 gpstk.CarrierBand.L5, gpstk.TrackingCode.L5I,
                                 gpstk.NavType.GPSCNAVL5),
            gpstk.NavMessageType.Almanac)
        self.assertEqual(expL5, uut.getUserTime())
        uut.signal = gpstk.NavMessageID(
            gpstk.NavSatelliteID(1, 1, gpstk.SatelliteSystem.GPS,
                                 gpstk.CarrierBand.L2, gpstk.TrackingCode.L2CM,
                                 gpstk.NavType.GPSCNAVL2),
            gpstk.NavMessageType.Almanac)
        self.assertEqual(expL2, uut.getUserTime())

    def test_fixFit(self):
        uut = gpstk.GPSCNavAlm()
        toa = gpstk.GPSWeekSecond(2100,135.0).toCommonTime()
        xmit = gpstk.GPSWeekSecond(2099,604000.0).toCommonTime()
        expBegin = xmit
        expEnd   = gpstk.CommonTime.END_OF_TIME
        uut.Toe = toa
        uut.xmitTime = xmit
        uut.fixFit()
        self.assertEqual(expBegin, uut.beginFit)
        self.assertEqual(expEnd, uut.endFit)

if __name__ == '__main__':
    run_unit_tests()
