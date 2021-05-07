#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestGPSLNavTimeOffset(unittest.TestCase):
    def test_constructor(self):
        uut = gpstk.GPSLNavTimeOffset()
        ct = gpstk.CommonTime()
        self.assertEqual(gpstk.TimeSystem.GPS, uut.src)
        self.assertEqual(gpstk.TimeSystem.UTC, uut.tgt)
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
        uut = gpstk.GPSLNavTimeOffset()
        self.assertEqual(True, uut.validate())
        uut.tot = 602112.0
        self.assertEqual(True, uut.validate())
        uut.dn = 7
        self.assertEqual(True, uut.validate())
        uut.dn = 8
        self.assertEqual(False, uut.validate())
        uut.dn = 7
        uut.tot = -0.001
        self.assertEqual(False, uut.validate())
        uut.tot = 602112.1
        self.assertEqual(False, uut.validate())

    def test_getUserTime(self):
        uut = gpstk.GPSLNavTimeOffset()
        uut.timeStamp = gpstk.GPSWeekSecond(2100,135.0).toCommonTime()
        exp = gpstk.GPSWeekSecond(2100,141.0).toCommonTime()
        self.assertEqual(exp, uut.getUserTime())

    def test_getOffset(self):
        uut = gpstk.GPSLNavTimeOffset()
        ws1 = gpstk.GPSWeekSecond(2092, 405504.0).toCommonTime()
        ws2 = gpstk.GPSWeekSecond(2093, 405504.0).toCommonTime()
        uut.deltatLS = 18.0
        uut.a0 = -2.79396772E-09
        uut.a1 = -7.10542736E-15
        uut.tot = 405504.0
        uut.wnot = 2092
        uut.refTime = gpstk.GPSWeekSecond(uut.wnot,uut.tot).toCommonTime()
        rv,offset = uut.getOffset(gpstk.TimeSystem.GPS,gpstk.TimeSystem.UTC,ws1)
        self.assertEqual(True, rv)
        self.assertEqual(17.99999999720603227615, offset)
        rv,offset = uut.getOffset(gpstk.TimeSystem.GPS,gpstk.TimeSystem.UTC,ws2)
        self.assertEqual(True, rv)
        self.assertEqual(17.99999999290866981028, offset)

if __name__ == '__main__':
    run_unit_tests()
