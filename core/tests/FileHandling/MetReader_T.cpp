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

#include "MetReader.hpp"
#include "build_config.h"
#include "TestUtil.hpp"

using namespace std;

class MetReader_T
{
public:
   unsigned constructorTest();
   unsigned readTest();
   static const string goodMet, nonExist;
};

const string MetReader_T::goodMet(gnsstk::getPathData() + gnsstk::getFileSep() +
                                  "arlm2000.15m");
const string MetReader_T::nonExist(gnsstk::getFileSep() + "hi" +
                                   gnsstk::getFileSep() + "there");

unsigned MetReader_T ::
constructorTest()
{
   TUDEF("MetReader", "MetReader");
   gnsstk::MetReader uut1;
   TUASSERTE(unsigned, 0, uut1.verboseLevel);
   try
   {
      gnsstk::MetReader uut2(goodMet);
      TUPASS("file read");
      TUASSERTE(unsigned, 96, uut2.wx.obs.size());
   }
   catch (...)
   {
      TUFAIL("Unexpected exception opening \"" + goodMet + "\"");
   }
   try
   {
      gnsstk::MetReader uut3(nonExist);
      TUFAIL("file read");
   }
   catch (gnsstk::FileMissingException)
   {
      TUPASS("Caught the right exception");
   }
   catch (...)
   {
      TUFAIL("Caught wrong exception");
   }
   TURETURN();
}


unsigned MetReader_T ::
readTest()
{
   TUDEF("MetReader", "read");
   gnsstk::MetReader uut;
   TUASSERTE(bool, true, uut.read(goodMet));
   TUASSERTE(unsigned, 96, uut.wx.obs.size());
   TUASSERTE(bool, false, uut.read(nonExist));
   TURETURN();
}


int main()
{
   unsigned errorTotal = 0;
   MetReader_T testClass;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.readTest();

   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;

   return errorTotal;
}
