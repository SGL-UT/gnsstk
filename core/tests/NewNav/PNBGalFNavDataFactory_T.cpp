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
#include "PNBGalFNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GalFNavTimeOffset.hpp"
#include "GalFNavHealth.hpp"
#include "GalFNavEph.hpp"
#include "GalFNavAlm.hpp"
#include "GalFNavIono.hpp"
#include "GalFNavISC.hpp"
#include "GALWeekSecond.hpp"
#include "TimeString.hpp"

using namespace std;

// avoid having to type out template params over and over.
using GalFactoryCounter = FactoryCounter<gnsstk::GalFNavAlm,gnsstk::GalFNavEph,gnsstk::GalFNavTimeOffset,gnsstk::GalFNavHealth,gnsstk::GalFNavIono,gnsstk::GalFNavISC>;

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


class PNBGalFNavDataFactory_T
{
public:
   PNBGalFNavDataFactory_T();

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

#include "GalFNavTestDataDecl.hpp"
};

PNBGalFNavDataFactory_T ::
PNBGalFNavDataFactory_T()
{
#include "GalFNavTestDataDef.hpp"
}


unsigned PNBGalFNavDataFactory_T ::
addDataAllTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gnsstk::PNBGalFNavDataFactory uut;
   gnsstk::SatID gloSid(1,gnsstk::SatelliteSystem::Glonass);
   gnsstk::ObsID gloid(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::G1,
                      gnsstk::TrackingCode::Standard);
   gnsstk::NavID gloNav(gnsstk::NavType::GloCivilF);
   gnsstk::PackedNavBitsPtr nonGPS =
      std::make_shared<gnsstk::PackedNavBits>(gloSid,gloid,gloNav,"XX",
                                             navFNAVGalPT1ct);
   gnsstk::NavDataPtrList navOut;
      // should refuse non-GPS data
   TUASSERTE(bool, false, uut.addData(nonGPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // The rest is checking that we're processing the good data
      // appropriately, but I'm not bothering to check the detailed
      // contents as that is assumed to be tested in the process*
      // tests.
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 1, 1, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 1, 2);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   fc.validateResults(navOut, __LINE__, 6, 3, 0, 0, 3);
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
addDataEphemerisTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gnsstk::PNBGalFNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
addDataAlmanacTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gnsstk::PNBGalFNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Almanac}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   fc.validateResults(navOut, __LINE__, 3, 3);
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
addDataHealthTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gnsstk::PNBGalFNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 3);
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
addDataTimeTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gnsstk::PNBGalFNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::TimeOffset}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 2);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
addDataEphHealthTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gnsstk::PNBGalFNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health,
                              gnsstk::NavMessageType::Ephemeris}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 3);
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
addDataAlmHealthTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   GalFactoryCounter fc(testFramework);
   gnsstk::PNBGalFNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health,
                              gnsstk::NavMessageType::Almanac}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   fc.validateResults(navOut, __LINE__, 6, 3, 0, 0, 3);
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
processEphTest()
{
   TUDEF("PNBGalFNavDataFactory", "processEph");
   GalFactoryCounter fc(testFramework);
   gnsstk::PNBGalFNavDataFactory uut;
   gnsstk::NavMessageID nmidExpE5a(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Galileo,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::E5aI,
                            gnsstk::NavType::GalFNAV),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime toeExp = gnsstk::GALWeekSecond(1014,517200.0);
   gnsstk::CommonTime tocExp = gnsstk::GALWeekSecond(1014,517200.0);
   gnsstk::CommonTime beginExp = navFNAVGalPT1ct;
   gnsstk::CommonTime endExp = toeExp + (4.0 * 3600.0);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GalFNavEph *eph;
   gnsstk::GalFNavTimeOffset *to;
   gnsstk::GalFNavHealth *hea;
   gnsstk::GalFNavIono *iono;
   gnsstk::GalFNavISC *isc;
   TUASSERTE(bool, true, uut.processEph(1, navFNAVGalPT1, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GalFNavHealth*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGalPT1ct, hea->timeStamp);
         nmidExpE5a.messageType = gnsstk::NavMessageType::Health;
         TUASSERTE(gnsstk::NavMessageID, nmidExpE5a, hea->signal);
            // NavHealthData has no fields
            // GalFNavHealth
         TUASSERTE(gnsstk::GalHealthStatus, gnsstk::GalHealthStatus::OK,
                   hea->sigHealthStatus);
         TUASSERTE(gnsstk::GalDataValid, gnsstk::GalDataValid::Valid,
                   hea->dataValidityStatus);
         TUASSERTE(unsigned, 107, hea->sisaIndex);
      }
      else if ((iono = dynamic_cast<gnsstk::GalFNavIono*>(i.get())) != nullptr)
      {
         nmidExpE5a.messageType = gnsstk::NavMessageType::Iono;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGalPT1ct, iono->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExpE5a, iono->signal);
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
      else if ((isc = dynamic_cast<gnsstk::GalFNavISC*>(i.get())) != nullptr)
      {
         nmidExpE5a.messageType = gnsstk::NavMessageType::ISC;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGalPT1ct, isc->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExpE5a, isc->signal);
         TUASSERTFE(-4.656613e-09, isc->isc);
      }
   }
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 1, 1, 1);
   TUASSERTE(bool, true, uut.processEph(2, navFNAVGalPT2, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(3, navFNAVGalPT3, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processEph(4, navFNAVGalPT4, navOut));
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gnsstk::GalFNavEph*>(i.get())) != nullptr)
      {
         nmidExpE5a.messageType = gnsstk::NavMessageType::Ephemeris;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGalPT1ct, eph->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExpE5a, eph->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, navFNAVGalPT1ct, eph->xmitTime);
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
         TUASSERTFE(-5.3351762471720576286E-04, eph->af0);
         TUASSERTFE(-8.0859763329499401152E-12, eph->af1);
         TUASSERTFE(0, eph->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
            // GalFNavEph
         TUASSERTFE(-4.656613E-09, eph->bgdE5aE1);
         TUASSERTE(unsigned, 107, eph->sisaIndex);
         TUASSERTE(unsigned, 1, eph->svid);
         TUASSERTE(gnsstk::CommonTime, navFNAVGalPT2ct, eph->xmit2);
         TUASSERTE(gnsstk::CommonTime, navFNAVGalPT3ct, eph->xmit3);
         TUASSERTE(gnsstk::CommonTime, navFNAVGalPT4ct, eph->xmit4);
         TUASSERTE(uint16_t, 94, eph->iodnav1);
         TUASSERTE(uint16_t, 94, eph->iodnav2);
         TUASSERTE(uint16_t, 94, eph->iodnav3);
         TUASSERTE(uint16_t, 94, eph->iodnav4);
         TUASSERTE(gnsstk::GalHealthStatus, gnsstk::GalHealthStatus::OK,
                   eph->hsE5a);
         TUASSERTE(gnsstk::GalDataValid, gnsstk::GalDataValid::Valid,
                   eph->dvsE5a);
      }
   }
   fc.validateResults(navOut, __LINE__, 3, 0, 1, 2);
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
processAlmTest()
{
   TUDEF("PNBGalFNavDataFactory", "processAlm");
   GalFactoryCounter fc(testFramework);
   gnsstk::PNBGalFNavDataFactory uut;
   gnsstk::NavMessageID nmidExpE5a(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Galileo,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::E5aI,
                            gnsstk::NavType::GalFNAV),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime toaExp = gnsstk::GALWeekSecond(1014,517200.0);
   gnsstk::CommonTime beginExp = navFNAVGalPT5ct;
   gnsstk::CommonTime endExp = toaExp + (74.0 * 3600.0);
   gnsstk::CommonTime expRefTime = gnsstk::GALWeekSecond(1014,518400);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GalFNavAlm *alm;
   gnsstk::GalFNavHealth *hea;
   gnsstk::GalFNavTimeOffset *tim;
   TUASSERTE(bool, true, uut.processAlm(5, navFNAVGalPT5, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processAlm(6, navFNAVGalPT6, navOut));
   for (const auto& i : navOut)
   {
      if ((alm = dynamic_cast<gnsstk::GalFNavAlm*>(i.get())) != nullptr)
      {
         gnsstk::NavMessageID nmid(nmidExpE5a);
         if (alm->signal.sat.id == 1)
         {
            nmid.sat.id = 1;
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT5ct, alm->timeStamp);
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT5ct, alm->xmitTime);
            TUASSERTFE(-3.016573219377113535E+00, alm->M0);
            TUASSERTFE(1.068115234375E-04, alm->ecc);
            TUASSERTFE(29600382.542594015598, alm->A);
            TUASSERTFE(5440.6233597441769234, alm->Ahalf);
            TUASSERTFE(2.963171513198844309E+00, alm->OMEGA0);
            TUASSERTFE(0.98984797501839538736, alm->i0);
            TUASSERTFE(1.4026336829229332781E-01, alm->w);
            TUASSERTFE(-5.12021328E-09, alm->OMEGAdot);
            TUASSERTFE(-5.340576171875E-04, alm->af0);
            TUASSERTFE(-7.27595761E-12, alm->af1);
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT5ct, alm->beginFit);
            TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
               // GalFNavAlm
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT5ct, alm->xmit2);
            TUASSERTFE(3.51562500E-02, alm->dAhalf);
            TUASSERTFE(1.2463593901570833855E-02, alm->deltai);
         }
         else if (alm->signal.sat.id == 2)
         {
            nmid.sat.id = 2;
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT5ct, alm->timeStamp);
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT5ct, alm->xmitTime);
            TUASSERTFE(1.6010924473556378822E-01, alm->M0);
            TUASSERTFE(2.899169921875E-04, alm->ecc);
            // TUASSERTFE(666, alm->A);
            // TUASSERTFE(666, alm->Ahalf);
            TUASSERTFE(2.963171513198844309E+00, alm->OMEGA0);
            // TUASSERTFE(666, alm->i0);
            TUASSERTFE(1.0344782938303792585E-01, alm->w);
            TUASSERTFE(-5.12021328E-09, alm->OMEGAdot);
            TUASSERTFE(4.57763671875E-05, alm->af0);
            TUASSERTFE(0, alm->af1);
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT5ct, alm->beginFit);
            TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
               // GalFNavAlm
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT6ct, alm->xmit2);
            TUASSERTFE(3.90625000E-02, alm->dAhalf);
            TUASSERTFE(1.2463593901570833855E-02, alm->deltai);
         }
         else if (alm->signal.sat.id == 3)
         {
            nmid.sat.id = 3;
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT6ct, alm->timeStamp);
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT6ct, alm->xmitTime);
            TUASSERTFE(1.8612939385007398752E+00, alm->M0);
            TUASSERTFE(2.44140625E-04, alm->ecc);
            // TUASSERTFE(666, alm->A);
            // TUASSERTFE(666, alm->Ahalf);
            TUASSERTFE(8.701506019281299853E-01, alm->OMEGA0);
            // TUASSERTFE(666, alm->i0);
            TUASSERTFE(4.9537992068781921962E-01, alm->w);
            TUASSERTFE(-5.85167232E-09, alm->OMEGAdot);
            TUASSERTFE(-1.2969970703125E-04, alm->af0);
            TUASSERTFE(-3.63797881E-12, alm->af1);
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT6ct, alm->beginFit);
            TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
               // GalFNavAlm
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT6ct, alm->xmit2);
            TUASSERTFE(1.75781250E-02, alm->dAhalf);
            TUASSERTFE(-2.4735440204655963214E-02, alm->deltai);
         }
         else
         {
            TUFAIL("Unexpected satellite ID");
         }
            // NavData fields
         nmid.messageType = gnsstk::NavMessageType::Almanac;
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
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(unsigned, 1014, alm->wna);
         TUASSERTFE(517200, alm->t0a);
         TUASSERTE(unsigned, 14, alm->ioda5);
         TUASSERTE(unsigned, 14, alm->ioda6);
         TUASSERTE(gnsstk::GalHealthStatus, gnsstk::GalHealthStatus::OK,
                   alm->hsE5a);
      }
      else if ((hea = dynamic_cast<gnsstk::GalFNavHealth*>(i.get())) != nullptr)
      {
         gnsstk::NavMessageID nmid(nmidExpE5a);
         nmid.messageType = gnsstk::NavMessageType::Health;
         if (hea->signal.sat.id == 1)
         {
            nmid.sat.id = 1;
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT5ct, hea->timeStamp);
         }
         else if (hea->signal.sat.id == 2)
         {
            nmid.sat.id = 2;
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT6ct, hea->timeStamp);
         }
         else if (hea->signal.sat.id == 3)
         {
            nmid.sat.id = 3;
            TUASSERTE(gnsstk::CommonTime, navFNAVGalPT6ct, hea->timeStamp);
         }
         else
         {
            TUFAIL("Unexpected satellite ID");
         }
         TUASSERTE(gnsstk::NavMessageID, nmid, hea->signal);
         TUASSERTE(gnsstk::GalHealthStatus, gnsstk::GalHealthStatus::OK,
                   hea->sigHealthStatus);
         TUASSERTE(gnsstk::GalDataValid, gnsstk::GalDataValid::Unknown,
                   hea->dataValidityStatus);
         TUASSERTE(unsigned, 255, hea->sisaIndex);
      }
   }
   fc.validateResults(navOut, __LINE__, 6, 3, 0, 0, 3);
   TURETURN();
}


int main()
{
   PNBGalFNavDataFactory_T testClass;
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

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
