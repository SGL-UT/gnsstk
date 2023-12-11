//==============================================================================
//
//  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
//
//==============================================================================


//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin, under contract to an agency or agencies
//  within the U.S. Department of Defense. The U.S. Government retains all
//  rights to use, duplicate, distribute, disclose, or release this software.
//
//  Pursuant to DoD Directive 523024
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public
//                            release, distribution is unlimited.
//
//==============================================================================
#include "NavLibrary.hpp"
#include "RinexNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavHealth.hpp"
#include "GPSLNavTimeOffset.hpp"
#include "TimeString.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gnsstk::SVHealth e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s,
                            const gnsstk::NavMessageTypeSet& nmts)
   {
      s << "{";
      for (const auto& i : nmts)
      {
         s << " " << gnsstk::StringUtils::asString(i);
      }
      s << " }";
      return s;
   }
}

/// Class to expose NavLibrary::factories rather than making a friend.
class TestClass : public gnsstk::NavLibrary
{
public:
   gnsstk::NavDataFactoryMap& getFactories()
   { return factories; }
};


/// Fake factory used for testing addFactory
class TestFactory : public gnsstk::NavDataFactoryWithStore
{
public:
   TestFactory()
   {
      supportedSignals.insert(gnsstk::NavSignalID(gnsstk::SatelliteSystem::GPS,
                                                 gnsstk::CarrierBand::L1,
                                                 gnsstk::TrackingCode::CA,
                                                 gnsstk::NavType::GPSLNAV));
      supportedSignals.insert(gnsstk::NavSignalID(gnsstk::SatelliteSystem::GPS,
                                                 gnsstk::CarrierBand::L1,
                                                 gnsstk::TrackingCode::P,
                                                 gnsstk::NavType::GPSLNAV));
      supportedSignals.insert(gnsstk::NavSignalID(gnsstk::SatelliteSystem::GPS,
                                                 gnsstk::CarrierBand::L1,
                                                 gnsstk::TrackingCode::Y,
                                                 gnsstk::NavType::GPSLNAV));
      supportedSignals.insert(gnsstk::NavSignalID(gnsstk::SatelliteSystem::GPS,
                                                 gnsstk::CarrierBand::L2,
                                                 gnsstk::TrackingCode::P,
                                                 gnsstk::NavType::GPSLNAV));
      supportedSignals.insert(gnsstk::NavSignalID(gnsstk::SatelliteSystem::GPS,
                                                 gnsstk::CarrierBand::L2,
                                                 gnsstk::TrackingCode::Y,
                                                 gnsstk::NavType::GPSLNAV));
   }
   gnsstk::NavValidityType getValidityFilter() const
   { return navValidity; }
   gnsstk::NavMessageTypeSet getTypeFilter() const
   { return procNavTypes; }
   bool addDataSource(const std::string& source) override
   { return false; }
   std::string getFactoryFormats() const override
   { return "BUNK"; }
};

class RinexTestFactory : public gnsstk::RinexNavDataFactory
{
public:
   gnsstk::NavValidityType getValidityFilter() const
   { return navValidity; }
   gnsstk::NavMessageTypeSet getTypeFilter() const
   { return procNavTypes; }
};


/// Automated tests for gnsstk::NavLibrary
class NavLibrary_T
{
public:
   NavLibrary_T();

      /** Make sure that NavLibrary::getXvt pulls the correct
       * ephemeris and computes the correct xvt. */
   unsigned getXvtTest();
   unsigned getHealthTest();
   unsigned getOffsetTest();
   unsigned findTest();
      /** Make sure NavLibrary::setValidityFilter updates navValidity
       * in all factories. */
   unsigned setValidityFilterTest();
      /** Make sure NavLibrary::setTypeFilter updates getTypeFilter() in
       * all factories. */
   unsigned setTypeFilterTest();
      /** Make sure NavLibrary::addTypeFilter and clearTypeFilter
       * updates getTypeFilter() in all factories. */
   unsigned addTypeFilterTest();
      /** Make sure addFactory() puts the factory in each supported
       * signal in the factories map. */
   unsigned addFactoryTest();
      /// Test getInitialTime() and getFinalTime()
   unsigned getTimeTest();
   unsigned getAvailableSatsTest();
   unsigned isPresentTest();
   unsigned getIonoCorrTest();
   unsigned getISCTest();

