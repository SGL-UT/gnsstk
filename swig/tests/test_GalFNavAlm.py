#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

class TestGalFNavAlm(unittest.TestCase):
    def test_constructor(self):
        uut = gnsstk.GalFNavAlm()
        ct = gnsstk.CommonTime()
        self.assertEqual(ct, uut.xmit2)
        self.assertEqual(0.0, uut.dAhalf)
        self.assertEqual(0.0, uut.deltai)
        self.assertEqual(0, uut.wna)
        self.assertEqual(0.0, uut.t0a)
        self.assertEqual(0, uut.ioda5)
        self.assertEqual(0, uut.ioda6)
        self.assertEqual(gnsstk.GalHealthStatus.Unknown, uut.hsE5a)

    def test_validate(self):
        uut = gnsstk.GalFNavAlm()
        self.assertEqual(True, uut.validate())

    def test_getUserTime(self):
        uut = gnsstk.GalFNavAlm()
        uut.xmitTime = gnsstk.GALWeekSecond(2100,141.0).toCommonTime()
        uut.xmit2 = gnsstk.GALWeekSecond(2100,135.0).toCommonTime()
        exp = uut.xmitTime + 10.0
        self.assertEqual(exp, uut.getUserTime())

    def test_fixFit(self):
        uut = gnsstk.GalFNavAlm()
        toa = gnsstk.GALWeekSecond(2100,135.0).toCommonTime()
        xmit = gnsstk.GALWeekSecond(2099,604000.0).toCommonTime()
        expBegin = xmit
        expEnd = toa + (74.0 * 3600.0)
        uut.Toe = toa
        uut.xmitTime = xmit
        uut.fixFit()
        self.assertEqual(expBegin, uut.beginFit)
        self.assertEqual(expEnd, uut.endFit)

    def test_fixHealth(self):
        uut = gnsstk.GalFNavAlm()
        uut.hsE5a = gnsstk.GalHealthStatus.OK
        uut.fixHealth()
        self.assertEqual(gnsstk.SVHealth.Healthy, uut.health)
        uut.hsE5a = gnsstk.GalHealthStatus.OutOfService
        uut.fixHealth()
        self.assertEqual(gnsstk.SVHealth.Unhealthy, uut.health)
        uut.hsE5a = gnsstk.GalHealthStatus.WillBeOOS
        uut.fixHealth()
        self.assertEqual(gnsstk.SVHealth.Degraded, uut.health)
        uut.hsE5a = gnsstk.GalHealthStatus.InTest
        uut.fixHealth()
        self.assertEqual(gnsstk.SVHealth.Unhealthy, uut.health)

if __name__ == '__main__':
    run_unit_tests()
