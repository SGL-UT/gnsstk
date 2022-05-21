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

/*********************************************************************
*
*  Test program to exercise YumaBase, YumaData, YumaStream.
*
// *********************************************************************/
// System
#include <iostream>
#include <fstream>

#include "YumaData.hpp"
#include "YumaStream.hpp"
#include "YumaBase.hpp"

#include "build_config.h"
#include "TestUtil.hpp"

class Yuma_T
{
public:
   unsigned openMissingFileTest();
   unsigned openInvalidFileTest();
   unsigned roundTripTest();
};


unsigned Yuma_T ::
openMissingFileTest()
{
   TUDEF("YumaStream", "open");
   const char testfn[] = "wioefoqwief.wiroug04tu24tgjlv";
   std::ifstream fstr(testfn, std::ios::in);
   TUASSERT(!fstr);
   gnsstk::YumaStream str(testfn, std::ios::in);
   TUASSERT(!str);
   TURETURN();
}


unsigned Yuma_T ::
openInvalidFileTest()
{
   TUDEF("YumaStream", "open");
   std::string testfn(gnsstk::getPathData() + gnsstk::getFileSep() +
                      "timeconvert_2015_200_0.exp");
   gnsstk::YumaStream str(testfn.c_str(), std::ios::in);
      // opening works because there's no header.
   TUASSERT((bool)str);
   gnsstk::YumaData data;
   str >> data;
      // reading data should fail
   TUASSERT(!str);
   TURETURN();
}


unsigned Yuma_T ::
roundTripTest()
{
   TUDEF("YumaData", "operator<</>>");
   std::string testfn(gnsstk::getPathData() + gnsstk::getFileSep() +
                      "test_input_yuma377.txt");
   std::string outfn(gnsstk::getPathTestTemp() + gnsstk::getFileSep() +
                     "test_output_yuma377.out");
   gnsstk::YumaStream istr(testfn.c_str(), std::ios::in);
   TUASSERT(static_cast<bool>(istr));
   gnsstk::YumaStream ostr(outfn.c_str(), std::ios::out);
   TUASSERT(static_cast<bool>(ostr));
   gnsstk::YumaData data;
   while (istr >> data)
   {
      ostr << data;
   }
   istr.close();
   ostr.close();
      // We need to use df_diff externally to compare the files
      // because of slight spacing changes as well as slight
      // formatting differences for numbers, e.g.
      // 5.9690475460E-03 vs
      // 0.5969047546E-002
      /** @todo determine if this is still valid or if the spec
       * dictates that exact format. */
   TURETURN();
}

using namespace std;
using namespace gnsstk;

int main( int argc, char * argv[] )
{
   int errorTotal = 0;
   Yuma_T testClass;

   errorTotal += testClass.openMissingFileTest();
   errorTotal += testClass.openInvalidFileTest();
   errorTotal += testClass.roundTripTest();

   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;

   return errorTotal;
}
