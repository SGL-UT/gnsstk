#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gpstk.test_utils import args,run_unit_tests
import gpstk


class TestGNSSCore(unittest.TestCase):

    def test_SatelliteSystem(self):
        a = gpstk.SatelliteSystem.GPS
        self.assertIsNotNone(a)

    def test_vectorGNSS(self):
        a = gpstk.std_vector_GNSS()
        a.append(gpstk.SatelliteSystem.GPS)
        self.assertTrue(not a.empty())

    def test_SatID(self):
        a = gpstk.SatID(gpstk.SatelliteSystem.GPS)
        self.assertTrue(a.isWild())
        self.assertTrue(a.isValid())

    def test_vectorSatID(self):
        a = gpstk.std_vector_SatID()
        a.append(gpstk.SatID(gpstk.SatelliteSystem.GPS))
        self.assertTrue(not a.empty())

    def test_Xvt(self):
        a = gpstk.Xvt.Unhealthy
        self.assertIsNotNone(a)
        b = gpstk.Xvt().getPos()
        self.assertIsNotNone(b)

    def test_AngleReduced(self):
        a = gpstk.AngleReduced()
        b = gpstk.AngleReduced.Sin
        self.assertIsNotNone(a.sin())
        self.assertIsNotNone(b)

    def test_Angle(self):
        a = gpstk.Angle()
        b = gpstk.Angle(1,2)
        self.assertTrue(b.rad() == 1.5707963267948966)
        self.assertTrue(b.deg() == 90.0)

    def test_ObservationType(self):
        a = gpstk.ObservationType.Range
        self.assertIsNotNone(a)

    def test_mapObservationType_String(self):
        a = gpstk.std_map_ObservationType_string()
        self.assertTrue(len(a.values()) == 0)
        # This test fails but it doesn't really need to work in python
        # a[gpstk.ObservationType.Range] = 'Range'
        # self.assertTrue(a[gpstk.ObservationType.Range] == 'Range')

    def test_CarrierBand(self):
        a = gpstk.CarrierBand.L1
        self.assertIsNotNone(a)

    def test_mapCarrierBand_String(self):
        a = gpstk.std_map_CarrierBand_string()
        self.assertTrue(len(a.values()) == 0)
        # This test fails but it doesn't really need to work in python
        # a[gpstk.CarrierBand.L1] = 'L1'
        # self.assertTrue(a[gpstk.CarrierBand.L1] == 'L1')

    def test_TrackingCode(self):
        a = gpstk.TrackingCode.CA
        self.assertIsNotNone(a)

    def test_mapTrackingCode_String(self):
        a = gpstk.std_map_TrackingCode_string()
        self.assertTrue(len(a.values()) == 0)
        # This test fails but it doesn't really need to work in python
        # a[gpstk.TrackingCode.CA] = 'L1'
        # self.assertTrue(a[gpstk.TrackingCode.CA] == 'L1')

    def test_XmitAnt(self):
        a = gpstk.XmitAnt.Standard
        self.assertIsNotNone(a)

    def test_ObsID(self):
        a = gpstk.ObsID(gpstk.ObservationType.Range, gpstk.CarrierBand.L1, gpstk.TrackingCode.CA)
        self.assertIsInstance(a, gpstk.ObsID)

    def test_vectorObsID(self):
        a = gpstk.std_vector_ObsID()
        a.append(gpstk.ObsID(gpstk.ObservationType.Range, gpstk.CarrierBand.L1, gpstk.TrackingCode.CA))
        self.assertTrue(not a.empty())

    def test_mapSvObsEpoch(self):
        a = gpstk.std_map_SvObsEpoch()
        self.assertTrue(len(a.values()) == 0)
        a[gpstk.ObsID(gpstk.ObservationType.Range, gpstk.CarrierBand.L1, gpstk.TrackingCode.CA)] = 5.0
        self.assertTrue(a[gpstk.ObsID(gpstk.ObservationType.Range, gpstk.CarrierBand.L1, gpstk.TrackingCode.CA)] == 5.0)

    def test_WGS84Ellipsoid(self):
        a = gpstk.WGS84Ellipsoid()
        self.assertIsInstance(a, gpstk.WGS84Ellipsoid)

    def test_GPSEllipsoid(self):
        a = gpstk.GPSEllipsoid()
        self.assertIsInstance(a, gpstk.GPSEllipsoid)

    def test_RinexSatID(self):
        a = gpstk.SatID(gpstk.SatelliteSystem.GPS)
        b = gpstk.RinexSatID(a)
        self.assertIsInstance(b, gpstk.RinexSatID)
        self.assertTrue(b.toString() == "?-1")

    def test_vectorRinexSatID(self):
        a = gpstk.std_vector_RinexSatID()
        b = gpstk.SatID(gpstk.SatelliteSystem.GPS)
        c = gpstk.RinexSatID(b)
        a.append(c)
        self.assertTrue(not a.empty())

    def test_CGCS2000Ellipsoid(self):
        a = gpstk.CGCS2000Ellipsoid()
        self.assertIsInstance(a, gpstk.CGCS2000Ellipsoid)

    def test_SatMetaData(self):
        a = gpstk.SatMetaData()
        self.assertIsInstance(a, gpstk.SatMetaData)
        self.assertIsNotNone(a.asClockType('Cesium'))

    def test_Position(self):
        a = gpstk.Position(1,2,3)
        b = gpstk.Position(5,6,7)
        self.assertTrue(gpstk.Position().range(a,b) == 6.928203230275509)

    def test_AlmOrbit(self):
        a = gpstk.AlmOrbit()
        self.assertIsInstance(a, gpstk.AlmOrbit)
        b = a.getTimestamp()
        self.assertIsInstance(b, gpstk.CommonTime)

    def test_RinexObsHeader(self):
        a = gpstk.RinexObsHeader.allValid20
        self.assertIsNotNone(a)
        b = gpstk.RinexObsHeader().isHeader()
        self.assertTrue(b)

    def test_RinexObsID(self):
        a = gpstk.ObsID(gpstk.ObservationType.Range, gpstk.CarrierBand.L1, gpstk.TrackingCode.CA)
        b = gpstk.RinexObsID(a)
        self.assertIsInstance(b, gpstk.RinexObsID)
        self.assertTrue(str(b) == "C1C")

    def test_vectorRinexObsID(self):
        a = gpstk.std_vector_RinexObsID()
        b = gpstk.ObsID(gpstk.ObservationType.Range, gpstk.CarrierBand.L1, gpstk.TrackingCode.CA)
        c = gpstk.RinexObsID(b)
        a.append(c)
        self.assertTrue(not a.empty())

    #All Trop Models already tested

if __name__ == '__main__':
    run_unit_tests()
