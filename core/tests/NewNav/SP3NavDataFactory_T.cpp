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
#include "SP3NavDataFactory.hpp"
#include "TestUtil.hpp"
#include "OrbitDataSP3.hpp"
#include "CivilTime.hpp"
#include "GPSWeekSecond.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

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
}

   /** Implement a test class to expose protected members rather than
    * using friends. */
class TestClass : public gnsstk::SP3NavDataFactory
{
public:
      /// Grant access to protected data.
   gnsstk::NavMessageMap& getData()
   { return data; }
};

/// Automated tests for gnsstk::SP3NavDataFactory
class SP3NavDataFactory_T
{
public:
      /// Make sure the constructor does what it's supposed to.
   unsigned constructorTest();
      /// Exercise loadIntoMap by loading data with different options in place.
   unsigned loadIntoMapTest();
      /// Test find where the time stamps exactly match epochs in source data.
   unsigned findExactTest();
      /// Test find where interpolation is required.
   unsigned findInterpTest();
      /// Test find with edge cases
   unsigned findEdgeTest();
      /// Test find with an SP3c file which contains P/EP, V/EV records
   unsigned sp3cPVTest();
      /// Test find with an SP3c file which contains P/EP records (no V)
   unsigned sp3cPTest();
      /// Test find with gap interval
   unsigned gapTest();
      /// Test nomTimeStep via the friendlier wrapper methods.
   unsigned nomTimeStepTest();
      /** Exercise loadIntoMap by loading mixed source data.
       * @param[in] badPos Set the rejectBadPosFlag to this value.
       * @param[in] badClk Set the rejectBadClkFlag to this value.
       * @param[in] rcBefore Call useRinexClockData with this value
       *   before calling addDataSource.
       * @param[in] rcAfter Call useRinexClockData with this value
       *   after calling addDataSource.
       */
   unsigned loadIntoMapFGNSSTest(bool badPos, bool badClk, bool rcBefore,
                                 bool rcAfter);
      /// Test loading of RINEX clock data.
   unsigned addRinexClockTest();
      /** Use dynamic_cast to verify that the contents of nmm are the
       * right class.
       * @param[in] testFramework The test framework created by TUDEF,
       *   used by TUASSERT macros in this function.
       * @param[in] nmm The data map to check. */
   template <class NavClass>
   void verifyDataType(gnsstk::TestUtil& testFramework,
                       gnsstk::NavMessageMap& nmm);
};


unsigned SP3NavDataFactory_T ::
constructorTest()
{
   TUDEF("SP3NavDataFactory", "SP3NavDataFactory");
   gnsstk::SP3NavDataFactory fact;
      // check for expected signal support
   gnsstk::NavSignalID nsid1(gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::CA,
                            gnsstk::NavType::GPSLNAV);
   TUASSERT(fact.supportedSignals.count(nsid1));
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
loadIntoMapTest()
{
   TUDEF("SP3NavDataFactory", "loadIntoMap");

      // test loading SP3a nav
   gnsstk::SP3NavDataFactory f2;
   std::string f2name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_SP3a.sp3";
      // this should implicitly load into the data map
   TUASSERT(f2.addDataSource(f2name));
   TUASSERTE(size_t, 232, f2.size());

      // test loading SP3b nav
   gnsstk::SP3NavDataFactory f3;
   std::string f3name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_SP3b.sp3";
      // this should implicitly load into the data map
   TUASSERT(f3.addDataSource(f3name));
   TUASSERTE(size_t, 232, f3.size());

   TestClass f4;
   std::string f4name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUCATCH(f4.setTypeFilter({gnsstk::NavMessageType::Ephemeris}));
      // this should implicitly load into the data map
   TUASSERT(f4.addDataSource(f4name));
   TUASSERTE(size_t, 116, f4.size());
   gnsstk::NavMessageMap &nmm4(f4.getData());
      // only one message type
   TUASSERTE(size_t, 1, nmm4.size());
      // and it's ephemeris.
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Ephemeris,
             nmm4.begin()->first);
   TUCSM("convertToOrbit");
   verifyDataType<gnsstk::OrbitDataSP3>(testFramework, nmm4);
   TUCSM("loadIntoMap");

   TestClass f5;
   std::string f5name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUCATCH(f5.setTypeFilter({gnsstk::NavMessageType::Health}));
      // this should implicitly load into the data map
   TUASSERT(f5.addDataSource(f5name));
   TUASSERTE(size_t, 0, f5.size());
   TUCSM("loadIntoMap");

   TestClass f6;
   std::string f6name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUCATCH(f6.setTypeFilter({gnsstk::NavMessageType::Almanac}));
      // this should implicitly load into the data map
   TUASSERT(f6.addDataSource(f6name));
      // except there isn't any data, because SP3 NAV doesn't contain almanacs
   TUASSERTE(size_t, 0, f6.size());

      // test loading something that isn't SP3
   TestClass f7;
   std::string f7name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "arlm2000.15n";
   TUASSERT(!f7.addDataSource(f7name));
   TUASSERTE(size_t, 0, f7.size());

   TURETURN();
}


