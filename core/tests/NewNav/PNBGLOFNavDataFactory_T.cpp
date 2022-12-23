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
#include "PNBGLOFNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GLOFNavTimeOffset.hpp"
#include "GLOFNavUT1TimeOffset.hpp"
#include "GLOFNavHealth.hpp"
#include "GLOFNavEph.hpp"
#include "GLOFNavAlm.hpp"
#include "GLOFNavISC.hpp"
#include "CivilTime.hpp"
#include "YDSTime.hpp"
#include "TimeString.hpp"

using namespace std;

/// GLONASS doesn't transmit iono data, so make up a class for FactoryCounter
class BunkIono
{
public:
   BunkIono()
   {}
};

namespace gnsstk
{
   ostream& operator<<(ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
   ostream& operator<<(ostream& s, GLOFNavSatType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   ostream& operator<<(ostream& s, GLOFNavPCode e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

// avoid having to type out template params over and over.
using GLOFactoryCounter = FactoryCounter<gnsstk::GLOFNavAlm,gnsstk::GLOFNavEph,gnsstk::GLOFNavTimeOffset,gnsstk::GLOFNavHealth,BunkIono,gnsstk::GLOFNavISC>;


/** Test PNBGLOFNavDataFactory.
 * @note The truth values haven't been fully vetted, beyond basic sanity check.
 */
class PNBGLOFNavDataFactory_T
{
public:
   PNBGLOFNavDataFactory_T();

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
      /// Test processAlm and processTime method, and implicitly, processAlmOrb
   unsigned processAlmTest();
      /// Test processTimeUT1 method
   unsigned processTimeUT1Test();
      /// Test getFrameNum method
   unsigned getFrameNumTest();

#include "GLOFNavTestDataDecl.hpp"
};


PNBGLOFNavDataFactory_T ::
PNBGLOFNavDataFactory_T()
{
#include "GLOFNavTestDataDef.hpp"
}


unsigned PNBGLOFNavDataFactory_T ::
addDataAllTest()
{
   TUDEF("PNBGLOFNavDataFactory", "addData");
   GLOFactoryCounter fc(testFramework);
   gnsstk::PNBGLOFNavDataFactory uut;
   gnsstk::SatID gpsSid(1,gnsstk::SatelliteSystem::GPS);
   gnsstk::ObsID gpsid(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::L1,
                       gnsstk::TrackingCode::CA);
   gnsstk::NavID gpsNav(gnsstk::NavType::GPSLNAV);
   gnsstk::PackedNavBitsPtr nonGLO =
      std::make_shared<gnsstk::PackedNavBits>(gpsSid,gpsid,gpsNav,"XX",
                                              navFNAVGLOStr1ct);
   gnsstk::NavDataPtrList navOut;
      // should refuse non-GLONASS data
   TUASSERTE(bool, false, uut.addData(nonGLO, navOut));
   fc.validateResults(navOut, __LINE__);
      // The rest is checking that we're processing the good data
      // appropriately, but I'm not bothering to check the detailed
      // contents as that is assumed to be tested in the process*
      // tests.
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr4, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 1, 0, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr5, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 1, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr6, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr7, navOut));
   fc.validateResults(navOut, __LINE__, 2, 1, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr14, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 0, 0, 0, 1);
   TURETURN();
}


unsigned PNBGLOFNavDataFactory_T ::
addDataEphemerisTest()
{
   TUDEF("PNBGLOFNavDataFactory", "addData");
   GLOFactoryCounter fc(testFramework);
   gnsstk::PNBGLOFNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr4, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr5, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr6, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr7, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGLOFNavDataFactory_T ::
addDataAlmanacTest()
{
   TUDEF("PNBGLOFNavDataFactory", "addData");
   GLOFactoryCounter fc(testFramework);
   gnsstk::PNBGLOFNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Almanac}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr5, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr6, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr7, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TURETURN();
}


unsigned PNBGLOFNavDataFactory_T ::
addDataHealthTest()
{
   TUDEF("PNBGLOFNavDataFactory", "addData");
   GLOFactoryCounter fc(testFramework);
   gnsstk::PNBGLOFNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr4, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr5, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr6, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr7, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TURETURN();
}


unsigned PNBGLOFNavDataFactory_T ::
addDataTimeTest()
{
   TUDEF("PNBGLOFNavDataFactory", "addData");
   GLOFactoryCounter fc(testFramework);
   gnsstk::PNBGLOFNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::TimeOffset}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr5, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr6, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr7, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGLOFNavDataFactory_T ::
addDataEphHealthTest()
{
   TUDEF("PNBGLOFNavDataFactory", "addData");
   GLOFactoryCounter fc(testFramework);
   gnsstk::PNBGLOFNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health,
                              gnsstk::NavMessageType::Ephemeris}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr4, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr5, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr6, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr7, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TURETURN();
}


unsigned PNBGLOFNavDataFactory_T ::
addDataAlmHealthTest()
{
   TUDEF("PNBGLOFNavDataFactory", "addData");
   GLOFactoryCounter fc(testFramework);
   gnsstk::PNBGLOFNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health,
                              gnsstk::NavMessageType::Almanac}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr4, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr5, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr6, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGLOStr7, navOut));
   fc.validateResults(navOut, __LINE__, 2, 1, 0, 0, 1);
   TURETURN();
}


