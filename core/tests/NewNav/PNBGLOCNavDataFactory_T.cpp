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
#include "FactoryCounter.hpp"
#include "PNBGLOCNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GLOCNavUT1TimeOffset.hpp"
#include "GLOCNavHealth.hpp"
#include "GLOCNavEph.hpp"
#include "GLOCNavAlm.hpp"
#include "GLOCNavIono.hpp"
#include "GLOCBits.hpp"
#include "CivilTime.hpp"
#include "YDSTime.hpp"
#include "GLONASSTime.hpp"
#include "TimeString.hpp"

using namespace std;

/// GLONASS doesn't transmit ISC data, so make up a class for FactoryCounter
class BunkISC
{
public:
   BunkISC()
   {}
};

/// expose protected data for test
class TestClass : public gnsstk::PNBGLOCNavDataFactory
{
public:
   const std::map<gnsstk::NavSatelliteID, gnsstk::GLOCNavLTDMP>& getLTDMP()
      const
   { return ltdmpAcc; }
};

namespace gnsstk
{
   ostream& operator<<(ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
   ostream& operator<<(ostream& s, GLOCSatType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   ostream& operator<<(ostream& s, GLOCRegime e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   ostream& operator<<(ostream& s, GLOCOrbitType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

// avoid having to type out template params over and over.
using GLOCactoryCounter = FactoryCounter<gnsstk::GLOCNavAlm,gnsstk::GLOCNavEph,gnsstk::GLOCNavUT1TimeOffset,gnsstk::GLOCNavHealth,gnsstk::GLOCNavIono,BunkISC>;


/** Test PNBGLOCNavDataFactory.
 * @note The truth values haven't been fully vetted, beyond basic sanity check.
 */
class PNBGLOCNavDataFactory_T
{
public:
   PNBGLOCNavDataFactory_T();

   unsigned addDataAllTest();
      /// Test addData with ephemeris selected only
   unsigned addDataEphemerisTest();
      /// Test addData with almanac selected only
   unsigned addDataAlmanacTest();
      /// Test addData with health data selected only
   unsigned addDataHealthTest();
      /// Test addData with time offset data selected only
   unsigned addDataTimeTest();
      /// One additional combo test.
   unsigned addDataEphHealthTest();
      /// Another combo test that makes sure alm health is processed correctly.
   unsigned addDataAlmHealthTest();
      /// Test processEph method
   unsigned processEphTest();
      /// Test processAlm method
   unsigned processAlmTest();
      /// Test processEarth method
   unsigned processEarthTest();
      /// Test processLTDMP method
   unsigned processLTDMPTest();
      /// Test processHealth method
   unsigned processHealthTest();

#include "GLOCNavTestDataDecl.hpp"
};


PNBGLOCNavDataFactory_T ::
PNBGLOCNavDataFactory_T()
{
#include "GLOCNavTestDataDef.hpp"
}


unsigned PNBGLOCNavDataFactory_T ::
addDataAllTest()
{
   TUDEF("PNBGLOCNavDataFactory", "addData");
   GLOCactoryCounter fc(testFramework);
   gnsstk::PNBGLOCNavDataFactory uut;
   gnsstk::SatID gpsSid(1,gnsstk::SatelliteSystem::GPS);
   gnsstk::ObsID gpsid(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::L1,
                       gnsstk::TrackingCode::CA);
   gnsstk::NavID gpsNav(gnsstk::NavType::GPSLNAV);
   gnsstk::PackedNavBitsPtr nonGLO =
      std::make_shared<gnsstk::PackedNavBits>(gpsSid,gpsid,gpsNav,"XX",
                                              navGLOCStr10act);
   gnsstk::NavDataPtrList navOut;
      // should refuse non-GLONASS data
   TUASSERTE(bool, false, uut.addData(nonGLO, navOut));
   fc.validateResults(navOut, __LINE__);
      // The rest is checking that we're processing the good data
      // appropriately, but I'm not bothering to check the detailed
      // contents as that is assumed to be tested in the process*
      // tests.
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navGLOCStr10a, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11a, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12a, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr20, navOut));
   fc.validateResults(navOut, __LINE__, 2, 1, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr31, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr32, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr10b, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11b, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12b, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr25, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 1, 1, 1);
   TURETURN();
}


unsigned PNBGLOCNavDataFactory_T ::
addDataEphemerisTest()
{
   TUDEF("PNBGLOCNavDataFactory", "addData");
   GLOCactoryCounter fc(testFramework);
   gnsstk::PNBGLOCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navGLOCStr10a, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11a, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12a, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr20, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr31, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr32, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr10b, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11b, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12b, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr25, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGLOCNavDataFactory_T ::
addDataAlmanacTest()
{
   TUDEF("PNBGLOCNavDataFactory", "addData");
   GLOCactoryCounter fc(testFramework);
   gnsstk::PNBGLOCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Almanac}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navGLOCStr10a, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11a, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12a, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr20, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr31, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr32, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr10b, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11b, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12b, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr25, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGLOCNavDataFactory_T ::
addDataHealthTest()
{
   TUDEF("PNBGLOCNavDataFactory", "addData");
   GLOCactoryCounter fc(testFramework);
   gnsstk::PNBGLOCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navGLOCStr10a, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11a, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12a, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr20, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr31, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr32, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr10b, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11b, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12b, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr25, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TURETURN();
}


unsigned PNBGLOCNavDataFactory_T ::
addDataTimeTest()
{
   TUDEF("PNBGLOCNavDataFactory", "addData");
   GLOCactoryCounter fc(testFramework);
   gnsstk::PNBGLOCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::TimeOffset}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navGLOCStr10a, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11a, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12a, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr20, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr31, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr32, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr10b, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11b, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12b, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navGLOCStr25, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
   TURETURN();
}


unsigned PNBGLOCNavDataFactory_T ::
addDataEphHealthTest()
{
   TUDEF("PNBGLOCNavDataFactory", "addData");
   GLOCactoryCounter fc(testFramework);
   gnsstk::PNBGLOCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health,
                              gnsstk::NavMessageType::Ephemeris}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navGLOCStr10a, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11a, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12a, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr20, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr31, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr32, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr10b, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11b, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12b, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr25, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TURETURN();
}


unsigned PNBGLOCNavDataFactory_T ::
addDataAlmHealthTest()
{
   TUDEF("PNBGLOCNavDataFactory", "addData");
   GLOCactoryCounter fc(testFramework);
   gnsstk::PNBGLOCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health,
                              gnsstk::NavMessageType::Almanac}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navGLOCStr10a, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11a, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12a, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr20, navOut));
   fc.validateResults(navOut, __LINE__, 2, 1, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr31, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr32, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr10b, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr11b, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr12b, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navGLOCStr25, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TURETURN();
}


