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
#include "NavDataFactoryWithStore.hpp"
#include "GPSWeekSecond.hpp"
#include "CivilTime.hpp"
#include "GALWeekSecond.hpp"
#include "BDSWeekSecond.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavAlm.hpp"
#include "GPSLNavHealth.hpp"
#include "GPSLNavTimeOffset.hpp"
#include "TestUtil.hpp"
// #include "BasicTimeSystemConverter.hpp"
#include "TimeString.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::SatelliteSystem e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gnsstk::CarrierBand e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gnsstk::TrackingCode e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gnsstk::NavType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


/// Used to test the proper setting of initialTime/finalTime.
class FakeODK : public gnsstk::OrbitDataKepler
{
public:
   FakeODK()
   {}
      /// Create a deep copy of this object.
   gnsstk::NavDataPtr clone() const override
   { return std::make_shared<FakeODK>(*this); }
   bool validate() const override
   { return true; }
   bool getXvt(const gnsstk::CommonTime& when, gnsstk::Xvt& xvt,
               const gnsstk::ObsID& oid = gnsstk::ObsID()) override
   { GNSSTK_THROW(gnsstk::Exception("Not implemented")); }
   double svRelativity(const gnsstk::CommonTime& when) const override
   { GNSSTK_THROW(gnsstk::Exception("Not implemented")); }
};

   /** Implement a test class to expose protected members rather than
    * using friends. */
class TestClass : public gnsstk::NavDataFactoryWithStore
{
public:
      /// Grant access to protected data.
   gnsstk::NavMessageMap& getData()
   { return data; }
   gnsstk::NavNearMessageMap& getNearestData()
   { return nearestData; }
   bool addDataSource(const std::string& source) override
   { return false; }
   size_t sizeNearest() const
   {
      size_t rv = 0;
      for (const auto& nnmi : nearestData)
      {
         for (const auto& nnsmi : nnmi.second)
         {
            for (const auto& nnmi : nnsmi.second)
            {
               rv += nnmi.second.size();
            }
         }
      }
      return rv;
   }
   size_t numSatellitesNearest() const
   {
      size_t rv = 0;
      for (const auto& nnmi : nearestData)
      {
         rv += nnmi.second.size();
      }
      return rv;
   }
   std::string getFactoryFormats() const override
   { return "BUNK"; }
};


class NavDataFactoryWithStore_T
{
public:
      /// Initialize time objects used for stamping fake nav data.
   NavDataFactoryWithStore_T();

   unsigned addNavDataTest();
   unsigned addNavDataTimeTest();
   unsigned findTest();
   unsigned find2Test();
   unsigned findNearestTest();
      /// Test find with various xmitHealth settings.
   unsigned findXmitHealthTest();
   unsigned getOffsetTest();
   unsigned getOffset2Test();
   unsigned editTest();
   unsigned clearTest();
   unsigned getAvailableSatsTest();
   unsigned getIndexSetTest();
   unsigned isPresentTest();
   unsigned countTest();
   unsigned getFirstLastTimeTest();

      /// Fill fact with test data
   void fillFactory(gnsstk::TestUtil& testFramework, TestClass& fact);
      /// Fill fact with test data for findXmitHealthTest()
   void fillFactoryXmitHealth(gnsstk::TestUtil& testFramework, TestClass& fact);
      /// Add a single NavData with the given parameters
   void addData(gnsstk::TestUtil& testFramework, TestClass& fact,
                const gnsstk::CommonTime& ct, unsigned long sat,
                unsigned long xmitSat,
                gnsstk::SatelliteSystem sys = gnsstk::SatelliteSystem::GPS,
                gnsstk::CarrierBand car = gnsstk::CarrierBand::L1,
                gnsstk::TrackingCode code = gnsstk::TrackingCode::CA,
                gnsstk::NavType nav = gnsstk::NavType::GPSLNAV,
                gnsstk::SVHealth hea = gnsstk::SVHealth::Healthy,
                gnsstk::NavMessageType nmt = gnsstk::NavMessageType::Ephemeris);
   void fillSignal(gnsstk::NavSignalID& signal,
                   gnsstk::SatelliteSystem sys = gnsstk::SatelliteSystem::GPS,
                   gnsstk::CarrierBand car = gnsstk::CarrierBand::L1,
                   gnsstk::TrackingCode code = gnsstk::TrackingCode::CA,
                   gnsstk::NavType nav = gnsstk::NavType::GPSLNAV);
   void fillSat(gnsstk::NavSatelliteID& satellite,
                unsigned long sat,
                unsigned long xmitSat,
                gnsstk::SatelliteSystem sys = gnsstk::SatelliteSystem::GPS,
                gnsstk::CarrierBand car = gnsstk::CarrierBand::L1,
                gnsstk::TrackingCode code = gnsstk::TrackingCode::CA,
                gnsstk::NavType nav = gnsstk::NavType::GPSLNAV);
      /// Check to make sure there are no empty maps in fact.
   void checkForEmpty(gnsstk::TestUtil& testFramework, TestClass& fact);

   gnsstk::GPSWeekSecond gws, gws5;
   gnsstk::CommonTime ct, ct5;
};


NavDataFactoryWithStore_T ::
NavDataFactoryWithStore_T()
      : gws(2101,3450.0),
        ct(gws),
        gws5(2101,3894.0),
        ct5(gws5)
{
      // 3450 / 750 = 4.6, so 4 full master frames before gws
      // 3450 % 750 = 450 / 30 = 15 so page 15.
      // add 14*30 + 24 seconds and that should be alm 5 transmit time. I think.
      // = 3894
}


unsigned NavDataFactoryWithStore_T ::
addNavDataTest()
{
   TUDEF("NavDataFactoryWithStore", "addNavData");

   TestClass fact;

      // NavData is abstract so we instantiate a GPSLNavEph instead
   gnsstk::SatID subjID(23,gnsstk::SatelliteSystem::GPS);
   gnsstk::SatID xmitID(32,gnsstk::SatelliteSystem::GPS);
   std::shared_ptr<gnsstk::GPSLNavEph> navOut =
      std::make_shared<gnsstk::GPSLNavEph>();
   navOut->timeStamp = ct;
   navOut->xmitTime = ct;
   navOut->xmit2 = ct + 6;
   navOut->xmit3 = ct + 12;
   navOut->signal.messageType = gnsstk::NavMessageType::Ephemeris;
   navOut->signal.sat = subjID;
   navOut->signal.xmitSat = xmitID;
   navOut->signal.system = gnsstk::SatelliteSystem::GPS;
   navOut->signal.obs = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                                     gnsstk::CarrierBand::L1,
                                     gnsstk::TrackingCode::CA);
   navOut->signal.nav = gnsstk::NavType::GPSLNAV;

   TUASSERT(fact.addNavData(navOut));
   TUASSERTE(size_t, 1, fact.size());

   gnsstk::NavMessageMap &nmm(fact.getData());
   for (auto& nmmi : nmm)
   {
      for (auto& nsatmi : nmmi.second)
      {
         TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS,
                   nsatmi.first.system);
         TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1,
                   nsatmi.first.obs.band);
         TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::CA,
                   nsatmi.first.obs.code);
         TUASSERTE(gnsstk::NavType, gnsstk::NavType::GPSLNAV, nsatmi.first.nav);
         TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS,
                   nsatmi.first.system);
         TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1,
                   nsatmi.first.obs.band);
         TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::CA,
                   nsatmi.first.obs.code);
         TUASSERTE(gnsstk::NavType, gnsstk::NavType::GPSLNAV,
                   nsatmi.first.nav);
         TUASSERTE(gnsstk::SatID, subjID, nsatmi.first.sat);
         TUASSERTE(gnsstk::SatID, xmitID, nsatmi.first.xmitSat);
         for (auto& nmi : nsatmi.second)
         {
            TUASSERTE(gnsstk::CommonTime, ct+18, nmi.first);
         }
      }
   }

   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
