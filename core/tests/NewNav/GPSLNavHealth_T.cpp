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
#include "GPSLNavHealth.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gnsstk::SVHealth e)
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
   gnsstk::GPSLNavHealth obj;
   TUASSERTE(uint8_t, 0x80, obj.svHealth);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Health,
             obj.signal.messageType);
   TURETURN();
}


unsigned GPSLNavHealth_T ::
getUserTimeTest()
{
   TUDEF("GPSLNavHealth", "getUserTime");
   gnsstk::GPSLNavHealth obj;
   obj.timeStamp = gnsstk::GPSWeekSecond(2100,135.0);
   gnsstk::CommonTime exp(gnsstk::GPSWeekSecond(2100,135.0));
      // 1x 6s subframe
   exp = exp + 6.0;
   TUASSERTE(gnsstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}


unsigned GPSLNavHealth_T ::
getHealthTest()
{
   TUDEF("GPSLNavHealth", "getHealth");
   gnsstk::GPSLNavHealth obj;
      // default should be unhealthy
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, obj.getHealth());
      // the one condition that should result in healthy
   obj.svHealth = 0;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, obj.getHealth());
      // weird looking loop, but basically it just wraps around to 0
   bool failed = false;
   for (obj.svHealth = 1; obj.svHealth != 0; obj.svHealth++)
   {
      if (obj.getHealth() != gnsstk::SVHealth::Unhealthy)
      {
         TUFAIL("health bits " + gnsstk::StringUtils::asString(obj.svHealth) +
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
