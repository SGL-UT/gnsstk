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
#include "FactoryCounter.hpp"
#include "PNBGalINavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GalINavTimeOffset.hpp"
#include "GalINavHealth.hpp"
#include "GalINavEph.hpp"
#include "GalINavAlm.hpp"
#include "GalINavIono.hpp"
#include "GalINavISC.hpp"
#include "GALWeekSecond.hpp"
#include "TimeString.hpp"

using namespace std;

// avoid having to type out template params over and over.
using GalFactoryCounter = FactoryCounter<gpstk::GalINavAlm,gpstk::GalINavEph,gpstk::GalINavTimeOffset,gpstk::GalINavHealth,gpstk::GalINavIono,gpstk::GalINavISC>;
namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, GalHealthStatus e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, GalDataValid e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class PNBGalINavDataFactory_T
{
public:
   PNBGalINavDataFactory_T();

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
      /// Test processAlm method, and implicitly, processAlmOrb
   unsigned processAlmTest();
      /// Test processOffset method
   unsigned processOffsetTest();

#include "GalINavTestDataDecl.hpp"
};

PNBGalINavDataFactory_T ::
PNBGalINavDataFactory_T()
{
#include "GalINavTestDataDef.hpp"
}


unsigned PNBGalINavDataFactory_T ::
addDataAllTest()
{
   TUDEF("PNBGalINavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gpstk::PNBGalINavDataFactory uut;
   gpstk::SatID gloSid(1,gpstk::SatelliteSystem::Glonass);
   gpstk::ObsID gloid(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::G1,
                      gpstk::TrackingCode::Standard);
   gpstk::NavID gloNav(gpstk::NavType::GloCivilF);
   gpstk::PackedNavBitsPtr nonGPS =
      std::make_shared<gpstk::PackedNavBits>(gloSid,gloid,gloNav,"XX",
                                             ephINAVGalWT1ct);
   gpstk::NavDataPtrList navOut;
      // should refuse non-GPS data
   TUASSERTE(bool, false, uut.addData(nonGPS, navOut));
      // The rest is checking that we're processing the good data
      // appropriately, but I'm not bothering to check the detailed
      // contents as that is assumed to be tested in the process*
      // tests.
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT6, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT7, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT8, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT5, navOut));
   fc.validateResults(navOut, __LINE__, 5, 0, 1, 0, 2, 1, 1);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT9, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT10, navOut));
      // 2 alms/4 hea instead of 3/6 because one of the almanacs is empty
   fc.validateResults(navOut, __LINE__, 7, 2, 0, 1, 4);
   TURETURN();
}


unsigned PNBGalINavDataFactory_T ::
addDataEphemerisTest()
{
   TUDEF("PNBGalINavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gpstk::PNBGalINavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Ephemeris}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT6, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT7, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT8, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT5, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT9, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT10, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGalINavDataFactory_T ::
addDataAlmanacTest()
{
   TUDEF("PNBGalINavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gpstk::PNBGalINavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Almanac}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT6, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT7, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT8, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT5, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT9, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT10, navOut));
      // 2 alms instead of 3 because one of the almanacs is empty
   fc.validateResults(navOut, __LINE__, 2, 2);
   TURETURN();
}


unsigned PNBGalINavDataFactory_T ::
addDataHealthTest()
{
   TUDEF("PNBGalINavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gpstk::PNBGalINavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Health}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT6, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT7, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT8, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT5, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 0, 2);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT9, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT10, navOut));
      // 4 hea instead of 6 because one of the almanacs is empty
   fc.validateResults(navOut, __LINE__, 4, 0, 0, 0, 4);
   TURETURN();
}


unsigned PNBGalINavDataFactory_T ::
addDataTimeTest()
{
   TUDEF("PNBGalINavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gpstk::PNBGalINavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::TimeOffset}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT6, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT7, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT8, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT5, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT9, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT10, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
   TURETURN();
}


unsigned PNBGalINavDataFactory_T ::
addDataEphHealthTest()
{
   TUDEF("PNBGalINavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gpstk::PNBGalINavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Health,
                              gpstk::NavMessageType::Ephemeris}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT6, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT7, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT8, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT5, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 1, 0, 2);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT9, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT10, navOut));
      // 4 hea instead of 6 because one of the almanacs is empty
   fc.validateResults(navOut, __LINE__, 4, 0, 0, 0, 4);
   TURETURN();
}


unsigned PNBGalINavDataFactory_T ::
addDataAlmHealthTest()
{
   TUDEF("PNBGalINavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gpstk::PNBGalINavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Health,
                              gpstk::NavMessageType::Almanac}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT6, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT7, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT8, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalWT5, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 0, 2);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT9, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navINAVGalWT10, navOut));
      // 2 alms/4 hea instead of 3/6 because one of the almanacs is empty
   fc.validateResults(navOut, __LINE__, 6, 2, 0, 0, 4);
   TURETURN();
}


