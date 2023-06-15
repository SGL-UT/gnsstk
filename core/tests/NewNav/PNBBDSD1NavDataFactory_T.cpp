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
#include <iostream>
#include <sstream>
#include <iterator>
#include "FactoryCounter.hpp"
#include "PNBBDSD1NavDataFactory.hpp"
#include "TestUtil.hpp"
#include "BDSD1NavTimeOffset.hpp"
#include "BDSD1NavHealth.hpp"
#include "BDSD1NavEph.hpp"
#include "BDSD1NavAlm.hpp"
#include "BDSD1NavIono.hpp"
#include "BDSD1NavISC.hpp"
#include "TimeString.hpp"
#include "CivilTime.hpp"

using namespace std;

// avoid having to type out template params over and over.
using BDSFactoryCounter = FactoryCounter<gnsstk::BDSD1NavAlm,gnsstk::BDSD1NavEph,gnsstk::BDSD1NavTimeOffset,gnsstk::BDSD1NavHealth,gnsstk::BDSD1NavIono,gnsstk::BDSD1NavISC>;

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
}

string asString(const gnsstk::NavMessageTypeSet& nmts)
{
   string rv;
   for (const auto& i : nmts)
   {
      rv = rv + " " + gnsstk::StringUtils::asString(i);
   }
   return rv;
}



class PNBBDSD1NavDataFactory_T
{
public:
   PNBBDSD1NavDataFactory_T();
      /// Test that BDS doesn't process GLONASS data
   unsigned foreignTest();
      /// The meat of all the filter state tests.
   unsigned filterTest(const gnsstk::NavMessageTypeSet& nmts);
      /// addData tests for a variety of filter states
   unsigned filterTests();
      /// Test validity checking.
   unsigned addDataValidityTest();
   unsigned processAlmTest();
   unsigned processEphTest();
   unsigned processSF5Pg7Test();
   unsigned processSF5Pg8Test();
   unsigned processSF5Pg9Test();
   unsigned processSF5Pg10Test();
   unsigned processSF5Pg24Test();
   unsigned isAlmDefaultTest();

#include "D1NavTestDataDecl.hpp"
};


PNBBDSD1NavDataFactory_T ::
PNBBDSD1NavDataFactory_T()
{
#include "D1NavTestDataDef.hpp"
}


unsigned PNBBDSD1NavDataFactory_T ::
foreignTest()
{
   TUDEF("PNBBDSD1NavDataFactory", "addData");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD1NavDataFactory uut;
   gnsstk::SatID gloSid(1,gnsstk::SatelliteSystem::Glonass);
   gnsstk::ObsID gloid(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::G1,
                      gnsstk::TrackingCode::Standard);
   gnsstk::NavID gloNav(gnsstk::NavType::GloCivilF);
   gnsstk::PackedNavBitsPtr nonBDS = std::make_shared<gnsstk::PackedNavBits>(
      gloSid, gloid, gloNav, "XX", ephD1NAVSF1ct);
   gnsstk::NavDataPtrList navOut;
      // should refuse non-BDS data
   TUASSERTE(bool, false, uut.addData(nonBDS, navOut));
   TURETURN();
}


