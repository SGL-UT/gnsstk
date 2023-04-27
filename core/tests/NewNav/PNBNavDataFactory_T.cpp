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
#include "PNBNavDataFactory.hpp"
#include "TestUtil.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


/// Give access to protected members for testing.
class TestClass : public gnsstk::PNBNavDataFactory
{
private:
   friend class PNBNavDataFactory_T;
   bool addData(const gnsstk::PackedNavBitsPtr& navIn,
                gnsstk::NavDataPtrList& navOut, double cadence = -1)
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
   TUASSERTE(gnsstk::NavValidityType, gnsstk::NavValidityType::Any,
             uut.navValidity);
   TUASSERTE(bool, true, uut.processEph);
   TUASSERTE(bool, true, uut.processAlm);
   TUASSERTE(bool, true, uut.processHea);
   TUASSERTE(bool, true, uut.processTim);
   TUASSERTE(bool, true, uut.processIono);
   TUASSERTE(bool, false, uut.processSys);
   TURETURN();
}


unsigned PNBNavDataFactory_T ::
setValidityFilterTest()
{
   TUDEF("PNBNavDataFactory", "satValidityFilter()");
   TestClass uut;
   TUASSERTE(gnsstk::NavValidityType, gnsstk::NavValidityType::Any,
             uut.navValidity);
   for (gnsstk::NavValidityType i : gnsstk::NavValidityTypeIterator())
   {
      uut.setValidityFilter(i);
      TUASSERTE(gnsstk::NavValidityType, i, uut.navValidity);
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
   TUASSERTE(bool, true,  uut.processIono);
   TUASSERTE(bool, false, uut.processSys);
   uut.setTypeFilter({gnsstk::NavMessageType::Almanac});
   TUASSERTE(bool, false, uut.processEph);
   TUASSERTE(bool, true,  uut.processAlm);
   TUASSERTE(bool, false, uut.processHea);
   TUASSERTE(bool, false, uut.processTim);
   TUASSERTE(bool, false, uut.processIono);
   TUASSERTE(bool, false, uut.processSys);
   uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris,
                      gnsstk::NavMessageType::TimeOffset});
   TUASSERTE(bool, true,  uut.processEph);
   TUASSERTE(bool, false, uut.processAlm);
   TUASSERTE(bool, false, uut.processHea);
   TUASSERTE(bool, true,  uut.processTim);
   TUASSERTE(bool, false, uut.processIono);
   TUASSERTE(bool, false, uut.processSys);
   uut.setTypeFilter({gnsstk::NavMessageType::Health,
                      gnsstk::NavMessageType::Clock});
   TUASSERTE(bool, false, uut.processEph);
   TUASSERTE(bool, false, uut.processAlm);
   TUASSERTE(bool, true,  uut.processHea);
   TUASSERTE(bool, false, uut.processTim);
   TUASSERTE(bool, false, uut.processIono);
   TUASSERTE(bool, false, uut.processSys);
   uut.setTypeFilter({gnsstk::NavMessageType::Clock});
   TUASSERTE(bool, false, uut.processEph);
   TUASSERTE(bool, false, uut.processAlm);
   TUASSERTE(bool, false, uut.processHea);
   TUASSERTE(bool, false, uut.processTim);
   TUASSERTE(bool, false, uut.processIono);
   TUASSERTE(bool, false, uut.processSys);
   uut.setTypeFilter({gnsstk::NavMessageType::Iono});
   TUASSERTE(bool, false, uut.processEph);
   TUASSERTE(bool, false, uut.processAlm);
   TUASSERTE(bool, false, uut.processHea);
   TUASSERTE(bool, false, uut.processTim);
   TUASSERTE(bool, true,  uut.processIono);
   TUASSERTE(bool, false, uut.processSys);
   uut.setTypeFilter({gnsstk::NavMessageType::System});
   TUASSERTE(bool, false, uut.processEph);
   TUASSERTE(bool, false, uut.processAlm);
   TUASSERTE(bool, false, uut.processHea);
   TUASSERTE(bool, false, uut.processTim);
   TUASSERTE(bool, false, uut.processIono);
   TUASSERTE(bool, true, uut.processSys);
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
