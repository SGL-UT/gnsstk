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
#include "MultiFormatNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "SP3NavDataFactory.hpp"
#include "RinexNavDataFactory.hpp"

class TestClass : public gnsstk::MultiFormatNavDataFactory
{
public:
   static gnsstk::NavDataFactoryMap& getFactories()
   { return factories(); }
};


class GNSSTKFormatInitializer_T
{
public:
      /** Make sure that the MultiFormatNavDataFactory contains the
       * expected factory objects. */
   unsigned constructorTest();
};


unsigned GNSSTKFormatInitializer_T ::
constructorTest()
{
   TUDEF("GNSSTKFormatInitializer", "GNSSTKFormatInitializer");
   bool foundRinex = false;
   bool foundSP3 = false;
      // Iterate over the factories and try to find the expected
      // factory objects.  The factories map may also contain ext
      // factories, so we ignore anything else.
   for (auto& i : TestClass::getFactories())
   {
      gnsstk::NavDataFactory *p = i.second.get();
      if (dynamic_cast<gnsstk::RinexNavDataFactory*>(p) != nullptr)
      {
         foundRinex = true;
      }
      else if (dynamic_cast<gnsstk::SP3NavDataFactory*>(p) != nullptr)
      {
         foundSP3 = true;
      }
   }
   TUASSERT(foundRinex);
   TUASSERT(foundSP3);
   TURETURN();
}


int main()
{
   GNSSTKFormatInitializer_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