unsigned PNBBDSD1NavDataFactory_T ::
filterTest(const gnsstk::NavMessageTypeSet& nmts)
{
   TUDEF("PNBBDSD1NavDataFactory", "addData("+asString(nmts)+")");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD1NavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter(nmts));
   fc.setInc(nmts);
      // Add subframe 1, expect 1 health, 1 ISC and 1 Iono.
   TUASSERTE(bool, true, uut.addData(ephD1NAVSF1, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 1, 1, 1);
      // Add subframe 2, expect nothing yet.
   TUASSERTE(bool, true, uut.addData(ephD1NAVSF2, navOut));
   fc.validateResults(navOut, __LINE__);
      // Add subframe 3, expect the completed ephemeris.
   TUASSERTE(bool, true, uut.addData(ephD1NAVSF3, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
      // Add subframe 4 page 1, expect nothing yet.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p1, navOut));
   fc.validateResults(navOut, __LINE__);
      // Add subframe 5 page 1, expect nothing yet.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p1, navOut));
   fc.validateResults(navOut, __LINE__);
      // Add subframe 4 page 7, expect nothing yet.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p7, navOut));
   fc.validateResults(navOut, __LINE__);
      // Add subframe 5 page 7, expect 19 health.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p7, navOut));
   fc.validateResults(navOut, __LINE__, 19, 0, 0, 0, 19);
      // Add subframe 4 page 8, expect nothing yet.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p8, navOut));
   fc.validateResults(navOut, __LINE__);
      // Add subframe 5 page 8, expect 11 health and 3 almanacs.
      // (page 7 will have been discarded for toa mismatch)
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p8, navOut));
   fc.validateResults(navOut, __LINE__, 14, 3, 0, 0, 11);
      // Add subframe 4 page 9, expect 1 almanac.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p9, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
      // Add subframe 5 page 9, expect 3 time offsets.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p9, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 3);
      // Add subframe 4 page 10, expect 1 almanac.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p10, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
      // Add subframe 5 page 10, expect 1 time offset.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p10, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
      // Add subframe 4 page 11, expect 1 almanac.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p11, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
      // Add subframe 5 page 11, expect nothing because 1) the
      // previous subframe has an AmEpID != b11 and 2) the data is all
      // zeroes.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p11, navOut));
   fc.validateResults(navOut, __LINE__);
      // Add subframe 4 page 24, expect nothing because the toa has changed
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p24B, navOut));
   fc.validateResults(navOut, __LINE__);
      // Add subframe 5 page 24, expect nothing because AmEpID=b11 and AmID=b00
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p24B, navOut));
   fc.validateResults(navOut, __LINE__);
      // add a different subframe 4, page 8, expect nothing yet
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p8B, navOut));
   fc.validateResults(navOut, __LINE__);
      // add a different subframe 5, page 8, expect 1 almanac, 11 health
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p8B, navOut));
   fc.validateResults(navOut, __LINE__, 12, 1, 0, 0, 11);
      // add a different subframe 4, page 11, expect nothing because no health
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p11B, navOut));
   fc.validateResults(navOut, __LINE__);
      // add a different subframe 5, page 11, expect nothing because
      // no health and toa mismatch.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p11B, navOut));
   fc.validateResults(navOut, __LINE__);
      // add a different subframe 4, page 12, expect nothing because no health
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p12B, navOut));
   fc.validateResults(navOut, __LINE__);
      // add a different subframe 5, page 12, expect nothing because no health
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p12B, navOut));
   fc.validateResults(navOut, __LINE__);
      // add a different subframe 4, page 7, expect nothing because no
      // health and toa mismatch
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p7B, navOut));
   fc.validateResults(navOut, __LINE__);
      // add a different subframe 5, page 7, expect 19 health and 3 alm
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p7B, navOut));
   fc.validateResults(navOut, __LINE__, 22, 3, 0, 0, 19);
      // add a different subframe 4, page 24, expect 1 alm
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p24B2, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
      // add a different subframe 5, page 24, expect 13 health and 1 alm
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p24B2, navOut));
   fc.validateResults(navOut, __LINE__, 14, 1, 0, 0, 13);
   TURETURN();
}


unsigned PNBBDSD1NavDataFactory_T ::
addDataValidityTest()
{
   TUDEF("PNBBDSD1NavDataFactory", "addData");
      /// @todo implement some tests after we have parity checking and such.
   TURETURN();
}


