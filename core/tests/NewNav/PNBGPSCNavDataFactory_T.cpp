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
#include "PNBGPSCNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GPSCNavTimeOffset.hpp"
#include "GPSCNavHealth.hpp"
#include "GPSCNavEph.hpp"
#include "GPSCNavAlm.hpp"
#include "GPSCNavRedAlm.hpp"
#include "GPSCNavIono.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
}


class PNBGPSCNavDataFactory_T
{
public:
   PNBGPSCNavDataFactory_T();

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
   unsigned processEphTest();
   unsigned processAlmOrbTest();
   unsigned processRedAlmOrbTest();
   unsigned process12Test();
   unsigned process30Test();
      /** Test decoding of message type 31 - clock & reduced almanac.
       * @note The truth data for the almanacs in this test have not
       *   been vetted. */
   unsigned process31Test();
   unsigned process33Test();
   unsigned process35Test();

#include "CNavTestDataDecl.hpp"

      /// Count the various types of messages present in navOut.
   void countResults(const gpstk::NavDataPtrList& navOut);

   void resetCount()
   { almCount = ephCount = toCount = heaCount = ionoCount = otherCount = 0; }
      /// Counts of messages, set by countResults.
   unsigned almCount, ephCount, toCount, heaCount, ionoCount, otherCount;
};


PNBGPSCNavDataFactory_T ::
PNBGPSCNavDataFactory_T()
{
   resetCount();
#include "CNavTestDataDef.hpp"
}