unsigned SP3NavDataFactory_T ::
findExactTest()
{
   TUDEF("SP3NavDataFactory", "find");
   std::string fname;
   gnsstk::OrbitDataSP3 *uut;
   gnsstk::SP3NavDataFactory fact;
   gnsstk::NavSatelliteID satID1(7, 7, gnsstk::SatelliteSystem::GPS,
                                gnsstk::CarrierBand::L1,
                                gnsstk::TrackingCode::CA,
                                gnsstk::NavType::GPSLNAV);
   gnsstk::NavSatelliteID satID1b(7, 7, gnsstk::SatelliteSystem::GPS,
                                 gnsstk::CarrierBand::L2,
                                 gnsstk::TrackingCode::Y,
                                 gnsstk::NavType::GPSLNAV);
   gnsstk::SatID expSat1(7, gnsstk::SatelliteSystem::GPS);
   gnsstk::NavMessageID nmid1(satID1, gnsstk::NavMessageType::Ephemeris);
   gnsstk::NavMessageID nmid1b(satID1b, gnsstk::NavMessageType::Ephemeris);
   fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUASSERT(fact.addDataSource(fname));
      // 116 ephemeris + 116 clock
   TUASSERTE(size_t, 232, fact.size());

      // test find with exact epoch at start of file
   gnsstk::CivilTime civ1(2001, 7, 22, 0, 0, 0, gnsstk::TimeSystem::GPS);
   gnsstk::CommonTime ct1(civ1);
   gnsstk::NavDataPtr nd1;
   TUASSERT(fact.find(nmid1, ct1, nd1, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd1.get());
      // NavData
   TUASSERTE(gnsstk::CommonTime, ct1, uut->timeStamp);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gnsstk::SatID, expSat1, uut->signal.sat);
   TUASSERTE(gnsstk::SatID, expSat1, uut->signal.xmitSat);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS,
             uut->signal.system);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, uut->signal.obs.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::CA, uut->signal.obs.code);
   TUASSERTE(gnsstk::NavType, gnsstk::NavType::GPSLNAV, uut->signal.nav);
      // OrbitData
      // OrbitDataSP3
      // Epsilon specified because of rounding errors and because of
      // what precision is available in the source data.
   TUASSERTFEPS(-18707.084879, uut->pos[0], 0.000001);
   TUASSERTFEPS( 16766.780691, uut->pos[1], 0.000001);
   TUASSERTFEPS(  8582.072924, uut->pos[2], 0.000001);
   TUASSERTFEPS(468.885781, uut->clkBias, 0.000001);
   TUASSERTFEPS(-10775.825536, uut->vel[0], 0.000001);
   TUASSERTFEPS(  2077.583886, uut->vel[1], 0.000001);
   TUASSERTFEPS(-28974.934472, uut->vel[2], 0.000001);
   TUASSERTFEPS(-2.52832e-5, uut->clkDrift, 0.0000000001);
   TUASSERTE(std::string, "WGS84", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::WGS84G873),
             uut->frame);

      // test find with a different carrier/ranging code
   TUASSERT(fact.find(nmid1b, ct1, nd1, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd1.get());
      // NavData
   TUASSERTE(gnsstk::CommonTime, ct1, uut->timeStamp);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gnsstk::SatID, expSat1, uut->signal.sat);
   TUASSERTE(gnsstk::SatID, expSat1, uut->signal.xmitSat);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS,
             uut->signal.system);
      // note that we do NOT change the code/carrier on output.
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, uut->signal.obs.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::CA, uut->signal.obs.code);
   TUASSERTE(gnsstk::NavType, gnsstk::NavType::GPSLNAV, uut->signal.nav);
      // OrbitData
      // OrbitDataSP3
      // Epsilon specified because of rounding errors and because of
      // what precision is available in the source data.
   TUASSERTFEPS(-18707.084879, uut->pos[0], 0.000001);
   TUASSERTFEPS( 16766.780691, uut->pos[1], 0.000001);
   TUASSERTFEPS(  8582.072924, uut->pos[2], 0.000001);
   TUASSERTFEPS(468.885781, uut->clkBias, 0.000001);
   TUASSERTFEPS(-10775.825536, uut->vel[0], 0.000001);
   TUASSERTFEPS(  2077.583886, uut->vel[1], 0.000001);
   TUASSERTFEPS(-28974.934472, uut->vel[2], 0.000001);
   TUASSERTFEPS(-2.52832e-5, uut->clkDrift, 0.0000000001);
   TUASSERTE(std::string, "WGS84", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::WGS84G873),
             uut->frame);

      // test find with exact epoch at end of file
   gnsstk::CivilTime civ2(2001, 7, 22, 0, 45, 0, gnsstk::TimeSystem::GPS);
   gnsstk::CommonTime ct2(civ2);
   gnsstk::NavDataPtr nd2;
   gnsstk::NavSatelliteID satID2(6, 6, gnsstk::SatelliteSystem::GPS,
                                gnsstk::CarrierBand::L1,
                                gnsstk::TrackingCode::CA,
                                gnsstk::NavType::GPSLNAV);
   gnsstk::SatID expSat2(6, gnsstk::SatelliteSystem::GPS);
   gnsstk::NavMessageID nmid2(satID2, gnsstk::NavMessageType::Ephemeris);
   TUASSERT(fact.find(nmid2, ct2, nd2, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd2.get());
      // NavData
   TUASSERTE(gnsstk::CommonTime, ct2, uut->timeStamp);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gnsstk::SatID, expSat2, uut->signal.sat);
   TUASSERTE(gnsstk::SatID, expSat2, uut->signal.xmitSat);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS,
             uut->signal.system);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, uut->signal.obs.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::CA, uut->signal.obs.code);
   TUASSERTE(gnsstk::NavType, gnsstk::NavType::GPSLNAV, uut->signal.nav);
      // OrbitData
      // OrbitDataSP3
      // Epsilon specified because of rounding errors and because of
      // what precision is available in the source data.
   TUASSERTFEPS(  21107.001443, uut->pos[0], 0.000001);
   TUASSERTFEPS(  -1108.815431, uut->pos[1], 0.000001);
   TUASSERTFEPS(  16383.326664, uut->pos[2], 0.000001);
   TUASSERTFEPS(     -2.073849, uut->clkBias, 0.000001);
   TUASSERTFEPS( -15090.299480, uut->vel[0], 0.000001);
   TUASSERTFEPS(  13950.254271, uut->vel[1], 0.000001);
   TUASSERTFEPS(  20382.028718, uut->vel[2], 0.000001);
   TUASSERTFEPS(      8.499e-7, uut->clkDrift, 0.0000000001);
   TUASSERTE(std::string, "WGS84", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::WGS84G873),
             uut->frame);

      // test find with exact epoch in the middle of file
   gnsstk::CivilTime civ3(2001, 7, 22, 0, 30, 0, gnsstk::TimeSystem::GPS);
   gnsstk::CommonTime ct3(civ3);
   gnsstk::NavDataPtr nd3;
   gnsstk::NavSatelliteID satID3(11, 11, gnsstk::SatelliteSystem::GPS,
                                gnsstk::CarrierBand::L1,
                                gnsstk::TrackingCode::CA,
                                gnsstk::NavType::GPSLNAV);
   gnsstk::SatID expSat3(11, gnsstk::SatelliteSystem::GPS);
   gnsstk::NavMessageID nmid3(satID3, gnsstk::NavMessageType::Ephemeris);
   TUASSERT(fact.find(nmid3, ct3, nd3, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd3.get());
      // NavData
   TUASSERTE(gnsstk::CommonTime, ct3, uut->timeStamp);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gnsstk::SatID, expSat3, uut->signal.sat);
   TUASSERTE(gnsstk::SatID, expSat3, uut->signal.xmitSat);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS,
             uut->signal.system);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, uut->signal.obs.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::CA, uut->signal.obs.code);
   TUASSERTE(gnsstk::NavType, gnsstk::NavType::GPSLNAV, uut->signal.nav);
      // OrbitData
      // OrbitDataSP3
      // Epsilon specified because of rounding errors and because of
      // what precision is available in the source data.
   TUASSERTFEPS( -20916.443568, uut->pos[0], 0.000001);
   TUASSERTFEPS( -14840.885192, uut->pos[1], 0.000001);
   TUASSERTFEPS(  -6804.522988, uut->pos[2], 0.000001);
   TUASSERTFEPS(      2.966233, uut->clkBias, 0.000001);
   TUASSERTFEPS(   9547.031126, uut->vel[0], 0.000001);
   TUASSERTFEPS(    -35.054953, uut->vel[1], 0.000001);
   TUASSERTFEPS( -29243.893155, uut->vel[2], 0.000001);
   TUASSERTFEPS(      2.133e-7, uut->clkDrift, 0.0000000001);
   TUASSERTE(std::string, "WGS84", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::WGS84G873),
             uut->frame);

      // Test using data where velocity is missing and must be interpolated.
      // This test makes sure we're getting velocity data when there
      // isn't any in the original "exact" record.
   gnsstk::SP3NavDataFactory fact4;
   fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_sp3_nav_ephemerisData.sp3";
   TUASSERT(fact4.addDataSource(fname));
   TUASSERTE(size_t, 4800, fact4.size());
   gnsstk::CommonTime ct4 = gnsstk::CivilTime(1997, 4, 6, 1, 15, 0,
                                            gnsstk::TimeSystem::GPS);
   gnsstk::NavDataPtr nd4;
   gnsstk::NavSatelliteID satID4(9, 9, gnsstk::SatelliteSystem::GPS,
                                gnsstk::CarrierBand::L1,
                                gnsstk::TrackingCode::CA,
                                gnsstk::NavType::GPSLNAV);
   gnsstk::SatID expSat4(9, gnsstk::SatelliteSystem::GPS);
   gnsstk::NavMessageID nmid4(satID4, gnsstk::NavMessageType::Ephemeris);
   TUASSERT(fact4.find(nmid4, ct4, nd4, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::ValidOnly,
                       gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd4.get());
      // NavData
   TUASSERTE(gnsstk::CommonTime, ct4, uut->timeStamp);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gnsstk::SatID, expSat4, uut->signal.sat);
   TUASSERTE(gnsstk::SatID, expSat4, uut->signal.xmitSat);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS,
             uut->signal.system);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, uut->signal.obs.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::CA, uut->signal.obs.code);
   TUASSERTE(gnsstk::NavType, gnsstk::NavType::GPSLNAV, uut->signal.nav);
      // OrbitData
      // OrbitDataSP3
   TUASSERTFE(-10012.152394, uut->pos[0]);
   TUASSERTFE(-14853.431997, uut->pos[1]);
   TUASSERTFE(-19726.785509, uut->pos[2]);
   TUASSERTFE(-33.642019, uut->clkBias);
   TUASSERTFE(24978.879448941785085, uut->vel[0]);
   TUASSERTFE(237.71409059520243545, uut->vel[1]);
   TUASSERTFE(-12599.181107279617208, uut->vel[2]);
   TUASSERTFE(8.4786249559075835378e-05, uut->clkDrift);
   TUASSERTE(std::string, "ITR94", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF94),
             uut->frame);
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
findInterpTest()
{
   TUDEF("SP3NavDataFactory", "find");
   gnsstk::SP3NavDataFactory fact;
   gnsstk::NavSatelliteID satID1(15, 15, gnsstk::SatelliteSystem::GPS,
                                gnsstk::CarrierBand::L1,
                                gnsstk::TrackingCode::CA,
                                gnsstk::NavType::GPSLNAV);
   gnsstk::SatID expSat1(15, gnsstk::SatelliteSystem::GPS);
   gnsstk::NavMessageID nmid1(satID1, gnsstk::NavMessageType::Ephemeris);
   gnsstk::CommonTime ct;
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_sp3_nav_ephemerisData.sp3";
   TUASSERT(fact.addDataSource(fname));
      // 2400 ephemeris + 2400 clock
   TUASSERTE(size_t, 4800, fact.size());
   ct = gnsstk::CivilTime(1997,4,6,0,45,2,gnsstk::TimeSystem::GPS);
   gnsstk::NavDataPtr nd1;
   TUASSERT(!fact.find(nmid1, ct, nd1, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::ValidOnly,
                       gnsstk::NavSearchOrder::User));

   ct = gnsstk::CivilTime(1997,4,6,6,17,36,gnsstk::TimeSystem::GPS);
   gnsstk::NavDataPtr nd2;
   TUASSERT(fact.find(nmid1, ct, nd2, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   gnsstk::OrbitDataSP3 *uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd2.get());
   if (uut == nullptr)
      TURETURN();
      // NavData
   TUASSERTE(gnsstk::CommonTime, ct, uut->timeStamp);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gnsstk::SatID, expSat1, uut->signal.sat);
   TUASSERTE(gnsstk::SatID, expSat1, uut->signal.xmitSat);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS,
             uut->signal.system);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, uut->signal.obs.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::CA, uut->signal.obs.code);
   TUASSERTE(gnsstk::NavType, gnsstk::NavType::GPSLNAV, uut->signal.nav);
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
   TUASSERTE(std::string, "ITR94", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF94),
             uut->frame);
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
findEdgeTest()
{
   TUDEF("SP3NavDataFactory", "find");
   gnsstk::SP3NavDataFactory fact;
   gnsstk::NavSatelliteID satID1(1, 1, gnsstk::SatelliteSystem::GPS,
                                gnsstk::CarrierBand::L1,
                                gnsstk::TrackingCode::CA,
                                gnsstk::NavType::GPSLNAV);
   gnsstk::SatID expSat1(1, gnsstk::SatelliteSystem::GPS);
   gnsstk::NavMessageID nmid1(satID1, gnsstk::NavMessageType::Ephemeris);
   gnsstk::CommonTime ct;
   gnsstk::NavDataPtr nd;
   gnsstk::OrbitDataSP3 *uut = nullptr;
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_sp3_nav_ephemerisData.sp3";
   TUASSERT(fact.addDataSource(fname));
   TUASSERTE(size_t, 4800, fact.size());

      // test really early time
   ct = gnsstk::CivilTime(1996,4,6,0,0,0,gnsstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::ValidOnly,
                       gnsstk::NavSearchOrder::User));
      // test just before data start
   ct = gnsstk::CivilTime(1997,4,5,23,59,59,gnsstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::ValidOnly,
                       gnsstk::NavSearchOrder::User));
      // exactly at start
      // SP3EphemerisStore rejects this case
   ct = gnsstk::CivilTime(1997,4,6,0,0,0,gnsstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd.get());
   TUASSERTFE(-21401.653416, uut->pos[0]);
   TUASSERTFE(15729.974324, uut->pos[1]);
   TUASSERTFE(36.858903, uut->pos[2]);
   TUASSERTFE(0, uut->posSig[0]);
   TUASSERTFE(0, uut->posSig[1]);
   TUASSERTFE(0, uut->posSig[2]);
   TUASSERTFE(0, uut->vel[0]);
   TUASSERTFE(0, uut->vel[1]);
   TUASSERTFE(0, uut->vel[2]);
   TUASSERTFE(0, uut->velSig[0]);
   TUASSERTFE(0, uut->velSig[1]);
   TUASSERTFE(0, uut->velSig[2]);
   TUASSERTFE(0, uut->acc[0]);
   TUASSERTFE(0, uut->acc[1]);
   TUASSERTFE(0, uut->acc[2]);
   TUASSERTFE(0, uut->accSig[0]);
   TUASSERTFE(0, uut->accSig[1]);
   TUASSERTFE(0, uut->accSig[2]);
   TUASSERTFE(16.836506, uut->clkBias);
   TUASSERTFE(0, uut->biasSig);
   TUASSERTFE(0, uut->clkDrift);
   TUASSERTFE(0, uut->driftSig);
   TUASSERTFE(0, uut->clkDrRate);
   TUASSERTFE(0, uut->drRateSig);
   TUASSERTE(std::string, "ITR94", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF94),
             uut->frame);
      // Just after start - can't interpolate
   ct = gnsstk::CivilTime(1997,4,6,0,0,2,gnsstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::ValidOnly,
                       gnsstk::NavSearchOrder::User));
      // exact match but too early to interpolate
      // SP3EphemerisStore rejects this case
   ct = gnsstk::CivilTime(1997,4,6,0,15,0,gnsstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd.get());
   TUASSERTFE(-21435.113111, uut->pos[0]);
   TUASSERTFE(15409.762699, uut->pos[1]);
   TUASSERTFE(-2801.263736, uut->pos[2]);
   TUASSERTFE(0, uut->posSig[0]);
   TUASSERTFE(0, uut->posSig[1]);
   TUASSERTFE(0, uut->posSig[2]);
   TUASSERTFE(0, uut->vel[0]);
   TUASSERTFE(0, uut->vel[1]);
   TUASSERTFE(0, uut->vel[2]);
   TUASSERTFE(0, uut->velSig[0]);
   TUASSERTFE(0, uut->velSig[1]);
   TUASSERTFE(0, uut->velSig[2]);
   TUASSERTFE(0, uut->acc[0]);
   TUASSERTFE(0, uut->acc[1]);
   TUASSERTFE(0, uut->acc[2]);
   TUASSERTFE(0, uut->accSig[0]);
   TUASSERTFE(0, uut->accSig[1]);
   TUASSERTFE(0, uut->accSig[2]);
   TUASSERTFE(16.869907, uut->clkBias);
   TUASSERTFE(0, uut->biasSig);
   TUASSERTFE(0, uut->clkDrift);
   TUASSERTFE(0, uut->driftSig);
   TUASSERTFE(0, uut->clkDrRate);
   TUASSERTFE(0, uut->drRateSig);
   TUASSERTE(std::string, "ITR94", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF94),
             uut->frame);
      // Still not enough data to interpolate
   ct = gnsstk::CivilTime(1997,4,6,0,15,2,gnsstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::ValidOnly,
                       gnsstk::NavSearchOrder::User));
      // exact match but too early to interpolate
      // SP3EphemerisStore rejects this case
   ct = gnsstk::CivilTime(1997,4,6,0,30,0,gnsstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd.get());
   TUASSERTFE(-21246.756641, uut->pos[0]);
   TUASSERTFE(14879.740466, uut->pos[1]);
   TUASSERTFE(-5591.090702, uut->pos[2]);
   TUASSERTFE(0, uut->posSig[0]);
   TUASSERTFE(0, uut->posSig[1]);
   TUASSERTFE(0, uut->posSig[2]);
   TUASSERTFE(0, uut->vel[0]);
   TUASSERTFE(0, uut->vel[1]);
   TUASSERTFE(0, uut->vel[2]);
   TUASSERTFE(0, uut->velSig[0]);
   TUASSERTFE(0, uut->velSig[1]);
   TUASSERTFE(0, uut->velSig[2]);
   TUASSERTFE(0, uut->acc[0]);
   TUASSERTFE(0, uut->acc[1]);
   TUASSERTFE(0, uut->acc[2]);
   TUASSERTFE(0, uut->accSig[0]);
   TUASSERTFE(0, uut->accSig[1]);
   TUASSERTFE(0, uut->accSig[2]);
   TUASSERTFE(16.861797, uut->clkBias);
   TUASSERTFE(0, uut->biasSig);
   TUASSERTFE(0, uut->clkDrift);
   TUASSERTFE(0, uut->driftSig);
   TUASSERTFE(0, uut->clkDrRate);
   TUASSERTFE(0, uut->drRateSig);
   TUASSERTE(std::string, "ITR94", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF94),
             uut->frame);
      // Still not enough data to interpolate
   ct = gnsstk::CivilTime(1997,4,6,0,30,2,gnsstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::ValidOnly,
                       gnsstk::NavSearchOrder::User));
      // Still not enough data to interpolate
   ct = gnsstk::CivilTime(1997,4,6,0,45,2,gnsstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::ValidOnly,
                       gnsstk::NavSearchOrder::User));
      // exact match but too early to interpolate
      // SP3EphemerisStore rejects this case
   ct = gnsstk::CivilTime(1997,4,6,1,0,0,gnsstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd.get());
   TUASSERTFE(-20332.738063, uut->pos[0]);
   TUASSERTFE(13115.151460, uut->pos[1]);
   TUASSERTFE(-10834.501567, uut->pos[2]);
   TUASSERTFE(0, uut->posSig[0]);
   TUASSERTFE(0, uut->posSig[1]);
   TUASSERTFE(0, uut->posSig[2]);
   TUASSERTFE(0, uut->vel[0]);
   TUASSERTFE(0, uut->vel[1]);
   TUASSERTFE(0, uut->vel[2]);
   TUASSERTFE(0, uut->velSig[0]);
   TUASSERTFE(0, uut->velSig[1]);
   TUASSERTFE(0, uut->velSig[2]);
   TUASSERTFE(0, uut->acc[0]);
   TUASSERTFE(0, uut->acc[1]);
   TUASSERTFE(0, uut->acc[2]);
   TUASSERTFE(0, uut->accSig[0]);
   TUASSERTFE(0, uut->accSig[1]);
   TUASSERTFE(0, uut->accSig[2]);
   TUASSERTFE(16.770325, uut->clkBias);
   TUASSERTFE(0, uut->biasSig);
   TUASSERTFE(0, uut->clkDrift);
   TUASSERTFE(0, uut->driftSig);
   TUASSERTFE(0, uut->clkDrRate);
   TUASSERTFE(0, uut->drRateSig);
   TUASSERTE(std::string, "ITR94", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF94),
             uut->frame);
      // now we can interpolate
   ct = gnsstk::CivilTime(1997,4,6,1,0,2,gnsstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd.get());
   TUASSERTFE(-20331.405273885055067, uut->pos[0]);
   TUASSERTFE(13112.643368661305431, uut->pos[1]);
   TUASSERTFE(-10839.975527923066693, uut->pos[2]);
   TUASSERTFE(0, uut->posSig[0]);
   TUASSERTFE(0, uut->posSig[1]);
   TUASSERTFE(0, uut->posSig[2]);
   TUASSERTFE(6665.3850065908245597, uut->vel[0]);
   TUASSERTFE(-12543.536761410498002, uut->vel[1]);
   TUASSERTFE(-27367.485483716285671, uut->vel[2]);
   TUASSERTFE(0, uut->velSig[0]);
   TUASSERTFE(0, uut->velSig[1]);
   TUASSERTFE(0, uut->velSig[2]);
   TUASSERTFE(0, uut->acc[0]);
   TUASSERTFE(0, uut->acc[1]);
   TUASSERTFE(0, uut->acc[2]);
   TUASSERTFE(0, uut->accSig[0]);
   TUASSERTFE(0, uut->accSig[1]);
   TUASSERTFE(0, uut->accSig[2]);
   TUASSERTFE(16.770692382796031694, uut->clkBias);
   TUASSERTFE(0, uut->biasSig);
   TUASSERTFE(0.00018445880134187152277, uut->clkDrift);
   TUASSERTFE(0, uut->driftSig);
   TUASSERTFE(0, uut->clkDrRate);
   TUASSERTFE(0, uut->drRateSig);
   TUASSERTE(std::string, "ITR94", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF94),
             uut->frame);
      // interpolating near the end of the data set
   ct = gnsstk::CivilTime(1997,4,6,22,30,2,gnsstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd.get());
   TUASSERTFE(-16143.2374434571866, uut->pos[0]);
   TUASSERTFE(15106.62860632441334, uut->pos[1]);
   TUASSERTFE(14841.24585356388161, uut->pos[2]);
   TUASSERTFE(0, uut->posSig[0]);
   TUASSERTFE(0, uut->posSig[1]);
   TUASSERTFE(0, uut->posSig[2]);
   TUASSERTFE(-18584.962108554645965, uut->vel[0]);
   TUASSERTFE(2647.4717546382453293, uut->vel[1]);
   TUASSERTFE(-23100.125997620009002, uut->vel[2]);
   TUASSERTFE(0, uut->velSig[0]);
   TUASSERTFE(0, uut->velSig[1]);
   TUASSERTFE(0, uut->velSig[2]);
   TUASSERTFE(0, uut->acc[0]);
   TUASSERTFE(0, uut->acc[1]);
   TUASSERTFE(0, uut->acc[2]);
   TUASSERTFE(0, uut->accSig[0]);
   TUASSERTFE(0, uut->accSig[1]);
   TUASSERTFE(0, uut->accSig[2]);
   TUASSERTFE(16.957767942749853773, uut->clkBias);
   TUASSERTFE(0, uut->biasSig);
   TUASSERTFE(-2.9789139950418162616e-05, uut->clkDrift);
   TUASSERTFE(0, uut->driftSig);
   TUASSERTFE(0, uut->clkDrRate);
   TUASSERTFE(0, uut->drRateSig);
   TUASSERTE(std::string, "ITR94", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF94),
             uut->frame);
      // The last time we can interpolate
   ct = gnsstk::CivilTime(1997,4,6,22,45,0,gnsstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd.get());
   TUASSERTFE(-17689.961147, uut->pos[0]);
   TUASSERTFE(15351.609005, uut->pos[1]);
   TUASSERTFE(12646.527369, uut->pos[2]);
   TUASSERTFE(0, uut->posSig[0]);
   TUASSERTFE(0, uut->posSig[1]);
   TUASSERTFE(0, uut->posSig[2]);
   TUASSERTFE(-15823.931961018606671, uut->vel[0]);
   TUASSERTFE(2741.9894129365502522, uut->vel[1]);
   TUASSERTFE(-25711.116386300676822, uut->vel[2]);
   TUASSERTFE(0, uut->velSig[0]);
   TUASSERTFE(0, uut->velSig[1]);
   TUASSERTFE(0, uut->velSig[2]);
   TUASSERTFE(0, uut->acc[0]);
   TUASSERTFE(0, uut->acc[1]);
   TUASSERTFE(0, uut->acc[2]);
   TUASSERTFE(0, uut->accSig[0]);
   TUASSERTFE(0, uut->accSig[1]);
   TUASSERTFE(0, uut->accSig[2]);
   TUASSERTFE(16.892600, uut->clkBias);
   TUASSERTFE(0, uut->biasSig);
   TUASSERTFE(-4.1154966490297820213e-05, uut->clkDrift);
   TUASSERTFE(0, uut->driftSig);
   TUASSERTFE(0, uut->clkDrRate);
   TUASSERTFE(0, uut->drRateSig);
   TUASSERTE(std::string, "ITR94", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF94),
             uut->frame);
      // no longer able to interpolate
   ct = gnsstk::CivilTime(1997,4,6,22,45,2,gnsstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                       gnsstk::NavValidityType::ValidOnly,
                       gnsstk::NavSearchOrder::User));
      // Exact match but too late to interpolate.  The last possible
      // time we can get from the source data.
      // SP3EphemerisStore rejects this case
   ct = gnsstk::CivilTime(1997,4,6,23,45,0,gnsstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd.get());
   TUASSERTFE(-21219.029296, uut->pos[0]);
   TUASSERTFE(15851.176408, uut->pos[1]);
   TUASSERTFE(2112.519321, uut->pos[2]);
   TUASSERTFE(0, uut->posSig[0]);
   TUASSERTFE(0, uut->posSig[1]);
   TUASSERTFE(0, uut->posSig[2]);
   TUASSERTFE(0, uut->vel[0]);
   TUASSERTFE(0, uut->vel[1]);
   TUASSERTFE(0, uut->vel[2]);
   TUASSERTFE(0, uut->velSig[0]);
   TUASSERTFE(0, uut->velSig[1]);
   TUASSERTFE(0, uut->velSig[2]);
   TUASSERTFE(0, uut->acc[0]);
   TUASSERTFE(0, uut->acc[1]);
   TUASSERTFE(0, uut->acc[2]);
   TUASSERTFE(0, uut->accSig[0]);
   TUASSERTFE(0, uut->accSig[1]);
   TUASSERTFE(0, uut->accSig[2]);
   TUASSERTFE(16.977759, uut->clkBias);
   TUASSERTFE(0, uut->biasSig);
   TUASSERTFE(0, uut->clkDrift);
   TUASSERTFE(0, uut->driftSig);
   TUASSERTFE(0, uut->clkDrRate);
   TUASSERTFE(0, uut->drRateSig);
   TUASSERTE(std::string, "ITR94", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame, gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF94),
             uut->frame);
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
sp3cPVTest()
{
   TUDEF("SP3NavDataFactory", "addDataSource");
   gnsstk::SP3NavDataFactory fact;
   gnsstk::NavSatelliteID satID1(15, 15, gnsstk::SatelliteSystem::GPS,
                                gnsstk::CarrierBand::L1,
                                gnsstk::TrackingCode::CA,
                                gnsstk::NavType::GPSLNAV);
   gnsstk::SatID expSat1(15, gnsstk::SatelliteSystem::GPS);
   gnsstk::NavMessageID nmid1(satID1, gnsstk::NavMessageType::Ephemeris);
   gnsstk::CommonTime ct;
   gnsstk::NavDataPtr nd;
   gnsstk::OrbitDataSP3 *uut = nullptr;
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_SP3c.sp3";
   TUASSERT(fact.addDataSource(fname));
   TUASSERTE(size_t, 1500, fact.size());
      // exact match
   ct = gnsstk::CivilTime(2011,10,9,3,0,0,gnsstk::TimeSystem::GPS);
   TUCSM("find");
   TUASSERT(fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd.get());
   TUASSERTFE(10376.802922, uut->pos[0]);
   TUASSERTFE(-12375.157793, uut->pos[1]);
   TUASSERTFE(21068.993967, uut->pos[2]);
   TUASSERTFE(240, uut->posSig[0]);
   TUASSERTFE(431, uut->posSig[1]);
   TUASSERTFE(95, uut->posSig[2]);
   TUASSERTFE(13892.969748999999865, uut->vel[0]);
   TUASSERTFE(22939.105573000000732, uut->vel[1]);
   TUASSERTFE(6812.2288810000000012, uut->vel[2]);
   TUASSERTFE(3739, uut->velSig[0]);
   TUASSERTFE(7214, uut->velSig[1]);
   TUASSERTFE(3199, uut->velSig[2]);
      /// @note acc values not confirmed by SP3EphemerisStore
   TUASSERTFE(0.16868203840330744381, uut->acc[0]);
   TUASSERTFE(-0.0048099658207224796347, uut->acc[1]);
   TUASSERTFE(-0.44889948984246021979, uut->acc[2]);
   TUASSERTFE(0, uut->accSig[0]);
   TUASSERTFE(0, uut->accSig[1]);
   TUASSERTFE(0, uut->accSig[2]);
   TUASSERTFE(-103.26419599999999832, uut->clkBias);
   TUASSERTFE(0.0010629999999999999259, uut->biasSig);
   TUASSERTFE(1.3259000000000000871e-06, uut->clkDrift);
   TUASSERTFE(1.0175328004541179996e-07, uut->driftSig);
   TUASSERTE(std::string, "WGS84", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::WGS84G1150),
             uut->frame);
      /// @note drift rate value not confirmed by SP3EphemerisStore
   TUASSERTFE(-2.1102292768909279853e-13, uut->clkDrRate);
   TUASSERTFE(1.130592000504575508e-10, uut->drRateSig);
      // interpolated match
   ct = gnsstk::CivilTime(2011,10,9,2,1,3,gnsstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd.get());
   TUASSERTFE(6606.1838019497754431, uut->pos[0]);
   TUASSERTFE(-20053.58342304438338, uut->pos[1]);
   TUASSERTFE(16014.263355443908949, uut->pos[2]);
   TUASSERTFE(196.15555052049893447, uut->posSig[0]);
   TUASSERTFE(570.07017111931054387, uut->posSig[1]);
   TUASSERTFE(220.51077071199944157, uut->posSig[2]);
   TUASSERTFE(7401.8992499702417263, uut->vel[0]);
   TUASSERTFE(19171.152303792601742, uut->vel[1]);
   TUASSERTFE(21143.244357782645238, uut->vel[2]);
   TUASSERTFE(4345.3593637350641075, uut->velSig[0]);
   TUASSERTFE(8512.6561072323365806, uut->velSig[1]);
   TUASSERTFE(13030.428695940898251, uut->velSig[2]);
   TUASSERTFE(0.17323990334114644507, uut->acc[0]);
   TUASSERTFE(0.21491163411773372971, uut->acc[1]);
   TUASSERTFE(-0.34305460226999479056, uut->acc[2]);
   TUASSERTFE(0, uut->accSig[0]);
   TUASSERTFE(0, uut->accSig[1]);
   TUASSERTFE(0, uut->accSig[2]);
   TUASSERTFE(-103.26888741910860858, uut->clkBias);
   TUASSERTFE(0.0012506254435281571272, uut->biasSig);
   TUASSERTFE(1.3265841739403660333e-06, uut->clkDrift);
   TUASSERTFE(9.9773393246897238821e-08, uut->driftSig);
   TUASSERTFE(-2.5152847174005956486e-13, uut->clkDrRate);
   TUASSERTFE(1.1085932582988582625e-10, uut->drRateSig);
   TUASSERTE(std::string, "WGS84", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::WGS84G1150),
             uut->frame);
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
sp3cPTest()
{
   TUDEF("SP3NavDataFactory", "addDataSource");
   gnsstk::SP3NavDataFactory fact;
   gnsstk::NavSatelliteID satID1(15, 15, gnsstk::SatelliteSystem::GPS,
                                gnsstk::CarrierBand::L1,
                                gnsstk::TrackingCode::CA,
                                gnsstk::NavType::GPSLNAV);
   gnsstk::SatID expSat1(15, gnsstk::SatelliteSystem::GPS);
   gnsstk::NavMessageID nmid1(satID1, gnsstk::NavMessageType::Ephemeris);
   gnsstk::CommonTime ct;
   gnsstk::NavDataPtr nd;
   gnsstk::OrbitDataSP3 *uut = nullptr;
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_SP3c_pos.sp3";
   TUASSERT(fact.addDataSource(fname));
   TUASSERTE(size_t, 1500, fact.size());
      // exact match
   ct = gnsstk::CivilTime(2011,10,9,3,0,0,gnsstk::TimeSystem::GPS);
   TUCSM("find");
   TUASSERT(fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd.get());
   TUASSERTFE(10376.802922, uut->pos[0]);
   TUASSERTFE(-12375.157793, uut->pos[1]);
   TUASSERTFE(21068.993967, uut->pos[2]);
   TUASSERTFE(240, uut->posSig[0]);
   TUASSERTFE(431, uut->posSig[1]);
   TUASSERTFE(95, uut->posSig[2]);
   TUASSERTFE(13892.970831807802824, uut->vel[0]);
   TUASSERTFE(22939.106487579352688, uut->vel[1]);
   TUASSERTFE(6812.2288926808514589, uut->vel[2]);
   TUASSERTFE(0, uut->velSig[0]);
   TUASSERTFE(0, uut->velSig[1]);
   TUASSERTFE(0, uut->velSig[2]);
   TUASSERTFE(0, uut->acc[0]);
   TUASSERTFE(0, uut->acc[1]);
   TUASSERTFE(0, uut->acc[2]);
   TUASSERTFE(0, uut->accSig[0]);
   TUASSERTFE(0, uut->accSig[1]);
   TUASSERTFE(0, uut->accSig[2]);
   TUASSERTFE(-103.26419599999999832, uut->clkBias);
   TUASSERTFE(0.0010629999999999999259, uut->biasSig);
   TUASSERTFE(1.3253443562296393209e-06, uut->clkDrift);
   TUASSERTFE(1.1811111111111110024e-06, uut->driftSig);
   TUASSERTFE(0, uut->clkDrRate);
   TUASSERTFE(0, uut->drRateSig);
   TUASSERTE(std::string, "WGS84", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::WGS84G1150),
             uut->frame);
      // interpolated match
   ct = gnsstk::CivilTime(2011,10,9,2,1,3,gnsstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
   uut = dynamic_cast<gnsstk::OrbitDataSP3*>(nd.get());

   TUASSERTFE(6606.1838019497745336, uut->pos[0]);
   TUASSERTFE(-20053.583423044379742, uut->pos[1]);
   TUASSERTFE(16014.263355443905311, uut->pos[2]);
   TUASSERTFE(196.15555052049893447, uut->posSig[0]);
   TUASSERTFE(570.07017111931054387, uut->posSig[1]);
   TUASSERTFE(220.51077071199944157, uut->posSig[2]);
   TUASSERTFE(7401.9009258281712391, uut->vel[0]);
   TUASSERTFE(19171.152904308662983, uut->vel[1]);
   TUASSERTFE(21143.244419124275737, uut->vel[2]);
   TUASSERTFE(0, uut->velSig[0]);
   TUASSERTFE(0, uut->velSig[1]);
   TUASSERTFE(0, uut->velSig[2]);
   TUASSERTFE(0, uut->acc[0]);
   TUASSERTFE(0, uut->acc[1]);
   TUASSERTFE(0, uut->acc[2]);
   TUASSERTFE(0, uut->accSig[0]);
   TUASSERTFE(0, uut->accSig[1]);
   TUASSERTFE(0, uut->accSig[2]);
   TUASSERTFE(-103.26888741910859437, uut->clkBias);
   TUASSERTFE(0.0012506254435281571272, uut->biasSig);
   TUASSERTFE(1.3266807555790851962e-06, uut->clkDrift);
   TUASSERTFE(1.3895838261423968146e-06, uut->driftSig);
   TUASSERTFE(0, uut->clkDrRate);
   TUASSERTFE(0, uut->drRateSig);
   TUASSERTE(std::string, "WGS84", uut->coordSystem);
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::WGS84G1150),
             uut->frame);
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
loadIntoMapFGNSSTest(bool badPos, bool badClk, bool rcBefore, bool rcAfter)
{
   using namespace gnsstk;
   TUDEF("SP3NavDataFactory", "loadIntoMap");

      // test loading SP3c with mixed systems nav
   TestClass uut;
   ObsID
      expOidGPS(ObservationType::NavMsg,CarrierBand::L1,TrackingCode::CA),
      expOidGal(ObservationType::NavMsg,CarrierBand::L5,TrackingCode::E5aI),
      expOidQZS(ObservationType::NavMsg,CarrierBand::L1,TrackingCode::CA),
      expOidGLO(ObservationType::NavMsg,CarrierBand::G1,TrackingCode::Standard),
      expOidBei(ObservationType::NavMsg,CarrierBand::B1,TrackingCode::B1I),
      expOidOth(ObservationType::NavMsg, CarrierBand::Unknown,
                TrackingCode::Unknown);
   NavType
      expNTGPS(NavType::GPSLNAV),
      expNTGal(NavType::GalFNAV),
      expNTQZS(NavType::GPSLNAV),
      expNTGLO(NavType::GloCivilF),
      expNTBei(NavType::BeiDou_D1),
      expNTOth(NavType::Unknown);

   std::string fname = getPathData() + getFileSep() +
      "test_input_SP3c_mgex1.sp3";
   uut.rejectBadPositions(badPos);
   uut.rejectBadClocks(badClk);
   uut.useRinexClockData(rcBefore);
      // this should implicitly load into the data map
   TUASSERT(uut.addDataSource(fname));
   uut.useRinexClockData(rcAfter);
   unsigned gps = 0, gal = 0, qzs = 0, glo = 0, irn = 0, bei = 0, leo = 0,
      other = 0;
   unsigned gpsExp = 320, galExp = 110, qzsExp = 10, gloExp = 230, irnExp = 0,
      beiExp = 80, leoExp = 50, otherExp = 0, allExp = 800;
   if (!rcBefore && !rcAfter)
   {
      if ((badPos == false) && (badClk == false))
      {
            // default values apply to these conditions
      }
      else if ((badPos == false) && (badClk == true))
      {
         galExp = 102;
         beiExp =  74;
         allExp = 786;
      }
      else if ((badPos == true) && (badClk == false))
      {
            // all positions good?
      }
      else if ((badPos == true) && (badClk == true))
      {
         galExp = 102;
         beiExp =  74;
         allExp = 786;
      }
   }
   else
   {
         // end results should be the same whether the change took
         // place before or after the addDataSource call.
      if ((badPos == false) && (badClk == false))
      {
            // Lazily divide everything by 2 since you USUALLY will
            // have P/V in pairs.
         gpsExp >>= 1;
         galExp >>= 1;
         qzsExp >>= 1;
         gloExp >>= 1;
         irnExp >>= 1;
         beiExp >>= 1;
         leoExp >>= 1;
         otherExp >>= 1;
         allExp >>= 1;
      }
      else if ((badPos == false) && (badClk == true))
      {
         gpsExp >>= 1;
         galExp =  51;
         qzsExp >>= 1;
         gloExp >>= 1;
         irnExp >>= 1;
         beiExp =  37;
         leoExp >>= 1;
         otherExp >>= 1;
         allExp = 393;
      }
      else if ((badPos == true) && (badClk == false))
      {
         gpsExp >>= 1;
         galExp >>= 1;
         qzsExp >>= 1;
         gloExp >>= 1;
         irnExp >>= 1;
         beiExp >>= 1;
         leoExp >>= 1;
         otherExp >>= 1;
         allExp >>= 1;
      }
      else if ((badPos == true) && (badClk == true))
      {
         gpsExp >>= 1;
         galExp =  51;
         qzsExp >>= 1;
         gloExp >>= 1;
         irnExp >>= 1;
         beiExp =  37;
         leoExp >>= 1;
         otherExp >>= 1;
         allExp = 393;
      }
   }
   TUASSERTE(size_t, allExp, uut.size());
   for (auto& nmti : uut.getData())
   {
      for (auto& sati : nmti.second)
      {
         for (auto& ti : sati.second)
         {
            OrbitDataSP3 *od = dynamic_cast<OrbitDataSP3*>(
               ti.second.get());
            TUASSERTE(SatID, od->signal.sat, od->signal.xmitSat);
            TUASSERTE(SatelliteSystem, od->signal.sat.system,
                      od->signal.system);
            switch (od->signal.sat.system)
            {
               case SatelliteSystem::GPS:
                  gps++;
                  TUASSERTE(ObsID, expOidGPS, od->signal.obs);
                  TUASSERTE(NavType, expNTGPS, od->signal.nav);
                  break;
               case SatelliteSystem::Galileo:
                  gal++;
                  TUASSERTE(ObsID, expOidGal, od->signal.obs);
                  TUASSERTE(NavType, expNTGal, od->signal.nav);
                  break;
               case SatelliteSystem::Glonass:
                  glo++;
                  TUASSERTE(ObsID, expOidGLO, od->signal.obs);
                  TUASSERTE(NavType, expNTGLO, od->signal.nav);
                  break;
               case SatelliteSystem::BeiDou:
                  bei++;
                  TUASSERTE(ObsID, expOidBei, od->signal.obs);
                  TUASSERTE(NavType, expNTBei, od->signal.nav);
                  break;
               case SatelliteSystem::QZSS:
                  qzs++;
                  TUASSERTE(ObsID, expOidQZS, od->signal.obs);
                  TUASSERTE(NavType, expNTQZS, od->signal.nav);
                  break;
               case SatelliteSystem::IRNSS:
                  irn++;
                  TUASSERTE(ObsID, expOidOth, od->signal.obs);
                  TUASSERTE(NavType, expNTOth, od->signal.nav);
                  break;
               case SatelliteSystem::LEO:
                  leo++;
                  TUASSERTE(ObsID, expOidOth, od->signal.obs);
                  TUASSERTE(NavType, expNTOth, od->signal.nav);
                  break;
               default:
                  other++;
                  TUASSERTE(ObsID, expOidOth, od->signal.obs);
                  TUASSERTE(NavType, expNTOth, od->signal.nav);
                  break;
            }
         }
      }
   }
   TUASSERTE(unsigned, gpsExp, gps);
   TUASSERTE(unsigned, galExp, gal);
   TUASSERTE(unsigned, gloExp, glo);
   TUASSERTE(unsigned, beiExp, bei);
   TUASSERTE(unsigned, qzsExp, qzs);
   TUASSERTE(unsigned, irnExp, irn);
   TUASSERTE(unsigned, leoExp, leo);
   TUASSERTE(unsigned, otherExp, other);
      // make sure we can find data
   gnsstk::NavSatelliteID satID1(193, 193, gnsstk::SatelliteSystem::QZSS,
                                gnsstk::CarrierBand::L2,
                                gnsstk::TrackingCode::L2CML,
                                gnsstk::NavType::GPSCNAVL2);
   gnsstk::NavMessageID nmid1(satID1, gnsstk::NavMessageType::Ephemeris);
   gnsstk::CivilTime civ1(2016, 5, 1, 0, 15, 0, gnsstk::TimeSystem::TAI);
   gnsstk::CommonTime ct1(civ1);
   gnsstk::NavDataPtr nd1;
   if (rcBefore || rcAfter)
   {
         /// @todo should we be able to form a useful OrbitDataSP3 w/o clock?
      TURETURN();
   }
   TUASSERT(uut.find(nmid1, ct1, nd1, gnsstk::SVHealth::Any,
                     gnsstk::NavValidityType::ValidOnly,
                     gnsstk::NavSearchOrder::User));

   gnsstk::NavSatelliteID satID2(8, 8, gnsstk::SatelliteSystem::Galileo,
                                gnsstk::CarrierBand::L5,
                                gnsstk::TrackingCode::E5aI,
                                gnsstk::NavType::GalFNAV);
   gnsstk::NavMessageID nmid2(satID2, gnsstk::NavMessageType::Ephemeris);
   TUASSERT(uut.find(nmid2, ct1, nd1, gnsstk::SVHealth::Any,
                     gnsstk::NavValidityType::ValidOnly,
                     gnsstk::NavSearchOrder::User));
   TURETURN();
}


