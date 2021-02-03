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
#include "GPSLNavAlm.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GPSLNavAlm_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
};


unsigned GPSLNavAlm_T ::
constructorTest()
{
   TUDEF("GPSLNavAlm", "GPSLNavAlm");
   gpstk::GPSLNavAlm uut;
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Almanac,
             uut.signal.messageType);
   TUASSERTE(uint8_t, 0xff, uut.healthBits);
   TURETURN();
}


unsigned GPSLNavAlm_T ::
getUserTimeTest()
{
   TUDEF("GPSLNavAlm", "getUserTime");
   gpstk::GPSLNavAlm uut;
   uut.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime exp(gpstk::GPSWeekSecond(2100,135.0));
      // almanac = 1 subframes * 6 seconds
   exp = exp + 6.0;
   TUASSERTE(gpstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GPSLNavAlm_T ::
fixFitTest()
{
   TUDEF("GPSLNavAlm", "fixFit");
   gpstk::CommonTime toa = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime expBegin = toa - (70.0 * 3600.0);
   gpstk::CommonTime expEnd   = toa + (74.0 * 3600.0);
   gpstk::GPSLNavAlm uut;
   uut.Toe = toa;
   TUCATCH(uut.fixFit());
   TUASSERTE(gpstk::CommonTime, expBegin, uut.beginFit);
   TUASSERTE(gpstk::CommonTime, expEnd, uut.endFit);
   TURETURN();
}


int main()
{
   GPSLNavAlm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
