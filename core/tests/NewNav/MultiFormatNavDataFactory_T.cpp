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
#include "MultiFormatNavDataFactory.hpp"
#include "RinexNavDataFactory.hpp"
#include "SP3NavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavHealth.hpp"
#include "OrbitDataSP3.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::SatelliteSystem e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::CarrierBand e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::TrackingCode e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::NavType e)
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

   /** Implement a test class to expose protected members rather than
    * using friends. */
class TestClass : public gpstk::MultiFormatNavDataFactory
{
public:
      /// Grant access to protected data.
   gpstk::NavMessageMap& getData()
   { return data; }
   static gpstk::NavDataFactoryMap& getFactories()
   { return factories(); }
};

   /// Expose NavDataFactory protected members
class TestFactory : public gpstk::NavDataFactoryWithStore
{
public:
   gpstk::NavMessageMap& getData()
   { return data; }
   gpstk::NavValidityType getValidityFilter() const
   { return navValidity; }
   gpstk::NavMessageTypeSet getTypeFilter() const
   { return procNavTypes; }
};


/** Automated tests for gpstk::MultiFormatNavDataFactory
 * @note addFactory is tested by GPSTKFormatInitializer_T
 */
class MultiFormatNavDataFactory_T
{
public:
      /// Make sure the constructor does what it's supposed to.
   unsigned constructorTest();
   unsigned findTest();
   unsigned editTest();
   unsigned clearTest();
   unsigned sizeTest();
   unsigned numSignalsTest();
   unsigned numSatellitesTest();
   unsigned setValidityFilterTest();
   unsigned setTypeFilterTest();
      /// Exercise loadIntoMap by loading data with different options in place.
   unsigned loadIntoMapTest();
};


unsigned MultiFormatNavDataFactory_T ::
constructorTest()
{
   TUDEF("MultiFormatNavDataFactory", "MultiFormatNavDataFactory");
   gpstk::MultiFormatNavDataFactory fact;
      // Check for expected signal support.  The
      // GPSTKFormatInitializer_T test takes care of making sure the
      // actual correct factories are added.
   gpstk::RinexNavDataFactory rinFact;
   for (const auto& i : rinFact.supportedSignals)
   {
      TUASSERT(fact.supportedSignals.count(i));
   }
   gpstk::SP3NavDataFactory sp3Fact;
   for (const auto& i : sp3Fact.supportedSignals)
   {
      TUASSERT(fact.supportedSignals.count(i));
   }
   TURETURN();
}