   gnsstk::CivilTime civ;
   gnsstk::CommonTime ct;
};


NavLibrary_T ::
NavLibrary_T()
      : civ(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS),
        ct(civ)
{
}


unsigned NavLibrary_T ::
getXvtTest()
{
   TUDEF("NavLibraryRinex", "getXvt");
   gnsstk::Xvt xvt;
   gnsstk::NavLibrary navLib;
   gnsstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "arlm2000.15n";
   TUCATCH(navLib.addFactory(ndfp));
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   gnsstk::NavSatelliteID sat(5, 5, gnsstk::SatelliteSystem::GPS,
                             gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                             gnsstk::NavType::GPSLNAV);
   TUASSERT(navLib.getXvt(sat, ct+35, xvt, gnsstk::SVHealth::Any));
      // TUASSERTE is not good for this check as we're testing a bunch
      // of floating point numbers, so we use TUSSERTFE instead for
      // each field.
      // @note These were checked against results provided by wheresat
      // while it was still using OrbElemStore.
   TUASSERTFE(  9345531.5274733770639, xvt.x[0]);
   TUASSERTFE(-12408177.088141856715,  xvt.x[1]);
   TUASSERTFE( 21486320.848036296666,  xvt.x[2]);
   TUASSERTFE(2081.276961058104007,    xvt.v[0]);
   TUASSERTFE(1792.4445008638492709,   xvt.v[1]);
   TUASSERTFE( 148.29209115082824155,  xvt.v[2]);
   TUASSERTFE(-0.00021641018042870913346, xvt.clkbias);
   TUASSERTFE(4.3200998334200003381e-12, xvt.clkdrift);
   TUASSERTFE(-8.8197758101551758427e-09, xvt.relcorr);

   TUASSERTE(gnsstk::Xvt::HealthStatus, gnsstk::Xvt::Healthy, xvt.health);
   TURETURN();
}


unsigned NavLibrary_T ::
getHealthTest()
{
   TUDEF("NavLibraryRinex", "getHealth");
   gnsstk::SVHealth health;
   gnsstk::NavLibrary navLib;
   gnsstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "arlm2000.15n";
   TUCATCH(navLib.addFactory(ndfp));
   RinexTestFactory *rndfp = dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   gnsstk::NavSatelliteID sat(10, 10, gnsstk::SatelliteSystem::GPS,
                             gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                             gnsstk::NavType::GPSLNAV);
      // shouldn't have data at this time
   TUASSERT(!navLib.getHealth(sat, ct+35, health, gnsstk::SVHealth::Any));
      // shouldn't have data at this time, either
      // Remember that the first line of a RINEX 2 nav like we use in
      // this test is the Toc, NOT the transmit time.
   gnsstk::CivilTime civ2(2015,7,19,12,35,35.0,gnsstk::TimeSystem::GPS);
   TUASSERT(!navLib.getHealth(sat, civ2, health, gnsstk::SVHealth::Any));
      // should have data at this time, and it should be unhealthy
   gnsstk::CivilTime civ3(2015,7,19,12,35,36.0,gnsstk::TimeSystem::GPS);
   TUASSERT(navLib.getHealth(sat, civ3, health, gnsstk::SVHealth::Any));
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, health);
      // should have this data and healthy
   gnsstk::NavSatelliteID sat4(2, 2, gnsstk::SatelliteSystem::GPS,
                              gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                              gnsstk::NavType::GPSLNAV);
   gnsstk::CivilTime civ4(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS);
   TUASSERT(navLib.getHealth(sat4, civ4, health, gnsstk::SVHealth::Any));
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, health);
   TURETURN();
}


