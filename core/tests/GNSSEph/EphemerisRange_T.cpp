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

class EphemerisRange_T
{
public:
   EphemerisRange_T();
   unsigned testComputeAtReceiveTime();
   unsigned testComputeAtTransmitTime();
   unsigned testComputeAtTransmitTime2();
   unsigned testComputeAtTransmitSvTime();

   gnsstk::NavLibrary navLib;
};


EphemerisRange_T ::
EphemerisRange_T()
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


unsigned EphemerisRange_T ::
testComputeAtReceiveTime()
{
   TUDEF("CorrectedEphemerisRange", "ComputeAtReceiveTime");

   gnsstk::Position rxPos(-7.0e5, -5.0e6, 3.0e6);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::CorrectedEphemerisRange cer;
   double corrected_range = cer.ComputeAtReceiveTime(time, rxPos, satId, navLib);

   TUASSERTFESMRT(22289257.145863413811, cer.rawrange);
   TUASSERTFESMRT(22354137.99468812719, corrected_range);
   TURETURN();
}


unsigned EphemerisRange_T ::
testComputeAtTransmitTime()
{
   TUDEF("CorrectedEphemerisRange", "ComputeAtTransmitTime");

   gnsstk::Position rxPos(-7.0e5, -5.0e6, 3.0e6);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::CorrectedEphemerisRange cer;
   double corrected_range = cer.ComputeAtTransmitTime(time, 2.7e7, rxPos, satId, navLib);

   TUASSERTFESMRT(22289249.959460116923, cer.rawrange);
   TUASSERTFESMRT(22354130.808302134275, corrected_range);
   TURETURN();
}


unsigned EphemerisRange_T ::
testComputeAtTransmitTime2()
{
   TUDEF("CorrectedEphemerisRange", "ComputeAtTransmitTime");

   gnsstk::Position rxPos(-7.0e5, -5.0e6, 3.0e6);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::CorrectedEphemerisRange cer;
   double corrected_range = cer.ComputeAtTransmitTime(time, rxPos, satId, navLib);

   TUASSERTFESMRT(22289257.145802032202, cer.rawrange);
   TUASSERTFESMRT(22354137.994626745582, corrected_range);
   TURETURN();
}


unsigned EphemerisRange_T ::
testComputeAtTransmitSvTime()
{
   TUDEF("CorrectedEphemerisRange", "ComputeAtTransmitSvTime");

   gnsstk::Position rxPos(-7.0e5, -5.0e6, 3.0e6);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::CorrectedEphemerisRange cer;
   double corrected_range = cer.ComputeAtTransmitSvTime(time, 2.7e7, rxPos, satId, navLib);

   TUASSERTFESMRT(22289288.75284050405, cer.rawrange);
   TUASSERTFESMRT(22354169.60158220306, corrected_range);
   TURETURN();
}


int main()
{
   EphemerisRange_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.testComputeAtReceiveTime();
   errorTotal += testClass.testComputeAtTransmitTime();
   errorTotal += testClass.testComputeAtTransmitTime2();
   errorTotal += testClass.testComputeAtTransmitSvTime();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;

   return errorTotal;
}