unsigned PNBGalINavDataFactory_T ::
processEphTest()
{
   TUDEF("PNBGalINavDataFactory", "processEph");
   GalFactoryCounter fc(testFramework);
   gpstk::PNBGalINavDataFactory uut;
   gpstk::NavMessageID nmidExpE1B(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::Galileo,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::E1B,
                            gpstk::NavType::GalINAV),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExpE5b(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::Galileo,
                            gpstk::CarrierBand::E5b, gpstk::TrackingCode::E5bI,
                            gpstk::NavType::GalINAV),
      gpstk::NavMessageType::Health);
   gpstk::CommonTime toeExp = gpstk::GALWeekSecond(1014,517200.0);
   gpstk::CommonTime tocExp = gpstk::GALWeekSecond(1014,517200.0);
   gpstk::CommonTime beginExp = ephINAVGalWT1ct;
   gpstk::CommonTime endExp = toeExp + (4.0 * 3600.0);
   gpstk::NavDataPtrList navOut;
   gpstk::GalINavEph *eph;
   gpstk::GalINavHealth *hea;
   gpstk::GalINavIono *iono;
   gpstk::GalINavISC *isc;
   TUASSERTE(bool, true, uut.processEph(1, ephINAVGalWT1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(2, ephINAVGalWT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(3, ephINAVGalWT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(4, ephINAVGalWT4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(5, ephINAVGalWT5, navOut));
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gpstk::GalINavEph*>(i.get())) != nullptr)
      {
         nmidExpE1B.messageType = gpstk::NavMessageType::Ephemeris;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, ephINAVGalWT1ct, eph->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExpE1B, eph->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gpstk::CommonTime, ephINAVGalWT1ct, eph->xmitTime);
         TUASSERTE(gpstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(gpstk::CommonTime, tocExp, eph->Toc);
         TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, eph->health);
         TUASSERTFE(4.610046744347E-06, eph->Cuc);
         TUASSERTFE(1.211278140545E-05, eph->Cus);
         TUASSERTFE(9.634375000000E+01, eph->Crc);
         TUASSERTFE(9.828125000000E+01, eph->Crs);
         TUASSERTFE(-3.725290298462E-09, eph->Cis);
         TUASSERTFE(-1.490116119385E-08, eph->Cic);
         TUASSERTFE(-3.0165642165792534435E+00, eph->M0);
         TUASSERTFE(2.529748231311E-09, eph->dn);
         TUASSERTFE(0, eph->dndot);
         TUASSERTFE(1.100952504203E-04, eph->ecc);
         TUASSERTFE(5.4406226139068603516E+03, eph->Ahalf);
         TUASSERTFE(2.9600374426954716444E+07, eph->A);
         TUASSERTFE(0, eph->Adot);
         TUASSERTFE(2.9631768162768818975E+00, eph->OMEGA0);
         TUASSERTFE(9.8982515960810546485E-01, eph->i0);
         TUASSERTFE(1.4027684908239376749E-01, eph->w);
         TUASSERTFE(-5.110570018612E-09, eph->OMEGAdot);
         TUASSERTFE(-1.832219176426E-10, eph->idot);
         TUASSERTFE(-5.335179739632E-04, eph->af0);
         TUASSERTFE(-8.100187187665E-12, eph->af1);
         TUASSERTFE(0, eph->af2);
         TUASSERTE(gpstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gpstk::CommonTime, endExp, eph->endFit);
            // GalINavEph
         TUASSERTFE(-4.656613E-09, eph->bgdE5aE1);
         TUASSERTFE(-5.122274E-09, eph->bgdE5bE1);
         TUASSERTE(unsigned, 107, eph->sisaIndex);
         TUASSERTE(unsigned, 1, eph->svid);
         TUASSERTE(gpstk::CommonTime, ephINAVGalWT2ct, eph->xmit2);
         TUASSERTE(gpstk::CommonTime, ephINAVGalWT3ct, eph->xmit3);
         TUASSERTE(gpstk::CommonTime, ephINAVGalWT4ct, eph->xmit4);
         TUASSERTE(gpstk::CommonTime, ephINAVGalWT5ct, eph->xmit5);
         TUASSERTE(uint16_t, 94, eph->iodnav1);
         TUASSERTE(uint16_t, 94, eph->iodnav2);
         TUASSERTE(uint16_t, 94, eph->iodnav3);
         TUASSERTE(uint16_t, 94, eph->iodnav4);
         TUASSERTE(gpstk::GalHealthStatus, gpstk::GalHealthStatus::OK,
                   eph->hsE5b);
         TUASSERTE(gpstk::GalHealthStatus, gpstk::GalHealthStatus::OK,
                   eph->hsE1B);
         TUASSERTE(gpstk::GalDataValid, gpstk::GalDataValid::Valid,
                   eph->dvsE5b);
         TUASSERTE(gpstk::GalDataValid, gpstk::GalDataValid::Valid,
                   eph->dvsE1B);
      }
      else if ((hea = dynamic_cast<gpstk::GalINavHealth*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gpstk::CommonTime, ephINAVGalWT5ct, hea->timeStamp);
         if (hea->signal.obs.band == gpstk::CarrierBand::L1)
         {
            nmidExpE1B.messageType = gpstk::NavMessageType::Health;
            TUASSERTE(gpstk::NavMessageID, nmidExpE1B, hea->signal);
         }
         else
         {
            TUASSERTE(gpstk::NavMessageID, nmidExpE5b, hea->signal);
         }
            // NavHealthData has no fields
            // GalINavHealth
            /** @todo while our assertions are the same for both
             * signals, that's not a guarantee, operationally.
             * Probably should add a test where the health status is
             * different between E5b and E1B. */
         TUASSERTE(gpstk::GalHealthStatus, gpstk::GalHealthStatus::OK,
                   hea->sigHealthStatus);
         TUASSERTE(gpstk::GalDataValid, gpstk::GalDataValid::Valid,
                   hea->dataValidityStatus);
         TUASSERTE(unsigned, 107, hea->sisaIndex);
      }
      else if ((iono = dynamic_cast<gpstk::GalINavIono*>(i.get())) != nullptr)
      {
         nmidExpE1B.messageType = gpstk::NavMessageType::Iono;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, ephINAVGalWT5ct, iono->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExpE1B, iono->signal);
            // NeQuickIonoData fields
            // values confirmed by hand.
         TUASSERTFE(45.75, iono->ai[0]);
         TUASSERTFE(0.1640625, iono->ai[1]);
         TUASSERTFE(0.00067138671875, iono->ai[2]);
         TUASSERTE(bool, false, iono->idf[0]);
         TUASSERTE(bool, false, iono->idf[1]);
         TUASSERTE(bool, false, iono->idf[2]);
         TUASSERTE(bool, false, iono->idf[3]);
         TUASSERTE(bool, false, iono->idf[4]);
      }
      else if ((isc = dynamic_cast<gpstk::GalINavISC*>(i.get())) != nullptr)
      {
         nmidExpE1B.messageType = gpstk::NavMessageType::ISC;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, ephINAVGalWT5ct, isc->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExpE1B, isc->signal);
         TUASSERTE(bool, true, std::isnan(isc->isc));
         TUASSERTFE(-4.656613E-09, isc->bgdE1E5a);
         TUASSERTFE(-5.122274E-09, isc->bgdE1E5b);
      }
   }
   fc.validateResults(navOut, __LINE__, 5, 0, 1, 0, 2, 1, 1);
   TURETURN();
}


