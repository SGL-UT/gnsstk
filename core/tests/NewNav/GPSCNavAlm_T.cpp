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
#include "GPSCNavAlm.hpp"
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

class GPSCNavAlm_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
};


unsigned GPSCNavAlm_T ::
constructorTest()
{
   TUDEF("GPSCNavAlm", "GPSCNavAlm");
   gpstk::GPSCNavAlm uut;
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


unsigned GPSCNavAlm_T ::
getUserTimeTest()
{
   TUDEF("GPSCNavAlm", "getUserTime");
   gpstk::GPSCNavAlm uut;
      // L2 has a 12s cadence, L5 has a 6s cadence
   gpstk::CommonTime expL2(gpstk::GPSWeekSecond(2100,147.0));
   gpstk::CommonTime expL5(gpstk::GPSWeekSecond(2100,141.0));
   uut.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   uut.signal = gpstk::NavMessageID(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::L5I,
                            gpstk::NavType::GPSCNAVL5),
      gpstk::NavMessageType::Almanac);
   TUASSERTE(gpstk::CommonTime, expL5, uut.getUserTime());
   uut.signal = gpstk::NavMessageID(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L2, gpstk::TrackingCode::L2CM,
                            gpstk::NavType::GPSCNAVL2),
      gpstk::NavMessageType::Almanac);
   TUASSERTE(gpstk::CommonTime, expL2, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNavAlm_T ::
fixFitTest()
{
   TUDEF("GPSCNavAlm", "fixFit");
   gpstk::CommonTime toa = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime xmit = gpstk::GPSWeekSecond(2099,604000.0);
   gpstk::CommonTime expBegin = xmit;
   gpstk::CommonTime expEnd   = gpstk::CommonTime::END_OF_TIME;
   gpstk::GPSCNavAlm uut;
   uut.Toe = toa;
   uut.xmitTime = xmit;
   TUCATCH(uut.fixFit());
   TUASSERTE(gpstk::CommonTime, expBegin, uut.beginFit);
   TUASSERTE(gpstk::CommonTime, expEnd, uut.endFit);
   TURETURN();
}


int main()
{
   GPSCNavAlm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
