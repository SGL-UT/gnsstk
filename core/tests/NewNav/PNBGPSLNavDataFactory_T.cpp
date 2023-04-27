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
#include "CarrierBand.hpp"
#include "FactoryCounter.hpp"
#include "GPSNavConfig.hpp"
#include "GPSSVConfig.hpp"
#include "NavMessageType.hpp"
#include "NavSatelliteID.hpp"
#include "NavType.hpp"
#include "PNBGPSLNavDataFactory.hpp"
#include "SatelliteSystem.hpp"
#include "TestUtil.hpp"
#include "GPSLNavTimeOffset.hpp"
#include "GPSLNavHealth.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavAlm.hpp"
#include "GPSLNavIono.hpp"
#include "GPSLNavISC.hpp"
#include "TimeString.hpp"
#include "CivilTime.hpp"
#include "TrackingCode.hpp"
#include <memory>

using namespace std;

// avoid having to type out template params over and over.
using GPSFactoryCounter = FactoryCounter<gnsstk::GPSLNavAlm,gnsstk::GPSLNavEph,gnsstk::GPSLNavTimeOffset,gnsstk::GPSLNavHealth,gnsstk::GPSLNavIono,gnsstk::GPSLNavISC>;

namespace gnsstk
{
   ostream& operator<<(ostream& s, GPSLNavL2Codes e)
   {
      s << static_cast<int>(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
}


class PNBGPSLNavDataFactory_T
{
public:
   PNBGPSLNavDataFactory_T();
      // addData tests for a variety of filter states
      /// Test addData with all message types enabled
   unsigned addDataAllTest();
      /// Test addData with almanac selected only
   unsigned addDataAlmanacTest();
      /// Test addData with ephemeris selected only
   unsigned addDataEphemerisTest();
      /// Test addData with health data selected only
   unsigned addDataHealthTest();
      /// Test addData with time offset data selected only
   unsigned addDataTimeTest();
      /// One additional combo test with no particular meaning behind selection.
   unsigned addDataEphHealthTest();
      /// Test validity checking.
   unsigned addDataValidityTest();
   unsigned processEphTest();
   unsigned processAlmOrbTest();
   unsigned processSVID51Test();
   unsigned processSVID63Test();
   unsigned processSVID56Test();

      // tests with QZSS data

      /// Test addData with all message types enabled
   unsigned addDataAllQZSSTest();
      // no need to mirror the other addData tests from GPS as they
      // are testing aspects that are not specific to GPS (type
      // filter).
   unsigned processEphQZSSTest();
   unsigned processAlmOrbQZSSTest();
   unsigned processSVID51QZSSTest();
   unsigned processSVID56QZSSTest();

#include "LNavTestDataDecl.hpp"
};


PNBGPSLNavDataFactory_T ::
PNBGPSLNavDataFactory_T()
{
#include "LNavTestDataDef.hpp"
}


unsigned PNBGPSLNavDataFactory_T ::
addDataAllTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSLNavDataFactory uut;

   // Add system messages to the default NavMessageTypeSet.
   gnsstk::NavMessageTypeSet nmts{gnsstk::allNavMessageTypes};
   nmts.emplace(gnsstk::NavMessageType::System);
   uut.setTypeFilter(nmts);

   gnsstk::SatID gloSid(1,gnsstk::SatelliteSystem::Glonass);
   gnsstk::ObsID gloid(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::G1,
                      gnsstk::TrackingCode::Standard);
   gnsstk::NavID gloNav(gnsstk::NavType::GloCivilF);
   gnsstk::PackedNavBitsPtr nonGPS = std::make_shared<gnsstk::PackedNavBits>(
      gloSid, gloid, gloNav, "XX", almLNAVGPS26ct);
   gnsstk::NavDataPtrList navOut;
      // should refuse non-GPS data
   TUASSERTE(bool, false, uut.addData(nonGPS, navOut));
      // The rest is checking that we're processing the good data
      // appropriately, but I'm not bothering to check the detailed
      // contents as that is assumed to be tested in the process*
      // tests.
      // Add subframe 1, expect 1 health and 1 ISC.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF1, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 0, 1, 0, 1);
      // add subframe 2, expect nothing yet
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF2, navOut));
   fc.validateResults(navOut, __LINE__);
      // add subframe 3, expect the completed ephemeris
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF3, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
      // add almanac sv id 25, expect health
   TUASSERTE(bool, true, uut.addData(almLNAVGPS25, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
      // add almanac sv id 26, expect health
   TUASSERTE(bool, true, uut.addData(almLNAVGPS26, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
      // add page 56, expect time offset and iono data
   TUASSERTE(bool, true, uut.addData(pg56LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 1, 0, 1);
      // add page 63, expect 8 health and 32 unknown (SV config)
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__, 40, 0, 0, 0, 8, 0, 0, 32);
      // add page 51, expect 24 health and 2 almanacs
   TUASSERTE(bool, true, uut.addData(pg51LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__, 26, 2, 0, 0, 24);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
addDataAlmanacTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Almanac}));
      // Add subframe 1, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF1, navOut));
   fc.validateResults(navOut, __LINE__);
      // add subframe 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF2, navOut));
   fc.validateResults(navOut, __LINE__);
      // add subframe 3, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF3, navOut));
   fc.validateResults(navOut, __LINE__);
      // add almanac sv id 25, expect nothing
   TUASSERTE(bool, true, uut.addData(almLNAVGPS25, navOut));
   fc.validateResults(navOut, __LINE__);
      // add almanac sv id 26, expect nothing
   TUASSERTE(bool, true, uut.addData(almLNAVGPS26, navOut));
   fc.validateResults(navOut, __LINE__);
      // add page 56, expect nothing
   TUASSERTE(bool, true, uut.addData(pg56LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // add page 63, expect nothing
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // add page 51, expect 2 almanacs
   TUASSERTE(bool, true, uut.addData(pg51LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__, 2, 2);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
addDataEphemerisTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris}));
      // Add subframe 1, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF1, navOut));
   fc.validateResults(navOut, __LINE__);
      // add subframe 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF2, navOut));
   fc.validateResults(navOut, __LINE__);
      // add subframe 3, expect a completed ephemeris.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF3, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
      // add almanac sv id 25, expect nothing
   TUASSERTE(bool, true, uut.addData(almLNAVGPS25, navOut));
   fc.validateResults(navOut, __LINE__);
      // add almanac sv id 26, expect nothing
   TUASSERTE(bool, true, uut.addData(almLNAVGPS26, navOut));
   fc.validateResults(navOut, __LINE__);
      // add page 56, expect nothing
   TUASSERTE(bool, true, uut.addData(pg56LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // add page 63, expect nothing
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // add page 51, expect nothing
   TUASSERTE(bool, true, uut.addData(pg51LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
addDataHealthTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health}));
      // Add subframe 1, expect 1 health object.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF1, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
      // add subframe 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF2, navOut));
   fc.validateResults(navOut, __LINE__);
      // add subframe 3, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF3, navOut));
   fc.validateResults(navOut, __LINE__);
      // add almanac sv id 25, expect 1 health
   TUASSERTE(bool, true, uut.addData(almLNAVGPS25, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
      // add almanac sv id 26, expect 1 health
   TUASSERTE(bool, true, uut.addData(almLNAVGPS26, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
      // add page 56, expect nothing
   TUASSERTE(bool, true, uut.addData(pg56LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // add page 63, expect 8 health
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__, 8, 0, 0, 0, 8);
      // add page 51, expect 24 health
   TUASSERTE(bool, true, uut.addData(pg51LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__, 24, 0, 0, 0, 24);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
addDataTimeTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::TimeOffset}));
      // Add subframe 1, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF1, navOut));
   fc.validateResults(navOut, __LINE__);
      // add subframe 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF2, navOut));
   fc.validateResults(navOut, __LINE__);
      // add subframe 3, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF3, navOut));
   fc.validateResults(navOut, __LINE__);
      // add almanac sv id 25, expect nothing.
   TUASSERTE(bool, true, uut.addData(almLNAVGPS25, navOut));
   fc.validateResults(navOut, __LINE__);
      // add almanac sv id 26, expect nothing.
   TUASSERTE(bool, true, uut.addData(almLNAVGPS26, navOut));
   fc.validateResults(navOut, __LINE__);
      // add page 56, expect time offset
   TUASSERTE(bool, true, uut.addData(pg56LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
      // add page 63, expect nothing.
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // add page 51, expect nothing.
   TUASSERTE(bool, true, uut.addData(pg51LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
addDataEphHealthTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData");
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   GPSFactoryCounter fc(testFramework);
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris,
                              gnsstk::NavMessageType::Health}));
      // Add subframe 1, expect 1 health and 1 ISC
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF1, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
      // add subframe 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF2, navOut));
   fc.validateResults(navOut, __LINE__);
      // add subframe 3, expect the completed ephemeris
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF3, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
      // add almanac sv id 25, expect 1 health
   TUASSERTE(bool, true, uut.addData(almLNAVGPS25, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
      // add almanac sv id 26, expect 1 health
   TUASSERTE(bool, true, uut.addData(almLNAVGPS26, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
      // add page 56, expect nothing.
   TUASSERTE(bool, true, uut.addData(pg56LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // add page 63, expect 8 health
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__, 8, 0, 0, 0, 8);
      // add page 51, expect 24 health
   TUASSERTE(bool, true, uut.addData(pg51LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__, 24, 0, 0, 0, 24);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
addDataValidityTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
      // make a copy of page 63 and tweak a bit for bad parity
   gnsstk::PackedNavBitsPtr pg63Bad = std::make_shared<gnsstk::PackedNavBits>(
      *pg63LNAVGPS);
   pg63Bad->insertUnsignedLong(0, 29, 1);
      // default = all validity
      // add page 63, expect 8 health
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__, 8, 0, 0, 0, 8);
      // check valid only
   uut.setValidityFilter(gnsstk::NavValidityType::ValidOnly);
      // add page 63, expect 8 health
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__, 8, 0, 0, 0, 8);
      // add BAD page 63, expect nothing
   TUASSERTE(bool, true, uut.addData(pg63Bad, navOut));
   fc.validateResults(navOut, __LINE__);
      // check invalid only
   uut.setValidityFilter(gnsstk::NavValidityType::InvalidOnly);
      // add page 63, expect nothing
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // add BAD page 63, expect 8 health.
   TUASSERTE(bool, true, uut.addData(pg63Bad, navOut));
   fc.validateResults(navOut, __LINE__, 8, 0, 0, 0, 8);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processEphTest()
{
   TUDEF("PNBGPSLNavDataFactory", "processEph");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(4, 4, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                            gnsstk::NavType::GPSLNAV),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime toeExp = gnsstk::GPSWeekSecond(1869,7200.0);
   gnsstk::CommonTime beginExp = gnsstk::GPSWeekSecond(1869, 0.0);
   gnsstk::CommonTime endExp = gnsstk::GPSWeekSecond(1869, 14400.0);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSLNavHealth *hea;
   gnsstk::GPSLNavEph *eph;
   gnsstk::GPSLNavISC *isc;
      // success, health data only
   TUASSERTE(bool, true, uut.processEph(1, ephLNAVGPSSF1, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GPSLNavHealth*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Health;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephLNAVGPSSF1ct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // GPSLNavHealth
            // Stored as uint8_t but I want failures to print numbers,
            // not characters, so I use unsigned.
         TUASSERTE(unsigned, 0, hea->svHealth);
      }
      else if ((isc = dynamic_cast<gnsstk::GPSLNavISC*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::ISC;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephLNAVGPSSF1ct, isc->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, isc->signal);
            // InterSigCorr
         TUASSERTFE(-6.519258E-09, isc->isc);
            // GPSLNavISC
         TUASSERTE(uint32_t, 0x8b, isc->pre);
         TUASSERTE(uint32_t, 0x34d, isc->tlm);
         TUASSERTE(bool, false, isc->isf);
         TUASSERTE(bool, false, isc->alert);
         TUASSERTE(bool, true, isc->asFlag);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 0, 1, 0, 1);
   TUASSERTE(bool, true, uut.processEph(2, ephLNAVGPSSF2, navOut));
   fc.validateResults(navOut, __LINE__);
      // success, and we have an ephemeris.
   TUASSERTE(bool, true, uut.processEph(3, ephLNAVGPSSF3, navOut));
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gnsstk::GPSLNavEph*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Ephemeris;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephLNAVGPSSF1ct, eph->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, eph->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, ephLNAVGPSSF1ct, eph->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, eph->health);
         TUASSERTFE(-4.73111868E-07, eph->Cuc);
         TUASSERTFE(8.9444220066070556641e-06, eph->Cus);
         TUASSERTFE(1.97843750E+02, eph->Crc);
         TUASSERTFE(-6.62500000E+00, eph->Crs);
         TUASSERTFE(-2.14204192E-07, eph->Cis);
         TUASSERTFE(-2.2910535335540771484e-07, eph->Cic);
         TUASSERTFE(1.6427405986254530745, eph->M0);
         TUASSERTFE(4.92949105E-09, eph->dn);
         TUASSERTFE(0, eph->dndot);
         TUASSERTFE(0.012018375913612544537, eph->ecc);
         TUASSERTFE(5153.6434803009033203, eph->Ahalf);
         TUASSERTFE(26560041.122048005462, eph->A);
         TUASSERTFE(0, eph->Adot);
         TUASSERTFE(1.4502525982818956773, eph->OMEGA0);
         TUASSERTFE(0.9404723483911350268, eph->i0);
         TUASSERTFE(1.1153119288528794417, eph->w);
         TUASSERTFE(-8.27748765E-09, eph->OMEGAdot);
         TUASSERTFE(3.90730561E-10, eph->idot);
         TUASSERTFE(-4.0083192288875579834e-05, eph->af0);
         TUASSERTFE(-5.34328137E-12, eph->af1);
         TUASSERTFE(0, eph->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
            // GPSLNavData fields
         TUASSERTE(uint32_t, 0x8b, eph->pre);
         TUASSERTE(uint32_t, 0x34d, eph->tlm);
         TUASSERTE(bool, false, eph->isf);
         TUASSERTE(bool, false, eph->alert);
         TUASSERTE(bool, true, eph->asFlag);
            // GPSLNavEph fields
         TUASSERTE(uint32_t, 0x8b, eph->pre2);
         TUASSERTE(uint32_t, 0x34d, eph->tlm2);
         TUASSERTE(bool, false, eph->isf2);
         TUASSERTE(uint32_t, 0x8b, eph->pre3);
         TUASSERTE(uint32_t, 0x34d, eph->tlm3);
         TUASSERTE(bool, false, eph->isf3);
         TUASSERTE(uint16_t, 0x4a, eph->iodc);
         TUASSERTE(uint16_t, 0x4a, eph->iode);
         TUASSERTE(unsigned, 0, eph->fitIntFlag);
         TUASSERTE(unsigned, 0, eph->healthBits);
         TUASSERTE(unsigned, 1, eph->uraIndex);
         TUASSERTFE(-6.519258E-09, eph->tgd);
         TUASSERTE(bool, false, eph->alert2);
         TUASSERTE(bool, false, eph->alert3);
         TUASSERTE(bool, true, eph->asFlag2);
         TUASSERTE(bool, true, eph->asFlag3);
         TUASSERTE(gnsstk::GPSLNavL2Codes,
                   gnsstk::GPSLNavL2Codes::Pcode, eph->codesL2);
         TUASSERTE(bool, false, eph->L2Pdata);
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processAlmOrbTest()
{
   TUDEF("PNBGPSLNavDataFactory", "processAlmOrb");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(25, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                            gnsstk::NavType::GPSLNAV),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime toaExp = gnsstk::GPSWeekSecond(1869,147456.0);
   gnsstk::CommonTime beginExp = gnsstk::GPSWeekSecond(1868, 500256.0);
   gnsstk::CommonTime endExp = gnsstk::GPSWeekSecond(1869, 413856.0);
   gnsstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.processAlmOrb(25, almLNAVGPS25, navOut));
      // no page 51 yet so we only get the health data back
   gnsstk::GPSLNavHealth *hea =
      dynamic_cast<gnsstk::GPSLNavHealth*>(navOut.begin()->get());
   TUASSERT(hea != nullptr);
      // NavData fields
   TUASSERTE(gnsstk::CommonTime, almLNAVGPS25ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
      // NavHealthData has no fields
      // GPSLNavHealth
   TUASSERTE(unsigned, 0, hea->svHealth);
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
   TUCSM("processSVID51()");
   navOut.clear();
   TUASSERTE(bool, true, uut.processSVID51(pg51LNAVGPS, navOut));
      // sv/page ID 51 contains health information for 24 satellites,
      // starting with PRN 1.
   unsigned subjPRN = 1;
   gnsstk::GPSLNavAlm *alm;
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GPSLNavHealth*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Health;
         nmidExp.sat.id = subjPRN++;
            // Yes this code can cause seg faults on failure, but that's ok.
         gnsstk::GPSLNavHealth *hea =
            dynamic_cast<gnsstk::GPSLNavHealth*>(i.get());
         TUASSERT(hea != nullptr);
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, pg51LNAVGPSct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // GPSLNavHealth
            // Stored as uint8_t but I want failures to print numbers,
            // not characters, so I use unsigned.
            // PRN 10 is unhealthy, but we've already incremented subjPRN
            // so compare it to 11.
         TUASSERTE(unsigned, (subjPRN == 11 ? 0x3f : 0), hea->svHealth);
      }
      else if ((alm = dynamic_cast<gnsstk::GPSLNavAlm*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Almanac;
            // make sure to reset the expected sat ID to the
            // appropriate value for the almanac page.
         nmidExp.sat.id = 25;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, almLNAVGPS25ct, alm->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, almLNAVGPS25ct, alm->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toe);
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, alm->health);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(1.714684548614031856, alm->M0);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(0.004675388336181640625, alm->ecc);
         TUASSERTFE(5153.56005859375, alm->Ahalf);
         TUASSERTFE(5153.56005859375 * 5153.56005859375, alm->A);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(-0.62474775600600196412, alm->OMEGA0);
         TUASSERTFE(0.036186367101724167816+gnsstk::GPSLNavData::refioffsetGPS,
                    alm->i0);
         TUASSERTFE(0.72259183672470084225, alm->w);
         TUASSERTFE(-7.92032991E-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-6.103515625e-05, alm->af0);
         TUASSERTFE(-3.63797881E-12, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
            // GPSLNavData fields
         TUASSERTE(uint32_t, 0x8b, alm->pre);
         TUASSERTE(uint32_t, 0x34d, alm->tlm);
         TUASSERTE(bool, false, alm->isf);
         TUASSERTE(bool, false, alm->alert);
         TUASSERTE(bool, true, alm->asFlag);
            // GPSLNavAlm fields
         TUASSERTE(unsigned, 0, alm->healthBits);
         TUASSERTFE(0.036186367101724167816, alm->deltai);
         TUASSERTFE(147456, alm->toa);
      }
   }
      // 1 GPSLNavAlm and 24 GPSLNavHealth.
   fc.validateResults(navOut, __LINE__, 25, 1, 0, 0, 24);
      // Now that a page 51 has been loaded, we should immediately get
      // both the almanac and health objects in return.
   TUCSM("processAlmOrb");
   TUASSERTE(bool, true, uut.processAlmOrb(26, almLNAVGPS26, navOut));
   nmidExp.sat.id = 26;
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GPSLNavHealth*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Health;
            // Yes this code can cause seg faults on failure, but that's ok.
         gnsstk::GPSLNavHealth *hea =
            dynamic_cast<gnsstk::GPSLNavHealth*>(i.get());
         TUASSERT(hea != nullptr);
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, almLNAVGPS26ct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // GPSLNavHealth
            // Stored as uint8_t but I want failures to print numbers,
            // not characters, so I use unsigned.
         TUASSERTE(unsigned, 0, hea->svHealth);
      }
      else if ((alm = dynamic_cast<gnsstk::GPSLNavAlm*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Almanac;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, almLNAVGPS26ct, alm->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, almLNAVGPS26ct, alm->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toe);
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, alm->health);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(1.3982957680142222845, alm->M0);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(0.0002918243408203125, alm->ecc);
         TUASSERTFE(5153.60791015625, alm->Ahalf);
         TUASSERTFE(5153.60791015625 * 5153.60791015625, alm->A);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(-0.63010732608789443709, alm->OMEGA0);
         TUASSERTFE(0.018239990305952701738+gnsstk::GPSLNavData::refioffsetGPS,
                    alm->i0);
         TUASSERTFE(-0.27279354095737701202, alm->w);
         TUASSERTFE(-8.1031946734407515747e-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-0.0001068115234375, alm->af0);
         TUASSERTFE(-1.4551915228366851807e-11, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
            // GPSLNavData fields
         TUASSERTE(uint32_t, 0x8b, alm->pre);
         TUASSERTE(uint32_t, 0x34d, alm->tlm);
         TUASSERTE(bool, false, alm->isf);
         TUASSERTE(bool, false, alm->alert);
         TUASSERTE(bool, true, alm->asFlag);
            // GPSLNavAlm fields
         TUASSERTE(unsigned, 0, alm->healthBits);
         TUASSERTFE(0.018239990305952701738, alm->deltai);
         TUASSERTFE(147456, alm->toa);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 1, 0, 0, 1);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processSVID51Test()
{
   TUDEF("PNBGPSLNavDataFactory", "processSVID51");
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                            gnsstk::NavType::GPSLNAV),
      gnsstk::NavMessageType::Health);
   gnsstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.processSVID51(pg51LNAVGPS, navOut));
   TUASSERTE(size_t, 24, navOut.size());
      // sv/page ID 51 contains health information for 24 satellites,
      // starting with PRN 1.
   unsigned subjPRN = 1;
   for (const auto& i : navOut)
   {
      nmidExp.sat.id = subjPRN++;
         // Yes this code can cause seg faults on failure, but that's ok.
      gnsstk::GPSLNavHealth *hea =
         dynamic_cast<gnsstk::GPSLNavHealth*>(i.get());
      TUASSERT(hea != nullptr);
         // NavData fields
      TUASSERTE(gnsstk::CommonTime, pg51LNAVGPSct, hea->timeStamp);
      TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
         // NavHealthData has no fields
         // GPSLNavHealth
         // Stored as uint8_t but I want failures to print numbers,
         // not characters, so I use unsigned.
         // PRN 10 is unhealthy, but we've already incremented subjPRN
         // so compare it to 11.
      TUASSERTE(unsigned, (subjPRN == 11 ? 0x3f : 0), hea->svHealth);
   }
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processSVID63Test()
{
   TUDEF("PNBGPSLNavDataFactory", "processSVID63");
   gnsstk::PNBGPSLNavDataFactory uut;

   // Add system messages to the default NavMessageTypeSet.
   gnsstk::NavMessageTypeSet nmts{gnsstk::allNavMessageTypes};
   nmts.emplace(gnsstk::NavMessageType::System);
   uut.setTypeFilter(nmts);

   gnsstk::NavSatelliteID nsidExp{
      1,
      1,
      gnsstk::SatelliteSystem::GPS,
      gnsstk::CarrierBand::L1,
      gnsstk::TrackingCode::CA,
      gnsstk::NavType::GPSLNAV};
   gnsstk::NavMessageID nmidHealthExp{nsidExp, gnsstk::NavMessageType::Health};
   gnsstk::NavMessageID nmidConfExp{nsidExp, gnsstk::NavMessageType::System};

   gnsstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.processSVID63(pg63LNAVGPS, navOut));
   TUASSERTE(size_t, 8 + 32, navOut.size());
      // sv/page ID 63 contains health information for 8 satellites,
      // starting with PRN 25, and SV config information for 32 satellites.
   unsigned healthPRN = 25;
   unsigned configPRN = 1;
   for (const auto& i : navOut)
   {
      if (auto hea = std::dynamic_pointer_cast<gnsstk::GPSLNavHealth>(i))
      {
         nmidHealthExp.sat.id = healthPRN++;
         TUASSERTE(gnsstk::CommonTime, pg63LNAVGPSct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidHealthExp, hea->signal);
            // NavHealthData has no fields
            // GPSLNavHealth
            // Stored as uint8_t but I want failures to print numbers,
            // not characters, so I use unsigned.
            /** @todo Add a test for SVID63 where there's actually an
             * unhealthy satellite. */
         TUASSERTE(unsigned, 0, hea->svHealth);
      }
      else if (auto conf = std::dynamic_pointer_cast<gnsstk::GPSNavConfig>(i))
      {
         nmidConfExp.sat.id = configPRN++;
         TUASSERTE(gnsstk::CommonTime, pg63LNAVGPSct, conf->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidConfExp, conf->signal);
            // SystemNavData has no fields
            // The best we can do for GPSNavConfig is assert getSVConfig() is
            // not noInfo, and that antispoof is on in our test data.
         TUASSERT(conf->antispoofOn);
         TUASSERT(conf->getSVConfig() != gnsstk::GPSSVConfig::noInfo);
      }
      else
      {
         TUFAIL("Could not cast to either GPSLNavHealth or GPSNavConfig!");
      }
   }
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processSVID56Test()
{
   TUDEF("PNBGPSLNavDataFactory", "processSVID56");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                            gnsstk::NavType::GPSLNAV),
      gnsstk::NavMessageType::TimeOffset);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSLNavTimeOffset *to;
   gnsstk::GPSLNavIono *iono;
   TUASSERTE(bool, true, uut.processSVID56(pg56LNAVGPS, navOut));
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gnsstk::GPSLNavTimeOffset*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::TimeOffset;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, pg56LNAVGPSct, to->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
            // TimeOffsetData has no fields
            // GPSLNavTimeOffset fields
         TUASSERTFE(17, to->deltatLS);
         TUASSERTFE(4.65661287E-09, to->a0);
         TUASSERTFE(1.42108547E-14, to->a1);
         TUASSERTFE(147456, to->tot);
         TUASSERTE(unsigned, 1869, to->wnot);
         TUASSERTE(unsigned, 1851, to->wnLSF);
         TUASSERTE(gnsstk::CommonTime,
                   gnsstk::GPSWeekSecond(1869,147456).convertToCommonTime(),
                   to->refTime);
         TUASSERTE(unsigned, 3, to->dn);
         TUASSERTFE(17, to->deltatLSF);
      }
      else if ((iono = dynamic_cast<gnsstk::GPSLNavIono*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Iono;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, pg56LNAVGPSct, iono->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, iono->signal);
         TUASSERTE(uint32_t, 0x8b, iono->pre);
         TUASSERTE(uint32_t, 845, iono->tlm);
         TUASSERTE(bool, false, iono->isf);
         TUASSERTE(bool, false, iono->alert);
         TUASSERTE(bool, true, iono->asFlag);
         TUASSERTFE( 2.142041922e-08, iono->alpha[0]);
         TUASSERTFE(-7.450580597e-09, iono->alpha[1]);
         TUASSERTFE(-1.192092896e-07, iono->alpha[2]);
         TUASSERTFE( 1.192092896e-07, iono->alpha[3]);
         TUASSERTFE( 1.290240000e+05, iono->beta[0]);
         TUASSERTFE(-4.915200000e+04, iono->beta[1]);
         TUASSERTFE(-1.966080000e+05, iono->beta[2]);
         TUASSERTFE( 6.553600000e+04, iono->beta[3]);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 1, 0, 1);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
addDataAllQZSSTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData(QZSS)");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
      // Add subframe 1, expect 1 health and 1 ISC.
   TUASSERTE(bool, true, uut.addData(ephLNAVQZSSSF1, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 0, 1, 0, 1);
      // add subframe 2, expect nothing yet
   TUASSERTE(bool, true, uut.addData(ephLNAVQZSSSF2, navOut));
   fc.validateResults(navOut, __LINE__);
      // add subframe 3, expect the completed ephemeris
   TUASSERTE(bool, true, uut.addData(ephLNAVQZSSSF3, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
      // add almanac sv id 1, expect health
   TUASSERTE(bool, true, uut.addData(almLNAVQZSS1, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
      // add almanac sv id 2, expect health
   TUASSERTE(bool, true, uut.addData(almLNAVQZSS2, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
      // add page 56, expect time offset and iono corrections
   TUASSERTE(bool, true, uut.addData(pg56LNAVQZSS, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 1, 0, 1);
      // add page 51, expect 10 health and 2 almanacs
   TUASSERTE(bool, true, uut.addData(pg51LNAVQZSS, navOut));
   fc.validateResults(navOut, __LINE__, 12, 2, 0, 0, 10);
      // add page 61, expect time offset and iono corrections
   TUASSERTE(bool, true, uut.addData(pg61LNAVQZSS, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 1, 0, 1);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processEphQZSSTest()
{
   TUDEF("PNBGPSLNavDataFactory", "processEph(QZSS)");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                            gnsstk::NavType::GPSLNAV),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime toeExp = gnsstk::GPSWeekSecond(2118,522000,
                                                   gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime beginExp = gnsstk::GPSWeekSecond(2118,518400,
                                                     gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime endExp = gnsstk::GPSWeekSecond(2118,530100,
                                                   gnsstk::TimeSystem::QZS);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSLNavHealth *hea;
   gnsstk::GPSLNavEph *eph;
   gnsstk::GPSLNavISC *isc;
      // success, 1 health data and 1 ISC
   TUASSERTE(bool, true, uut.processEph(1, ephLNAVQZSSSF1, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GPSLNavHealth*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Health;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephLNAVQZSSSF1ct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // GPSLNavHealth
            // Stored as uint8_t but I want failures to print numbers,
            // not characters, so I use unsigned.
         TUASSERTE(unsigned, 0, hea->svHealth);
      }
      else if ((isc = dynamic_cast<gnsstk::GPSLNavISC*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::ISC;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephLNAVQZSSSF1ct, isc->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, isc->signal);
            // InterSigCorr
         TUASSERTFE(-5.5879354476928710938e-09, isc->isc);
            // GPSLNavISC
         TUASSERTE(uint32_t, 0x8b, isc->pre);
         TUASSERTE(uint32_t, 0xaa, isc->tlm);
         TUASSERTE(bool, false, isc->isf);
         TUASSERTE(bool, false, isc->alert);
         TUASSERTE(bool, false, isc->asFlag);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 0, 1, 0, 1);
   TUASSERTE(bool, true, uut.processEph(2, ephLNAVQZSSSF2, navOut));
   fc.validateResults(navOut, __LINE__);
      // success, and we have an ephemeris.
   TUASSERTE(bool, true, uut.processEph(3, ephLNAVQZSSSF3, navOut));
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gnsstk::GPSLNavEph*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Ephemeris;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, ephLNAVQZSSSF1ct, eph->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, eph->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, ephLNAVQZSSSF1ct, eph->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, eph->health);
         TUASSERTFE(-3.25962901E-07, eph->Cuc);
         TUASSERTFE(1.3930723071098327637E-05, eph->Cus);
         TUASSERTFE(-2.38843750E+02, eph->Crc);
         TUASSERTFE(-3.04062500E+01, eph->Crs);
         TUASSERTFE(-5.36441803E-07, eph->Cis);
         TUASSERTFE(-9.12696123E-08, eph->Cic);
         TUASSERTFE(1.1030415015803312073E+00, eph->M0);
         TUASSERTFE(2.43152985E-09, eph->dn);
         TUASSERTFE(0, eph->dndot);
         TUASSERTFE(7.5891476939432322979E-02, eph->ecc);
         TUASSERTFE(::sqrt(4.2168440525536708534E+07), eph->Ahalf);
         TUASSERTFE(4.2168440525536708534E+07, eph->A);
         TUASSERTFE(0, eph->Adot);
         TUASSERTFE(-3.0561569811139608355E+00, eph->OMEGA0);
         TUASSERTFE(7.2813125210358609074E-01, eph->i0);
         TUASSERTFE(-1.5543714330778595212E+00, eph->w);
         TUASSERTFE(-2.63439545E-09, eph->OMEGAdot);
         TUASSERTFE(9.47539469E-10, eph->idot);
         TUASSERTFE(-1.8891552463173866272E-04, eph->af0);
         TUASSERTFE(4.09272616E-12, eph->af1);
         TUASSERTFE(0, eph->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
            // GPSLNavData fields
         TUASSERTE(uint32_t, 0x8b, eph->pre);
         TUASSERTE(uint32_t, 0xaa, eph->tlm);
         TUASSERTE(bool, false, eph->isf);
         TUASSERTE(bool, false, eph->alert);
         TUASSERTE(bool, false, eph->asFlag);
            // GPSLNavEph fields
         TUASSERTE(uint32_t, 0x8b, eph->pre2);
         TUASSERTE(uint32_t, 0xaa, eph->tlm2);
         TUASSERTE(uint32_t, 0x8b, eph->pre3);
         TUASSERTE(uint32_t, 0xaa, eph->tlm3);
         TUASSERTE(bool, false, eph->isf2);
         TUASSERTE(bool, false, eph->isf3);
         TUASSERTE(uint16_t, 0x381, eph->iodc);
         TUASSERTE(uint16_t, 0x81, eph->iode);
         TUASSERTE(unsigned, 0, eph->fitIntFlag);
         TUASSERTE(unsigned, 0, eph->healthBits);
         TUASSERTE(unsigned, 1, eph->uraIndex);
         TUASSERTFE(-5.5879354476928710938E-09, eph->tgd);
         TUASSERTE(bool, false, eph->alert2);
         TUASSERTE(bool, false, eph->alert3);
         TUASSERTE(bool, false, eph->asFlag2);
         TUASSERTE(bool, false, eph->asFlag3);
         TUASSERTE(gnsstk::GPSLNavL2Codes,
                   gnsstk::GPSLNavL2Codes::CAcode, eph->codesL2);
         TUASSERTE(bool, true, eph->L2Pdata);
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processAlmOrbQZSSTest()
{
   TUDEF("PNBGPSLNavDataFactory", "processAlmOrb(QZSS)");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                            gnsstk::NavType::GPSLNAV),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime toaExp = gnsstk::GPSWeekSecond(2119,86016);
   gnsstk::CommonTime beginExp = toaExp - (70.0 * 3600.0);
   gnsstk::CommonTime endExp = toaExp + (74.0 * 3600.0);
   gnsstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.processAlmOrb(193, almLNAVQZSS1, navOut));
   TUASSERTE(size_t, 1, navOut.size());
      // no page 51 yet so we only get the health data back
   gnsstk::GPSLNavHealth *hea =
      dynamic_cast<gnsstk::GPSLNavHealth*>(navOut.begin()->get());
   TUASSERT(hea != nullptr);
      // NavData fields
   TUASSERTE(gnsstk::CommonTime, almLNAVQZSS1ct, hea->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
      // NavHealthData has no fields
      // GPSLNavHealth
   TUASSERTE(unsigned, 0, hea->svHealth);
   TUCSM("processSVID51()");
   navOut.clear();
   TUASSERTE(bool, true, uut.processSVID51(pg51LNAVQZSS, navOut));
      // sv/page ID 51 contains health information for 5 satellites,
      // starting with PRN 193.
   unsigned subjPRN = 193;
   gnsstk::GPSLNavAlm *alm;
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GPSLNavHealth*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Health;
         nmidExp.sat.id = subjPRN;
            // Yes this code can cause seg faults on failure, but that's ok.
         gnsstk::GPSLNavHealth *hea =
            dynamic_cast<gnsstk::GPSLNavHealth*>(i.get());
         TUASSERT(hea != nullptr);
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, pg51LNAVQZSSct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // GPSLNavHealth
         switch (subjPRN)
         {
            case 193:
            case 194:
            case 195:
            case 199:
               TUASSERTE(unsigned, 0, hea->svHealth);
               break;
            case 196:
            case 197:
            case 198:
            case 200:
            case 201:
            case 202:
               TUASSERTE(unsigned, 0x3e, hea->svHealth);
               break;
            default:
               TUFAIL("Unknown subjPRN "+gnsstk::StringUtils::asString(subjPRN));
               break;
         }
         subjPRN++;
      }
      else if ((alm = dynamic_cast<gnsstk::GPSLNavAlm*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Almanac;
            // make sure to reset the expected sat ID to the
            // appropriate value for the almanac page.
         nmidExp.sat.id = 193;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, almLNAVQZSS1ct, alm->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, almLNAVQZSS1ct, alm->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toe);
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, alm->health);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(8.4556833509804563143E-01, alm->M0);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(1.5868663787841796875E-02 + gnsstk::GPSLNavData::refEccQZSS,
                    alm->ecc);
         TUASSERTFE(6.493703125E+03, alm->Ahalf);
         TUASSERTFE(6.493703125E+03 * 6.493703125E+03, alm->A);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(3.1062594134985190841E+00, alm->OMEGA0);
         TUASSERTFE(-5.7116815898929418349E-02 +
                    gnsstk::GPSLNavData::refioffsetQZSS,
                    alm->i0);
         TUASSERTFE(-1.5547277282389830866E+00, alm->w);
         TUASSERTFE(-2.17151902E-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-1.8787384033203125E-04, alm->af0);
         TUASSERTFE(3.63797881E-12, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
            // GPSLNavData fields
         TUASSERTE(uint32_t, 0x8b, alm->pre);
         TUASSERTE(uint32_t, 0xaa, alm->tlm);
         TUASSERTE(bool, false, alm->isf);
         TUASSERTE(bool, false, alm->alert);
         TUASSERTE(bool, false, alm->asFlag);
            // GPSLNavAlm fields
         TUASSERTE(unsigned, 0, alm->healthBits);
         TUASSERTFE(-5.7116815898929418349E-02, alm->deltai);
         TUASSERTFE(86016, alm->toa);
      }
   }
      // 1 GPSLNavAlm and 10 GPSLNavHealth.
   fc.validateResults(navOut, __LINE__, 11, 1, 0, 0, 10);
      // Now that a page 51 has been loaded, we should immediately get
      // both the almanac and health objects in return.
   TUCSM("processAlmOrb");
   TUASSERTE(bool, true, uut.processAlmOrb(194, almLNAVQZSS2, navOut));
   nmidExp.sat.id = 194;
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GPSLNavHealth*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Health;
            // Yes this code can cause seg faults on failure, but that's ok.
         gnsstk::GPSLNavHealth *hea =
            dynamic_cast<gnsstk::GPSLNavHealth*>(i.get());
         TUASSERT(hea != nullptr);
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, almLNAVQZSS2ct, hea->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // GPSLNavHealth
            // Stored as uint8_t but I want failures to print numbers,
            // not characters, so I use unsigned.
         TUASSERTE(unsigned, 0, hea->svHealth);
      }
      else if ((alm = dynamic_cast<gnsstk::GPSLNavAlm*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Almanac;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, almLNAVQZSS2ct, alm->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, almLNAVQZSS2ct, alm->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toe);
         TUASSERTE(gnsstk::CommonTime, toaExp, alm->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, alm->health);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(-1.3581512361304759473E+00, alm->M0);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(1.51500701904296875E-02 + gnsstk::GPSLNavData::refEccQZSS,
                    alm->ecc);
         TUASSERTFE(6.49335595703125E+03, alm->Ahalf);
         TUASSERTFE(6.49335595703125E+03 * 6.49335595703125E+03, alm->A);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(-8.9252926940374188725E-01, alm->OMEGA0);
         TUASSERTFE(-3.7139112981700014848E-02 +
                    gnsstk::GPSLNavData::refioffsetQZSS,
                    alm->i0);
         TUASSERTFE(-1.5782527617281980614E+00, alm->w);
         TUASSERTFE(-2.49153235E-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-2.86102294921875E-06, alm->af0);
         TUASSERTFE(0, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
            // GPSLNavData fields
         TUASSERTE(uint32_t, 0x8b, alm->pre);
         TUASSERTE(uint32_t, 0xaa, alm->tlm);
         TUASSERTE(bool, false, alm->isf);
         TUASSERTE(bool, false, alm->alert);
         TUASSERTE(bool, false, alm->asFlag);
            // GPSLNavAlm fields
         TUASSERTE(unsigned, 0, alm->healthBits);
         TUASSERTFE(-3.7139112981700014848E-02, alm->deltai);
         TUASSERTFE(86016, alm->toa);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 1, 0, 0, 1);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processSVID51QZSSTest()
{
   TUDEF("PNBGPSLNavDataFactory", "processSVID51(QZSS)");
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                            gnsstk::NavType::GPSLNAV),
      gnsstk::NavMessageType::Health);
   gnsstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.processSVID51(pg51LNAVQZSS, navOut));
   TUASSERTE(size_t, 10, navOut.size());
      // sv/page ID 51 contains health information for 10 satellites,
      // starting with PRN 193.
   unsigned subjPRN = 193;
   for (const auto& i : navOut)
   {
      nmidExp.sat.id = subjPRN;
         // Yes this code can cause seg faults on failure, but that's ok.
      gnsstk::GPSLNavHealth *hea =
         dynamic_cast<gnsstk::GPSLNavHealth*>(i.get());
      TUASSERT(hea != nullptr);
         // NavData fields
      TUASSERTE(gnsstk::CommonTime, pg51LNAVQZSSct, hea->timeStamp);
      TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
         // NavHealthData has no fields
         // GPSLNavHealth
      switch (subjPRN)
      {
         case 193:
         case 194:
         case 195:
         case 199:
            TUASSERTE(unsigned, 0, hea->svHealth);
            break;
         case 196:
         case 197:
         case 198:
         case 200:
         case 201:
         case 202:
            TUASSERTE(unsigned, 0x3e, hea->svHealth);
            break;
         default:
            TUFAIL("Unknown subjPRN "+gnsstk::StringUtils::asString(subjPRN));
            break;
      }
      subjPRN++;
   }
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processSVID56QZSSTest()
{
   TUDEF("PNBGPSLNavDataFactory", "processSVID56(QZSS)");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSLNavDataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                            gnsstk::NavType::GPSLNAV),
      gnsstk::NavMessageType::TimeOffset);
   gnsstk::CommonTime expRef = gnsstk::CivilTime(2020,8,17,2,10,8,
                                               gnsstk::TimeSystem::QZS);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSLNavTimeOffset *to;
   gnsstk::GPSLNavIono *iono;
      // using page 56
   TUASSERTE(bool, true, uut.processSVID56(pg56LNAVQZSS, navOut));
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gnsstk::GPSLNavTimeOffset*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::TimeOffset;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, pg56LNAVQZSSct, to->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
            // TimeOffsetData has no fields
            // StdNavTimeOffset fields
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::QZS, to->src);
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC, to->tgt);
         TUASSERTFE(2.79396772E-09, to->a0);
         TUASSERTFE(0.00000000E+00, to->a1);
         TUASSERTFE(0.00000000E+00, to->a2);
         TUASSERTFE(18, to->deltatLS);
         TUASSERTE(gnsstk::CommonTime, expRef, to->refTime);
         TUASSERTFE(94208, to->tot);
         TUASSERTE(unsigned, 2119, to->wnot);
         TUASSERTE(unsigned, 2185, to->wnLSF);
         TUASSERTE(unsigned, 7, to->dn);
         TUASSERTFE(18, to->deltatLSF);
      }
      else if ((iono = dynamic_cast<gnsstk::GPSLNavIono*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Iono;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, pg56LNAVQZSSct, iono->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, iono->signal);
         TUASSERTE(uint32_t, 0x8b, iono->pre);
         TUASSERTE(uint32_t, 170, iono->tlm);
         TUASSERTE(bool, false, iono->isf);
         TUASSERTE(bool, false, iono->alert);
         TUASSERTE(bool, false, iono->asFlag);
         TUASSERTFE( 5.587935448e-09, iono->alpha[0]);
         TUASSERTFE(-7.450580597e-09, iono->alpha[1]);
         TUASSERTFE(-4.768371582e-07, iono->alpha[2]);
         TUASSERTFE(-5.364418030e-07, iono->alpha[3]);
         TUASSERTFE( 6.963200000e+04, iono->beta[0]);
         TUASSERTFE(-5.242880000e+05, iono->beta[1]);
         TUASSERTFE(-6.094848000e+06, iono->beta[2]);
         TUASSERTFE( 8.323072000e+06, iono->beta[3]);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 1, 0, 1);
      // using page 61
   TUASSERTE(bool, true, uut.processSVID56(pg61LNAVQZSS, navOut));
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gnsstk::GPSLNavTimeOffset*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::TimeOffset;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, pg61LNAVQZSSct, to->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
            // TimeOffsetData has no fields
            // StdNavTimeOffset fields
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::QZS, to->src);
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC, to->tgt);
         TUASSERTFE(2.79396772E-09, to->a0);
         TUASSERTFE(0.00000000E+00, to->a1);
         TUASSERTFE(0.00000000E+00, to->a2);
         TUASSERTFE(18, to->deltatLS);
         TUASSERTE(gnsstk::CommonTime, expRef, to->refTime);
         TUASSERTFE(94208, to->tot);
         TUASSERTE(unsigned, 2119, to->wnot);
         TUASSERTE(unsigned, 2185, to->wnLSF);
         TUASSERTE(unsigned, 7, to->dn);
         TUASSERTFE(18, to->deltatLSF);
      }
      else if ((iono = dynamic_cast<gnsstk::GPSLNavIono*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Iono;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, pg61LNAVQZSSct, iono->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, iono->signal);
         TUASSERTE(uint32_t, 0x8b, iono->pre);
         TUASSERTE(uint32_t, 170, iono->tlm);
         TUASSERTE(bool, false, iono->isf);
         TUASSERTE(bool, false, iono->alert);
         TUASSERTE(bool, false, iono->asFlag);
         TUASSERTFE( 1.024454832e-08, iono->alpha[0]);
         TUASSERTFE(-5.960464478e-08, iono->alpha[1]);
         TUASSERTFE(-2.980232239e-07, iono->alpha[2]);
         TUASSERTFE( 0.000000000e+00, iono->alpha[3]);
         TUASSERTFE( 7.782400000e+04, iono->beta[0]);
         TUASSERTFE( 3.112960000e+05, iono->beta[1]);
         TUASSERTFE(-8.257536000e+06, iono->beta[2]);
         TUASSERTFE( 8.323072000e+06, iono->beta[3]);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 1, 0, 1);
   TURETURN();
}


int main()
{
   PNBGPSLNavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.addDataAllTest();
   errorTotal += testClass.addDataAlmanacTest();
   errorTotal += testClass.addDataEphemerisTest();
   errorTotal += testClass.addDataHealthTest();
   errorTotal += testClass.addDataTimeTest();
   errorTotal += testClass.addDataEphHealthTest();
   errorTotal += testClass.addDataValidityTest();
   errorTotal += testClass.processEphTest();
   errorTotal += testClass.processAlmOrbTest();
   errorTotal += testClass.processSVID51Test();
   errorTotal += testClass.processSVID63Test();
   errorTotal += testClass.processSVID56Test();
   errorTotal += testClass.addDataAllQZSSTest();
   errorTotal += testClass.processEphQZSSTest();
   errorTotal += testClass.processAlmOrbQZSSTest();
   errorTotal += testClass.processSVID51QZSSTest();
   errorTotal += testClass.processSVID56QZSSTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
