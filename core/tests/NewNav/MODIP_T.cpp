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
};


/// Copied value from NeQuickG_JRC_MODIP_test.c
const double MODIP_T::MODIP_EPSILON = 1e-5;


MODIP_T ::
MODIP_T()
{
}


unsigned MODIP_T ::
stModipTest()
{
   TUDEF("MODIP", "MODIP()");
   gpstk::MODIP uut;
   DEBUGTRACE_ENABLE();
      /// @note These data came from the ESA code, NeQuickG_JRC_MODIP_test.c
   TUASSERTFEPS(76.28407, uut.stModip(82.49429, 297.65954), MODIP_EPSILON);
   TUASSERTFEPS(19.52877, uut.stModip(5.25218, 307.19404), MODIP_EPSILON);
   TUASSERTFEPS(47.85769, uut.stModip(40.42916, 355.75034), MODIP_EPSILON);
   TUASSERTFEPS(-23.31631, uut.stModip(-2.99591, 40.19439), MODIP_EPSILON);
   TUASSERTFEPS(-71.81130, uut.stModip(-77.83835, 166.66933), MODIP_EPSILON);
   TUASSERTFEPS(46.48742, uut.stModip(39.13517, 141.13283), MODIP_EPSILON);
   TUASSERTFEPS(33.05457, uut.stModip(19.80135, 204.54366), MODIP_EPSILON);
   TUASSERTFEPS(-51.37982, uut.stModip(-31.80197, 115.88525), MODIP_EPSILON);
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
