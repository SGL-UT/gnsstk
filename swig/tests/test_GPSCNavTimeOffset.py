#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestGPSCNavTimeOffset(unittest.TestCase):
    def test_constructor(self):
        uut = gpstk.GPSCNavTimeOffset()
        ct = gpstk.CommonTime()
        self.assertEqual(gpstk.TimeSystem.Unknown, uut.tgt)
        self.assertEqual(0.0, uut.a0)
        self.assertEqual(0.0, uut.a1)
        self.assertEqual(0.0, uut.a2)
        self.assertEqual(0.0, uut.deltatLS)
        self.assertEqual(ct, uut.refTime)
        self.assertEqual(0.0, uut.tot)
        self.assertEqual(0, uut.wnot)
        self.assertEqual(0, uut.wnLSF)
        self.assertEqual(0, uut.dn)
        self.assertEqual(0.0, uut.deltatLSF)

    def test_validate(self):
        uut = gpstk.GPSCNavTimeOffset()
        self.assertEqual(True, uut.validate())
        uut.tot = 604784.0
        self.assertEqual(True, uut.validate())
        uut.dn = 7
        self.assertEqual(True, uut.validate())
        uut.dn = 8
        self.assertEqual(False, uut.validate())
        uut.dn = 7
        uut.tot = -0.001
        self.assertEqual(False, uut.validate())
        uut.tot = 604784.1
        self.assertEqual(False, uut.validate())

    def test_getUserTime(self):
        uut = gpstk.GPSCNavTimeOffset()
        expL2 = gpstk.GPSWeekSecond(2100,147.0).toCommonTime()
        expL5 = gpstk.GPSWeekSecond(2100,141.0).toCommonTime()
        uut.timeStamp = gpstk.GPSWeekSecond(2100,135.0).toCommonTime()
        uut.signal = gpstk.NavMessageID(
            gpstk.NavSatelliteID(1, 1, gpstk.SatelliteSystem.GPS,
                                 gpstk.CarrierBand.L5, gpstk.TrackingCode.L5I,
                                 gpstk.NavType.GPSCNAVL5),
            gpstk.NavMessageType.TimeOffset)
        self.assertEqual(expL5, uut.getUserTime())
        uut.signal = gpstk.NavMessageID(
            gpstk.NavSatelliteID(1, 1, gpstk.SatelliteSystem.GPS,
                                 gpstk.CarrierBand.L2, gpstk.TrackingCode.L2CM,
                                 gpstk.NavType.GPSCNAVL2),
            gpstk.NavMessageType.TimeOffset)
        self.assertEqual(expL2, uut.getUserTime())

    def test_getOffset(self):
        uut = gpstk.GPSCNavTimeOffset()
        ws1 = gpstk.GPSWeekSecond(2060, 405504.0).toCommonTime()
        ws2 = gpstk.GPSWeekSecond(2061, 405504.0).toCommonTime()
        uut.tgt = gpstk.TimeSystem.UTC
        uut.a0 = 1.9790604711E-09
        uut.a1 = 7.5495165675E-15
        uut.a2 = 0
        uut.deltatLS = 18.0
        uut.tot = 21600.0
        uut.wnot = 2060
        uut.refTime = gpstk.GPSWeekSecond(uut.wnot,uut.tot).toCommonTime()
        rv,offset = uut.getOffset(gpstk.TimeSystem.GPS,gpstk.TimeSystem.UTC,ws1)
        self.assertEqual(True, rv)
        self.assertEqual(18.000000004877350079, offset)
        rv,offset = uut.getOffset(gpstk.TimeSystem.GPS,gpstk.TimeSystem.UTC,ws2)
        self.assertEqual(True, rv)
        self.assertEqual(18.000000009443297699, offset)

if __name__ == '__main__':
    run_unit_tests()
