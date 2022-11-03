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
#include "PNBGPSCNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GPSCNavTimeOffset.hpp"
#include "GPSCNavHealth.hpp"
#include "GPSCNavEph.hpp"
#include "GPSCNavAlm.hpp"
#include "GPSCNavRedAlm.hpp"
#include "GPSCNavIono.hpp"
#include "GPSCNavISC.hpp"
#include "TimeString.hpp"

using namespace std;

// avoid having to type out template params over and over.
using GPSFactoryCounter = FactoryCounter<gnsstk::GPSCNavAlm,gnsstk::GPSCNavEph,gnsstk::GPSCNavTimeOffset,gnsstk::GPSCNavHealth,gnsstk::GPSCNavIono,gnsstk::GPSCNavISC>;

namespace gnsstk
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
      // week rollover test for decoding ephemerides
   unsigned processEphWRTest();

#include "CNavTestDataDecl.hpp"
};


PNBGPSCNavDataFactory_T ::
PNBGPSCNavDataFactory_T()
{
#include "CNavTestDataDef.hpp"
}


unsigned PNBGPSCNavDataFactory_T ::
addDataAllTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::SatID gloSid(1,gnsstk::SatelliteSystem::Glonass);
   gnsstk::ObsID gloid(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::G1,
                      gnsstk::TrackingCode::Standard);
   gnsstk::NavID gloNav(gnsstk::NavType::GloCivilF);
   gnsstk::PackedNavBitsPtr nonGPS =
      std::make_shared<gnsstk::PackedNavBits>(gloSid,gloid,gloNav,"XX",
                                             msg10CNAVGPSL2ct);
   gnsstk::NavDataPtrList navOut;
      // should refuse non-GPS data
   TUASSERTE(bool, false, uut.addData(nonGPS, navOut));
   fc.validateResults(navOut, __LINE__);
      /// @todo check that it also rejects GPS LNAV
      // get 3 health from ephemeris 1
   TUASSERTE(bool, true, uut.addData(msg10CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 3);
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // clock data completes the ephemeris
   TUASSERTE(bool, true, uut.addData(msg30CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 1, 0, 0, 1, 1);
      // nothing in message type 32 that we care about (not completing
      // an ephemeris)
   TUASSERTE(bool, true, uut.addData(msg32CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // expecting a time offset record from 33
   TUASSERTE(bool, true, uut.addData(msg33CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
      //
      // QZSS CNav data (L5)
      //
      // get 3 health from ephemeris 1
   TUASSERTE(bool, true, uut.addData(msg10CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 3);
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__);
      // clock data completes the ephemeris
   TUASSERTE(bool, true, uut.addData(msg30CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 1, 0, 0, 1, 1);
      // nothing in message type 32 that we care about (not completing
      // an ephemeris)
   TUASSERTE(bool, true, uut.addData(msg32CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__);
      // expecting an almanac and 3 health from message type 37
   TUASSERTE(bool, true, uut.addData(msg37CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__, 4, 1, 0, 0, 3);
      // expecting 4 almanacs and 12 health from message type 31
   TUASSERTE(bool, true, uut.addData(msg31CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__, 16, 4, 0, 0, 12);
      // expecting nothing from message type 12 because it's empty.
   TUASSERTE(bool, true, uut.addData(msg12CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__);
      // expecting a time offset record from 35
   TUASSERTE(bool, true, uut.addData(msg35CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
addDataEphemerisTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris}));
      // add ephemeris 1, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg10CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // clock data completes the ephemeris
   TUASSERTE(bool, true, uut.addData(msg30CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
      // nothing in message type 32 that we care about (not completing
      // an ephemeris)
   TUASSERTE(bool, true, uut.addData(msg32CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // message type 33, expect nothing
   TUASSERTE(bool, true, uut.addData(msg33CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
addDataAlmanacTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Almanac}));
      /// @note We have no GPS CNav almanac data to test with so use QZSS
      // add ephemeris 1, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg10CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__);
      // add ephemeris 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg11CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__);
      // clock data completes the ephemeris
   TUASSERTE(bool, true, uut.addData(msg30CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__);
      // nothing in message type 32 that we care about
   TUASSERTE(bool, true, uut.addData(msg32CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__);
      // message type 37, expect one almanac
   TUASSERTE(bool, true, uut.addData(msg37CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
addDataHealthTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health}));
      // add ephemeris 1, expect 3 health objects.
   TUASSERTE(bool, true, uut.addData(msg10CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 3);
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // clock data completes the ephemeris, but expect nothing
   TUASSERTE(bool, true, uut.addData(msg30CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // nothing in message type 32 that we care about
   TUASSERTE(bool, true, uut.addData(msg32CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // message type 33, expect nothing
   TUASSERTE(bool, true, uut.addData(msg33CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
addDataTimeTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::TimeOffset}));
      // add ephemeris 1, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg10CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // clock data completes the ephemeris, but expect nothing
   TUASSERTE(bool, true, uut.addData(msg30CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // nothing in message type 32 that we care about
   TUASSERTE(bool, true, uut.addData(msg32CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // message type 33, expect 1 time offset message
   TUASSERTE(bool, true, uut.addData(msg33CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
      // message type 35, expect 1 time offset message
   TUASSERTE(bool, true, uut.addData(msg35CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
addDataEphHealthTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris,
                              gnsstk::NavMessageType::Health}));
      // add ephemeris 1, expect 3 health objects.
   TUASSERTE(bool, true, uut.addData(msg10CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 3);
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // clock data completes the ephemeris
   TUASSERTE(bool, true, uut.addData(msg30CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
      // nothing in message type 32 that we care about (not completing
      // an ephemeris)
   TUASSERTE(bool, true, uut.addData(msg32CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // message type 33, expect nothing
   TUASSERTE(bool, true, uut.addData(msg33CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
addDataAlmHealthTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Almanac,
                              gnsstk::NavMessageType::Health}));
      // add ephemeris 1, expect 3 health objects.
   TUASSERTE(bool, true, uut.addData(msg10CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 3);
      // add ephemeris 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg11CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__);
      // add clock data, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg30CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__);
      // add message type 32, expect nothing.
   TUASSERTE(bool, true, uut.addData(msg32CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__);
      // add message type 37, expect 3 health objects and 1 almanac.
   TUASSERTE(bool, true, uut.addData(msg37CNAVQZSSL5, navOut));
   fc.validateResults(navOut, __LINE__, 4, 1, 0, 0, 3);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
processEphTest()
{
   TUDEF("PNBGPSCNavDataFactory", "processEph");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavMessageID nmidExpL1(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                            gnsstk::NavType::GPSLNAV),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpL2(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CM,
                            gnsstk::NavType::GPSCNAVL2),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpL5(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I,
                            gnsstk::NavType::GPSCNAVL5),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CM,
                            gnsstk::NavType::GPSCNAVL2),
      gnsstk::NavMessageType::Ephemeris);
   gnsstk::CommonTime toeExp = gnsstk::GPSWeekSecond(2060,91800.0);
   gnsstk::CommonTime topExp = gnsstk::GPSWeekSecond(2060,21600.0);
   gnsstk::CommonTime beginExp = gnsstk::GPSWeekSecond(2060, 86400.0);
   gnsstk::CommonTime endExp = gnsstk::GPSWeekSecond(2060, 97200.0);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSCNavHealth *hea;
   gnsstk::GPSCNavEph *eph;
      // Fail because we're telling processEph this is a message ID
      // 13.  No I don't think it's strictly worthwhile testing every
      // possible message type.
   TUASSERTE(bool, false, uut.processEph(13, msg10CNAVGPSL2, navOut));
      // success, health data only
   TUASSERTE(bool, true, uut.processEph(10, msg10CNAVGPSL2, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, msg10CNAVGPSL2ct, hea->timeStamp);
         bool expHealth = false;
         switch (hea->signal.nav)
         {
            case gnsstk::NavType::GPSLNAV:
               TUASSERTE(gnsstk::NavMessageID, nmidExpL1, hea->signal);
               break;
            case gnsstk::NavType::GPSCNAVL2:
               TUASSERTE(gnsstk::NavMessageID, nmidExpL2, hea->signal);
               break;
            case gnsstk::NavType::GPSCNAVL5:
               TUASSERTE(gnsstk::NavMessageID, nmidExpL5, hea->signal);
               expHealth = true;
               break;
            default:
               TUFAIL("Unexpected nav type: " +
                      gnsstk::StringUtils::asString(hea->signal.nav));
               break;
         }
            // NavHealthData has no fields
            // GPSCNavHealth
         TUASSERTE(bool, expHealth, hea->health);
      }
      else
      {
         TUFAIL("What is this?");
      }
   }
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 3);
   TUASSERTE(bool, true, uut.processEph(11, msg11CNAVGPSL2, navOut));
   fc.validateResults(navOut, __LINE__);
      // success, and we have an ephemeris.
   TUASSERTE(bool, true, uut.processEph(30, msg30CNAVGPSL2, navOut));
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gnsstk::GPSCNavEph*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, msg10CNAVGPSL2ct, eph->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, eph->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, msg10CNAVGPSL2ct, eph->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, eph->health);
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
         TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
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
         TUASSERTE(gnsstk::CommonTime, topExp, eph->top);
         TUASSERTE(gnsstk::CommonTime, msg11CNAVGPSL2ct, eph->xmit11);
         TUASSERTE(gnsstk::CommonTime, msg30CNAVGPSL2ct, eph->xmitClk);
      }
      else
      {
         TUFAIL("What is this?");
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
processAlmOrbTest()
{
   TUDEF("PNBGPSCNavDataFactory", "processAlmOrb");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavMessageID nmidExpL1(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                            gnsstk::NavType::GPSLNAV),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpL2(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CM,
                            gnsstk::NavType::GPSCNAVL2),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpL5(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I,
                            gnsstk::NavType::GPSCNAVL5),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I,
                            gnsstk::NavType::GPSCNAVL5),
      gnsstk::NavMessageType::Almanac);
   gnsstk::CommonTime toaExp = gnsstk::GPSWeekSecond(2097, 520192.0,
                                                   gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime beginExp = gnsstk::GPSWeekSecond(2097, 345642.0,
                                                     gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime endExp = gnsstk::CommonTime::END_OF_TIME;
   gnsstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.processAlmOrb(37, msg37CNAVQZSSL5, navOut));
   gnsstk::GPSCNavHealth *hea;
   gnsstk::GPSCNavAlm *alm;
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Health;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, msg37CNAVQZSSL5ct, hea->timeStamp);
         switch (hea->signal.nav)
         {
            case gnsstk::NavType::GPSLNAV:
               TUASSERTE(gnsstk::NavMessageID, nmidExpL1, hea->signal);
               break;
            case gnsstk::NavType::GPSCNAVL2:
               TUASSERTE(gnsstk::NavMessageID, nmidExpL2, hea->signal);
               break;
            case gnsstk::NavType::GPSCNAVL5:
               TUASSERTE(gnsstk::NavMessageID, nmidExpL5, hea->signal);
               break;
            default:
               TUFAIL("Unexpected nav type: " +
                      gnsstk::StringUtils::asString(hea->signal.nav));
               break;
         }
            // NavHealthData has no fields
            // GPSCNavHealth fields
         TUASSERTE(bool, false, hea->health);
      }
      else if ((alm = dynamic_cast<gnsstk::GPSCNavAlm*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Almanac;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, msg37CNAVQZSSL5ct, alm->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, msg37CNAVQZSSL5ct, alm->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toe);
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, alm->health);
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
         TUASSERTFE(-0.059633503129054299396 + 0.25*gnsstk::PI, alm->i0);
         TUASSERTFE(-1.5729055503782394254, alm->w);
         TUASSERTFE(-2.19437712E-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-0.00034046173095703125, alm->af0);
         TUASSERTFE(7.27595761E-12, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
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
      else
      {
         TUFAIL("What is this?");
      }
   }
   fc.validateResults(navOut, __LINE__, 4, 1, 0, 0, 3);
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
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.process12(12, msg12CNAVQZSSL5, navOut));
      // easy test, the data is all empty
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
process30Test()
{
   TUDEF("PNBGPSCNavDataFactory", "process30");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(ephCNAVGPSL2sid, ephCNAVGPSL2sid, oidCNAVGPSL2,
                            gnsstk::NavType::GPSCNAVL2),
      gnsstk::NavMessageType::Iono);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSCNavIono *iono = nullptr;
   gnsstk::GPSCNavISC *isc = nullptr;
   TUASSERTE(bool, true, uut.process30(msg30CNAVGPSL2, navOut));
   for (const auto& i : navOut)
   {
      if ((iono = dynamic_cast<gnsstk::GPSCNavIono*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Iono;
         TUASSERTE(gnsstk::CommonTime, msg30CNAVGPSL2ct, iono->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, iono->signal);
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
      }
      else if ((isc = dynamic_cast<gnsstk::GPSCNavISC*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::ISC;
         TUASSERTE(gnsstk::CommonTime, msg30CNAVGPSL2ct, isc->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, isc->signal);
         TUASSERTFE(5.58793545E-09, isc->isc);
         TUASSERTE(uint32_t, 0x8b, isc->pre);
         TUASSERTE(bool, false, isc->alert);
         TUASSERTFE(-3.49245965E-10, isc->iscL1CA);
         TUASSERTFE(-3.14321369E-09, isc->iscL2C);
         TUASSERTFE(6.43194653E-09, isc->iscL5I5);
         TUASSERTFE(6.54836185E-09, isc->iscL5Q5);
      }
      else
      {
         TUFAIL("What is this?");
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 0, 0, 1, 1);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
process31Test()
{
   TUDEF("PNBGPSCNavDataFactory", "process31");
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavMessageID nmidExpL1(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                            gnsstk::NavType::GPSLNAV),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpL2(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CM,
                            gnsstk::NavType::GPSCNAVL2),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpL5(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I,
                            gnsstk::NavType::GPSCNAVL5),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I,
                            gnsstk::NavType::GPSCNAVL5),
      gnsstk::NavMessageType::Almanac);
   gnsstk::CommonTime toaExp = gnsstk::GPSWeekSecond(2097, 3937.0,
                                                   gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime beginExp = msg31CNAVQZSSL5ct;
   gnsstk::CommonTime endExp = gnsstk::CommonTime::END_OF_TIME;
   gnsstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.process31(31, msg31CNAVQZSSL5, navOut));
      // 4 almanacs, 12 health
   TUASSERTE(size_t, 16, navOut.size());
   gnsstk::GPSCNavHealth *hea = nullptr;
   gnsstk::GPSCNavRedAlm *alm = nullptr;
      // Not really trying to enforce order, but this was the easiest
      // way to verify that we got all the results we're expecting.
   auto i = navOut.begin();
      // first result
   TUASSERT((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpL1, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // second
   ++i;
   TUASSERT((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpL2, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // third
   ++i;
   TUASSERT((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpL5, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // fourth
   ++i;
   TUASSERT((alm = dynamic_cast<gnsstk::GPSCNavRedAlm*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, alm->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, alm->xmitTime);
   TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toe);
   TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toc); // same value as toe
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, alm->health);
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
   TUASSERTFE((0.25+0.0056)*gnsstk::PI, alm->i0);
   TUASSERTFE(4.712388980384689674, alm->w);
   TUASSERTFE(-8.7e-10*gnsstk::PI, alm->OMEGAdot);
   TUASSERTFE(0, alm->idot);
   TUASSERTFE(0, alm->af0);
   TUASSERTFE(0, alm->af1);
   TUASSERTFE(0, alm->af2);
   TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
   TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
   TUASSERTE(uint32_t, 0x8b, alm->pre);
   TUASSERTE(bool, false, alm->alert);
   TUASSERTE(bool, false, alm->healthL1);
   TUASSERTE(bool, false, alm->healthL2);
   TUASSERTE(bool, false, alm->healthL5);
   TUASSERTFE(0.0056*gnsstk::PI, alm->deltai);
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
   TUASSERT((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpL1, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // sixth
   ++i;
   TUASSERT((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpL2, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // seventh
   ++i;
   TUASSERT((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpL5, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // eighth
   ++i;
   TUASSERT((alm = dynamic_cast<gnsstk::GPSCNavRedAlm*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, alm->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, alm->xmitTime);
   TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toe);
   TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toc); // same value as toe
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, alm->health);
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
   TUASSERTFE((0.25+0.0056)*gnsstk::PI, alm->i0);
   TUASSERTFE(4.712388980384689674, alm->w);
   TUASSERTFE(-8.7e-10*gnsstk::PI, alm->OMEGAdot);
   TUASSERTFE(0, alm->idot);
   TUASSERTFE(0, alm->af0);
   TUASSERTFE(0, alm->af1);
   TUASSERTFE(0, alm->af2);
   TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
   TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
   TUASSERTE(uint32_t, 0x8b, alm->pre);
   TUASSERTE(bool, false, alm->alert);
   TUASSERTE(bool, false, alm->healthL1);
   TUASSERTE(bool, false, alm->healthL2);
   TUASSERTE(bool, false, alm->healthL5);
   TUASSERTFE(0.0056*gnsstk::PI, alm->deltai);
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
   TUASSERT((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpL1, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // tenth
   ++i;
   TUASSERT((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpL2, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // eleventh
   ++i;
   TUASSERT((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpL5, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // twelfth
   ++i;
   TUASSERT((alm = dynamic_cast<gnsstk::GPSCNavRedAlm*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, alm->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, alm->xmitTime);
   TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toe);
   TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toc); // same value as toe
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, alm->health);
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
   TUASSERTFE((0.25+0.0056)*gnsstk::PI, alm->i0);
   TUASSERTFE(4.712388980384689674, alm->w);
   TUASSERTFE(-8.7e-10*gnsstk::PI, alm->OMEGAdot);
   TUASSERTFE(0, alm->idot);
   TUASSERTFE(0, alm->af0);
   TUASSERTFE(0, alm->af1);
   TUASSERTFE(0, alm->af2);
   TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
   TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
   TUASSERTE(uint32_t, 0x8b, alm->pre);
   TUASSERTE(bool, false, alm->alert);
   TUASSERTE(bool, false, alm->healthL1);
   TUASSERTE(bool, false, alm->healthL2);
   TUASSERTE(bool, false, alm->healthL5);
   TUASSERTFE(0.0056*gnsstk::PI, alm->deltai);
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
   TUASSERT((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpL1, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // fourteenth
   ++i;
   TUASSERT((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpL2, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // fifteenth
   ++i;
   TUASSERT((hea = dynamic_cast<gnsstk::GPSCNavHealth*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpL5, hea->signal);
   TUASSERTE(bool, false, hea->health);
   hea = nullptr;
   alm = nullptr;
      // sixteenth
   ++i;
   TUASSERT((alm = dynamic_cast<gnsstk::GPSCNavRedAlm*>(i->get())) != nullptr);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, alm->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
   TUASSERTE(gnsstk::CommonTime, msg31CNAVQZSSL5ct, alm->xmitTime);
   TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toe);
   TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toc); // same value as toe
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, alm->health);
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
   TUASSERTFE((0.25+0.0056)*gnsstk::PI, alm->i0);
   TUASSERTFE(4.712388980384689674, alm->w);
   TUASSERTFE(-8.7e-10*gnsstk::PI, alm->OMEGAdot);
   TUASSERTFE(0, alm->idot);
   TUASSERTFE(0, alm->af0);
   TUASSERTFE(0, alm->af1);
   TUASSERTFE(0, alm->af2);
   TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
   TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
   TUASSERTE(uint32_t, 0x8b, alm->pre);
   TUASSERTE(bool, false, alm->alert);
   TUASSERTE(bool, false, alm->healthL1);
   TUASSERTE(bool, false, alm->healthL2);
   TUASSERTE(bool, false, alm->healthL5);
   TUASSERTFE(0.0056*gnsstk::PI, alm->deltai);
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
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CM,
                            gnsstk::NavType::GPSCNAVL2),
      gnsstk::NavMessageType::TimeOffset);
   gnsstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.process33(msg33CNAVGPSL2, navOut));
   TUASSERTE(size_t, 1, navOut.size());
      // Yes this code can cause seg faults on failure, but that's ok.
   gnsstk::GPSCNavTimeOffset *to =
      dynamic_cast<gnsstk::GPSCNavTimeOffset*>(navOut.begin()->get());
   TUASSERT(to != nullptr);
      // NavData fields
   TUASSERTE(gnsstk::CommonTime, msg33CNAVGPSL2ct, to->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
      // TimeOffsetData has no fields
      // GPSCNavTimeOffset fields
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC, to->tgt);
   TUASSERTFE(1.9790604711E-09, to->a0);
   TUASSERTFE(7.5495165675E-15, to->a1);
   TUASSERTFE(0.0000000000E+00, to->a2);
   TUASSERTFE(18, to->deltatLS);
   TUASSERTFE(21600, to->tot);
   TUASSERTE(unsigned, 2060, to->wnot);
   TUASSERTE(unsigned, 1929, to->wnLSF);
   TUASSERTE(gnsstk::CommonTime,
             gnsstk::GPSWeekSecond(2060,21600).convertToCommonTime(),
             to->refTime);
   TUASSERTE(unsigned, 7, to->dn);
   TUASSERTFE(18, to->deltatLSF);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
process35Test()
{
   TUDEF("PNBGPSCNavDataFactory", "process35");
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I,
                            gnsstk::NavType::GPSCNAVL5),
      gnsstk::NavMessageType::TimeOffset);
   gnsstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.process35(msg35CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 1, navOut.size());
      // Yes this code can cause seg faults on failure, but that's ok.
   gnsstk::GPSCNavTimeOffset *to =
      dynamic_cast<gnsstk::GPSCNavTimeOffset*>(navOut.begin()->get());
   TUASSERT(to != nullptr);
      // NavData fields
   TUASSERTE(gnsstk::CommonTime, msg35CNAVQZSSL5ct, to->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
      // TimeOffsetData has no fields
      // GPSCNavTimeOffset fields
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::QZS, to->tgt);
   TUASSERTFE(0, to->a0);
   TUASSERTFE(0, to->a1);
   TUASSERTFE(0, to->a2);
   TUASSERTFE(0, to->deltatLS);
   TUASSERTFE(356400, to->tot);
   TUASSERTE(unsigned, 2097, to->wnot);
   TUASSERTE(unsigned, 0, to->wnLSF);
   TUASSERTE(gnsstk::CommonTime,
             gnsstk::GPSWeekSecond(
                2097,356400,gnsstk::TimeSystem::QZS).convertToCommonTime(),
             to->refTime);
   TUASSERTE(unsigned, 0, to->dn);
   TUASSERTFE(0, to->deltatLSF);
   TURETURN();
}


unsigned PNBGPSCNavDataFactory_T ::
processEphWRTest()
{
   TUDEF("PNBGPSCNavDataFactory", "processEph");
   gnsstk::PNBGPSCNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSCNavEph *eph;
   gnsstk::CommonTime toeExp(gnsstk::GPSWeekSecond(2220,5400));
   gnsstk::CommonTime topExp(gnsstk::GPSWeekSecond(2219,527400));
   gnsstk::CommonTime beginExp(gnsstk::GPSWeekSecond(2220,0));
   gnsstk::CommonTime endExp(gnsstk::GPSWeekSecond(2220,10800));
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris}));
   TUASSERTE(bool, true, uut.addData(msg10CNAVWR, navOut));
   TUASSERTE(gnsstk::NavDataPtrList::size_type, 0, navOut.size());
   TUASSERTE(bool, true, uut.addData(msg11CNAVWR, navOut));
   TUASSERTE(gnsstk::NavDataPtrList::size_type, 0, navOut.size());
   TUASSERTE(bool, true, uut.addData(msg30CNAVWR, navOut));
   TUASSERTE(gnsstk::NavDataPtrList::size_type, 1, navOut.size());
   if (navOut.size() >= 1)
   {
      if ((eph = dynamic_cast<gnsstk::GPSCNavEph*>(navOut.begin()->get()))
          != nullptr)
      {
         TUASSERTE(gnsstk::CommonTime, msg10CNAVWRct, eph->timeStamp);
         TUASSERTE(gnsstk::CommonTime, msg10CNAVWRct, eph->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toc);
         TUASSERTE(gnsstk::CommonTime, topExp, eph->top);
         TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
         TUASSERTE(gnsstk::CommonTime, msg11CNAVWRct, eph->xmit11);
         TUASSERTE(gnsstk::CommonTime, msg30CNAVWRct, eph->xmitClk);
      }
   }
   TURETURN();
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
   errorTotal += testClass.processEphWRTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