unsigned MultiFormatNavDataFactory_T ::
findTest()
{
   TUDEF("MultiFormatNavDataFactory", "find");
   gpstk::MultiFormatNavDataFactory fact;
   gpstk::NavSatelliteID satID1a(7, 7, gpstk::SatelliteSystem::GPS,
                                 gpstk::CarrierBand::L1,
                                 gpstk::TrackingCode::CA,
                                 gpstk::NavType::GPSLNAV);
   gpstk::NavMessageID nmid1a(satID1a, gpstk::NavMessageType::Ephemeris);
   std::string dpath = gpstk::getPathData() + gpstk::getFileSep();
   gpstk::CivilTime civ1a(2015, 7, 19, 16, 0, 0, gpstk::TimeSystem::GPS);
   gpstk::CivilTime civ1b(2015, 7, 19, 18, 0, 0, gpstk::TimeSystem::GPS);
   gpstk::NavDataPtr nd1a, nd1b;
   TUCSM("loadIntoMap");
   TUASSERT(fact.addDataSource(dpath + "arlm2000.15n"));
   TUASSERT(fact.addDataSource(dpath + "test_input_sp3_nav_ephemerisData.sp3"));
   TUASSERT(fact.find(nmid1a, civ1a, nd1a, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
      // nd1a should be a pointer to a GPSLNavEph
   gpstk::GPSLNavEph *ephPtr = dynamic_cast<gpstk::GPSLNavEph*>(nd1a.get());
   TUASSERT(ephPtr != nullptr);
   if (ephPtr == nullptr)
      TURETURN();
      // make sure we got the ephemeris we expected.
   TUASSERTE(uint16_t, 0x4f, ephPtr->iodc);
      // This tests getting an updated ephemeris, i.e. a Toe not
      // aligned to 2 hours
   TUASSERT(fact.find(nmid1a, civ1b, nd1b, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
      // nd1b should be a pointer to a GPSLNavEph
   ephPtr = dynamic_cast<gpstk::GPSLNavEph*>(nd1b.get());
   TUASSERT(ephPtr != nullptr);
   if (ephPtr == nullptr)
      TURETURN();
      // make sure we got the ephemeris we expected.
   TUASSERTE(uint16_t, 0x0a, ephPtr->iodc);
      // now check for SP3 data
   gpstk::NavSatelliteID satID2(15, 15, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::SatID expSat2(15, gpstk::SatelliteSystem::GPS);
   gpstk::NavMessageID nmid2(satID2, gpstk::NavMessageType::Ephemeris);
   gpstk::CommonTime ct;
   ct = gpstk::CivilTime(1997,4,6,6,17,36,gpstk::TimeSystem::GPS);
   gpstk::NavDataPtr nd2;
   TUASSERT(fact.find(nmid2, ct, nd2, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   gpstk::OrbitDataSP3 *uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd2.get());
   if (uut == nullptr)
      TURETURN();
      // NavData
   TUASSERTE(gpstk::CommonTime, ct, uut->timeStamp);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gpstk::SatID, expSat2, uut->signal.sat);
   TUASSERTE(gpstk::SatID, expSat2, uut->signal.xmitSat);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
             uut->signal.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1, uut->signal.obs.band);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::CA, uut->signal.obs.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, uut->signal.nav);
      // OrbitData
      // OrbitDataSP3
   TUASSERTFE(-15643.515779275317982, uut->pos[0]);
   TUASSERTFE(17046.376009584488202, uut->pos[1]);
   TUASSERTFE(12835.522993916223641, uut->pos[2]);
   TUASSERTFE(0, uut->posSig[0]);
   TUASSERTFE(0, uut->posSig[1]);
   TUASSERTFE(0, uut->posSig[2]);
   TUASSERTFE(4118.735914736347695, uut->vel[0]);
   TUASSERTFE(-16208.344018608038823, uut->vel[1]);
   TUASSERTFE(26087.591273316520528, uut->vel[2]);
   TUASSERTFE(0, uut->velSig[0]);
   TUASSERTFE(0, uut->velSig[1]);
   TUASSERTFE(0, uut->velSig[2]);
   TUASSERTFE(0, uut->acc[0]);
   TUASSERTFE(0, uut->acc[1]);
   TUASSERTFE(0, uut->acc[2]);
   TUASSERTFE(0, uut->accSig[0]);
   TUASSERTFE(0, uut->accSig[1]);
   TUASSERTFE(0, uut->accSig[2]);
   TUASSERTFE(411.55797411176871492, uut->clkBias);
   TUASSERTFE(0, uut->biasSig);
   TUASSERTFE(2.2909472663417080121e-06, uut->clkDrift);
   TUASSERTFE(0, uut->driftSig);
   TUASSERTFE(0, uut->clkDrRate);
   TUASSERTFE(0, uut->drRateSig);
   TURETURN();
}


unsigned MultiFormatNavDataFactory_T ::
editTest()
{
   TUDEF("MultiFormatNavDataFactory", "edit");
      // time before any of the loaded data
   gpstk::CivilTime before(2000, 1, 1, 0, 0, 0, gpstk::TimeSystem::GPS);
   gpstk::CivilTime between(2005, 1, 1, 0, 0, 0, gpstk::TimeSystem::GPS);
   gpstk::NavSatelliteID
      satID2a(7, 7, gpstk::SatelliteSystem::GPS, gpstk::CarrierBand::L1,
              gpstk::TrackingCode::CA, gpstk::NavType::GPSLNAV),
      satID2b(99, 99, gpstk::SatelliteSystem::GPS, gpstk::CarrierBand::L1,
              gpstk::TrackingCode::CA, gpstk::NavType::GPSLNAV),
      satID2c(7, 7, gpstk::SatelliteSystem::GPS, gpstk::CarrierBand::L5,
              gpstk::TrackingCode::CA, gpstk::NavType::GPSLNAV),
      satID2d(11, 11, gpstk::SatelliteSystem::GPS, gpstk::CarrierBand::L1,
              gpstk::TrackingCode::Y, gpstk::NavType::GPSLNAV),
      satID2e(23, 32, gpstk::SatelliteSystem::GPS, gpstk::CarrierBand::L1,
              gpstk::TrackingCode::CA, gpstk::NavType::GPSLNAV);
   gpstk::MultiFormatNavDataFactory fact;
   std::string dpath = gpstk::getPathData() + gpstk::getFileSep();
      // get pointers to known factories to verify results
   gpstk::RinexNavDataFactory *rinFact = nullptr;
   gpstk::SP3NavDataFactory *sp3Fact = nullptr;
   for (auto& i : TestClass::getFactories())
   {
      gpstk::NavDataFactory *p = i.second.get();
      if (rinFact == nullptr)
      {
         rinFact = dynamic_cast<gpstk::RinexNavDataFactory*>(p);
      }
      if (sp3Fact == nullptr)
      {
         sp3Fact = dynamic_cast<gpstk::SP3NavDataFactory*>(p);
      }
   }
   TUASSERT(rinFact != nullptr);
   TUASSERT(sp3Fact != nullptr);
   if ((rinFact == nullptr) || (sp3Fact == nullptr))
   {
         // don't try to continue with tests that will cause seg faults.
      TURETURN();
   }
   TUCSM("loadIntoMap");
   TUASSERT(fact.addDataSource(dpath + "arlm2000.15n"));
   TUASSERTE(size_t, 337, rinFact->size());
   TUASSERT(fact.addDataSource(dpath + "test_input_SP3a.sp3"));
   TUASSERTE(size_t, 232, sp3Fact->size());

   TUCSM("edit");
      // remove nothing
   TUCATCH(fact.edit(gpstk::CommonTime::BEGINNING_OF_TIME, before));
   TUASSERTE(size_t, 337, rinFact->size());
   TUASSERTE(size_t, 232, sp3Fact->size());
      // remove a time span that covers all of the SP3 data but none
      // of the RINEX data.
   TUCATCH(fact.edit(before, between));
   TUASSERTE(size_t, 337, rinFact->size());
   TUASSERTE(size_t, 0, sp3Fact->size());
      // reload SP3 data for next test
   TUCSM("loadIntoMap");
   TUASSERT(fact.addDataSource(dpath + "test_input_SP3a.sp3"));
   TUASSERTE(size_t, 232, sp3Fact->size());
   TUCSM("edit");
      // remove a time span that covers all of the RINEX data but none
      // of the SP3 data.
   TUCATCH(fact.edit(between, gpstk::CommonTime::END_OF_TIME));
   TUASSERTE(size_t, 0, rinFact->size());
   TUASSERTE(size_t, 232, sp3Fact->size());
      // reload RINEX data for next test
   TUCSM("loadIntoMap");
   TUASSERT(fact.addDataSource(dpath + "arlm2000.15n"));
   TUASSERTE(size_t, 337, rinFact->size());
      // verify initial conditions
   TUCSM("numSatellites");
   TUASSERTE(size_t, 32, rinFact->numSatellites());
   TUASSERTE(size_t, 29, sp3Fact->numSatellites());
      // remove nothing - time
   TUCSM("edit");
   TUCATCH(fact.edit(gpstk::CommonTime::BEGINNING_OF_TIME, before, satID2a));
   TUASSERTE(size_t, 337, rinFact->size());
   TUASSERTE(size_t, 232, sp3Fact->size());
      // remove nothing, this time because the satellite ID isn't present
   TUCATCH(fact.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                     gpstk::CommonTime::END_OF_TIME, satID2b));
   TUASSERTE(size_t, 337, rinFact->size());
   TUASSERTE(size_t, 232, sp3Fact->size());
      // remove nothing, this time because the signal isn't present
   TUCATCH(fact.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                     gpstk::CommonTime::END_OF_TIME, satID2c));
   TUASSERTE(size_t, 337, rinFact->size());
   TUASSERTE(size_t, 232, sp3Fact->size());
   TURETURN();
}


unsigned MultiFormatNavDataFactory_T ::
clearTest()
{
   TUDEF("MultiFormatNavDataFactory", "clear");
   gpstk::MultiFormatNavDataFactory fact;
   std::string dpath = gpstk::getPathData() + gpstk::getFileSep();
   TUCSM("loadIntoMap");
   TUASSERT(fact.addDataSource(dpath + "arlm2000.15n"));
   TUASSERT(fact.addDataSource(dpath + "test_input_SP3a.sp3"));
   TUCSM("size");
   TUASSERTE(size_t, 337+232, fact.size());
   TUCSM("clear");
   TUCATCH(fact.clear());
   TUASSERTE(size_t, 0, fact.size());
   TURETURN();
}


unsigned MultiFormatNavDataFactory_T ::
sizeTest()
{
   TUDEF("MultiFormatNavDataFactory", "size");
   gpstk::MultiFormatNavDataFactory fact;
   std::string dpath = gpstk::getPathData() + gpstk::getFileSep();
   TUCSM("loadIntoMap");
   TUASSERT(fact.addDataSource(dpath + "arlm2000.15n"));
   TUASSERT(fact.addDataSource(dpath + "test_input_SP3a.sp3"));
   TUCSM("size");
   TUASSERTE(size_t, 337+232, fact.size());
   TURETURN();
}


unsigned MultiFormatNavDataFactory_T ::
numSignalsTest()
{
   TUDEF("MultiFormatNavDataFactory", "numSignals");
   gpstk::MultiFormatNavDataFactory fact;
   gpstk::NavSignalID expSig(gpstk::SatelliteSystem::GPS,
                             gpstk::CarrierBand::L1,
                             gpstk::TrackingCode::CA,
                             gpstk::NavType::GPSLNAV);
   std::string dpath = gpstk::getPathData() + gpstk::getFileSep();
   TUCSM("loadIntoMap");
   TUASSERT(fact.addDataSource(dpath + "arlm2000.15n"));
   TUASSERT(fact.addDataSource(dpath + "test_input_SP3a.sp3"));
   TUCSM("numSignals");
      // should just be one, L1 C/A LNav
   TUASSERTE(size_t, 1, fact.numSignals());
   for (const auto& fi : TestClass::getFactories())
   {
      TestFactory *tfp = reinterpret_cast<TestFactory*>(fi.second.get());
      TUASSERT(tfp != nullptr);
      if (tfp == nullptr)
         continue;
      for (const auto& di : tfp->getData())
      {
         for (const auto& sigi : di.second)
         {
            TUASSERTE(gpstk::NavSignalID, expSig, sigi.first);
         }
      }
   }
   TURETURN();
}


unsigned MultiFormatNavDataFactory_T ::
numSatellitesTest()
{
   TUDEF("MultiFormatNavDataFactory", "numSatellites");
   gpstk::MultiFormatNavDataFactory fact;
   std::string dpath = gpstk::getPathData() + gpstk::getFileSep();
   TUCSM("loadIntoMap");
   TUASSERT(fact.addDataSource(dpath + "arlm2000.15n"));
   TUASSERT(fact.addDataSource(dpath + "test_input_SP3a.sp3"));
   TUCSM("numSatellites");
   TUASSERTE(size_t, 32, fact.numSatellites());
   TURETURN();
}


unsigned MultiFormatNavDataFactory_T ::
setValidityFilterTest()
{
   TUDEF("MultiFormatNavDataFactory", "setValidityFilter");
   TestClass mfact;
   mfact.setValidityFilter(gpstk::NavValidityType::ValidOnly);
   for (const auto& i : TestClass::getFactories())
   {
      TestFactory *tfp = reinterpret_cast<TestFactory*>(i.second.get());
      TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::ValidOnly,
                tfp->getValidityFilter());
   }
   mfact.setValidityFilter(gpstk::NavValidityType::Any);
   for (const auto& i : TestClass::getFactories())
   {
      TestFactory *tfp = reinterpret_cast<TestFactory*>(i.second.get());
      TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::Any,
                tfp->getValidityFilter());
   }
   TURETURN();
}


