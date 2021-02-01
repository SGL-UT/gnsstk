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
#include "GPSCNav2Health.hpp"
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

class GPSCNav2Health_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned getHealthTest();
};


unsigned GPSCNav2Health_T ::
constructorTest()
{
   TUDEF("GPSCNav2Health", "GPSCNav2Health");
   gpstk::GPSCNav2Health uut;
   TUASSERTE(uint8_t, true, uut.health);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Health,
             uut.signal.messageType);
   TURETURN();
}


unsigned GPSCNav2Health_T ::
getUserTimeTest()
{
   TUDEF("GPSCNav2Health", "getUserTime");
   gpstk::GPSCNav2Health uut;
   uut.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
      // eph = 12s, alm = 5.48s
   gpstk::CommonTime expEph = uut.timeStamp + 12.0;
   gpstk::CommonTime expAlm = uut.timeStamp + 5.48;
   uut.setEph(true);
   TUASSERTE(gpstk::CommonTime, expEph, uut.getUserTime());
   uut.setEph(false);
   TUASSERTE(gpstk::CommonTime, expAlm, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNav2Health_T ::
getHealthTest()
{
   TUDEF("GPSCNav2Health", "getHealth");
   gpstk::GPSCNav2Health uut;
      // default should be unhealthy
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unhealthy, uut.getHealth());
      // the one condition that should result in healthy
   uut.health = false;
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, uut.getHealth());
   TURETURN();
}


int main()
{
   GPSCNav2Health_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getHealthTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
