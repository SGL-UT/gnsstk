#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

class TestGPSCNav2Eph(unittest.TestCase):
    def test_constructor(self):
        uut = gnsstk.GPSCNav2Eph()
        ct = gnsstk.CommonTime()
        self.assertEqual(True, uut.healthL1C)
        self.assertEqual(-16, uut.uraED)
        self.assertEqual(-16, uut.uraNED0)
        self.assertEqual(0, uut.uraNED1)
        self.assertEqual(0, uut.uraNED2)
        self.assertEqual(False, uut.integStat)
        self.assertEqual(0.0, uut.deltaA)
        self.assertEqual(0.0, uut.dOMEGAdot)
        self.assertEqual(ct, uut.top)
        self.assertEqual(0.0, uut.tgd)
        self.assertEqual(0.0, uut.iscL1CP)
        self.assertEqual(0.0, uut.iscL1CD)

    def test_validate(self):
        uut = gnsstk.GPSCNav2Eph()
        self.assertEqual(True, uut.validate())

    def test_getUserTime(self):
        uut = gnsstk.GPSCNav2Eph()
        uut.timeStamp = gnsstk.GPSWeekSecond(2100,135.0).toCommonTime()
        uut.xmitTime = gnsstk.GPSWeekSecond(2100,139.0).toCommonTime()
        exp = gnsstk.GPSWeekSecond(2100,147.0).toCommonTime()
        uut.signal = gnsstk.NavMessageID(
            gnsstk.NavSatelliteID(1, 1, gnsstk.SatelliteSystem.GPS,
                                 gnsstk.CarrierBand.L1, gnsstk.TrackingCode.L1CD,
                                 gnsstk.NavType.GPSCNAV2),
            gnsstk.NavMessageType.Ephemeris)
        self.assertEqual(exp, uut.getUserTime())

    def test_fixFit(self):
        uut = gnsstk.GPSCNav2Eph()
        beginExp2 = gnsstk.GPSWeekSecond(2059, 597600).toCommonTime()
        endExp2 = gnsstk.GPSWeekSecond(2060, 3600).toCommonTime()
        uut.Toe = gnsstk.GPSWeekSecond(2059, 603000).toCommonTime()
        uut.xmitTime = gnsstk.GPSWeekSecond(2059,597600).toCommonTime()
        uut.fixFit()
        self.assertEqual(beginExp2, uut.beginFit)
        self.assertEqual(endExp2, uut.endFit)

if __name__ == '__main__':
    run_unit_tests()
