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

#include "TestUtil.hpp"
#include "NavDataFactory.hpp"
#include "RinexNavDataFactory.hpp"
#include "NavLibrary.hpp"
#include "EphemerisRange.hpp"
#include "PreciseRange.hpp"
#include "SolarSystem.hpp"

class PreciseRange_T
{
public:
   PreciseRange_T();
   unsigned testComputeAtTransmitTimeWithAntenna();
   unsigned testComputeAtTransmitTimeWithoutAntenna();

   gnsstk::NavLibrary navLib;
};


PreciseRange_T ::
PreciseRange_T()
      : navLib()
{
   gnsstk::NavDataFactoryPtr
      ndfp(std::make_shared<gnsstk::RinexNavDataFactory>());

   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "arlm2000.15n";

   navLib.addFactory(ndfp);

   gnsstk::RinexNavDataFactory *rndfp =
      dynamic_cast<gnsstk::RinexNavDataFactory*>(ndfp.get());

   GNSSTK_ASSERT(rndfp->addDataSource(fname));
}


unsigned PreciseRange_T ::
testComputeAtTransmitTimeWithAntenna()
{
   TUDEF("PreciseRange_T", "ComputeAtTransmitTime");

   gnsstk::Position rxPos(-7.0e5, -5.0e6, 3.0e6);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));

   gnsstk::AntexData::antennaPCOandPCVData l1data;
   l1data.PCOvalue[0] = 1;
   l1data.PCOvalue[1] = 2;
   l1data.PCOvalue[2] = 3;
   l1data.hasAzimuth = true;
   gnsstk::AntexData::azimZenMap l1PcvData = {
      {  0.0, {{0.0, 0.0}, {90.0, 16.0}}},
      {360.0, {{0.0, 0.0}, {90.0, 16.0}}}
   };
   l1data.PCVvalue = l1PcvData;

   gnsstk::AntexData::antennaPCOandPCVData l2data;
   l2data.PCOvalue[0] = 3;
   l2data.PCOvalue[1] = 2;
   l2data.PCOvalue[2] = 1;
   l2data.hasAzimuth = true;
   gnsstk::AntexData::azimZenMap l2PcvData = {
      {  0.0, {{0.0, 0.0}, {90.0, 11.0}}},
      {360.0, {{0.0, 0.0}, {90.0, 11.0}}}
   };
   l2data.PCVvalue = l2PcvData;

   gnsstk::AntexData antenna;
   antenna.valid = gnsstk::AntexData::validBits::allValid13;
   antenna.isRxAntenna = true;

   std::map<std::string, gnsstk::AntexData::antennaPCOandPCVData> antennaData;
   antennaData["G01"] = l1data;
   antennaData["G02"] = l2data;
   antenna.freqPCVmap = antennaData;

      // Not bothering to setup a mock solar system object.
      // PreciseRange should default to a lower accuracy computation
   gnsstk::SolarSystem solsys;

   gnsstk::PreciseRange pr;
   double corrected_range =
         pr.ComputeAtTransmitTime(time, 2e7, rxPos, satId, antenna,
                                  "G01", "G02", solsys, navLib, true);

   TUASSERTFESMRT(22289260.787348996848, pr.rawrange);
   TUASSERTFESMRT(22354141.670028157532, corrected_range);
   TURETURN();
}


unsigned PreciseRange_T ::
testComputeAtTransmitTimeWithoutAntenna()
{
   TUDEF("PreciseRange_T", "ComputeAtTransmitTime");

   gnsstk::Position rxPos(-7.0e5, -5.0e6, 3.0e6);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::PreciseRange pr;
   double corrected_range = pr.ComputeAtTransmitTime(time, 2e7, rxPos, satId, navLib);

   TUASSERTFESMRT(22289260.787348996848, pr.rawrange);
   TUASSERTFESMRT(22354141.654476162046, corrected_range);
   TURETURN();
}


int main()
{
   PreciseRange_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.testComputeAtTransmitTimeWithAntenna();
   errorTotal += testClass.testComputeAtTransmitTimeWithoutAntenna();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;

   return errorTotal;
}
