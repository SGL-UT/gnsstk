#include "NavDataFactoryWithStore.hpp"
#include "GPSWeekSecond.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavAlm.hpp"
#include "GPSLNavHealth.hpp"
#include "GPSLNavTimeOffset.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
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

   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

   /** Implement a test class to expose protected members rather than
    * using friends. */
class TestClass : public gpstk::NavDataFactoryWithStore
{
public:
      /// Grant access to protected data.
   gpstk::NavMessageMap& getData()
   { return data; }
   gpstk::NavNearMessageMap& getNearestData()
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
};


class NavDataFactoryWithStore_T
{
public:
      /// Initialize time objects used for stamping fake nav data.
   NavDataFactoryWithStore_T();

   unsigned addNavDataTest();
   unsigned findTest();
   unsigned find2Test();
   unsigned findNearestTest();
      /// Test find with various xmitHealth settings.
   unsigned findXmitHealthTest();
   unsigned getOffsetTest();
   unsigned getOffset2Test();
   unsigned editTest();
   unsigned clearTest();

      /// Fill fact with test data
   void fillFactory(gpstk::TestUtil& testFramework, TestClass& fact);
      /// Fill fact with test data for findXmitHealthTest()
   void fillFactoryXmitHealth(gpstk::TestUtil& testFramework, TestClass& fact);
      /// Add a single NavData with the given parameters
   void addData(gpstk::TestUtil& testFramework, TestClass& fact,
                const gpstk::CommonTime& ct, unsigned long sat,
                unsigned long xmitSat,
                gpstk::SatelliteSystem sys = gpstk::SatelliteSystem::GPS,
                gpstk::CarrierBand car = gpstk::CarrierBand::L1,
                gpstk::TrackingCode code = gpstk::TrackingCode::CA,
                gpstk::NavType nav = gpstk::NavType::GPSLNAV,
                gpstk::SVHealth hea = gpstk::SVHealth::Healthy,
                gpstk::NavMessageType nmt = gpstk::NavMessageType::Ephemeris);
   void fillSignal(gpstk::NavSignalID& signal,
                   gpstk::SatelliteSystem sys = gpstk::SatelliteSystem::GPS,
                   gpstk::CarrierBand car = gpstk::CarrierBand::L1,
                   gpstk::TrackingCode code = gpstk::TrackingCode::CA,
                   gpstk::NavType nav = gpstk::NavType::GPSLNAV);
   void fillSat(gpstk::NavSatelliteID& satellite,
                unsigned long sat,
                unsigned long xmitSat,
                gpstk::SatelliteSystem sys = gpstk::SatelliteSystem::GPS,
                gpstk::CarrierBand car = gpstk::CarrierBand::L1,
                gpstk::TrackingCode code = gpstk::TrackingCode::CA,
                gpstk::NavType nav = gpstk::NavType::GPSLNAV);
      /// Check to make sure there are no empty maps in fact.
   void checkForEmpty(gpstk::TestUtil& testFramework, TestClass& fact);

   gpstk::GPSWeekSecond gws, gws5;
   gpstk::CommonTime ct, ct5;
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
   gpstk::SatID subjID(23,gpstk::SatelliteSystem::GPS);
   gpstk::SatID xmitID(32,gpstk::SatelliteSystem::GPS);
   gpstk::NavDataPtr navOut = std::make_shared<gpstk::GPSLNavEph>();
   navOut->timeStamp = ct;
   navOut->signal.messageType = gpstk::NavMessageType::Ephemeris;
   navOut->signal.sat = subjID;
   navOut->signal.xmitSat = xmitID;
   navOut->signal.system = gpstk::SatelliteSystem::GPS;
   navOut->signal.carrier = gpstk::CarrierBand::L1;
   navOut->signal.code = gpstk::TrackingCode::CA;
   navOut->signal.nav = gpstk::NavType::GPSLNAV;

   TUASSERT(fact.addNavData(navOut));
   TUASSERTE(size_t, 1, fact.size());

   gpstk::NavMessageMap &nmm(fact.getData());
   for (auto& nmmi : nmm)
   {
      for (auto& nsatmi : nmmi.second)
      {
         TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
                   nsatmi.first.system);
         TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1,
                   nsatmi.first.carrier);
         TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::CA,
                   nsatmi.first.code);
         TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, nsatmi.first.nav);
         TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
                   nsatmi.first.system);
         TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1,
                   nsatmi.first.carrier);
         TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::CA,
                   nsatmi.first.code);
         TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV,
                   nsatmi.first.nav);
         TUASSERTE(gpstk::SatID, subjID, nsatmi.first.sat);
         TUASSERTE(gpstk::SatID, xmitID, nsatmi.first.xmitSat);
         for (auto& nmi : nsatmi.second)
         {
            TUASSERTE(gpstk::CommonTime, ct+18, nmi.first);
         }
      }
   }

   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
