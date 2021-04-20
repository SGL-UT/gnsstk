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
#include "PNBGalFNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GalFNavTimeOffset.hpp"
#include "GalNavHealth.hpp"
#include "GalFNavEph.hpp"
#include "GalFNavAlm.hpp"
#include "GALWeekSecond.hpp"
#include "TimeString.hpp"

using namespace std;

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

      /// Count the various types of messages present in navOut.
   void countResults(const gpstk::NavDataPtrList& navOut);

      /// Counts of messages, set by countResults.
   unsigned almCount, ephCount, toCount, heaCount, otherCount;
};

PNBGalFNavDataFactory_T ::
PNBGalFNavDataFactory_T()
      : almCount(0), ephCount(0), toCount(0), heaCount(0), otherCount(0)
{
#include "GalFNavTestDataDef.hpp"
}


unsigned PNBGalFNavDataFactory_T ::
addDataAllTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   gpstk::PNBGalFNavDataFactory uut;
   gpstk::SatID gloSid(1,gpstk::SatelliteSystem::Glonass);
   gpstk::ObsID gloid(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::G1,
                      gpstk::TrackingCode::Standard);
   gpstk::NavID gloNav(gpstk::NavType::GloCivilF);
   gpstk::PackedNavBitsPtr nonGPS =
      std::make_shared<gpstk::PackedNavBits>(gloSid,gloid,gloNav,"XX",
                                             navFNAVGalPT1ct);
   gpstk::NavDataPtrList navOut;
      // should refuse non-GPS data
   TUASSERTE(bool, false, uut.addData(nonGPS, navOut));
      // The rest is checking that we're processing the good data
      // appropriately, but I'm not bothering to check the detailed
      // contents as that is assumed to be tested in the process*
      // tests.
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   TUASSERTE(unsigned, 2, toCount);
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   TUASSERTE(size_t, 6, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, almCount);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
addDataEphemerisTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   gpstk::PNBGalFNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Ephemeris}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
addDataAlmanacTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   gpstk::PNBGalFNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Almanac}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, almCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
addDataHealthTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   gpstk::PNBGalFNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Health}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
addDataTimeTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   gpstk::PNBGalFNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::TimeOffset}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   TUASSERTE(size_t, 2, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 2, toCount);
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
addDataEphHealthTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   gpstk::PNBGalFNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Health,
                              gpstk::NavMessageType::Ephemeris}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
addDataAlmHealthTest()
{
   TUDEF("PNBGalFNavDataFactory", "addData");
   gpstk::PNBGalFNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Health,
                              gpstk::NavMessageType::Almanac}));
      // Add nav pages in time order
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT1, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT3, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT4, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(navFNAVGalPT6, navOut));
   TUASSERTE(size_t, 6, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, almCount);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
