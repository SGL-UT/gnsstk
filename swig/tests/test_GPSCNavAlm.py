#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

class TestGPSCNavAlm(unittest.TestCase):
    def test_constructor(self):
        uut = gnsstk.GPSCNavAlm()
        self.assertEqual(True, uut.healthL1)
        self.assertEqual(True, uut.healthL2)
        self.assertEqual(True, uut.healthL5)
        self.assertEqual(0, uut.deltai)
        self.assertEqual(0, uut.wna)
        self.assertEqual(0, uut.toa)

    def test_validate(self):
        uut = gnsstk.GPSCNavAlm()
        self.assertEqual(True, uut.validate())

    def test_getUserTime(self):
        uut = gnsstk.GPSCNavAlm()
        expL2 = gnsstk.GPSWeekSecond(2100,147.0).toCommonTime()
        expL5 = gnsstk.GPSWeekSecond(2100,141.0).toCommonTime()
        uut.timeStamp = gnsstk.GPSWeekSecond(2100,135.0).toCommonTime()
        uut.signal = gnsstk.NavMessageID(
            gnsstk.NavSatelliteID(1, 1, gnsstk.SatelliteSystem.GPS,
                                 gnsstk.CarrierBand.L5, gnsstk.TrackingCode.L5I,
                                 gnsstk.NavType.GPSCNAVL5),
            gnsstk.NavMessageType.Almanac)
        self.assertEqual(expL5, uut.getUserTime())
        uut.signal = gnsstk.NavMessageID(
            gnsstk.NavSatelliteID(1, 1, gnsstk.SatelliteSystem.GPS,
                                 gnsstk.CarrierBand.L2, gnsstk.TrackingCode.L2CM,
                                 gnsstk.NavType.GPSCNAVL2),
            gnsstk.NavMessageType.Almanac)
        self.assertEqual(expL2, uut.getUserTime())

    def test_fixFit(self):
        uut = gnsstk.GPSCNavAlm()
        toa = gnsstk.GPSWeekSecond(2100,135.0).toCommonTime()
        xmit = gnsstk.GPSWeekSecond(2099,604000.0).toCommonTime()
        expBegin = xmit
        expEnd   = gnsstk.CommonTime.END_OF_TIME
        uut.Toe = toa
        uut.xmitTime = xmit
        uut.fixFit()
        self.assertEqual(expBegin, uut.beginFit)
        self.assertEqual(expEnd, uut.endFit)

if __name__ == '__main__':
    run_unit_tests()