unsigned PNBBDSD1NavDataFactory_T ::
processAlmTest()
{
   TUDEF("PNBBDSD1NavDataFactory", "processAlm");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD1NavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(32, 19, gnsstk::SatelliteSystem::BeiDou,
                            gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I,
                            gnsstk::NavType::BeiDou_D1),
      gnsstk::NavMessageType::Almanac);
   gnsstk::CommonTime toeExp = gnsstk::BDSWeekSecond(810,81920.0);
   gnsstk::CommonTime beginExp = almD1NAVSF5p12Bct;
   gnsstk::CommonTime endExp = gnsstk::CommonTime::END_OF_TIME;
   gnsstk::NavDataPtrList navOut;
   gnsstk::BDSD1NavAlm *alm;
   endExp.setTimeSystem(gnsstk::TimeSystem::BDT);
      // First store the subframes with the extra necessary data.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p8B, navOut));
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p8B, navOut));   // WNa
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p12B, navOut));
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p24B2, navOut)); // health
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p24B2, navOut));
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p12B, navOut));  // AmEpID
   navOut.clear();
      // success, almanac data only
      /// @warning data values not vetted
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p12B, navOut));
   for (const auto& i : navOut)
   {
      if ((alm = dynamic_cast<gnsstk::BDSD1NavAlm*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, almD1NAVSF5p12Bct, alm->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, almD1NAVSF5p12Bct, alm->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, alm->Toe);
         TUASSERTE(gnsstk::CommonTime, toeExp, alm->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, alm->health);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(-0.10815548896789550781, alm->M0);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(0.0005245208740234375, alm->ecc);
         TUASSERTFE(5282.57421875, alm->Ahalf);
         TUASSERTFE(27905590.376602172852, alm->A);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(-0.92388606071472167969, alm->OMEGA0);
         TUASSERTFE(0.3073299407958984264, alm->i0);
         TUASSERTFE(-0.31140887737274169922, alm->w);
         TUASSERTFE(-2.1864252630621194839e-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-0.00092411041259765625, alm->af0);
         TUASSERTFE(0, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
            // BDSD1NavData fields
         TUASSERTE(uint32_t, 0x712, alm->pre);
         TUASSERTE(uint32_t, 0, alm->rev);
         TUASSERTE(unsigned, 5, alm->fraID);
         TUASSERTE(uint32_t, 345954, alm->sow);
            // BDSD1NavAlm fields
         TUASSERTE(unsigned, 12, alm->pnum);
         TUASSERTFE(0.0073299407958984375, alm->deltai);
         TUASSERTFE(81920, alm->toa);
         TUASSERTE(uint16_t, 88, alm->healthBits);
         TUASSERTE(bool, false, alm->isDefault);
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 1);
   TURETURN();
}


unsigned PNBBDSD1NavDataFactory_T ::
processEphTest()
{
   TUDEF("PNBBDSD1NavDataFactory", "processEph");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD1NavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(6, 6, gnsstk::SatelliteSystem::BeiDou,
                            gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I,
                            gnsstk::NavType::BeiDou_D1),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime toeExp = gnsstk::BDSWeekSecond(810,345600.0);
   gnsstk::CommonTime beginExp = gnsstk::BDSWeekSecond(810,345614.0);
   gnsstk::CommonTime endExp = gnsstk::BDSWeekSecond(810,432044.0);
   gnsstk::NavDataPtrList navOut;
   gnsstk::BDSD1NavHealth *hea;
   gnsstk::BDSD1NavEph *eph;
   gnsstk::BDSD1NavIono *iono;
   gnsstk::BDSD1NavISC *isc;
      // success, health, ISC and iono data only
   TUASSERTE(bool, true, uut.processEph(1, ephD1NAVSF1, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::BDSD1NavHealth*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Health;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephD1NAVSF1ct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // BDSD1NavHealth
         TUASSERTE(bool, false, hea->isAlmHealth);
         TUASSERTE(bool, false, hea->satH1);
         TUASSERTE(uint16_t, 255, hea->svHealth);
      }
      else if ((iono = dynamic_cast<gnsstk::BDSD1NavIono*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Iono;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephD1NAVSF1ct, iono->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, iono->signal);
            // KlobucharIonoNavData
         TUASSERTFE( 6.519258E-09, iono->alpha[0]);
         TUASSERTFE( 2.98023223876953125e-08, iono->alpha[1]);
         TUASSERTFE(-2.98023223876953125e-07, iono->alpha[2]);
         TUASSERTFE( 4.17232513427734375e-07, iono->alpha[3]);
         TUASSERTFE( 112640, iono->beta[0]);
         TUASSERTFE( 180224, iono->beta[1]);
         TUASSERTFE(-524288, iono->beta[2]);
         TUASSERTFE( 458752, iono->beta[3]);
            // BDSD1NavIono
         TUASSERTE(uint32_t, 0x712, iono->pre);
         TUASSERTE(uint32_t, 0, iono->rev);
         TUASSERTE(unsigned, 1, iono->fraID);
         TUASSERTE(uint32_t, 345600, iono->sow);
      }
      else if ((isc = dynamic_cast<gnsstk::BDSD1NavISC*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::ISC;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephD1NAVSF1ct, isc->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, isc->signal);
            // InterSigCorr
         TUASSERTE(bool, true, std::isnan(isc->isc));
            // BDSD1NavISC
         TUASSERTE(uint32_t, 0x712, isc->pre);
         TUASSERTE(uint32_t, 0, isc->rev);
         TUASSERTE(unsigned, 1, isc->fraID);
         TUASSERTE(uint32_t, 345600, isc->sow);
         TUASSERTFE(8.2e-9, isc->tgd1);
         TUASSERTFE(-1.9e-9, isc->tgd2);
      }
   }
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 1, 1, 1);
      // success but no additional data
   TUASSERTE(bool, true, uut.processEph(2, ephD1NAVSF2, navOut));
   fc.validateResults(navOut, __LINE__);
      // success and we have an ephemeris.
   TUASSERTE(bool, true, uut.processEph(3, ephD1NAVSF3, navOut));
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gnsstk::BDSD1NavEph*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Ephemeris;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephD1NAVSF1ct, eph->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, eph->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, ephD1NAVSF1ct, eph->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, eph->health);
         TUASSERTFE( 2.102460712194442749E-05, eph->Cus);
         TUASSERTFE(-4.5062042772769927979E-06, eph->Cuc);
         TUASSERTFE(-4.04640625E+02, eph->Crc);
         TUASSERTFE(-1.43203125E+02, eph->Crs);
         TUASSERTFE( 7.91624188E-08, eph->Cic);
         TUASSERTFE(-3.39932740E-08, eph->Cis);
         TUASSERTFE(-8.5672685685526606125E-02, eph->M0);
         TUASSERTFE( 1.20183578E-09, eph->dn);
         TUASSERTFE(0, eph->dndot);
         TUASSERTFE( 1.1284291860647499561E-02, eph->ecc);
         TUASSERTFE(sqrt(4.2170134522822760046E+07), eph->Ahalf);
         TUASSERTFE( 4.2170134522822760046E+07, eph->A);
         TUASSERTFE(0, eph->Adot);
         TUASSERTFE(-1.9160860634641034839E+00, eph->OMEGA0);
         TUASSERTFE(9.450732988963341796E-01, eph->i0);
         TUASSERTFE(-2.1772769963262814663E+00, eph->w);
         TUASSERTFE(-1.78078846E-09, eph->OMEGAdot);
         TUASSERTFE( 4.37518224E-10, eph->idot);
         TUASSERTFE( 6.2543887179344892502E-04, eph->af0);
         TUASSERTFE( 4.82804907E-11, eph->af1);
         TUASSERTFE( 0.00000000E+00, eph->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
            // BDSD1NavData fields
         TUASSERTE(uint32_t, 0x712, eph->pre);
         TUASSERTE(uint32_t, 0, eph->rev);
         TUASSERTE(unsigned, 1, eph->fraID);
         TUASSERTE(uint32_t, 345600, eph->sow);
            // BDSD1NavEph fields
         TUASSERTE(uint32_t, 0x712, eph->pre2);
         TUASSERTE(uint32_t, 0x712, eph->pre3);
         TUASSERTE(uint32_t, 0, eph->rev2);
         TUASSERTE(uint32_t, 0, eph->rev3);
         TUASSERTE(uint32_t, 345606, eph->sow2);
         TUASSERTE(uint32_t, 345612, eph->sow3);
         TUASSERTE(bool, false, eph->satH1);
         TUASSERTE(unsigned, 0, eph->aodc);
         TUASSERTE(unsigned, 1, eph->aode);
         TUASSERTE(unsigned, 0, eph->uraIndex);
         TUASSERTE(gnsstk::CommonTime, ephD1NAVSF2ct, eph->xmit2);
         TUASSERTE(gnsstk::CommonTime, ephD1NAVSF3ct, eph->xmit3);
         TUASSERTFE(8.2e-9, eph->tgd1);
         TUASSERTFE(-1.9e-9, eph->tgd2);
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
   TURETURN();
}