template <class NavClass>
void SP3NavDataFactory_T ::
verifyDataType(gnsstk::TestUtil& testFramework,
               gnsstk::NavMessageMap& nmm)
{
   for (auto& nmti : nmm)
   {
      for (auto& sati : nmti.second)
      {
         for (auto& ti : sati.second)
         {
            TUASSERT(dynamic_cast<NavClass*>(ti.second.get()) != nullptr);
         }
      }
   }
}


unsigned SP3NavDataFactory_T ::
addRinexClockTest()
{
   using namespace gnsstk;
   TUDEF("SP3NavDataFactory", "addRinexClockTest");

      // test loading SP3c with mixed systems nav
   TestClass uut, uut2;
   std::string fnameSP3 = getPathData() + getFileSep() +
      "test_input_SP3c_mgex1.sp3";
   std::string fnameClk = getPathData() + getFileSep() +
      "test_input_rinex3_clock_RinexClockExample.96c";
      // this should implicitly load into the data map
   TUASSERT(uut.addDataSource(fnameSP3));
   TUASSERTE(size_t, 786, uut.size());
      // this should fail because it's a different time system
   TUASSERT(!uut.addDataSource(fnameClk));
   TUASSERTE(size_t, 786, uut.size());
      // Make sure time system incompatibility is flagged as an error
   fnameSP3 = getPathData() + getFileSep() + "test_input_SP3c.sp3";
   fnameClk = getPathData() + getFileSep() +
      "test_input_rinex3_clock_RinexClockExample.96c";
   TUASSERT(!uut.addDataSource(fnameSP3));
      // First, add appropriate SP3 data (time system matching)
   TUASSERT(uut2.addDataSource(fnameSP3));
   TUASSERTE(size_t, 1500, uut2.size());
      // Add the RINEX clock data which should clear the SP3 clock records out
   TUASSERT(uut2.addDataSource(fnameClk));
   TUASSERTE(size_t, 751, uut2.size());
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
gapTest()
{
   TUDEF("SP3NavDataFactory", "setPosGapInterval");
   TestClass uut;
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_sp3_nav_2015_200.sp3";
   TUASSERT(uut.addDataSource(fname));
   TUASSERTE(size_t, 17856, uut.size());
   gnsstk::NavDataPtr navOut;
   gnsstk::NavSatelliteID satID(15, 15, gnsstk::SatelliteSystem::GPS,
                                gnsstk::CarrierBand::L1,
                                gnsstk::TrackingCode::CA,
                                gnsstk::NavType::GPSLNAV);
   gnsstk::NavMessageID nmid(satID, gnsstk::NavMessageType::Ephemeris);
   gnsstk::CommonTime when = gnsstk::GPSWeekSecond(1854,3599.926);
   TUASSERT(uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                     gnsstk::NavValidityType::ValidOnly,
                     gnsstk::NavSearchOrder::User));
      // set a tiny position gap interval and expect failure
   TUCATCH(uut.setPosGapInterval(1));
   TUASSERT(!uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
      // set a reasonable gap interval and expect success
   TUCATCH(uut.setPosGapInterval(301));
   TUASSERT(uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                     gnsstk::NavValidityType::ValidOnly,
                     gnsstk::NavSearchOrder::User));
      // set a slightly unreasonable gap interval and expect failure
   TUCATCH(uut.setPosGapInterval(299));
   TUASSERT(!uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
      // disable and verify
   TUCATCH(uut.disablePosDataGapCheck());
   TUASSERT(uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                     gnsstk::NavValidityType::ValidOnly,
                     gnsstk::NavSearchOrder::User));
      // set a tiny clock gap interval and expect failure
   TUCATCH(uut.setClkGapInterval(1));
   TUASSERT(!uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
      // disable and verify
   TUCATCH(uut.disableClkDataGapCheck());
   TUASSERT(uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                     gnsstk::NavValidityType::ValidOnly,
                     gnsstk::NavSearchOrder::User));
      // set a tiny max interval and expect failure
   TUCATCH(uut.setPosMaxInterval(1));
   TUASSERT(!uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
      // set a reasonable max interval and expect success
   TUCATCH(uut.setPosMaxInterval(2701));
   TUASSERT(uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                     gnsstk::NavValidityType::ValidOnly,
                     gnsstk::NavSearchOrder::User));
      // set a slightly unreasonable max interval and expect failure
   TUCATCH(uut.setPosMaxInterval(2699));
   TUASSERT(!uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
      // disable and verify
   TUCATCH(uut.disablePosIntervalCheck());
   TUASSERT(uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                     gnsstk::NavValidityType::ValidOnly,
                     gnsstk::NavSearchOrder::User));
      // set a tiny clock max interval and expect failure
   TUCATCH(uut.setClockMaxInterval(1));
   TUASSERT(!uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
      // set a reasonable clock max interval and expect success
   TUCATCH(uut.setClockMaxInterval(2701));
   TUASSERT(uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                     gnsstk::NavValidityType::ValidOnly,
                     gnsstk::NavSearchOrder::User));
      // set a slightly unreasonable clock max interval and expect failure
   TUCATCH(uut.setClockMaxInterval(2699));
   TUASSERT(!uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                      gnsstk::NavValidityType::ValidOnly,
                      gnsstk::NavSearchOrder::User));
      // disable and verify
   TUCATCH(uut.disableClockIntervalCheck());
   TUASSERT(uut.find(nmid, when, navOut, gnsstk::SVHealth::Any,
                     gnsstk::NavValidityType::ValidOnly,
                     gnsstk::NavSearchOrder::User));
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
nomTimeStepTest()
{
   TUDEF("SP3NavDataFactory", "nomTimeStep");
   TestClass uut;
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_sp3_nav_2015_200.sp3";
   TUASSERT(uut.addDataSource(fname));
   TUASSERTE(size_t, 17856, uut.size());
   gnsstk::SatID satID(15, gnsstk::SatelliteSystem::GPS);
   TUASSERTE(double, 300.0, uut.getPositionTimeStep(satID));
   TUASSERTE(double, 300.0, uut.getClockTimeStep(satID));
   TURETURN();
}