unsigned NavLibrary_T ::
getOffsetTest()
{
   TUDEF("NavLibraryRinex", "getOffset");
   gnsstk::NavLibrary navLib;
   gnsstk::NavDataFactoryPtr ndfp(std::make_shared<TestFactory>());
   TestFactory *fact1 = dynamic_cast<TestFactory*>(ndfp.get());
   gnsstk::NavDataPtr navOut = std::make_shared<gnsstk::GPSLNavTimeOffset>();
   navOut->timeStamp = ct;
   navOut->signal.messageType = gnsstk::NavMessageType::TimeOffset;
   gnsstk::GPSLNavTimeOffset *toptr = dynamic_cast<gnsstk::GPSLNavTimeOffset*>(
      navOut.get());
   navOut->signal.system = gnsstk::SatelliteSystem::GPS;
   navOut->signal.obs.band = gnsstk::CarrierBand::L1;
   navOut->signal.obs.code = gnsstk::TrackingCode::CA;
   navOut->signal.nav = gnsstk::NavType::GPSLNAV;
   navOut->signal.sat = gnsstk::SatID(23,gnsstk::SatelliteSystem::GPS);
   navOut->signal.xmitSat = gnsstk::SatID(32,gnsstk::SatelliteSystem::GPS);
   toptr->deltatLS = 23; // set a simple, easy to verify value.
   toptr->refTime = ct;
   TUASSERT(fact1->addNavData(navOut));
   TUCATCH(navLib.addFactory(ndfp));
   double result;
   TUASSERT(navLib.getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC,
                             ct+35, result, gnsstk::SVHealth::Any,
                             gnsstk::NavValidityType::Any));
   TUASSERTFE(23.0, result);
      // reverse the conversion and expect negative.
   gnsstk::CommonTime utc35(ct+35);
   utc35.setTimeSystem(gnsstk::TimeSystem::UTC);
   TUASSERT(navLib.getOffset(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::GPS,
                             utc35, result, gnsstk::SVHealth::Any,
                             gnsstk::NavValidityType::Any));
   TUASSERTFE(-23.0, result);
      // expect this to not work
   TUASSERT(!navLib.getOffset(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::BDT,
                              utc35, result, gnsstk::SVHealth::Any,
                              gnsstk::NavValidityType::Any));
   TURETURN();
}


