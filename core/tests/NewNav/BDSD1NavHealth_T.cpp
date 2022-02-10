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
#include "BDSD1NavHealth.hpp"
#include "TestUtil.hpp"
#include "BDSWeekSecond.hpp"

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

class BDSD1NavHealth_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned getHealthTest();
};


unsigned BDSD1NavHealth_T ::
constructorTest()
{
   TUDEF("BDSD1NavHealth", "BDSD1NavHealth");
   gnsstk::BDSD1NavHealth obj;
   TUASSERTE(bool, true, obj.isAlmHealth);
   TUASSERTE(bool, true, obj.satH1);
   TUASSERTE(uint8_t, 0xff, obj.svHealth);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Health,
             obj.signal.messageType);
   TURETURN();
}


unsigned BDSD1NavHealth_T ::
getUserTimeTest()
{
   TUDEF("BDSD1NavHealth", "getUserTime");
   gnsstk::BDSD1NavHealth obj;
   obj.timeStamp = gnsstk::BDSWeekSecond(2100,135.0);
   gnsstk::CommonTime exp(gnsstk::BDSWeekSecond(2100,135.0));
      // 1x 6s subframe
   exp = exp + 6.0;
   TUASSERTE(gnsstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}


unsigned BDSD1NavHealth_T ::
getHealthTest()
{
   TUDEF("BDSD1NavHealth", "getHealth");
   gnsstk::BDSD1NavHealth uut;
      // default should be unhealthy
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut.getHealth());
      // the one condition that should result in healthy
   uut.svHealth = 0;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut.getHealth());
      // weird looking loop, but basically it just wraps around to 0
   bool failed = false;
   for (uut.svHealth = 1; uut.svHealth != 0; uut.svHealth++)
   {
      if (uut.getHealth() != gnsstk::SVHealth::Unhealthy)
      {
         TUFAIL("health bits " + gnsstk::StringUtils::asString(uut.svHealth) +
                " should be Unhealthy");
         failed = true;
         break;
      }
   }
   if (!failed)
   {
      TUPASS("health bits");
   }
   uut.isAlmHealth = false;
   uut.satH1 = false;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut.getHealth());
   uut.satH1 = true;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut.getHealth());
   TURETURN();
}


int main()
{
   BDSD1NavHealth_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getHealthTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
