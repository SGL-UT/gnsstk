#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestGPSLNavEph(unittest.TestCase):
    def test_constructor(self):
        uut = gpstk.GPSLNavEph()
        self.assertEqual(0, uut.pre2)
        self.assertEqual(0, uut.pre3)
        self.assertEqual(0, uut.tlm2)
        self.assertEqual(0, uut.tlm3)
        self.assertEqual(0, uut.iodc)
        self.assertEqual(0, uut.iode)
        self.assertEqual( 0, uut.fitIntFlag)
        self.assertEqual(0xff, uut.healthBits)
        self.assertEqual(0, uut.uraIndex)
        self.assertEqual(0.0, uut.tgd)
        self.assertEqual(False, uut.asFlag2)
        self.assertEqual(False, uut.asFlag3)
        self.assertEqual(False, uut.alert2)
        self.assertEqual(False, uut.alert3)
        self.assertEqual(False, uut.L2Pdata)

    def test_validate(self):
        uut = gpstk.GPSLNavEph()
        self.assertEqual(True, uut.validate())
        self.assertEqual(True, uut.validate())
        uut.pre = 0x22c
        self.assertEqual(False, uut.validate())
        uut.pre = 0x8b
        uut.pre2 = 0x22c
        self.assertEqual(False, uut.validate())
        uut.pre2 = 0x8b
        uut.pre3 = 0x22c
        self.assertEqual(False, uut.validate())
        uut.pre3 = 0x8b
        self.assertEqual(True, uut.validate())

    def test_getUserTime(self):
        uut = gpstk.GPSLNavEph()
        uut.timeStamp = gpstk.GPSWeekSecond(2100,135.0).toCommonTime()
        exp = gpstk.GPSWeekSecond(2100,153.0).toCommonTime()
        self.assertEqual(exp, uut.getUserTime())

    def test_fixFit(self):
        uut = gpstk.GPSLNavEph()
        beginExp2 = gpstk.GPSWeekSecond(2121, 603360).toCommonTime()
        endExp2 = gpstk.GPSWeekSecond(2122, 7200).toCommonTime()
        uut.Toe = gpstk.GPSWeekSecond(2122, 0).toCommonTime()
        uut.xmitTime = gpstk.GPSWeekSecond(2121,603360).toCommonTime()
        uut.fixFit()
        self.assertEqual(beginExp2, uut.beginFit)
        self.assertEqual(endExp2, uut.endFit)

if __name__ == '__main__':
    run_unit_tests()