unsigned PNBGPSCNavDataFactory_T ::
addDataAllTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::SatID gloSid(1,gpstk::SatelliteSystem::Glonass);
   gpstk::ObsID gloid(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::G1,
                      gpstk::TrackingCode::Standard);
   gpstk::NavID gloNav(gpstk::NavType::GloCivilF);
   gpstk::PackedNavBitsPtr nonGPS =
      std::make_shared<gpstk::PackedNavBits>(gloSid,gloid,gloNav,"XX",
                                             msg10CNAVGPSL2ct);
   gpstk::NavDataPtrList navOut;
      // should refuse non-GPS data
   TUASSERTE(bool, false, uut.addData(nonGPS, navOut));
   countResults(navOut);
      /// @todo check that it also rejects GPS LNAV
      // get 3 health from ephemeris 1
   TUASSERTE(bool, true, uut.addData(msg10CNAVGPSL2, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // clock data completes the ephemeris
   TUASSERTE(bool, true, uut.addData(msg30CNAVGPSL2, navOut));
   TUASSERTE(size_t, 2, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   TUASSERTE(unsigned, 1, ionoCount);
   navOut.clear();
      // nothing in message type 32 that we care about (not completing
      // an ephemeris)
   TUASSERTE(bool, true, uut.addData(msg32CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // expecting a time offset record from 33
   TUASSERTE(bool, true, uut.addData(msg33CNAVGPSL2, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, toCount);
   navOut.clear();
      //
      // QZSS CNav data (L5)
      //
      // get 3 health from ephemeris 1
   TUASSERTE(bool, true, uut.addData(msg10CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // clock data completes the ephemeris
   TUASSERTE(bool, true, uut.addData(msg30CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 2, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   TUASSERTE(unsigned, 1, ionoCount);
   navOut.clear();
      // nothing in message type 32 that we care about (not completing
      // an ephemeris)
   TUASSERTE(bool, true, uut.addData(msg32CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // expecting an almanac and 3 health from message type 37
   TUASSERTE(bool, true, uut.addData(msg37CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 4, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, almCount);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
      // expecting 4 almanacs and 12 health from message type 31
   TUASSERTE(bool, true, uut.addData(msg31CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 16, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 4, almCount);
   TUASSERTE(unsigned, 12, heaCount);
   navOut.clear();
      // expecting nothing from message type 12 because it's empty.
   TUASSERTE(bool, true, uut.addData(msg12CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // expecting a time offset record from 35
   TUASSERTE(bool, true, uut.addData(msg35CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, toCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
addDataEphemerisTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Ephemeris}));
      // add ephemeris 1, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg10CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // clock data completes the ephemeris
   TUASSERTE(bool, true, uut.addData(msg30CNAVGPSL2, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   navOut.clear();
      // nothing in message type 32 that we care about (not completing
      // an ephemeris)
   TUASSERTE(bool, true, uut.addData(msg32CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // message type 33, expect nothing
   TUASSERTE(bool, true, uut.addData(msg33CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
addDataAlmanacTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Almanac}));
      /// @note We have no GPS CNav almanac data to test with so use QZSS
      // add ephemeris 1, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg10CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add ephemeris 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg11CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // clock data completes the ephemeris
   TUASSERTE(bool, true, uut.addData(msg30CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // nothing in message type 32 that we care about
   TUASSERTE(bool, true, uut.addData(msg32CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // message type 37, expect one almanac
   TUASSERTE(bool, true, uut.addData(msg37CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, almCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
addDataHealthTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Health}));
      // add ephemeris 1, expect 3 health objects.
   TUASSERTE(bool, true, uut.addData(msg10CNAVGPSL2, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // clock data completes the ephemeris, but expect nothing
   TUASSERTE(bool, true, uut.addData(msg30CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // nothing in message type 32 that we care about
   TUASSERTE(bool, true, uut.addData(msg32CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // message type 33, expect nothing
   TUASSERTE(bool, true, uut.addData(msg33CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
addDataTimeTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::TimeOffset}));
      // add ephemeris 1, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg10CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // clock data completes the ephemeris, but expect nothing
   TUASSERTE(bool, true, uut.addData(msg30CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // nothing in message type 32 that we care about
   TUASSERTE(bool, true, uut.addData(msg32CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // message type 33, expect 1 time offset message
   TUASSERTE(bool, true, uut.addData(msg33CNAVGPSL2, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, toCount);
   navOut.clear();
      // message type 35, expect 1 time offset message
   TUASSERTE(bool, true, uut.addData(msg35CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, toCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
addDataEphHealthTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Ephemeris,
                              gpstk::NavMessageType::Health}));
      // add ephemeris 1, expect 3 health objects.
   TUASSERTE(bool, true, uut.addData(msg10CNAVGPSL2, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // clock data completes the ephemeris
   TUASSERTE(bool, true, uut.addData(msg30CNAVGPSL2, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   navOut.clear();
      // nothing in message type 32 that we care about (not completing
      // an ephemeris)
   TUASSERTE(bool, true, uut.addData(msg32CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // message type 33, expect nothing
   TUASSERTE(bool, true, uut.addData(msg33CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
addDataAlmHealthTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Almanac,
                              gpstk::NavMessageType::Health}));
      // add ephemeris 1, expect 3 health objects.
   TUASSERTE(bool, true, uut.addData(msg10CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
      // add ephemeris 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg11CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add clock data, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg30CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add message type 32, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg32CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add message type 37, expect 3 health objects and 1 almanac.
   TUASSERTE(bool, true, uut.addData(msg37CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 4, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   TUASSERTE(unsigned, 1, almCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
processEphTest()
{
   TUDEF("PNBGPSCNavDataFactory", "processEph");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::NavMessageID nmidExpL1(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                            gpstk::NavType::GPSLNAV),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExpL2(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L2, gpstk::TrackingCode::L2CM,
                            gpstk::NavType::GPSCNAVL2),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExpL5(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::L5I,
                            gpstk::NavType::GPSCNAVL5),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L2, gpstk::TrackingCode::L2CM,
                            gpstk::NavType::GPSCNAVL2),
      gpstk::NavMessageType::Ephemeris);
   gpstk::CommonTime toeExp = gpstk::GPSWeekSecond(2060,91800.0);
   gpstk::CommonTime topExp = gpstk::GPSWeekSecond(2060,21600.0);
   gpstk::CommonTime beginExp = gpstk::GPSWeekSecond(2060, 86400.0);
   gpstk::CommonTime endExp = gpstk::GPSWeekSecond(2060, 97200.0);
   gpstk::NavDataPtrList navOut;
   gpstk::GPSCNavHealth *hea;
   gpstk::GPSCNavEph *eph;
      // Fail because we're telling processEph this is a message ID
      // 13.  No I don't think it's strictly worthwhile testing every
      // possible message type.
   TUASSERTE(bool, false, uut.processEph(13, msg10CNAVGPSL2, navOut));
      // success, health data only
   TUASSERTE(bool, true, uut.processEph(10, msg10CNAVGPSL2, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   resetCount();
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i.get())) != nullptr)
      {
         heaCount++;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, msg10CNAVGPSL2ct, hea->timeStamp);
         bool expHealth = false;
         switch (hea->signal.nav)
         {
            case gpstk::NavType::GPSLNAV:
               TUASSERTE(gpstk::NavMessageID, nmidExpL1, hea->signal);
               break;
            case gpstk::NavType::GPSCNAVL2:
               TUASSERTE(gpstk::NavMessageID, nmidExpL2, hea->signal);
               break;
            case gpstk::NavType::GPSCNAVL5:
               TUASSERTE(gpstk::NavMessageID, nmidExpL5, hea->signal);
               expHealth = true;
               break;
            default:
               TUFAIL("Unexpected nav type: " +
                      gpstk::StringUtils::asString(hea->signal.nav));
               break;
         }
            // NavHealthData has no fields
            // GPSCNavHealth
         TUASSERTE(bool, expHealth, hea->health);
      }
      else
      {
         otherCount++;
      }
   }
   TUASSERTE(unsigned, 3, heaCount);
   TUASSERTE(unsigned, 0, otherCount);
   navOut.clear();
   resetCount();
   TUASSERTE(bool, true, uut.processEph(11, msg11CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // success, and we have an ephemeris.
   TUASSERTE(bool, true, uut.processEph(30, msg30CNAVGPSL2, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gpstk::GPSCNavEph*>(i.get())) != nullptr)
      {
         ephCount++;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, msg10CNAVGPSL2ct, eph->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExp, eph->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gpstk::CommonTime, msg10CNAVGPSL2ct, eph->xmitTime);
         TUASSERTE(gpstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(gpstk::CommonTime, toeExp, eph->Toc); // same value as toe
         TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, eph->health);
         TUASSERTFE(-6.6496431827545166016e-06, eph->Cuc);
         TUASSERTFE(1.1029653251171112061e-05, eph->Cus);
         TUASSERTFE(175.46484375, eph->Crc);
         TUASSERTFE(-126.9296875, eph->Crs);
         TUASSERTFE(1.62050128E-07, eph->Cic);
         TUASSERTFE(1.1455267667770385742e-07, eph->Cis);
         TUASSERTFE(-0.99809220403226028839, eph->M0);
         TUASSERTFE(3.93284239E-09, eph->dn);
         TUASSERTFE(3.99142951E-14, eph->dndot);
         TUASSERTFE(0.0090113340411335229874, eph->ecc);
         TUASSERTFE(::sqrt(26560186.60546875), eph->Ahalf);
         TUASSERTFE(26560186.60546875, eph->A);
         TUASSERTFE(-0.0058078765869140625, eph->Adot);
         TUASSERTFE(2.6792122624362644245, eph->OMEGA0);
         TUASSERTFE(0.97640098663170327153, eph->i0);
         TUASSERTFE(0.72637343022039169149, eph->w);
         TUASSERTFE(-7.71776500E-09, eph->OMEGAdot);
         TUASSERTFE(1.73221501E-10, eph->idot);
         TUASSERTFE(-5.5062846513465046883e-05, eph->af0);
         TUASSERTFE(-1.01287867E-11, eph->af1);
         TUASSERTFE(0, eph->af2);
         TUASSERTE(gpstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gpstk::CommonTime, endExp, eph->endFit);
            // GPSCNavData fields
         TUASSERTE(uint32_t, 0x8b, eph->pre);
         TUASSERTE(bool, false, eph->alert);
            // GPSCNavEph fields
         TUASSERTE(uint32_t, 0x8b, eph->pre11);
         TUASSERTE(uint32_t, 0x8b, eph->preClk);;
         TUASSERTE(bool, false, eph->healthL1);
         TUASSERTE(bool, false, eph->healthL2);
         TUASSERTE(bool, true, eph->healthL5);
         TUASSERTE(int, 0, eph->uraED);
         TUASSERTE(int, -5, eph->uraNED0);
         TUASSERTE(unsigned, 1, eph->uraNED1);
         TUASSERTE(unsigned, 7, eph->uraNED2);
         TUASSERTE(bool, false, eph->alert11);
         TUASSERTE(bool, false, eph->alertClk);
         TUASSERTE(bool, false, eph->integStat);
         TUASSERTE(bool, false, eph->phasingL2C);
         TUASSERTFE(476.60546875, eph->deltaA);
         TUASSERTFE(4.5037590282126179865e-10, eph->dOMEGAdot);
         TUASSERTE(gpstk::CommonTime, topExp, eph->top);
         TUASSERTE(gpstk::CommonTime, msg11CNAVGPSL2ct, eph->xmit11);
         TUASSERTE(gpstk::CommonTime, msg30CNAVGPSL2ct, eph->xmitClk);
      }
      else
      {
         otherCount++;
      }
   }
   TUASSERTE(unsigned, 1, ephCount);
   TUASSERTE(unsigned, 0, otherCount);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
processAlmOrbTest()
{
   TUDEF("PNBGPSCNavDataFactory", "processAlmOrb");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::NavMessageID nmidExpL1(
      gpstk::NavSatelliteID(193, 193, gpstk::SatelliteSystem::QZSS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                            gpstk::NavType::GPSLNAV),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExpL2(
      gpstk::NavSatelliteID(193, 193, gpstk::SatelliteSystem::QZSS,
                            gpstk::CarrierBand::L2, gpstk::TrackingCode::L2CM,
                            gpstk::NavType::GPSCNAVL2),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExpL5(
      gpstk::NavSatelliteID(193, 193, gpstk::SatelliteSystem::QZSS,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::L5I,
                            gpstk::NavType::GPSCNAVL5),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(193, 193, gpstk::SatelliteSystem::QZSS,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::L5I,
                            gpstk::NavType::GPSCNAVL5),
      gpstk::NavMessageType::Almanac);
   gpstk::CommonTime toaExp = gpstk::GPSWeekSecond(2097, 520192.0,
                                                   gpstk::TimeSystem::QZS);
   gpstk::CommonTime beginExp = gpstk::GPSWeekSecond(2097, 345642.0,
                                                     gpstk::TimeSystem::QZS);
   gpstk::CommonTime endExp = gpstk::CommonTime::END_OF_TIME;
   gpstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.processAlmOrb(37, msg37CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 4, navOut.size());
   resetCount();
   gpstk::GPSCNavHealth *hea;
   gpstk::GPSCNavAlm *alm;
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i.get())) != nullptr)
      {
         heaCount++;
         nmidExp.messageType = gpstk::NavMessageType::Health;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, msg37CNAVQZSSL5ct, hea->timeStamp);
         switch (hea->signal.nav)
         {
            case gpstk::NavType::GPSLNAV:
               TUASSERTE(gpstk::NavMessageID, nmidExpL1, hea->signal);
               break;
            case gpstk::NavType::GPSCNAVL2:
               TUASSERTE(gpstk::NavMessageID, nmidExpL2, hea->signal);
               break;
            case gpstk::NavType::GPSCNAVL5:
               TUASSERTE(gpstk::NavMessageID, nmidExpL5, hea->signal);
               break;
            default:
               TUFAIL("Unexpected nav type: " +
                      gpstk::StringUtils::asString(hea->signal.nav));
               break;
         }
            // NavHealthData has no fields
            // GPSCNavHealth fields
         TUASSERTE(bool, false, hea->health);
      }
      else if ((alm = dynamic_cast<gpstk::GPSCNavAlm*>(i.get())) != nullptr)
      {
         almCount++;
         nmidExp.messageType = gpstk::NavMessageType::Almanac;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, msg37CNAVQZSSL5ct, alm->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExp, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gpstk::CommonTime, msg37CNAVQZSSL5ct, alm->xmitTime);
         TUASSERTE(gpstk::CommonTime, toaExp, alm->Toe);
         TUASSERTE(gpstk::CommonTime, toaExp, alm->Toc); // same value as toe
         TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, alm->health);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(-1.626115508958022593, alm->M0);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(0.014862060546875, alm->ecc);
         TUASSERTFE(6.49281250E+03, alm->Ahalf);
         TUASSERTFE(6.49281250E+03*6.49281250E+03, alm->A);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(-0.4903944831271909166, alm->OMEGA0);
         TUASSERTFE(-0.059633503129054299396 + 0.25*gpstk::PI, alm->i0);
         TUASSERTFE(-1.5729055503782394254, alm->w);
         TUASSERTFE(-2.19437712E-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-0.00034046173095703125, alm->af0);
         TUASSERTFE(7.27595761E-12, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gpstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gpstk::CommonTime, endExp, alm->endFit);
            // GPSCNavData fields
         TUASSERTE(uint32_t, 0x8b, alm->pre);
         TUASSERTE(bool, false, alm->alert);
            // GPSCNavAlm fields
         TUASSERTE(bool, false, alm->healthL1);
         TUASSERTE(bool, false, alm->healthL2);
         TUASSERTE(bool, false, alm->healthL5);
         TUASSERTFE(-0.059633503129054299396, alm->deltai);
         TUASSERTE(unsigned, 2097, alm->wna);
         TUASSERTFE(520192, alm->toa);
      }
   }
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
processRedAlmOrbTest()
{
   TUDEF("PNBGPSCNavDataFactory", "processRedAlmOrb");
      // This method is being tested implicitly by process12Test and
      // process31Test, but I'm leaving this here as a placeholder in
      // case someone gets ambitious.
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
process12Test()
{
   TUDEF("PNBGPSCNavDataFactory", "process12");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.process12(12, msg12CNAVQZSSL5, navOut));
      // easy test, the data is all empty
   TUASSERTE(size_t, 0, navOut.size());
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
process30Test()
{
   TUDEF("PNBGPSCNavDataFactory", "process30");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(ephCNAVGPSL2sid, ephCNAVGPSL2sid, oidCNAVGPSL2,
                            gpstk::NavType::GPSCNAVL2),
      gpstk::NavMessageType::Iono);
   gpstk::NavDataPtrList navOut;
   gpstk::GPSCNavIono *iono = nullptr;
   TUASSERTE(bool, true, uut.process30(msg30CNAVGPSL2, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   TUASSERT((iono = dynamic_cast<gpstk::GPSCNavIono*>(navOut.begin()->get()))
            != nullptr);
   TUASSERTE(gpstk::CommonTime, msg30CNAVGPSL2ct, iono->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExp, iono->signal);
   TUASSERTFE( 4.656612870e-09, iono->alpha[0]);
   TUASSERTFE( 1.490116118e-08, iono->alpha[1]);
   TUASSERTFE(-5.960464478e-08, iono->alpha[2]);
   TUASSERTFE(-1.192092897e-07, iono->alpha[3]);
   TUASSERTFE( 8.192000000e+04, iono->beta[0]);
   TUASSERTFE( 8.192000000e+04, iono->beta[1]);
   TUASSERTFE(-6.553600000e+04, iono->beta[2]);
   TUASSERTFE(-5.242880000e+05, iono->beta[3]);
   TUASSERTE(uint32_t, 0x8b, iono->pre);
   TUASSERTE(bool, false, iono->alert);
   TURETURN();
}

unsigned PNBGPSCNavDataFactory_T ::
process31Test()
{
   TUDEF("PNBGPSCNavDataFactory", "process31");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::NavMessageID nmidExpL1(
      gpstk::NavSatelliteID(193, 193, gpstk::SatelliteSystem::QZSS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                            gpstk::NavType::GPSLNAV),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExpL2(
      gpstk::NavSatelliteID(193, 193, gpstk::SatelliteSystem::QZSS,
                            gpstk::CarrierBand::L2, gpstk::TrackingCode::L2CM,
                            gpstk::NavType::GPSCNAVL2),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExpL5(
      gpstk::NavSatelliteID(193, 193, gpstk::SatelliteSystem::QZSS,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::L5I,
                            gpstk::NavType::GPSCNAVL5),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(193, 193, gpstk::SatelliteSystem::QZSS,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::L5I,
                            gpstk::NavType::GPSCNAVL5),
      gpstk::NavMessageType::Almanac);
   gpstk::CommonTime toaExp = gpstk::GPSWeekSecond(2097, 3937.0,
                                                   gpstk::TimeSystem::QZS);
   gpstk::CommonTime beginExp = msg31CNAVQZSSL5ct;
   gpstk::CommonTime endExp = gpstk::CommonTime::END_OF_TIME;
   gpstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.process31(31, msg31CNAVQZSSL5, navOut));
      // 4 almanacs, 12 health
   TUASSERTE(size_t, 16, navOut.size());
   gpstk::GPSCNavHealth *hea = nullptr;
   gpstk::GPSCNavRedAlm *alm = nullptr;
      // Not really trying to enforce order, but this was the easiest
      // way to verify that we got all the results we're expecting.
   auto i = navOut.begin();
      // first result
   TUASSERT((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExpL1, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // second
   ++i;
   TUASSERT((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExpL2, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // third
   ++i;
   TUASSERT((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExpL5, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // fourth
   ++i;
   TUASSERT((alm = dynamic_cast<gpstk::GPSCNavRedAlm*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, alm->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExp, alm->signal);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, alm->xmitTime);
   TUASSERTE(gpstk::CommonTime, toaExp, alm->Toe);
   TUASSERTE(gpstk::CommonTime, toaExp, alm->Toc); // same value as toe
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, alm->health);
   TUASSERTFE(0, alm->Cuc);
   TUASSERTFE(0, alm->Cus);
   TUASSERTFE(0, alm->Crc);
   TUASSERTFE(0, alm->Crs);
   TUASSERTFE(0, alm->Cis);
   TUASSERTFE(0, alm->Cic);
   TUASSERTFE(3.0925052683774527651, alm->M0);
   TUASSERTFE(0, alm->dn);
   TUASSERTFE(0, alm->dndot);
   TUASSERTFE(7.50000000E-02, alm->ecc);
   TUASSERTFE(::sqrt(4.21560080E+07), alm->Ahalf);
   TUASSERTFE(4.21560080E+07, alm->A);
   TUASSERTFE(0, alm->Adot);
   TUASSERTFE(-0.49087385212340517437, alm->OMEGA0);
   TUASSERTFE((0.25+0.0056)*gpstk::PI, alm->i0);
   TUASSERTFE(4.712388980384689674, alm->w);
   TUASSERTFE(-8.7e-10*gpstk::PI, alm->OMEGAdot);
   TUASSERTFE(0, alm->idot);
   TUASSERTFE(0, alm->af0);
   TUASSERTFE(0, alm->af1);
   TUASSERTFE(0, alm->af2);
   TUASSERTE(gpstk::CommonTime, beginExp, alm->beginFit);
   TUASSERTE(gpstk::CommonTime, endExp, alm->endFit);
   TUASSERTE(uint32_t, 0x8b, alm->pre);
   TUASSERTE(bool, false, alm->alert);
   TUASSERTE(bool, false, alm->healthL1);
   TUASSERTE(bool, false, alm->healthL2);
   TUASSERTE(bool, false, alm->healthL5);
   TUASSERTFE(0.0056*gpstk::PI, alm->deltai);
   TUASSERTE(unsigned, 2097, alm->wna);
   TUASSERTFE(3937, alm->toa);
   TUASSERTFE(-8192, alm->deltaA);
   TUASSERTFE(3.0925052683774527651, alm->phi0);
   hea = nullptr;
   alm = nullptr;
      // fifth
   ++i;
   nmidExpL1.sat.id = nmidExpL2.sat.id = nmidExpL5.sat.id = nmidExp.sat.id =
      194;
   TUASSERT((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExpL1, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // sixth
   ++i;
   TUASSERT((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExpL2, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // seventh
   ++i;
   TUASSERT((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExpL5, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr; 
      // eighth
   ++i;
   TUASSERT((alm = dynamic_cast<gpstk::GPSCNavRedAlm*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, alm->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExp, alm->signal);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, alm->xmitTime);
   TUASSERTE(gpstk::CommonTime, toaExp, alm->Toe);
   TUASSERTE(gpstk::CommonTime, toaExp, alm->Toc); // same value as toe
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, alm->health);
   TUASSERTFE(0, alm->Cuc);
   TUASSERTFE(0, alm->Cus);
   TUASSERTFE(0, alm->Crc);
   TUASSERTFE(0, alm->Crs);
   TUASSERTFE(0, alm->Cis);
   TUASSERTFE(0, alm->Cic);
   TUASSERTFE(0.88357293382212931387, alm->M0);
   TUASSERTFE(0, alm->dn);
   TUASSERTFE(0, alm->dndot);
   TUASSERTFE(7.50000000E-02, alm->ecc);
   TUASSERTFE(::sqrt(4.21657360E+07), alm->Ahalf);
   TUASSERTFE(4.21657360E+07, alm->A);
   TUASSERTFE(0, alm->Adot);
   TUASSERTFE(1.8162332528565992007, alm->OMEGA0);
   TUASSERTFE((0.25+0.0056)*gpstk::PI, alm->i0);
   TUASSERTFE(4.712388980384689674, alm->w);
   TUASSERTFE(-8.7e-10*gpstk::PI, alm->OMEGAdot);
   TUASSERTFE(0, alm->idot);
   TUASSERTFE(0, alm->af0);
   TUASSERTFE(0, alm->af1);
   TUASSERTFE(0, alm->af2);
   TUASSERTE(gpstk::CommonTime, beginExp, alm->beginFit);
   TUASSERTE(gpstk::CommonTime, endExp, alm->endFit);
   TUASSERTE(uint32_t, 0x8b, alm->pre);
   TUASSERTE(bool, false, alm->alert);
   TUASSERTE(bool, false, alm->healthL1);
   TUASSERTE(bool, false, alm->healthL2);
   TUASSERTE(bool, false, alm->healthL5);
   TUASSERTFE(0.0056*gpstk::PI, alm->deltai);
   TUASSERTE(unsigned, 2097, alm->wna);
   TUASSERTFE(3937, alm->toa);
   TUASSERTFE(1536, alm->deltaA);
   TUASSERTFE(0.88357293382212931387, alm->phi0);
   hea = nullptr;
   alm = nullptr;
      // ninth
   ++i;
   nmidExpL1.sat.id = nmidExpL2.sat.id = nmidExpL5.sat.id = nmidExp.sat.id =
      199;
   TUASSERT((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExpL1, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // tenth
   ++i;
   TUASSERT((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExpL2, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // eleventh
   ++i;
   TUASSERT((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExpL5, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // twelfth
   ++i;
   TUASSERT((alm = dynamic_cast<gpstk::GPSCNavRedAlm*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, alm->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExp, alm->signal);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, alm->xmitTime);
   TUASSERTE(gpstk::CommonTime, toaExp, alm->Toe);
   TUASSERTE(gpstk::CommonTime, toaExp, alm->Toc); // same value as toe
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, alm->health);
   TUASSERTFE(0, alm->Cuc);
   TUASSERTFE(0, alm->Cus);
   TUASSERTFE(0, alm->Crc);
   TUASSERTFE(0, alm->Crs);
   TUASSERTFE(0, alm->Cis);
   TUASSERTFE(0, alm->Cic);
   TUASSERTFE(2.4543692606170259829, alm->M0);
   TUASSERTFE(0, alm->dn);
   TUASSERTFE(0, alm->dndot);
   TUASSERTFE(7.50000000E-02, alm->ecc);
   TUASSERTFE(::sqrt(4.21642000E+07), alm->Ahalf);
   TUASSERTFE(4.21642000E+07, alm->A);
   TUASSERTFE(0, alm->Adot);
   TUASSERTFE(0, alm->OMEGA0);
   TUASSERTFE((0.25+0.0056)*gpstk::PI, alm->i0);
   TUASSERTFE(4.712388980384689674, alm->w);
   TUASSERTFE(-8.7e-10*gpstk::PI, alm->OMEGAdot);
   TUASSERTFE(0, alm->idot);
   TUASSERTFE(0, alm->af0);
   TUASSERTFE(0, alm->af1);
   TUASSERTFE(0, alm->af2);
   TUASSERTE(gpstk::CommonTime, beginExp, alm->beginFit);
   TUASSERTE(gpstk::CommonTime, endExp, alm->endFit);
   TUASSERTE(uint32_t, 0x8b, alm->pre);
   TUASSERTE(bool, false, alm->alert);
   TUASSERTE(bool, false, alm->healthL1);
   TUASSERTE(bool, false, alm->healthL2);
   TUASSERTE(bool, false, alm->healthL5);
   TUASSERTFE(0.0056*gpstk::PI, alm->deltai);
   TUASSERTE(unsigned, 2097, alm->wna);
   TUASSERTFE(3937, alm->toa);
   TUASSERTFE(0, alm->deltaA);
   TUASSERTFE(2.4543692606170259829, alm->phi0);
   hea = nullptr;
   alm = nullptr;
      // thirteenth
   ++i;
   nmidExpL1.sat.id = nmidExpL2.sat.id = nmidExpL5.sat.id = nmidExp.sat.id =
      195;
   TUASSERT((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExpL1, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // fourteenth
   ++i;
   TUASSERT((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExpL2, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // fifteenth
   ++i;
   TUASSERT((hea = dynamic_cast<gpstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExpL5, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // sixteenth
   ++i;
   TUASSERT((alm = dynamic_cast<gpstk::GPSCNavRedAlm*>(i->get())) != nullptr);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, alm->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExp, alm->signal);
   TUASSERTE(gpstk::CommonTime, msg31CNAVQZSSL5ct, alm->xmitTime);
   TUASSERTE(gpstk::CommonTime, toaExp, alm->Toe);
   TUASSERTE(gpstk::CommonTime, toaExp, alm->Toc); // same value as toe
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, alm->health);
   TUASSERTFE(0, alm->Cuc);
   TUASSERTFE(0, alm->Cus);
   TUASSERTFE(0, alm->Crc);
   TUASSERTFE(0, alm->Crs);
   TUASSERTFE(0, alm->Cis);
   TUASSERTFE(0, alm->Cic);
   TUASSERTFE(-0.83448554860978885195, alm->M0);
   TUASSERTFE(0, alm->dn);
   TUASSERTFE(0, alm->dndot);
   TUASSERTFE(7.50000000E-02, alm->ecc);
   TUASSERTFE(::sqrt(42161128), alm->Ahalf);
   TUASSERTFE(42161128, alm->A);
   TUASSERTFE(0, alm->Adot);
   TUASSERTFE(-2.7979809571034093274, alm->OMEGA0);
   TUASSERTFE((0.25+0.0056)*gpstk::PI, alm->i0);
   TUASSERTFE(4.712388980384689674, alm->w);
   TUASSERTFE(-8.7e-10*gpstk::PI, alm->OMEGAdot);
   TUASSERTFE(0, alm->idot);
   TUASSERTFE(0, alm->af0);
   TUASSERTFE(0, alm->af1);
   TUASSERTFE(0, alm->af2);
   TUASSERTE(gpstk::CommonTime, beginExp, alm->beginFit);
   TUASSERTE(gpstk::CommonTime, endExp, alm->endFit);
   TUASSERTE(uint32_t, 0x8b, alm->pre);
   TUASSERTE(bool, false, alm->alert);
   TUASSERTE(bool, false, alm->healthL1);
   TUASSERTE(bool, false, alm->healthL2);
   TUASSERTE(bool, false, alm->healthL5);
   TUASSERTFE(0.0056*gpstk::PI, alm->deltai);
   TUASSERTE(unsigned, 2097, alm->wna);
   TUASSERTFE(3937, alm->toa);
   TUASSERTFE(-3072, alm->deltaA);
   TUASSERTFE(-0.83448554860978885195, alm->phi0);
   hea = nullptr;
   alm = nullptr;
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
process33Test()
{
   TUDEF("PNBGPSCNavDataFactory", "process33");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L2, gpstk::TrackingCode::L2CM,
                            gpstk::NavType::GPSCNAVL2),
      gpstk::NavMessageType::TimeOffset);
   gpstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.process33(msg33CNAVGPSL2, navOut));
   TUASSERTE(size_t, 1, navOut.size());
      // Yes this code can cause seg faults on failure, but that's ok.
   gpstk::GPSCNavTimeOffset *to =
      dynamic_cast<gpstk::GPSCNavTimeOffset*>(navOut.begin()->get());
   TUASSERT(to != nullptr);
      // NavData fields
   TUASSERTE(gpstk::CommonTime, msg33CNAVGPSL2ct, to->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExp, to->signal);
      // TimeOffsetData has no fields
      // GPSCNavTimeOffset fields
   TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::UTC, to->tgt);
   TUASSERTFE(1.9790604711E-09, to->a0);
   TUASSERTFE(7.5495165675E-15, to->a1);
   TUASSERTFE(0.0000000000E+00, to->a2);
   TUASSERTFE(18, to->deltatLS);
   TUASSERTFE(21600, to->tot);
   TUASSERTE(unsigned, 2060, to->wnot);
   TUASSERTE(unsigned, 1929, to->wnLSF);
   TUASSERTE(gpstk::CommonTime,
             gpstk::GPSWeekSecond(2060,21600).convertToCommonTime(),
             to->refTime);
   TUASSERTE(unsigned, 7, to->dn);
   TUASSERTFE(18, to->deltatLSF);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
process35Test()
{
   TUDEF("PNBGPSCNavDataFactory", "process35");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(193, 193, gpstk::SatelliteSystem::QZSS,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::L5I,
                            gpstk::NavType::GPSCNAVL5),
      gpstk::NavMessageType::TimeOffset);
   gpstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.process35(msg35CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 1, navOut.size());
      // Yes this code can cause seg faults on failure, but that's ok.
   gpstk::GPSCNavTimeOffset *to =
      dynamic_cast<gpstk::GPSCNavTimeOffset*>(navOut.begin()->get());
   TUASSERT(to != nullptr);
      // NavData fields
   TUASSERTE(gpstk::CommonTime, msg35CNAVQZSSL5ct, to->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExp, to->signal);
      // TimeOffsetData has no fields
      // GPSCNavTimeOffset fields
   TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::QZS, to->tgt);
   TUASSERTFE(0, to->a0);
   TUASSERTFE(0, to->a1);
   TUASSERTFE(0, to->a2);
   TUASSERTFE(0, to->deltatLS);
   TUASSERTFE(356400, to->tot);
   TUASSERTE(unsigned, 2097, to->wnot);
   TUASSERTE(unsigned, 0, to->wnLSF);
   TUASSERTE(gpstk::CommonTime,
             gpstk::GPSWeekSecond(
                2097,356400,gpstk::TimeSystem::QZS).convertToCommonTime(),
             to->refTime);
   TUASSERTE(unsigned, 0, to->dn);
   TUASSERTFE(0, to->deltatLSF);
   TURETURN();
}


void PNBGPSCNavDataFactory_T ::
countResults(const gpstk::NavDataPtrList& navOut)
{
   resetCount();
   for (const auto& i : navOut)
   {
      if (dynamic_cast<gpstk::GPSCNavAlm*>(i.get()) != nullptr)
      {
         almCount++;
      }
      else if (dynamic_cast<gpstk::GPSCNavEph*>(i.get()) != nullptr)
      {
         ephCount++;
      }
      else if (dynamic_cast<gpstk::GPSCNavTimeOffset*>(i.get()) != nullptr)
      {
         toCount++;
      }
      else if (dynamic_cast<gpstk::GPSCNavHealth*>(i.get()) != nullptr)
      {
         heaCount++;
      }
      else if (dynamic_cast<gpstk::GPSCNavIono*>(i.get()) != nullptr)
      {
         ionoCount++;
      }
      else
      {
         otherCount++;
      }
         //i->dump(cerr, gpstk::DumpDetail::Full);
   }
}


int main()
{
   PNBGPSCNavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.addDataAllTest();
   errorTotal += testClass.addDataAlmanacTest();
   errorTotal += testClass.addDataEphemerisTest();
   errorTotal += testClass.addDataHealthTest();
   errorTotal += testClass.addDataTimeTest();
   errorTotal += testClass.addDataEphHealthTest();
   errorTotal += testClass.addDataAlmHealthTest();
   errorTotal += testClass.processEphTest();
   errorTotal += testClass.processAlmOrbTest();
   errorTotal += testClass.processRedAlmOrbTest();
   errorTotal += testClass.process12Test();
   errorTotal += testClass.process30Test();
   errorTotal += testClass.process31Test();
   errorTotal += testClass.process33Test();
   errorTotal += testClass.process35Test();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
