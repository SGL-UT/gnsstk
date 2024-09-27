//==============================================================================
////
////  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
////
////  The GNSSTk is free software; you can redistribute it and/or modify
////  it under the terms of the GNU Lesser General Public License as published
////  by the Free Software Foundation; either version 3.0 of the License, or
////  any later version.
////
////  The GNSSTk is distributed in the hope that it will be useful,
////  but WITHOUT ANY WARRANTY; without even the implied warranty of
////  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
////  GNU Lesser General Public License for more details.
////
////  You should have received a copy of the GNU Lesser General Public
////  License along with GNSSTk; if not, write to the Free Software Foundation,
////  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
////
////  This software was developed by Applied Research Laboratories at the
////  University of Texas at Austin.
////  Copyright 2004-2022, The Board of Regents of The University of Texas System
////
////==============================================================================
//
////==============================================================================
////
////  This software was developed by Applied Research Laboratories at the
////  University of Texas at Austin, under contract to an agency or agencies
////  within the U.S. Department of Defense. The U.S. Government retains all
////  rights to use, duplicate, distribute, disclose, or release this software.
////
////  Pursuant to DoD Directive 523024
////
////  DISTRIBUTION STATEMENT A: This software has been approved for public
////                            release, distribution is unlimited.
////
////==============================================================================

#include <iostream>
#include <fstream>
#include "SvObsEpoch.hpp"
#include "TestUtil.hpp"

using namespace std;
using namespace gnsstk;

class SvObsEpoch_T
{
   public:
      SvObsEpoch_T(){} // Default Constructor
      ~SvObsEpoch_T(){} // Default Destructor 
      int test_operator();
};

int SvObsEpoch_T::test_operator()
{
   TUDEF("SvObsEpoch", "operator <<");
   // Verify that the << operator properly prints out the details of the object

   try
   {
      // Initialize an SvObsEpoch object
      SvObsEpoch soe;
      ObsID oid = ObsID();
      soe[oid] = 100.999;

      // Function call and conversion to string
      stringstream out;
      out << soe;
      string result = out.str();

      TUASSERT(result == "UnknownBand UnknownCode UnknownType: 100.999");
   }
   catch( const gnsstk::Exception& e)
   {
      TUFAIL("Exception: " + std::string(e.what()));
   }
   catch(...)
   {
      TUFAIL("An unexpected error occurred.");
   }

   TURETURN();
}

int main() // Main function to initialize and run all tests above
{
   int errorTotal = 0;

   SvObsEpoch_T testClass;

   errorTotal += testClass.test_operator();
   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl; 
   return errorTotal;
}