unsigned NavLibrary_T ::
findTest()
{
   TUDEF("NavLibraryRinex", "find");
   gnsstk::NavLibrary navLib;
   gnsstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "arlm2000.15n";
   gnsstk::NavDataPtr ndp;
   TUCATCH(navLib.addFactory(ndfp));
   RinexTestFactory *rndfp = dynamic_cast<RinexTestFactory*>(ndfp.get());
   gnsstk::GPSLNavEph *eph;
   TUASSERT(rndfp->addDataSource(fname));
   gnsstk::NavSatelliteID sat(10, 10, gnsstk::SatelliteSystem::GPS,
                             gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                             gnsstk::NavType::GPSLNAV);
   gnsstk::NavMessageID nmide(sat, gnsstk::NavMessageType::Ephemeris);
   gnsstk::NavMessageID nmida(sat, gnsstk::NavMessageType::Almanac);
      // shouldn't have data at this time
   TUASSERT(!navLib.find(nmide, ct+35, ndp, gnsstk::SVHealth::Any,
                         gnsstk::NavValidityType::ValidOnly,
                         gnsstk::NavSearchOrder::User));
      // shouldn't have data at this time, either
      // Remember that the first line of a RINEX 2 nav like we use in
      // this test is the Toc, NOT the transmit time.
   gnsstk::CivilTime civ2(2015,7,19,12,35,35.0,gnsstk::TimeSystem::GPS);
   TUASSERT(!navLib.find(nmide, civ2, ndp, gnsstk::SVHealth::Any,
                         gnsstk::NavValidityType::ValidOnly,
                         gnsstk::NavSearchOrder::User));
      // should have data at this time
   gnsstk::CivilTime civ3(2015,7,19,12,35,48.0,gnsstk::TimeSystem::GPS);
   TUASSERT(navLib.find(nmide, civ3, ndp, gnsstk::SVHealth::Any,
                        gnsstk::NavValidityType::ValidOnly,
                        gnsstk::NavSearchOrder::User));
   eph = dynamic_cast<gnsstk::GPSLNavEph*>(ndp.get());
   TUASSERT(eph != nullptr);
   TUASSERTE(uint16_t, 64, eph->iode);

      // shouldn't have almanac data though
   TUASSERT(!navLib.find(nmida, civ3, ndp, gnsstk::SVHealth::Any,
                         gnsstk::NavValidityType::ValidOnly,
                         gnsstk::NavSearchOrder::User));
      // should have this data
   gnsstk::NavSatelliteID sat4(2, 2, gnsstk::SatelliteSystem::GPS,
                              gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                              gnsstk::NavType::GPSLNAV);
   gnsstk::NavMessageID nmide4(sat4, gnsstk::NavMessageType::Ephemeris);
   gnsstk::CivilTime civ4(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS);
   TUASSERT(navLib.find(nmide4, civ4, ndp, gnsstk::SVHealth::Any,
                        gnsstk::NavValidityType::ValidOnly,
                        gnsstk::NavSearchOrder::User));
   eph = dynamic_cast<gnsstk::GPSLNavEph*>(ndp.get());
   TUASSERT(eph != nullptr);
   TUASSERTE(uint16_t, 7, eph->iode);
   TURETURN();
}


unsigned NavLibrary_T ::
setValidityFilterTest()
{
   TUDEF("NavLibraryRinex", "setValidityFilter");
   gnsstk::NavLibrary navLib;
   gnsstk::NavDataFactoryPtr ndfp1(std::make_shared<TestFactory>());
   gnsstk::NavDataFactoryPtr
      ndfp2(std::make_shared<RinexTestFactory>());
   TUCATCH(navLib.addFactory(ndfp1));
   TUCATCH(navLib.addFactory(ndfp2));
   TestFactory *tfp = dynamic_cast<TestFactory*>(ndfp1.get());
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp2.get());
   TUASSERT(tfp != nullptr);
   TUASSERT(rndfp != nullptr);
   navLib.setValidityFilter(gnsstk::NavValidityType::ValidOnly);
   TUASSERTE(gnsstk::NavValidityType, gnsstk::NavValidityType::ValidOnly,
             tfp->getValidityFilter());
   TUASSERTE(gnsstk::NavValidityType, gnsstk::NavValidityType::ValidOnly,
             rndfp->getValidityFilter());
   navLib.setValidityFilter(gnsstk::NavValidityType::Any);
   TUASSERTE(gnsstk::NavValidityType, gnsstk::NavValidityType::Any,
             tfp->getValidityFilter());
   TUASSERTE(gnsstk::NavValidityType, gnsstk::NavValidityType::Any,
             rndfp->getValidityFilter());
   TURETURN();
}