unsigned PNBGLOCNavDataFactory_T ::
processEphTest()
{
   TUDEF("PNBGLOCNavDataFactory", "processEph");
   GLOCactoryCounter fc(testFramework);
   gnsstk::PNBGLOCNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(11, 11, gnsstk::SatelliteSystem::Glonass,
                             gnsstk::CarrierBand::G3,
                             gnsstk::TrackingCode::L3OCD,
                             gnsstk::NavType::GloCivilC),
      gnsstk::NavMessageType::Ephemeris);
   gnsstk::CommonTime toeExp = gnsstk::CivilTime(2022,5,3,10,15,0,
                                                 gnsstk::TimeSystem::GLO);
   gnsstk::CommonTime beginExp = navGLOCStr10act;
   gnsstk::CommonTime endExp = toeExp + 900.0;
   gnsstk::NavDataPtrList navOut;
   gnsstk::GLOCNavEph *eph;
   gnsstk::GLOCNavHealth *hea;
   TUASSERTE(bool, true, uut.processEph(10, navGLOCStr10a, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(11, navGLOCStr11a, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(12, navGLOCStr12a, navOut));
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gnsstk::GLOCNavEph*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navGLOCStr10act, eph->timeStamp);
         nmidExp.messageType = gnsstk::NavMessageType::Ephemeris;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, eph->signal);
            // OrbitData has no data of its own
            // GLOCNavData fields
         TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
            // GLOCNavHeader fields
         TUASSERTE(gnsstk::CommonTime, navGLOCStr10act, eph->header.xmit);
         TUASSERTE(uint32_t, gnsstk::gloc::valPreamble, eph->header.preamble);
         TUASSERTE(uint16_t, 15795, eph->header.TS);
         TUASSERTE(unsigned, 11, eph->header.svid);
         TUASSERTE(bool, true, eph->header.svUnhealthy);
         TUASSERTE(bool, false, eph->header.dataInvalid);
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy,
                   eph->header.health);
         TUASSERTE(unsigned, 0, eph->header.P1);
         TUASSERTE(bool, false, eph->header.P2);
         TUASSERTE(unsigned, 0, eph->header.KP);
         TUASSERTE(bool, false, eph->header.A);
         TUASSERTE(gnsstk::CommonTime, navGLOCStr11act, eph->header11.xmit);
         TUASSERTE(uint32_t, gnsstk::gloc::valPreamble, eph->header11.preamble);
         TUASSERTE(uint16_t, 15796, eph->header11.TS);
         TUASSERTE(unsigned, 11, eph->header11.svid);
         TUASSERTE(bool, true, eph->header11.svUnhealthy);
         TUASSERTE(bool, false, eph->header11.dataInvalid);
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy,
                   eph->header11.health);
         TUASSERTE(unsigned, 0, eph->header11.P1);
         TUASSERTE(bool, false, eph->header11.P2);
         TUASSERTE(unsigned, 0, eph->header11.KP);
         TUASSERTE(bool, false, eph->header11.A);
         TUASSERTE(gnsstk::CommonTime, navGLOCStr12act, eph->header12.xmit);
         TUASSERTE(uint32_t, gnsstk::gloc::valPreamble, eph->header12.preamble);
         TUASSERTE(uint16_t, 15797, eph->header12.TS);
         TUASSERTE(unsigned, 11, eph->header12.svid);
         TUASSERTE(bool, true, eph->header12.svUnhealthy);
         TUASSERTE(bool, false, eph->header12.dataInvalid);
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy,
                   eph->header12.health);
         TUASSERTE(unsigned, 0, eph->header12.P1);
         TUASSERTE(bool, false, eph->header12.P2);
         TUASSERTE(unsigned, 0, eph->header12.KP);
         TUASSERTE(bool, false, eph->header12.A);
            // GLOCNavEph fields
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(unsigned, 7, eph->N4);
         TUASSERTE(uint16_t, 854, eph->NT);
         TUASSERTE(gnsstk::GLOCSatType, gnsstk::GLOCSatType::GLONASS_K1,
                   eph->Mj);
         TUASSERTE(unsigned, 5, eph->PS);
         TUASSERTE(uint16_t, 47700, eph->tb);
         TUASSERTE(unsigned, 0, eph->EjE);
         TUASSERTE(unsigned, 0, eph->EjT);
         TUASSERTE(gnsstk::GLOCRegime, gnsstk::GLOCRegime::Prediction,
                   eph->RjE);
         TUASSERTE(gnsstk::GLOCRegime, gnsstk::GLOCRegime::Prediction,
                   eph->RjT);
         TUASSERTE(int, 3, eph->FjE);
         TUASSERTE(int, 0, eph->FjT);
         TUASSERTFE(-3.9905342418933287263e-05, eph->clkBias);
         TUASSERTFE(-9.201528428093297407e-13, eph->freqBias);
         TUASSERTFE(0.0, eph->driftRate);
         TUASSERTFE(1.3969838619232177734e-09, eph->tauc);
         TUASSERTFE(0.0, eph->taucdot);
         TUASSERTFE(-4131.7772378921508789, eph->pos[0]);
         TUASSERTFE(11290.402944564819336, eph->pos[1]);
         TUASSERTFE(-22486.111017227172852, eph->pos[2]);
         TUASSERTFE(-2.0378112019971013069, eph->vel[0]);
         TUASSERTFE(-2.3122485717758536339, eph->vel[1]);
         TUASSERTFE(-0.78744945116341114044, eph->vel[2]);
         TUASSERTFE(-4.9294612836092710495e-10, eph->acc[0]);
         TUASSERTFE(-2.1263986127451062202e-09, eph->acc[1]);
         TUASSERTFE(2.0027073333039879799e-09, eph->acc[2]);
         TUASSERTFE(-1.427734375, eph->apcOffset[0]);
         TUASSERTFE(0.005859375, eph->apcOffset[1]);
         TUASSERTFE(0.0, eph->apcOffset[2]);
         TUASSERTFE(1.0186340659856796265e-10, eph->tauDelta);
         TUASSERTFE(4.889443516731262207e-08, eph->tauGPS);
            /// @todo check the LTDMP
         TUASSERTFE(60.0, eph->step);
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
   TURETURN();
}


