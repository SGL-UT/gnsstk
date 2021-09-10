#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests
import gnsstk


class TestConstants(unittest.TestCase):
    """Tests the various constants defined in the gnsstk"""
    def testConstants(self):
        """Tests a subset of the gnsstk constants since there are so many.
        Add more as we mature the testing."""
        self.assertEqual(gnsstk.C_MPS, 299792458)
        self.assertEqual(gnsstk.CA_CHIP_FREQ_GPS, 1023000)
        self.assertEqual(gnsstk.L1_FREQ_GPS, 1575420000)
        self.assertEqual(gnsstk.L2_FREQ_GPS, 1227600000)
        self.assertAlmostEqual(gnsstk.PI, 3.14159265359, places=12)
        self.assertEqual(gnsstk.ZCOUNT_PER_DAY, 57600)
        with self.assertRaises(AttributeError):
            print(gnsstk.foo.bar)

class GPS_consants_test(unittest.TestCase):
    def test_constants(self):
        self.assertEqual(32, gnsstk.MAX_PRN)
        self.assertEqual(32, gnsstk.MAX_PRN_GPS)


class Geometry_test(unittest.TestCase):
    def test_constants(self):
        self.assertEqual(1.7453292519943e-2, gnsstk.DEG_TO_RAD)
        self.assertEqual(57.295779513082, gnsstk.RAD_TO_DEG)


class GNSSconstants_test(unittest.TestCase):
    def test_constants(self):
        self.assertEqual(gnsstk.PI, 3.141592653589793238462643383280)
        self.assertEqual(gnsstk.OSC_FREQ_GPS, 10.23e6)
        self.assertEqual(gnsstk.L6_FREQ_GAL, 1278.75e6)

    def test_functions(self):
        self.assertEqual(4, gnsstk.getLegacyFitInterval(15, 27))
        self.assertAlmostEqual(0.190293672798, gnsstk.getWavelength(gnsstk.SatelliteSystem.GPS, 1),)
        self.assertAlmostEqual(1.2833333333333334, gnsstk.getBeta(1, 1, 2))
        self.assertAlmostEqual(0.6469444444444448, gnsstk.getAlpha(1, 1, 2))

if __name__ == '__main__':
    run_unit_tests()
