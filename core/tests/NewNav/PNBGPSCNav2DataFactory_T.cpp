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
#include "PNBGPSCNav2DataFactory.hpp"
#include "TestUtil.hpp"
#include "GPSCNav2TimeOffset.hpp"
#include "GPSCNav2Health.hpp"
#include "GPSCNav2Eph.hpp"
#include "GPSCNav2Alm.hpp"
//#include "GPSCNav2RedAlm.hpp"
#include "GPSCNav2Iono.hpp"
#include "GPSCNav2ISC.hpp"
#include "TimeString.hpp"

using namespace std;

// avoid having to type out template params over and over.
using GPSFactoryCounter = FactoryCounter<gnsstk::GPSCNav2Alm,gnsstk::GPSCNav2Eph,gnsstk::GPSCNav2TimeOffset,gnsstk::GPSCNav2Health,gnsstk::GPSCNav2Iono,gnsstk::GPSCNav2ISC>;

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
}


class PNBGPSCNav2DataFactory_T
{
public:
   PNBGPSCNav2DataFactory_T();

#include "CNav2TestDataDecl.hpp"

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
   unsigned processUTCIonoTest();
   unsigned processGGTOEOPTest();
   unsigned processEphQZSSTest();
   unsigned processAlmOrbQZSSTest();
   unsigned processUTCIonoQZSSTest();
   unsigned processGGTOEOPQZSSTest();
   unsigned processISCTest();

   void validateAlmGPS(gnsstk::TestUtil& testFramework,
                       const gnsstk::GPSCNav2Alm* alm,
                       const gnsstk::CommonTime& expTime);
   void validateEphGPS(gnsstk::TestUtil& testFramework,
                       const gnsstk::GPSCNav2Eph* eph,
                       const gnsstk::CommonTime& expTime);
   void validateTOGPS(gnsstk::TestUtil& testFramework,
                      const gnsstk::GPSCNav2TimeOffset* to);
   void validateEphHealthGPS(gnsstk::TestUtil& testFramework,
                             const gnsstk::GPSCNav2Health* hea,
                             const gnsstk::CommonTime& expTime);
   void validateAlmHealthGPS(gnsstk::TestUtil& testFramework,
                             const gnsstk::GPSCNav2Health* hea,
                             const gnsstk::CommonTime& expTime);
   void validateIonoGPS(gnsstk::TestUtil& testFramework,
                        const gnsstk::GPSCNav2Iono* iono);
   void validateISCGPS(gnsstk::TestUtil& testFramework,
                       const gnsstk::GPSCNav2ISC* isc);
};


