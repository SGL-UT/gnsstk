#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests

import gpstk

class TestNavSignalID(unittest.TestCase):
    def test_str(self):
        nsig = gpstk.NavSignalID(gpstk.SatelliteSystem.GPS, gpstk.CarrierBand.L1, gpstk.TrackingCode.CA, gpstk.NavType.GPSLNAV)
        self.assertEqual(str(nsig), 'GPS L1 GPSC/A navmsg GPS_LNAV')

class TestNavSatelliteID(unittest.TestCase):
    def test_str(self):
        nsat = gpstk.NavSatelliteID(19, 23, gpstk.SatelliteSystem.GPS, gpstk.CarrierBand.L1, gpstk.TrackingCode.CA, gpstk.NavType.GPSLNAV)
        self.assertEqual(str(nsat), 'subj:GPS 19 xmit:GPS 23 GPS L1 GPSC/A navmsg GPS_LNAV')

class TestNavMessageID(unittest.TestCase):
    def test_str(self):
        nsat = gpstk.NavSatelliteID(19, 23, gpstk.SatelliteSystem.GPS, gpstk.CarrierBand.L1, gpstk.TrackingCode.CA, gpstk.NavType.GPSLNAV)
        nmid = gpstk.NavMessageID(nsat,gpstk.NavMessageType.Almanac)
        self.assertEqual(str(nmid), 'Almanac subj:GPS 19 xmit:GPS 23 GPS L1 GPSC/A navmsg GPS_LNAV')

class TestNavMessageType(unittest.TestCase):
    def test_str(self):
        self.assertEqual(str(gpstk.NavMessageType.Unknown), 'Unknown')
        self.assertEqual(str(gpstk.NavMessageType.Almanac), 'Almanac')
        self.assertEqual(str(gpstk.NavMessageType.Ephemeris), 'Ephemeris')
        self.assertEqual(str(gpstk.NavMessageType.TimeOffset), 'TimeOffset')
        self.assertEqual(str(gpstk.NavMessageType.Health), 'Health')
        self.assertEqual(str(gpstk.NavMessageType.Clock), 'Clock')
        self.assertEqual(str(gpstk.NavMessageType.Last), 'Last')

class TestSVHealth(unittest.TestCase):
    def test_str(self):
        self.assertEqual(str(gpstk.SVHealth.Unknown), 'Unknown')
        self.assertEqual(str(gpstk.SVHealth.Any), 'Any')
        self.assertEqual(str(gpstk.SVHealth.Healthy), 'Healthy')
        self.assertEqual(str(gpstk.SVHealth.Unhealthy), 'Unhealthy')
        self.assertEqual(str(gpstk.SVHealth.Degraded), 'Degraded')
        self.assertEqual(str(gpstk.SVHealth.Last), 'Last')

class TestNavSearchOrder(unittest.TestCase):
    def test_str(self):
        self.assertEqual(str(gpstk.NavSearchOrder.User), 'User')
        self.assertEqual(str(gpstk.NavSearchOrder.Nearest), 'Nearest')

class TestNavValidityType(unittest.TestCase):
    def test_str(self):
        self.assertEqual(str(gpstk.NavValidityType.Unknown), 'Unknown')
        self.assertEqual(str(gpstk.NavValidityType.ValidOnly), 'ValidOnly')
        self.assertEqual(str(gpstk.NavValidityType.InvalidOnly), 'InvalidOnly')
        self.assertEqual(str(gpstk.NavValidityType.Any), 'Any')
        self.assertEqual(str(gpstk.NavValidityType.Last), 'Last')

class TestDumpDetail(unittest.TestCase):
    def test_str(self):
        self.assertEqual(str(gpstk.DumpDetail.OneLine), 'OneLine')
        self.assertEqual(str(gpstk.DumpDetail.Brief), 'Brief')
        self.assertEqual(str(gpstk.DumpDetail.Full), 'Full')

if __name__ == '__main__':
    run_unit_tests()