processEphTest()
{
   TUDEF("PNBGalFNavDataFactory", "processEph");
   gpstk::PNBGalFNavDataFactory uut;
   gpstk::NavMessageID nmidExpE5a(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::Galileo,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::E5aI,
                            gpstk::NavType::GalFNAV),
      gpstk::NavMessageType::Health);
   gpstk::CommonTime toeExp = gpstk::GALWeekSecond(1014,517200.0);
   gpstk::CommonTime tocExp = gpstk::GALWeekSecond(1014,517200.0);
   gpstk::CommonTime beginExp = navFNAVGalPT1ct;
   gpstk::CommonTime endExp = toeExp + (4.0 * 3600.0);
   gpstk::NavDataPtrList navOut;
   gpstk::GalFNavEph *eph;
   gpstk::GalFNavTimeOffset *to;
   gpstk::GalNavHealth *hea;
   TUASSERTE(bool, true, uut.processEph(1, navFNAVGalPT1, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   almCount = ephCount = toCount = heaCount = otherCount = 0;
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gpstk::GalNavHealth*>(i.get())) != nullptr)
      {
         heaCount++;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, navFNAVGalPT1ct, hea->timeStamp);
         nmidExpE5a.messageType = gpstk::NavMessageType::Health;
         TUASSERTE(gpstk::NavMessageID, nmidExpE5a, hea->signal);
            // NavHealthData has no fields
            // GalNavHealth
         TUASSERTE(gpstk::GalHealthStatus, gpstk::GalHealthStatus::OK,
                   hea->sigHealthStatus);
         TUASSERTE(gpstk::GalDataValid, gpstk::GalDataValid::Valid,
                   hea->dataValidityStatus);
         TUASSERTE(unsigned, 107, hea->sisaIndex);
      }
      else
      {
         otherCount++;
      }
   }
   TUASSERTE(unsigned, 1, heaCount);
   TUASSERTE(unsigned, 0, otherCount);
   navOut.clear();
   TUASSERTE(bool, true, uut.processEph(2, navFNAVGalPT2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.processEph(3, navFNAVGalPT3, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.processEph(4, navFNAVGalPT4, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   almCount = ephCount = toCount = heaCount = otherCount = 0;
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gpstk::GalFNavEph*>(i.get())) != nullptr)
      {
         ephCount++;
         nmidExpE5a.messageType = gpstk::NavMessageType::Ephemeris;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, navFNAVGalPT1ct, eph->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExpE5a, eph->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gpstk::CommonTime, navFNAVGalPT1ct, eph->xmitTime);
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
         TUASSERTFE(-5.3351762471720576286E-04, eph->af0);
         TUASSERTFE(-8.0859763329499401152E-12, eph->af1);
         TUASSERTFE(0, eph->af2);
         TUASSERTE(gpstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gpstk::CommonTime, endExp, eph->endFit);
            // GalFNavEph
         TUASSERTFE(-4.656613E-09, eph->bgdE5aE1);
         TUASSERTE(unsigned, 107, eph->sisaIndex);
         TUASSERTE(unsigned, 1, eph->svid);
         TUASSERTE(gpstk::CommonTime, navFNAVGalPT2ct, eph->xmit2);
         TUASSERTE(gpstk::CommonTime, navFNAVGalPT3ct, eph->xmit3);
         TUASSERTE(gpstk::CommonTime, navFNAVGalPT4ct, eph->xmit4);
         TUASSERTE(uint16_t, 94, eph->iodnav1);
         TUASSERTE(uint16_t, 94, eph->iodnav2);
         TUASSERTE(uint16_t, 94, eph->iodnav3);
         TUASSERTE(uint16_t, 94, eph->iodnav4);
         TUASSERTE(gpstk::GalHealthStatus, gpstk::GalHealthStatus::OK,
                   eph->hsE5a);
         TUASSERTE(gpstk::GalDataValid, gpstk::GalDataValid::Valid,
                   eph->dvsE5a);
      }
      else if ((to = dynamic_cast<gpstk::GalFNavTimeOffset*>(i.get()))
               != nullptr)
      {
         toCount++;
      }
      else
      {
         otherCount++;
      }
   }
   TUASSERTE(unsigned, 1, ephCount);
   TUASSERTE(unsigned, 2, toCount);
   TUASSERTE(unsigned, 0, otherCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGalFNavDataFactory_T ::
processAlmTest()
{
   TUDEF("PNBGalFNavDataFactory", "processAlm");
   gpstk::PNBGalFNavDataFactory uut;
   gpstk::NavMessageID nmidExpE5a(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::Galileo,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::E5aI,
                            gpstk::NavType::GalFNAV),
      gpstk::NavMessageType::Health);
   gpstk::CommonTime toaExp = gpstk::GALWeekSecond(1014,517200.0);
   gpstk::CommonTime beginExp = navFNAVGalPT5ct;
   gpstk::CommonTime endExp = toaExp + (74.0 * 3600.0);
   gpstk::CommonTime expRefTime = gpstk::GALWeekSecond(1014,518400);
   gpstk::NavDataPtrList navOut;
   gpstk::GalFNavAlm *alm;
   gpstk::GalNavHealth *hea;
   gpstk::GalFNavTimeOffset *tim;
   TUASSERTE(bool, true, uut.processAlm(5, navFNAVGalPT5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.processAlm(6, navFNAVGalPT6, navOut));
   TUASSERTE(size_t, 6, navOut.size());
   almCount = ephCount = toCount = heaCount = otherCount = 0;
   for (const auto& i : navOut)
   {
      if ((alm = dynamic_cast<gpstk::GalFNavAlm*>(i.get())) != nullptr)
      {
         almCount++;
         gpstk::NavMessageID nmid(nmidExpE5a);
         if (alm->signal.sat.id == 1)
         {
            nmid.sat.id = 1;
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT5ct, alm->timeStamp);
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT5ct, alm->xmitTime);
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
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT5ct, alm->beginFit);
            TUASSERTE(gpstk::CommonTime, endExp, alm->endFit);
               // GalFNavAlm
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT5ct, alm->xmit2);
            TUASSERTFE(3.51562500E-02, alm->dAhalf);
            TUASSERTFE(1.2463593901570833855E-02, alm->deltai);
         }
         else if (alm->signal.sat.id == 2)
         {
            nmid.sat.id = 2;
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT5ct, alm->timeStamp);
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT5ct, alm->xmitTime);
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
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT5ct, alm->beginFit);
            TUASSERTE(gpstk::CommonTime, endExp, alm->endFit);
               // GalFNavAlm
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT6ct, alm->xmit2);
            TUASSERTFE(3.90625000E-02, alm->dAhalf);
            TUASSERTFE(1.2463593901570833855E-02, alm->deltai);
         }
         else if (alm->signal.sat.id == 3)
         {
            nmid.sat.id = 3;
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT6ct, alm->timeStamp);
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT6ct, alm->xmitTime);
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
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT6ct, alm->beginFit);
            TUASSERTE(gpstk::CommonTime, endExp, alm->endFit);
               // GalFNavAlm
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT6ct, alm->xmit2);
            TUASSERTFE(1.75781250E-02, alm->dAhalf);
            TUASSERTFE(-2.4735440204655963214E-02, alm->deltai);
         }
         else
         {
            TUFAIL("Unexpected satellite ID");
         }
            // NavData fields
         nmid.messageType = gpstk::NavMessageType::Almanac;
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
         TUASSERTE(gpstk::GalHealthStatus, gpstk::GalHealthStatus::OK,
                   alm->hsE5a);
      }
      else if ((hea = dynamic_cast<gpstk::GalNavHealth*>(i.get())) != nullptr)
      {
         heaCount++;
         gpstk::NavMessageID nmid(nmidExpE5a);
         nmid.messageType = gpstk::NavMessageType::Health;
         if (hea->signal.sat.id == 1)
         {
            nmid.sat.id = 1;
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT5ct, hea->timeStamp);
         }
         else if (hea->signal.sat.id == 2)
         {
            nmid.sat.id = 2;
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT6ct, hea->timeStamp);
         }
         else if (hea->signal.sat.id == 3)
         {
            nmid.sat.id = 3;
            TUASSERTE(gpstk::CommonTime, navFNAVGalPT6ct, hea->timeStamp);
         }
         else
         {
            TUFAIL("Unexpected satellite ID");
         }
         TUASSERTE(gpstk::NavMessageID, nmid, hea->signal);
         TUASSERTE(gpstk::GalHealthStatus, gpstk::GalHealthStatus::OK,
                   hea->sigHealthStatus);
         TUASSERTE(gpstk::GalDataValid, gpstk::GalDataValid::Unknown,
                   hea->dataValidityStatus);
         TUASSERTE(unsigned, 255, hea->sisaIndex);
      }
      else
      {
         otherCount++;
      }
   }
   TUASSERTE(unsigned, 3, almCount);
   TUASSERTE(unsigned, 3, heaCount);
   TUASSERTE(unsigned, 0, otherCount);
   navOut.clear();
   TURETURN();
}


void PNBGalFNavDataFactory_T ::
countResults(const gpstk::NavDataPtrList& navOut)
{
   almCount = ephCount = toCount = heaCount = otherCount = 0;
   for (const auto& i : navOut)
   {
      if (dynamic_cast<gpstk::GalFNavAlm*>(i.get()) != nullptr)
      {
         almCount++;
      }
      else if (dynamic_cast<gpstk::GalFNavEph*>(i.get()) != nullptr)
      {
         ephCount++;
      }
      else if (dynamic_cast<gpstk::GalFNavTimeOffset*>(i.get()) != nullptr)
      {
         toCount++;
      }
      else if (dynamic_cast<gpstk::GalNavHealth*>(i.get()) != nullptr)
      {
         heaCount++;
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
