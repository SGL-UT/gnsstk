#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests
import gnsstk


class TestGNSSEph(unittest.TestCase):

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

    def test_RawRange(self):
        """
        Test swig interface to RawRange class

        Correctness test already exists in C++ at RawRange_T.cpp.
        This test ensures that the class is accessible and usable in python
        """
        rxPos = gnsstk.Position(1000, 2000, 3000)
        svPos = gnsstk.Position(30000, 20000, 10000)
        tof = 0.07
        ellipsoid = gnsstk.GPSEllipsoid()

        rawrange, svPos = gnsstk.RawRange.computeRange(rxPos, svPos, tof, ellipsoid)
        self.assertIsInstance(rawrange, float)
        self.assertGreater(rawrange, 0)
        self.assertIsInstance(svPos, gnsstk.Position)

        svXvt = gnsstk.Xvt()
        svXvt.x = gnsstk.Position(30000, 20000, 100000)
        success, rawrange, svXvt = gnsstk.RawRange.fromSvPos(rxPos, svXvt, ellipsoid)
        self.assertTrue(success)
        self.assertGreater(rawrange, 0)
        self.assertIsInstance(svXvt, gnsstk.Xvt)





if __name__ == '__main__':
    run_unit_tests()
