#!/usr/env python

import unittest, sys, os
sys.path.insert(0, os.path.abspath(".."))
from gnsstk.test_utils import args,run_unit_tests

import gnsstk

class TestNavLibrary(unittest.TestCase):
    def test_getXvt(self):
        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.RinexNavDataFactory()
        navLib.addFactory(ndf)
        ndf.addDataSource(args.input_dir+'/arlm2000.15n')
        sat = gnsstk.NavSatelliteID(5, 5, gnsstk.SatelliteSystem.GPS,
                                   gnsstk.CarrierBand.L1,
                                   gnsstk.TrackingCode.CA, gnsstk.NavType.GPSLNAV)
        ct = gnsstk.CivilTime(2015,7,19,2,0,35,
                             gnsstk.TimeSystem.GPS).toCommonTime()
        # test getXvt (first signature)
        xvt = gnsstk.Xvt()
        rv = navLib.getXvt(sat, ct, xvt, False)
        self.assertEqual(True, rv)
        self.assertEqual(  9345531.5274733770639, xvt.x[0])
        self.assertEqual(-12408177.088141856715,  xvt.x[1])
        self.assertEqual( 21486320.848036296666,  xvt.x[2])
        self.assertEqual(2081.276961058104007,    xvt.v[0])
        self.assertEqual(1792.4445008638492709,   xvt.v[1])
        self.assertEqual( 148.29209115082824155,  xvt.v[2])
        self.assertEqual(-0.00021641018042870913346, xvt.clkbias)
        self.assertEqual(4.3200998334200003381e-12, xvt.clkdrift)
        self.assertEqual(-8.8197758101551758427e-09, xvt.relcorr)
        self.assertEqual(gnsstk.Xvt.Healthy, xvt.health)
        # test getXvt (second signature)
        xvt2 = gnsstk.Xvt()
        rv = navLib.getXvt(sat, ct, xvt2, gnsstk.SVHealth.Any)
        self.assertEqual(True, rv)
        self.assertEqual(  9345531.5274733770639, xvt.x[0])
        self.assertEqual(-12408177.088141856715,  xvt.x[1])
        self.assertEqual( 21486320.848036296666,  xvt.x[2])
        self.assertEqual(2081.276961058104007,    xvt.v[0])
        self.assertEqual(1792.4445008638492709,   xvt.v[1])
        self.assertEqual( 148.29209115082824155,  xvt.v[2])
        self.assertEqual(-0.00021641018042870913346, xvt.clkbias)
        self.assertEqual(4.3200998334200003381e-12, xvt.clkdrift)
        self.assertEqual(-8.8197758101551758427e-09, xvt.relcorr)
        self.assertEqual(gnsstk.Xvt.Healthy, xvt.health)

    def test_getHealth(self):
        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.RinexNavDataFactory()
        navLib.addFactory(ndf)
        ndf.addDataSource(args.input_dir+'/arlm2000.15n')
        sat = gnsstk.NavSatelliteID(10, 10, gnsstk.SatelliteSystem.GPS,
                                   gnsstk.CarrierBand.L1,
                                   gnsstk.TrackingCode.CA, gnsstk.NavType.GPSLNAV)
        ct = gnsstk.CivilTime(2015,7,19,2,0,35,
                             gnsstk.TimeSystem.GPS).toCommonTime()
        rv,health = navLib.getHealth(sat, ct, gnsstk.SVHealth.Any)
        self.assertEqual(False, rv)
        ct = gnsstk.CivilTime(2015,7,19,12,35,35,
                             gnsstk.TimeSystem.GPS).toCommonTime()
        rv,health = navLib.getHealth(sat, ct, gnsstk.SVHealth.Any)
        self.assertEqual(False, rv)
        ct = gnsstk.CivilTime(2015,7,19,12,35,36,
                             gnsstk.TimeSystem.GPS).toCommonTime()
        rv,health = navLib.getHealth(sat, ct, gnsstk.SVHealth.Any)
        self.assertEqual(True, rv)
        self.assertEqual(gnsstk.SVHealth.Unhealthy, health)
        sat = gnsstk.NavSatelliteID(2, 2, gnsstk.SatelliteSystem.GPS,
                                   gnsstk.CarrierBand.L1,
                                   gnsstk.TrackingCode.CA, gnsstk.NavType.GPSLNAV)
        ct = gnsstk.CivilTime(2015,7,19,2,0,0,
                             gnsstk.TimeSystem.GPS).toCommonTime()
        rv,health = navLib.getHealth(sat, ct, gnsstk.SVHealth.Any)
        self.assertEqual(True, rv)
        self.assertEqual(gnsstk.SVHealth.Healthy, health)

    def test_getOffset(self):
        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.RinexNavDataFactory()
        navLib.addFactory(ndf)
        ndf.addDataSource(args.input_dir+
                           '/test_input_rinex_nav_RinexNavExample.99n')
        self.assertEqual(9, ndf.size())
        ct = gnsstk.GPSWeekSecond(1025, 410500).toCommonTime()
        # test getOffset with NavDataPtr
        rv,nd = navLib.getOffset(gnsstk.TimeSystem.GPS, gnsstk.TimeSystem.UTC, ct)
        # time range of ct is going to fail to find the time offset
        self.assertEqual(False, rv)
        ct2 = gnsstk.GPSWeekSecond(1025,552970).toCommonTime()
        rv,nd = navLib.getOffset(gnsstk.TimeSystem.GPS,gnsstk.TimeSystem.UTC,ct2)
        self.assertEqual(True, rv)
        self.assertEqual(13.0, nd.deltatLS)

    def test_find(self):
        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.RinexNavDataFactory()
        navLib.addFactory(ndf)
        ndf.addDataSource(args.input_dir+'/arlm2000.15n')
        sat = gnsstk.NavSatelliteID(10, 10, gnsstk.SatelliteSystem.GPS,
                                   gnsstk.CarrierBand.L1,
                                   gnsstk.TrackingCode.CA, gnsstk.NavType.GPSLNAV)
        ct = gnsstk.CivilTime(2015,7,19,2,0,35,
                             gnsstk.TimeSystem.GPS).toCommonTime()
        nmide = gnsstk.NavMessageID(sat, gnsstk.NavMessageType.Ephemeris)
        nmida = gnsstk.NavMessageID(sat, gnsstk.NavMessageType.Almanac)
        rv,nd = navLib.find(nmide, ct, gnsstk.SVHealth.Any,
                            gnsstk.NavValidityType.ValidOnly,
                            gnsstk.NavSearchOrder.User)
        self.assertEqual(False, rv)
        ct = gnsstk.CivilTime(2015,7,19,12,35,48,
                             gnsstk.TimeSystem.GPS).toCommonTime()
        rv,nd = navLib.find(nmide, ct, gnsstk.SVHealth.Any,
                            gnsstk.NavValidityType.ValidOnly,
                            gnsstk.NavSearchOrder.User)
        self.assertEqual(True, rv)
        self.assertEqual(64, nd.iode)
        rv,nd = navLib.find(nmida, ct, gnsstk.SVHealth.Any,
                            gnsstk.NavValidityType.ValidOnly,
                            gnsstk.NavSearchOrder.User)
        self.assertEqual(False, rv)
        sat = gnsstk.NavSatelliteID(2, 2, gnsstk.SatelliteSystem.GPS,
                                   gnsstk.CarrierBand.L1,
                                   gnsstk.TrackingCode.CA, gnsstk.NavType.GPSLNAV)
        nmide = gnsstk.NavMessageID(sat, gnsstk.NavMessageType.Ephemeris)
        ct = gnsstk.CivilTime(2015,7,19,2,0,0,
                             gnsstk.TimeSystem.GPS).toCommonTime()
        rv,nd = navLib.find(nmide, ct, gnsstk.SVHealth.Any,
                            gnsstk.NavValidityType.ValidOnly,
                            gnsstk.NavSearchOrder.User)
        self.assertEqual(True, rv)
        self.assertEqual(7, nd.iode)

    def test_setValidityFilter(self):
        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.RinexNavDataFactory()
        navLib.addFactory(ndf)
        # rudimentary test of setValidityFilter, just make sure it doesn't die
        navLib.setValidityFilter(gnsstk.NavValidityType.ValidOnly)

    def test_setTypeFilter(self):
        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.RinexNavDataFactory()
        navLib.addFactory(ndf)
        # rudimentary test of setTypeFilter
        nmts = gnsstk.NavMessageTypeSet()
        nmts.add(gnsstk.NavMessageType.Almanac)
        navLib.setTypeFilter(nmts)
        nmts = ndf.getTypeFilter()
        self.assertEqual(1, len(nmts))

        # we can test addTypeFilter and clearTypeFilter up to a point, we can't
        # actually verify the contents without causing the same sort of errors
        # experienced by setTypeFilter() (meaning, the set of unknown objects)
    def test_addTypeFilter(self):
        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.RinexNavDataFactory()
        navLib.addFactory(ndf)
        navLib.clearTypeFilter();
        nmts = ndf.getTypeFilter()
        self.assertEqual(0, len(nmts))
        navLib.addTypeFilter(gnsstk.NavMessageType.Almanac)
        nmts = ndf.getTypeFilter()
        self.assertFalse(len(nmts) == 0)
        # The above filter should result in an empty store (no alms in RINEX)
        ndf.addDataSource(args.input_dir+'/arlm2000.15n')
        satset = navLib.getAvailableSats(gnsstk.CommonTime.BEGINNING_OF_TIME,
                                         gnsstk.CommonTime.END_OF_TIME)
        self.assertEqual(0, len(satset))
        # Try again, this time with only ephemerides, which RINEX has.
        navLib.clear()
        navLib.clearTypeFilter()
        navLib.addTypeFilter(gnsstk.NavMessageType.Ephemeris)
        ndf.addDataSource(args.input_dir+'/arlm2000.15n')
        satset = navLib.getAvailableSats(gnsstk.CommonTime.BEGINNING_OF_TIME,
                                         gnsstk.CommonTime.END_OF_TIME)
        self.assertEqual(31, len(satset))

    def test_getTime(self):
        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.RinexNavDataFactory()
        navLib.addFactory(ndf)
        ndf.addDataSource(args.input_dir+'/arlm2000.15n')
        expIni = gnsstk.CivilTime(2015,7,19,0,0,0,
                                 gnsstk.TimeSystem.GPS).toCommonTime()
        expFin = gnsstk.CivilTime(2015,7,20,2,0,0,
                                 gnsstk.TimeSystem.GPS).toCommonTime()
        self.assertEqual(expIni, navLib.getInitialTime())
        self.assertEqual(expFin, navLib.getFinalTime())

    def test_editClear(self):
        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.RinexNavDataFactory()
        navLib.addFactory(ndf)
        ndf.addDataSource(args.input_dir+'/arlm2000.15n')
        self.assertEqual(507, ndf.size())
        navLib.clear()
        self.assertEqual(0, ndf.size())
        ndf.addDataSource(args.input_dir+'/arlm2000.15n')
        self.assertEqual(507, ndf.size())
        navLib.edit(gnsstk.GPSWeekSecond(0,0).toCommonTime(),
                    gnsstk.GPSWeekSecond(1854,28700).toCommonTime())
        self.assertEqual(343, ndf.size())
        sat = gnsstk.NavSatelliteID(1, 1, gnsstk.SatelliteSystem.GPS,
                                   gnsstk.CarrierBand.L1,
                                   gnsstk.TrackingCode.CA, gnsstk.NavType.GPSLNAV)
        navLib.clear()
        self.assertEqual(0, ndf.size())
        ndf.addDataSource(args.input_dir+'/arlm2000.15n')
        self.assertEqual(507, ndf.size())
        navLib.edit(gnsstk.GPSWeekSecond(0,0).toCommonTime(),
                    gnsstk.GPSWeekSecond(1854,28700).toCommonTime(), sat)
        self.assertEqual(504, ndf.size())
        navLib.clear()
        self.assertEqual(0, ndf.size())
        ndf.addDataSource(args.input_dir+'/arlm2000.15n')
        self.assertEqual(507, ndf.size())
        sig = gnsstk.NavSignalID(gnsstk.SatelliteSystem.GPS, gnsstk.CarrierBand.L1,
                                gnsstk.TrackingCode.Y, gnsstk.NavType.GPSLNAV)
        navLib.edit(gnsstk.GPSWeekSecond(0,0).toCommonTime(),
                    gnsstk.GPSWeekSecond(1854,28700).toCommonTime(), sig)
        self.assertEqual(507, ndf.size())
        sig = gnsstk.NavSignalID(gnsstk.SatelliteSystem.GPS, gnsstk.CarrierBand.L1,
                                gnsstk.TrackingCode.CA, gnsstk.NavType.GPSLNAV)
        navLib.edit(gnsstk.GPSWeekSecond(0,0).toCommonTime(),
                    gnsstk.GPSWeekSecond(1854,28700).toCommonTime(), sig)
        self.assertEqual(343, ndf.size())

    def test_getAvailableSats(self):
        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.RinexNavDataFactory()
        navLib.addFactory(ndf)
        ndf.addDataSource(args.input_dir+'/arlm2000.15n')
        satid = gnsstk.SatID(23,gnsstk.SatelliteSystem.GPS)
        sat1 = gnsstk.NavSatelliteID(satid)
        satset = navLib.getAvailableSats(gnsstk.CommonTime.BEGINNING_OF_TIME,
                                         gnsstk.CommonTime.END_OF_TIME)
        self.assertEqual(32, len(satset))
        t1 = gnsstk.CivilTime(2020,4,12,0,56,0,gnsstk.TimeSystem.GPS).toCommonTime()
        t2 = gnsstk.CivilTime(2020,4,12,0,57,0,gnsstk.TimeSystem.GPS).toCommonTime()
        satset = navLib.getAvailableSats(t1, t2)
        self.assertEqual(0, len(satset))

    def test_isPresent(self):
        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.RinexNavDataFactory()
        navLib.addFactory(ndf)
        ndf.addDataSource(args.input_dir+'/arlm2000.15n')
        satid = gnsstk.SatID(23,gnsstk.SatelliteSystem.GPS)
        sat1 = gnsstk.NavSatelliteID(satid)
        nmid1e = gnsstk.NavMessageID(sat1, gnsstk.NavMessageType.Ephemeris)
        nmid1a = gnsstk.NavMessageID(sat1, gnsstk.NavMessageType.Almanac)
        t1 = gnsstk.CivilTime(2015,7,19,10,0,0,gnsstk.TimeSystem.GPS).toCommonTime()
        t2 = gnsstk.CivilTime(2015,7,19,11,0,0,gnsstk.TimeSystem.GPS).toCommonTime()
        rv = navLib.isPresent(nmid1e, t1, t2)
        self.assertEqual(True, rv)
        rv = navLib.isPresent(nmid1a, t1, t2)
        self.assertEqual(False, rv)

    def test_isTypePresent(self):
        navLib = gnsstk.NavLibrary()
        ndf = gnsstk.RinexNavDataFactory()
        navLib.addFactory(ndf)
        ndf.addDataSource(args.input_dir+'/arlm2000.15n')
        satid = gnsstk.SatID(23,gnsstk.SatelliteSystem.GPS)
        sat1 = gnsstk.NavSatelliteID(satid)
        t1 = gnsstk.CivilTime(2015,7,19,10,0,0,gnsstk.TimeSystem.GPS).toCommonTime()
        t2 = gnsstk.CivilTime(2015,7,19,11,0,0,gnsstk.TimeSystem.GPS).toCommonTime()
        rv = navLib.isTypePresent(gnsstk.NavMessageType.Ephemeris, sat1, t1, t2)
        self.assertEqual(True, rv)
        rv = navLib.isTypePresent(gnsstk.NavMessageType.Almanac, sat1, t1, t2)
        self.assertEqual(False, rv)

if __name__ == '__main__':
    run_unit_tests()
