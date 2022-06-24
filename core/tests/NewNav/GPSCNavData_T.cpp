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
#include "GPSCNavData.hpp"
#include "TestUtil.hpp"

/// Make a non-abstract child class of GPSCNavData for testing
class TestClass : public gnsstk::GPSCNavData
{
public:
   gnsstk::CommonTime getUserTime() const override
   { return gnsstk::CommonTime::BEGINNING_OF_TIME; }
};


class GPSCNavData_T
{
public:
      /// Make sure constructor behavior remains unchanged.
   unsigned constructorTest();
   unsigned validateTest();
};


unsigned GPSCNavData_T ::
constructorTest()
{
   TUDEF("GPSCNavData", "GPSCNavData");
   TestClass obj;
   TUASSERTE(uint32_t, 0, obj.pre);
   TUASSERTE(bool, false, obj.alert);
   TURETURN();
}


unsigned GPSCNavData_T ::
validateTest()
{
   TUDEF("GPSCNavData", "validate");
   TestClass obj;
   TUASSERTE(bool, true, obj.validate());
   obj.pre = 0x22c; // no this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.pre = 0x8b; // but this is
   TUASSERTE(bool, true, obj.validate());
   TURETURN();
}


int main()
{
   GPSCNavData_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.validateTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
