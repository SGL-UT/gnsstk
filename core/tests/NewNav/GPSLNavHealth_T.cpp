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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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
#include "GPSLNavHealth.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gpstk::SVHealth e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GPSLNavHealth_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned getHealthTest();
};


unsigned GPSLNavHealth_T ::
constructorTest()
{
   TUDEF("GPSLNavHealth", "GPSLNavHealth");
   gpstk::GPSLNavHealth obj;
   TUASSERTE(uint8_t, 0x80, obj.svHealth);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Health,
             obj.signal.messageType);
   TURETURN();
}


unsigned GPSLNavHealth_T ::
getUserTimeTest()
{
   TUDEF("GPSLNavHealth", "getUserTime");
   gpstk::GPSLNavHealth obj;
   obj.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime exp(gpstk::GPSWeekSecond(2100,135.0));
      // 1x 6s subframe
   exp = exp + 6.0;
   TUASSERTE(gpstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}


unsigned GPSLNavHealth_T ::
getHealthTest()
{
   TUDEF("GPSLNavHealth", "getHealth");
   gpstk::GPSLNavHealth obj;
      // default should be unhealthy
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unhealthy, obj.getHealth());
      // the one condition that should result in healthy
   obj.svHealth = 0;
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, obj.getHealth());
      // weird looking loop, but basically it just wraps around to 0
   bool failed = false;
   for (obj.svHealth = 1; obj.svHealth != 0; obj.svHealth++)
   {
      if (obj.getHealth() != gpstk::SVHealth::Unhealthy)
      {
         TUFAIL("health bits " + gpstk::StringUtils::asString(obj.svHealth) +
                " should be Unhealthy");
         failed = true;
         break;
      }
   }
   if (!failed)
   {
      TUPASS("health bits");
   }
   TURETURN();
}


int main()
{
   GPSLNavHealth_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getHealthTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
