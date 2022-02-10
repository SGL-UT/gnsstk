#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests
import gnsstk


class TestGNSSCore(unittest.TestCase):

    def test_NMCTMeta(self):
        a = gnsstk.NMCTMeta()
        self.assertIsInstance(a,gnsstk.NMCTMeta)

    def test_EngNav(self):
        a = gnsstk.EngNav()
        self.assertIsInstance(a,gnsstk.EngNav)
        b = a.computeParity(3,5)
        self.assertTrue(b == 22)

    def test_BrcClockCorrection(self):
        a = gnsstk.BrcClockCorrection()
        self.assertIsInstance(a,gnsstk.BrcClockCorrection)
        b = a.hasData()
        self.assertTrue(not b)

    def test_BrcKeplerOrbit(self):
        a = gnsstk.BrcKeplerOrbit()
        self.assertIsInstance(a,gnsstk.BrcKeplerOrbit)
        b = a.hasData()
        self.assertTrue(not b)

    def test_NavType(self):
        a = gnsstk.NavType.GPSLNAV
        self.assertTrue(str(a) == 'GPSLNAV')

    def test_NavID(self):
        a = gnsstk.NavID(gnsstk.SatID(gnsstk.SatelliteSystem.GPS),gnsstk.ObsID(gnsstk.ObservationType.Range, gnsstk.CarrierBand.L1, gnsstk.TrackingCode.CA))
        self.assertTrue(str(a) == 'Unknown')

    def test_PackedNavBits(self):
        a = gnsstk.PackedNavBits()
        b = gnsstk.PackedNavBits.psPassed
        self.assertIsInstance(a, gnsstk.PackedNavBits)
        self.assertTrue(b == 1)

    def test_EngEphemeris(self):
        a = gnsstk.EngEphemeris()
        b = a.isValid()
        self.assertIsInstance(a, gnsstk.EngEphemeris)
        self.assertTrue(not b)

    def test_SP3SatID(self):
        a = gnsstk.SP3SatID(gnsstk.SatID(gnsstk.SatelliteSystem.GPS))
        b = a.getfill()
        self.assertIsInstance(a, gnsstk.SP3SatID)
        self.assertIsNotNone(b)

    def test_EngAlmanac(self):
        a = gnsstk.EngAlmanac()
        self.assertIsInstance(a, gnsstk.EngAlmanac)

    def EphemerisRange(self):
        a = gnsstk.CorrectedEphemerisRange()
        self.assertIsInstance(a, gnsstk.CorrectedEphemerisRange)

    def test_IonoModel(self):
        a = gnsstk.IonoModel()
        self.assertIsInstance(a, gnsstk.IonoModel)

    def test_IonoModelStore(self):
        a = gnsstk.IonoModelStore()
        a.edit(gnsstk.CommonTime.END_OF_TIME)
        self.assertIsInstance(a, gnsstk.IonoModelStore)



if __name__ == '__main__':
    run_unit_tests()
