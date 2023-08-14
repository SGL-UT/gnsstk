#!/usr/env python

import unittest, sys, os
import math
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests
import gnsstk

from tests import AbstractBaseTesters

class TestGNSSCore(unittest.TestCase):

    def test_SatelliteSystem(self):
        a = gnsstk.SatelliteSystem.GPS
        self.assertIsNotNone(a)
        b = gnsstk.satTimeSystem(a)
        self.assertIsNotNone(b)

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


class TestBCISCorrector(unittest.TestCase):
    """
    Tests for BCISCorrector
    """

    def test_getCorr(self):
        """
        Test BCISCorrector setup and getCorr

        Python users should be able to create a BCISCorrector
        and compute a correction from inputs.
        """
        navLib = gnsstk.NavLibrary()
        isc = gnsstk.BCISCorrector(navLib)

        ndf = gnsstk.MultiFormatNavDataFactory()
        ndf.addDataSource(os.path.join(args.input_dir, "arlm2000.15n"))
        navLib.addFactory(ndf)

        stnPos = gnsstk.Position(-740290.01, -5457071.705, 3207245.599)
        svPos = gnsstk.Position(-16208820.579, -207275.833, 21038422.516)
        when = gnsstk.CivilTime(2015,7,19,4,30,0,gnsstk.TimeSystem.Any).toCommonTime()
        sat = gnsstk.SatID(27, gnsstk.SatelliteSystem.GPS)
        oid = gnsstk.ObsID(gnsstk.ObservationType.Phase,gnsstk.CarrierBand.L1,gnsstk.TrackingCode.CA)
        nav = gnsstk.NavType.GPSLNAV

        rv, corr = isc.getCorr(stnPos, svPos, sat, oid, when, nav)

        self.assertEqual(True, rv)
        self.assertAlmostEqual(2.04890966415e-08, corr)

        svXvt = gnsstk.Xvt()
        svXvt.x = svPos
        rv, corr = isc.getCorr(stnPos, svXvt, sat, oid, when, nav)

        self.assertEqual(True, rv)
        self.assertAlmostEqual(2.04890966415e-08, corr)


class TestBCIonoCorrector(unittest.TestCase):
    """
    Tests for BCIonoCorrector
    """

    def test_getCorr(self):
        """
        Test BCIonoCorrector setup and getCorr.

        Python users should be able to create a BCIonoCorrector
        and compute a correction from inputs.
        """
        navLib = gnsstk.NavLibrary()
        iono = gnsstk.BCIonoCorrector(navLib)

        ndf = gnsstk.MultiFormatNavDataFactory()
        ndf.addDataSource(os.path.join(args.input_dir, "mixed.06n"))
        navLib.addFactory(ndf)

        stnPos = gnsstk.Position(-740290.01, -5457071.705, 3207245.599)
        svPos = gnsstk.Position(-16208820.579, -207275.833, 21038422.516)
        when = gnsstk.CivilTime(2006,10,1,4,30,0,gnsstk.TimeSystem.Any).toCommonTime()
        sat = gnsstk.SatID(27, gnsstk.SatelliteSystem.GPS)
        oid = gnsstk.ObsID(gnsstk.ObservationType.Iono,gnsstk.CarrierBand.L1,gnsstk.TrackingCode.CA)
        nav = gnsstk.NavType.GPSLNAV

        rv, corr = iono.getCorr(stnPos, svPos, sat, oid, when, nav)

        self.assertEqual(True, rv)
        self.assertAlmostEqual(3.623343027333741, corr)

        svXvt = gnsstk.Xvt()
        svXvt.x = svPos
        rv, corr = iono.getCorr(stnPos, svXvt, sat, oid, when, nav)

        self.assertEqual(True, rv)
        self.assertAlmostEqual(3.623343027333741, corr)


class TestCorrDupHandling(AbstractBaseTesters.EnumTester):
    """
    Test existence and string conversions of CorrDupHandling enum
    """

    ENUM = gnsstk.CorrDupHandling

    EXP_ENUM_STRS = {
        gnsstk.CorrDupHandling.Unknown: "Unknown",
        gnsstk.CorrDupHandling.ComputeFirst: "ComputeFirst",
        gnsstk.CorrDupHandling.ComputeLast: "ComputeLast",
        gnsstk.CorrDupHandling.UseFirst: "UseFirst",
    }

    STR_CONVERTER = gnsstk.asCorrDupHandling


class TestCorrectorType(AbstractBaseTesters.EnumTester):
    """
    Test existence and string conversions of CorrectorType enum
    """

    ENUM = gnsstk.CorrectorType

    EXP_ENUM_STRS = {
        gnsstk.CorrectorType.Unknown: "Unknown",
        gnsstk.CorrectorType.Trop: "Trop",
        gnsstk.CorrectorType.Iono: "Iono",
        gnsstk.CorrectorType.ISC: "ISC",
        gnsstk.CorrectorType.Multipath: "Multipath",
        gnsstk.CorrectorType.RxChlBias: "RxChlBias"
    }

    STR_CONVERTER = gnsstk.asCorrectorType


