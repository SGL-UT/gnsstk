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
#include "DebugTrace.hpp"

using namespace std;

// avoid having to type out template params over and over.
using GalFactoryCounter = FactoryCounter<gnsstk::GalINavAlm,gnsstk::GalINavEph,gnsstk::GalINavTimeOffset,gnsstk::GalINavHealth,gnsstk::GalINavIono,gnsstk::GalINavISC>;

namespace gnsstk
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
      // Test addData with page pairs of ephemeris
   unsigned addDataPagePairsTest();
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
      // week rollover test for decoding ephemerides
   unsigned processEphWRTest();
      // A common set of checks between processEphTest and addDataPagePairsTest
   void checkNav(gnsstk::TestUtil& testFramework, 
                 gnsstk::NavDataPtrList& navOut,
                 gnsstk::NavMessageID&, gnsstk::NavMessageID&,
                 gnsstk::CommonTime&, gnsstk::CommonTime&,
                 gnsstk::CommonTime&, gnsstk::CommonTime&); 

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
   gnsstk::PNBGalINavDataFactory uut;
   gnsstk::SatID gloSid(1,gnsstk::SatelliteSystem::Glonass);
   gnsstk::ObsID gloid(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::G1,
                      gnsstk::TrackingCode::Standard);
   gnsstk::NavID gloNav(gnsstk::NavType::GloCivilF);
   gnsstk::PackedNavBitsPtr nonGPS =
      std::make_shared<gnsstk::PackedNavBits>(gloSid,gloid,gloNav,"XX",
                                             ephINAVGalWT1ct);
   gnsstk::NavDataPtrList navOut;
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
   gnsstk::PNBGalINavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris}));
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
   gnsstk::PNBGalINavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Almanac}));
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
   gnsstk::PNBGalINavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health}));
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
   gnsstk::PNBGalINavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::TimeOffset}));
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
   gnsstk::PNBGalINavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health,
                              gnsstk::NavMessageType::Ephemeris}));
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
   gnsstk::PNBGalINavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health,
                              gnsstk::NavMessageType::Almanac}));
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

void PNBGalINavDataFactory_T ::
checkNav(gnsstk::TestUtil& testFramework, gnsstk::NavDataPtrList& navOut,
         gnsstk::NavMessageID& nmidExpE5b, gnsstk::NavMessageID& nmidExpE1B,
         gnsstk::CommonTime& toeExp, gnsstk::CommonTime& tocExp,
         gnsstk::CommonTime& beginExp, gnsstk::CommonTime& endExp)
{
   gnsstk::GalINavEph *eph;
   gnsstk::GalINavHealth *hea;
   gnsstk::GalINavIono *iono;
   gnsstk::GalINavISC *isc;
   
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gnsstk::GalINavEph*>(i.get())) != nullptr)
      {
         nmidExpE1B.messageType = gnsstk::NavMessageType::Ephemeris;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephINAVGalWT1ct, eph->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExpE1B, eph->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, ephINAVGalWT1ct, eph->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(gnsstk::CommonTime, tocExp, eph->Toc);
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, eph->health);
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
         TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
            // GalINavEph
         TUASSERTFE(-4.656613E-09, eph->bgdE5aE1);
         TUASSERTFE(-5.122274E-09, eph->bgdE5bE1);
         TUASSERTE(unsigned, 107, eph->sisaIndex);
         TUASSERTE(unsigned, 1, eph->svid);
         TUASSERTE(gnsstk::CommonTime, ephINAVGalWT2ct, eph->xmit2);
         TUASSERTE(gnsstk::CommonTime, ephINAVGalWT3ct, eph->xmit3);
         TUASSERTE(gnsstk::CommonTime, ephINAVGalWT4ct, eph->xmit4);
         TUASSERTE(gnsstk::CommonTime, ephINAVGalWT5ct, eph->xmit5);
         TUASSERTE(uint16_t, 94, eph->iodnav1);
         TUASSERTE(uint16_t, 94, eph->iodnav2);
         TUASSERTE(uint16_t, 94, eph->iodnav3);
         TUASSERTE(uint16_t, 94, eph->iodnav4);
         TUASSERTE(gnsstk::GalHealthStatus, gnsstk::GalHealthStatus::OK,
                   eph->hsE5b);
         TUASSERTE(gnsstk::GalHealthStatus, gnsstk::GalHealthStatus::OK,
                   eph->hsE1B);
         TUASSERTE(gnsstk::GalDataValid, gnsstk::GalDataValid::Valid,
                   eph->dvsE5b);
         TUASSERTE(gnsstk::GalDataValid, gnsstk::GalDataValid::Valid,
                   eph->dvsE1B);
      }
      else if ((hea = dynamic_cast<gnsstk::GalINavHealth*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephINAVGalWT5ct, hea->timeStamp);
         if (hea->signal.obs.band == gnsstk::CarrierBand::L1)
         {
            nmidExpE1B.messageType = gnsstk::NavMessageType::Health;
            TUASSERTE(gnsstk::NavMessageID, nmidExpE1B, hea->signal);
         }
         else
         {
            TUASSERTE(gnsstk::NavMessageID, nmidExpE5b, hea->signal);
         }
            // NavHealthData has no fields
            // GalINavHealth
            /** @todo while our assertions are the same for both
             * signals, that's not a guarantee, operationally.
             * Probably should add a test where the health status is
             * different between E5b and E1B. */
         TUASSERTE(gnsstk::GalHealthStatus, gnsstk::GalHealthStatus::OK,
                   hea->sigHealthStatus);
         TUASSERTE(gnsstk::GalDataValid, gnsstk::GalDataValid::Valid,
                   hea->dataValidityStatus);
         TUASSERTE(unsigned, 107, hea->sisaIndex);
      }
      else if ((iono = dynamic_cast<gnsstk::GalINavIono*>(i.get())) != nullptr)
      {
         nmidExpE1B.messageType = gnsstk::NavMessageType::Iono;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephINAVGalWT5ct, iono->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExpE1B, iono->signal);
            // NeQuickIonoNavData fields
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
      else if ((isc = dynamic_cast<gnsstk::GalINavISC*>(i.get())) != nullptr)
      {
         nmidExpE1B.messageType = gnsstk::NavMessageType::ISC;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephINAVGalWT5ct, isc->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExpE1B, isc->signal);
         TUASSERTE(bool, true, std::isnan(isc->isc));
         TUASSERTFE(-4.656613E-09, isc->bgdE1E5a);
         TUASSERTFE(-5.122274E-09, isc->bgdE1E5b);
      }
   }

}


