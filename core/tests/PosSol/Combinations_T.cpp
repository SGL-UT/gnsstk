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
#include "Combinations.hpp"

using namespace std;
using namespace gnsstk;

class Combinations_T
{
public:
   Combinations_T(){}   // Default constructor
   ~Combinations_T(){}   // Default destructor

   unsigned test_copyConstructor();
   unsigned test_assignOperator();
   unsigned test_SelectionBad();
   unsigned test_initBad();
};

unsigned Combinations_T::test_copyConstructor()
{
   TUDEF("Combinations", "copyConstructor");

   try
   {
      Combinations combo1(75, 25);

      // Apply copy constructor
      Combinations combo2(combo1);

      int selection1 = combo1.Selection(5);
      int selection2 = combo2.Selection(5);

      TUASSERT(selection1 == selection2);
   }
   catch(const gnsstk::Exception& e)
   {
      TUFAIL("Exception: " + std::string(e.what()));
   }
   catch(...)
   {
      TUFAIL("An unexpected error occurred.");
   }

   TURETURN();
}

unsigned Combinations_T::test_assignOperator()
{
   TUDEF("Combinations", "assignOperator");

   try
   {
      // Use no-arg constructor (n, k = 0) so that noArgCombo.Selection(1) should fail
      Combinations noArgCombo;

      // Assign noArgCombo to Combination(4,2) so that noArgCombo.Selection(1) now passes
      noArgCombo = Combinations(4,2);
      int selection1 = noArgCombo.Selection(1);

      TUASSERT(selection1 == 1);
   }
   catch(const gnsstk::Exception& e)
   {
      TUFAIL("Exception: " + std::string(e.what()));
   }
   catch(...)
   {
      TUFAIL("An unexpected error occurred.");
   }

   TURETURN();
}

unsigned Combinations_T::test_SelectionBad()
{
   TUDEF("Combinations", "SelectionBad");

   try
   {
      // Create a Combinations object
      Combinations combos(100, 4);

      // Call Selection(j) such that j < 0
      int selectionBad = combos.Selection(-45);

      TUASSERT(selectionBad == -1);
   }
   catch(const gnsstk::Exception& e)
   {
      TUFAIL("Exception: " + std::string(e.what()));
   }
   catch(...)
   {
      TUFAIL("An unexpected error occurred.");
   }

   TURETURN();
}

unsigned Combinations_T::test_initBad()
{
   TUDEF("Combinations", "initBad");

   // Create a Combinations object such that N < K so that it throws an Exception
   TUTHROW(Combinations combos(2,4));

   TURETURN();
}

int main()
{
   Combinations_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.test_copyConstructor();
   errorTotal += testClass.test_assignOperator();
   errorTotal += testClass.test_SelectionBad();
   errorTotal += testClass.test_initBad();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;

   return errorTotal;
}



