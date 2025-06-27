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

#include <math.h>
#include "TestUtil.hpp"
#include "GNSSconstants.hpp"

class GNSSconstants_T
{
public:
   GNSSconstants_T();
   unsigned equivalenceTest();

};


GNSSconstants_T ::
GNSSconstants_T()
{
}


unsigned GNSSconstants_T ::
equivalenceTest()
{
   TUDEF("Constants", "Constants");
   using gnsstk::PI;
   using gnsstk::C_MPS;

   // These are the values dictated in IS-GPS-200. Please do not miscellaneously/minorly tidy them.
   TUASSERTFE(PI, 3.1415926535898);
   TUASSERTFE(C_MPS, 2.99792458e8);

   TURETURN();
}


int main()
{
   unsigned errorTotal = 0;
   GNSSconstants_T testClass;

   errorTotal += testClass.equivalenceTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}