unsigned PNBGLOCNavDataFactory_T ::
processAlmTest()
{
   TUDEF("PNBGLOCNavDataFactory", "processAlm");
   GLOCactoryCounter fc(testFramework);
   gnsstk::PNBGLOCNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(21, 11, gnsstk::SatelliteSystem::Glonass,
                             gnsstk::CarrierBand::G3,
                             gnsstk::TrackingCode::L3OCD,
                             gnsstk::NavType::GloCivilC),
      gnsstk::NavMessageType::Almanac);
   gnsstk::CommonTime toaExp = gnsstk::GLONASSTime(7, 853, 33143);
   gnsstk::CommonTime beginExp = navGLOCStr20ct;
   gnsstk::CommonTime endExp = gnsstk::CommonTime::END_OF_TIME;
   gnsstk::NavDataPtrList navOut;
   gnsstk::GLOCNavAlm *alm;
   endExp.setTimeSystem(gnsstk::TimeSystem::GLO);
   TUASSERTE(bool, true, uut.processAlm(navGLOCStr20, navOut));
   for (const auto& i : navOut)
   {
      if ((alm = dynamic_cast<gnsstk::GLOCNavAlm*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navGLOCStr20ct, alm->timeStamp);
         nmidExp.messageType = gnsstk::NavMessageType::Almanac;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
            // OrbitData has no data of its own
            // GLOCNavData fields
         TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
            // GLOCNavHeader fields
         TUASSERTE(gnsstk::CommonTime, navGLOCStr20ct, alm->header.xmit);
         TUASSERTE(uint32_t, gnsstk::gloc::valPreamble, alm->header.preamble);
         TUASSERTE(uint16_t, 15798, alm->header.TS);
         TUASSERTE(unsigned, 21, alm->header.svid);
         TUASSERTE(bool, true, alm->header.svUnhealthy);
         TUASSERTE(bool, false, alm->header.dataInvalid);
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy,
                   alm->header.health);
         TUASSERTE(unsigned, 0, alm->header.P1);
         TUASSERTE(bool, false, alm->header.P2);
         TUASSERTE(unsigned, 0, alm->header.KP);
         TUASSERTE(bool, false, alm->header.A);
            // GLOCNavAlm
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toa);
         TUASSERTE(gnsstk::GLOCOrbitType, gnsstk::GLOCOrbitType::Circ19100,
                   alm->orbitType);
         TUASSERTE(unsigned, 2, alm->numSVs);
         TUASSERTE(unsigned, 4, alm->aoa);
         TUASSERTE(unsigned, 1, alm->statusReg);
         TUASSERTE(gnsstk::GLOCSatType, gnsstk::GLOCSatType::GLONASS_M,
                   alm->satType);
         TUASSERTFE(0.00029850006103515625, alm->tau);
         TUASSERTFE(0.285129547119140625, alm->lambda);
         TUASSERTFE(33143, alm->tLambda);
         TUASSERTFE(0.0054931640625, alm->deltai);
         TUASSERTFE(0.000415802001953125, alm->ecc);
         TUASSERTFE(0.85430908203125, alm->omega);
         TUASSERTFE(0.263671875, alm->deltaT);
         TUASSERTFE(0.0, alm->deltaTdot);
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 1);
   TURETURN();
}


