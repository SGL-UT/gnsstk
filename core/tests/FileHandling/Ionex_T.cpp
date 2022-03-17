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

#include "IonexStream.hpp"
#include "IonexHeader.hpp"
#include "IonexData.hpp"
#include "TestUtil.hpp"
#include <iostream>
#include <string>

using namespace std;

class Ionex_T
{
public:
      /// Do a round-trip test on an Ionex 1.0 file
   unsigned roundTrip10Test();
};


unsigned Ionex_T ::
roundTrip10Test()
{
   TUDEF("IonexStream", "open");
   gnsstk::IonexStream is, os;
   gnsstk::IonexHeader header;
   gnsstk::IonexData data;
   string ps(gnsstk::getFileSep());
   string pd(gnsstk::getPathData());
   string tp(gnsstk::getPathTestTemp());
   string reffn(pd + ps + "igrg2680.20i");
   string outfn(tp + ps + "igrg2680.20i");
   TUCATCH(is.open(reffn.c_str(), ios::in));
   TUCATCH(os.open(outfn.c_str(), ios::out));
   TUCSM("reallyGetRecord");
   TUCATCH(is >> header);
   if (!is)
   {
      TUFAIL("Unable to read input stream");
   }
   TUCSM("reallyPutRecord");
   TUCATCH(os << header);
   if (!os)
   {
      TUFAIL("Unable to write to output stream");
   }
   TUCSM("close");
   while (is)
   {
      TUCSM("reallyGetRecord");
      TUCATCH(is >> data);
      TUCSM("reallyPutRecord");
      TUCATCH(os << data);
   }
   TUCATCH(is.close());
   TUCATCH(os.close());
   TUCMPFILE(reffn, outfn, 0);
   TURETURN();
}


int main()
{
   unsigned errorTotal = 0;
   Ionex_T testClass;

   errorTotal += testClass.roundTrip10Test();
   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;
   return errorTotal;
}