unsigned PNBGLOFNavDataFactory_T ::
processEphTest()
{
   TUDEF("PNBGLOFNavDataFactory", "processEph");
   GLOFactoryCounter fc(testFramework);
   gnsstk::PNBGLOFNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Glonass,
                             gnsstk::CarrierBand::G1,
                             gnsstk::TrackingCode::Standard,
                             gnsstk::NavType::GloCivilF),
      gnsstk::NavMessageType::Ephemeris);
   gnsstk::CommonTime toeExp = gnsstk::CivilTime(2015,6,27,0,45,0,
                                                 gnsstk::TimeSystem::GLO);
   gnsstk::CommonTime refExp = gnsstk::CivilTime(2015,6,27,3,40,0,
                                                 gnsstk::TimeSystem::GLO);
   gnsstk::CommonTime beginExp = navFNAVGLOStr1ct;
   gnsstk::CommonTime endExp = toeExp + 31.0*30.0;
   gnsstk::NavDataPtrList navOut;
   gnsstk::GLOFNavEph *eph;
   gnsstk::GLOFNavHealth *hea;
   gnsstk::GLOFNavISC *isc;
   TUASSERTE(bool, true, uut.processEph(1, navFNAVGLOStr1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(2, navFNAVGLOStr2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(3, navFNAVGLOStr3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(4, navFNAVGLOStr4, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GLOFNavHealth*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr4ct, hea->timeStamp);
         nmidExp.messageType = gnsstk::NavMessageType::Health;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no data of its own
            // GLOFNavHealth fields
         TUASSERTE(unsigned, 0, hea->healthBits);
         TUASSERTE(bool, false, hea->ln);
         TUASSERTE(bool, false, hea->Cn.is_valid());
      }
      else if ((isc = dynamic_cast<gnsstk::GLOFNavISC*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr4ct, isc->timeStamp);
         nmidExp.messageType = gnsstk::NavMessageType::ISC;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, isc->signal);
            // InterSigCorr
         TUASSERTFE(8.3819031715393066406e-09, isc->isc);
            // GLOFNavISC has no data of its own
      }
      else if ((eph = dynamic_cast<gnsstk::GLOFNavEph*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr1ct, eph->timeStamp);
         nmidExp.messageType = gnsstk::NavMessageType::Ephemeris;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, eph->signal);
            // OrbitData has no data of its own
            // GLOFNavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr2ct, eph->xmit2);
         TUASSERTE(gnsstk::GLOFNavSatType,
                   gnsstk::GLOFNavSatType::GLONASS_M, eph->satType);
         TUASSERTE(unsigned, 1, eph->slot);
         TUASSERTE(bool, false, eph->lhealth);
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, eph->health);
         TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
            // GLOFNavEph fields
         TUASSERTE(gnsstk::CommonTime, refExp, eph->ref);
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr3ct, eph->xmit3);
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr4ct, eph->xmit4);
         TUASSERTFE(10148.4248046875, eph->pos[0]);
         TUASSERTFE(5279.21240234375, eph->pos[1]);
         TUASSERTFE(22808.46533203125, eph->pos[2]);
         TUASSERTFE(-.6433887481689453125, eph->vel[0]);
         TUASSERTFE(3.0522518157958984375, eph->vel[1]);
         TUASSERTFE(-.4191455841064453125, eph->vel[2]);
         TUASSERTFE(-9.3132257461547851562e-10, eph->acc[0]);
         TUASSERTFE(9.3132257461547851562e-10, eph->acc[1]);
         TUASSERTFE(-1.8626451492309570312e-09, eph->acc[2]);
         TUASSERTFE(0.00015267729759216308594, eph->clkBias);
         TUASSERTFE(0, eph->freqBias);
         TUASSERTE(unsigned, 0, eph->healthBits);
         TUASSERTE(unsigned, 225, eph->tb);
         TUASSERTE(unsigned, 0, eph->P1);
         TUASSERTE(unsigned, 1, eph->P2);
         TUASSERTE(unsigned, 1, eph->P3);
         TUASSERTE(unsigned, 0, eph->P4);
         TUASSERTE(unsigned, 0, eph->interval);
         TUASSERTE(gnsstk::GLOFNavPCode,
                   gnsstk::GLOFNavPCode::CCalcGPSCalc, eph->opStatus);
         TUASSERTFE(8.3819031715393066406e-09, eph->tauDelta);
         TUASSERTE(unsigned, 0, eph->aod);
         TUASSERTE(unsigned, 1, eph->accIndex);
         TUASSERTE(unsigned, 1274, eph->dayCount);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toe);
         TUASSERTFE(60.0, eph->step);
      }
   }
   fc.validateResults(navOut, __LINE__, 3, 0, 1, 0, 1, 0, 1);
   TURETURN();
}