unsigned PNBGLOCNavDataFactory_T ::
processEarthTest()
{
   TUDEF("PNBGLOCNavDataFactory", "processEarth");
   GLOCactoryCounter fc(testFramework);
   gnsstk::PNBGLOCNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(11, 11, gnsstk::SatelliteSystem::Glonass,
                             gnsstk::CarrierBand::G3,
                             gnsstk::TrackingCode::L3OCD,
                             gnsstk::NavType::GloCivilC),
      gnsstk::NavMessageType::Iono);
   gnsstk::CommonTime refExp = gnsstk::GLONASSTime(7, 853, 0);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GLOCNavIono *iono;
   gnsstk::GLOCNavUT1TimeOffset *tim;
      // make sure timeAcc is set
   TUASSERTE(bool, true, uut.processEph(10, navGLOCStr10b, navOut));
   TUASSERTE(bool, true, uut.processEarth(navGLOCStr25, navOut));
   for (const auto& i : navOut)
   {
      if ((iono = dynamic_cast<gnsstk::GLOCNavIono*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navGLOCStr25ct, iono->timeStamp);
         nmidExp.messageType = gnsstk::NavMessageType::Iono;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, iono->signal);
            // IonoNavData has no fields of its own
            // GLOCNavIono fields
         TUASSERTFE(1, iono->peakTECF2);
         TUASSERTFE(122, iono->solarIndex);
         TUASSERTFE(24, iono->geoIndex);
      }
      else if ((tim = dynamic_cast<gnsstk::GLOCNavUT1TimeOffset*>(i.get()))
               != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navGLOCStr25ct, tim->timeStamp);
         nmidExp.messageType = gnsstk::NavMessageType::TimeOffset;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, tim->signal);
            // TimeOffsetData has no fields of its own
            // GLOCNavUT1TimeOffset
         TUASSERTFE(-0.096923828125, tim->B0);
         TUASSERTFE(0.0005950927734375, tim->B1);
         TUASSERTFE(0, tim->B2);
         TUASSERTE(gnsstk::CommonTime, refExp, tim->refTime);
         TUASSERTE(unsigned, 853, tim->NB);
         TUASSERTFE(-37, tim->UTCTAI);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 1, 0, 1);
   TURETURN();
}


