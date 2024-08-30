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
#include <cmath>

#include "CommonTime.hpp"
#include "GPSLNavNMC.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"
#include "GPSNMCTAI.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::GPSNMCTAI e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GPSLNavNMC_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned validateTest();
   unsigned updateTNMCTTest();
   unsigned cloneTest();
   unsigned isSameDataTest();
};


unsigned GPSLNavNMC_T ::
constructorTest()
{
   TUDEF("GPSLNavNMC", "GPSLNavNMC");
   gnsstk::GPSLNavNMC obj;
   TUASSERTE(long, -1, obj.aodo);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::END_OF_TIME, obj.Toe);
   TUASSERTE(gnsstk::GPSNMCTAI, gnsstk::GPSNMCTAI::NotAvailable, obj.availabilityIndicator);
   TUASSERTE(bool, true, std::isnan(obj.erd));
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::END_OF_TIME, obj.Tnmct);
   TURETURN();
}


unsigned GPSLNavNMC_T ::
getUserTimeTest()
{
   TUDEF("GPSLNavNMC", "getUserTime");
   gnsstk::GPSLNavNMC obj;
   obj.timeStamp = gnsstk::GPSWeekSecond(2100,253.0);
   gnsstk::CommonTime exp(gnsstk::GPSWeekSecond(2100,259.0));
   TUASSERTE(gnsstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}


unsigned GPSLNavNMC_T ::
validateTest()
{
   TUDEF("GPSLNavNMC", "validate");
   gnsstk::GPSLNavNMC obj;
   obj.aodo = 1;
   obj.erd = 9;
   TUASSERTE(bool, true, obj.validate());
   
   obj.aodo = -1;
   TUASSERTE(bool, false, obj.validate());
   obj.aodo = 30000;
   TUASSERTE(bool, false, obj.validate());
   // reset aodo to valid and verify
   obj.aodo = 1;
   TUASSERTE(bool, true, obj.validate());

   obj.erd = 15;
   TUASSERTE(bool, false, obj.validate());
   obj.erd = -15;
   TUASSERTE(bool, false, obj.validate());
   // reset erd to valid and verify
   obj.erd = 9;
   TUASSERTE(bool, true, obj.validate());

   TURETURN();
}


unsigned GPSLNavNMC_T ::
updateTNMCTTest()
{
   TUDEF("GPSLNavNMC", "updateTNMCT");
   gnsstk::GPSLNavNMC uut;
   uut.Toe = gnsstk::GPSWeekSecond(2100,253.0);
   uut.aodo = 200;
   uut.updateTNMCT();
   gnsstk::CommonTime exp(gnsstk::GPSWeekSecond(2100, 7000));
   TUASSERTE(gnsstk::CommonTime, exp, uut.Tnmct);

   uut.Toe = gnsstk::GPSWeekSecond(2100,7200);
   uut.aodo = 200;
   uut.updateTNMCT();
   gnsstk::CommonTime exp2(gnsstk::GPSWeekSecond(2100, 7000));
   TUASSERTE(gnsstk::CommonTime, exp2, uut.Tnmct);
   
   TURETURN();
}

unsigned GPSLNavNMC_T ::
cloneTest()
{
   TUDEF("GPSLNavNMC", "updateTNMCT");
   gnsstk::GPSLNavNMC uut;
   uut.aodo = 300;

   gnsstk::NavDataPtr clone = uut.clone();
   std::shared_ptr<gnsstk::GPSLNavNMC> clone2 = std::dynamic_pointer_cast<gnsstk::GPSLNavNMC>(clone);
   TUASSERTE(bool, true, clone2 != nullptr);
   TUASSERTE(long, 300, clone2->aodo);
   clone2->aodo = 66;
   TUASSERTE(long, 66, clone2->aodo);
   TUASSERTE(long, 300, uut.aodo);
   
   TURETURN();
}

unsigned GPSLNavNMC_T ::
isSameDataTest()
{
   TUDEF("GPSLNavNMC", "isSameData");
   gnsstk::GPSLNavNMC uut;
   auto uut2 = std::make_shared<gnsstk::GPSLNavNMC>();

   TUASSERTE(bool, true, uut.isSameData(uut2, true));
   uut.Tnmct = gnsstk::GPSWeekSecond(2100,253.0);
   TUASSERTE(bool, false, uut.isSameData(uut2, true));
   
   TURETURN();
}


int main()
{
   GPSLNavNMC_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.validateTest();
   errorTotal += testClass.updateTNMCTTest();
   errorTotal += testClass.cloneTest();
   errorTotal += testClass.isSameDataTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
