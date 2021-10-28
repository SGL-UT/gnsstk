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
//  Copyright 2004, The Board of Regents of The University of Texas System
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


/*********************************************************************
*
*  Test program to exercise SEMBase, SEMData, and SEMStream.
*
// *********************************************************************/
#include <iostream>
#include <fstream>

#include "ReferenceFrame.hpp"
#include "SEMData.hpp"
#include "SEMStream.hpp"
#include "SEMHeader.hpp"
#include "SEMBase.hpp"
#include "Triple.hpp"
#include "Xvt.hpp"

#include "build_config.h"
#include "TestUtil.hpp"

using namespace std;
using namespace gnsstk;

class SEM_T
{
public:
   unsigned roundTripTest();
   unsigned validXvtTest();
};

unsigned SEM_T :: roundTripTest()
{
   TUDEF("SEMData", "operator<<");
   try
   {
         // read infn, write what should be identical output to outfn
         // write almanac data to almfn, which should be the same as almexp
      string infn(getPathData() + getFileSep() + "test_input_sem387.txt");
      string outfn(getPathTestTemp() + getFileSep() + "test_output_sem387.out");
      string almfn(getPathTestTemp() + getFileSep() + "test_output_SEM_T.out");
      string almexp(getPathData() + getFileSep() + "test_output_SEM_T.exp");
      SEMStream instr(infn.c_str());
      SEMStream outstr(outfn.c_str(), ios::out);
      SEMStream almstr(almfn.c_str(), ios::out);
      SEMHeader hdr;
      SEMData data;
      TUASSERT(static_cast<bool>(instr));
      TUASSERT(static_cast<bool>(outstr));
      TUCATCH(instr >> hdr);
      TUCATCH(outstr << hdr);
      while (instr >> data)
      {
         outstr << data;
         OrbAlmGen oag = OrbAlmGen(data);
         oag.dump(almstr);
      }
      instr.close();
      outstr.close();
      almstr.close();
      TUCMPFILE(infn, outfn, 0);
      TUCMPFILE(almexp, almfn, 0);
   }
   catch (gnsstk::Exception& exc)
   {
      cerr << exc << endl;
      TUFAIL("Unexpected exception");
   }
   catch (std::exception& exc)
   {
      TUFAIL("Unexpected exception: " + string(exc.what()));
   }
   catch (...)
   {
      TUFAIL("Unknown exception");
   }

   TURETURN();
}

unsigned SEM_T :: validXvtTest()
{
   TUDEF("SEMData", "operator<<");

   // Use the test almanac start time as the test time.
   const auto testTime = gnsstk::GPSWeekSecond(1411, 589824).convertToCommonTime();

   // x, v, and clkbias "truth" values for PRNs 1 and 2 dumped from a run of
   // this test code with GNSSTk 3.0.0, the last version to use an old backend for
   // gnsstk::SEMData (i.e. the last version to have independent values to compare
   // current results against). That version did not set clkdrift, relcorr, or
   // health, so those are untested here.
   //
   // All floating point constants are represented with 17 significant digits,
   // the minimum number of decimal digits necessary to uniquely specify a
   // given binary floating point representation.
   gnsstk::Xvt prn1Truth{};
   prn1Truth.x = gnsstk::Triple{-2.1686348646190856e5,  1.4659098042987783e7, -2.1942741735400379e7};
   prn1Truth.v = gnsstk::Triple{-2.7919297517587174e3,  4.5277122633266595e2,  3.2117936708783310e2};
   prn1Truth.clkbias = 2.3570060729980000e-3;
   prn1Truth.frame = gnsstk::ReferenceFrame::WGS84;

   gnsstk::Xvt prn2Truth{};
   prn2Truth.x = gnsstk::Triple{-1.4508183990306815e7,  6.8755647450474072e6,  2.0886429030714110e7};
   prn2Truth.v = gnsstk::Triple{-1.8961933284170113e3, -1.9459202870291399e3, -6.9400626591158095e2};
   prn2Truth.clkbias = 2.3303031921387000e-3;
   prn2Truth.frame = gnsstk::ReferenceFrame::WGS84;

   // Define function to compare two Xvts for approximate equality
   auto xvtsApproxEqual = [] (const gnsstk::Xvt& a, const gnsstk::Xvt& b) -> bool {
      constexpr double maxRelError = 1e-12;
      if (   ((a.x - b.x).mag() / a.x.mag()) <= maxRelError
          && ((a.v - b.v).mag() / a.v.mag()) <= maxRelError
          && (std::abs(a.clkbias - b.clkbias) / a.clkbias) <= maxRelError)
      {
         return true;
      }
      return false;
   };

   try
   {
      // Read in test SEM almanac, get Xvts for PRNs 1 and 2, and compare
      // against truth data.
      const std::string infn(getPathData() + getFileSep() + "test_input_sem387.txt");
      gnsstk::SEMStream instr(infn.c_str());
      gnsstk::SEMHeader hdr;
      gnsstk::SEMData data;
      TUASSERT(static_cast<bool>(instr));
      TUCATCH(instr >> hdr);

      while(instr >> data)
      {
         switch (data.PRN)
         {
            case 1:
               TUASSERT(xvtsApproxEqual(prn1Truth, OrbAlmGen(data).svXvt(testTime)));
               break;
            case 2:
               TUASSERT(xvtsApproxEqual(prn2Truth, OrbAlmGen(data).svXvt(testTime)));
               break;
            default:
               break;
         }
      }

      instr.close();
   }
   catch (const gnsstk::Exception& exc)
   {
      cerr << exc << endl;
      TUFAIL("Unexpected exception");
   }
   catch (const std::exception& exc)
   {
      TUFAIL("Unexpected exception: " + string(exc.what()));
   }
   catch (...)
   {
      TUFAIL("Unknown exception");
   }

   TURETURN();
}

int main( int argc, char * argv[] )
{
   unsigned errorTotal = 0;
   SEM_T testClass;

   errorTotal += testClass.roundTripTest();
   errorTotal += testClass.validXvtTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
