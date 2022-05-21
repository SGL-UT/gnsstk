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

#include "build_config.h"
#include "TestUtil.hpp"

using namespace std;
using namespace gnsstk;

class SEM_T
{
public:
   unsigned openMissingFileTest();
   unsigned openInvalidFileTest();
   unsigned roundTripTest();
};


unsigned SEM_T ::
openMissingFileTest()
{
   TUDEF("SEMStream", "open");
   const char testfn[] = "wioefoqwief.wiroug04tu24tgjlv";
   std::ifstream fstr(testfn, std::ios::in);
   TUASSERT(!fstr);
   gnsstk::SEMStream str(testfn, std::ios::in);
   TUASSERT(!str);
   TURETURN();
}


unsigned SEM_T ::
openInvalidFileTest()
{
   TUDEF("SEMStream", "open");
   std::string testfn(gnsstk::getPathData() + gnsstk::getFileSep() +
                      "timeconvert_2015_200_0.exp");
   gnsstk::SEMStream str(testfn.c_str(), std::ios::in);
   TUASSERT((bool)str);
   gnsstk::SEMHeader header;
   str >> header;
      // reading header should fail
   TUASSERT(!str);
   TURETURN();
}


unsigned SEM_T :: roundTripTest()
{
   TUDEF("SEMData", "operator<<");
      // read infn, write what should be identical output to outfn
   string infn(getPathData() + getFileSep() + "test_input_sem387.txt");
   string outfn(getPathTestTemp() + getFileSep() + "test_output_sem387.out");
   SEMStream instr(infn.c_str());
   SEMStream outstr(outfn.c_str(), ios::out);
   SEMHeader hdr;
   SEMData data;
   TUASSERT(static_cast<bool>(instr));
   TUASSERT(static_cast<bool>(outstr));
   TUCATCH(instr >> hdr);
   TUCATCH(outstr << hdr);
   while (instr >> data)
   {
      outstr << data;
   }
   instr.close();
   outstr.close();
   TUCMPFILE(infn, outfn, 0);

   TURETURN();
}



int main( int argc, char * argv[] )
{
   unsigned errorTotal = 0;
   SEM_T testClass;

   errorTotal += testClass.openMissingFileTest();
   errorTotal += testClass.openInvalidFileTest();
   errorTotal += testClass.roundTripTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
