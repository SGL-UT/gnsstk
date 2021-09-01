#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests
import gpstk


class TestGNSSCore(unittest.TestCase):

    def test_OrbitEph(self):
        a = gpstk.OrbitEph()
        self.assertIsInstance(a,gpstk.OrbitEph)

    def test_BDSEphemeris(self):
        a = gpstk.BDSEphemeris()
        self.assertIsInstance(a,gpstk.BDSEphemeris)

    def test_OrbitEphStore(self):
        a = gpstk.OrbitEphStore()
        self.assertIsInstance(a,gpstk.OrbitEphStore)
        a.addEphemeris(gpstk.OrbitEph())
        self.assertIsInstance(a,gpstk.OrbitEphStore)

    def test_NMCTMeta(self):
        a = gpstk.NMCTMeta()
        self.assertIsInstance(a,gpstk.NMCTMeta)

    def test_EngNav(self):
        a = gpstk.EngNav()
        self.assertIsInstance(a,gpstk.EngNav)
        b = a.computeParity(3,5)
        self.assertTrue(b == 22)

    def test_BrcClockCorrection(self):
        a = gpstk.BrcClockCorrection()
        self.assertIsInstance(a,gpstk.BrcClockCorrection)
        b = a.hasData()
        self.assertTrue(not b)

    def test_BrcKeplerOrbit(self):
        a = gpstk.BrcKeplerOrbit()
        self.assertIsInstance(a,gpstk.BrcKeplerOrbit)
        b = a.hasData()
        self.assertTrue(not b)

    def test_NavType(self):
        a = gpstk.NavType.GPSLNAV
        self.assertTrue(str(a) == 'GPSLNAV')

    def test_NavID(self):
        a = gpstk.NavID(gpstk.SatID(gpstk.SatelliteSystem.GPS),gpstk.ObsID(gpstk.ObservationType.Range, gpstk.CarrierBand.L1, gpstk.TrackingCode.CA))
        self.assertTrue(str(a) == 'Unknown')

    def test_PackedNavBits(self):
        a = gpstk.PackedNavBits()
        b = gpstk.PackedNavBits.psPassed
        self.assertIsInstance(a, gpstk.PackedNavBits)
        self.assertTrue(b == 1)

    def test_GPSEphemeris(self):
        a = gpstk.GPSEphemeris()
        b = a.isValid(gpstk.CommonTime.END_OF_TIME)
        self.assertIsInstance(a, gpstk.GPSEphemeris)
        self.assertTrue(not b)

    def test_EngEphemeris(self):
        a = gpstk.EngEphemeris()
        b = a.isValid()
        self.assertIsInstance(a, gpstk.EngEphemeris)
        self.assertTrue(not b)

    def test_GPSEphemerisStore(self):
        a = gpstk.GPSEphemerisStore()
        b = a.getName()
        self.assertIsInstance(a, gpstk.GPSEphemerisStore)
        self.assertTrue(b == "GPSEphemerisStore")

    def test_SP3SatID(self):
        a = gpstk.SP3SatID(gpstk.SatID(gpstk.SatelliteSystem.GPS))
        b = a.getfill()
        self.assertIsInstance(a, gpstk.SP3SatID)
        self.assertIsNotNone(b)

    def test_EngAlmanac(self):
        a = gpstk.EngAlmanac()
        self.assertIsInstance(a, gpstk.EngAlmanac)

    def EphemerisRange(self):
        a = gpstk.CorrectedEphemerisRange()
        self.assertIsInstance(a, gpstk.CorrectedEphemerisRange)

    def test_GloEphemeris(self):
        a = gpstk.GloEphemeris()
        self.assertIsInstance(a, gpstk.GloEphemeris)

    def test_GalEphemeris(self):
        a = gpstk.GalEphemeris()
        self.assertIsInstance(a, gpstk.GalEphemeris)

    def test_OrbElemBase(self):
        a = gpstk.OrbElemBase
        self.assertIsNotNone(a, gpstk.OrbElemBase)

    def test_OrbElemStore(self):
        a = gpstk.OrbElemStore()
        self.assertIsInstance(a, gpstk.OrbElemStore)

    def test_IonoModel(self):
        a = gpstk.IonoModel()
        self.assertIsInstance(a, gpstk.IonoModel)

    def test_IonoModelStore(self):
        a = gpstk.IonoModelStore()
        a.edit(gpstk.CommonTime.END_OF_TIME)
        self.assertIsInstance(a, gpstk.IonoModelStore)



if __name__ == '__main__':
    run_unit_tests()
