//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//  
//  This software was developed by Applied Research Laboratories at the 
//  University of Texas at Austin.
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
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

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::SVHealth e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s,
                            const gpstk::NavMessageTypeSet& nmts)
   {
      s << "{";
      for (const auto& i : nmts)
      {
         s << " " << gpstk::StringUtils::asString(i);
      }
      s << " }";
      return s;
   }
}

/// Class to expose NavLibrary::factories rather than making a friend.
class TestClass : public gpstk::NavLibrary
{
public:
   gpstk::NavDataFactoryMap& getFactories()
   { return factories; }
};


/// Fake factory used for testing addFactory
class TestFactory : public gpstk::NavDataFactoryWithStore
{
public:
   TestFactory()
   {
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L1,
                                                 gpstk::TrackingCode::CA,
                                                 gpstk::NavType::GPSLNAV));
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L1,
                                                 gpstk::TrackingCode::P,
                                                 gpstk::NavType::GPSLNAV));
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L1,
                                                 gpstk::TrackingCode::Y,
                                                 gpstk::NavType::GPSLNAV));
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L2,
                                                 gpstk::TrackingCode::P,
                                                 gpstk::NavType::GPSLNAV));
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L2,
                                                 gpstk::TrackingCode::Y,
                                                 gpstk::NavType::GPSLNAV));
   }
   gpstk::NavValidityType getValidityFilter() const
   { return navValidity; }
   gpstk::NavMessageTypeSet getTypeFilter() const
   { return procNavTypes; }
   bool addDataSource(const std::string& source) override
   { return false; }
   std::string getFactoryFormats() const override
   { return "BUNK"; }
};

class RinexTestFactory : public gpstk::RinexNavDataFactory
{
public:
   gpstk::NavValidityType getValidityFilter() const
   { return navValidity; }
   gpstk::NavMessageTypeSet getTypeFilter() const
   { return procNavTypes; }
};


/// Automated tests for gpstk::NavLibrary
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
      /** Make sure addFactory() puts the factory in each supported
       * signal in the factories map. */
   unsigned addFactoryTest();
      /// Test getInitialTime() and getFinalTime()
   unsigned getTimeTest();
   unsigned getAvailableSatsTest();
   unsigned isPresentTest();

   gpstk::CivilTime civ;
   gpstk::CommonTime ct;
};


NavLibrary_T ::
NavLibrary_T()
      : civ(2015,7,19,2,0,0.0,gpstk::TimeSystem::GPS),
        ct(civ)
{
}


unsigned NavLibrary_T ::
getXvtTest()
{
   TUDEF("NavLibrary", "getXvt");
   gpstk::Xvt xvt;
   gpstk::NavLibrary navLib;
   gpstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "arlm2000.15n";
   TUCATCH(navLib.addFactory(ndfp));
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   gpstk::NavSatelliteID sat(5, 5, gpstk::SatelliteSystem::GPS,
                             gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                             gpstk::NavType::GPSLNAV);
   TUASSERT(navLib.getXvt(sat, ct+35, xvt, gpstk::SVHealth::Any));
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

   TUASSERTE(gpstk::Xvt::HealthStatus, gpstk::Xvt::Healthy, xvt.health);
   TURETURN();
}


unsigned NavLibrary_T ::
getHealthTest()
{
   TUDEF("NavLibrary", "getHealth");
   gpstk::SVHealth health;
   gpstk::NavLibrary navLib;
   gpstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "arlm2000.15n";
   TUCATCH(navLib.addFactory(ndfp));
   RinexTestFactory *rndfp = dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   gpstk::NavSatelliteID sat(10, 10, gpstk::SatelliteSystem::GPS,
                             gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                             gpstk::NavType::GPSLNAV);
      // shouldn't have data at this time
   TUASSERT(!navLib.getHealth(sat, ct+35, health, gpstk::SVHealth::Any));
      // shouldn't have data at this time, either
      // Remember that the first line of a RINEX 2 nav like we use in
      // this test is the Toc, NOT the transmit time.
   gpstk::CivilTime civ2(2015,7,19,12,35,35.0,gpstk::TimeSystem::GPS);
   TUASSERT(!navLib.getHealth(sat, civ2, health, gpstk::SVHealth::Any));
      // should have data at this time, and it should be unhealthy
   gpstk::CivilTime civ3(2015,7,19,12,35,36.0,gpstk::TimeSystem::GPS);
   TUASSERT(navLib.getHealth(sat, civ3, health, gpstk::SVHealth::Any));
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unhealthy, health);
      // should have this data and healthy
   gpstk::NavSatelliteID sat4(2, 2, gpstk::SatelliteSystem::GPS,
                              gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                              gpstk::NavType::GPSLNAV);
   gpstk::CivilTime civ4(2015,7,19,2,0,0.0,gpstk::TimeSystem::GPS);
   TUASSERT(navLib.getHealth(sat4, civ4, health, gpstk::SVHealth::Any));
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, health);
   TURETURN();
}