unsigned PNBGLOFNavDataFactory_T ::
processAlmTest()
{
   TUDEF("PNBGLOFNavDataFactory", "processAlm");
   GLOFactoryCounter fc(testFramework);
   gnsstk::PNBGLOFNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Glonass,
                             gnsstk::CarrierBand::G1,
                             gnsstk::TrackingCode::Standard,
                             gnsstk::NavType::GloCivilF),
      gnsstk::NavMessageType::Almanac);
   gnsstk::CommonTime toaExp = gnsstk::YDSTime(2015,178,2599.4375,
                                               gnsstk::TimeSystem::GLO);
   gnsstk::CommonTime refExp = gnsstk::CivilTime(2015,6,27,3,40,0,
                                                 gnsstk::TimeSystem::GLO);
   gnsstk::CommonTime beginExp = navFNAVGLOStr6ct;
   gnsstk::CommonTime endExp = gnsstk::CommonTime::END_OF_TIME;
   gnsstk::NavDataPtrList navOut;
   gnsstk::GLOFNavAlm *alm;
   gnsstk::GLOFNavHealth *hea;
   gnsstk::GLOFNavTimeOffset *ofs;
   endExp.setTimeSystem(gnsstk::TimeSystem::GLO);
   TUCSM("processTime");
   TUASSERTE(bool, true, uut.processTime(navFNAVGLOStr5, navOut));
   for (const auto& i : navOut)
   {
      if ((ofs = dynamic_cast<gnsstk::GLOFNavTimeOffset*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr5ct, ofs->timeStamp);
         nmidExp.messageType = gnsstk::NavMessageType::TimeOffset;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, ofs->signal);
            // TimeOffsetData has no data of its own
            // StdNavTimeOffset fields
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GPS, ofs->src);
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GLO, ofs->tgt);
         TUASSERTFE(-1.5832483768463134766e-08, ofs->a0);
         TUASSERTFE(0, ofs->a1);
         TUASSERTFE(0, ofs->a2);
         TUASSERTFE(16, ofs->deltatLS);
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr5ct, ofs->refTime);
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr5ct, ofs->effTime);
         TUASSERTFE(0, ofs->tot);
         TUASSERTE(unsigned, 0, ofs->wnot);
         TUASSERTE(unsigned, 0, ofs->wnLSF);
         TUASSERTE(unsigned, 0, ofs->dn);
         TUASSERTFE(0, ofs->deltatLSF);
         TUASSERTE(unsigned, 1, ofs->dnSun);
            // GLOFNavTimeOffset has no data of its own
      }
      else if ((hea = dynamic_cast<gnsstk::GLOFNavHealth*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr5ct, hea->timeStamp);
         nmidExp.messageType = gnsstk::NavMessageType::Health;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no data of its own
            // GLOFNavHealth fields
         TUASSERTE(bool, false, hea->healthBits.is_valid());
         TUASSERTE(bool, false, hea->ln);
         TUASSERTE(bool, false, hea->Cn.is_valid());
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 1, 1);
   TUCSM("processAlm");
   TUASSERTE(bool, true, uut.processAlm(6, navFNAVGLOStr6, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processAlm(7, navFNAVGLOStr7, navOut));
   for (const auto& i : navOut)
   {
      if ((alm = dynamic_cast<gnsstk::GLOFNavAlm*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr6ct, alm->timeStamp);
         nmidExp.messageType = gnsstk::NavMessageType::Almanac;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
            // OrbitData has no data of its own
            // GLOFNavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr7ct, alm->xmit2);
         TUASSERTE(gnsstk::GLOFNavSatType,
                   gnsstk::GLOFNavSatType::GLONASS_M, alm->satType);
         TUASSERTE(unsigned, 1, alm->slot);
         TUASSERTE(bool, false, alm->lhealth);
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, alm->health);
         TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
            // GLOFNavAlm
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toa);
         TUASSERTE(bool, true, alm->healthBits);
         TUASSERTFE(0.000152587890625, alm->taunA);
         TUASSERTFE(-0.56266535141894369332, alm->lambdanA);
         TUASSERTFE(0.020648819511929372705, alm->deltainA);
         TUASSERTFE(0.00044727325439453125, alm->eccnA);
         TUASSERTFE(-0.75078772187077846834, alm->omeganA);
         TUASSERTFE(2599.4375, alm->tLambdanA);
         TUASSERTFE(-2656.337890625, alm->deltaTnA);
         TUASSERTFE(0.00030517578125, alm->deltaTdotnA);
         TUASSERTE(int, 1, alm->freqnA);
      }
      else if ((hea = dynamic_cast<gnsstk::GLOFNavHealth*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr6ct, hea->timeStamp);
         nmidExp.messageType = gnsstk::NavMessageType::Health;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no data of its own
            // GLOFNavHealth fields
         TUASSERTE(bool, false, hea->healthBits.is_valid());
         TUASSERTE(bool, false, hea->ln);
         TUASSERTE(bool, true, hea->Cn);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 1, 0, 0, 1);
   TURETURN();
}