addNavDataTimeTest()
{
   TUDEF("NavDataFactoryWithStore", "addNavData");

   TestClass fact;
      // test ordering with multiple time systems
   // gnsstk::BasicTimeSystemConverter btsc;
   gnsstk::CommonTime
      bt0(gnsstk::GPSWeekSecond(2020, 54321)),
      bt1(gnsstk::GPSWeekSecond(2020, 54327)),
      bt2(gnsstk::GALWeekSecond(996, 54329)),
      bt3(gnsstk::BDSWeekSecond(664, 54328)),
      bt4(gnsstk::GPSWeekSecond(2020, 60480)),
      et0(bt0 + 7200),
      et1(bt1 + 7200),
      et2(bt2 + 3600),
      et3(bt3 + 14400),
      et4(bt4 + 7200);

   // std::cout << bt0 << " " << et0 << std::endl << bt1 << " " << et1 << std::endl << bt2 << " " << et2 << std::endl << bt3 << " " << et3 << std::endl << bt4 << " " << et4 << std::endl;
   // bt0.changeTimeSystem(gnsstk::TimeSystem::UTC, &btsc);
   // bt1.changeTimeSystem(gnsstk::TimeSystem::UTC, &btsc);
   // bt2.changeTimeSystem(gnsstk::TimeSystem::UTC, &btsc);
   // bt3.changeTimeSystem(gnsstk::TimeSystem::UTC, &btsc);
   // bt4.changeTimeSystem(gnsstk::TimeSystem::UTC, &btsc);
   // et0.changeTimeSystem(gnsstk::TimeSystem::UTC, &btsc);
   // et1.changeTimeSystem(gnsstk::TimeSystem::UTC, &btsc);
   // et2.changeTimeSystem(gnsstk::TimeSystem::UTC, &btsc);
   // et3.changeTimeSystem(gnsstk::TimeSystem::UTC, &btsc);
   // et4.changeTimeSystem(gnsstk::TimeSystem::UTC, &btsc);
   // std::cout << bt0 << " " << et0 << std::endl << bt1 << " " << et1 << std::endl << bt2 << " " << et2 << std::endl << bt3 << " " << et3 << std::endl << bt4 << " " << et4 << std::endl;

      // add 5 NavDataPtr objects not in time order
      // #1
   gnsstk::SatID subjID(23,gnsstk::SatelliteSystem::GPS);
   gnsstk::SatID xmitID(32,gnsstk::SatelliteSystem::GPS);
   gnsstk::NavDataPtr navOut = std::make_shared<FakeODK>();
   dynamic_cast<FakeODK*>(navOut.get())->beginFit = bt3;
   dynamic_cast<FakeODK*>(navOut.get())->endFit = et3;
   navOut->signal.messageType = gnsstk::NavMessageType::Ephemeris;
   navOut->signal.sat = subjID;
   navOut->signal.xmitSat = xmitID;
   navOut->signal.system = gnsstk::SatelliteSystem::GPS;
   navOut->signal.obs = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                                     gnsstk::CarrierBand::L1,
                                     gnsstk::TrackingCode::CA);
   navOut->signal.nav = gnsstk::NavType::GPSLNAV;
   TUASSERT(fact.addNavData(navOut));
      // #2
   navOut = std::make_shared<FakeODK>();
   dynamic_cast<FakeODK*>(navOut.get())->beginFit = bt2;
   dynamic_cast<FakeODK*>(navOut.get())->endFit = et2;
   navOut->signal.messageType = gnsstk::NavMessageType::Ephemeris;
   navOut->signal.sat = subjID;
   navOut->signal.xmitSat = xmitID;
   navOut->signal.system = gnsstk::SatelliteSystem::GPS;
   navOut->signal.obs = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                                     gnsstk::CarrierBand::L1,
                                     gnsstk::TrackingCode::CA);
   navOut->signal.nav = gnsstk::NavType::GPSLNAV;
   TUASSERT(fact.addNavData(navOut));
      // #3
   navOut = std::make_shared<FakeODK>();
   dynamic_cast<FakeODK*>(navOut.get())->beginFit = bt4;
   dynamic_cast<FakeODK*>(navOut.get())->endFit = et4;
   navOut->signal.messageType = gnsstk::NavMessageType::Ephemeris;
   navOut->signal.sat = subjID;
   navOut->signal.xmitSat = xmitID;
   navOut->signal.system = gnsstk::SatelliteSystem::GPS;
   navOut->signal.obs = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                                     gnsstk::CarrierBand::L1,
                                     gnsstk::TrackingCode::CA);
   navOut->signal.nav = gnsstk::NavType::GPSLNAV;
   TUASSERT(fact.addNavData(navOut));
      // #4
   navOut = std::make_shared<FakeODK>();
   dynamic_cast<FakeODK*>(navOut.get())->beginFit = bt0;
   dynamic_cast<FakeODK*>(navOut.get())->endFit = et0;
   navOut->signal.messageType = gnsstk::NavMessageType::Ephemeris;
   navOut->signal.sat = subjID;
   navOut->signal.xmitSat = xmitID;
   navOut->signal.system = gnsstk::SatelliteSystem::GPS;
   navOut->signal.obs = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                                     gnsstk::CarrierBand::L1,
                                     gnsstk::TrackingCode::CA);
   navOut->signal.nav = gnsstk::NavType::GPSLNAV;
   TUASSERT(fact.addNavData(navOut));
      // #5
   navOut = std::make_shared<FakeODK>();
   dynamic_cast<FakeODK*>(navOut.get())->beginFit = bt1;
   dynamic_cast<FakeODK*>(navOut.get())->endFit = et1;
   navOut->signal.messageType = gnsstk::NavMessageType::Ephemeris;
   navOut->signal.sat = subjID;
   navOut->signal.xmitSat = xmitID;
   navOut->signal.system = gnsstk::SatelliteSystem::GPS;
   navOut->signal.obs = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                                     gnsstk::CarrierBand::L1,
                                     gnsstk::TrackingCode::CA);
   navOut->signal.nav = gnsstk::NavType::GPSLNAV;
   TUASSERT(fact.addNavData(navOut));

   TUASSERTE(gnsstk::CommonTime, bt0, fact.getInitialTime());
   TUASSERTE(gnsstk::CommonTime, et3, fact.getFinalTime());

   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