unsigned NavLibrary_T ::
setTypeFilterTest()
{
   TUDEF("NavLibraryRinex", "setTypeFilter");
   gnsstk::NavLibrary navLib;
   gnsstk::NavDataFactoryPtr ndfp1(std::make_shared<TestFactory>());
   gnsstk::NavDataFactoryPtr
      ndfp2(std::make_shared<RinexTestFactory>());
   TUCATCH(navLib.addFactory(ndfp1));
   TUCATCH(navLib.addFactory(ndfp2));
   TestFactory *tfp = dynamic_cast<TestFactory*>(ndfp1.get());
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp2.get());
   TUASSERT(tfp != nullptr);
   TUASSERT(rndfp != nullptr);
   gnsstk::NavMessageTypeSet nmts1 { gnsstk::NavMessageType::Unknown };
   gnsstk::NavMessageTypeSet nmts2 { gnsstk::NavMessageType::Ephemeris };
   navLib.setTypeFilter(nmts1);
   TUASSERTE(gnsstk::NavMessageTypeSet, nmts1, tfp->getTypeFilter());
   TUASSERTE(gnsstk::NavMessageTypeSet, nmts1, rndfp->getTypeFilter());
   navLib.setTypeFilter(nmts2);
   TUASSERTE(gnsstk::NavMessageTypeSet, nmts2, tfp->getTypeFilter());
   TUASSERTE(gnsstk::NavMessageTypeSet, nmts2, rndfp->getTypeFilter());
   navLib.setTypeFilter(gnsstk::allNavMessageTypes);
   TUASSERTE(gnsstk::NavMessageTypeSet, gnsstk::allNavMessageTypes,
             tfp->getTypeFilter());
   TUASSERTE(gnsstk::NavMessageTypeSet, gnsstk::allNavMessageTypes,
             rndfp->getTypeFilter());
   TURETURN();
}


unsigned NavLibrary_T ::
addTypeFilterTest()
{
   TUDEF("NavLibraryRinex", "clearTypeFilter");
   gnsstk::NavLibrary navLib;
   gnsstk::NavDataFactoryPtr ndfp1(std::make_shared<TestFactory>());
   gnsstk::NavDataFactoryPtr
      ndfp2(std::make_shared<RinexTestFactory>());
   TUCATCH(navLib.addFactory(ndfp1));
   TUCATCH(navLib.addFactory(ndfp2));
   TestFactory *tfp = dynamic_cast<TestFactory*>(ndfp1.get());
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp2.get());
   TUASSERT(tfp != nullptr);
   TUASSERT(rndfp != nullptr);
   gnsstk::NavMessageTypeSet nmts1 { gnsstk::NavMessageType::Unknown };
   gnsstk::NavMessageTypeSet nmts2 { gnsstk::NavMessageType::Ephemeris };
   TUCATCH(navLib.clearTypeFilter());
   TUASSERT(tfp->getTypeFilter().empty());
   TUASSERT(rndfp->getTypeFilter().empty());
   TUCSM("addTypeFilter");
   TUCATCH(navLib.addTypeFilter(gnsstk::NavMessageType::Unknown));
   TUASSERTE(gnsstk::NavMessageTypeSet, nmts1, tfp->getTypeFilter());
   TUASSERTE(gnsstk::NavMessageTypeSet, nmts1, rndfp->getTypeFilter());
   TUCATCH(navLib.clearTypeFilter());
   TUCATCH(navLib.addTypeFilter(gnsstk::NavMessageType::Ephemeris));
   TUASSERTE(gnsstk::NavMessageTypeSet, nmts2, tfp->getTypeFilter());
   TUASSERTE(gnsstk::NavMessageTypeSet, nmts2, rndfp->getTypeFilter());
      // Set type type filter back to all so that other tests work.
   navLib.setTypeFilter(gnsstk::allNavMessageTypes);
   TUASSERTE(gnsstk::NavMessageTypeSet, gnsstk::allNavMessageTypes,
             tfp->getTypeFilter());
   TUASSERTE(gnsstk::NavMessageTypeSet, gnsstk::allNavMessageTypes,
             rndfp->getTypeFilter());
   TURETURN();
}


