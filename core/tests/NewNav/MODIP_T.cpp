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

#include "TestUtil.hpp"
#include "MODIP.hpp"
#include "DebugTrace.hpp"

class MODIP_T
{
public:
   static const double MODIP_EPSILON;

   MODIP_T();

      /// Test stModip() and implicitly, interpolate().
   unsigned stModipTest();

      /// Hold input/truth data for stModipTest
   class TestData
   {
   public:
         /// dc* are don't-care values copied from EU test code.
      TestData(double longitude, double dc1, double dc2, double dc3,
               double latitude, double dc4, double dc5, double dc6,
               double dc7, int dc8, double dc9, double dc10, double expect)
            : pos(latitude,longitude,0,gnsstk::Position::Geodetic),
              expMODIP(expect)
      {}
      gnsstk::Position pos;
      double expMODIP;
   };
      /// Input/truth data for stModipTest
   static const TestData testData[];
};


// Copied value from NeQuickG_JRC_MODIP_test.c
const double MODIP_T::MODIP_EPSILON = 1e-5;

// Copied data from NeQuickG_JRC_MODIP_test.c
const MODIP_T::TestData MODIP_T::testData[] =
{
   {
      297.65954, 0.0, 0.0, 0.0, 82.49429, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0,
      76.28407
   },
   {
      307.19404, 0.0, 0.0, 0.0, 5.25218, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0,
      19.52877
   },
   {
      355.75034, 0.0, 0.0, 0.0, 40.42916, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0,
      47.85769
   },
   {
      40.19439, 0.0, 0.0, 0.0, -2.99591, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0,
      -23.31631
   },
   {
      166.66933, 0.0, 0.0, 0.0, -77.83835, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0,
      -71.81130
   },
   {
      141.13283, 0.0, 0.0, 0.0, 39.13517, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0,
      46.48742
   },
   {
      204.54366, 0.0, 0.0, 0.0, 19.80135, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0,
      33.05457
   },
   {
      115.88525, 0.0, 0.0, 0.0, -31.80197, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0,
      -51.37982
   }
};


MODIP_T ::
MODIP_T()
{
}


unsigned MODIP_T ::
stModipTest()
{
   TUDEF("MODIP", "MODIP()");
   gnsstk::MODIP uut;
   unsigned numTests = sizeof(testData)/sizeof(testData[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestData& td(testData[testNum]);
      TUASSERTFEPS(td.expMODIP, uut.stModip(td.pos), MODIP_EPSILON);
   }
   TURETURN();
}



int main(int argc, char *argv[])
{
   MODIP_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.stModipTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