unsigned MultiFormatNavDataFactory_T ::
setTypeFilterTest()
{
   TUDEF("MultiFormatNavDataFactory", "setTypeFilter");
   TestClass mfact;
   gpstk::NavMessageTypeSet nmts1 { gpstk::NavMessageType::Unknown };
   gpstk::NavMessageTypeSet nmts2 { gpstk::NavMessageType::Ephemeris };
   mfact.setTypeFilter(nmts1);
   for (const auto& i : TestClass::getFactories())
   {
      TestFactory *tfp = reinterpret_cast<TestFactory*>(i.second.get());
      TUASSERTE(gpstk::NavMessageTypeSet, nmts1, tfp->getTypeFilter());
   }
   mfact.setTypeFilter(nmts2);
   for (const auto& i : TestClass::getFactories())
   {
      TestFactory *tfp = reinterpret_cast<TestFactory*>(i.second.get());
      TUASSERTE(gpstk::NavMessageTypeSet, nmts2, tfp->getTypeFilter());
   }
   mfact.setTypeFilter(gpstk::allNavMessageTypes);
   for (const auto& i : TestClass::getFactories())
   {
      TestFactory *tfp = reinterpret_cast<TestFactory*>(i.second.get());
      TUASSERTE(gpstk::NavMessageTypeSet, gpstk::allNavMessageTypes,
                tfp->getTypeFilter());
   }
   TURETURN();
}