unsigned NavLibrary_T ::
getOffsetTest()
{
   TUDEF("NavLibrary", "getOffset");
   gpstk::NavLibrary navLib;
   gpstk::NavDataFactoryPtr ndfp(std::make_shared<TestFactory>());
   TestFactory *fact1 = dynamic_cast<TestFactory*>(ndfp.get());
   gpstk::NavDataPtr navOut = std::make_shared<gpstk::GPSLNavTimeOffset>();
   navOut->timeStamp = ct;
   navOut->signal.messageType = gpstk::NavMessageType::TimeOffset;
   gpstk::GPSLNavTimeOffset *toptr = dynamic_cast<gpstk::GPSLNavTimeOffset*>(
      navOut.get());
   navOut->signal.system = gpstk::SatelliteSystem::GPS;
   navOut->signal.obs.band = gpstk::CarrierBand::L1;
   navOut->signal.obs.code = gpstk::TrackingCode::CA;
   navOut->signal.nav = gpstk::NavType::GPSLNAV;
   navOut->signal.sat = gpstk::SatID(23,gpstk::SatelliteSystem::GPS);
   navOut->signal.xmitSat = gpstk::SatID(32,gpstk::SatelliteSystem::GPS);
   toptr->deltatLS = 23; // set a simple, easy to verify value.
   TUASSERT(fact1->addNavData(navOut));
   TUCATCH(navLib.addFactory(ndfp));
   double result;
   TUASSERT(navLib.getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC,
                             ct+35, result, gpstk::SVHealth::Any,
                             gpstk::NavValidityType::Any));
   TUASSERTFE(23.0, result);
      // reverse the conversion and expect negative.
   TUASSERT(navLib.getOffset(gpstk::TimeSystem::UTC, gpstk::TimeSystem::GPS,
                             ct+35, result, gpstk::SVHealth::Any,
                             gpstk::NavValidityType::Any));
   TUASSERTFE(-23.0, result);
      // expect this to not work
   TUASSERT(!navLib.getOffset(gpstk::TimeSystem::UTC, gpstk::TimeSystem::BDT,
                              ct+35, result, gpstk::SVHealth::Any,
                              gpstk::NavValidityType::Any));
   TURETURN();
}


unsigned NavLibrary_T ::
findTest()
{
   TUDEF("NavLibrary", "find");
   gpstk::NavLibrary navLib;
   gpstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "arlm2000.15n";
   gpstk::NavDataPtr ndp;
   TUCATCH(navLib.addFactory(ndfp));
   RinexTestFactory *rndfp = dynamic_cast<RinexTestFactory*>(ndfp.get());
   gpstk::GPSLNavEph *eph;
   TUASSERT(rndfp->addDataSource(fname));
   gpstk::NavSatelliteID sat(10, 10, gpstk::SatelliteSystem::GPS,
                             gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                             gpstk::NavType::GPSLNAV);
   gpstk::NavMessageID nmide(sat, gpstk::NavMessageType::Ephemeris);
   gpstk::NavMessageID nmida(sat, gpstk::NavMessageType::Almanac);
      // shouldn't have data at this time
   TUASSERT(!navLib.find(nmide, ct+35, ndp, gpstk::SVHealth::Any,
                         gpstk::NavValidityType::ValidOnly,
                         gpstk::NavSearchOrder::User));
      // shouldn't have data at this time, either
      // Remember that the first line of a RINEX 2 nav like we use in
      // this test is the Toc, NOT the transmit time.
   gpstk::CivilTime civ2(2015,7,19,12,35,35.0,gpstk::TimeSystem::GPS);
   TUASSERT(!navLib.find(nmide, civ2, ndp, gpstk::SVHealth::Any,
                         gpstk::NavValidityType::ValidOnly,
                         gpstk::NavSearchOrder::User));
      // should have data at this time
   gpstk::CivilTime civ3(2015,7,19,12,35,48.0,gpstk::TimeSystem::GPS);
   TUASSERT(navLib.find(nmide, civ3, ndp, gpstk::SVHealth::Any,
                        gpstk::NavValidityType::ValidOnly,
                        gpstk::NavSearchOrder::User));
   eph = dynamic_cast<gpstk::GPSLNavEph*>(ndp.get());
   TUASSERT(eph != nullptr);
   TUASSERTE(uint16_t, 64, eph->iode);
      // shouldn't have almanac data though
   TUASSERT(!navLib.find(nmida, civ3, ndp, gpstk::SVHealth::Any,
                         gpstk::NavValidityType::ValidOnly,
                         gpstk::NavSearchOrder::User));
      // should have this data
   gpstk::NavSatelliteID sat4(2, 2, gpstk::SatelliteSystem::GPS,
                              gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                              gpstk::NavType::GPSLNAV);
   gpstk::NavMessageID nmide4(sat4, gpstk::NavMessageType::Ephemeris);
   gpstk::CivilTime civ4(2015,7,19,2,0,0.0,gpstk::TimeSystem::GPS);
   TUASSERT(navLib.find(nmide4, civ4, ndp, gpstk::SVHealth::Any,
                        gpstk::NavValidityType::ValidOnly,
                        gpstk::NavSearchOrder::User));
   eph = dynamic_cast<gpstk::GPSLNavEph*>(ndp.get());
   TUASSERT(eph != nullptr);
   TUASSERTE(uint16_t, 7, eph->iode);
   TURETURN();
}