unsigned PNBBDSD1NavDataFactory_T ::
processSF5Pg7Test()
{
   TUDEF("PNBBDSD1NavDataFactory", "processSF5Pg7");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD1NavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(1, 6, gnsstk::SatelliteSystem::BeiDou,
                            gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I,
                            gnsstk::NavType::BeiDou_D1),
      gnsstk::NavMessageType::Health);
   gnsstk::NavDataPtrList navOut;
   gnsstk::BDSD1NavHealth *hea;
   uint16_t expHea;
      // success, health data only
   TUASSERTE(bool, true, uut.processSF5Pg7(almD1NAVSF5p7, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::BDSD1NavHealth*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, almD1NAVSF5p7ct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // BDSD1NavHealth
         TUASSERTE(bool, true, hea->isAlmHealth);
         TUASSERTE(bool, true, hea->satH1);
         if ((nmidExp.sat.id == 7) || (nmidExp.sat.id == 17))
            expHea = 256;
         else if ((nmidExp.sat.id == 15) || (nmidExp.sat.id == 18))
            expHea = 510;
         else if (nmidExp.sat.id >= 19)
            expHea = 88;
         else
            expHea = 0;
         TUASSERTE(uint16_t, expHea, hea->svHealth);
            // While it's not strictly necessary that the results come
            // in PRN order, they do currently and this statement
            // addresses that fact.
         nmidExp.sat.id++;
      }
   }
   fc.validateResults(navOut, __LINE__, 19, 0, 0, 0, 19);
   TURETURN();
}


