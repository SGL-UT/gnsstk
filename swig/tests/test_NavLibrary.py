#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestNavLibrary(unittest.TestCase):
    def test_withRINEX(self):
        nl = gpstk.NavLibrary()
        ndf = gpstk.RinexNavDataFactory()
        nl.addFactory(ndf)
        ndf.addDataSource(args.input_dir+
                           '/test_input_rinex_nav_RinexNavExample.99n')
        self.assertEqual(5, ndf.size())
        sat = gpstk.NavSatelliteID(13, 13, gpstk.SatelliteSystem.GPS,
                                   gpstk.CarrierBand.Any,
                                   gpstk.TrackingCode.Any, gpstk.NavType.Any)
        ct = gpstk.GPSWeekSecond(1025, 410500).toCommonTime()
        xvt = gpstk.Xvt()
        rv = nl.getXvt(sat, ct, xvt, False)
        self.assertEqual(True, rv)
        self.assertEqual(-18213272.68714565, xvt.x[0])
        self.assertEqual(-19253201.93693008, xvt.x[1])
        self.assertEqual(2228475.809880065, xvt.x[2])
        self.assertEqual(65.377177313781, xvt.v[0])
        self.assertEqual(328.8751719122555, xvt.v[1])
        self.assertEqual(3436.22814085365, xvt.v[2])
        self.assertEqual(0.0004900184549112235, xvt.clkbias)
        self.assertEqual(2.04636307899e-12, xvt.clkdrift)
        self.assertEqual(-3.0273120425046433e-09, xvt.relcorr)
        rv = nl.getXvt(sat, ct, xvt)
        self.assertEqual(True, rv)
        self.assertEqual(-18213272.68714565, xvt.x[0])
        self.assertEqual(-19253201.93693008, xvt.x[1])
        self.assertEqual(2228475.809880065, xvt.x[2])
        self.assertEqual(65.377177313781, xvt.v[0])
        self.assertEqual(328.8751719122555, xvt.v[1])
        self.assertEqual(3436.22814085365, xvt.v[2])
        self.assertEqual(0.0004900184549112235, xvt.clkbias)
        self.assertEqual(2.04636307899e-12, xvt.clkdrift)
        self.assertEqual(-3.0273120425046433e-09, xvt.relcorr)
        # complains about wrong number of arguments.
        # health = gpstk.SVHealth.Any
        # rv = nl.getHealth(sat, ct, health, gpstk.SVHealth.Any,
        #                   gpstk.NavValidityType.ValidOnly,
        #                   gpstk.NavSearchOrder.User)
        # self.assertEqual(True, rv)
        # can't instantiate abstract class NavData
        print("foo")
        # nmid = gpstk.NavMessageID(sat, gpstk.NavMessageType.Health)
        # rv,nd = nl.find(nmid, ct, gpstk.SVHealth.Any,
        #                 gpstk.NavValidityType.Any, gpstk.NavSearchOrder.User)
        # print(rv)
        # print(nd)
        # self.assertEqual(True, rv)
        # rv,nd = nl.getOffset(gpstk.TimeSystem.GPS, gpstk.TimeSystem.UTC, ct)
        # print(rv)
        # print(nd)
        # self.assertEqual(True, rv)

if __name__ == '__main__':
    run_unit_tests()
