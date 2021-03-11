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
        # test getInitialTime/getFinalTime
        expIni = gpstk.GPSWeekSecond(1025,406800).toCommonTime()
        expFin = gpstk.GPSWeekSecond(1025,422100).toCommonTime()
        self.assertEqual(expIni, nl.getInitialTime())
        self.assertEqual(expFin, nl.getFinalTime())
        # test getXvt
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
        xvt2 = gpstk.Xvt()
        # test getXvt (second signature)
        rv = nl.getXvt(sat, ct, xvt2)
        self.assertEqual(True, rv)
        self.assertEqual(-18213272.68714565, xvt2.x[0])
        self.assertEqual(-19253201.93693008, xvt2.x[1])
        self.assertEqual(2228475.809880065, xvt2.x[2])
        self.assertEqual(65.377177313781, xvt2.v[0])
        self.assertEqual(328.8751719122555, xvt2.v[1])
        self.assertEqual(3436.22814085365, xvt2.v[2])
        self.assertEqual(0.0004900184549112235, xvt2.clkbias)
        self.assertEqual(2.04636307899e-12, xvt2.clkdrift)
        self.assertEqual(-3.0273120425046433e-09, xvt2.relcorr)
        rv,health = nl.getHealth(sat, ct, gpstk.SVHealth.Any,
                                 gpstk.NavValidityType.ValidOnly,
                                 gpstk.NavSearchOrder.User)
        self.assertEqual(True, rv)
        self.assertEqual(gpstk.SVHealth.Healthy, health)
        # test getOffset with NavDataPtr
        rv,nd = nl.getOffset(gpstk.TimeSystem.GPS, gpstk.TimeSystem.UTC, ct)
        # time range of ct is going to fail to find the time offset
        self.assertEqual(False, rv)
        ct2 = gpstk.GPSWeekSecond(1025,552970).toCommonTime()
        rv,nd = nl.getOffset(gpstk.TimeSystem.GPS, gpstk.TimeSystem.UTC, ct2)
        self.assertEqual(True, rv)
        self.assertEqual(13.0, nd.deltatLS)
        # test find
        nmid = gpstk.NavMessageID(sat, gpstk.NavMessageType.Health)
        rv,nd = nl.find(nmid, ct2, gpstk.SVHealth.Any,
                        gpstk.NavValidityType.Any, gpstk.NavSearchOrder.User)
        self.assertEqual(True, rv)
        self.assertEqual(0, nd.svHealth)
        # rudimentary test of setValidityFilter, just make sure it doesn't die
        nl.setValidityFilter(gpstk.NavValidityType.ValidOnly)
        # rudimentary test of setTypeFilter
        # nmts = gpstk.NavMessageTypeSet()
        # nmts.add(gpstk.NavMessageType.Almanac)
        # nl.setTypeFilter(nmts)

if __name__ == '__main__':
    run_unit_tests()