unsigned PNBGLOFNavDataFactory_T ::
processTimeUT1Test()
{
   TUDEF("PNBGLOFNavDataFactory", "processTimeUT1");
   GLOFactoryCounter fc(testFramework);
   gnsstk::PNBGLOFNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Glonass,
                             gnsstk::CarrierBand::G1,
                             gnsstk::TrackingCode::Standard,
                             gnsstk::NavType::GloCivilF),
      gnsstk::NavMessageType::TimeOffset);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GLOFNavUT1TimeOffset *to;
      // processTimeUT1 requires the reference time from string 5 and
      // NT from string 4 (the contents of which are verified in processEph)
   TUCSM("processEph");
   TUASSERTE(bool, true, uut.processEph(4, navFNAVGLOStr4, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 0, 0, 1);
   TUCSM("processTime");
   TUASSERTE(bool, true, uut.processTime(navFNAVGLOStr5, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 1, 1);
   TUCSM("processTimeUT1");
   TUASSERTE(bool, true, uut.processTimeUT1(navFNAVGLOStr14, navOut));
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gnsstk::GLOFNavUT1TimeOffset*>(i.get()))
          != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGLOStr14ct, to->timeStamp);
         nmidExp.messageType = gnsstk::NavMessageType::TimeOffset;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
            // TimeOffsetData has no data of its own
            // GLOFNavUT1TimeOffset fields
         TUASSERTFE(-8.3819031715393066406e-09, to->tauc);
         TUASSERTFE(-0.6728515625, to->B1);
         TUASSERTFE(-0.0006561279296875, to->B2);
         TUASSERTE(unsigned, 1, to->KP);
         TUASSERTE(unsigned, 1274, to->NT);
         TUASSERTE(unsigned, 1274, to->NA);
      }
   }
      // UT1 time offset is in a different class so FactoryCounter
      // calls it "other"
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 0, 0, 0, 1);
   TURETURN();
}


unsigned PNBGLOFNavDataFactory_T ::
getFrameNumTest()
{
   TUDEF("PNBGLOFNavDataFactory", "getFrameNum");
   gnsstk::PNBGLOFNavDataFactory uut;
   gnsstk::CommonTime
      t2(gnsstk::CivilTime(2015,6,27,0,40,40,gnsstk::TimeSystem::GLO)),
      t3(gnsstk::CivilTime(2015,6,27,0,41,10,gnsstk::TimeSystem::GLO)),
      t4(gnsstk::CivilTime(2015,6,27,0,41,40,gnsstk::TimeSystem::GLO));
   TUASSERTE(unsigned, 1, uut.getFrameNum(navFNAVGLOStr1ct));
   TUASSERTE(unsigned, 1, uut.getFrameNum(navFNAVGLOStr2ct));
   TUASSERTE(unsigned, 1, uut.getFrameNum(navFNAVGLOStr3ct));
   TUASSERTE(unsigned, 1, uut.getFrameNum(navFNAVGLOStr4ct));
   TUASSERTE(unsigned, 1, uut.getFrameNum(navFNAVGLOStr5ct));
   TUASSERTE(unsigned, 1, uut.getFrameNum(navFNAVGLOStr6ct));
   TUASSERTE(unsigned, 1, uut.getFrameNum(navFNAVGLOStr7ct));
   TUASSERTE(unsigned, 2, uut.getFrameNum(t2));
   TUASSERTE(unsigned, 3, uut.getFrameNum(t3));
   TUASSERTE(unsigned, 4, uut.getFrameNum(t4));
   TUASSERTE(unsigned, 5, uut.getFrameNum(navFNAVGLOStr14ct));
   TURETURN();
}


int main()
{
   PNBGLOFNavDataFactory_T testClass;
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
   errorTotal += testClass.processTimeUT1Test();
   errorTotal += testClass.getFrameNumTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
