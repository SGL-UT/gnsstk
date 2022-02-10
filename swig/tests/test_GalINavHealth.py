#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

class TestGalINavHealth(unittest.TestCase):
    def test_constructor(self):
        uut = gnsstk.GalINavHealth()
        self.assertEqual(gnsstk.GalHealthStatus.Unknown, uut.sigHealthStatus)
        self.assertEqual(gnsstk.GalDataValid.Unknown, uut.dataValidityStatus)
        self.assertEqual(255, uut.sisaIndex)

    def test_validate(self):
        uut = gnsstk.GalINavHealth()
        self.assertEqual(True, uut.validate())

    def test_getUserTime(self):
        uut = gnsstk.GalINavHealth()
        uut.timeStamp = gnsstk.GALWeekSecond(2100,135.0).toCommonTime()
        exp = gnsstk.GALWeekSecond(2100,137.0).toCommonTime()
        self.assertEqual(exp, uut.getUserTime())

    def test_getHealth(self):
        uut = gnsstk.GalINavHealth()
        self.assertEqual(gnsstk.SVHealth.Unknown, uut.getHealth())
        uut.sigHealthStatus = gnsstk.GalHealthStatus.OutOfService
        self.assertEqual(gnsstk.SVHealth.Unhealthy, uut.getHealth())
        uut.sigHealthStatus = gnsstk.GalHealthStatus.Unknown
        self.assertEqual(gnsstk.SVHealth.Unknown, uut.getHealth())
        uut.sigHealthStatus = gnsstk.GalHealthStatus.InTest
        self.assertEqual(gnsstk.SVHealth.Unhealthy, uut.getHealth())
        uut.sigHealthStatus = gnsstk.GalHealthStatus.WillBeOOS
        self.assertEqual(gnsstk.SVHealth.Degraded, uut.getHealth())
        uut.sigHealthStatus = gnsstk.GalHealthStatus.OK
        uut.dataValidityStatus = gnsstk.GalDataValid.NoGuarantee
        self.assertEqual(gnsstk.SVHealth.Degraded, uut.getHealth())
        uut.dataValidityStatus = gnsstk.GalDataValid.Valid
        uut.sisaIndex = 255
        self.assertEqual(gnsstk.SVHealth.Degraded, uut.getHealth())
        uut.sisaIndex = 254
        self.assertEqual(gnsstk.SVHealth.Healthy, uut.getHealth())
        uut.sisaIndex = 0
        self.assertEqual(gnsstk.SVHealth.Healthy, uut.getHealth())
        uut.dataValidityStatus = gnsstk.GalDataValid.Unknown
        self.assertEqual(gnsstk.SVHealth.Unknown, uut.getHealth())

if __name__ == '__main__':
    run_unit_tests()
