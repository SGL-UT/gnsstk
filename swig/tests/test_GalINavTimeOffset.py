#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

class TestGalINavTimeOffset(unittest.TestCase):
    def test_constructor(self):
        uut = gnsstk.GalINavTimeOffset()
        ct = gnsstk.CommonTime()
        self.assertEqual(gnsstk.TimeSystem.Unknown, uut.tgt)
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
        uut = gnsstk.GalINavTimeOffset()
        self.assertEqual(True, uut.validate())
        uut.tot = 604784.0
        self.assertEqual(True, uut.validate())
        uut.dn = 7
        self.assertEqual(True, uut.validate())
        uut.dn = 8
        self.assertEqual(True, uut.validate())
        uut.dn = 7
        uut.tot = -0.001
        self.assertEqual(True, uut.validate())
        uut.tot = 604784.1
        self.assertEqual(True, uut.validate())

    def test_getOffset(self):
        uut = gnsstk.GalINavTimeOffset()
        ws1 = gnsstk.GALWeekSecond(2060, 405504.0).toCommonTime()
        ws2 = gnsstk.GALWeekSecond(2061, 405504.0).toCommonTime()
        uut.tgt = gnsstk.TimeSystem.UTC
        uut.a0 = 1.9790604711E-09
        uut.a1 = 7.5495165675E-15
        uut.a2 = 0
        uut.deltatLS = 18.0
        uut.tot = 21600.0
        uut.wnot = 2060
        uut.refTime = gnsstk.GALWeekSecond(uut.wnot,uut.tot).toCommonTime()
        rv,offset = uut.getOffset(gnsstk.TimeSystem.GAL,gnsstk.TimeSystem.UTC,ws1)
        self.assertEqual(True, rv)
        self.assertEqual(18.000000004877350079, offset)
        rv,offset = uut.getOffset(gnsstk.TimeSystem.GAL,gnsstk.TimeSystem.UTC,ws2)
        self.assertEqual(True, rv)
        self.assertEqual(18.000000009443297699, offset)

if __name__ == '__main__':
    run_unit_tests()
