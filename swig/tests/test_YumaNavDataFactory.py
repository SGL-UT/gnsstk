#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

ws = gnsstk.GPSWeekSecond(377, 319488.0, gnsstk.TimeSystem.GPS)
toa = ws.toCommonTime()
ti = toa - 70*3600
tf = toa + 74*3600

class TestYumaNavDataFactory(unittest.TestCase):
    def test_addDataSource(self):
        ndf = gnsstk.YumaNavDataFactory()
        self.assertTrue(ndf.addDataSource(args.input_dir+'/test_input_yuma377.txt'))
        self.assertEqual(60, ndf.size())
        ndf.clear()
        self.assertEqual(0, ndf.size())

    def test_times(self):
        eph = gnsstk.NavLibrary()
        ndf = gnsstk.YumaNavDataFactory()
        eph.addFactory(ndf)
        self.assertTrue(ndf.addDataSource(args.input_dir+'/test_input_yuma377.txt'))

        self.assertEqual(str(ti), str(eph.getInitialTime()))
        self.assertEqual(str(tf), str(eph.getFinalTime()))

    def test_isPresent(self):
        eph = gnsstk.NavLibrary()
        ndf = gnsstk.YumaNavDataFactory()
        eph.addFactory(ndf)
        self.assertTrue(ndf.addDataSource(args.input_dir+'/test_input_yuma377.txt'))

        sat_id = gnsstk.SatID(1, gnsstk.SatelliteSystem.GPS)
        nsid = gnsstk.NavSatelliteID(sat_id)
        nmid = gnsstk.NavMessageID(nsid, gnsstk.NavMessageType.Almanac)
        nmid.obs.mcodeMask = 0
        self.assertTrue(eph.isPresent(nmid, toa, toa+1))


    def test_getxvt(self):
        eph = gnsstk.NavLibrary()
        ndf = gnsstk.YumaNavDataFactory()
        eph.addFactory(ndf)
        print(f"{args.input_dir}")
        self.assertTrue(ndf.addDataSource(args.input_dir+'/test_input_yuma377.txt'))

        expected = "x:(1.54042e+07, 2.40059e+06, -2.13103e+07), v:(763.726, 2637.13, 834.708), clk bias:8.96454e-05, clk drift:3.63798e-12, relcorr:-6.83963e-09"

        sat_id = gnsstk.SatID(1, gnsstk.SatelliteSystem.GPS)
        nsid = gnsstk.NavSatelliteID(sat_id)
        nmid = gnsstk.NavMessageID(nsid, gnsstk.NavMessageType.Almanac)
        nmid.obs.mcodeMask = 0
        valid = gnsstk.NavValidityType.Any
        order = gnsstk.NavSearchOrder.Nearest
        sv_health = gnsstk.SVHealth.Any
        xvt = gnsstk.Xvt()
        self.assertTrue(eph.getXvt(nmid, toa, xvt, sv_health, valid, order))
        self.assertEqual(expected, str(xvt))


    def test_epoch(self):
        gnsstk.YumaData.nearFullWeek.fset(ws.week + 1024)
        toa = gnsstk.GPSWeekSecond(gnsstk.YumaData.nearFullWeek.fget(), ws.sow, gnsstk.TimeSystem.GPS).toCommonTime()
        ti = toa - 70*3600

        eph = gnsstk.NavLibrary()
        ndf = gnsstk.YumaNavDataFactory()
        eph.addFactory(ndf)
        self.assertTrue(ndf.addDataSource(args.input_dir+'/test_input_yuma377.txt'))

        print(f"ti:{gnsstk.CivilTime(ti)}, gti:{gnsstk.CivilTime(eph.getInitialTime())}")
        self.assertEqual(str(ti), str(eph.getInitialTime()))
        gnsstk.YumaData.nearFullWeek.fset(0)


if __name__ == '__main__':
    run_unit_tests()