findTest()
{
   TUDEF("NavDataFactoryWithStore", "find");

   TestClass fact1;
   gnsstk::NavMessageID nmid1a;
   gnsstk::NavDataPtr result;
   TUCATCH(fillFactory(testFramework, fact1));
   TUCATCH(fillSat(nmid1a, 23, 32));
   nmid1a.messageType = gnsstk::NavMessageType::Ephemeris;
   gnsstk::CommonTime ect(ct-3600); // make the base time match the ephemeris
      // check basic time stamps that should work
   TUASSERT(fact1.find(nmid1a, ect+35, result, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::Any,
                       gnsstk::NavSearchOrder::User));
   TUASSERT(fact1.find(nmid1a, ect+30, result, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::Any,
                       gnsstk::NavSearchOrder::User));
   TUASSERT(fact1.find(nmid1a, ect+60, result, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::Any,
                       gnsstk::NavSearchOrder::User));
      // check time stamps that don't work
   TUASSERT(!fact1.find(nmid1a, ect, result, gnsstk::SVHealth::Any,
                        gnsstk::NavValidityType::Any,
                        gnsstk::NavSearchOrder::User));
      // time edge cases.
   TUASSERT(!fact1.find(nmid1a, ect+17, result, gnsstk::SVHealth::Any,
                        gnsstk::NavValidityType::Any,
                        gnsstk::NavSearchOrder::User));
   TUASSERT(fact1.find(nmid1a, ect+18, result, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::Any,
                       gnsstk::NavSearchOrder::User));
      // test validity flags
   TUASSERT(fact1.find(nmid1a, ect+30, result, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::ValidOnly,
                       gnsstk::NavSearchOrder::User));
   TUASSERT(!fact1.find(nmid1a, ect+30, result, gnsstk::SVHealth::Any,
                        gnsstk::NavValidityType::InvalidOnly,
                        gnsstk::NavSearchOrder::User));

   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
find2Test()
{
   TUDEF("NavDataFactoryWithStore", "find");
   TestClass uut;
   using SS = gnsstk::SatelliteSystem;
   using CB = gnsstk::CarrierBand;
   using TC = gnsstk::TrackingCode;
   using NT = gnsstk::NavType;
   using SH = gnsstk::SVHealth;
   using MT = gnsstk::NavMessageType;
   using VT = gnsstk::NavValidityType;
   using SO = gnsstk::NavSearchOrder;
   gnsstk::CommonTime refsf1ct = gnsstk::GPSWeekSecond(2101, 0);
      // between copies 2 and 3, or i==57
   gnsstk::CommonTime unhealthyStart = gnsstk::GPSWeekSecond(2101, 1710);
   gnsstk::CommonTime refpg2ct = gnsstk::GPSWeekSecond(2101, 54);
   gnsstk::CommonTime findTime1 = gnsstk::GPSWeekSecond(2101, 3838);
   gnsstk::NavMessageID findNMID1a(
      gnsstk::NavSatelliteID(2, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Almanac);
   gnsstk::NavMessageID findNMID2a(
      gnsstk::NavSatelliteID(2, SS::GPS, CB::Any, TC::Any, NT::Any),
      MT::Almanac);
   gnsstk::NavMessageID findNMID3a(
      gnsstk::NavSatelliteID(2, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Almanac);
   gnsstk::NavMessageID findNMID4a(
      gnsstk::NavSatelliteID(2, 1, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Almanac);
   gnsstk::NavMessageID findNMID5a(
      gnsstk::NavSatelliteID(2, 3, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Almanac);
   gnsstk::NavMessageID findNMID1h(
      gnsstk::NavSatelliteID(1, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Health);
   gnsstk::NavMessageID findNMID1L1CAe(
      gnsstk::NavSatelliteID(1, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Ephemeris);
   gnsstk::NavMessageID findNMID1L2Ye(
      gnsstk::NavSatelliteID(1, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Ephemeris);
   gnsstk::NavMessageID findNMID3L1CAe(
      gnsstk::NavSatelliteID(3, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Ephemeris);
   gnsstk::NavMessageID findNMID3e(
      gnsstk::NavSatelliteID(3, SS::GPS, CB::Any, TC::Any, NT::Any),
      MT::Ephemeris);
   gnsstk::NavMessageID findNMID3L2Ye(
      gnsstk::NavSatelliteID(3, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Ephemeris);
      // fill with "almanac pages"
   for (unsigned i = 0; i < 10; i++)
   {
      addData(testFramework, uut, refpg2ct + (750*i), 2, 1, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 1, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 1, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 3, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 3, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 3, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 4, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 4, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 4, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Almanac);
   }
   TUASSERTE(size_t, 90, uut.size());
      // add "ephemeris health"
   for (unsigned i = 0; i < 228; i++)
   {
         // health data
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, (i >= 57 ? SH :: Unhealthy : SH::Healthy),
              MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, (i >= 57 ? SH :: Unhealthy : SH::Healthy),
              MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, (i >= 57 ? SH :: Unhealthy : SH::Healthy),
              MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
         // ephemeris data
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, (i >= 57 ? SH :: Unhealthy : SH::Healthy),
              MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, (i >= 57 ? SH :: Unhealthy : SH::Healthy),
              MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, (i >= 57 ? SH :: Unhealthy : SH::Healthy),
              MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
   }
   TUASSERTE(size_t, 5562, uut.size());
   gnsstk::NavDataPtr result;
   gnsstk::GPSLNavHealth *hea;
      // check find almanac with no restrictions on health or validity
   TUASSERTE(bool, true,
             uut.find(findNMID1a,findTime1,result,SH::Any,VT::Any,SO::User));
   gnsstk::CommonTime expTime1(gnsstk::GPSWeekSecond(2101, 3804));
   TUASSERTE(gnsstk::CommonTime, expTime1, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.obs.band);
   TUASSERTE(TC, TC::CA, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // look for health data before and after the state transition
   TUASSERTE(bool, true,
             uut.find(findNMID1h,gnsstk::GPSWeekSecond(2101,1710),result,
                      SH::Any,VT::Any,SO::User));
   // result->dump(std::cerr, gnsstk::DumpDetail::Full);
   if (result)
   {
      hea = dynamic_cast<gnsstk::GPSLNavHealth*>(result.get());
      TUASSERTE(unsigned, 0, hea->svHealth);
   }
   TUASSERTE(bool, true,
             uut.find(findNMID1h,gnsstk::GPSWeekSecond(2101,1716),result,
                      SH::Any, VT::Any,SO::User));
   // result->dump(std::cerr, gnsstk::DumpDetail::Full);
   if (result)
   {
      hea = dynamic_cast<gnsstk::GPSLNavHealth*>(result.get());
      TUASSERTE(unsigned, 1, hea->svHealth);
   }
      // Look for healthy almanac data after the transition to
      // unhealthy.  This should result in ignoring the almanac
      // transmitted by PRN 1, since PRN 1 is unhealthy at this time,
      // and returning the almanac transmitted by PRN 3 instead.
   TUASSERTE(bool, true,
             uut.find(findNMID1a,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gnsstk::CommonTime, expTime1, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.obs.band);
   TUASSERTE(TC, TC::CA, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for healthy almanac data after the transition to
      // unhealthy with wildcards in the signal (band, code, nav).
      // Should yield the same results as above because of ordering.
   TUASSERTE(bool, true,
             uut.find(findNMID2a,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gnsstk::CommonTime, expTime1, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.obs.band);
   TUASSERTE(TC, TC::CA, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for almanac data after the transition to unhealthy where
      // we don't care about the health but we do specifically want
      // L2-Y, so we expect xmit PRN = 1, L2/Y/LNav
   TUASSERTE(bool, true,
             uut.find(findNMID3a,findTime1,result,SH::Any,VT::Any,
                      SO::User));
   TUASSERTE(gnsstk::CommonTime, expTime1, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.obs.band);
   TUASSERTE(TC, TC::Y, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for almanac data after the transition to unhealthy where
      // we do want healthy transmit satellites and do specifically
      // want L2-Y, so we expect xmit PRN = 3, L2/Y/LNav
   TUASSERTE(bool, true,
             uut.find(findNMID3a,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gnsstk::CommonTime, expTime1, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.obs.band);
   TUASSERTE(TC, TC::Y, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for almanac data after the transition to unhealthy where
      // we want the most recent almanac transmitted by PRN 1 when it
      // was healthy, so we expect xmit PRN = 1, L2/Y/LNav at time 2*750+54
   gnsstk::CommonTime expTime2(gnsstk::GPSWeekSecond(2101, 1554));
   TUASSERTE(bool, true,
             uut.find(findNMID4a,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gnsstk::CommonTime, expTime2, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.obs.band);
   TUASSERTE(TC, TC::Y, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for almanac data after the transition to unhealthy where
      // we want the most recent almanac transmitted by PRN 1 when it
      // was UNhealthy, so we expect xmit PRN = 1, L2/Y/LNav at time 5*750+54
   TUASSERTE(bool, true,
             uut.find(findNMID4a,findTime1,result,SH::Unhealthy,VT::Any,
                      SO::User));
   TUASSERTE(gnsstk::CommonTime, expTime1, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.obs.band);
   TUASSERTE(TC, TC::Y, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for almanac data transmitted by PRN 3 when it was
      // UNhealthy, which it isn't, so we expect no results.
   TUASSERTE(bool, false,
             uut.find(findNMID5a,findTime1,result,SH::Unhealthy,VT::Any,
                      SO::User));
      // Look for ephemeris data from PRN 1 L1 CA
   TUASSERTE(bool, true,
             uut.find(findNMID1L1CAe,findTime1,result,SH::Any,VT::Any,
                      SO::User));
   gnsstk::CommonTime expTime1Eph(gnsstk::GPSWeekSecond(2101, 3210));
   TUASSERTE(gnsstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.obs.band);
   TUASSERTE(TC, TC::CA, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 L1 CA (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID3L1CAe,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gnsstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 3, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.obs.band);
   TUASSERTE(TC, TC::CA, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 any any (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID3e,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gnsstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 3, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.obs.band);
   TUASSERTE(TC, TC::CA, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime1,result,SH::Any,VT::Any,
                      SO::User));
   TUASSERTE(gnsstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.obs.band);
   TUASSERTE(TC, TC::Y, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 L2 Y (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID3L2Ye,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gnsstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 3, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.obs.band);
   TUASSERTE(TC, TC::Y, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   gnsstk::CommonTime expTime2Eph(gnsstk::GPSWeekSecond(2100, 602880));
   TUASSERTE(gnsstk::CommonTime, expTime2Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.obs.band);
   TUASSERTE(TC, TC::Y, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y (Unhealthy)
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime1,result,SH::Unhealthy,VT::Any,
                      SO::User));
   TUASSERTE(gnsstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.obs.band);
   TUASSERTE(TC, TC::Y, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 L2 Y (Unhealthy)
   TUASSERTE(bool, false,
             uut.find(findNMID3L2Ye,findTime1,result,SH::Unhealthy,VT::Any,
                      SO::User));
   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
findNearestTest()
{
   TUDEF("NavDataFactoryWithStore", "findNearest");
   TestClass uut;
   using SS = gnsstk::SatelliteSystem;
   using CB = gnsstk::CarrierBand;
   using TC = gnsstk::TrackingCode;
   using NT = gnsstk::NavType;
   using SH = gnsstk::SVHealth;
   using MT = gnsstk::NavMessageType;
   using VT = gnsstk::NavValidityType;
   using SO = gnsstk::NavSearchOrder;
   gnsstk::CommonTime refsf1ct = gnsstk::GPSWeekSecond(2101, 0);
      // between copies 2 and 3, or i==57
   gnsstk::CommonTime unhealthyStart = gnsstk::GPSWeekSecond(2101, 1710);
   gnsstk::CommonTime refpg2ct = gnsstk::GPSWeekSecond(2101, 54);
   gnsstk::CommonTime findTime1 = gnsstk::GPSWeekSecond(2101, 3838);
   gnsstk::CommonTime findTime2 = gnsstk::GPSWeekSecond(2101, 3000);
   gnsstk::NavMessageID findNMID1a(
      gnsstk::NavSatelliteID(2, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Almanac);
   gnsstk::NavMessageID findNMID2a(
      gnsstk::NavSatelliteID(2, SS::GPS, CB::Any, TC::Any, NT::Any),
      MT::Almanac);
   gnsstk::NavMessageID findNMID3a(
      gnsstk::NavSatelliteID(2, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Almanac);
   gnsstk::NavMessageID findNMID4a(
      gnsstk::NavSatelliteID(2, 1, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Almanac);
   gnsstk::NavMessageID findNMID5a(
      gnsstk::NavSatelliteID(2, 3, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Almanac);
   gnsstk::NavMessageID findNMID1h(
      gnsstk::NavSatelliteID(1, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Health);
   gnsstk::NavMessageID findNMID1L1CAe(
      gnsstk::NavSatelliteID(1, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Ephemeris);
   gnsstk::NavMessageID findNMID1L2Ye(
      gnsstk::NavSatelliteID(1, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Ephemeris);
   gnsstk::NavMessageID findNMID3L1CAe(
      gnsstk::NavSatelliteID(3, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Ephemeris);
   gnsstk::NavMessageID findNMID3e(
      gnsstk::NavSatelliteID(3, SS::GPS, CB::Any, TC::Any, NT::Any),
      MT::Ephemeris);
   gnsstk::NavMessageID findNMID3L2Ye(
      gnsstk::NavSatelliteID(3, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Ephemeris);
      // fill with "almanac pages"
   for (unsigned i = 0; i <= 10; i++)
   {
      addData(testFramework, uut, refpg2ct + (750*i), 2, 1, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 1, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 1, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 3, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 3, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 3, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 4, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 4, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Almanac);
      addData(testFramework, uut, refpg2ct + (750*i), 2, 4, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Almanac);
   }
   TUASSERTE(size_t, 99, uut.size());
      // add "ephemeris health"
   for (unsigned i = 0; i <= 251; i++)
   {
         // health data
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV,
              ((i >= 57 && i < 240) ? SH :: Unhealthy : SH::Healthy),
              MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV,
              ((i >= 57 && i < 240) ? SH :: Unhealthy : SH::Healthy),
              MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV,
              ((i >= 57 && i < 240) ? SH :: Unhealthy : SH::Healthy),
              MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
         // ephemeris data
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV,
              ((i >= 57 && i < 240) ? SH :: Unhealthy : SH::Healthy),
              MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV,
              ((i >= 57 && i < 240) ? SH :: Unhealthy : SH::Healthy),
              MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV,
              ((i >= 57 && i < 240) ? SH :: Unhealthy : SH::Healthy),
              MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
   }
   TUASSERTE(size_t, 6147, uut.size());
   gnsstk::NavDataPtr result;
   gnsstk::GPSLNavHealth *hea;
      /// @todo add tests for findNearest using Almanac data and maybe Health.
      // Look for ephemeris data from PRN 1 L1 CA
   TUASSERTE(bool, true,
             uut.find(findNMID1L1CAe,findTime1,result,SH::Any,VT::Any,
                      SO::Nearest));
   gnsstk::CommonTime expTime1Eph(gnsstk::GPSWeekSecond(2101, 3600));
   TUASSERTE(gnsstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.obs.band);
   TUASSERTE(TC, TC::CA, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 L1 CA (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID3L1CAe,findTime1,result,SH::Healthy,VT::Any,
                      SO::Nearest));
   TUASSERTE(gnsstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 3, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.obs.band);
   TUASSERTE(TC, TC::CA, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 any any (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID3e,findTime1,result,SH::Healthy,VT::Any,
                      SO::Nearest));
   TUASSERTE(gnsstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 3, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.obs.band);
   TUASSERTE(TC, TC::CA, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime1,result,SH::Any,VT::Any,
                      SO::Nearest));
   TUASSERTE(gnsstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.obs.band);
   TUASSERTE(TC, TC::Y, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 L2 Y (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID3L2Ye,findTime1,result,SH::Healthy,VT::Any,
                      SO::Nearest));
   TUASSERTE(gnsstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 3, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.obs.band);
   TUASSERTE(TC, TC::Y, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime1,result,SH::Healthy,VT::Any,
                      SO::Nearest));
   TUASSERTE(gnsstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.obs.band);
   TUASSERTE(TC, TC::Y, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y (Healthy) at an earlier time
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime2,result,SH::Healthy,VT::Any,
                      SO::Nearest));
   gnsstk::CommonTime expTime2Eph(gnsstk::GPSWeekSecond(2100, 601200));
   TUASSERTE(gnsstk::CommonTime, expTime2Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.obs.band);
   TUASSERTE(TC, TC::Y, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y (Unhealthy)
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime1,result,SH::Unhealthy,VT::Any,
                      SO::Nearest));
      // this is the first of the unhealthy data
   gnsstk::CommonTime expTime3Eph(gnsstk::GPSWeekSecond(2100, 602910));
   TUASSERTE(gnsstk::CommonTime, expTime3Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.obs.band);
   TUASSERTE(TC, TC::Y, result->signal.obs.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 L2 Y (Unhealthy)
   TUASSERTE(bool, false,
             uut.find(findNMID3L2Ye,findTime1,result,SH::Unhealthy,VT::Any,
                      SO::Nearest));
   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
getOffsetTest()
{
   TUDEF("NavDataFactoryWithStore", "getOffset");
   TestClass fact1;
   gnsstk::NavDataPtr navOut = std::make_shared<gnsstk::GPSLNavTimeOffset>();
   navOut->timeStamp = ct;
   navOut->signal.messageType = gnsstk::NavMessageType::TimeOffset;
   gnsstk::GPSLNavTimeOffset *toptr = dynamic_cast<gnsstk::GPSLNavTimeOffset*>(
      navOut.get());
   fillSat(navOut->signal, 23, 23);
   toptr->deltatLS = 23; // set a simple, easy to verify value.
   toptr->refTime = ct;
   TUASSERT(fact1.addNavData(navOut));
   gnsstk::TimeOffsetData *top;
   gnsstk::NavDataPtr result;
   double offset;
   TUASSERT(fact1.getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC,
                            ct+35, result, gnsstk::SVHealth::Any,
                            gnsstk::NavValidityType::Any));
   top = dynamic_cast<gnsstk::TimeOffsetData*>(result.get());
   TUASSERT(top->getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC,
                           ct+35, offset));
   TUASSERTFE(23.0, offset);
      // reverse the conversion and expect negative.
   TUASSERT(fact1.getOffset(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::GPS,
                            ct+35, result, gnsstk::SVHealth::Any,
                            gnsstk::NavValidityType::Any));
   top = dynamic_cast<gnsstk::TimeOffsetData*>(result.get());
   gnsstk::CommonTime utc35(ct+35);
   utc35.setTimeSystem(gnsstk::TimeSystem::UTC);
   TUASSERT(top->getOffset(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::GPS,
                           utc35, offset));
   TUASSERTFE(-23.0, offset);
      // expect this to not work
   TUASSERT(!fact1.getOffset(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::BDT,
                             utc35, result, gnsstk::SVHealth::Any,
                             gnsstk::NavValidityType::Any));
   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
getOffset2Test()
{
   TUDEF("NavDataFactoryWithStore", "getOffset");
   TestClass uut;
   using SS = gnsstk::SatelliteSystem;
   using CB = gnsstk::CarrierBand;
   using TC = gnsstk::TrackingCode;
   using NT = gnsstk::NavType;
   using SH = gnsstk::SVHealth;
   using MT = gnsstk::NavMessageType;
   using VT = gnsstk::NavValidityType;
   using SO = gnsstk::NavSearchOrder;
   gnsstk::CommonTime refsf1ct = gnsstk::GPSWeekSecond(2101, 0);
      // between copies 2 and 3, or i==57
   gnsstk::CommonTime unhealthyStart = gnsstk::GPSWeekSecond(2101, 1710);
   gnsstk::CommonTime refpg56ct = gnsstk::GPSWeekSecond(2101, 528);
   gnsstk::CommonTime findTime1 = gnsstk::GPSWeekSecond(2101, 3838);
   gnsstk::CommonTime findTime2 = gnsstk::GPSWeekSecond(2101, 0);
   gnsstk::CommonTime findTime3 = gnsstk::GPSWeekSecond(2101, 7300);
      // fill with "almanac pages"
   for (unsigned i = 0; i < 10; i++)
   {
      addData(testFramework, uut, refpg56ct + (750*i), 2, 1, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::TimeOffset);
      addData(testFramework, uut, refpg56ct + (750*i), 2, 1, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::TimeOffset);
      addData(testFramework, uut, refpg56ct + (750*i), 2, 1, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::TimeOffset);
      addData(testFramework, uut, refpg56ct + (750*i), 2, 3, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::TimeOffset);
      addData(testFramework, uut, refpg56ct + (750*i), 2, 3, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::TimeOffset);
      addData(testFramework, uut, refpg56ct + (750*i), 2, 3, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::TimeOffset);
      addData(testFramework, uut, refpg56ct + (750*i), 2, 4, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::TimeOffset);
      addData(testFramework, uut, refpg56ct + (750*i), 2, 4, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::TimeOffset);
      addData(testFramework, uut, refpg56ct + (750*i), 2, 4, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::TimeOffset);
   }
   TUASSERTE(size_t, 90, uut.size());
      // add "ephemeris health"
   for (unsigned i = 0; i < 228; i++)
   {
         // health data
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, (i >= 57 ? SH :: Unhealthy : SH::Healthy),
              MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, (i >= 57 ? SH :: Unhealthy : SH::Healthy),
              MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, (i >= 57 ? SH :: Unhealthy : SH::Healthy),
              MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Health);
         // ephemeris data
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, (i >= 57 ? SH :: Unhealthy : SH::Healthy),
              MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, (i >= 57 ? SH :: Unhealthy : SH::Healthy),
              MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 1, 1, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, (i >= 57 ? SH :: Unhealthy : SH::Healthy),
              MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 2, 2, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 3, 3, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L1,
              TC::CA, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L1,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
      addData(testFramework, uut, refsf1ct + (30*i), 4, 4, SS::GPS, CB::L2,
              TC::Y, NT::GPSLNAV, SH::Healthy, MT::Ephemeris);
   }
   TUASSERTE(size_t, 5562, uut.size());
   gnsstk::TimeOffsetData *top;
   gnsstk::NavDataPtr result;
   double offset;
      // check getOffset with no restrictions on health or validity
   TUASSERTE(bool, true,
             uut.getOffset(gnsstk::TimeSystem::GPS,gnsstk::TimeSystem::UTC,
                           findTime1,result,SH::Any,VT::Any));
   top = dynamic_cast<gnsstk::TimeOffsetData*>(result.get());
   TUASSERT(top->getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC,
                           findTime1, offset));
   TUASSERTFE(1, offset); // PRN == offset == 1
      // check getOffset requiring data from a healthy SV
   TUASSERTE(bool, true,
             uut.getOffset(gnsstk::TimeSystem::GPS,gnsstk::TimeSystem::UTC,
                           findTime1,result,SH::Healthy,VT::Any));
   top = dynamic_cast<gnsstk::TimeOffsetData*>(result.get());
   TUASSERT(top->getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC,
                           findTime1, offset));
   TUASSERTFE(3, offset); // PRN == offset == 3
      // check getOffset with time prior to earliest data
   TUASSERTE(bool, false,
             uut.getOffset(gnsstk::TimeSystem::GPS,gnsstk::TimeSystem::UTC,
                           findTime2,result,SH::Any,VT::Any));
      // check getOffset with time after most recent data
   TUASSERTE(bool, true,
             uut.getOffset(gnsstk::TimeSystem::GPS,gnsstk::TimeSystem::UTC,
                           findTime3,result,SH::Any,VT::Any));
   top = dynamic_cast<gnsstk::TimeOffsetData*>(result.get());
   TUASSERT(top->getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC,
                           findTime3, offset));
   TUASSERTFE(1, offset); // PRN == offset == 1
   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
editTest()
{
   TUDEF("NavDataFactoryWithStore", "edit");

      // First check the plain edit
   TestClass fact1;
   gnsstk::CommonTime ect(ct-3600); // make the base time match the ephemeris
   TUCATCH(fillFactory(testFramework, fact1));
      // verify initial conditions
   TUASSERTE(size_t, 8, fact1.size());
   TUASSERTE(size_t, 2, fact1.numSignals());
   TUASSERTE(size_t, 3, fact1.numSatellites());
   TUASSERTE(size_t, 8, fact1.sizeNearest());
   TUASSERTE(size_t, 3, fact1.numSatellitesNearest());
      // remove nothing
   TUCATCH(fact1.edit(gnsstk::CommonTime::BEGINNING_OF_TIME, ect));
   TUASSERTE(size_t, 8, fact1.size());
   TUASSERTE(size_t, 2, fact1.numSignals());
   TUASSERTE(size_t, 3, fact1.numSatellites());
   TUASSERTE(size_t, 8, fact1.sizeNearest());
   TUASSERTE(size_t, 3, fact1.numSatellitesNearest());
   checkForEmpty(testFramework, fact1);
      // remove messages at ect
   TUCATCH(fact1.edit(gnsstk::CommonTime::BEGINNING_OF_TIME, ect+30));
   TUASSERTE(size_t, 5, fact1.size());
   TUASSERTE(size_t, 2, fact1.numSignals());
   TUASSERTE(size_t, 3, fact1.numSatellites());
   TUASSERTE(size_t, 5, fact1.sizeNearest());
   TUASSERTE(size_t, 3, fact1.numSatellitesNearest());
   checkForEmpty(testFramework, fact1);
      // remove messages at ect+30
   TUCATCH(fact1.edit(gnsstk::CommonTime::BEGINNING_OF_TIME, ect+60));
   TUASSERTE(size_t, 2, fact1.size());
   TUASSERTE(size_t, 1, fact1.numSignals());
   TUASSERTE(size_t, 1, fact1.numSatellites());
   TUASSERTE(size_t, 2, fact1.sizeNearest());
   TUASSERTE(size_t, 1, fact1.numSatellitesNearest());
   checkForEmpty(testFramework, fact1);
      // remove messages at ect+60
   TUCATCH(fact1.edit(gnsstk::CommonTime::BEGINNING_OF_TIME, ect+90));
   TUASSERTE(size_t, 1, fact1.size());
   TUASSERTE(size_t, 1, fact1.numSignals());
   TUASSERTE(size_t, 1, fact1.numSatellites());
   TUASSERTE(size_t, 1, fact1.sizeNearest());
   TUASSERTE(size_t, 1, fact1.numSatellitesNearest());
   checkForEmpty(testFramework, fact1);
      // remove messages at ect+90
   TUCATCH(fact1.edit(gnsstk::CommonTime::BEGINNING_OF_TIME, ect+120));
   TUASSERTE(size_t, 0, fact1.size());
   TUASSERTE(size_t, 0, fact1.numSignals());
   TUASSERTE(size_t, 0, fact1.numSatellites());
   TUASSERTE(size_t, 0, fact1.sizeNearest());
   TUASSERTE(size_t, 0, fact1.numSatellitesNearest());
   checkForEmpty(testFramework, fact1);

      // Now check satellite editing
   TestClass fact2;
   gnsstk::NavSatelliteID satID2a, satID2b, satID2c, satID2d, satID2e;
   TUCATCH(fillFactory(testFramework, fact2));
   TUCATCH(fillSat(satID2a, 7, 7));
   TUCATCH(fillSat(satID2b, 99, 99));
   TUCATCH(fillSat(satID2c, 7, 7, gnsstk::SatelliteSystem::GPS,
                   gnsstk::CarrierBand::L5));
   TUCATCH(fillSat(satID2d, 11, 11, gnsstk::SatelliteSystem::GPS,
                   gnsstk::CarrierBand::L1, gnsstk::TrackingCode::Y));
   TUCATCH(fillSat(satID2e, 23, 32));
      // verify initial conditions
   TUASSERTE(size_t, 8, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 3, fact2.numSatellites());
   TUASSERTE(size_t, 8, fact2.sizeNearest());
   TUASSERTE(size_t, 3, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove nothing
   TUCATCH(fact2.edit(gnsstk::CommonTime::BEGINNING_OF_TIME, ect, satID2a));
   TUASSERTE(size_t, 8, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 3, fact2.numSatellites());
   TUASSERTE(size_t, 8, fact2.sizeNearest());
   TUASSERTE(size_t, 3, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove nothing, this time because the satellite ID isn't present
   TUCATCH(fact2.edit(gnsstk::CommonTime::BEGINNING_OF_TIME,
                      gnsstk::CommonTime::END_OF_TIME, satID2b));
   TUASSERTE(size_t, 8, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 3, fact2.numSatellites());
   TUASSERTE(size_t, 8, fact2.sizeNearest());
   TUASSERTE(size_t, 3, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove nothing, this time because the signal isn't present
   TUCATCH(fact2.edit(gnsstk::CommonTime::BEGINNING_OF_TIME,
                      gnsstk::CommonTime::END_OF_TIME, satID2c));
   TUASSERTE(size_t, 8, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 3, fact2.numSatellites());
   TUASSERTE(size_t, 8, fact2.sizeNearest());
   TUASSERTE(size_t, 3, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove all sat 7 data
   TUCATCH(fact2.edit(gnsstk::CommonTime::BEGINNING_OF_TIME,
                      gnsstk::CommonTime::END_OF_TIME, satID2a));
   TUASSERTE(size_t, 6, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 2, fact2.numSatellites());
   TUASSERTE(size_t, 6, fact2.sizeNearest());
   TUASSERTE(size_t, 2, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove all sat 11 data
   TUCATCH(fact2.edit(gnsstk::CommonTime::BEGINNING_OF_TIME,
                      gnsstk::CommonTime::END_OF_TIME, satID2d));
   TUASSERTE(size_t, 4, fact2.size());
   TUASSERTE(size_t, 1, fact2.numSignals());
   TUASSERTE(size_t, 1, fact2.numSatellites());
   TUASSERTE(size_t, 4, fact2.sizeNearest());
   TUASSERTE(size_t, 1, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove some of sat 23 data
   TUCATCH(fact2.edit(gnsstk::CommonTime::BEGINNING_OF_TIME, ect+30, satID2e));
   TUASSERTE(size_t, 3, fact2.size());
   TUASSERTE(size_t, 1, fact2.numSignals());
   TUASSERTE(size_t, 1, fact2.numSatellites());
   TUASSERTE(size_t, 3, fact2.sizeNearest());
   TUASSERTE(size_t, 1, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove the rest of sat 23 data
   TUCATCH(fact2.edit(gnsstk::CommonTime::BEGINNING_OF_TIME,
                      gnsstk::CommonTime::END_OF_TIME, satID2e));
   TUASSERTE(size_t, 0, fact2.size());
   TUASSERTE(size_t, 0, fact2.numSignals());
   TUASSERTE(size_t, 0, fact2.numSatellites());
   TUASSERTE(size_t, 0, fact2.sizeNearest());
   TUASSERTE(size_t, 0, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);

      // finally, check editing by signal
   TestClass fact3;
   gnsstk::NavSignalID sig3a, sig3b, sig3c;
   TUCATCH(fillFactory(testFramework, fact3));
   TUCATCH(fillSignal(sig3a));
   TUCATCH(fillSignal(sig3b, gnsstk::SatelliteSystem::Glonass));
   TUCATCH(fillSignal(sig3c, gnsstk::SatelliteSystem::GPS,
                      gnsstk::CarrierBand::L1, gnsstk::TrackingCode::Y));
      // verify initial conditions
   TUASSERTE(size_t, 8, fact3.size());
   TUASSERTE(size_t, 2, fact3.numSignals());
   TUASSERTE(size_t, 3, fact3.numSatellites());
   TUASSERTE(size_t, 8, fact3.sizeNearest());
   TUASSERTE(size_t, 3, fact3.numSatellitesNearest());
   checkForEmpty(testFramework, fact3);
      // remove nothing
   TUCATCH(fact3.edit(gnsstk::CommonTime::BEGINNING_OF_TIME, ect, sig3a));
   TUASSERTE(size_t, 8, fact3.size());
   TUASSERTE(size_t, 2, fact3.numSignals());
   TUASSERTE(size_t, 3, fact3.numSatellites());
   TUASSERTE(size_t, 8, fact3.sizeNearest());
   TUASSERTE(size_t, 3, fact3.numSatellitesNearest());
   checkForEmpty(testFramework, fact3);
      // remove nothing, this time because the signal isn't present
   TUCATCH(fact3.edit(gnsstk::CommonTime::BEGINNING_OF_TIME,
                      gnsstk::CommonTime::END_OF_TIME, sig3b));
   TUASSERTE(size_t, 8, fact3.size());
   TUASSERTE(size_t, 2, fact3.numSignals());
   TUASSERTE(size_t, 3, fact3.numSatellites());
   TUASSERTE(size_t, 8, fact3.sizeNearest());
   TUASSERTE(size_t, 3, fact3.numSatellitesNearest());
   checkForEmpty(testFramework, fact3);
      // remove all L1-Y data
   TUCATCH(fact3.edit(gnsstk::CommonTime::BEGINNING_OF_TIME,
                      gnsstk::CommonTime::END_OF_TIME, sig3c));
   TUASSERTE(size_t, 6, fact3.size());
   TUASSERTE(size_t, 1, fact3.numSignals());
   TUASSERTE(size_t, 2, fact3.numSatellites());
   TUASSERTE(size_t, 6, fact3.sizeNearest());
   TUASSERTE(size_t, 2, fact3.numSatellitesNearest());
   checkForEmpty(testFramework, fact3);
      // remove some of L1-CA data
   TUCATCH(fact3.edit(gnsstk::CommonTime::BEGINNING_OF_TIME, ect+30, sig3a));
   TUASSERTE(size_t, 4, fact3.size());
   TUASSERTE(size_t, 1, fact3.numSignals());
   TUASSERTE(size_t, 2, fact3.numSatellites());
   TUASSERTE(size_t, 4, fact3.sizeNearest());
   TUASSERTE(size_t, 2, fact3.numSatellitesNearest());
   checkForEmpty(testFramework, fact3);
      // remove the rest of L1-CA data
   TUCATCH(fact3.edit(gnsstk::CommonTime::BEGINNING_OF_TIME,
                      gnsstk::CommonTime::END_OF_TIME, sig3a));
   TUASSERTE(size_t, 0, fact3.size());
   TUASSERTE(size_t, 0, fact3.numSignals());
   TUASSERTE(size_t, 0, fact3.numSatellites());
   TUASSERTE(size_t, 0, fact3.sizeNearest());
   TUASSERTE(size_t, 0, fact3.numSatellitesNearest());
   checkForEmpty(testFramework, fact3);

   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
clearTest()
{
   TUDEF("NavDataFactoryWithStore", "clear");

   TestClass fact;

      // NavData is abstract so we instantiate a GPSLNavEph instead
   gnsstk::NavDataPtr navOut = std::make_shared<gnsstk::GPSLNavEph>();
   navOut->timeStamp = ct;
   navOut->signal.messageType = gnsstk::NavMessageType::Ephemeris;
   navOut->signal.sat = gnsstk::SatID(23,gnsstk::SatelliteSystem::GPS);
   navOut->signal.xmitSat = gnsstk::SatID(32,gnsstk::SatelliteSystem::GPS);
   navOut->signal.system = gnsstk::SatelliteSystem::GPS;
   navOut->signal.obs = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                                     gnsstk::CarrierBand::L1,
                                     gnsstk::TrackingCode::CA);
   navOut->signal.nav = gnsstk::NavType::GPSLNAV;

   TUASSERT(fact.addNavData(navOut));
   TUASSERTE(size_t, 1, fact.size());
   TUASSERTE(size_t, 1, fact.sizeNearest());

   fact.clear();
   TUASSERTE(size_t, 0, fact.size());
   TUASSERTE(size_t, 0, fact.sizeNearest());
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::BEGINNING_OF_TIME,
             fact.getFinalTime());
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::END_OF_TIME,
             fact.getInitialTime());

   TURETURN();
}


void NavDataFactoryWithStore_T ::
fillFactory(gnsstk::TestUtil& testFramework, TestClass& fact)
{
   addData(testFramework, fact, ct+0,  23, 32);
   addData(testFramework, fact, ct+0,   7, 7);
   addData(testFramework, fact, ct+0,  11, 11, gnsstk::SatelliteSystem::GPS,
           gnsstk::CarrierBand::L1, gnsstk::TrackingCode::Y);
   addData(testFramework, fact, ct+30, 23, 32);
   addData(testFramework, fact, ct+30,  7, 7);
   addData(testFramework, fact, ct+30, 11, 11, gnsstk::SatelliteSystem::GPS,
           gnsstk::CarrierBand::L1, gnsstk::TrackingCode::Y);
   addData(testFramework, fact, ct+60, 23, 32);
   addData(testFramework, fact, ct+90, 23, 32);
}


void NavDataFactoryWithStore_T ::
addData(gnsstk::TestUtil& testFramework, TestClass& fact,
        const gnsstk::CommonTime& ct, unsigned long sat,
        unsigned long xmitSat, gnsstk::SatelliteSystem sys,
        gnsstk::CarrierBand car, gnsstk::TrackingCode code, gnsstk::NavType nav,
        gnsstk::SVHealth hea, gnsstk::NavMessageType nmt)
{
   gnsstk::NavDataPtr navOut;
   gnsstk::GPSWeekSecond toe = ct;
   if (nmt == gnsstk::NavMessageType::Ephemeris)
   {
      navOut = std::make_shared<gnsstk::GPSLNavEph>();
      navOut->timeStamp = ct-3600;
      dynamic_cast<gnsstk::OrbitDataKepler*>(navOut.get())->health = hea;
      toe.sow -= fmod(toe.sow,7200);
      dynamic_cast<gnsstk::OrbitDataKepler*>(navOut.get())->Toe = toe;
      dynamic_cast<gnsstk::OrbitDataKepler*>(navOut.get())->Toc = toe;
      dynamic_cast<gnsstk::OrbitDataKepler*>(navOut.get())->xmitTime = ct-3600;
      dynamic_cast<gnsstk::GPSLNavEph*>(navOut.get())->xmit2 = ct-3594;
      dynamic_cast<gnsstk::GPSLNavEph*>(navOut.get())->xmit3 = ct-3588;
      dynamic_cast<gnsstk::GPSLNavEph*>(navOut.get())->fixFit();
   }
   else if (nmt == gnsstk::NavMessageType::Almanac)
   {
      navOut = std::make_shared<gnsstk::GPSLNavAlm>();
      navOut->timeStamp = ct;
      dynamic_cast<gnsstk::OrbitDataKepler*>(navOut.get())->health = hea;
      toe.sow = toe.sow - fmod(toe.sow,86400) + (xmitSat == 3 ? 61000 : 61056);
      dynamic_cast<gnsstk::OrbitDataKepler*>(navOut.get())->Toe = toe;
      dynamic_cast<gnsstk::OrbitDataKepler*>(navOut.get())->xmitTime = ct;
      dynamic_cast<gnsstk::GPSLNavAlm*>(navOut.get())->fixFit();
   }
   else if (nmt == gnsstk::NavMessageType::Health)
   {
      navOut = std::make_shared<gnsstk::GPSLNavHealth>();
      navOut->timeStamp = ct;
      gnsstk::GPSLNavHealth *hp =
         dynamic_cast<gnsstk::GPSLNavHealth*>(navOut.get());
      switch (hea)
      {
         case gnsstk::SVHealth::Healthy:
            hp->svHealth = 0;
            break;
         case gnsstk::SVHealth::Unhealthy:
            hp->svHealth = 1;
            break;
         default:
            TUFAIL("GPS can't handle health type " +
                   gnsstk::StringUtils::asString(hea));
            break;
      }
   }
   else if (nmt == gnsstk::NavMessageType::TimeOffset)
   {
      navOut = std::make_shared<gnsstk::GPSLNavTimeOffset>();
      navOut->timeStamp = ct;
      gnsstk::GPSLNavTimeOffset *to =
         dynamic_cast<gnsstk::GPSLNavTimeOffset*>(navOut.get());
         // Set deltatLS to the transmitting satellite which gives us
         // a means to determine if the expected time offset message
         // was used.
      to->deltatLS = xmitSat;
         // to->a0 = 0.0;
         // to->a1 = 0.0;
         // to->tot = 0.0;
         // to->wnt = 0;
         // to->wnLSF = 0;
         // to->dn = 0;
         // to->deltatLSF = 0.0;
      to->refTime = gnsstk::GPSWeekSecond(0,0);
   }
   navOut->signal.messageType = nmt;
   fillSat(navOut->signal, sat, xmitSat, sys, car, code, nav);
   TUASSERT(fact.addNavData(navOut));
}


void NavDataFactoryWithStore_T ::
fillSignal(gnsstk::NavSignalID& signal, gnsstk::SatelliteSystem sys,
           gnsstk::CarrierBand car, gnsstk::TrackingCode code, gnsstk::NavType nav)
{
   signal.system = sys;
   signal.obs = gnsstk::ObsID(gnsstk::ObservationType::NavMsg,car,code);
   signal.nav = nav;
}


void NavDataFactoryWithStore_T ::
fillSat(gnsstk::NavSatelliteID& satellite,
        unsigned long sat, unsigned long xmitSat,
        gnsstk::SatelliteSystem sys, gnsstk::CarrierBand car,
        gnsstk::TrackingCode code, gnsstk::NavType nav)
{
   fillSignal(satellite, sys, car, code, nav);
   satellite.sat = gnsstk::SatID(sat,sys);
   satellite.xmitSat = gnsstk::SatID(xmitSat,sys);
}


void NavDataFactoryWithStore_T ::
checkForEmpty(gnsstk::TestUtil& testFramework, TestClass& fact)
{
   gnsstk::NavMessageMap &data(fact.getData());
   for (auto& nmmi : data)
   {
      TUASSERT(!nmmi.second.empty());
      for (auto& sati : nmmi.second)
      {
         TUASSERT(!sati.second.empty());
      }
   }
   gnsstk::NavNearMessageMap &nearestData(fact.getNearestData());
   for (auto& nnmmi : nearestData)
   {
      TUASSERT(!nnmmi.second.empty());
      for (auto& nnsmi : nnmmi.second)
      {
         TUASSERT(!nnsmi.second.empty());
         for (auto& nnmi : nnsmi.second)
         {
            TUASSERT(!nnmi.second.empty());
         }
      }
   }
}


void NavDataFactoryWithStore_T ::
fillFactoryXmitHealth(gnsstk::TestUtil& testFramework, TestClass& fact)
{
      // test almanac for prn 5 (subframe 5 page 5)
      // one copy broadcast by prn 1 which is healthy
      // one copy broadcast by prn 2 which is unhealthy
      // first add PRN 1 ephemeris
   addData(testFramework, fact, ct, 1, 1);
      // then add PRN 2 unhealthy ephemeris
   addData(testFramework, fact, ct, 2, 2, gnsstk::SatelliteSystem::GPS,
           gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
           gnsstk::NavType::GPSLNAV, gnsstk::SVHealth::Unhealthy);
      // add the almanac data... Note the unhealthy here refers to PRN 5
   addData(testFramework, fact, ct5, 5, 2, gnsstk::SatelliteSystem::GPS,
           gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
           gnsstk::NavType::GPSLNAV, gnsstk::SVHealth::Unhealthy,
           gnsstk::NavMessageType::Almanac);
   addData(testFramework, fact, ct5, 5, 1, gnsstk::SatelliteSystem::GPS,
           gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
           gnsstk::NavType::GPSLNAV, gnsstk::SVHealth::Unhealthy,
           gnsstk::NavMessageType::Almanac);
      // finally, add health data, without which the find method won't
      // give the expected results when searching for specific health status
   addData(testFramework, fact, ct, 1, 1, gnsstk::SatelliteSystem::GPS,
           gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
           gnsstk::NavType::GPSLNAV, gnsstk::SVHealth::Healthy,
           gnsstk::NavMessageType::Health);
   addData(testFramework, fact, ct, 2, 2, gnsstk::SatelliteSystem::GPS,
           gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
           gnsstk::NavType::GPSLNAV, gnsstk::SVHealth::Unhealthy,
           gnsstk::NavMessageType::Health);
}


unsigned NavDataFactoryWithStore_T ::
findXmitHealthTest()
{
   TUDEF("NavDataFactoryWithStore", "find");
   TestClass fact1;
   gnsstk::NavMessageID nmid1a;
   gnsstk::NavDataPtr result;
   TUCATCH(fillFactoryXmitHealth(testFramework, fact1));
   TUASSERTE(size_t, 6, fact1.size());
   TUCATCH(fillSat(nmid1a, 5, 0));
   nmid1a.xmitSat.wildId = true;
   nmid1a.messageType = gnsstk::NavMessageType::Almanac;
      // make sure we get something with no filters
   TUASSERT(fact1.find(nmid1a, ct5+7, result, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::Any,
                       gnsstk::NavSearchOrder::User));
   result.reset();
      // ask for almanac data from a healthy SV
   TUASSERT(fact1.find(nmid1a, ct5+7, result, gnsstk::SVHealth::Healthy,
                       gnsstk::NavValidityType::Any,
                       gnsstk::NavSearchOrder::User));
      // we asked for almanac from healthy SV, so we should have
      // gotten the data from PRN 1
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   result.reset();
      // ask for almanac data from an unhealthy SV
   TUASSERT(fact1.find(nmid1a, ct5+7, result, gnsstk::SVHealth::Unhealthy,
                       gnsstk::NavValidityType::Any,
                       gnsstk::NavSearchOrder::User));
      // we asked for almanac from unhealthy SV, so we should have
      // gotten the data from PRN 2
   TUASSERTE(int, 2, result->signal.xmitSat.id);
   result.reset();
   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
getAvailableSatsTest()
{
   TUDEF("NavDataFactoryWithStore", "getAvailableSats");
   TestClass uut;
   TUCATCH(fillFactory(testFramework, uut));
   gnsstk::NavSatelliteIDSet satset;
   gnsstk::NavSatelliteID sat1, sat2, sat3;
   fillSat(sat1, 23, 32);
   fillSat(sat2, 7, 7);
   fillSat(sat3, 11, 11, gnsstk::SatelliteSystem::GPS,
           gnsstk::CarrierBand::L1, gnsstk::TrackingCode::Y);
      // test over entire time span
   TUCATCH(satset = uut.getAvailableSats(
              gnsstk::CommonTime::BEGINNING_OF_TIME,
              gnsstk::CommonTime::END_OF_TIME));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat1));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat2));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat3));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 3, satset.size());
   TUCATCH(satset = uut.getAvailableSats(
              gnsstk::NavMessageType::Ephemeris,
              gnsstk::CommonTime::BEGINNING_OF_TIME,
              gnsstk::CommonTime::END_OF_TIME));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat1));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat2));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat3));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 3, satset.size());
   TUCATCH(satset = uut.getAvailableSats(
              gnsstk::NavMessageType::Almanac,
              gnsstk::CommonTime::BEGINNING_OF_TIME,
              gnsstk::CommonTime::END_OF_TIME));
   TUASSERTE(bool, true, satset.empty());
      // test with time span before any data
   TUCATCH(satset = uut.getAvailableSats(
              gnsstk::CivilTime(2020,4,12,0,56,0,gnsstk::TimeSystem::GPS),
              gnsstk::CivilTime(2020,4,12,0,57,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, satset.empty());
      // test with time span after all data
   TUCATCH(satset = uut.getAvailableSats(
              gnsstk::CivilTime(2020,4,12,1,0,0,gnsstk::TimeSystem::GPS),
              gnsstk::CivilTime(2020,4,12,2,0,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, satset.empty());
      // test with a time span that will get all satellites even
      // though it's only partial coverage.
   TUCATCH(satset = uut.getAvailableSats(
              gnsstk::CivilTime(2020,4,11,23,56,0,gnsstk::TimeSystem::GPS),
              gnsstk::CivilTime(2020,4,11,23,58,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat1));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat2));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat3));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 3, satset.size());
      // test with a time span that will only get one satellite
   TUCATCH(satset = uut.getAvailableSats(
              gnsstk::CivilTime(2020,4,11,23,59,0,gnsstk::TimeSystem::GPS),
              gnsstk::CivilTime(2020,4,12,0,0,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat1));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.size());
      // test with a time span that is in the middle of the data time
      // span, but without any matches
   TUCATCH(satset = uut.getAvailableSats(
              gnsstk::CivilTime(2020,4,11,23,57,50,gnsstk::TimeSystem::GPS),
              gnsstk::CivilTime(2020,4,11,23,58,10,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, satset.empty());
   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
getIndexSetTest()
{
   TUDEF("NavDataFactoryWithStore", "getIndexSet");
   TestClass uut;
   TUCATCH(fillFactory(testFramework, uut));
   std::set<gnsstk::SatID> satset;
   gnsstk::SatID
      sat1(23,gnsstk::SatelliteSystem::GPS),
      sat2(7,gnsstk::SatelliteSystem::GPS),
      sat3(11,gnsstk::SatelliteSystem::GPS);
      // test over entire time span
   TUCATCH(satset = uut.getIndexSet(
              gnsstk::CommonTime::BEGINNING_OF_TIME,
              gnsstk::CommonTime::END_OF_TIME));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat1));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat2));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat3));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 3, satset.size());
   TUCATCH(satset = uut.getIndexSet(
              gnsstk::NavMessageType::Ephemeris,
              gnsstk::CommonTime::BEGINNING_OF_TIME,
              gnsstk::CommonTime::END_OF_TIME));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat1));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat2));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat3));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 3, satset.size());
   TUCATCH(satset = uut.getIndexSet(
              gnsstk::NavMessageType::Almanac,
              gnsstk::CommonTime::BEGINNING_OF_TIME,
              gnsstk::CommonTime::END_OF_TIME));
   TUASSERTE(bool, true, satset.empty());
      // test with time span before any data
   TUCATCH(satset = uut.getIndexSet(
              gnsstk::CivilTime(2020,4,12,0,56,0,gnsstk::TimeSystem::GPS),
              gnsstk::CivilTime(2020,4,12,0,57,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, satset.empty());
      // test with time span after all data
   TUCATCH(satset = uut.getIndexSet(
              gnsstk::CivilTime(2020,4,12,1,0,0,gnsstk::TimeSystem::GPS),
              gnsstk::CivilTime(2020,4,12,2,0,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, satset.empty());
      // test with a time span that will get all satellites even
      // though it's only partial coverage.
   TUCATCH(satset = uut.getIndexSet(
              gnsstk::CivilTime(2020,4,11,23,56,0,gnsstk::TimeSystem::GPS),
              gnsstk::CivilTime(2020,4,11,23,58,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat1));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat2));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat3));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 3, satset.size());
      // test with a time span that will only get one satellite
   TUCATCH(satset = uut.getIndexSet(
              gnsstk::CivilTime(2020,4,11,23,59,0,gnsstk::TimeSystem::GPS),
              gnsstk::CivilTime(2020,4,12,0,0,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.count(sat1));
   TUASSERTE(gnsstk::NavSatelliteIDSet::size_type, 1, satset.size());
      // test with a time span that is in the middle of the data time
      // span, but without any matches
   TUCATCH(satset = uut.getIndexSet(
              gnsstk::CivilTime(2020,4,11,23,57,50,gnsstk::TimeSystem::GPS),
              gnsstk::CivilTime(2020,4,11,23,58,10,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, satset.empty());
   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
isPresentTest()
{
   TUDEF("NavDataFactoryWithStore", "isPresent");
   TestClass uut;
   TUCATCH(fillFactory(testFramework, uut));
   gnsstk::NavSatelliteID sat1, sat2, sat3,
      sat4(gnsstk::SatID(23,gnsstk::SatelliteSystem::GPS));
   fillSat(sat1, 23, 32);
   fillSat(sat2, 7, 7);
   fillSat(sat3, 11, 11, gnsstk::SatelliteSystem::GPS,
           gnsstk::CarrierBand::L1, gnsstk::TrackingCode::Y);
   gnsstk::NavMessageID nmid1e(sat1, gnsstk::NavMessageType::Ephemeris),
      nmid1a(sat1, gnsstk::NavMessageType::Almanac),
      nmid2e(sat2, gnsstk::NavMessageType::Ephemeris),
      nmid2a(sat2, gnsstk::NavMessageType::Almanac),
      nmid3e(sat3, gnsstk::NavMessageType::Ephemeris),
      nmid3a(sat3, gnsstk::NavMessageType::Almanac),
      nmid4e(sat4, gnsstk::NavMessageType::Ephemeris);
      // test with time span before any data
   TUASSERTE(bool, false, uut.isPresent(
                nmid1e,
                gnsstk::CivilTime(2020,4,11,23,56,0,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,11,23,57,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, false, uut.isPresent(
                nmid2e,
                gnsstk::CivilTime(2020,4,11,23,56,0,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,11,23,57,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, false, uut.isPresent(
                nmid3e,
                gnsstk::CivilTime(2020,4,11,23,56,0,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,11,23,57,0,gnsstk::TimeSystem::GPS)));
      // test with time span after all data
   TUASSERTE(bool, true, uut.isPresent(
                nmid1e,
                gnsstk::CivilTime(2020,4,12,1,0,0,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,12,2,0,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, uut.isPresent(
                nmid2e,
                gnsstk::CivilTime(2020,4,12,1,0,0,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,12,2,0,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, uut.isPresent(
                nmid3e,
                gnsstk::CivilTime(2020,4,12,0,0,0,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,12,1,0,0,gnsstk::TimeSystem::GPS)));
      // test with a time span that will get all satellites even
      // though it's only partial coverage.
   // uut.dump(std::cout, gnsstk::DumpDetail::Brief);
   TUASSERTE(bool, true, uut.isPresent(
                nmid1e,
                gnsstk::CivilTime(2020,4,12,0,56,0,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,12,0,58,19,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, uut.isPresent(
                nmid2e,
                gnsstk::CivilTime(2020,4,12,0,56,0,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,12,0,58,19,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, uut.isPresent(
                nmid3e,
                gnsstk::CivilTime(2020,4,11,23,56,0,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,11,23,58,19,gnsstk::TimeSystem::GPS)));
      // test with a time span that only contains one satellite, but
      // prior valid ephemerides exist for the other two.
   TUASSERTE(bool, true, uut.isPresent(
                nmid1e,
                gnsstk::CivilTime(2020,4,12,0,59,0,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,12,1,0,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, uut.isPresent(
                nmid2e,
                gnsstk::CivilTime(2020,4,12,0,59,0,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,12,1,0,0,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, uut.isPresent(
                nmid3e,
                gnsstk::CivilTime(2020,4,11,23,59,0,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,12,0,0,0,gnsstk::TimeSystem::GPS)));
      // test with a time span that is in the middle of the data time
      // span, but without any data contained within (which just
      // matches the prior available ephemerides)
   TUASSERTE(bool, true, uut.isPresent(
                nmid1e,
                gnsstk::CivilTime(2020,4,12,0,57,50,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,12,0,58,10,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, uut.isPresent(
                nmid2e,
                gnsstk::CivilTime(2020,4,12,0,57,50,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,12,0,58,10,gnsstk::TimeSystem::GPS)));
   TUASSERTE(bool, true, uut.isPresent(
                nmid3e,
                gnsstk::CivilTime(2020,4,11,23,57,50,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,11,23,58,10,gnsstk::TimeSystem::GPS)));
      // test with wildcards
   TUASSERTE(bool, true, uut.isPresent(
                nmid4e,
                gnsstk::CivilTime(2020,4,11,23,56,0,gnsstk::TimeSystem::GPS),
                gnsstk::CivilTime(2020,4,11,23,58,0,gnsstk::TimeSystem::GPS)));
   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
countTest()
{
   TUDEF("NavDataFactoryWithStore", "count");
   TestClass uut;
   TUCATCH(fillFactory(testFramework, uut));
   size_t totalCount = uut.size();
   gnsstk::NavMessageID key1(
      gnsstk::NavSatelliteID(gnsstk::SatID(gnsstk::SatelliteSystem::Unknown),
                             gnsstk::SatID(gnsstk::SatelliteSystem::Unknown),
                             gnsstk::ObsID(gnsstk::ObservationType::Any,
                                           gnsstk::CarrierBand::Any,
                                           gnsstk::TrackingCode::Any,
                                           gnsstk::XmitAnt::Any),
                             gnsstk::NavID(gnsstk::NavType::Any)),
      gnsstk::NavMessageType::Unknown);
   key1.sat.makeWild();
   key1.xmitSat.makeWild();
      // key1, being a complete wildcard, should yield the same
      // results as size()
   TUASSERTE(size_t, totalCount, uut.count(key1));

   gnsstk::NavMessageID key2(
      gnsstk::NavSatelliteID(gnsstk::SatID(gnsstk::SatelliteSystem::GPS),
                             gnsstk::SatID(gnsstk::SatelliteSystem::GPS),
                             gnsstk::ObsID(gnsstk::ObservationType::Any,
                                           gnsstk::CarrierBand::Any,
                                           gnsstk::TrackingCode::Any,
                                           gnsstk::XmitAnt::Any),
                             gnsstk::NavID(gnsstk::NavType::Any)),
      gnsstk::NavMessageType::Unknown);
      // key2, is mostly a wildcard, but does specify the satellite
      // system as GPS, and should yield the same results as size()
   TUASSERTE(size_t, totalCount, uut.count(key2));

   gnsstk::NavMessageID key3(
      gnsstk::NavSatelliteID(gnsstk::SatID(gnsstk::SatelliteSystem::Galileo),
                             gnsstk::SatID(gnsstk::SatelliteSystem::Galileo),
                             gnsstk::ObsID(gnsstk::ObservationType::Any,
                                           gnsstk::CarrierBand::Any,
                                           gnsstk::TrackingCode::Any,
                                           gnsstk::XmitAnt::Any),
                             gnsstk::NavID(gnsstk::NavType::Any)),
      gnsstk::NavMessageType::Unknown);
      // we have nothing in the store for Galileo.
   TUASSERTE(size_t, 0, uut.count(key3));

      // count GPS L1
   gnsstk::NavMessageID key4(
      gnsstk::NavSatelliteID(gnsstk::SatID(gnsstk::SatelliteSystem::GPS),
                             gnsstk::SatID(gnsstk::SatelliteSystem::GPS),
                             gnsstk::ObsID(gnsstk::ObservationType::Any,
                                           gnsstk::CarrierBand::L1,
                                           gnsstk::TrackingCode::Any,
                                           gnsstk::XmitAnt::Any),
                             gnsstk::NavID(gnsstk::NavType::Any)),
      gnsstk::NavMessageType::Unknown);
   TUASSERTE(size_t, totalCount, uut.count(key4));

      // count GPS L2
   gnsstk::NavMessageID key5(
      gnsstk::NavSatelliteID(gnsstk::SatID(gnsstk::SatelliteSystem::GPS),
                             gnsstk::SatID(gnsstk::SatelliteSystem::GPS),
                             gnsstk::ObsID(gnsstk::ObservationType::Any,
                                           gnsstk::CarrierBand::L2,
                                           gnsstk::TrackingCode::Any,
                                           gnsstk::XmitAnt::Any),
                             gnsstk::NavID(gnsstk::NavType::Any)),
      gnsstk::NavMessageType::Unknown);
   TUASSERTE(size_t, 0, uut.count(key5));

      // count using SatelliteSystem only
   TUASSERTE(size_t, totalCount, uut.count(gnsstk::SatelliteSystem::GPS));
   TUASSERTE(size_t, 0, uut.count(gnsstk::SatelliteSystem::Galileo));
      // count using subject satellite
   TUASSERTE(size_t, 4,
             uut.count(gnsstk::SatID(23,gnsstk::SatelliteSystem::GPS)));
   TUASSERTE(size_t, 0,
             uut.count(gnsstk::SatID(32,gnsstk::SatelliteSystem::GPS)));
      // count using message type
   TUASSERTE(size_t, totalCount, uut.count(gnsstk::NavMessageType::Ephemeris));
   TUASSERTE(size_t, 0, uut.count(gnsstk::NavMessageType::Almanac));

   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
getFirstLastTimeTest()
{
   TUDEF("NavDataFactoryWithStore", "getFirstTime");
   TestClass uut;
   TUCATCH(fillFactory(testFramework, uut));
   gnsstk::SatID sat23(23,gnsstk::SatelliteSystem::GPS),
      sat7(7,gnsstk::SatelliteSystem::GPS),
      sat11(11,gnsstk::SatelliteSystem::GPS),
      sat12(12,gnsstk::SatelliteSystem::GPS),
      sat23R(23,gnsstk::SatelliteSystem::Glonass);
   TUASSERTE(gnsstk::CommonTime, ct-3600, uut.getFirstTime(sat23));
   TUASSERTE(gnsstk::CommonTime, ct-3510, uut.getLastTime(sat23));
   TUASSERTE(gnsstk::CommonTime, ct-3600, uut.getFirstTime(sat7));
   TUASSERTE(gnsstk::CommonTime, ct-3570, uut.getLastTime(sat7));
   TUASSERTE(gnsstk::CommonTime, ct-3600, uut.getFirstTime(sat11));
   TUASSERTE(gnsstk::CommonTime, ct-3570, uut.getLastTime(sat11));
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::END_OF_TIME,
             uut.getFirstTime(sat12));
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::BEGINNING_OF_TIME,
             uut.getLastTime(sat12));
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::END_OF_TIME,
             uut.getFirstTime(sat23R));
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::BEGINNING_OF_TIME,
             uut.getLastTime(sat23R));
   TURETURN();
}


int main()
{
   NavDataFactoryWithStore_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.addNavDataTest();
   errorTotal += testClass.addNavDataTimeTest();
   errorTotal += testClass.editTest();
   errorTotal += testClass.clearTest();
   errorTotal += testClass.findTest();
   errorTotal += testClass.find2Test();
   errorTotal += testClass.findNearestTest();
   errorTotal += testClass.findXmitHealthTest();
   errorTotal += testClass.getOffsetTest();
   errorTotal += testClass.getOffset2Test();
   errorTotal += testClass.getAvailableSatsTest();
   errorTotal += testClass.getIndexSetTest();
   errorTotal += testClass.isPresentTest();
   errorTotal += testClass.countTest();
   errorTotal += testClass.getFirstLastTimeTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