unsigned PNBBDSD1NavDataFactory_T ::
processSF5Pg8Test()
{
   TUDEF("PNBBDSD1NavDataFactory", "processSF5Pg8");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD1NavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(20, 6, gnsstk::SatelliteSystem::BeiDou,
                            gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I,
                            gnsstk::NavType::BeiDou_D1),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime toeExp = gnsstk::BDSWeekSecond(810,81920.0);
   gnsstk::CommonTime beginExp = almD1NAVSF5p1ct;
   gnsstk::CommonTime endExp = gnsstk::CommonTime::END_OF_TIME;
   gnsstk::NavDataPtrList navOut;
   gnsstk::BDSD1NavAlm *alm;
   gnsstk::BDSD1NavHealth *hea;
   uint16_t expHea = 88;
      // First store the subframes with almanacs that shouldn't be
      // immediately returned.
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(almD1NAVSF5p1, navOut));
   fc.validateResults(navOut, __LINE__);
      // should work the same whether SF4 pg8 was processed or not.
   TUASSERTE(bool, true, uut.processSF5Pg8(almD1NAVSF5p8, navOut));
   for (const auto& i : navOut)
   {
      if ((alm = dynamic_cast<gnsstk::BDSD1NavAlm*>(i.get())) != nullptr)
      {
            /// @warning data values not vetted
         nmidExp.messageType = gnsstk::NavMessageType::Almanac;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, almD1NAVSF5p1ct, alm->timeStamp);
         gnsstk::SatID tmpSat = nmidExp.sat;
         nmidExp.sat.id = 25;
         TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
         nmidExp.sat = tmpSat;
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, almD1NAVSF5p1ct, alm->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, alm->Toe);
         TUASSERTE(gnsstk::CommonTime, toeExp, alm->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, alm->health);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(-0.60474646091461181641, alm->M0);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(0.0003604888916015625, alm->ecc);
         TUASSERTFE(5282.56201171875, alm->Ahalf);
         TUASSERTFE(27905461.407654047012, alm->A);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(-0.25995099544525146484, alm->OMEGA0);
         TUASSERTFE(0.3025119781494140514, alm->i0);
         TUASSERTFE(0.017611861228942871094, alm->w);
         TUASSERTFE(-2.25554686039686203e-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-0.0008869171142578125, alm->af0);
         TUASSERTFE(7.2759576141834259033e-12, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
            // BDSD1NavData fields
         TUASSERTE(uint32_t, 0x712, alm->pre);
         TUASSERTE(uint32_t, 0, alm->rev);
         TUASSERTE(unsigned, 5, alm->fraID);
         TUASSERTE(uint32_t, 345624, alm->sow);
            // BDSD1NavAlm fields
         TUASSERTE(unsigned, 1, alm->pnum);
         TUASSERTFE(0.0025119781494140625, alm->deltai);
         TUASSERTFE(81920, alm->toa);
         TUASSERTE(uint16_t, 88, alm->healthBits);
         TUASSERTE(bool, false, alm->isDefault);
      }
      else if ((hea = dynamic_cast<gnsstk::BDSD1NavHealth*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Health;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, almD1NAVSF5p8ct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // BDSD1NavHealth
         TUASSERTE(bool, true, hea->isAlmHealth);
         TUASSERTE(bool, true, hea->satH1);
         TUASSERTE(uint16_t, expHea, hea->svHealth);
            // While it's not strictly necessary that the results come
            // in PRN order, they do currently and this statement
            // addresses that fact.
         nmidExp.sat.id++;
      }
   }
      // Note that sf4 pg1 is going to be the almanac for PRN 1 which
      // we won't have health for when processSF5Pg8 is called so it
      // won't show up.  SF5 pg1 OTOH is PRN 25 whose health is
      // present in pg 8.
   fc.validateResults(navOut, __LINE__, 12, 1, 0, 0, 11);
   TURETURN();
}