unsigned NavLibrary_T ::
addFactoryTest()
{
   TUDEF("NavLibraryRinex", "addFactory");
   TestClass navLib;
   gnsstk::NavDataFactoryPtr ndfp1(std::make_shared<TestFactory>());
   gnsstk::NavDataFactoryPtr
      ndfp2(std::make_shared<RinexTestFactory>());
   TUCATCH(navLib.addFactory(ndfp1));
   TUCATCH(navLib.addFactory(ndfp2));
   TestFactory *tfp = dynamic_cast<TestFactory*>(ndfp1.get());
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp2.get());
   TUASSERT(tfp != nullptr);
   TUASSERT(rndfp != nullptr);
   gnsstk::NavDataFactoryMap &fm(navLib.getFactories());
      // make sure the factories map has the same number of items
   size_t expTotal = tfp->supportedSignals.size() +
      rndfp->supportedSignals.size();
   TUASSERTE(size_t, expTotal, fm.size());
      // make sure the map has the same set of signals
   gnsstk::NavSignalSet allSignals;
   std::set_union(tfp->supportedSignals.begin(), tfp->supportedSignals.end(),
                  rndfp->supportedSignals.begin(),rndfp->supportedSignals.end(),
                  std::inserter(allSignals, allSignals.end()));
   for (const auto& fi : fm)
   {
      TUASSERTE(size_t, 1, allSignals.count(fi.first));
   }

   TURETURN();
}


unsigned NavLibrary_T ::
getTimeTest()
{
   TUDEF("NavLibraryRinex", "getInitialTime");
   gnsstk::NavLibrary navLib;
   gnsstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "arlm2000.15n";
   TUCATCH(navLib.addFactory(ndfp));
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   TUASSERTE(gnsstk::CommonTime,
             gnsstk::CivilTime(2015,7,19,0,0,0.0,
                              gnsstk::TimeSystem::GPS).convertToCommonTime(),
             navLib.getInitialTime());
   TUASSERTE(gnsstk::CommonTime,
             gnsstk::CivilTime(2015,7,20,2,0,0.0,
                              gnsstk::TimeSystem::GPS).convertToCommonTime(),
             navLib.getFinalTime());
   TURETURN();
}


