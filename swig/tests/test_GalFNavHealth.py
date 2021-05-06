#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestGalFNavHealth(unittest.TestCase):
    def test_constructor(self):
        uut = gpstk.GalFNavHealth()
        self.assertEqual(gpstk.GalHealthStatus.Unknown, uut.sigHealthStatus)
        self.assertEqual(gpstk.GalDataValid.Unknown, uut.dataValidityStatus)
        self.assertEqual(255, uut.sisaIndex)

    def test_validate(self):
        uut = gpstk.GalFNavHealth()
        self.assertEqual(True, uut.validate())

    def test_getUserTime(self):
        uut = gpstk.GalFNavHealth()
        uut.timeStamp = gpstk.GALWeekSecond(2100,135.0).toCommonTime()
        exp = gpstk.GALWeekSecond(2100,145.0).toCommonTime()
        self.assertEqual(exp, uut.getUserTime())

    def test_getHealth(self):
        uut = gpstk.GalFNavHealth()
        self.assertEqual(gpstk.SVHealth.Unknown, uut.getHealth())
        uut.sigHealthStatus = gpstk.GalHealthStatus.OutOfService
        self.assertEqual(gpstk.SVHealth.Unhealthy, uut.getHealth())
        uut.sigHealthStatus = gpstk.GalHealthStatus.Unknown
        self.assertEqual(gpstk.SVHealth.Unknown, uut.getHealth())
        uut.sigHealthStatus = gpstk.GalHealthStatus.InTest
        self.assertEqual(gpstk.SVHealth.Unhealthy, uut.getHealth())
        uut.sigHealthStatus = gpstk.GalHealthStatus.WillBeOOS
        self.assertEqual(gpstk.SVHealth.Degraded, uut.getHealth())
        uut.sigHealthStatus = gpstk.GalHealthStatus.OK
        uut.dataValidityStatus = gpstk.GalDataValid.NoGuarantee
        self.assertEqual(gpstk.SVHealth.Degraded, uut.getHealth())
        uut.dataValidityStatus = gpstk.GalDataValid.Valid
        uut.sisaIndex = 255
        self.assertEqual(gpstk.SVHealth.Degraded, uut.getHealth())
        uut.sisaIndex = 254
        self.assertEqual(gpstk.SVHealth.Healthy, uut.getHealth())
        uut.sisaIndex = 0
        self.assertEqual(gpstk.SVHealth.Healthy, uut.getHealth())
        uut.dataValidityStatus = gpstk.GalDataValid.Unknown
        self.assertEqual(gpstk.SVHealth.Unknown, uut.getHealth())

if __name__ == '__main__':
    run_unit_tests()
