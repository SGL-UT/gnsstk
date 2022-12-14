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
#include <string>

#include "FreqConsts.hpp"
#include "GNSSconstants.hpp"
#include "TestUtil.hpp"
#include "ord.hpp"
#include "EphemerisRange.hpp"
#include "NavLibrary.hpp"
#include "GPSLNavData.hpp"
#include "ObsID.hpp"
#include "RinexNavDataFactory.hpp"

using namespace gnsstk::ord;

class OrdRegressionTests_T
{
public:
   OrdRegressionTests_T();
   unsigned testIonoFreeRange();
   unsigned testRawRange1();
   unsigned testRawRange2();
   unsigned testRawRange3();
   unsigned testRawRange4();

   gnsstk::NavLibrary navLib;
};

OrdRegressionTests_T ::
OrdRegressionTests_T()
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


unsigned OrdRegressionTests_T ::
testIonoFreeRange()
{
   TUDEF("ORD", "IonosphereFreeRange");

   std::vector<double> frequencies;
   frequencies.push_back(gnsstk::FREQ_GPS_L1);
   frequencies.push_back(gnsstk::FREQ_GPS_L2);

   std::vector<double> pseudoranges;
   pseudoranges.push_back(5000.0);
   pseudoranges.push_back(6000.0);

   for (int i = 0; i < pseudoranges.size(); i++) {
       std::cout << "PR[" << i << "] is: " << pseudoranges[i] << std::endl;
   }

   double range = IonosphereFreeRange(frequencies, pseudoranges);

      // Iononsphere Corrected Pseudorange: Old calculation in ObsRngDev.cpp
      // for dual frequency see IS-GPS-200, section 20.3.3.3.3.3
   double icpr = (pseudoranges[1] - gnsstk::GAMMA_GPS * pseudoranges[0])
                  / (1 - gnsstk::GAMMA_GPS);

      // Compare the new calculation to the old, for our contrived variables.
   double delta = fabs(range-icpr);
   std::cout << "difference of: " << delta << std::endl;
      // @todo Is this an acceptable difference?
   TUASSERTFEPS(icpr, range, 1e-5);
   TURETURN();
}

unsigned OrdRegressionTests_T ::
testRawRange1()
{
   TUDEF("ORD", "RawRange1");

   gnsstk::Position rxLocation(10, 10, 0);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::Xvt xvt;

   double resultRange = RawRange1(rxLocation, satId, time, navLib, xvt);
   resultRange += SvClockBiasCorrection(xvt);
   resultRange += SvRelativityCorrection(xvt);

   gnsstk::CorrectedEphemerisRange cer;

   double originalRange =
      cer.ComputeAtReceiveTime(time, rxLocation, satId, navLib);

      // Compare the new calculation to the old, for our contrived variables.
   TUASSERTFE(originalRange, resultRange);
   TURETURN();
}

unsigned OrdRegressionTests_T ::
testRawRange2()
{
   TUDEF("ORD", "RawRange2");

   gnsstk::Position rxLocation(10, 10, 0);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::Xvt xvt;
   double pseduorange = 999999999;

   double resultRange =
      RawRange2(pseduorange, rxLocation, satId, time, navLib, xvt);
   resultRange += SvClockBiasCorrection(xvt);
   resultRange += SvRelativityCorrection(xvt);

   gnsstk::CorrectedEphemerisRange cer;
   double originalRange =
      cer.ComputeAtTransmitTime(time, pseduorange, rxLocation, satId, navLib);

      // Compare the new calculation to the old, for our contrived variables.
   TUASSERTFESMRT(originalRange, resultRange);
   TURETURN();
}

unsigned OrdRegressionTests_T ::
testRawRange3()
{
   TUDEF("ORD", "RawRange3");

   gnsstk::Position rxLocation(10, 10, 0);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::Xvt xvt;
   double pseduorange = 999999999;

   double resultRange =
      RawRange3(pseduorange, rxLocation, satId, time, navLib, xvt);
   resultRange += SvClockBiasCorrection(xvt);
   resultRange += SvRelativityCorrection(xvt);

   gnsstk::CorrectedEphemerisRange cer;

   double originalRange =
      cer.ComputeAtTransmitSvTime(time, pseduorange, rxLocation, satId, navLib);

      // Compare the new calculation to the old, for our contrived variables.
   TUASSERTFESMRT(originalRange, resultRange);
   TURETURN();
}

unsigned OrdRegressionTests_T ::
testRawRange4()
{
   TUDEF("ORD", "RawRange4");

   gnsstk::Position rxLocation(10, 10, 0);
   gnsstk::SatID satId(5, gnsstk::SatelliteSystem::GPS);
   gnsstk::CommonTime
      time(gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   gnsstk::Xvt xvt;

   double resultRange = RawRange4(rxLocation, satId, time, navLib, xvt);
   resultRange += SvClockBiasCorrection(xvt);
   resultRange += SvRelativityCorrection(xvt);

   gnsstk::CorrectedEphemerisRange cer;
   double originalRange =
      cer.ComputeAtTransmitTime(time, rxLocation, satId, navLib);

      // Compare the new calculation to the old, for our contrived variables.
   TUASSERTFESMRT(originalRange, resultRange);
   TURETURN();
}

int main()
{
   OrdRegressionTests_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.testIonoFreeRange();
   errorTotal += testClass.testRawRange1();
   errorTotal += testClass.testRawRange2();
   errorTotal += testClass.testRawRange3();
   errorTotal += testClass.testRawRange4();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;

   return errorTotal;
}