findTest()
{
   TUDEF("NavDataFactoryWithStore", "find");

   TestClass fact1;
   gpstk::NavMessageID nmid1a;
   gpstk::NavDataPtr result;
   TUCATCH(fillFactory(testFramework, fact1));
   TUCATCH(fillSat(nmid1a, 23, 32));
   nmid1a.messageType = gpstk::NavMessageType::Ephemeris;
      // check basic time stamps that should work
   TUASSERT(fact1.find(nmid1a, ct+35, result, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::Any,
                       gpstk::NavSearchOrder::User));
   TUASSERT(fact1.find(nmid1a, ct+30, result, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::Any,
                       gpstk::NavSearchOrder::User));
   TUASSERT(fact1.find(nmid1a, ct+60, result, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::Any,
                       gpstk::NavSearchOrder::User));
      // check time stamps that don't work
   TUASSERT(!fact1.find(nmid1a, ct, result, gpstk::SVHealth::Any,
                        gpstk::NavValidityType::Any,
                        gpstk::NavSearchOrder::User));
      // time edge cases.
   TUASSERT(!fact1.find(nmid1a, ct+17, result, gpstk::SVHealth::Any,
                        gpstk::NavValidityType::Any,
                        gpstk::NavSearchOrder::User));
   TUASSERT(fact1.find(nmid1a, ct+18, result, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::Any,
                       gpstk::NavSearchOrder::User));
      // test validity flags
   TUASSERT(fact1.find(nmid1a, ct+30, result, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::ValidOnly,
                       gpstk::NavSearchOrder::User));
   TUASSERT(!fact1.find(nmid1a, ct+30, result, gpstk::SVHealth::Any,
                        gpstk::NavValidityType::InvalidOnly,
                        gpstk::NavSearchOrder::User));

   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
find2Test()
{
   TUDEF("NavDataFactoryWithStore", "find");
   TestClass uut;
   using SS = gpstk::SatelliteSystem;
   using CB = gpstk::CarrierBand;
   using TC = gpstk::TrackingCode;
   using NT = gpstk::NavType;
   using SH = gpstk::SVHealth;
   using MT = gpstk::NavMessageType;
   using VT = gpstk::NavValidityType;
   using SO = gpstk::NavSearchOrder;
   gpstk::CommonTime refsf1ct = gpstk::GPSWeekSecond(2101, 0);
      // between copies 2 and 3, or i==57
   gpstk::CommonTime unhealthyStart = gpstk::GPSWeekSecond(2101, 1710);
   gpstk::CommonTime refpg2ct = gpstk::GPSWeekSecond(2101, 54);
   gpstk::CommonTime findTime1 = gpstk::GPSWeekSecond(2101, 3838);
   gpstk::NavMessageID findNMID1a(
      gpstk::NavSatelliteID(2, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Almanac);
   gpstk::NavMessageID findNMID2a(
      gpstk::NavSatelliteID(2, SS::GPS, CB::Any, TC::Any, NT::Any),
      MT::Almanac);
   gpstk::NavMessageID findNMID3a(
      gpstk::NavSatelliteID(2, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Almanac);
   gpstk::NavMessageID findNMID4a(
      gpstk::NavSatelliteID(2, 1, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Almanac);
   gpstk::NavMessageID findNMID5a(
      gpstk::NavSatelliteID(2, 3, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Almanac);
   gpstk::NavMessageID findNMID1h(
      gpstk::NavSatelliteID(1, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Health);
   gpstk::NavMessageID findNMID1L1CAe(
      gpstk::NavSatelliteID(1, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Ephemeris);
   gpstk::NavMessageID findNMID1L2Ye(
      gpstk::NavSatelliteID(1, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Ephemeris);
   gpstk::NavMessageID findNMID3L1CAe(
      gpstk::NavSatelliteID(3, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Ephemeris);
   gpstk::NavMessageID findNMID3e(
      gpstk::NavSatelliteID(3, SS::GPS, CB::Any, TC::Any, NT::Any),
      MT::Ephemeris);
   gpstk::NavMessageID findNMID3L2Ye(
      gpstk::NavSatelliteID(3, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
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
   gpstk::NavDataPtr result;
   gpstk::GPSLNavHealth *hea;
      // check find almanac with no restrictions on health or validity
   TUASSERTE(bool, true,
             uut.find(findNMID1a,findTime1,result,SH::Any,VT::Any,SO::User));
   gpstk::CommonTime expTime1(gpstk::GPSWeekSecond(2101, 3804));
   TUASSERTE(gpstk::CommonTime, expTime1, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.carrier);
   TUASSERTE(TC, TC::CA, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // look for health data before and after the state transition
   TUASSERTE(bool, true,
             uut.find(findNMID1h,gpstk::GPSWeekSecond(2101,1710),result,
                      SH::Any,VT::Any,SO::User));
   result->dump(std::cerr, gpstk::NavData::Detail::Full);
   if (result)
   {
      hea = dynamic_cast<gpstk::GPSLNavHealth*>(result.get());
      TUASSERTE(unsigned, 0, hea->svHealth);
   }
   TUASSERTE(bool, true,
             uut.find(findNMID1h,gpstk::GPSWeekSecond(2101,1716),result,
                      SH::Any, VT::Any,SO::User));
   result->dump(std::cerr, gpstk::NavData::Detail::Full);
   if (result)
   {
      hea = dynamic_cast<gpstk::GPSLNavHealth*>(result.get());
      TUASSERTE(unsigned, 1, hea->svHealth);
   }
      // Look for healthy almanac data after the transition to
      // unhealthy.  This should result in ignoring the almanac
      // transmitted by PRN 1, since PRN 1 is unhealthy at this time,
      // and returning the almanac transmitted by PRN 3 instead.
   TUASSERTE(bool, true,
             uut.find(findNMID1a,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gpstk::CommonTime, expTime1, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.carrier);
   TUASSERTE(TC, TC::CA, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for healthy almanac data after the transition to
      // unhealthy with wildcards in the signal (band, code, nav).
      // Should yield the same results as above because of ordering.
   TUASSERTE(bool, true,
             uut.find(findNMID2a,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gpstk::CommonTime, expTime1, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.carrier);
   TUASSERTE(TC, TC::CA, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for almanac data after the transition to unhealthy where
      // we don't care about the health but we do specifically want
      // L2-Y, so we expect xmit PRN = 1, L2/Y/LNav
   TUASSERTE(bool, true,
             uut.find(findNMID3a,findTime1,result,SH::Any,VT::Any,
                      SO::User));
   TUASSERTE(gpstk::CommonTime, expTime1, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.carrier);
   TUASSERTE(TC, TC::Y, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for almanac data after the transition to unhealthy where
      // we do want healthy transmit satellites and do specifically
      // want L2-Y, so we expect xmit PRN = 3, L2/Y/LNav
   TUASSERTE(bool, true,
             uut.find(findNMID3a,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gpstk::CommonTime, expTime1, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.carrier);
   TUASSERTE(TC, TC::Y, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for almanac data after the transition to unhealthy where
      // we want the most recent almanac transmitted by PRN 1 when it
      // was healthy, so we expect xmit PRN = 1, L2/Y/LNav at time 2*750+54
   gpstk::CommonTime expTime2(gpstk::GPSWeekSecond(2101, 1554));
   TUASSERTE(bool, true,
             uut.find(findNMID4a,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gpstk::CommonTime, expTime2, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.carrier);
   TUASSERTE(TC, TC::Y, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for almanac data after the transition to unhealthy where
      // we want the most recent almanac transmitted by PRN 1 when it
      // was UNhealthy, so we expect xmit PRN = 1, L2/Y/LNav at time 5*750+54
   TUASSERTE(bool, true,
             uut.find(findNMID4a,findTime1,result,SH::Unhealthy,VT::Any,
                      SO::User));
   TUASSERTE(gpstk::CommonTime, expTime1, result->timeStamp);
   TUASSERTE(MT, MT::Almanac, result->signal.messageType);
   TUASSERTE(int, 2, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.carrier);
   TUASSERTE(TC, TC::Y, result->signal.code);
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
   gpstk::CommonTime expTime1Eph(gpstk::GPSWeekSecond(2101, 3810));
   TUASSERTE(gpstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.carrier);
   TUASSERTE(TC, TC::CA, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 L1 CA (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID3L1CAe,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gpstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 3, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.carrier);
   TUASSERTE(TC, TC::CA, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 any any (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID3e,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gpstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 3, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.carrier);
   TUASSERTE(TC, TC::CA, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime1,result,SH::Any,VT::Any,
                      SO::User));
   TUASSERTE(gpstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.carrier);
   TUASSERTE(TC, TC::Y, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 L2 Y (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID3L2Ye,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   TUASSERTE(gpstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 3, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.carrier);
   TUASSERTE(TC, TC::Y, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime1,result,SH::Healthy,VT::Any,
                      SO::User));
   gpstk::CommonTime expTime2Eph(gpstk::GPSWeekSecond(2101, 1680));
   TUASSERTE(gpstk::CommonTime, expTime2Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.carrier);
   TUASSERTE(TC, TC::Y, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y (Unhealthy)
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime1,result,SH::Unhealthy,VT::Any,
                      SO::User));
   TUASSERTE(gpstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.carrier);
   TUASSERTE(TC, TC::Y, result->signal.code);
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
   using SS = gpstk::SatelliteSystem;
   using CB = gpstk::CarrierBand;
   using TC = gpstk::TrackingCode;
   using NT = gpstk::NavType;
   using SH = gpstk::SVHealth;
   using MT = gpstk::NavMessageType;
   using VT = gpstk::NavValidityType;
   using SO = gpstk::NavSearchOrder;
   gpstk::CommonTime refsf1ct = gpstk::GPSWeekSecond(2101, 0);
      // between copies 2 and 3, or i==57
   gpstk::CommonTime unhealthyStart = gpstk::GPSWeekSecond(2101, 1710);
   gpstk::CommonTime refpg2ct = gpstk::GPSWeekSecond(2101, 54);
   gpstk::CommonTime findTime1 = gpstk::GPSWeekSecond(2101, 3838);
   gpstk::CommonTime findTime2 = gpstk::GPSWeekSecond(2101, 3000);
   gpstk::NavMessageID findNMID1a(
      gpstk::NavSatelliteID(2, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Almanac);
   gpstk::NavMessageID findNMID2a(
      gpstk::NavSatelliteID(2, SS::GPS, CB::Any, TC::Any, NT::Any),
      MT::Almanac);
   gpstk::NavMessageID findNMID3a(
      gpstk::NavSatelliteID(2, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Almanac);
   gpstk::NavMessageID findNMID4a(
      gpstk::NavSatelliteID(2, 1, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Almanac);
   gpstk::NavMessageID findNMID5a(
      gpstk::NavSatelliteID(2, 3, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Almanac);
   gpstk::NavMessageID findNMID1h(
      gpstk::NavSatelliteID(1, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Health);
   gpstk::NavMessageID findNMID1L1CAe(
      gpstk::NavSatelliteID(1, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Ephemeris);
   gpstk::NavMessageID findNMID1L2Ye(
      gpstk::NavSatelliteID(1, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
      MT::Ephemeris);
   gpstk::NavMessageID findNMID3L1CAe(
      gpstk::NavSatelliteID(3, SS::GPS, CB::L1, TC::CA, NT::GPSLNAV),
      MT::Ephemeris);
   gpstk::NavMessageID findNMID3e(
      gpstk::NavSatelliteID(3, SS::GPS, CB::Any, TC::Any, NT::Any),
      MT::Ephemeris);
   gpstk::NavMessageID findNMID3L2Ye(
      gpstk::NavSatelliteID(3, SS::GPS, CB::L2, TC::Y, NT::GPSLNAV),
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
   gpstk::NavDataPtr result;
   gpstk::GPSLNavHealth *hea;
      /// @todo add tests for findNearest using Almanac data and maybe Health.
      // Look for ephemeris data from PRN 1 L1 CA
   TUASSERTE(bool, true,
             uut.find(findNMID1L1CAe,findTime1,result,SH::Any,VT::Any,
                      SO::Nearest));
   gpstk::CommonTime expTime1Eph(gpstk::GPSWeekSecond(2101, 7200));
   TUASSERTE(gpstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.carrier);
   TUASSERTE(TC, TC::CA, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 L1 CA (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID3L1CAe,findTime1,result,SH::Healthy,VT::Any,
                      SO::Nearest));
   TUASSERTE(gpstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 3, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.carrier);
   TUASSERTE(TC, TC::CA, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 any any (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID3e,findTime1,result,SH::Healthy,VT::Any,
                      SO::Nearest));
   TUASSERTE(gpstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 3, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L1, result->signal.carrier);
   TUASSERTE(TC, TC::CA, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime1,result,SH::Any,VT::Any,
                      SO::Nearest));
   TUASSERTE(gpstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.carrier);
   TUASSERTE(TC, TC::Y, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 3 L2 Y (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID3L2Ye,findTime1,result,SH::Healthy,VT::Any,
                      SO::Nearest));
   TUASSERTE(gpstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 3, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 3, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.carrier);
   TUASSERTE(TC, TC::Y, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y (Healthy)
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime1,result,SH::Healthy,VT::Any,
                      SO::Nearest));
   TUASSERTE(gpstk::CommonTime, expTime1Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.carrier);
   TUASSERTE(TC, TC::Y, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y (Healthy) at an earlier time
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime2,result,SH::Healthy,VT::Any,
                      SO::Nearest));
   gpstk::CommonTime expTime2Eph(gpstk::GPSWeekSecond(2101, 0));
   TUASSERTE(gpstk::CommonTime, expTime2Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.carrier);
   TUASSERTE(TC, TC::Y, result->signal.code);
   TUASSERTE(NT, NT::GPSLNAV, result->signal.nav);
      // Look for ephemeris data from PRN 1 L2 Y (Unhealthy)
   TUASSERTE(bool, true,
             uut.find(findNMID1L2Ye,findTime1,result,SH::Unhealthy,VT::Any,
                      SO::Nearest));
      // this is the first of the unhealthy data
   gpstk::CommonTime expTime3Eph(gpstk::GPSWeekSecond(2101, 1710));
   TUASSERTE(gpstk::CommonTime, expTime3Eph, result->timeStamp);
   TUASSERTE(MT, MT::Ephemeris, result->signal.messageType);
   TUASSERTE(int, 1, result->signal.sat.id);
   TUASSERTE(bool, false, result->signal.sat.wildId);
   TUASSERTE(int64_t, 0, result->signal.sat.extra);
   TUASSERTE(int64_t, -1, result->signal.sat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.sat.system);
   TUASSERTE(bool, false, result->signal.sat.wildSys);
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   TUASSERTE(bool, false, result->signal.xmitSat.wildId);
   TUASSERTE(int64_t, 0, result->signal.xmitSat.extra);
   TUASSERTE(int64_t, -1, result->signal.xmitSat.extraMask);
   TUASSERTE(SS, SS::GPS, result->signal.xmitSat.system);
   TUASSERTE(bool, false, result->signal.xmitSat.wildSys);
   TUASSERTE(SS, SS::GPS, result->signal.system);
   TUASSERTE(CB, CB::L2, result->signal.carrier);
   TUASSERTE(TC, TC::Y, result->signal.code);
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
   gpstk::NavDataPtr navOut = std::make_shared<gpstk::GPSLNavTimeOffset>();
   navOut->timeStamp = ct;
   navOut->signal.messageType = gpstk::NavMessageType::TimeOffset;
   gpstk::GPSLNavTimeOffset *toptr = dynamic_cast<gpstk::GPSLNavTimeOffset*>(
      navOut.get());
   fillSat(navOut->signal, 23, 23);
   toptr->deltatLS = 23; // set a simple, easy to verify value.
   TUASSERT(fact1.addNavData(navOut));
   gpstk::TimeOffsetData *top;
   gpstk::NavDataPtr result;
   double offset;
   TUASSERT(fact1.getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC,
                            ct+35, result, gpstk::SVHealth::Any,
                            gpstk::NavValidityType::Any));
   top = dynamic_cast<gpstk::TimeOffsetData*>(result.get());
   TUASSERT(top->getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC,
                           ct+35, offset));
   TUASSERTFE(23.0, offset);
      // reverse the conversion and expect negative.
   TUASSERT(fact1.getOffset(gpstk::TimeSystem::UTC, gpstk::TimeSystem::GPS,
                            ct+35, result, gpstk::SVHealth::Any,
                            gpstk::NavValidityType::Any));
   top = dynamic_cast<gpstk::TimeOffsetData*>(result.get());
   TUASSERT(top->getOffset(gpstk::TimeSystem::UTC, gpstk::TimeSystem::GPS,
                           ct+35, offset));
   TUASSERTFE(-23.0, offset);
      // expect this to not work
   TUASSERT(!fact1.getOffset(gpstk::TimeSystem::UTC, gpstk::TimeSystem::BDT,
                             ct+35, result, gpstk::SVHealth::Any,
                             gpstk::NavValidityType::Any));
   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
getOffset2Test()
{
   TUDEF("NavDataFactoryWithStore", "getOffset");
   TestClass uut;
   using SS = gpstk::SatelliteSystem;
   using CB = gpstk::CarrierBand;
   using TC = gpstk::TrackingCode;
   using NT = gpstk::NavType;
   using SH = gpstk::SVHealth;
   using MT = gpstk::NavMessageType;
   using VT = gpstk::NavValidityType;
   using SO = gpstk::NavSearchOrder;
   gpstk::CommonTime refsf1ct = gpstk::GPSWeekSecond(2101, 0);
      // between copies 2 and 3, or i==57
   gpstk::CommonTime unhealthyStart = gpstk::GPSWeekSecond(2101, 1710);
   gpstk::CommonTime refpg56ct = gpstk::GPSWeekSecond(2101, 528);
   gpstk::CommonTime findTime1 = gpstk::GPSWeekSecond(2101, 3838);
   gpstk::CommonTime findTime2 = gpstk::GPSWeekSecond(2101, 0);
   gpstk::CommonTime findTime3 = gpstk::GPSWeekSecond(2101, 7300);
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
   gpstk::TimeOffsetData *top;
   gpstk::NavDataPtr result;
   double offset;
      // check getOffset with no restrictions on health or validity
   TUASSERTE(bool, true,
             uut.getOffset(gpstk::TimeSystem::GPS,gpstk::TimeSystem::UTC,
                           findTime1,result,SH::Any,VT::Any));
   top = dynamic_cast<gpstk::TimeOffsetData*>(result.get());
   TUASSERT(top->getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC,
                           findTime1, offset));
   TUASSERTFE(1, offset); // PRN == offset == 1
      // check getOffset requiring data from a healthy SV
   TUASSERTE(bool, true,
             uut.getOffset(gpstk::TimeSystem::GPS,gpstk::TimeSystem::UTC,
                           findTime1,result,SH::Healthy,VT::Any));
   top = dynamic_cast<gpstk::TimeOffsetData*>(result.get());
   TUASSERT(top->getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC,
                           findTime1, offset));
   TUASSERTFE(3, offset); // PRN == offset == 3
      // check getOffset with time prior to earliest data
   TUASSERTE(bool, false,
             uut.getOffset(gpstk::TimeSystem::GPS,gpstk::TimeSystem::UTC,
                           findTime2,result,SH::Any,VT::Any));
      // check getOffset with time after most recent data
   TUASSERTE(bool, true,
             uut.getOffset(gpstk::TimeSystem::GPS,gpstk::TimeSystem::UTC,
                           findTime3,result,SH::Any,VT::Any));
   top = dynamic_cast<gpstk::TimeOffsetData*>(result.get());
   TUASSERT(top->getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC,
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
   TUCATCH(fillFactory(testFramework, fact1));
      // verify initial conditions
   TUASSERTE(size_t, 8, fact1.size());
   TUASSERTE(size_t, 2, fact1.numSignals());
   TUASSERTE(size_t, 3, fact1.numSatellites());
   TUASSERTE(size_t, 8, fact1.sizeNearest());
   TUASSERTE(size_t, 3, fact1.numSatellitesNearest());
      // remove nothing
   TUCATCH(fact1.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct));
   TUASSERTE(size_t, 8, fact1.size());
   TUASSERTE(size_t, 2, fact1.numSignals());
   TUASSERTE(size_t, 3, fact1.numSatellites());
   TUASSERTE(size_t, 8, fact1.sizeNearest());
   TUASSERTE(size_t, 3, fact1.numSatellitesNearest());
   checkForEmpty(testFramework, fact1);
      // remove messages at ct
   TUCATCH(fact1.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct+30));
   TUASSERTE(size_t, 5, fact1.size());
   TUASSERTE(size_t, 2, fact1.numSignals());
   TUASSERTE(size_t, 3, fact1.numSatellites());
   TUASSERTE(size_t, 5, fact1.sizeNearest());
   TUASSERTE(size_t, 3, fact1.numSatellitesNearest());
   checkForEmpty(testFramework, fact1);
      // remove messages at ct+30
   TUCATCH(fact1.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct+60));
   TUASSERTE(size_t, 2, fact1.size());
   TUASSERTE(size_t, 1, fact1.numSignals());
   TUASSERTE(size_t, 1, fact1.numSatellites());
   TUASSERTE(size_t, 2, fact1.sizeNearest());
   TUASSERTE(size_t, 1, fact1.numSatellitesNearest());
   checkForEmpty(testFramework, fact1);
      // remove messages at ct+60
   TUCATCH(fact1.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct+90));
   TUASSERTE(size_t, 1, fact1.size());
   TUASSERTE(size_t, 1, fact1.numSignals());
   TUASSERTE(size_t, 1, fact1.numSatellites());
   TUASSERTE(size_t, 1, fact1.sizeNearest());
   TUASSERTE(size_t, 1, fact1.numSatellitesNearest());
   checkForEmpty(testFramework, fact1);
      // remove messages at ct+90
   TUCATCH(fact1.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct+120));
   TUASSERTE(size_t, 0, fact1.size());
   TUASSERTE(size_t, 0, fact1.numSignals());
   TUASSERTE(size_t, 0, fact1.numSatellites());
   TUASSERTE(size_t, 0, fact1.sizeNearest());
   TUASSERTE(size_t, 0, fact1.numSatellitesNearest());
   checkForEmpty(testFramework, fact1);

      // Now check satellite editing
   TestClass fact2;
   gpstk::NavSatelliteID satID2a, satID2b, satID2c, satID2d, satID2e;
   TUCATCH(fillFactory(testFramework, fact2));
   TUCATCH(fillSat(satID2a, 7, 7));
   TUCATCH(fillSat(satID2b, 99, 99));
   TUCATCH(fillSat(satID2c, 7, 7, gpstk::SatelliteSystem::GPS,
                   gpstk::CarrierBand::L5));
   TUCATCH(fillSat(satID2d, 11, 11, gpstk::SatelliteSystem::GPS,
                   gpstk::CarrierBand::L1, gpstk::TrackingCode::Y));
   TUCATCH(fillSat(satID2e, 23, 32));
      // verify initial conditions
   TUASSERTE(size_t, 8, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 3, fact2.numSatellites());
   TUASSERTE(size_t, 8, fact2.sizeNearest());
   TUASSERTE(size_t, 3, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove nothing
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct, satID2a));
   TUASSERTE(size_t, 8, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 3, fact2.numSatellites());
   TUASSERTE(size_t, 8, fact2.sizeNearest());
   TUASSERTE(size_t, 3, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove nothing, this time because the satellite ID isn't present
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, satID2b));
   TUASSERTE(size_t, 8, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 3, fact2.numSatellites());
   TUASSERTE(size_t, 8, fact2.sizeNearest());
   TUASSERTE(size_t, 3, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove nothing, this time because the signal isn't present
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, satID2c));
   TUASSERTE(size_t, 8, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 3, fact2.numSatellites());
   TUASSERTE(size_t, 8, fact2.sizeNearest());
   TUASSERTE(size_t, 3, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove all sat 7 data
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, satID2a));
   TUASSERTE(size_t, 6, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 2, fact2.numSatellites());
   TUASSERTE(size_t, 6, fact2.sizeNearest());
   TUASSERTE(size_t, 2, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove all sat 11 data
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, satID2d));
   TUASSERTE(size_t, 4, fact2.size());
   TUASSERTE(size_t, 1, fact2.numSignals());
   TUASSERTE(size_t, 1, fact2.numSatellites());
   TUASSERTE(size_t, 4, fact2.sizeNearest());
   TUASSERTE(size_t, 1, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove some of sat 23 data
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct+30, satID2e));
   TUASSERTE(size_t, 3, fact2.size());
   TUASSERTE(size_t, 1, fact2.numSignals());
   TUASSERTE(size_t, 1, fact2.numSatellites());
   TUASSERTE(size_t, 3, fact2.sizeNearest());
   TUASSERTE(size_t, 1, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);
      // remove the rest of sat 23 data
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, satID2e));
   TUASSERTE(size_t, 0, fact2.size());
   TUASSERTE(size_t, 0, fact2.numSignals());
   TUASSERTE(size_t, 0, fact2.numSatellites());
   TUASSERTE(size_t, 0, fact2.sizeNearest());
   TUASSERTE(size_t, 0, fact2.numSatellitesNearest());
   checkForEmpty(testFramework, fact2);

      // finally, check editing by signal
   TestClass fact3;
   gpstk::NavSignalID sig3a, sig3b, sig3c;
   TUCATCH(fillFactory(testFramework, fact3));
   TUCATCH(fillSignal(sig3a));
   TUCATCH(fillSignal(sig3b, gpstk::SatelliteSystem::Glonass));
   TUCATCH(fillSignal(sig3c, gpstk::SatelliteSystem::GPS,
                      gpstk::CarrierBand::L1, gpstk::TrackingCode::Y));
      // verify initial conditions
   TUASSERTE(size_t, 8, fact3.size());
   TUASSERTE(size_t, 2, fact3.numSignals());
   TUASSERTE(size_t, 3, fact3.numSatellites());
   TUASSERTE(size_t, 8, fact3.sizeNearest());
   TUASSERTE(size_t, 3, fact3.numSatellitesNearest());
   checkForEmpty(testFramework, fact3);
      // remove nothing
   TUCATCH(fact3.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct, sig3a));
   TUASSERTE(size_t, 8, fact3.size());
   TUASSERTE(size_t, 2, fact3.numSignals());
   TUASSERTE(size_t, 3, fact3.numSatellites());
   TUASSERTE(size_t, 8, fact3.sizeNearest());
   TUASSERTE(size_t, 3, fact3.numSatellitesNearest());
   checkForEmpty(testFramework, fact3);
      // remove nothing, this time because the signal isn't present
   TUCATCH(fact3.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, sig3b));
   TUASSERTE(size_t, 8, fact3.size());
   TUASSERTE(size_t, 2, fact3.numSignals());
   TUASSERTE(size_t, 3, fact3.numSatellites());
   TUASSERTE(size_t, 8, fact3.sizeNearest());
   TUASSERTE(size_t, 3, fact3.numSatellitesNearest());
   checkForEmpty(testFramework, fact3);
      // remove all L1-Y data
   TUCATCH(fact3.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, sig3c));
   TUASSERTE(size_t, 6, fact3.size());
   TUASSERTE(size_t, 1, fact3.numSignals());
   TUASSERTE(size_t, 2, fact3.numSatellites());
   TUASSERTE(size_t, 6, fact3.sizeNearest());
   TUASSERTE(size_t, 2, fact3.numSatellitesNearest());
   checkForEmpty(testFramework, fact3);
      // remove some of L1-CA data
   TUCATCH(fact3.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct+30, sig3a));
   TUASSERTE(size_t, 4, fact3.size());
   TUASSERTE(size_t, 1, fact3.numSignals());
   TUASSERTE(size_t, 2, fact3.numSatellites());
   TUASSERTE(size_t, 4, fact3.sizeNearest());
   TUASSERTE(size_t, 2, fact3.numSatellitesNearest());
   checkForEmpty(testFramework, fact3);
      // remove the rest of L1-CA data
   TUCATCH(fact3.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, sig3a));
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
   gpstk::NavDataPtr navOut = std::make_shared<gpstk::GPSLNavEph>();
   navOut->timeStamp = ct;
   navOut->signal.messageType = gpstk::NavMessageType::Ephemeris;
   navOut->signal.sat = gpstk::SatID(23,gpstk::SatelliteSystem::GPS);
   navOut->signal.xmitSat = gpstk::SatID(32,gpstk::SatelliteSystem::GPS);
   navOut->signal.system = gpstk::SatelliteSystem::GPS;
   navOut->signal.carrier = gpstk::CarrierBand::L1;
   navOut->signal.code = gpstk::TrackingCode::CA;
   navOut->signal.nav = gpstk::NavType::GPSLNAV;

   TUASSERT(fact.addNavData(navOut));
   TUASSERTE(size_t, 1, fact.size());
   TUASSERTE(size_t, 1, fact.sizeNearest());

   fact.clear();
   TUASSERTE(size_t, 0, fact.size());
   TUASSERTE(size_t, 0, fact.sizeNearest());

   TURETURN();
}


void NavDataFactoryWithStore_T ::
fillFactory(gpstk::TestUtil& testFramework, TestClass& fact)
{
   addData(testFramework, fact, ct+0,  23, 32);
   addData(testFramework, fact, ct+0,   7, 7);
   addData(testFramework, fact, ct+0,  11, 11, gpstk::SatelliteSystem::GPS,
           gpstk::CarrierBand::L1, gpstk::TrackingCode::Y);
   addData(testFramework, fact, ct+30, 23, 32);
   addData(testFramework, fact, ct+30,  7, 7);
   addData(testFramework, fact, ct+30, 11, 11, gpstk::SatelliteSystem::GPS,
           gpstk::CarrierBand::L1, gpstk::TrackingCode::Y);
   addData(testFramework, fact, ct+60, 23, 32);
   addData(testFramework, fact, ct+90, 23, 32);
}


void NavDataFactoryWithStore_T ::
addData(gpstk::TestUtil& testFramework, TestClass& fact,
        const gpstk::CommonTime& ct, unsigned long sat,
        unsigned long xmitSat, gpstk::SatelliteSystem sys,
        gpstk::CarrierBand car, gpstk::TrackingCode code, gpstk::NavType nav,
        gpstk::SVHealth hea, gpstk::NavMessageType nmt)
{
   gpstk::NavDataPtr navOut;
   gpstk::GPSWeekSecond toe = ct;
   if (nmt == gpstk::NavMessageType::Ephemeris)
   {
      navOut = std::make_shared<gpstk::GPSLNavEph>();
      dynamic_cast<gpstk::OrbitDataKepler*>(navOut.get())->health = hea;
      toe.sow -= fmod(toe.sow,7200);
      dynamic_cast<gpstk::OrbitDataKepler*>(navOut.get())->Toe = toe;
   }
   else if (nmt == gpstk::NavMessageType::Almanac)
   {
      navOut = std::make_shared<gpstk::GPSLNavAlm>();
      dynamic_cast<gpstk::OrbitDataKepler*>(navOut.get())->health = hea;
      toe.sow = toe.sow - fmod(toe.sow,86400) + (xmitSat == 3 ? 61000 : 61056);
      dynamic_cast<gpstk::OrbitDataKepler*>(navOut.get())->Toe = toe;
   }
   else if (nmt == gpstk::NavMessageType::Health)
   {
      navOut = std::make_shared<gpstk::GPSLNavHealth>();
      gpstk::GPSLNavHealth *hp =
         dynamic_cast<gpstk::GPSLNavHealth*>(navOut.get());
      switch (hea)
      {
         case gpstk::SVHealth::Healthy:
            hp->svHealth = 0;
            break;
         case gpstk::SVHealth::Unhealthy:
            hp->svHealth = 1;
            break;
         default:
            TUFAIL("GPS can't handle health type " +
                   gpstk::StringUtils::asString(hea));
            break;
      }
   }
   else if (nmt == gpstk::NavMessageType::TimeOffset)
   {
      navOut = std::make_shared<gpstk::GPSLNavTimeOffset>();
      gpstk::GPSLNavTimeOffset *to =
         dynamic_cast<gpstk::GPSLNavTimeOffset*>(navOut.get());
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
   }
   navOut->timeStamp = ct;
   navOut->signal.messageType = nmt;
   fillSat(navOut->signal, sat, xmitSat, sys, car, code, nav);
   TUASSERT(fact.addNavData(navOut));
}


void NavDataFactoryWithStore_T ::
fillSignal(gpstk::NavSignalID& signal, gpstk::SatelliteSystem sys,
           gpstk::CarrierBand car, gpstk::TrackingCode code, gpstk::NavType nav)
{
   signal.system = sys;
   signal.carrier = car;
   signal.code = code;
   signal.nav = nav;
}


void NavDataFactoryWithStore_T ::
fillSat(gpstk::NavSatelliteID& satellite,
        unsigned long sat, unsigned long xmitSat,
        gpstk::SatelliteSystem sys, gpstk::CarrierBand car,
        gpstk::TrackingCode code, gpstk::NavType nav)
{
   fillSignal(satellite, sys, car, code, nav);
   satellite.sat = gpstk::SatID(sat,sys);
   satellite.xmitSat = gpstk::SatID(xmitSat,sys);
}


void NavDataFactoryWithStore_T ::
checkForEmpty(gpstk::TestUtil& testFramework, TestClass& fact)
{
   gpstk::NavMessageMap &data(fact.getData());
   for (auto& nmmi : data)
   {
      TUASSERT(!nmmi.second.empty());
      for (auto& sati : nmmi.second)
      {
         TUASSERT(!sati.second.empty());
      }
   }
   gpstk::NavNearMessageMap &nearestData(fact.getNearestData());
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
fillFactoryXmitHealth(gpstk::TestUtil& testFramework, TestClass& fact)
{
      // test almanac for prn 5 (subframe 5 page 5)
      // one copy broadcast by prn 1 which is healthy
      // one copy broadcast by prn 2 which is unhealthy
      // first add PRN 1 ephemeris
   addData(testFramework, fact, ct, 1, 1);
      // then add PRN 2 unhealthy ephemeris
   addData(testFramework, fact, ct, 2, 2, gpstk::SatelliteSystem::GPS,
           gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
           gpstk::NavType::GPSLNAV, gpstk::SVHealth::Unhealthy);
      // add the almanac data... Note the unhealthy here refers to PRN 5
   addData(testFramework, fact, ct5, 5, 2, gpstk::SatelliteSystem::GPS,
           gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
           gpstk::NavType::GPSLNAV, gpstk::SVHealth::Unhealthy,
           gpstk::NavMessageType::Almanac);
   addData(testFramework, fact, ct5, 5, 1, gpstk::SatelliteSystem::GPS,
           gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
           gpstk::NavType::GPSLNAV, gpstk::SVHealth::Unhealthy,
           gpstk::NavMessageType::Almanac);
      // finally, add health data, without which the find method won't
      // give the expected results when searching for specific health status
   addData(testFramework, fact, ct, 1, 1, gpstk::SatelliteSystem::GPS,
           gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
           gpstk::NavType::GPSLNAV, gpstk::SVHealth::Healthy,
           gpstk::NavMessageType::Health);
   addData(testFramework, fact, ct, 2, 2, gpstk::SatelliteSystem::GPS,
           gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
           gpstk::NavType::GPSLNAV, gpstk::SVHealth::Unhealthy,
           gpstk::NavMessageType::Health);
}


unsigned NavDataFactoryWithStore_T ::
findXmitHealthTest()
{
   TUDEF("NavDataFactoryWithStore", "find");
   TestClass fact1;
   gpstk::NavMessageID nmid1a;
   gpstk::NavDataPtr result;
   TUCATCH(fillFactoryXmitHealth(testFramework, fact1));
   TUASSERTE(size_t, 6, fact1.size());
   TUCATCH(fillSat(nmid1a, 5, 0));
   nmid1a.xmitSat.wildId = true;
   nmid1a.messageType = gpstk::NavMessageType::Almanac;
      // make sure we get something with no filters
   TUASSERT(fact1.find(nmid1a, ct5+7, result, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::Any,
                       gpstk::NavSearchOrder::User));
   result.reset();
      // ask for almanac data from a healthy SV
   TUASSERT(fact1.find(nmid1a, ct5+7, result, gpstk::SVHealth::Healthy,
                       gpstk::NavValidityType::Any,
                       gpstk::NavSearchOrder::User));
      // we asked for almanac from healthy SV, so we should have
      // gotten the data from PRN 1
   TUASSERTE(int, 1, result->signal.xmitSat.id);
   result.reset();
      // ask for almanac data from an unhealthy SV
   TUASSERT(fact1.find(nmid1a, ct5+7, result, gpstk::SVHealth::Unhealthy,
                       gpstk::NavValidityType::Any,
                       gpstk::NavSearchOrder::User));
      // we asked for almanac from unhealthy SV, so we should have
      // gotten the data from PRN 2
   TUASSERTE(int, 2, result->signal.xmitSat.id);
   result.reset();
   TURETURN();
}


int main()
{
   NavDataFactoryWithStore_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.addNavDataTest();
   errorTotal += testClass.editTest();
   errorTotal += testClass.clearTest();
   errorTotal += testClass.findTest();
   errorTotal += testClass.find2Test();
   errorTotal += testClass.findNearestTest();
   errorTotal += testClass.findXmitHealthTest();
   errorTotal += testClass.getOffsetTest();
   errorTotal += testClass.getOffset2Test();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
