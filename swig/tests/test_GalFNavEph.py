#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

class TestGalFNavEph(unittest.TestCase):
    def test_constructor(self):
        uut = gnsstk.GalFNavEph()
        ct = gnsstk.CommonTime()
        self.assertEqual(0.0, uut.bgdE5aE1)
        self.assertEqual(255, uut.sisaIndex)
        self.assertEqual(0, uut.svid)
        self.assertEqual(ct, uut.xmit2)
        self.assertEqual(ct, uut.xmit3)
        self.assertEqual(ct, uut.xmit4)
        self.assertEqual(0, uut.iodnav1)
        self.assertEqual(0, uut.iodnav2)
        self.assertEqual(0, uut.iodnav3)
        self.assertEqual(0, uut.iodnav4)
        self.assertEqual(gnsstk.GalHealthStatus.Unknown, uut.hsE5a)
        self.assertEqual(gnsstk.GalDataValid.Unknown, uut.dvsE5a)
        self.assertEqual(0, uut.wn1);
        self.assertEqual(0, uut.tow1);
        self.assertEqual(0, uut.wn2);
        self.assertEqual(0, uut.tow2);
        self.assertEqual(0, uut.wn3);
        self.assertEqual(0, uut.tow3);
        self.assertEqual(0, uut.tow4);

    def test_validate(self):
        uut = gnsstk.GalFNavEph()
        self.assertEqual(True, uut.validate())

    def test_getUserTime(self):
        uut = gnsstk.GalFNavEph()
        uut.timeStamp = gnsstk.GALWeekSecond(2100,135.0).toCommonTime()
        uut.xmitTime = gnsstk.GALWeekSecond(2100,139.0).toCommonTime()
        uut.xmit2 = gnsstk.GALWeekSecond(2100,200.0).toCommonTime()
        uut.xmit3 = gnsstk.GALWeekSecond(2100,1.0).toCommonTime()
        uut.xmit4 = gnsstk.GALWeekSecond(2100,7.0).toCommonTime()
        exp = gnsstk.GALWeekSecond(2100,210.0).toCommonTime()
        self.assertEqual(exp, uut.getUserTime())

    def test_fixFit(self):
        uut = gnsstk.GalFNavEph()
        beginExp2 = gnsstk.GALWeekSecond(2059, 597600).toCommonTime()
        endExp2 = gnsstk.GALWeekSecond(2060, 12600).toCommonTime()
        uut.Toe = gnsstk.GALWeekSecond(2059, 603000).toCommonTime()
        uut.xmitTime = gnsstk.GALWeekSecond(2059,597600).toCommonTime()
        uut.fixFit()
        self.assertEqual(beginExp2, uut.beginFit)
        self.assertEqual(endExp2, uut.endFit)

if __name__ == '__main__':
    run_unit_tests()