unsigned PNBGLOCNavDataFactory_T ::
processLTDMPTest()
{
   TUDEF("PNBGLOCNavDataFactory", "processLTDMP");
      // LTDMP is metadata, so it's processed a bit differently
   TestClass uut;
   const std::map<gnsstk::NavSatelliteID, gnsstk::GLOCNavLTDMP>&
      mapRef(uut.getLTDMP());
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(11, 11, gnsstk::SatelliteSystem::Glonass,
                             gnsstk::CarrierBand::G3,
                             gnsstk::TrackingCode::L3OCD,
                             gnsstk::NavType::GloCivilC),
      gnsstk::NavMessageType::Iono);
   gnsstk::CommonTime refExp = gnsstk::GLONASSTime(7, 853, 0);
   TUASSERTE(bool, true, uut.processLTDMP(31, navGLOCStr31));
   TUASSERTE(unsigned, 1, mapRef.size());
   const auto& it1 = mapRef.find(nmidExp);
   TUASSERT(it1 != mapRef.end());
   if (it1 != mapRef.end())
   {
         // GLOCNavHeader fields
      TUASSERTE(gnsstk::CommonTime, navGLOCStr31ct, it1->second.header31.xmit);
      TUASSERTE(uint32_t, gnsstk::gloc::valPreamble,
                it1->second.header31.preamble);
      TUASSERTE(uint16_t, 15803, it1->second.header31.TS);
      TUASSERTE(unsigned, 11, it1->second.header31.svid);
      TUASSERTE(bool, true, it1->second.header31.svUnhealthy);
      TUASSERTE(bool, false, it1->second.header31.dataInvalid);
         // We don't set the interpreted health here, but there's no
         // reason it needs to be left Unknown.
      TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unknown,
                it1->second.header31.health);
      TUASSERTE(unsigned, 0, it1->second.header31.P1);
      TUASSERTE(bool, false, it1->second.header31.P2);
      TUASSERTE(unsigned, 0, it1->second.header31.KP);
      TUASSERTE(bool, false, it1->second.header31.A);
         // GLOCNavLTDMP fields
      TUASSERTE(uint16_t, 47700, it1->second.tb31);
      TUASSERTE(uint16_t, 65535, it1->second.tb32);
      TUASSERTFE(0.0, it1->second.dax0);
      TUASSERTFE(0.0, it1->second.day0);
      TUASSERTFE(0.0, it1->second.daz0);
      TUASSERTFE(0.0, it1->second.ax1);
      TUASSERTFE(0.0, it1->second.ay1);
      TUASSERTFE(0.0, it1->second.az1);
      TUASSERTFE(0.0, it1->second.ax2);
      TUASSERTFE(0.0, it1->second.ay2);
      TUASSERTFE(0.0, it1->second.az2);
      TUASSERTE(int, 1, std::isnan(it1->second.ax3));
      TUASSERTE(int, 1, std::isnan(it1->second.ay3));
      TUASSERTE(int, 1, std::isnan(it1->second.az3));
      TUASSERTE(int, 1, std::isnan(it1->second.ax4));
      TUASSERTE(int, 1, std::isnan(it1->second.ay4));
      TUASSERTE(int, 1, std::isnan(it1->second.az4));
   }
   TUASSERTE(bool, true, uut.processLTDMP(32, navGLOCStr32));
   TUASSERTE(unsigned, 1, mapRef.size());
   const auto& it2 = mapRef.find(nmidExp);
   TUASSERT(it2 != mapRef.end());
   if (it2 != mapRef.end())
   {
         // GLOCNavHeader fields
      TUASSERTE(gnsstk::CommonTime, navGLOCStr32ct, it2->second.header32.xmit);
      TUASSERTE(uint32_t, gnsstk::gloc::valPreamble,
                it2->second.header32.preamble);
      TUASSERTE(uint16_t, 15804, it2->second.header32.TS);
      TUASSERTE(unsigned, 11, it2->second.header32.svid);
      TUASSERTE(bool, true, it2->second.header32.svUnhealthy);
      TUASSERTE(bool, false, it2->second.header32.dataInvalid);
         // We don't set the interpreted health here, but there's no
         // reason it needs to be left Unknown.
      TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unknown,
                it2->second.header32.health);
      TUASSERTE(unsigned, 0, it2->second.header32.P1);
      TUASSERTE(bool, false, it2->second.header32.P2);
      TUASSERTE(unsigned, 0, it2->second.header32.KP);
      TUASSERTE(bool, false, it2->second.header32.A);
         // GLOCNavLTDMP fields
      TUASSERTE(uint16_t, 47700, it1->second.tb31);
      TUASSERTE(uint16_t, 47700, it1->second.tb32);
      TUASSERTFE(0.0, it1->second.dax0);
      TUASSERTFE(0.0, it1->second.day0);
      TUASSERTFE(0.0, it1->second.daz0);
      TUASSERTFE(0.0, it1->second.ax1);
      TUASSERTFE(0.0, it1->second.ay1);
      TUASSERTFE(0.0, it1->second.az1);
      TUASSERTFE(0.0, it1->second.ax2);
      TUASSERTFE(0.0, it1->second.ay2);
      TUASSERTFE(0.0, it1->second.az2);
      TUASSERTFE(0.0, it1->second.ax3);
      TUASSERTFE(0.0, it1->second.ay3);
      TUASSERTFE(0.0, it1->second.az3);
      TUASSERTFE(0.0, it1->second.ax4);
      TUASSERTFE(0.0, it1->second.ay4);
      TUASSERTFE(0.0, it1->second.az4);
   }
   TURETURN();
}