unsigned PNBBDSD1NavDataFactory_T ::
processSF5Pg9Test()
{
   TUDEF("PNBBDSD1NavDataFactory", "processSF5Pg9");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD1NavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(6, 6, gnsstk::SatelliteSystem::BeiDou,
                            gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I,
                            gnsstk::NavType::BeiDou_D1),
      gnsstk::NavMessageType::TimeOffset);
   gnsstk::CommonTime refTimeExp = gnsstk::BDSWeekSecond(810,0);
   gnsstk::CommonTime effTimeExp;
   gnsstk::NavDataPtrList navOut, navOut2, navOut3, navOut4;
   gnsstk::BDSD1NavTimeOffset *to;
   uint16_t expHea;
      // success, time offset data only
   TUASSERTE(bool, true, uut.processSF5Pg9(almD1NAVSF5p9, navOut));
      // Time systems are processed in the order they appear in the
      // nav message. Doesn't need to be enforced, but doing so
      // simplifies the validation of the output.
   gnsstk::TimeSystem expTS = gnsstk::TimeSystem::GPS;
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gnsstk::BDSD1NavTimeOffset*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, almD1NAVSF5p9ct, to->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
            // TimeOffsetData has no fields
            // StdNavTimeOffset
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::BDT, to->src);
         TUASSERTE(gnsstk::TimeSystem, expTS, to->tgt);
         TUASSERTFE(0.0, to->a0);
         TUASSERTFE(0.0, to->a1);
         TUASSERTFE(0.0, to->a2);
         TUASSERTFE(0.0, to->deltatLS);
         TUASSERTE(gnsstk::CommonTime, refTimeExp, to->refTime);
         TUASSERTE(gnsstk::CommonTime, effTimeExp, to->effTime);
         TUASSERTFE(0.0, to->tot);
         TUASSERTE(unsigned, 0, to->wnot);
         TUASSERTE(unsigned, 0, to->wnLSF);
         TUASSERTE(unsigned, 0, to->dn);
         TUASSERTFE(0.0, to->deltatLSF);
            // BDSD1NavTimeOffset has no fields, but we check validate here
         TUCSM("validate");
         TUASSERTE(bool, false, to->validate());
         if (expTS == gnsstk::TimeSystem::GPS)
            expTS = gnsstk::TimeSystem::GAL;
         else if (expTS == gnsstk::TimeSystem::GAL)
            expTS = gnsstk::TimeSystem::GLO;
         TUCSM("processSF5Pg9");
      }
   }
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 3);
      // Test with double-zero filtering enabled
   gnsstk::FactoryControl ctrl;
   ctrl.bdsTimeZZfilt = true;
   gnsstk::PNBBDSD1NavDataFactory uut2;
   uut2.setControl(ctrl);
   TUASSERTE(bool, true, uut2.processSF5Pg9(almD1NAVSF5p9, navOut2));
      // all the time offset data has double-zeroes
   TUASSERT(navOut2.empty());
      // Test double-zero filter with A0 != 0, A1 = 0 
   gnsstk::PNBBDSD1NavDataFactory uut3;
   uut3.setControl(ctrl);
   TUASSERTE(bool, true, uut3.processSF5Pg9(almD1NAVSF5p9A0, navOut3));
   fc.validateResults(navOut3, __LINE__, 3, 0, 0, 3);
      // Test double-zero filter with A0 = 0, A1 != 0 
   gnsstk::PNBBDSD1NavDataFactory uut4;
   uut4.setControl(ctrl);
   TUASSERTE(bool, true, uut4.processSF5Pg9(almD1NAVSF5p9A0, navOut4));
   fc.validateResults(navOut4, __LINE__, 3, 0, 0, 3);
   TURETURN();
}


