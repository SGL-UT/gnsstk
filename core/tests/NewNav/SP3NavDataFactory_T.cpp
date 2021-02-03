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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::GPSLNavEph::L2Codes e)
   {
      s << static_cast<long>(e);
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
}

   /** Implement a test class to expose protected members rather than
    * using friends. */
class TestClass : public gpstk::SP3NavDataFactory
{
public:
      /// Grant access to protected data.
   gpstk::NavMessageMap& getData()
   { return data; }
};

/// Automated tests for gpstk::SP3NavDataFactory
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
      /** Use dynamic_cast to verify that the contents of nmm are the
       * right class.
       * @param[in] testFramework The test framework created by TUDEF,
       *   used by TUASSERT macros in this function.
       * @param[in] nmm The data map to check. */
   template <class NavClass>
   void verifyDataType(gpstk::TestUtil& testFramework,
                       gpstk::NavMessageMap& nmm);
};


unsigned SP3NavDataFactory_T ::
constructorTest()
{
   TUDEF("SP3NavDataFactory", "SP3NavDataFactory");
   gpstk::SP3NavDataFactory fact;
      // check for expected signal support
   gpstk::NavSignalID nsid1(gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::CA,
                            gpstk::NavType::GPSLNAV);
   TUASSERT(fact.supportedSignals.count(nsid1));
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
loadIntoMapTest()
{
   TUDEF("SP3NavDataFactory", "loadIntoMap");

      // test loading SP3a nav
   gpstk::SP3NavDataFactory f2;
   std::string f2name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3a.sp3";
      // this should implicitly load into the data map
   TUASSERT(f2.addDataSource(f2name));
   TUASSERTE(size_t, 232, f2.size());

      // test loading SP3b nav
   gpstk::SP3NavDataFactory f3;
   std::string f3name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3b.sp3";
      // this should implicitly load into the data map
   TUASSERT(f3.addDataSource(f3name));
   TUASSERTE(size_t, 232, f3.size());

   TestClass f4;
   std::string f4name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUCATCH(f4.setTypeFilter({gpstk::NavMessageType::Ephemeris}));
      // this should implicitly load into the data map
   TUASSERT(f4.addDataSource(f4name));
   TUASSERTE(size_t, 116, f4.size());
   gpstk::NavMessageMap &nmm4(f4.getData());
      // only one message type
   TUASSERTE(size_t, 1, nmm4.size());
      // and it's ephemeris.
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             nmm4.begin()->first);
   TUCSM("convertToOrbit");
   verifyDataType<gpstk::OrbitDataSP3>(testFramework, nmm4);
   TUCSM("loadIntoMap");

   TestClass f5;
   std::string f5name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUCATCH(f5.setTypeFilter({gpstk::NavMessageType::Health}));
      // this should implicitly load into the data map
   TUASSERT(f5.addDataSource(f5name));
   TUASSERTE(size_t, 0, f5.size());
   TUCSM("loadIntoMap");

   TestClass f6;
   std::string f6name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUCATCH(f6.setTypeFilter({gpstk::NavMessageType::Almanac}));
      // this should implicitly load into the data map
   TUASSERT(f6.addDataSource(f6name));
      // except there isn't any data, because SP3 NAV doesn't contain almanacs
   TUASSERTE(size_t, 0, f6.size());

      // test loading something that isn't SP3
   TestClass f7;
   std::string f7name = gpstk::getPathData() + gpstk::getFileSep() +
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
   gpstk::OrbitDataSP3 *uut;
   gpstk::SP3NavDataFactory fact;
   gpstk::NavSatelliteID satID1(7, 7, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::SatID expSat1(7, gpstk::SatelliteSystem::GPS);
   gpstk::NavMessageID nmid1(satID1, gpstk::NavMessageType::Ephemeris);
   fname = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUASSERT(fact.addDataSource(fname));
      // 116 ephemeris + 116 clock
   TUASSERTE(size_t, 232, fact.size());

      // test find with exact epoch at start of file
   gpstk::CivilTime civ1(2001, 7, 22, 0, 0, 0, gpstk::TimeSystem::GPS);
   gpstk::CommonTime ct1(civ1);
   gpstk::NavDataPtr nd1;
   TUASSERT(fact.find(nmid1, ct1, nd1, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd1.get());
      // NavData
   TUASSERTE(gpstk::CommonTime, ct1, uut->timeStamp);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gpstk::SatID, expSat1, uut->signal.sat);
   TUASSERTE(gpstk::SatID, expSat1, uut->signal.xmitSat);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
             uut->signal.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1, uut->signal.obs.band);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::CA, uut->signal.obs.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, uut->signal.nav);
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

      // test find with exact epoch at end of file
   gpstk::CivilTime civ2(2001, 7, 22, 0, 45, 0, gpstk::TimeSystem::GPS);
   gpstk::CommonTime ct2(civ2);
   gpstk::NavDataPtr nd2;
   gpstk::NavSatelliteID satID2(6, 6, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::SatID expSat2(6, gpstk::SatelliteSystem::GPS);
   gpstk::NavMessageID nmid2(satID2, gpstk::NavMessageType::Ephemeris);
   TUASSERT(fact.find(nmid2, ct2, nd2, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd2.get());
      // NavData
   TUASSERTE(gpstk::CommonTime, ct2, uut->timeStamp);
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

      // test find with exact epoch in the middle of file
   gpstk::CivilTime civ3(2001, 7, 22, 0, 30, 0, gpstk::TimeSystem::GPS);
   gpstk::CommonTime ct3(civ3);
   gpstk::NavDataPtr nd3;
   gpstk::NavSatelliteID satID3(11, 11, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::SatID expSat3(11, gpstk::SatelliteSystem::GPS);
   gpstk::NavMessageID nmid3(satID3, gpstk::NavMessageType::Ephemeris);
   TUASSERT(fact.find(nmid3, ct3, nd3, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd3.get());
      // NavData
   TUASSERTE(gpstk::CommonTime, ct3, uut->timeStamp);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gpstk::SatID, expSat3, uut->signal.sat);
   TUASSERTE(gpstk::SatID, expSat3, uut->signal.xmitSat);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
             uut->signal.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1, uut->signal.obs.band);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::CA, uut->signal.obs.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, uut->signal.nav);
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

      // Test using data where velocity is missing and must be interpolated.
      // This test makes sure we're getting velocity data when there
      // isn't any in the original "exact" record.
   gpstk::SP3NavDataFactory fact4;
   fname = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_sp3_nav_ephemerisData.sp3";
   TUASSERT(fact4.addDataSource(fname));
   TUASSERTE(size_t, 4800, fact4.size());
   gpstk::CommonTime ct4 = gpstk::CivilTime(1997, 4, 6, 1, 15, 0,
                                            gpstk::TimeSystem::GPS);
   gpstk::NavDataPtr nd4;
   gpstk::NavSatelliteID satID4(9, 9, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::SatID expSat4(9, gpstk::SatelliteSystem::GPS);
   gpstk::NavMessageID nmid4(satID4, gpstk::NavMessageType::Ephemeris);
   TUASSERT(fact4.find(nmid4, ct4, nd4, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::ValidOnly,
                       gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd4.get());
      // NavData
   TUASSERTE(gpstk::CommonTime, ct4, uut->timeStamp);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gpstk::SatID, expSat4, uut->signal.sat);
   TUASSERTE(gpstk::SatID, expSat4, uut->signal.xmitSat);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
             uut->signal.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1, uut->signal.obs.band);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::CA, uut->signal.obs.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, uut->signal.nav);
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
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
findInterpTest()
{
   TUDEF("SP3NavDataFactory", "find");
   gpstk::SP3NavDataFactory fact;
   gpstk::NavSatelliteID satID1(15, 15, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::SatID expSat1(15, gpstk::SatelliteSystem::GPS);
   gpstk::NavMessageID nmid1(satID1, gpstk::NavMessageType::Ephemeris);
   gpstk::CommonTime ct;
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_sp3_nav_ephemerisData.sp3";
   TUASSERT(fact.addDataSource(fname));
      // 2400 ephemeris + 2400 clock
   TUASSERTE(size_t, 4800, fact.size());
   ct = gpstk::CivilTime(1997,4,6,0,45,2,gpstk::TimeSystem::GPS);
   gpstk::NavDataPtr nd1;
   TUASSERT(!fact.find(nmid1, ct, nd1, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::ValidOnly,
                       gpstk::NavSearchOrder::User));

   ct = gpstk::CivilTime(1997,4,6,6,17,36,gpstk::TimeSystem::GPS);
   gpstk::NavDataPtr nd2;
   TUASSERT(fact.find(nmid1, ct, nd2, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   gpstk::OrbitDataSP3 *uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd2.get());
   if (uut == nullptr)
      TURETURN();
      // NavData
   TUASSERTE(gpstk::CommonTime, ct, uut->timeStamp);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gpstk::SatID, expSat1, uut->signal.sat);
   TUASSERTE(gpstk::SatID, expSat1, uut->signal.xmitSat);
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


unsigned SP3NavDataFactory_T ::
findEdgeTest()
{
   TUDEF("SP3NavDataFactory", "find");
   gpstk::SP3NavDataFactory fact;
   gpstk::NavSatelliteID satID1(1, 1, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::SatID expSat1(1, gpstk::SatelliteSystem::GPS);
   gpstk::NavMessageID nmid1(satID1, gpstk::NavMessageType::Ephemeris);
   gpstk::CommonTime ct;
   gpstk::NavDataPtr nd;
   gpstk::OrbitDataSP3 *uut = nullptr;
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_sp3_nav_ephemerisData.sp3";
   TUASSERT(fact.addDataSource(fname));
   TUASSERTE(size_t, 4800, fact.size());

      // test really early time
   ct = gpstk::CivilTime(1996,4,6,0,0,0,gpstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::ValidOnly,
                       gpstk::NavSearchOrder::User));
      // test just before data start
   ct = gpstk::CivilTime(1997,4,5,23,59,59,gpstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::ValidOnly,
                       gpstk::NavSearchOrder::User));
      // exactly at start
      // SP3EphemerisStore rejects this case
   ct = gpstk::CivilTime(1997,4,6,0,0,0,gpstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd.get());
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
      // Just after start - can't interpolate
   ct = gpstk::CivilTime(1997,4,6,0,0,2,gpstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::ValidOnly,
                       gpstk::NavSearchOrder::User));
      // exact match but too early to interpolate
      // SP3EphemerisStore rejects this case
   ct = gpstk::CivilTime(1997,4,6,0,15,0,gpstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd.get());
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
      // Still not enough data to interpolate
   ct = gpstk::CivilTime(1997,4,6,0,15,2,gpstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::ValidOnly,
                       gpstk::NavSearchOrder::User));
      // exact match but too early to interpolate
      // SP3EphemerisStore rejects this case
   ct = gpstk::CivilTime(1997,4,6,0,30,0,gpstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd.get());
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
      // Still not enough data to interpolate
   ct = gpstk::CivilTime(1997,4,6,0,30,2,gpstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::ValidOnly,
                       gpstk::NavSearchOrder::User));
      // Still not enough data to interpolate
   ct = gpstk::CivilTime(1997,4,6,0,45,2,gpstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::ValidOnly,
                       gpstk::NavSearchOrder::User));
      // exact match but too early to interpolate
      // SP3EphemerisStore rejects this case
   ct = gpstk::CivilTime(1997,4,6,1,0,0,gpstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd.get());
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
      // now we can interpolate
   ct = gpstk::CivilTime(1997,4,6,1,0,2,gpstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd.get());
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
      // interpolating near the end of the data set
   ct = gpstk::CivilTime(1997,4,6,22,30,2,gpstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd.get());
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
      // The last time we can interpolate
   ct = gpstk::CivilTime(1997,4,6,22,45,0,gpstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd.get());
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
      // no longer able to interpolate
   ct = gpstk::CivilTime(1997,4,6,22,45,2,gpstk::TimeSystem::GPS);
   TUASSERT(!fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::ValidOnly,
                       gpstk::NavSearchOrder::User));
      // Exact match but too late to interpolate.  The last possible
      // time we can get from the source data.
      // SP3EphemerisStore rejects this case
   ct = gpstk::CivilTime(1997,4,6,23,45,0,gpstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd.get());
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
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
sp3cPVTest()
{
   TUDEF("SP3NavDataFactory", "addDataSource");
   gpstk::SP3NavDataFactory fact;
   gpstk::NavSatelliteID satID1(15, 15, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::SatID expSat1(15, gpstk::SatelliteSystem::GPS);
   gpstk::NavMessageID nmid1(satID1, gpstk::NavMessageType::Ephemeris);
   gpstk::CommonTime ct;
   gpstk::NavDataPtr nd;
   gpstk::OrbitDataSP3 *uut = nullptr;
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3c.sp3";
   TUASSERT(fact.addDataSource(fname));
   TUASSERTE(size_t, 1500, fact.size());
      // exact match
   ct = gpstk::CivilTime(2011,10,9,3,0,0,gpstk::TimeSystem::GPS);
   TUCSM("find");
   TUASSERT(fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd.get());
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
   TUASSERTFE(7.2199999999999998003e-08, uut->driftSig);
      /// @note drift rate value not confirmed by SP3EphemerisStore
   TUASSERTFE(-2.1102292768909279853e-13, uut->clkDrRate);
   TUASSERTFE(8.0222222222222225231e-11, uut->drRateSig);
      // interpolated match
   ct = gpstk::CivilTime(2011,10,9,2,1,3,gpstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd.get());
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
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
sp3cPTest()
{
   TUDEF("SP3NavDataFactory", "addDataSource");
   gpstk::SP3NavDataFactory fact;
   gpstk::NavSatelliteID satID1(15, 15, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::SatID expSat1(15, gpstk::SatelliteSystem::GPS);
   gpstk::NavMessageID nmid1(satID1, gpstk::NavMessageType::Ephemeris);
   gpstk::CommonTime ct;
   gpstk::NavDataPtr nd;
   gpstk::OrbitDataSP3 *uut = nullptr;
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3c_pos.sp3";
   TUASSERT(fact.addDataSource(fname));
   TUASSERTE(size_t, 1500, fact.size());
      // exact match
   ct = gpstk::CivilTime(2011,10,9,3,0,0,gpstk::TimeSystem::GPS);
   TUCSM("find");
   TUASSERT(fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd.get());
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
      // interpolated match
   ct = gpstk::CivilTime(2011,10,9,2,1,3,gpstk::TimeSystem::GPS);
   TUASSERT(fact.find(nmid1, ct, nd, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd.get());
   
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
   TURETURN();
}


template <class NavClass>
void SP3NavDataFactory_T ::
verifyDataType(gpstk::TestUtil& testFramework,
               gpstk::NavMessageMap& nmm)
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

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
