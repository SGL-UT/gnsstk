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
using BDSFactoryCounter = FactoryCounter<gpstk::BDSD1NavAlm,gpstk::BDSD1NavEph,gpstk::BDSD1NavTimeOffset,gpstk::BDSD1NavHealth,gpstk::BDSD1NavIono,gpstk::BDSD1NavISC>;

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
}


class PNBBDSD1NavDataFactory_T
{
public:
   PNBBDSD1NavDataFactory_T();
      // addData tests for a variety of filter states
      /// Test addData with all message types enabled
   unsigned addDataAllTest();

   void bunk();

#include "D1NavTestDataDecl.hpp"
};


PNBBDSD1NavDataFactory_T ::
PNBBDSD1NavDataFactory_T()
{
#include "D1NavTestDataDef.hpp"
}


unsigned PNBBDSD1NavDataFactory_T ::
addDataAllTest()
{
   TUDEF("PNBBDSD1NavDataFactory", "addData");
   BDSFactoryCounter fc(testFramework);
   gpstk::PNBBDSD1NavDataFactory uut;
   gpstk::SatID gloSid(1,gpstk::SatelliteSystem::Glonass);
   gpstk::ObsID gloid(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::G1,
                      gpstk::TrackingCode::Standard);
   gpstk::NavID gloNav(gpstk::NavType::GloCivilF);
   gpstk::PackedNavBitsPtr nonBDS = std::make_shared<gpstk::PackedNavBits>(
      gloSid, gloid, gloNav, "XX", ephD1NAVSF1ct);
   gpstk::NavDataPtrList navOut;
      // should refuse non-BDS data
   TUASSERTE(bool, false, uut.addData(nonBDS, navOut));
      // The rest is checking that we're processing the good data
      // appropriately, but I'm not bothering to check the detailed
      // contents as that is assumed to be tested in the process*
      // tests.
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
   TURETURN();
}

void PNBBDSD1NavDataFactory_T ::
bunk()
{
   gpstk::PNBBDSD1NavDataFactory uut;
   uut.bunk();
}


int main()
{
   PNBBDSD1NavDataFactory_T testClass;
   unsigned errorTotal = 0;

   testClass.bunk();
   errorTotal += testClass.addDataAllTest();
   // errorTotal += testClass.addDataAlmanacTest();
   // errorTotal += testClass.addDataEphemerisTest();
   // errorTotal += testClass.addDataHealthTest();
   // errorTotal += testClass.addDataTimeTest();
   // errorTotal += testClass.addDataEphHealthTest();
   // errorTotal += testClass.addDataValidityTest();
   // errorTotal += testClass.processEphTest();
   // errorTotal += testClass.processAlmOrbTest();
   // errorTotal += testClass.processSVID51Test();
   // errorTotal += testClass.processSVID63Test();
   // errorTotal += testClass.processSVID56Test();
   // errorTotal += testClass.addDataAllQZSSTest();
   // errorTotal += testClass.processEphQZSSTest();
   // errorTotal += testClass.processAlmOrbQZSSTest();
   // errorTotal += testClass.processSVID51QZSSTest();
   // errorTotal += testClass.processSVID56QZSSTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