unsigned NavLibrary_T ::
setValidityFilterTest()
{
   TUDEF("NavLibrary", "setValidityFilter");
   gpstk::NavLibrary navLib;
   gpstk::NavDataFactoryPtr ndfp1(std::make_shared<TestFactory>());
   gpstk::NavDataFactoryPtr
      ndfp2(std::make_shared<RinexTestFactory>());
   TUCATCH(navLib.addFactory(ndfp1));
   TUCATCH(navLib.addFactory(ndfp2));
   TestFactory *tfp = dynamic_cast<TestFactory*>(ndfp1.get());
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp2.get());
   TUASSERT(tfp != nullptr);
   TUASSERT(rndfp != nullptr);
   navLib.setValidityFilter(gpstk::NavValidityType::ValidOnly);
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::ValidOnly,
             tfp->getValidityFilter());
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::ValidOnly,
             rndfp->getValidityFilter());
   navLib.setValidityFilter(gpstk::NavValidityType::Any);
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::Any,
             tfp->getValidityFilter());
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::Any,
             rndfp->getValidityFilter());
   TURETURN();
}


unsigned NavLibrary_T ::
setTypeFilterTest()
{
   TUDEF("NavLibrary", "setTypeFilter");
   gpstk::NavLibrary navLib;
   gpstk::NavDataFactoryPtr ndfp1(std::make_shared<TestFactory>());
   gpstk::NavDataFactoryPtr
      ndfp2(std::make_shared<RinexTestFactory>());
   TUCATCH(navLib.addFactory(ndfp1));
   TUCATCH(navLib.addFactory(ndfp2));
   TestFactory *tfp = dynamic_cast<TestFactory*>(ndfp1.get());
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp2.get());
   TUASSERT(tfp != nullptr);
   TUASSERT(rndfp != nullptr);
   gpstk::NavMessageTypeSet nmts1 { gpstk::NavMessageType::Unknown };
   gpstk::NavMessageTypeSet nmts2 { gpstk::NavMessageType::Ephemeris };
   navLib.setTypeFilter(nmts1);
   TUASSERTE(gpstk::NavMessageTypeSet, nmts1, tfp->getTypeFilter());
   TUASSERTE(gpstk::NavMessageTypeSet, nmts1, rndfp->getTypeFilter());
   navLib.setTypeFilter(nmts2);
   TUASSERTE(gpstk::NavMessageTypeSet, nmts2, tfp->getTypeFilter());
   TUASSERTE(gpstk::NavMessageTypeSet, nmts2, rndfp->getTypeFilter());
   navLib.setTypeFilter(gpstk::allNavMessageTypes);
   TUASSERTE(gpstk::NavMessageTypeSet, gpstk::allNavMessageTypes,
             tfp->getTypeFilter());
   TUASSERTE(gpstk::NavMessageTypeSet, gpstk::allNavMessageTypes,
             rndfp->getTypeFilter());
   TURETURN();
}


