#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestGPSCNavEph(unittest.TestCase):
    def test_constructor(self):
        uut = gpstk.GPSCNavEph()
        self.assertEqual(0, uut.pre11)
        self.assertEqual(0, uut.preClk)
        self.assertEqual(True, uut.healthL1)
        self.assertEqual(True, uut.healthL2)
        self.assertEqual(True, uut.healthL5)
        self.assertEqual(-16, uut.uraED)
        self.assertEqual(-16, uut.uraNED0)
        self.assertEqual(0, uut.uraNED1)
        self.assertEqual(0, uut.uraNED2)
        self.assertEqual(False, uut.alert11)
        self.assertEqual(False, uut.alertClk)
        self.assertEqual(False, uut.integStat)
        self.assertEqual(False, uut.phasingL2C)
        self.assertEqual(0, uut.deltaA)
        self.assertEqual(0, uut.dOMEGAdot)
        self.assertEqual(gpstk.CommonTime.BEGINNING_OF_TIME, uut.top)
        self.assertEqual(gpstk.CommonTime.BEGINNING_OF_TIME, uut.xmit11)
        self.assertEqual(gpstk.CommonTime.BEGINNING_OF_TIME, uut.xmitClk)

    def test_validate(self):
        uut = gpstk.GPSCNavEph()
        self.assertEqual(True, uut.validate())
        uut.pre = 0x22c
        self.assertEqual(False, uut.validate())
        uut.pre = 0x8b
        uut.pre11 = 0x22c
        self.assertEqual(False, uut.validate())
        uut.pre11 = 0x8b
        uut.preClk = 0x22c
        self.assertEqual(False, uut.validate())
        uut.preClk = 0x8b
        self.assertEqual(True, uut.validate())

    def test_getUserTime(self):
        uut = gpstk.GPSCNavEph()
        uut.timeStamp = gpstk.GPSWeekSecond(2100,135.0).toCommonTime()
        uut.xmitTime = gpstk.GPSWeekSecond(2100,139.0).toCommonTime()
        uut.xmit11 = gpstk.GPSWeekSecond(2100,200.0).toCommonTime()
        uut.xmitClk = gpstk.GPSWeekSecond(2100,1.0).toCommonTime()
        expL2 = gpstk.GPSWeekSecond(2100,212.0).toCommonTime()
        expL5 = gpstk.GPSWeekSecond(2100,206.0).toCommonTime()
        uut.signal = gpstk.NavMessageID(
            gpstk.NavSatelliteID(1, 1, gpstk.SatelliteSystem.GPS,
                                 gpstk.CarrierBand.L5, gpstk.TrackingCode.L5I,
                                 gpstk.NavType.GPSCNAVL5),
            gpstk.NavMessageType.Ephemeris)
        self.assertEqual(expL5, uut.getUserTime())
        uut.signal = gpstk.NavMessageID(
            gpstk.NavSatelliteID(1, 1, gpstk.SatelliteSystem.GPS,
                                 gpstk.CarrierBand.L2, gpstk.TrackingCode.L2CM,
                                 gpstk.NavType.GPSCNAVL2),
            gpstk.NavMessageType.Ephemeris)
        self.assertEqual(expL2, uut.getUserTime())

    def test_fixFit(self):
        uut = gpstk.GPSCNavEph()
        beginExp2 = gpstk.GPSWeekSecond(2059, 597600).toCommonTime()
        endExp2 = gpstk.GPSWeekSecond(2060, 3600).toCommonTime()
        uut.Toe = gpstk.GPSWeekSecond(2059, 603000).toCommonTime()
        uut.xmitTime = gpstk.GPSWeekSecond(2059,597600).toCommonTime()
        uut.xmit11 = gpstk.GPSWeekSecond(2059,597612).toCommonTime()
        uut.xmitClk = gpstk.GPSWeekSecond(2059,597624).toCommonTime()
        uut.fixFit()
        self.assertEqual(beginExp2, uut.beginFit)
        self.assertEqual(endExp2, uut.endFit)

if __name__ == '__main__':
    run_unit_tests()
