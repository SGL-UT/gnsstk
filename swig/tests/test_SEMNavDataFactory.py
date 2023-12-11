#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

ws =  gnsstk.GPSWeekSecond(387, 589824, gnsstk.TimeSystem.GPS)
toa = ws.toCommonTime()
ti = toa - 70*3600
tf = toa + 74*3600

class TestSEMNavDataFactory(unittest.TestCase):
    def test_addDataSource(self):
        ndf = gnsstk.SEMNavDataFactory()
        self.assertTrue(ndf.addDataSource(args.input_dir+'/test_input_sem387.txt'))
        self.assertEqual(60, ndf.size())
        ndf.clear()
        self.assertEqual(0, ndf.size())


    def test_times(self):
        eph = gnsstk.NavLibrary()
        ndf = gnsstk.SEMNavDataFactory()
        eph.addFactory(ndf)
        self.assertTrue(ndf.addDataSource(args.input_dir+'/test_input_sem387.txt'))

        self.assertEqual(str(ti), str(eph.getInitialTime()))
        self.assertEqual(str(tf), str(eph.getFinalTime()))

    def test_isPresent(self):
        eph = gnsstk.NavLibrary()
        ndf = gnsstk.SEMNavDataFactory()
        eph.addFactory(ndf)
        self.assertTrue(ndf.addDataSource(args.input_dir+'/test_input_sem387.txt'))

        sat_id = gnsstk.SatID(1, gnsstk.SatelliteSystem.GPS)
        nsid = gnsstk.NavSatelliteID(sat_id)
        nmid = gnsstk.NavMessageID(nsid, gnsstk.NavMessageType.Almanac)
        nmid.obs.mcodeMask = 0
        self.assertTrue(eph.isPresent(nmid, toa, toa+1))

    def test_getxvt(self):
        eph = gnsstk.NavLibrary()
        ndf = gnsstk.SEMNavDataFactory()
        eph.addFactory(ndf)
        self.assertTrue(ndf.addDataSource(args.input_dir+'/test_input_sem387.txt'))

        expected = "x:(-1.41348e+07, 2.17416e+07, 5.79849e+06), v:(-608.115, 407.603, -3129.31), clk bias:0.000103951, clk drift:3.63798e-12, relcorr:1.53053e-08"

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
        gnsstk.SEMHeader.nearFullWeek.fset(ws.week + 1024)
        toa = gnsstk.GPSWeekSecond(gnsstk.SEMHeader.nearFullWeek.fget(), ws.sow, gnsstk.TimeSystem.GPS).toCommonTime()
        ti = toa - 70*3600

        eph = gnsstk.NavLibrary()
        ndf = gnsstk.SEMNavDataFactory()
        eph.addFactory(ndf)
        self.assertTrue(ndf.addDataSource(args.input_dir+'/test_input_sem387.txt'))

        print(f"ti:{gnsstk.CivilTime(ti)}, gti:{gnsstk.CivilTime(eph.getInitialTime())}")
        self.assertEqual(str(ti), str(eph.getInitialTime()))
        gnsstk.SEMHeader.nearFullWeek.fset(0)


if __name__ == '__main__':
    run_unit_tests()