unsigned PNBGalINavDataFactory_T ::
processEphTest()
{
   TUDEF("PNBGalINavDataFactory", "processEph");
   GalFactoryCounter fc(testFramework);
   gnsstk::PNBGalINavDataFactory uut;
   gnsstk::NavMessageID nmidExpE1B(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Galileo,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::E1B,
                            gnsstk::NavType::GalINAV),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpE5b(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Galileo,
                            gnsstk::CarrierBand::E5b, gnsstk::TrackingCode::E5bI,
                            gnsstk::NavType::GalINAV),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime toeExp = gnsstk::GALWeekSecond(1014,517200.0);
   gnsstk::CommonTime tocExp = gnsstk::GALWeekSecond(1014,517200.0);
   gnsstk::CommonTime beginExp = ephINAVGalWT1ct;
   gnsstk::CommonTime endExp = toeExp + (4.0 * 3600.0);
   gnsstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.processEph(1, ephINAVGalWT1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(2, ephINAVGalWT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(3, ephINAVGalWT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(4, ephINAVGalWT4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(5, ephINAVGalWT5, navOut));
   checkNav(testFramework, navOut, nmidExpE5b, nmidExpE1B, toeExp, tocExp, 
            beginExp, endExp);
   fc.validateResults(navOut, __LINE__, 5, 0, 1, 0, 2, 1, 1);
   TURETURN();
}


unsigned PNBGalINavDataFactory_T ::
addDataPagePairsTest()
{
   TUDEF("PNBGalINavDataFactory", "addData(pagePairs)");
   GalFactoryCounter fc(testFramework);
   gnsstk::PNBGalINavDataFactory uut;
   gnsstk::NavMessageID nmidExpE1B(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Galileo,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::E1B,
                            gnsstk::NavType::GalINAV),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpE5b(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Galileo,
                            gnsstk::CarrierBand::E5b, gnsstk::TrackingCode::E5bI,
                            gnsstk::NavType::GalINAV),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime toeExp = gnsstk::GALWeekSecond(1014,517200.0);
   gnsstk::CommonTime tocExp = gnsstk::GALWeekSecond(1014,517200.0);
   gnsstk::CommonTime beginExp = ephINAVGalWT1ct;
   gnsstk::CommonTime endExp = toeExp + (4.0 * 3600.0);
   gnsstk::NavDataPtrList navOut;
   DEBUGTRACE_ENABLE();
   TUASSERTE(bool, true, uut.addData(ephINAVGalPP1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalPP2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalPP3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalPP4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(ephINAVGalPP5, navOut));
   checkNav(testFramework, navOut, nmidExpE5b, nmidExpE1B, toeExp, tocExp, 
            beginExp, endExp); 
   fc.validateResults(navOut, __LINE__, 5, 0, 1, 0, 2, 1, 1);
   TURETURN();
}


unsigned PNBGalINavDataFactory_T ::
processAlmTest()
{
   TUDEF("PNBGalINavDataFactory", "processAlm");
   GalFactoryCounter fc(testFramework);
   gnsstk::PNBGalINavDataFactory uut;
   gnsstk::NavMessageID nmidExpE1B(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Galileo,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::E1B,
                            gnsstk::NavType::GalINAV),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpE5b(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Galileo,
                            gnsstk::CarrierBand::E5b, gnsstk::TrackingCode::E5bI,
                            gnsstk::NavType::GalINAV),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime toaExp = gnsstk::GALWeekSecond(1014,517200.0);
   gnsstk::CommonTime beginExp = navINAVGalWT7ct;
   gnsstk::CommonTime endExp = toaExp + (74.0 * 3600.0);
   gnsstk::CommonTime expRefTime = gnsstk::GALWeekSecond(1014,518400);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GalINavAlm *alm;
   gnsstk::GalINavHealth *hea;
   gnsstk::GalINavTimeOffset *tim;
   TUASSERTE(bool, true, uut.processAlm(7, navINAVGalWT7, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processAlm(8, navINAVGalWT8, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processAlm(9, navINAVGalWT9, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processAlm(10, navINAVGalWT10, navOut));
   for (const auto& i : navOut)
   {
      if ((alm = dynamic_cast<gnsstk::GalINavAlm*>(i.get())) != nullptr)
      {
         gnsstk::NavMessageID nmid(nmidExpE1B);
         if (alm->signal.sat.id == 19)
         {
            nmid.sat.id = 19;
            TUASSERTE(gnsstk::CommonTime, navINAVGalWT7ct, alm->timeStamp);
            TUASSERTE(gnsstk::CommonTime, navINAVGalWT7ct, alm->xmitTime);
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
            TUASSERTE(gnsstk::CommonTime, navINAVGalWT7ct, alm->beginFit);
            TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
               // GalINavAlm
            TUASSERTE(gnsstk::CommonTime, navINAVGalWT8ct, alm->xmit2);
            TUASSERTFE(2.34375000E-02, alm->dAhalf);
            TUASSERTFE(-1.8983012250084810996E-02, alm->deltai);
         }
         else
         {
            nmid.sat.id = 21;
            TUASSERTE(gnsstk::CommonTime, navINAVGalWT9ct, alm->timeStamp);
            TUASSERTE(gnsstk::CommonTime, navINAVGalWT9ct, alm->xmitTime);
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
            TUASSERTE(gnsstk::CommonTime, navINAVGalWT9ct, alm->beginFit);
            TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
               // GalINavAlm
            TUASSERTE(gnsstk::CommonTime, navINAVGalWT10ct, alm->xmit2);
            TUASSERTFE(4.68750000E-02, alm->dAhalf);
            TUASSERTFE(1.2080098704599424864E-02, alm->deltai);
         }
         nmid.messageType = gnsstk::NavMessageType::Almanac;
            // NavData fields
         TUASSERTE(gnsstk::NavMessageID, nmid, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toe);
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toc);
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, alm->health);
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
         TUASSERTE(gnsstk::GalHealthStatus, gnsstk::GalHealthStatus::OK,
                   alm->hsE5b);
         TUASSERTE(gnsstk::GalHealthStatus, gnsstk::GalHealthStatus::OK,
                   alm->hsE1B);
      }
      else if ((hea = dynamic_cast<gnsstk::GalINavHealth*>(i.get())) != nullptr)
      {
         gnsstk::NavMessageID nmid;
         if (hea->signal.obs.band == gnsstk::CarrierBand::L1)
         {
            nmid = nmidExpE1B;
            nmid.messageType = gnsstk::NavMessageType::Health;
         }
         else
         {
            nmid = nmidExpE5b;
            nmid.messageType = gnsstk::NavMessageType::Health;
         }
         if (hea->signal.sat.id == 19)
         {
            nmid.sat.id = 19;
            TUASSERTE(gnsstk::CommonTime, navINAVGalWT8ct, hea->timeStamp);
         }
         else
         {
            nmid.sat.id = 21;
            TUASSERTE(gnsstk::CommonTime, navINAVGalWT10ct, hea->timeStamp);
         }
         TUASSERTE(gnsstk::GalHealthStatus, gnsstk::GalHealthStatus::OK,
                   hea->sigHealthStatus);
         TUASSERTE(gnsstk::GalDataValid, gnsstk::GalDataValid::Unknown,
                   hea->dataValidityStatus);
         TUASSERTE(unsigned, 255, hea->sisaIndex);
      }
      else if ((tim = dynamic_cast<gnsstk::GalINavTimeOffset*>(i.get()))
               != nullptr)
      {
         nmidExpE1B.messageType = gnsstk::NavMessageType::TimeOffset;
         TUASSERTE(gnsstk::CommonTime, navINAVGalWT10ct, tim->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExpE1B, tim->signal);
            /** @warning these truth values have not been vetted due
             * to a lack of an existing tool to print the information
             * (the old implementation only handles the data in
             * WT6). */
            // StdNavTimeOffset
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GAL, tim->src);
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GPS, tim->tgt);
         TUASSERTFE(-6.3155312091112136841e-09, tim->a0);
         TUASSERTFE(-6.6613381477509392425e-15, tim->a1);
         TUASSERTFE(0, tim->a2);
         TUASSERTFE(0, tim->deltatLS);
         TUASSERTE(gnsstk::CommonTime, expRefTime, tim->refTime);
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
   gnsstk::PNBGalINavDataFactory uut;
   gnsstk::NavMessageID nmidExpE1B(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Galileo,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::E1B,
                            gnsstk::NavType::GalINAV),
      gnsstk::NavMessageType::TimeOffset);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GalINavTimeOffset *tim;
   gnsstk::CommonTime expRefTime(gnsstk::GALWeekSecond(1014,432000));
   TUASSERTE(bool, true, uut.processOffset(navINAVGalWT6, navOut));
   for (const auto& i : navOut)
   {
      if ((tim = dynamic_cast<gnsstk::GalINavTimeOffset*>(i.get()))
               != nullptr)
      {
         TUASSERTE(gnsstk::CommonTime, navINAVGalWT6ct, tim->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExpE1B, tim->signal);
            // StdNavTimeOffset
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GAL, tim->src);
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC, tim->tgt);
         TUASSERTFE(-9.31322575E-10, tim->a0);
         TUASSERTFE( 8.88178420E-16, tim->a1);
         TUASSERTFE( 0.00000000E+00, tim->a2);
         TUASSERTFE(18, tim->deltatLS);
         TUASSERTE(gnsstk::CommonTime, expRefTime, tim->refTime);
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


unsigned PNBGalINavDataFactory_T :: processEphWRTest()
{
   TUDEF("PNBGalINavDataFactory", "processEph");
   gnsstk::PNBGalINavDataFactory uut;
   gnsstk::CommonTime toeExp = gnsstk::GALWeekSecond(1195,603600.0);
   gnsstk::CommonTime tocExp = gnsstk::GALWeekSecond(1195,603600.0);
   gnsstk::CommonTime beginExp = navINAVWRWT1ct;
   gnsstk::CommonTime endExp = toeExp + (4.0 * 3600.0);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GalINavEph *eph;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris}));
   TUASSERTE(bool, true, uut.addData(navINAVWRWT1, navOut));
   TUASSERTE(gnsstk::NavDataPtrList::size_type, 0, navOut.size());
   TUASSERTE(bool, true, uut.addData(navINAVWRWT2, navOut));
   TUASSERTE(gnsstk::NavDataPtrList::size_type, 0, navOut.size());
   TUASSERTE(bool, true, uut.addData(navINAVWRWT3, navOut));
   TUASSERTE(gnsstk::NavDataPtrList::size_type, 0, navOut.size());
   TUASSERTE(bool, true, uut.addData(navINAVWRWT4, navOut));
   TUASSERTE(gnsstk::NavDataPtrList::size_type, 0, navOut.size());
   TUASSERTE(bool, true, uut.addData(navINAVWRWT5, navOut));
   TUASSERTE(gnsstk::NavDataPtrList::size_type, 1, navOut.size());
   if (navOut.size() >= 1)
   {
      if ((eph = dynamic_cast<gnsstk::GalINavEph*>(navOut.begin()->get()))
          != nullptr)
      {
         TUASSERTE(gnsstk::CommonTime, navINAVWRWT1ct, eph->timeStamp);
         TUASSERTE(gnsstk::CommonTime, navINAVWRWT1ct, eph->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(gnsstk::CommonTime, tocExp, eph->Toc);
         TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
         TUASSERTE(gnsstk::CommonTime, navINAVWRWT2ct, eph->xmit2);
         TUASSERTE(gnsstk::CommonTime, navINAVWRWT3ct, eph->xmit3);
         TUASSERTE(gnsstk::CommonTime, navINAVWRWT4ct, eph->xmit4);
         TUASSERTE(gnsstk::CommonTime, navINAVWRWT5ct, eph->xmit5);
      }
   }
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
   errorTotal += testClass.addDataPagePairsTest();
   errorTotal += testClass.processAlmTest();
   errorTotal += testClass.processOffsetTest();
   errorTotal += testClass.processEphWRTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