unsigned PNBBDSD1NavDataFactory_T ::
processSF5Pg10Test()
{
   TUDEF("PNBBDSD1NavDataFactory", "processSF5Pg10");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD1NavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(6, 6, gnsstk::SatelliteSystem::BeiDou,
                            gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I,
                            gnsstk::NavType::BeiDou_D1),
      gnsstk::NavMessageType::TimeOffset);
   gnsstk::CommonTime refTimeExp = gnsstk::BDSWeekSecond(810,0);
      // Seems strange that this would be in the future rather than the past.
   gnsstk::CommonTime effTimeExp = gnsstk::BDSWeekSecond(829,518400);
   gnsstk::NavDataPtrList navOut, navOut2, navOut3, navOut4;
   gnsstk::BDSD1NavTimeOffset *to;
   uint16_t expHea;
      // success, time offset data only
   TUASSERTE(bool, true, uut.processSF5Pg10(almD1NAVSF5p10, navOut));
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gnsstk::BDSD1NavTimeOffset*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, almD1NAVSF5p10ct, to->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
            // TimeOffsetData has no fields
            // StdNavTimeOffset
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::BDT, to->src);
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC, to->tgt);
         TUASSERTFE(-6.519258e-09, to->a0);
         TUASSERTFE(0.0, to->a1);
         TUASSERTFE(0.0, to->a2);
         TUASSERTFE(4.0, to->deltatLS);
         TUASSERTE(gnsstk::CommonTime, refTimeExp, to->refTime);
         TUASSERTE(gnsstk::CommonTime, effTimeExp, to->effTime);
         TUASSERTFE(0.0, to->tot);
         TUASSERTE(unsigned, 0, to->wnot);
         TUASSERTE(unsigned, 829, to->wnLSF);
         TUASSERTE(unsigned, 6, to->dn);
         TUASSERTFE(4.0, to->deltatLSF);
            // BDSD1NavTimeOffset has no fields, but we check validate here
         TUCSM("validate");
         TUASSERTE(bool, true, to->validate());
         TUCSM("processSF5Pg10");
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
      // Test double-zero filter with A0 != 0, A1 = 0
   gnsstk::FactoryControl ctrl;
   ctrl.bdsTimeZZfilt = true;
   gnsstk::PNBBDSD1NavDataFactory uut2;
   uut2.setControl(ctrl);
   TUASSERTE(bool, true, uut2.processSF5Pg10(almD1NAVSF5p10, navOut2));
   fc.validateResults(navOut2, __LINE__, 1, 0, 0, 1);
      // Test with double-zero filtering enabled
   gnsstk::PNBBDSD1NavDataFactory uut3;
   uut3.setControl(ctrl);
   TUASSERTE(bool, true, uut3.processSF5Pg10(almD1NAVSF5p10ZZ, navOut3));
   fc.validateResults(navOut3, __LINE__, 0, 0, 0, 0);
      // Test double-zero filter with A0 != 0, A1 = 0
   gnsstk::PNBBDSD1NavDataFactory uut4;
   uut4.setControl(ctrl);
   TUASSERTE(bool, true, uut4.processSF5Pg10(almD1NAVSF5p10, navOut4));
   fc.validateResults(navOut4, __LINE__, 1, 0, 0, 1);
   TURETURN();
}