unsigned PNBGLOCNavDataFactory_T ::
processHealthTest()
{
   TUDEF("PNBGLOCNavDataFactory", "processHealth");
   GLOCactoryCounter fc(testFramework);
   gnsstk::PNBGLOCNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(21, 11, gnsstk::SatelliteSystem::Glonass,
                             gnsstk::CarrierBand::G3,
                             gnsstk::TrackingCode::L3OCD,
                             gnsstk::NavType::GloCivilC),
      gnsstk::NavMessageType::Health);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GLOCNavHealth *hea;
   TUASSERTE(bool, true, uut.processHealth(navGLOCStr20, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GLOCNavHealth*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navGLOCStr20ct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no data of its own
            // GLOCNavHealth
         TUASSERTE(bool, true, hea->Hj);
         TUASSERTE(bool, false, hea->lj);
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TURETURN();
}


int main()
{
   PNBGLOCNavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.addDataAllTest();
   errorTotal += testClass.addDataAlmanacTest();
   errorTotal += testClass.addDataEphemerisTest();
   errorTotal += testClass.addDataHealthTest();
   errorTotal += testClass.addDataTimeTest();
   errorTotal += testClass.addDataEphHealthTest();
   errorTotal += testClass.addDataAlmHealthTest();
   errorTotal += testClass.processEphTest();
   errorTotal += testClass.processAlmTest();
   errorTotal += testClass.processEarthTest();
   errorTotal += testClass.processLTDMPTest();
   errorTotal += testClass.processHealthTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
