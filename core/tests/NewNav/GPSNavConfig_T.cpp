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
#include "GPSNavConfig.hpp"
#include "GPSSVConfig.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gnsstk::GPSSVConfig c)
   {
      s << StringUtils::asString(c);
      return s;
   }
}

class GPSNavConfig_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned getConfigTest();
};


unsigned GPSNavConfig_T ::
constructorTest()
{
   TUDEF("GPSNavConfig", "GPSNavConfig");
   gnsstk::GPSNavConfig obj;
   TUASSERT(!obj.antispoofOn);
   TUASSERTE(uint8_t, 0, obj.svConfig);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::System,
             obj.signal.messageType);
   TURETURN();
}


unsigned GPSNavConfig_T ::
getUserTimeTest()
{
   TUDEF("GPSNavConfig", "getUserTime");
   gnsstk::GPSNavConfig obj;
   obj.timeStamp = gnsstk::GPSWeekSecond(2100,135.0);
   gnsstk::CommonTime exp(gnsstk::GPSWeekSecond(2100,135.0));
      // 1x 6s subframe
   exp = exp + 6.0;
   TUASSERTE(gnsstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}


unsigned GPSNavConfig_T ::
getConfigTest()
{
   TUDEF("GPSNavConfig", "getConfig");
   gnsstk::GPSNavConfig obj;
      // default should be noInfo
   TUASSERTE(
      gnsstk::GPSSVConfig, gnsstk::GPSSVConfig::noInfo, obj.getSVConfig());
      // explicitly test the defined SV config enum values
   for (const auto config : gnsstk::GPSSVConfigIterator())
   {
      obj.svConfig = static_cast<decltype(obj.svConfig)>(config);
      TUASSERTE(gnsstk::GPSSVConfig, config, obj.getSVConfig());
   }
      // validate that other values for svConfig result in noInfo
   for (obj.svConfig = static_cast<uint8_t>(gnsstk::GPSSVConfig::last);
        obj.svConfig != static_cast<uint8_t>(gnsstk::GPSSVConfig::noInfo);
        ++obj.svConfig)
   {
      TUASSERTE(
         gnsstk::GPSSVConfig, gnsstk::GPSSVConfig::noInfo, obj.getSVConfig());
   }
   TURETURN();
}


int main()
{
   GPSNavConfig_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getConfigTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal << '\n';

   return errorTotal;
}