unsigned PNBGalINavDataFactory_T ::
processAlmTest()
{
   TUDEF("PNBGalINavDataFactory", "processAlm");
   GalFactoryCounter fc(testFramework);
   gpstk::PNBGalINavDataFactory uut;
   gpstk::NavMessageID nmidExpE1B(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::Galileo,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::E1B,
                            gpstk::NavType::GalINAV),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExpE5b(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::Galileo,
                            gpstk::CarrierBand::E5b, gpstk::TrackingCode::E5bI,
                            gpstk::NavType::GalINAV),
      gpstk::NavMessageType::Health);
   gpstk::CommonTime toaExp = gpstk::GALWeekSecond(1014,517200.0);
   gpstk::CommonTime beginExp = navINAVGalWT7ct;
   gpstk::CommonTime endExp = toaExp + (74.0 * 3600.0);
   gpstk::CommonTime expRefTime = gpstk::GALWeekSecond(1014,518400);
   gpstk::NavDataPtrList navOut;
   gpstk::GalINavAlm *alm;
   gpstk::GalINavHealth *hea;
   gpstk::GalINavTimeOffset *tim;
   TUASSERTE(bool, true, uut.processAlm(7, navINAVGalWT7, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processAlm(8, navINAVGalWT8, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processAlm(9, navINAVGalWT9, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processAlm(10, navINAVGalWT10, navOut));
   for (const auto& i : navOut)
   {
      if ((alm = dynamic_cast<gpstk::GalINavAlm*>(i.get())) != nullptr)
      {
         gpstk::NavMessageID nmid(nmidExpE1B);
         if (alm->signal.sat.id == 19)
         {
            nmid.sat.id = 19;
            TUASSERTE(gpstk::CommonTime, navINAVGalWT7ct, alm->timeStamp);
            TUASSERTE(gpstk::CommonTime, navINAVGalWT7ct, alm->xmitTime);
            TUASSERTFE(2.9141800017857466543E+00, alm->M0);
            TUASSERTFE(1.068115234375E-04, alm->ecc);
            TUASSERTFE(5440.6116409941769234, alm->Ahalf);
            TUASSERTFE(29600255.028121352196, alm->A);
            TUASSERTFE(8.669867665531157952E-01, alm->OMEGA0);
            TUASSERTFE(0.95840136886673976679, alm->i0);
            TUASSERTFE(2.6636617643641731235E+00, alm->w);
            TUASSERTFE(-5.48594280E-09, alm->OMEGAdot);
            TUASSERTFE(-9.5367431640625e-06, alm->af0);
            TUASSERTFE(0, alm->af1);
            TUASSERTE(gpstk::CommonTime, navINAVGalWT7ct, alm->beginFit);
            TUASSERTE(gpstk::CommonTime, endExp, alm->endFit);
               // GalINavAlm
            TUASSERTE(gpstk::CommonTime, navINAVGalWT8ct, alm->xmit2);
            TUASSERTFE(2.34375000E-02, alm->dAhalf);
            TUASSERTFE(-1.8983012250084810996E-02, alm->deltai);
         }
         else
         {
            nmid.sat.id = 21;
            TUASSERTE(gpstk::CommonTime, navINAVGalWT9ct, alm->timeStamp);
            TUASSERTE(gpstk::CommonTime, navINAVGalWT9ct, alm->xmitTime);
            TUASSERTFE(-2.8944299991417192786E+00, alm->M0);
            TUASSERTFE(1.068115234375E-04, alm->ecc);
            TUASSERTFE(5440.6350784941769234, alm->Ahalf);
            TUASSERTFE(29600510.057341337204, alm->A);
            TUASSERTFE(2.9605829206192875169E+00, alm->OMEGA0);
            TUASSERTFE(0.98946447982142404776, alm->i0);
            TUASSERTFE(8.1847462413623239819E-01, alm->w);
            TUASSERTFE(-5.12021328E-09, alm->OMEGAdot);
            TUASSERTFE(-0.0005168914794921875, alm->af0);
            TUASSERTFE(-3.6379788070917129517e-12, alm->af1);
            TUASSERTE(gpstk::CommonTime, navINAVGalWT9ct, alm->beginFit);
            TUASSERTE(gpstk::CommonTime, endExp, alm->endFit);
               // GalINavAlm
            TUASSERTE(gpstk::CommonTime, navINAVGalWT10ct, alm->xmit2);
            TUASSERTFE(4.68750000E-02, alm->dAhalf);
            TUASSERTFE(1.2080098704599424864E-02, alm->deltai);
         }
         nmid.messageType = gpstk::NavMessageType::Almanac;
            // NavData fields
         TUASSERTE(gpstk::NavMessageID, nmid, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gpstk::CommonTime, toaExp, alm->Toe);
         TUASSERTE(gpstk::CommonTime, toaExp, alm->Toc);
         TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, alm->health);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(unsigned, 1014, alm->wna);
         TUASSERTFE(517200, alm->t0a);
         TUASSERTE(unsigned, 14, alm->ioda1);
         TUASSERTE(unsigned, 14, alm->ioda2);
         TUASSERTE(gpstk::GalHealthStatus, gpstk::GalHealthStatus::OK,
                   alm->hsE5b);
         TUASSERTE(gpstk::GalHealthStatus, gpstk::GalHealthStatus::OK,
                   alm->hsE1B);
      }
      else if ((hea = dynamic_cast<gpstk::GalINavHealth*>(i.get())) != nullptr)
      {
         gpstk::NavMessageID nmid;
         if (hea->signal.obs.band == gpstk::CarrierBand::L1)
         {
            nmid = nmidExpE1B;
            nmid.messageType = gpstk::NavMessageType::Health;
         }
         else
         {
            nmid = nmidExpE5b;
            nmid.messageType = gpstk::NavMessageType::Health;
         }
         if (hea->signal.sat.id == 19)
         {
            nmid.sat.id = 19;
            TUASSERTE(gpstk::CommonTime, navINAVGalWT8ct, hea->timeStamp);
         }
         else
         {
            nmid.sat.id = 21;
            TUASSERTE(gpstk::CommonTime, navINAVGalWT10ct, hea->timeStamp);
         }
         TUASSERTE(gpstk::GalHealthStatus, gpstk::GalHealthStatus::OK,
                   hea->sigHealthStatus);
         TUASSERTE(gpstk::GalDataValid, gpstk::GalDataValid::Unknown,
                   hea->dataValidityStatus);
         TUASSERTE(unsigned, 255, hea->sisaIndex);
      }
      else if ((tim = dynamic_cast<gpstk::GalINavTimeOffset*>(i.get()))
               != nullptr)
      {
         nmidExpE1B.messageType = gpstk::NavMessageType::TimeOffset;
         TUASSERTE(gpstk::CommonTime, navINAVGalWT10ct, tim->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExpE1B, tim->signal);
            /** @warning these truth values have not been vetted due
             * to a lack of an existing tool to print the information
             * (the old implementation only handles the data in
             * WT6). */
            // StdNavTimeOffset
         TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::GAL, tim->src);
         TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::GPS, tim->tgt);
         TUASSERTFE(-6.3155312091112136841e-09, tim->a0);
         TUASSERTFE(-6.6613381477509392425e-15, tim->a1);
         TUASSERTFE(0, tim->a2);
         TUASSERTFE(0, tim->deltatLS);
         TUASSERTE(gpstk::CommonTime, expRefTime, tim->refTime);
         TUASSERTFE(518400, tim->tot);
         TUASSERTE(unsigned, 54, tim->wnot);
         TUASSERTE(unsigned, 0, tim->wnLSF);
         TUASSERTE(unsigned, 0, tim->dn);
         TUASSERTFE(0, tim->deltatLSF);
               // GalINavTimeOffset
         TUASSERTFE(0, tim->tow);
      }
   }
   fc.validateResults(navOut, __LINE__, 7, 2, 0, 1, 4);
   TURETURN();
}