unsigned NavLibrary_T ::
getAvailableSatsTest()
{
   TUDEF("NavLibraryRinex", "getAvailableSats");
   gnsstk::NavLibrary uut;
   gnsstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "arlm2000.15n";
   gnsstk::NavSatelliteIDSet satset;
   TUCATCH(uut.addFactory(ndfp));
   RinexTestFactory *rndfp = dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
      // really basic tests, the real tests are in NavDataFactoryWithStore_T etc
   TUCATCH(satset = uut.getAvailableSats(
              gnsstk::CommonTime::BEGINNING_OF_TIME,
              gnsstk::CommonTime::END_OF_TIME));
      // We're loading RINEX, so the 33rd "satellite" is really a
      // dummy value because RINEX gives no signal source for any of
      // the time offset data it contains.
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 33, satset.size());
   TUCATCH(satset = uut.getAvailableSats(
              gnsstk::CivilTime(2020,4,12,0,56,0,gnsstk::TimeSystem::GPS),
              gnsstk::CivilTime(2020,4,12,0,57,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, satset.empty());
   TURETURN();
}


unsigned NavLibrary_T ::
isPresentTest()
{
   TUDEF("NavLibraryRinex", "isPresent");
   gnsstk::NavLibrary uut;
   gnsstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "arlm2000.15n";
      // really basic tests, the real tests are in NavDataFactoryWithStore_T etc
   TUCATCH(uut.addFactory(ndfp));
   RinexTestFactory *rndfp = dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   gnsstk::NavSatelliteID sat1(gnsstk::SatID(23,gnsstk::SatelliteSystem::GPS));
   gnsstk::NavMessageID nmid1e(sat1, gnsstk::NavMessageType::Ephemeris),
      nmid1a(sat1, gnsstk::NavMessageType::Almanac);
   gnsstk::CivilTime t1(2015,7,19,10,0,0,gnsstk::TimeSystem::GPS);
   gnsstk::CivilTime t2(2015,7,19,11,0,0,gnsstk::TimeSystem::GPS);
   // uut.dump(std::cerr, gnsstk::DumpDetail::Brief);
   TUASSERTE(bool, true, uut.isPresent(nmid1e, t1, t2));
   TUASSERTE(bool, false, uut.isPresent(nmid1a, t1, t2));
   TUCSM("isTypePresent");
   TUASSERTE(bool, true, uut.isTypePresent(
                gnsstk::NavMessageType::Ephemeris, sat1, t1, t2));
   TUASSERTE(bool, false, uut.isTypePresent(
                gnsstk::NavMessageType::Almanac, sat1, t1, t2));
   TURETURN();
}


unsigned NavLibrary_T ::
getIonoCorrTest()
{
   TUDEF("NavLibraryRinex", "getIonoCorr");
   gnsstk::NavLibrary uut;
   gnsstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "arlm2000.15n";
   gnsstk::CommonTime when = gnsstk::CivilTime(2015,7,19,10,0,0,
                                             gnsstk::TimeSystem::GPS);
   gnsstk::Position rx, sv;
   static const double expCorr = 2.3429392704808575942;
   static const double corrEps = 1e-12; // 1 picometer ought to be adequate.
   double corr = 0.0;
   rx.setECEF( -1575232.0141, -4707872.2332,  3993198.4383);
   sv.setECEF(-22188225.701295968145, -12374229.731898581609,
              8029748.4487511720508);
   TUCATCH(uut.addFactory(ndfp));
   RinexTestFactory *rndfp = dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
      // test both entry points and make sure they yield the same
      // (w/in 1 picometer) results.
   TUASSERTE(bool, true, uut.getIonoCorr(
                gnsstk::SatID(1, gnsstk::SatelliteSystem::GPS), when, rx,
                gnsstk::CarrierBand::L1, corr, gnsstk::NavType::GPSLNAV));
   TUASSERTFEPS(expCorr, corr, corrEps);
   corr = 0.0;
   TUASSERTE(bool, true, uut.getIonoCorr(
                gnsstk::SatelliteSystem::GPS, when, rx, sv,
                gnsstk::CarrierBand::L1, corr, gnsstk::NavType::GPSLNAV));
   TUASSERTFEPS(expCorr, corr, corrEps);
   TURETURN();
}


unsigned NavLibrary_T ::
getISCTest()
{
   TUDEF("NavLibraryRinex", "getISC");
   gnsstk::NavLibrary uut;
   gnsstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "arlm2000.15n";
   const gnsstk::SatID sat(1, gnsstk::SatelliteSystem::GPS);
   const gnsstk::CommonTime when = gnsstk::CivilTime(2015,7,19,10,0,0,
                                                   gnsstk::TimeSystem::GPS);
   const gnsstk::ObsID oid(gnsstk::ObservationType::Phase, gnsstk::CarrierBand::L2,
                          gnsstk::TrackingCode::Y);
   const gnsstk::ObsID woid1(gnsstk::ObservationType::Phase,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L2CL);
   const gnsstk::ObsID woid2(gnsstk::ObservationType::Phase,
                            gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L1CP);
   static const double expCorr = -8.4361009713756425465e-09;
   double corr = 0.0;
   TUCATCH(uut.addFactory(ndfp));
   RinexTestFactory *rndfp = dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   TUASSERTE(bool, true, uut.getISC(sat, oid, when, corr));
   TUASSERTFE(expCorr, corr);
      // check wonky obs ID
   TUASSERTE(bool, false, uut.getISC(sat, woid1, when, corr));
   TURETURN();
}


int main()
{
   NavLibrary_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.getXvtTest();
   errorTotal += testClass.getHealthTest();
   errorTotal += testClass.getOffsetTest();
   errorTotal += testClass.findTest();
   errorTotal += testClass.setValidityFilterTest();
   errorTotal += testClass.setTypeFilterTest();
   errorTotal += testClass.addTypeFilterTest();
   errorTotal += testClass.addFactoryTest();
   errorTotal += testClass.getTimeTest();
   errorTotal += testClass.getAvailableSatsTest();
   errorTotal += testClass.isPresentTest();
   errorTotal += testClass.getIonoCorrTest();
   errorTotal += testClass.getISCTest();
      /// @todo test edit(), clear()
   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
