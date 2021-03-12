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
#include "MultiFormatNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "SEMNavDataFactory.hpp"
#include "YumaNavDataFactory.hpp"

class TestClass : public gpstk::MultiFormatNavDataFactory
{
public:
   static gpstk::NavDataFactoryMap& getFactories()
   { return factories(); }
};


class ExtFactoryInitializer_T
{
public:
      /** Make sure that the MultiFormatNavDataFactory contains the
       * expected factory objects. */
   unsigned constructorTest();
};


unsigned ExtFactoryInitializer_T ::
constructorTest()
{
   TUDEF("ExtFactoryInitializer", "ExtFactoryInitializer");
   bool foundYuma = false;
   bool foundSEM = false;
      // Iterate over the factories and try to find the expected
      // factory objects.  The factories map may also contain ext
      // factories, so we ignore anything else.
   for (auto& i : TestClass::getFactories())
   {
      gpstk::NavDataFactory *p = i.second.get();
      if (dynamic_cast<gpstk::YumaNavDataFactory*>(p) != nullptr)
      {
         foundYuma = true;
      }
      else if (dynamic_cast<gpstk::SEMNavDataFactory*>(p) != nullptr)
      {
         foundSEM = true;
      }
   }
   TUASSERT(foundYuma);
   TUASSERT(foundSEM);
   TURETURN();
}


int main()
{
   ExtFactoryInitializer_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
