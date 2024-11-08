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
#include "CNavCrossSourceFilter.hpp"

using namespace std;
using namespace gnsstk;

class CNavCrossSourceFilter_T
{
public:
   CNavCrossSourceFilter_T();
   unsigned testCNavCrossSourceFilter();
};

//-------------------------------------------------------------------
CNavCrossSourceFilter_T :: 
CNavCrossSourceFilter_T() {
}

//-------------------------------------------------------------------
unsigned CNavCrossSourceFilter_T::testCNavCrossSourceFilter() {
   TUDEF("CNavCrossSourceFilter_T", "Constructor");
   CNavCrossSourceFilter filter;

   TUASSERTE(int, 1, filter.processingDepth());
   TUASSERTE(string, "CrossSource", filter.filterName());

   // Set minimum number identical messages needed
   filter.setMinIdentical(100);
   TUASSERTE(int, filter.minIdentical, 100);
   TUASSERTE(int, filter.getMinIdentical(), 100);

   filter.setMinIdentical(50);
   TUASSERTE(int, filter.minIdentical, 50);
   TUASSERTE(int, filter.getMinIdentical(), 50);

   // Dump testing
   std::ostringstream stream;
   std::string expectedOutput = "#--------------------------------\n  Dump of CNavCrossSourceFilter::NavMap\n";
   filter.dump(stream);

   TUASSERTE(std::string, expectedOutput, stream.str());
   TURETURN();
 }

//-------------------------------------------------------------------
int main()
{
   unsigned errorTotal = 0;

   CNavCrossSourceFilter_T testClass;

   errorTotal += testClass.testCNavCrossSourceFilter();

   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;

   return errorTotal; // Return the total number of errors
}