unsigned PNBGalINavDataFactory_T ::
processOffsetTest()
{
   TUDEF("PNBGalINavDataFactory", "processOffset");
   GalFactoryCounter fc(testFramework);
   gpstk::PNBGalINavDataFactory uut;
   gpstk::NavMessageID nmidExpE1B(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::Galileo,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::E1B,
                            gpstk::NavType::GalINAV),
      gpstk::NavMessageType::TimeOffset);
   gpstk::NavDataPtrList navOut;
   gpstk::GalINavTimeOffset *tim;
   gpstk::CommonTime expRefTime(gpstk::GALWeekSecond(1014,432000));
   TUASSERTE(bool, true, uut.processOffset(navINAVGalWT6, navOut));
   for (const auto& i : navOut)
   {
      if ((tim = dynamic_cast<gpstk::GalINavTimeOffset*>(i.get()))
               != nullptr)
      {
         TUASSERTE(gpstk::CommonTime, navINAVGalWT6ct, tim->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExpE1B, tim->signal);
            // StdNavTimeOffset
         TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::GAL, tim->src);
         TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::UTC, tim->tgt);
         TUASSERTFE(-9.31322575E-10, tim->a0);
         TUASSERTFE( 8.88178420E-16, tim->a1);
         TUASSERTFE( 0.00000000E+00, tim->a2);
         TUASSERTFE(18, tim->deltatLS);
         TUASSERTE(gpstk::CommonTime, expRefTime, tim->refTime);
         TUASSERTFE(432000, tim->tot);
         TUASSERTE(unsigned, 1014, tim->wnot);
         TUASSERTE(unsigned, 905, tim->wnLSF);
         TUASSERTE(unsigned, 7, tim->dn);
         TUASSERTFE(18, tim->deltatLSF);
               // GalINavTimeOffset
         TUASSERTFE(518405, tim->tow);
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
   TURETURN();
}


int main()
{
   PNBGalINavDataFactory_T testClass;
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
   errorTotal += testClass.processOffsetTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