int main()
{
   SP3NavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.loadIntoMapTest();
   errorTotal += testClass.findExactTest();
   errorTotal += testClass.findInterpTest();
   errorTotal += testClass.findEdgeTest();
   errorTotal += testClass.sp3cPVTest();
   errorTotal += testClass.sp3cPTest();
   errorTotal += testClass.loadIntoMapFGNSSTest(false, false, false, false);
   errorTotal += testClass.loadIntoMapFGNSSTest(false, true, false, false);
   errorTotal += testClass.loadIntoMapFGNSSTest(true, false, false, false);
   errorTotal += testClass.loadIntoMapFGNSSTest(true, true, false, false);

   errorTotal += testClass.loadIntoMapFGNSSTest(false, false, true, false);
   errorTotal += testClass.loadIntoMapFGNSSTest(false, true, true, false);
   errorTotal += testClass.loadIntoMapFGNSSTest(true, false, true, false);
   errorTotal += testClass.loadIntoMapFGNSSTest(true, true, true, false);

   errorTotal += testClass.loadIntoMapFGNSSTest(false, false, false, true);
   errorTotal += testClass.loadIntoMapFGNSSTest(false, true, false, true);
   errorTotal += testClass.loadIntoMapFGNSSTest(true, false, false, true);
   errorTotal += testClass.loadIntoMapFGNSSTest(true, true, false, true);
   errorTotal += testClass.addRinexClockTest();
   errorTotal += testClass.gapTest();
   errorTotal += testClass.nomTimeStepTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