PNBGPSCNav2DataFactory_T ::
PNBGPSCNav2DataFactory_T()
{
#include "CNav2TestDataDef.hpp"
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataAllTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::SatID gloSid(1,gnsstk::SatelliteSystem::Glonass);
   gnsstk::ObsID gloid(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::G1,
                      gnsstk::TrackingCode::Standard);
   gnsstk::NavID gloNav(gnsstk::NavType::GloCivilF);
   gnsstk::PackedNavBitsPtr nonGPS =
      std::make_shared<gnsstk::PackedNavBits>(gloSid,gloid,gloNav,"XX",
                                             sf2CNAV2GPSct);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSCNav2Alm *alm;
   gnsstk::GPSCNav2Eph *eph;
   gnsstk::GPSCNav2TimeOffset *to;
   gnsstk::GPSCNav2Health *hea;
   gnsstk::GPSCNav2Iono *iono;
   gnsstk::GPSCNav2ISC *isc;
      // should refuse non-GPS data
   TUASSERTE(bool, false, uut.addData(nonGPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get 1 health and 1 ephemeris from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 1, 0, 1);
      // get 1 time offset, 1 iono and 1 ISC from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 1, 0, 1, 1);
      // get 0 time offsets from subframe 3 page 2 (because no data available)
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get 3 health and 1 almanac from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__, 4, 1, 0, 0, 3);
      //
      // QZSS CNAV2 data
      //
      // get 1 health and 1 ephemeris from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2QZSS, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 1, 0, 1);
      // get 1 time offset, 1 iono and 1 ISC from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2QZSS, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 1, 0, 1, 1);
      // get 1 time offset from subframe 3 page 2
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2QZSS, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
      // get 3 health and 1 almanac from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2QZSS, navOut));
   fc.validateResults(navOut, __LINE__, 4, 1, 0, 0, 3);
      //
      // GPS with a complete message including sf3p1
      //
   uut.resetState();
   TUASSERTE(bool, true, uut.addData(sf123p1CNAV2GPS, navOut));
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gnsstk::GPSCNav2Eph*>(i.get())) != nullptr)
      {
         validateEphGPS(testFramework, eph, sf123p1CNAV2GPSct + 0.52);
      }
      else if ((to = dynamic_cast<gnsstk::GPSCNav2TimeOffset*>(i.get()))
               != nullptr)
      {
         validateTOGPS(testFramework, to);
      }
      else if ((hea = dynamic_cast<gnsstk::GPSCNav2Health*>(i.get())) != nullptr)
      {
         validateEphHealthGPS(testFramework, hea, sf123p1CNAV2GPSct + 0.52);
      }
      else if ((iono = dynamic_cast<gnsstk::GPSCNav2Iono*>(i.get())) != nullptr)
      {
         validateIonoGPS(testFramework, iono);
      }
      else if ((isc = dynamic_cast<gnsstk::GPSCNav2ISC*>(i.get())) != nullptr)
      {
         validateISCGPS(testFramework, isc);
      }
      else
      {
         TUFAIL("What is this?");
      }
   }
      // sf2: ephemeris, health
      // sf3p1: time offset, iono, ISC
   fc.validateResults(navOut, __LINE__, 5, 0, 1, 1, 1, 1, 1);
      //
      // GPS with a complete message including sf3p2
      //
   uut.resetState();
   TUASSERTE(bool, true, uut.addData(sf123p2CNAV2GPS, navOut));
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gnsstk::GPSCNav2Eph*>(i.get())) != nullptr)
      {
         validateEphGPS(testFramework, eph, sf123p2CNAV2GPSct + 0.52);
      }
      else if ((hea = dynamic_cast<gnsstk::GPSCNav2Health*>(i.get())) != nullptr)
      {
         validateEphHealthGPS(testFramework, hea, sf123p2CNAV2GPSct + 0.52);
      }
      else
      {
         TUFAIL("What is this?");
      }
   }
      // sf2: ephemeris, health
      // sf3p2: NO time offset (no data available)
   fc.validateResults(navOut, __LINE__, 2, 0, 1, 0, 1);
      //
      // GPS with a complete message including sf3p4
      //
   uut.resetState();
   TUASSERTE(bool, true, uut.addData(sf123p4CNAV2GPS, navOut));
   for (const auto& i : navOut)
   {
      if ((alm = dynamic_cast<gnsstk::GPSCNav2Alm*>(i.get())) != nullptr)
      {
         validateAlmGPS(testFramework, alm, sf123p4CNAV2GPSct + 12.52);
      }
      else if ((eph = dynamic_cast<gnsstk::GPSCNav2Eph*>(i.get())) != nullptr)
      {
         validateEphGPS(testFramework, eph, sf123p4CNAV2GPSct + 0.52);
      }
      else if ((hea = dynamic_cast<gnsstk::GPSCNav2Health*>(i.get())) != nullptr)
      {
         if (hea->signal.sat.id == 4)
         {
            validateEphHealthGPS(testFramework, hea, sf123p4CNAV2GPSct + 0.52);
         }
         else
         {
            validateAlmHealthGPS(testFramework, hea, sf123p4CNAV2GPSct + 12.52);
         }
      }
      else
      {
         TUFAIL("What is this?");
      }
   }
      // sf2: ephemeris, health
      // sf3p4: almanac, 3x health
   fc.validateResults(navOut, __LINE__, 6, 1, 1, 0, 4);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataEphemerisTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris}));
      // get 1 ephemeris from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 1);
      // get nothing from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get nothing from subframe 3 page 2
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get nothing from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataAlmanacTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Almanac}));
      // get nothing from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get nothing from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get nothing from subframe 3 page 2
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get 1 almanac from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__, 1, 1);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataHealthTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Health}));
      // get 1 health from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
      // get nothing from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get nothing from subframe 3 page 2
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get 3 health from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 3);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataTimeTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::TimeOffset}));
      // get nothing from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get 1 time offset from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
      // get nothing from subframe 3 page 2
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get nothing from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataEphHealthTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris,
                              gnsstk::NavMessageType::Health}));
      // get 1 ephemeris, 1 health from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__, 2, 0, 1, 0, 1);
      // get nothing from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get nothing from subframe 3 page 2
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get 3 health from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__, 3, 0, 0, 0, 3);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataAlmHealthTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gnsstk::NavMessageType::Almanac,
                              gnsstk::NavMessageType::Health}));
      // get 1 health from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 1);
      // get nothing from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get nothing from subframe 3 page 2
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // get 1 almanac, 3 health from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__, 4, 1, 0, 0, 3);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
processEphTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "processEph");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavMessageID nmidExpHea(
      gnsstk::NavSatelliteID(4, 4, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::Health);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSCNav2Health *hea;
   gnsstk::GPSCNav2Eph *eph;
      // success, health and ephemeris
   TUASSERTE(bool, true, uut.processEph(sf2CNAV2GPS, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GPSCNav2Health*>(i.get())) != nullptr)
      {
         validateEphHealthGPS(testFramework, hea, sf2CNAV2GPSct);
      }
      else if ((eph = dynamic_cast<gnsstk::GPSCNav2Eph*>(i.get())) != nullptr)
      {
         validateEphGPS(testFramework, eph, sf2CNAV2GPSct);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 0, 1, 0, 1);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
processAlmOrbTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "processAlmOrb");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSCNav2Health *hea;
   gnsstk::GPSCNav2Alm *alm;
      // success, health and almanac
   TUASSERTE(bool, true, uut.processAlmOrb(sf3p4CNAV2GPS, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GPSCNav2Health*>(i.get())) != nullptr)
      {
         validateAlmHealthGPS(testFramework, hea, sf3p4CNAV2GPSct);
      }
      else if ((alm = dynamic_cast<gnsstk::GPSCNav2Alm*>(i.get())) != nullptr)
      {
         validateAlmGPS(testFramework, alm, sf3p4CNAV2GPSct);
      }
   }
   fc.validateResults(navOut, __LINE__, 4, 1, 0, 0, 3);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
processUTCIonoTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "processUTCIono");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSCNav2TimeOffset *to;
   gnsstk::GPSCNav2Iono *iono;
      // success, time offset only
   TUASSERTE(bool, true, uut.processUTCIono(sf3p1CNAV2GPS, navOut));
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gnsstk::GPSCNav2TimeOffset*>(i.get())) != nullptr)
      {
         validateTOGPS(testFramework, to);
      }
      else if ((iono = dynamic_cast<gnsstk::GPSCNav2Iono*>(i.get())) != nullptr)
      {
         validateIonoGPS(testFramework, iono);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 1, 0, 1);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
processGGTOEOPTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "processGGTOEOP");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(4, 4, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::TimeOffset);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSCNav2TimeOffset *to;
      // make sure we have at least the right number of bits
   TUASSERTE(size_t, 274, sf3p2fakeCNAV2GPS->getNumBits());
      // success, time offset only
   TUASSERTE(bool, true, uut.processGGTOEOP(sf3p2fakeCNAV2GPS, navOut));
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gnsstk::GPSCNav2TimeOffset*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, sf3p2fakeCNAV2GPSct, to->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
            // TimeOffsetData has no fields
            // GPSCNav2TimeOffset
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GLO, to->tgt);
         TUASSERTFE(6.5192580223e-09, to->a0);
         TUASSERTFE(1.5099033135e-14, to->a1);
         TUASSERTFE(0, to->a2);
         TUASSERTFE(0, to->deltatLS);
         TUASSERTFE(3456, to->tot);
         TUASSERTE(unsigned, 2044, to->wnot);
         TUASSERTE(unsigned, 0, to->wnLSF);
         TUASSERTE(gnsstk::CommonTime,
                   gnsstk::GPSWeekSecond(2044,3456).convertToCommonTime(),
                   to->refTime);
         TUASSERTE(unsigned, 0, to->dn);
         TUASSERTFE(0, to->deltatLSF);
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
processEphQZSSTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "processEph(QZSS)");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavMessageID nmidExpHea(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpEph(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::Ephemeris);
   gnsstk::CommonTime toeExp = gnsstk::GPSWeekSecond(2155,327600,
                                                   gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime topExp = gnsstk::GPSWeekSecond(2155,327600,
                                                   gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime beginExp = gnsstk::GPSWeekSecond(2155,324504,
                                                     gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime endExp = gnsstk::GPSWeekSecond(2155,333300,
                                                   gnsstk::TimeSystem::QZS);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSCNav2Health *hea;
   gnsstk::GPSCNav2Eph *eph;
      // success, health and ephemeris
   TUASSERTE(bool, true, uut.processEph(sf2CNAV2QZSS, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GPSCNav2Health*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, sf2CNAV2QZSSct, hea->timeStamp);
         TUASSERTE(gnsstk::NavType, gnsstk::NavType::GPSCNAV2, hea->signal.nav);
         TUASSERTE(gnsstk::NavMessageID, nmidExpHea, hea->signal);
            // NavHealthData has no fields
            // GPSCNav2Health
         TUASSERTE(bool, false, hea->health);
      }
      else if ((eph = dynamic_cast<gnsstk::GPSCNav2Eph*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, sf2CNAV2QZSSct, eph->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExpEph, eph->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, sf2CNAV2QZSSct, eph->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, eph->health);
         TUASSERTFE( 8.66129994E-08, eph->Cuc);
         TUASSERTFE(-8.9611858129501342773E-06, eph->Cus);
         TUASSERTFE( 4.5444921875E+02, eph->Crc);
         TUASSERTFE(-4.06328125E+01, eph->Crs);
         TUASSERTFE( 1.5646219253540039062E-07, eph->Cic);
         TUASSERTFE( 1.37835741E-07, eph->Cis);
         TUASSERTFE(-2.2116779253087699431E+00, eph->M0);
         TUASSERTFE( 3.86105369E-09, eph->dn);
         TUASSERTFE(-6.05799160E-14, eph->dndot);
         TUASSERTFE( 7.5641123345121741295E-02, eph->ecc);
         TUASSERTFE(::sqrt(42164200-2493.421875), eph->Ahalf);
         TUASSERTFE( 42164200-2493.421875, eph->A);
         TUASSERTFE( 1.2159824371337890625E-02, eph->Adot);
         TUASSERTFE(-1.2915573706629632778E+00, eph->OMEGA0);
         TUASSERTFE( 7.3069146699824139013E-01, eph->i0);
         TUASSERTFE(-1.5648458687401483047E+00, eph->w);
         TUASSERTFE(-3.85563984E-09, eph->OMEGAdot);
         TUASSERTFE( 2.53046255E-10, eph->idot);
         TUASSERTFE(-3.7147360853850841522E-04, eph->af0);
         TUASSERTFE(-5.06616971E-12, eph->af1);
         TUASSERTFE( 0.00000000E+00, eph->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
            // GPSCNavEph fields
         TUASSERTE(bool, false, eph->healthL1C);
         TUASSERTE(int, -8, eph->uraED);
         TUASSERTE(int, -3, eph->uraNED0);
         TUASSERTE(unsigned, 0, eph->uraNED1);
         TUASSERTE(unsigned, 0, eph->uraNED2);
         TUASSERTE(bool, false, eph->integStat);
         TUASSERTFE(-2493.421875, eph->deltaA);
         TUASSERTFE(4.3125010615506153166e-09, eph->dOMEGAdot);
         TUASSERTE(gnsstk::CommonTime, topExp, eph->top);
         TUASSERTFE(-5.4715201258659362793e-09, eph->tgd);
         TUASSERTFE( 2.91038305E-10, eph->iscL1CP);
         TUASSERTFE( 4.36557457E-10, eph->iscL1CD);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 0, 1, 0, 1);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
processAlmOrbQZSSTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "processAlmOrb");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavMessageID nmidExpHeaL1(
      gnsstk::NavSatelliteID(195, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpHeaL2(
      gnsstk::NavSatelliteID(195, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CM,
                            gnsstk::NavType::GPSCNAVL2),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpHeaL5(
      gnsstk::NavSatelliteID(195, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I,
                            gnsstk::NavType::GPSCNAVL5),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpAlm(
      gnsstk::NavSatelliteID(195, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::Almanac);
   gnsstk::CommonTime toeExp = gnsstk::GPSWeekSecond(2155,520192,
                                                   gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime beginExp = sf3p4CNAV2QZSSct;
   gnsstk::CommonTime endExp = gnsstk::CommonTime::END_OF_TIME;
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSCNav2Health *hea;
   gnsstk::GPSCNav2Alm *alm;
   endExp.setTimeSystem(gnsstk::TimeSystem::QZS);
      // success, health and almanac
   TUASSERTE(bool, true, uut.processAlmOrb(sf3p4CNAV2QZSS, navOut));
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gnsstk::GPSCNav2Health*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, sf3p4CNAV2QZSSct, hea->timeStamp);
         bool expHea = false;
         switch (hea->signal.nav)
         {
            case gnsstk::NavType::GPSCNAV2:
               TUASSERTE(gnsstk::NavMessageID, nmidExpHeaL1, hea->signal);
               break;
            case gnsstk::NavType::GPSCNAVL2:
               TUASSERTE(gnsstk::NavMessageID, nmidExpHeaL2, hea->signal);
               break;
            case gnsstk::NavType::GPSCNAVL5:
               TUASSERTE(gnsstk::NavMessageID, nmidExpHeaL5, hea->signal);
               break;
            default:
               TUFAIL("Unexpected nav type: " +
                      gnsstk::StringUtils::asString(hea->signal.nav));
               break;
         }
            // NavHealthData has no fields
            // GPSCNav2Health
         TUASSERTE(bool, expHea, hea->health);
      }
      else if ((alm = dynamic_cast<gnsstk::GPSCNav2Alm*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, sf3p4CNAV2QZSSct, alm->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExpAlm, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gnsstk::CommonTime, sf3p4CNAV2QZSSct, alm->xmitTime);
         TUASSERTE(gnsstk::CommonTime, toeExp, alm->Toe);
         TUASSERTE(gnsstk::CommonTime, toeExp, alm->Toc); // same value as toe
         TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, alm->health);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE( 1.5375281184576268245E+00, alm->M0);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE( 1.44500732421875E-02, alm->ecc);
         TUASSERTFE( 6.4934375000E+03, alm->Ahalf);
         TUASSERTFE( 6.4934375000E+03*6.4934375000E+03, alm->A);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE( 2.6930950207317287948E+00, alm->OMEGA0);
         TUASSERTFE((0.25*gnsstk::PI)-7.2672339826082246739E-02, alm->i0);
         TUASSERTFE(-1.5638934132494111129E+00, alm->w);
         TUASSERTFE(-2.5601066387E-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-4.7683715820E-06, alm->af0);
         TUASSERTFE( 0.0000000000E+00, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
            // GPSCNavAlm fields
         TUASSERTE(bool, false, alm->healthL1);
         TUASSERTE(bool, false, alm->healthL2);
         TUASSERTE(bool, false, alm->healthL5);
         TUASSERTFE(-7.2672339826082246739E-02, alm->deltai);
         TUASSERTE(unsigned, 2155, alm->wna);
         TUASSERTFE(520192, alm->toa);
      }
   }
   fc.validateResults(navOut, __LINE__, 4, 1, 0, 0, 3);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
processUTCIonoQZSSTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "processUTCIono");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::TimeOffset);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSCNav2TimeOffset *to;
   gnsstk::GPSCNav2Iono *iono;
      // success, time offset and iono
   TUASSERTE(bool, true, uut.processUTCIono(sf3p1CNAV2QZSS, navOut));
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gnsstk::GPSCNav2TimeOffset*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::TimeOffset;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, sf3p1CNAV2QZSSct, to->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
            // TimeOffsetData has no fields
            // GPSCNav2TimeOffset
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC, to->tgt);
         TUASSERTFE(-1.0390067473e-08, to->a0);
         TUASSERTFE( 0.0000000000e+00, to->a1);
         TUASSERTFE( 0.0000000000e+00, to->a2);
         TUASSERTFE(18, to->deltatLS);
         TUASSERTFE(529200, to->tot);
         TUASSERTE(unsigned, 2155, to->wnot);
         TUASSERTE(unsigned, 1929, to->wnLSF);
         TUASSERTE(gnsstk::CommonTime,
                   gnsstk::GPSWeekSecond(
                      2155,529200,gnsstk::TimeSystem::QZS).convertToCommonTime(),
                   to->refTime);
         TUASSERTE(unsigned, 7, to->dn);
         TUASSERTFE(18, to->deltatLSF);
      }
      else if ((iono = dynamic_cast<gnsstk::GPSCNav2Iono*>(i.get())) != nullptr)
      {
         nmidExp.messageType = gnsstk::NavMessageType::Iono;
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, sf3p1CNAV2QZSSct, iono->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, iono->signal);
         TUASSERTFE( 1.303851600e-08, iono->alpha[0]);
         TUASSERTFE( 0.000000000e+00, iono->alpha[1]);
         TUASSERTFE(-4.172325133e-07, iono->alpha[2]);
         TUASSERTFE(-6.556510925e-07, iono->alpha[3]);
         TUASSERTFE( 1.044480000e+05, iono->beta[0]);
         TUASSERTFE(-2.293760000e+05, iono->beta[1]);
         TUASSERTFE(-3.538944000e+06, iono->beta[2]);
         TUASSERTFE( 8.323072000e+06, iono->beta[3]);
      }
   }
   fc.validateResults(navOut, __LINE__, 2, 0, 0, 1, 0, 1);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
processGGTOEOPQZSSTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "processGGTOEOP");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::TimeOffset);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSCNav2TimeOffset *to;
      // make sure we have at least the right number of bits
   TUASSERTE(size_t, 274, sf3p2CNAV2QZSS->getNumBits());
      // success, time offset only
   TUASSERTE(bool, true, uut.processGGTOEOP(sf3p2CNAV2QZSS, navOut));
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gnsstk::GPSCNav2TimeOffset*>(i.get())) != nullptr)
      {
            // NavData fields
         TUASSERTE(gnsstk::CommonTime, sf3p2CNAV2QZSSct, to->timeStamp);
         TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
            // TimeOffsetData has no fields
            // GPSCNav2TimeOffset
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::QZS, to->tgt);
         TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GPS, to->src);
         TUASSERTFE(0, to->a0);
         TUASSERTFE(0, to->a1);
         TUASSERTFE(0, to->a2);
         TUASSERTFE(0, to->deltatLS);
         TUASSERTFE(356400, to->tot);
         TUASSERTE(unsigned, 2155, to->wnot);
         TUASSERTE(unsigned, 0, to->wnLSF);
         TUASSERTE(gnsstk::CommonTime,
                   gnsstk::GPSWeekSecond(
                      2155,356400,gnsstk::TimeSystem::QZS).convertToCommonTime(),
                   to->refTime);
         TUASSERTE(unsigned, 0, to->dn);
         TUASSERTFE(0, to->deltatLSF);
      }
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 1);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
processISCTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "processISC");
   GPSFactoryCounter fc(testFramework);
   gnsstk::PNBGPSCNav2DataFactory uut;
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(4, 4, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::ISC);
   gnsstk::NavDataPtrList navOut;
   gnsstk::GPSCNav2ISC *isc;
      // test sequences:
      // SF2, SF2
      // SF2, SF3 (pg2), SF3 (pg1)
      // SF3, SF2
      // SF123
      // first test two SF2 in sequence
   TUASSERTE(bool, true, uut.processISC(sf2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processISC(sf2CNAV2GPS2, navOut));
   if ((isc = dynamic_cast<gnsstk::GPSCNav2ISC*>(navOut.begin()->get()))
       != nullptr)
   {
      TUASSERTE(gnsstk::CommonTime, sf2CNAV2GPSct, isc->timeStamp);
      TUASSERTE(gnsstk::NavMessageID, nmidExp, isc->signal);
      TUASSERTE(bool, true, isc->haveSF2);
      TUASSERTE(bool, false, isc->haveSF3);
      TUASSERTFE(-8.78935679793358e-09, isc->isc);
      TUASSERTFE(-7.85803422E-10, isc->iscL1CP);
      TUASSERTFE(-3.49245965E-10, isc->iscL1CD);
      TUASSERTE(bool, true, std::isnan(isc->iscL1CA));
      TUASSERTE(bool, true, std::isnan(isc->iscL2C));
      TUASSERTE(bool, true, std::isnan(isc->iscL5I5));
      TUASSERTE(bool, true, std::isnan(isc->iscL5Q5));
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 0, 0, 1);
      // Add a subframe 3 page 2
   TUASSERTE(bool, true, uut.processISC(sf3p2CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
      // Add a subframe 3 page 1
   TUASSERTE(bool, true, uut.processISC(sf3p1CNAV2GPS, navOut));
   if ((isc = dynamic_cast<gnsstk::GPSCNav2ISC*>(navOut.begin()->get()))
       != nullptr)
   {
      TUASSERTE(gnsstk::CommonTime, sf3p1CNAV2GPSct, isc->timeStamp);
      TUASSERTE(gnsstk::NavMessageID, nmidExp, isc->signal);
      TUASSERTE(bool, true, isc->haveSF2);
      TUASSERTE(bool, true, isc->haveSF3);
      TUASSERTFE(-8.78935679793358e-09, isc->isc);
      TUASSERTFE(-7.85803422E-10, isc->iscL1CP);
      TUASSERTFE(-3.49245965E-10, isc->iscL1CD);
      TUASSERTFE(-5.82076609e-10, isc->iscL1CA);
      TUASSERTFE(-6.60656951e-09, isc->iscL2C);
      TUASSERTFE(-1.17870513e-08, isc->iscL5I5);
      TUASSERTFE(-1.17870513e-08, isc->iscL5Q5);
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 0, 0, 1);
      // SF3 -> SF2
   uut.resetState();
   TUASSERTE(bool, true, uut.processISC(sf3p1CNAV2GPS, navOut));
   fc.validateResults(navOut, __LINE__);
   TUASSERTE(bool, true, uut.processISC(sf2CNAV2GPS2, navOut));
   if ((isc = dynamic_cast<gnsstk::GPSCNav2ISC*>(navOut.begin()->get()))
       != nullptr)
   {
      TUASSERTE(gnsstk::CommonTime, sf2CNAV2GPS2ct, isc->timeStamp);
      TUASSERTE(gnsstk::NavMessageID, nmidExp, isc->signal);
      TUASSERTE(bool, true, isc->haveSF2);
      TUASSERTE(bool, true, isc->haveSF3);
      TUASSERTFE(-8.78935679793358e-09, isc->isc);
      TUASSERTFE(-7.85803422E-10, isc->iscL1CP);
      TUASSERTFE(-3.49245965E-10, isc->iscL1CD);
      TUASSERTFE(-5.82076609e-10, isc->iscL1CA);
      TUASSERTFE(-6.60656951e-09, isc->iscL2C);
      TUASSERTFE(-1.17870513e-08, isc->iscL5I5);
      TUASSERTFE(-1.17870513e-08, isc->iscL5Q5);
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 0, 0, 1);
      // SF123
   uut.resetState();
   TUASSERTE(bool, true, uut.processISC(sf123p1CNAV2GPS, navOut));
   if ((isc = dynamic_cast<gnsstk::GPSCNav2ISC*>(navOut.begin()->get()))
       != nullptr)
   {
      validateISCGPS(testFramework, isc);
   }
   fc.validateResults(navOut, __LINE__, 1, 0, 0, 0, 0, 0, 1);
   TURETURN();
}


void PNBGPSCNav2DataFactory_T ::
validateEphGPS(gnsstk::TestUtil& testFramework,
               const gnsstk::GPSCNav2Eph* eph,
               const gnsstk::CommonTime& expTime)
{
   gnsstk::NavMessageID nmidExpEph(
      gnsstk::NavSatelliteID(4, 4, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::Ephemeris);
   gnsstk::CommonTime toeExp = gnsstk::GPSWeekSecond(2049,351000.0);
   gnsstk::CommonTime topExp = gnsstk::GPSWeekSecond(2049,321300.0);
   gnsstk::CommonTime beginExp = gnsstk::GPSWeekSecond(2049,345600.0);
   gnsstk::CommonTime endExp = gnsstk::GPSWeekSecond(2049,356400.0);
      // NavData fields
   TUASSERTE(gnsstk::CommonTime, expTime, eph->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpEph, eph->signal);
      // OrbitData has no fields
      // OrbitDataKepler fields
   TUASSERTE(gnsstk::CommonTime, expTime, eph->xmitTime);
   TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toe);
   TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toc); // same value as toe
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, eph->health);
   TUASSERTFE(1.52643770E-06, eph->Cuc);
   TUASSERTFE(1.529045403003692627E-05, eph->Cus);
   TUASSERTFE(8.210546875E+01, eph->Crc);
   TUASSERTFE(3.112890625E+01, eph->Crs);
   TUASSERTFE(3.16649675E-08, eph->Cic);
   TUASSERTFE(1.86264515E-08, eph->Cis);
   TUASSERTFE(1.9394493464900481872, eph->M0);
   TUASSERTFE(4.16410202E-09, eph->dn);
   TUASSERTFE(-1.14162296E-13, eph->dndot);
   TUASSERTFE(3.7740299012511968613E-04, eph->ecc);
   TUASSERTFE(::sqrt(2.656154014453125E+07), eph->Ahalf);
   TUASSERTFE(2.656154014453125E+07, eph->A);
   TUASSERTFE(1.1958599090576171875E-02, eph->Adot);
   TUASSERTFE(-1.0895206889670716599E-01, eph->OMEGA0);
   TUASSERTFE(9.6039230773739858726E-01, eph->i0);
   TUASSERTFE(-1.6173077678173934935E+00, eph->w);
   TUASSERTFE(-7.84259162E-09, eph->OMEGAdot);
   TUASSERTFE(-6.62527597E-11, eph->idot);
   TUASSERTFE(1.5038633137010037899E-04, eph->af0);
   TUASSERTFE(4.96669372E-12, eph->af1);
   TUASSERTFE(0, eph->af2);
   TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
   TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
      // GPSCNavEph fields
   TUASSERTE(bool, true, eph->healthL1C);
   TUASSERTE(int, -3, eph->uraED);
   TUASSERTE(int, -4, eph->uraNED0);
   TUASSERTE(unsigned, 2, eph->uraNED1);
   TUASSERTE(unsigned, 7, eph->uraNED2);
   TUASSERTE(bool, false, eph->integStat);
   TUASSERTFE(1830.14453125, eph->deltaA);
   TUASSERTFE(3.2554927471973048162e-10, eph->dOMEGAdot);
   TUASSERTE(gnsstk::CommonTime, topExp, eph->top);
   TUASSERTFE(-8.7893567979335784912e-09, eph->tgd);
   TUASSERTFE(-7.85803422E-10, eph->iscL1CP);
   TUASSERTFE(-3.49245965E-10, eph->iscL1CD);
}