class TestCorrectionResult(unittest.TestCase):
    """
    Tests for CorrectionResult
    """

    def test_result(self):
        """
        Test general manipulation of CorrectionResult object
        """
        corr_nan = gnsstk.CorrectionResult()
        navLib = gnsstk.NavLibrary()
        corr_iono = gnsstk.CorrectionResult(100, gnsstk.BCIonoCorrector(navLib))

        self.assertTrue(math.isnan(corr_nan.result))
        self.assertEqual(corr_iono.result, 100)
        self.assertEqual(corr_iono.source.corrType, gnsstk.CorrectorType.Iono)

class TestCorrectionResults(unittest.TestCase):
    """
    Tests for CorrectionResults
    """

    def test_results(self):
        """
        Test general manipulation of CorrectionResults objects.

        Most important will be the ability to get the total sum of corrections
        and getting individual corrections within the list.
        """
        corr_results = gnsstk.CorrectionResults()
        navLib = gnsstk.NavLibrary()

        corr_trop = gnsstk.CorrectionResult(150, gnsstk.GlobalTropCorrector())
        corr_iono = gnsstk.CorrectionResult(100, gnsstk.BCIonoCorrector(navLib))
        corr_ISC = gnsstk.CorrectionResult(-75, gnsstk.BCISCorrector(navLib))
        corr_ISC2 = gnsstk.CorrectionResult(45, gnsstk.BCISCorrector(navLib))

        corr_results.addResult(corr_trop)
        corr_results.addResult(corr_iono)
        corr_results.addResult(corr_ISC)
        corr_results.addResult(corr_ISC2)

        self.assertEqual(len(corr_results.getResults()), 4)
        self.assertEqual(corr_results.getResults()[0].result, 150)
        self.assertEqual(corr_results.getCorrSum(gnsstk.CorrDupHandling.ComputeFirst), 175)
        self.assertEqual(corr_results.getCorrSum(gnsstk.CorrDupHandling.ComputeLast), 295)

class TestGroupPathCorr(unittest.TestCase):
    """
    Tests of the GroupPathCorr SWIG bindings.
    """

    def test_groupPathCorrManual(self):
        """
        Test manual setup of GroupPathCorr.

        GroupPathCorr should allow manually appending correctors.
        """
        pec = gnsstk.GroupPathCorr()

        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.MultiFormatNavDataFactory()
        ndf.addDataSource(os.path.join(args.input_dir, "arlm2000.15n"))
        navLib.addFactory(ndf)

        stnPos = gnsstk.Position(-740290.01, -5457071.705, 3207245.599)
        svPos = gnsstk.Position(-16208820.579, -207275.833, 21038422.516)
        when = gnsstk.CivilTime(2015,7,19,4,30,0,gnsstk.TimeSystem.Any).toCommonTime()
        sat = gnsstk.SatID(27, gnsstk.SatelliteSystem.GPS)
        oid = gnsstk.ObsID(gnsstk.ObservationType.Iono,gnsstk.CarrierBand.L1,gnsstk.TrackingCode.CA)
        nav = gnsstk.NavType.GPSLNAV

        iono = gnsstk.BCIonoCorrector(navLib)
        trop = gnsstk.GlobalTropCorrector()
        trop.setDefaultWx()

        isc = gnsstk.BCISCorrector(navLib)

        pec.calcs.append(iono)
        pec.calcs.append(trop)
        pec.calcs.append(isc)

        rv, corr = pec.getCorr(stnPos, svPos, sat, oid, when, nav)

        self.assertTrue(rv)
        self.assertAlmostEqual(corr, 13.085598163018554)

    def test_groupPathCorrInit(self):
        """
        Test the `init` method and resulting correction.

        This should automatically setup GroupPathCorr with default ISC and Iono
        models.
        """
        pec = gnsstk.GroupPathCorr()

        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.MultiFormatNavDataFactory()
        ndf.addDataSource(os.path.join(args.input_dir, "arlm2000.15n"))
        navLib.addFactory(ndf)

        stnPos = gnsstk.Position(-740290.01, -5457071.705, 3207245.599)
        svPos = gnsstk.Position(-16208820.579, -207275.833, 21038422.516)
        when = gnsstk.CivilTime(2015,7,19,4,30,0,gnsstk.TimeSystem.Any).toCommonTime()
        sat = gnsstk.SatID(27, gnsstk.SatelliteSystem.GPS)
        oid = gnsstk.ObsID(gnsstk.ObservationType.Iono,gnsstk.CarrierBand.L1,gnsstk.TrackingCode.CA)
        nav = gnsstk.NavType.GPSLNAV

        rv = pec.init(navLib)
        self.assertTrue(rv)

        rv, corr = pec.getCorr(stnPos, svPos, sat, oid, when, nav)
        self.assertTrue(rv)
        self.assertAlmostEqual(corr, 3.6233430478228374)

    def test_groupPathCorrGlobal(self):
        """
        Test the `initGlobal` method and resulting correction.

        This should automatically setup GroupPathCorr with default, ISC, Iono,
        and Global Trop models.
        """
        pec = gnsstk.GroupPathCorr()

        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.MultiFormatNavDataFactory()
        ndf.addDataSource(os.path.join(args.input_dir, "arlm2000.15n"))
        navLib.addFactory(ndf)

        stnPos = gnsstk.Position(-740290.01, -5457071.705, 3207245.599)
        svPos = gnsstk.Position(-16208820.579, -207275.833, 21038422.516)
        when = gnsstk.CivilTime(2015,7,19,4,30,0,gnsstk.TimeSystem.Any).toCommonTime()
        sat = gnsstk.SatID(27, gnsstk.SatelliteSystem.GPS)
        oid = gnsstk.ObsID(gnsstk.ObservationType.Iono,gnsstk.CarrierBand.L1,gnsstk.TrackingCode.CA)
        nav = gnsstk.NavType.GPSLNAV

        rv = pec.initGlobal(navLib, os.path.join(args.input_dir, "arlm2000.15m"))
        self.assertTrue(rv)

        rv, corr = pec.getCorr(stnPos, svPos, sat, oid, when, nav)
        self.assertTrue(rv)
        self.assertAlmostEqual(corr, 13.399231057432754)

    def test_groupPathCorrNB(self):
        """
        Test the `initNB` method and resulting correction

        This should automatically setup GroupPathCorr with default
        ISC, Iono, and NB Trop models.
        """
        pec = gnsstk.GroupPathCorr()

        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.MultiFormatNavDataFactory()
        ndf.addDataSource(os.path.join(args.input_dir, "arlm2000.15n"))
        navLib.addFactory(ndf)

        stnPos = gnsstk.Position(-740290.01, -5457071.705, 3207245.599)
        svPos = gnsstk.Position(-16208820.579, -207275.833, 21038422.516)
        when = gnsstk.CivilTime(2015,7,19,4,30,0,gnsstk.TimeSystem.Any).toCommonTime()
        sat = gnsstk.SatID(27, gnsstk.SatelliteSystem.GPS)
        oid = gnsstk.ObsID(gnsstk.ObservationType.Iono,gnsstk.CarrierBand.L1,gnsstk.TrackingCode.CA)
        nav = gnsstk.NavType.GPSLNAV

        rv = pec.initNB(navLib, os.path.join(args.input_dir, "arlm2000.15m"))
        self.assertTrue(rv)

        rv, corr = pec.getCorr(stnPos, svPos, sat, oid, when, nav)
        self.assertTrue(rv)
        self.assertAlmostEqual(corr, 12.468046669500502)