unsigned PNBBDSD1NavDataFactory_T ::
processSF5Pg24Test()
{
   TUDEF("PNBBDSD1NavDataFactory", "processSF5Pg24");
   BDSFactoryCounter fc(testFramework);
   gnsstk::PNBBDSD1NavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(31, 19, gnsstk::SatelliteSystem::BeiDou,
                            gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I,
                            gnsstk::NavType::BeiDou_D1),
      gnsstk::NavMessageType::Health);
   gnsstk::NavDataPtrList navOut;
   gnsstk::BDSD1NavHealth *hea;
   uint16_t expHea = 88;
      // No need to check amID=0 as that's already tested in filterTest.
      // Test with AmID=b01
   TUASSERTE(bool, true, uut.addData(almD1NAVSF4p24B2, navOut));
      // success, health data only
   TUASSERTE(bool, true, uut.processSF5Pg24(almD1NAVSF5p24B2, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::BDSD1NavHealth*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, almD1NAVSF5p24B2ct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // BDSD1NavHealth
         TUASSERTE(bool, true, hea->isAlmHealth);
         TUASSERTE(bool, true, hea->satH1);
         TUASSERTE(uint16_t, expHea, hea->svHealth);
            // While it's not strictly necessary that the results come
            // in PRN order, they do currently and this statement
            // addresses that fact.
         nmidExp.sat.id++;
      }
   }
      /// @todo test with AmID=b10 and b11
   fc.validateResults(navOut, __LINE__, 13, 0, 0, 0, 13);
   TURETURN();
}


unsigned PNBBDSD1NavDataFactory_T ::
isAlmDefaultTest()
{
   TUDEF("PNBBDSD1NavDataFactory", "isAlmDefault");
   gnsstk::PNBBDSD1NavDataFactory uut;
   TUASSERTE(bool, true, uut.isAlmDefault(almD1Default1));
   TUASSERTE(bool, false, uut.isAlmDefault(almD1Default2));
   TUASSERTE(bool, false, uut.isAlmDefault(almD1Default3));
   TURETURN();
}


unsigned PNBBDSD1NavDataFactory_T ::
filterTests()
{
   return filterTest(gnsstk::allNavMessageTypes) +
      filterTest({gnsstk::NavMessageType::Almanac}) +
      filterTest({gnsstk::NavMessageType::Ephemeris}) +
      filterTest({gnsstk::NavMessageType::Health}) +
      filterTest({gnsstk::NavMessageType::TimeOffset}) +
      filterTest({gnsstk::NavMessageType::Iono}) +
      filterTest({gnsstk::NavMessageType::ISC}) +
      filterTest({gnsstk::NavMessageType::Ephemeris,
                  gnsstk::NavMessageType::Health});
}


int main()
{
   PNBBDSD1NavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.foreignTest();
   errorTotal += testClass.filterTests();
   errorTotal += testClass.addDataValidityTest();
   errorTotal += testClass.processAlmTest();
   errorTotal += testClass.processEphTest();
   errorTotal += testClass.processSF5Pg7Test();
   errorTotal += testClass.processSF5Pg8Test();
   errorTotal += testClass.processSF5Pg9Test();
   errorTotal += testClass.processSF5Pg10Test();
   errorTotal += testClass.processSF5Pg24Test();
   errorTotal += testClass.isAlmDefaultTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