unsigned MultiFormatNavDataFactory_T ::
loadIntoMapTest()
{
   TUDEF("MultiFormatNavDataFactory", "loadIntoMap");

      // get pointers to known factories to verify results
   gpstk::RinexNavDataFactory *rinFact = nullptr;
   gpstk::SP3NavDataFactory *sp3Fact = nullptr;
   for (auto& i : TestClass::getFactories())
   {
      gpstk::NavDataFactory *p = i.second.get();
      if (rinFact == nullptr)
      {
         rinFact = dynamic_cast<gpstk::RinexNavDataFactory*>(p);
      }
      if (sp3Fact == nullptr)
      {
         sp3Fact = dynamic_cast<gpstk::SP3NavDataFactory*>(p);
      }
   }
   TUASSERT(rinFact != nullptr);
   TUASSERT(sp3Fact != nullptr);
   if ((rinFact == nullptr) || (sp3Fact == nullptr))
   {
         // don't try to continue with tests that will cause seg faults.
      TURETURN();
   }

      // test loading RINEX 2 nav
   gpstk::MultiFormatNavDataFactory fact;
   std::string f2name = gpstk::getPathData() + gpstk::getFileSep() +
      "arlm2000.15n";
      // this should implicitly load into the data map
   TUASSERT(fact.addDataSource(f2name));
   TUASSERTE(size_t, 337, rinFact->size());

      // test loading SP3 nav
   std::string f3name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUASSERT(fact.addDataSource(f3name));
   TUASSERTE(size_t, 232, sp3Fact->size());

      // Not testing filter combinations here.  This should be
      // implicitly tested by the individual factories'
      // loadIntoMapTests and the
      // setTypeFilterTest/setValidityFilterTest methods here.

   TURETURN();
}


int main()
{
   MultiFormatNavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.findTest();
   errorTotal += testClass.editTest();
   errorTotal += testClass.clearTest();
   errorTotal += testClass.sizeTest();
   errorTotal += testClass.numSignalsTest();
   errorTotal += testClass.numSatellitesTest();
   errorTotal += testClass.setValidityFilterTest();
   errorTotal += testClass.setTypeFilterTest();
   errorTotal += testClass.loadIntoMapTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