void PNBGPSCNav2DataFactory_T ::
validateTOGPS(gnsstk::TestUtil& testFramework,
              const gnsstk::GPSCNav2TimeOffset* to)
{
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(4, 4, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::TimeOffset);
      // NavData fields
   TUASSERTE(gnsstk::CommonTime, sf3p1CNAV2GPSct, to->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExp, to->signal);
      // TimeOffsetData has no fields
      // GPSCNav2TimeOffset
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC, to->tgt);
   TUASSERTFE(6.5192580223e-09, to->a0);
   TUASSERTFE(1.5099033135e-14, to->a1);
   TUASSERTFE(0, to->a2);
   TUASSERTFE(18, to->deltatLS);
   TUASSERTFE(503808, to->tot);
   TUASSERTE(unsigned, 2049, to->wnot);
   TUASSERTE(unsigned, 1929, to->wnLSF);
   TUASSERTE(gnsstk::CommonTime,
             gnsstk::GPSWeekSecond(2049,503808).convertToCommonTime(),
             to->refTime);
   TUASSERTE(unsigned, 7, to->dn);
   TUASSERTFE(18, to->deltatLSF);
}


void PNBGPSCNav2DataFactory_T ::
validateEphHealthGPS(gnsstk::TestUtil& testFramework,
                     const gnsstk::GPSCNav2Health* hea,
                     const gnsstk::CommonTime& expTime)
{
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(4, 4, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::Health);
      // NavData fields
   TUASSERTE(gnsstk::CommonTime, expTime, hea->timeStamp);
   TUASSERTE(gnsstk::NavType, gnsstk::NavType::GPSCNAV2, hea->signal.nav);
   TUASSERTE(gnsstk::NavMessageID, nmidExp, hea->signal);
      // NavHealthData has no fields
      // GPSCNav2Health
   TUASSERTE(bool, true, hea->health);
}