unsigned NavLibrary_T ::
addFactoryTest()
{
   TUDEF("NavLibrary", "addFactory");
   TestClass navLib;
   gpstk::NavDataFactoryPtr ndfp1(std::make_shared<TestFactory>());
   gpstk::NavDataFactoryPtr
      ndfp2(std::make_shared<RinexTestFactory>());
   TUCATCH(navLib.addFactory(ndfp1));
   TUCATCH(navLib.addFactory(ndfp2));
   TestFactory *tfp = dynamic_cast<TestFactory*>(ndfp1.get());
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp2.get());
   TUASSERT(tfp != nullptr);
   TUASSERT(rndfp != nullptr);
   gpstk::NavDataFactoryMap &fm(navLib.getFactories());
      // make sure the factories map has the same number of items
   size_t expTotal = tfp->supportedSignals.size() +
      rndfp->supportedSignals.size();
   TUASSERTE(size_t, expTotal, fm.size());
      // make sure the map has the same set of signals
   gpstk::NavSignalSet allSignals;
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
   TUDEF("NavLibrary", "getInitialTime");
   gpstk::NavLibrary navLib;
   gpstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "arlm2000.15n";
   TUCATCH(navLib.addFactory(ndfp));
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   TUASSERTE(gpstk::CommonTime,
             gpstk::CivilTime(2015,7,19,0,0,0.0,
                              gpstk::TimeSystem::GPS).convertToCommonTime(),
             navLib.getInitialTime());
   TUASSERTE(gpstk::CommonTime,
             gpstk::CivilTime(2015,7,20,2,0,0.0,
                              gpstk::TimeSystem::GPS).convertToCommonTime(),
             navLib.getFinalTime());
   TURETURN();
}


unsigned NavLibrary_T ::
getAvailableSatsTest()
{
   TUDEF("NavLibrary", "getAvailableSats");
   gpstk::NavLibrary uut;
   gpstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "arlm2000.15n";
   gpstk::NavSatelliteIDSet satset;
   TUCATCH(uut.addFactory(ndfp));
   RinexTestFactory *rndfp = dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
      // really basic tests, the real tests are in NavDataFactoryWithStore_T etc
   TUCATCH(satset = uut.getAvailableSats(
              gpstk::CommonTime::BEGINNING_OF_TIME,
              gpstk::CommonTime::END_OF_TIME));
   TUASSERTE(gpstk::NavSatelliteIDSet::size_type, 31, satset.size());
   TUCATCH(satset = uut.getAvailableSats(
              gpstk::CivilTime(2020,4,12,0,56,0,gpstk::TimeSystem::GPS),
              gpstk::CivilTime(2020,4,12,0,57,0,gpstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, satset.empty());
   TURETURN();
}


unsigned NavLibrary_T ::
isPresentTest()
{
   TUDEF("NavLibrary", "isPresent");
   gpstk::NavLibrary uut;
   gpstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "arlm2000.15n";
      // really basic tests, the real tests are in NavDataFactoryWithStore_T etc
   TUCATCH(uut.addFactory(ndfp));
   RinexTestFactory *rndfp = dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   gpstk::NavSatelliteID sat1(gpstk::SatID(23,gpstk::SatelliteSystem::GPS));
   gpstk::NavMessageID nmid1e(sat1, gpstk::NavMessageType::Ephemeris),
      nmid1a(sat1, gpstk::NavMessageType::Almanac);
   gpstk::CivilTime t1(2015,7,19,10,0,0,gpstk::TimeSystem::GPS);
   gpstk::CivilTime t2(2015,7,19,11,0,0,gpstk::TimeSystem::GPS);
   // uut.dump(std::cerr, gpstk::DumpDetail::Brief);
   TUASSERTE(bool, true, uut.isPresent(nmid1e, t1, t2));
   TUASSERTE(bool, false, uut.isPresent(nmid1a, t1, t2));
   TUCSM("isTypePresent");
   TUASSERTE(bool, true, uut.isTypePresent(
                gpstk::NavMessageType::Ephemeris, sat1, t1, t2));
   TUASSERTE(bool, false, uut.isTypePresent(
                gpstk::NavMessageType::Almanac, sat1, t1, t2));
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
   errorTotal += testClass.addFactoryTest();
   errorTotal += testClass.getTimeTest();
   errorTotal += testClass.getAvailableSatsTest();
   errorTotal += testClass.isPresentTest();
      /// @todo test edit(), clear()
   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
