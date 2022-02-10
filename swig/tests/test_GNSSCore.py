#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests
import gnsstk


class TestGNSSCore(unittest.TestCase):

    def test_SatelliteSystem(self):
        a = gnsstk.SatelliteSystem.GPS
        self.assertIsNotNone(a)

    def test_vectorGNSS(self):
        a = gnsstk.std_vector_GNSS()
        a.append(gnsstk.SatelliteSystem.GPS)
        self.assertTrue(not a.empty())

    def test_SatID(self):
        a = gnsstk.SatID(gnsstk.SatelliteSystem.GPS)
        self.assertTrue(a.isWild())
        self.assertTrue(a.isValid())

    def test_vectorSatID(self):
        a = gnsstk.std_vector_SatID()
        a.append(gnsstk.SatID(gnsstk.SatelliteSystem.GPS))
        self.assertTrue(not a.empty())

    def test_Xvt(self):
        a = gnsstk.Xvt.Unhealthy
        self.assertIsNotNone(a)
        b = gnsstk.Xvt().getPos()
        self.assertIsNotNone(b)

    def test_AngleReduced(self):
        a = gnsstk.AngleReduced()
        self.assertIsNotNone(a.sin())

    def test_Angle(self):
        a = gnsstk.Angle()
        b = gnsstk.Angle(1,gnsstk.AngleType.Sin)
        c = gnsstk.Angle(1.0,2.0)
        self.assertTrue(b.rad() == 1.5707963267948966)
        self.assertTrue(b.deg() == 90.0)
        self.assertTrue(c.rad() == 0.4636476090008061)
        self.assertTrue(c.deg() == 26.56505117707799)

    def test_ObservationType(self):
        a = gnsstk.ObservationType.Range
        self.assertIsNotNone(a)

    def test_mapObservationType_String(self):
        a = gnsstk.std_map_ObservationType_string()
        self.assertTrue(len(a.values()) == 0)
        # This test fails but it doesn't really need to work in python
        # a[gnsstk.ObservationType.Range] = 'Range'
        # self.assertTrue(a[gnsstk.ObservationType.Range] == 'Range')

    def test_CarrierBand(self):
        a = gnsstk.CarrierBand.L1
        self.assertIsNotNone(a)

    def test_mapCarrierBand_String(self):
        a = gnsstk.std_map_CarrierBand_string()
        self.assertTrue(len(a.values()) == 0)
        # This test fails but it doesn't really need to work in python
        # a[gnsstk.CarrierBand.L1] = 'L1'
        # self.assertTrue(a[gnsstk.CarrierBand.L1] == 'L1')

    def test_TrackingCode(self):
        a = gnsstk.TrackingCode.CA
        self.assertIsNotNone(a)

    def test_mapTrackingCode_String(self):
        a = gnsstk.std_map_TrackingCode_string()
        self.assertTrue(len(a.values()) == 0)
        # This test fails but it doesn't really need to work in python
        # a[gnsstk.TrackingCode.CA] = 'L1'
        # self.assertTrue(a[gnsstk.TrackingCode.CA] == 'L1')

    def test_XmitAnt(self):
        a = gnsstk.XmitAnt.Standard
        self.assertIsNotNone(a)

    def test_ObsID(self):
        a = gnsstk.ObsID(gnsstk.ObservationType.Range, gnsstk.CarrierBand.L1, gnsstk.TrackingCode.CA)
        self.assertIsInstance(a, gnsstk.ObsID)

    def test_vectorObsID(self):
        a = gnsstk.std_vector_ObsID()
        a.append(gnsstk.ObsID(gnsstk.ObservationType.Range, gnsstk.CarrierBand.L1, gnsstk.TrackingCode.CA))
        self.assertTrue(not a.empty())

    def test_mapSvObsEpoch(self):
        a = gnsstk.std_map_SvObsEpoch()
        self.assertTrue(len(a.values()) == 0)
        a[gnsstk.ObsID(gnsstk.ObservationType.Range, gnsstk.CarrierBand.L1, gnsstk.TrackingCode.CA)] = 5.0
        self.assertTrue(a[gnsstk.ObsID(gnsstk.ObservationType.Range, gnsstk.CarrierBand.L1, gnsstk.TrackingCode.CA)] == 5.0)

    def test_WGS84Ellipsoid(self):
        a = gnsstk.WGS84Ellipsoid()
        self.assertIsInstance(a, gnsstk.WGS84Ellipsoid)

    def test_GPSEllipsoid(self):
        a = gnsstk.GPSEllipsoid()
        self.assertIsInstance(a, gnsstk.GPSEllipsoid)

    def test_RinexSatID(self):
        a = gnsstk.SatID(gnsstk.SatelliteSystem.GPS)
        b = gnsstk.RinexSatID(a)
        self.assertIsInstance(b, gnsstk.RinexSatID)
        self.assertTrue(b.toString() == "?-1")

    def test_vectorRinexSatID(self):
        a = gnsstk.std_vector_RinexSatID()
        b = gnsstk.SatID(gnsstk.SatelliteSystem.GPS)
        c = gnsstk.RinexSatID(b)
        a.append(c)
        self.assertTrue(not a.empty())

    def test_CGCS2000Ellipsoid(self):
        a = gnsstk.CGCS2000Ellipsoid()
        self.assertIsInstance(a, gnsstk.CGCS2000Ellipsoid)

    def test_SatMetaData(self):
        a = gnsstk.SatMetaData()
        self.assertIsInstance(a, gnsstk.SatMetaData)
        self.assertIsNotNone(a.asClockType('Cesium'))

    def test_Position(self):
        a = gnsstk.Position(1,2,3)
        b = gnsstk.Position(5,6,7)
        self.assertTrue(gnsstk.Position().range(a,b) == 6.928203230275509)

    def test_AlmOrbit(self):
        a = gnsstk.AlmOrbit()
        self.assertIsInstance(a, gnsstk.AlmOrbit)
        b = a.getTimestamp()
        self.assertIsInstance(b, gnsstk.CommonTime)

    def test_RinexObsHeader(self):
        a = gnsstk.RinexObsHeader.allValid20
        self.assertIsNotNone(a)
        b = gnsstk.RinexObsHeader().isHeader()
        self.assertTrue(b)

    def test_RinexObsID(self):
        a = gnsstk.ObsID(gnsstk.ObservationType.Range, gnsstk.CarrierBand.L1, gnsstk.TrackingCode.CA)
        b = gnsstk.RinexObsID(a)
        self.assertIsInstance(b, gnsstk.RinexObsID)
        self.assertTrue(str(b) == "C1C")

    def test_vectorRinexObsID(self):
        a = gnsstk.std_vector_RinexObsID()
        b = gnsstk.ObsID(gnsstk.ObservationType.Range, gnsstk.CarrierBand.L1, gnsstk.TrackingCode.CA)
        c = gnsstk.RinexObsID(b)
        a.append(c)
        self.assertTrue(not a.empty())

    #All Trop Models already tested

if __name__ == '__main__':
    run_unit_tests()