void PNBGPSCNav2DataFactory_T ::
validateAlmHealthGPS(gnsstk::TestUtil& testFramework,
                     const gnsstk::GPSCNav2Health* hea,
                     const gnsstk::CommonTime& expTime)
{
      // NavData fields
   TUASSERTE(gnsstk::CommonTime, expTime, hea->timeStamp);
   gnsstk::NavMessageID nmidExpHeaL1(
      gnsstk::NavSatelliteID(2, 4, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpHeaL2(
      gnsstk::NavSatelliteID(2, 4, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CM,
                            gnsstk::NavType::GPSCNAVL2),
      gnsstk::NavMessageType::Health);
   gnsstk::NavMessageID nmidExpHeaL5(
      gnsstk::NavSatelliteID(2, 4, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I,
                            gnsstk::NavType::GPSCNAVL5),
      gnsstk::NavMessageType::Health);
   bool expHea;
   switch (hea->signal.nav)
   {
      case gnsstk::NavType::GPSCNAV2:
         TUASSERTE(gnsstk::NavMessageID, nmidExpHeaL1, hea->signal);
         expHea = false;
         break;
      case gnsstk::NavType::GPSCNAVL2:
         TUASSERTE(gnsstk::NavMessageID, nmidExpHeaL2, hea->signal);
         expHea = false;
         break;
      case gnsstk::NavType::GPSCNAVL5:
         TUASSERTE(gnsstk::NavMessageID, nmidExpHeaL5, hea->signal);
         expHea = true;
         break;
      default:
         TUFAIL("Unexpected nav type: " +
                gnsstk::StringUtils::asString(hea->signal.nav));
         break;
   }
      // NavHealthData has no fields
      // GPSCNav2Health
   TUASSERTE(bool, expHea, hea->health);
}


void PNBGPSCNav2DataFactory_T ::
validateIonoGPS(gnsstk::TestUtil& testFramework,
                const gnsstk::GPSCNav2Iono* iono)
{
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(4, 4, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::Iono);
   nmidExp.messageType = gnsstk::NavMessageType::Iono;
      // NavData fields
   TUASSERTE(gnsstk::CommonTime, sf3p1CNAV2GPSct, iono->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExp, iono->signal);
   TUASSERTFE( 1.024454830e-08, iono->alpha[0]);
   TUASSERTFE( 1.490116118e-08, iono->alpha[1]);
   TUASSERTFE(-5.960464478e-08, iono->alpha[2]);
   TUASSERTFE(-1.192092897e-07, iono->alpha[3]);
   TUASSERTFE( 8.806400000e+04, iono->beta[0]);
   TUASSERTFE( 3.276800000e+04, iono->beta[1]);
   TUASSERTFE(-1.966080000e+05, iono->beta[2]);
   TUASSERTFE(-1.966080000e+05, iono->beta[3]);
}


void PNBGPSCNav2DataFactory_T ::
validateISCGPS(gnsstk::TestUtil& testFramework,
               const gnsstk::GPSCNav2ISC* isc)
{
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(4, 4, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::ISC);
   TUASSERTE(gnsstk::CommonTime, sf123p1CNAV2GPSct+12.52, isc->timeStamp);
   TUASSERTE(gnsstk::CommonTime, sf123p1CNAV2GPSct+0.52, isc->xmit2);
   TUASSERTE(gnsstk::CommonTime, sf123p1CNAV2GPSct+12.52, isc->xmit3);
   TUASSERTE(gnsstk::NavMessageID, nmidExp, isc->signal);
   TUASSERTE(bool, true, isc->haveSF2);
   TUASSERTE(bool, true, isc->haveSF3);
   TUASSERTFE(-8.78935679793358e-09, isc->isc);
   TUASSERTFE(-7.85803422E-10, isc->iscL1CP);
   TUASSERTFE(-3.49245965E-10, isc->iscL1CD);
   TUASSERTFE(-5.82076609e-10, isc->iscL1CA);
   TUASSERTFE(-6.60656951e-09, isc->iscL2C);
   TUASSERTFE(-1.17870513e-08, isc->iscL5I5);
   TUASSERTFE(-1.17870513e-08, isc->iscL5Q5);
}


void PNBGPSCNav2DataFactory_T ::
validateAlmGPS(gnsstk::TestUtil& testFramework,
               const gnsstk::GPSCNav2Alm* alm,
               const gnsstk::CommonTime& expTime)
{
   gnsstk::NavMessageID nmidExpAlm(
      gnsstk::NavSatelliteID(2, 4, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::Almanac);
   gnsstk::CommonTime toeExp = gnsstk::GPSWeekSecond(2049,503808.0);
   gnsstk::CommonTime beginExp = expTime;
   gnsstk::CommonTime endExp = gnsstk::CommonTime::END_OF_TIME;
   endExp.setTimeSystem(gnsstk::TimeSystem::GPS);
      // NavData fields
   TUASSERTE(gnsstk::CommonTime, expTime, alm->timeStamp);
   TUASSERTE(gnsstk::NavMessageID, nmidExpAlm, alm->signal);
      // OrbitData has no fields
      // OrbitDataKepler fields
   TUASSERTE(gnsstk::CommonTime, expTime, alm->xmitTime);
   TUASSERTE(gnsstk::CommonTime, toeExp, alm->Toe);
   TUASSERTE(gnsstk::CommonTime, toeExp, alm->Toc); // same value as toe
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, alm->health);
   TUASSERTFE(0, alm->Cuc);
   TUASSERTFE(0, alm->Cus);
   TUASSERTFE(0, alm->Crc);
   TUASSERTFE(0, alm->Crs);
   TUASSERTFE(0, alm->Cic);
   TUASSERTFE(0, alm->Cis);
   TUASSERTFE(1.0574880056486639368E-01, alm->M0);
   TUASSERTFE(0, alm->dn);
   TUASSERTFE(0, alm->dndot);
   TUASSERTFE(1.89056396484375E-02, alm->ecc);
   TUASSERTFE(5153.625, alm->Ahalf);
   TUASSERTFE(26559850.640625, alm->A);
   TUASSERTFE(0, alm->Adot);
   TUASSERTFE(-2.2980949678511763956E+00, alm->OMEGA0);
   TUASSERTFE(0.95417439958456595761, alm->i0);
   TUASSERTFE(-1.7481628553941737092E+00, alm->w);
   TUASSERTFE(-8.0460494360E-09, alm->OMEGAdot);
   TUASSERTFE(0, alm->idot);
   TUASSERTFE(-1.926422119140625E-04, alm->af0);
   TUASSERTFE(-7.2759576142E-12, alm->af1);
   TUASSERTFE(0, alm->af2);
   TUASSERTE(gnsstk::CommonTime, beginExp, alm->beginFit);
   TUASSERTE(gnsstk::CommonTime, endExp, alm->endFit);
      // GPSCNavAlm fields
   TUASSERTE(bool, false, alm->healthL1);
   TUASSERTE(bool, false, alm->healthL2);
   TUASSERTE(bool, true, alm->healthL5);
   TUASSERTFE(1.1696603507628014137E-02, alm->deltai);
   TUASSERTE(unsigned, 2049, alm->wna);
   TUASSERTFE(503808, alm->toa);
}


int main()
{
   PNBGPSCNav2DataFactory_T testClass;
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
   errorTotal += testClass.processUTCIonoTest();
   errorTotal += testClass.processGGTOEOPTest();
   errorTotal += testClass.processEphQZSSTest();
   errorTotal += testClass.processAlmOrbQZSSTest();
   errorTotal += testClass.processUTCIonoQZSSTest();
   errorTotal += testClass.processGGTOEOPQZSSTest();
   errorTotal += testClass.processISCTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
