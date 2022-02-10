#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

class TestNavSignalID(unittest.TestCase):
    def test_str(self):
        nsig = gnsstk.NavSignalID(gnsstk.SatelliteSystem.GPS, gnsstk.CarrierBand.L1, gnsstk.TrackingCode.CA, gnsstk.NavType.GPSLNAV)
        self.assertEqual(str(nsig), 'GPS L1 GPSC/A navmsg GPS_LNAV')

class TestNavSatelliteID(unittest.TestCase):
    def test_str(self):
        nsat = gnsstk.NavSatelliteID(19, 23, gnsstk.SatelliteSystem.GPS, gnsstk.CarrierBand.L1, gnsstk.TrackingCode.CA, gnsstk.NavType.GPSLNAV)
        self.assertEqual(str(nsat), 'subj:GPS 19 xmit:GPS 23 GPS L1 GPSC/A navmsg GPS_LNAV')

class TestNavMessageID(unittest.TestCase):
    def test_str(self):
        nsat = gnsstk.NavSatelliteID(19, 23, gnsstk.SatelliteSystem.GPS, gnsstk.CarrierBand.L1, gnsstk.TrackingCode.CA, gnsstk.NavType.GPSLNAV)
        nmid = gnsstk.NavMessageID(nsat,gnsstk.NavMessageType.Almanac)
        self.assertEqual(str(nmid), 'Almanac subj:GPS 19 xmit:GPS 23 GPS L1 GPSC/A navmsg GPS_LNAV')

class TestNavMessageType(unittest.TestCase):
    def test_str(self):
        self.assertEqual(str(gnsstk.NavMessageType.Unknown), 'Unknown')
        self.assertEqual(str(gnsstk.NavMessageType.Almanac), 'Almanac')
        self.assertEqual(str(gnsstk.NavMessageType.Ephemeris), 'Ephemeris')
        self.assertEqual(str(gnsstk.NavMessageType.TimeOffset), 'TimeOffset')
        self.assertEqual(str(gnsstk.NavMessageType.Health), 'Health')
        self.assertEqual(str(gnsstk.NavMessageType.Clock), 'Clock')
        self.assertEqual(str(gnsstk.NavMessageType.Last), 'Last')

class TestSVHealth(unittest.TestCase):
    def test_str(self):
        self.assertEqual(str(gnsstk.SVHealth.Unknown), 'Unknown')
        self.assertEqual(str(gnsstk.SVHealth.Any), 'Any')
        self.assertEqual(str(gnsstk.SVHealth.Healthy), 'Healthy')
        self.assertEqual(str(gnsstk.SVHealth.Unhealthy), 'Unhealthy')
        self.assertEqual(str(gnsstk.SVHealth.Degraded), 'Degraded')
        self.assertEqual(str(gnsstk.SVHealth.Last), 'Last')

class TestNavSearchOrder(unittest.TestCase):
    def test_str(self):
        self.assertEqual(str(gnsstk.NavSearchOrder.User), 'User')
        self.assertEqual(str(gnsstk.NavSearchOrder.Nearest), 'Nearest')

class TestNavValidityType(unittest.TestCase):
    def test_str(self):
        self.assertEqual(str(gnsstk.NavValidityType.Unknown), 'Unknown')
        self.assertEqual(str(gnsstk.NavValidityType.ValidOnly), 'ValidOnly')
        self.assertEqual(str(gnsstk.NavValidityType.InvalidOnly), 'InvalidOnly')
        self.assertEqual(str(gnsstk.NavValidityType.Any), 'Any')
        self.assertEqual(str(gnsstk.NavValidityType.Last), 'Last')

class TestDumpDetail(unittest.TestCase):
    def test_str(self):
        self.assertEqual(str(gnsstk.DumpDetail.OneLine), 'OneLine')
        self.assertEqual(str(gnsstk.DumpDetail.Brief), 'Brief')
        self.assertEqual(str(gnsstk.DumpDetail.Full), 'Full')

if __name__ == '__main__':
    run_unit_tests()
