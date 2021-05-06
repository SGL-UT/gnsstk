#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestGalINavAlm(unittest.TestCase):
    def test_constructor(self):
        uut = gpstk.GalINavAlm()
        ct = gpstk.CommonTime()
        self.assertEqual(ct, uut.xmit2)
        self.assertEqual(0.0, uut.dAhalf)
        self.assertEqual(0.0, uut.deltai)
        self.assertEqual(0, uut.wna)
        self.assertEqual(0.0, uut.t0a)
        self.assertEqual(0, uut.ioda1)
        self.assertEqual(0, uut.ioda2)
        self.assertEqual(gpstk.GalHealthStatus.Unknown, uut.hsE5b)
        self.assertEqual(gpstk.GalHealthStatus.Unknown, uut.hsE1B)

    def test_validate(self):
        uut = gpstk.GalINavAlm()
        self.assertEqual(True, uut.validate())

    def test_getUserTime(self):
        uut = gpstk.GalINavAlm()
        uut.xmitTime = gpstk.GALWeekSecond(2100,141.0).toCommonTime()
        uut.xmit2 = gpstk.GALWeekSecond(2100,135.0).toCommonTime()
        exp = uut.xmitTime + 2.0
        self.assertEqual(exp, uut.getUserTime())

    def test_fixFit(self):
        uut = gpstk.GalINavAlm()
        toa = gpstk.GALWeekSecond(2100,135.0).toCommonTime()
        xmit = gpstk.GALWeekSecond(2099,604000.0).toCommonTime()
        expBegin = xmit
        expEnd = toa + (74.0 * 3600.0)
        uut.Toe = toa
        uut.xmitTime = xmit
        uut.fixFit()
        self.assertEqual(expBegin, uut.beginFit)
        self.assertEqual(expEnd, uut.endFit)

    def test_fixHealth(self):
        uut = gpstk.GalINavAlm()
        uut.hsE5b = gpstk.GalHealthStatus.OK
        uut.hsE1B = gpstk.GalHealthStatus.OK
        uut.fixHealth()
        self.assertEqual(gpstk.SVHealth.Healthy, uut.health)
        uut.hsE1B = gpstk.GalHealthStatus.OutOfService
        uut.fixHealth()
        self.assertEqual(gpstk.SVHealth.Degraded, uut.health)
        uut.hsE5b = gpstk.GalHealthStatus.OutOfService
        uut.fixHealth()
        self.assertEqual(gpstk.SVHealth.Unhealthy, uut.health)

if __name__ == '__main__':
    run_unit_tests()