class TestTropCorrector(unittest.TestCase):

    def test_getCorr(self):
        """
        Test TropCorrector getCorr.

        This is a regression test of the computed trop correction
        """
        trop = gnsstk.GlobalTropCorrector()
        errcalc = gnsstk.GroupPathCorr()
        errcalc.calcs.append(trop)
        rv = trop.loadFile(os.path.join(args.input_dir, "arlm2000.15m"))
        self.assertEqual(True, rv)
        stnPos = gnsstk.Position(-740290.01, -5457071.705, 3207245.599)
        svPos = gnsstk.Position(-16208820.579, -207275.833, 21038422.516)
        when = gnsstk.CivilTime(2015,7,19,4,30,0,gnsstk.TimeSystem.Any).toCommonTime()
        sat = gnsstk.SatID(27, gnsstk.SatelliteSystem.GPS)
        oid = gnsstk.ObsID()
        nav = gnsstk.NavType.GPSLNAV
        rv,corr = errcalc.getCorr(stnPos,svPos,sat,oid,when,nav)
        self.assertEqual(True, rv)
        self.assertAlmostEqual(9.775888009609917, corr)

    def test_getCorrDefaultWx(self):
        """
        TropCorrector must accept default weather.

        This also serves as a regression test of the compute correction.
        """
        trop = gnsstk.NBTropCorrector()
        errcalc = gnsstk.GroupPathCorr()
        errcalc.calcs.append(trop)
        trop.setDefaultWx(15, 1000, 75)
        stnPos = gnsstk.Position(-740290.01, -5457071.705, 3207245.599)
        svPos = gnsstk.Position(-16208820.579, -207275.833, 21038422.516)
        when = gnsstk.CivilTime(2015,7,19,4,30,0,gnsstk.TimeSystem.Any).toCommonTime()
        sat = gnsstk.SatID(27, gnsstk.SatelliteSystem.GPS)
        oid = gnsstk.ObsID()
        nav = gnsstk.NavType.GPSLNAV
        rv,corr = errcalc.getCorr(stnPos,svPos,sat,oid,when,nav)
        self.assertEqual(True, rv)
        self.assertAlmostEqual(9.06752734042942, corr)

if __name__ == '__main__':
    run_unit_tests()
