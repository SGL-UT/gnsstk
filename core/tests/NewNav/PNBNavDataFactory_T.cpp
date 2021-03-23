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
#include "PNBNavDataFactory.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


/// Give access to protected members for testing.
class TestClass : public gpstk::PNBNavDataFactory
{
private:
   friend class PNBNavDataFactory_T;
   bool addData(const gpstk::PackedNavBitsPtr& navIn,
                gpstk::NavDataPtrList& navOut, double cadence = -1)
      override
   { return false; }
   void resetState() override {}
};

class PNBNavDataFactory_T
{
public:
   unsigned constructorTest();
   unsigned setValidityFilterTest();
   unsigned setTypeFilterTest();
};


unsigned PNBNavDataFactory_T ::
constructorTest()
{
   TUDEF("PNBNavDataFactory", "PNBNavDataFactory()");
   TestClass uut;
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::Any,
             uut.navValidity);
   TUASSERTE(bool, true, uut.processEph);
   TUASSERTE(bool, true, uut.processAlm);
   TUASSERTE(bool, true, uut.processHea);
   TUASSERTE(bool, true, uut.processTim);
   TURETURN();
}


unsigned PNBNavDataFactory_T ::
setValidityFilterTest()
{
   TUDEF("PNBNavDataFactory", "satValidityFilter()");
   TestClass uut;
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::Any,
             uut.navValidity);
   for (gpstk::NavValidityType i : gpstk::NavValidityTypeIterator())
   {
      uut.setValidityFilter(i);
      TUASSERTE(gpstk::NavValidityType, i, uut.navValidity);
   }
   TURETURN();
}


unsigned PNBNavDataFactory_T ::
setTypeFilterTest()
{
   TUDEF("PNBNavDataFactory", "setTypeFilter()");
   TestClass uut;
   TUASSERTE(bool, true,  uut.processEph);
   TUASSERTE(bool, true,  uut.processAlm);
   TUASSERTE(bool, true,  uut.processHea);
   TUASSERTE(bool, true,  uut.processTim);
   uut.setTypeFilter({gpstk::NavMessageType::Almanac});
   TUASSERTE(bool, false, uut.processEph);
   TUASSERTE(bool, true,  uut.processAlm);
   TUASSERTE(bool, false, uut.processHea);
   TUASSERTE(bool, false, uut.processTim);
   uut.setTypeFilter({gpstk::NavMessageType::Ephemeris,
                      gpstk::NavMessageType::TimeOffset});
   TUASSERTE(bool, true,  uut.processEph);
   TUASSERTE(bool, false, uut.processAlm);
   TUASSERTE(bool, false, uut.processHea);
   TUASSERTE(bool, true,  uut.processTim);
   uut.setTypeFilter({gpstk::NavMessageType::Health,
                      gpstk::NavMessageType::Clock});
   TUASSERTE(bool, false, uut.processEph);
   TUASSERTE(bool, false, uut.processAlm);
   TUASSERTE(bool, true,  uut.processHea);
   TUASSERTE(bool, false, uut.processTim);
   uut.setTypeFilter({gpstk::NavMessageType::Clock});
   TUASSERTE(bool, false, uut.processEph);
   TUASSERTE(bool, false, uut.processAlm);
   TUASSERTE(bool, false, uut.processHea);
   TUASSERTE(bool, false, uut.processTim);
   TURETURN();
}


int main()
{
   PNBNavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.setValidityFilterTest();
   errorTotal += testClass.setTypeFilterTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
