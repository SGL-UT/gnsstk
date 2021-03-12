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
#include "GPSCNav2Alm.hpp"
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

class GPSCNav2Alm_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
};


unsigned GPSCNav2Alm_T ::
constructorTest()
{
   TUDEF("GPSCNav2Alm", "GPSCNav2Alm");
   gpstk::GPSCNav2Alm uut;
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Almanac,
             uut.signal.messageType);
   TUASSERTE(bool, true, uut.healthL1);
   TUASSERTE(bool, true, uut.healthL2);
   TUASSERTE(bool, true, uut.healthL5);
   TUASSERTFE(0, uut.deltai);
   TUASSERTE(unsigned, 0, uut.wna);
   TUASSERTFE(0, uut.toa);
   TURETURN();
}


unsigned GPSCNav2Alm_T ::
getUserTimeTest()
{
   TUDEF("GPSCNav2Alm", "getUserTime");
   gpstk::GPSCNav2Alm uut;
   uut.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
      // CNAV2 almanac is 548 symbols / 5.48 seconds long
   gpstk::CommonTime exp(uut.timeStamp + 5.48);
   uut.signal = gpstk::NavMessageID(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::L1CD,
                            gpstk::NavType::GPSCNAV2),
      gpstk::NavMessageType::Almanac);
   TUASSERTE(gpstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNav2Alm_T ::
fixFitTest()
{
   TUDEF("GPSCNav2Alm", "fixFit");
   gpstk::CommonTime toa = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime xmit = gpstk::GPSWeekSecond(2099,604000.0);
   gpstk::CommonTime expBegin = xmit;
   gpstk::CommonTime expEnd   = gpstk::CommonTime::END_OF_TIME;
   gpstk::GPSCNav2Alm uut;
   uut.Toe = toa;
   uut.xmitTime = xmit;
   TUCATCH(uut.fixFit());
   TUASSERTE(gpstk::CommonTime, expBegin, uut.beginFit);
   TUASSERTE(gpstk::CommonTime, expEnd, uut.endFit);
   TURETURN();
}


int main()
{
   GPSCNav2Alm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
