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
#include "GPSCNav2Eph.hpp"
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

class GPSCNav2Eph_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned validateTest();
};


unsigned GPSCNav2Eph_T ::
constructorTest()
{
   TUDEF("GPSCNav2Eph", "GPSCNav2Eph");
   gpstk::GPSCNav2Eph uut;
   TUASSERTE(bool, true, uut.healthL1C);
   TUASSERTE(int8_t, -16, uut.uraED);
   TUASSERTE(int8_t, -16, uut.uraNED0);
   TUASSERTE(uint8_t, 0, uut.uraNED1);
   TUASSERTE(uint8_t, 0, uut.uraNED2);
   TUASSERTE(bool, false, uut.integStat);
   TUASSERTFE(0, uut.deltaA);
   TUASSERTFE(0, uut.dOMEGAdot);
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime::BEGINNING_OF_TIME, uut.top);
   TUASSERTFE(0, uut.tgd);
   TUASSERTFE(0, uut.iscL1CP);
   TUASSERTFE(0, uut.iscL1CD);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             uut.signal.messageType);
   TURETURN();
}


unsigned GPSCNav2Eph_T ::
getUserTimeTest()
{
   TUDEF("GPSCNav2Eph", "getUserTime");
   gpstk::GPSCNav2Eph uut;
      // just using made-up numbers
   uut.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   uut.xmitTime = gpstk::GPSWeekSecond(2100,139.0);
   gpstk::CommonTime exp(gpstk::GPSWeekSecond(2100,151.0));
   uut.signal = gpstk::NavMessageID(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::L1CD,
                            gpstk::NavType::GPSCNAV2),
      gpstk::NavMessageType::Ephemeris);
   TUASSERTE(gpstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNav2Eph_T ::
fixFitTest()
{
   TUDEF("GPSCNav2Eph", "fixFit");
   gpstk::GPSCNav2Eph uut;
   gpstk::GPSWeekSecond beginExpWS2(2059, 597600), endExpWS2(2060, 3600);
   gpstk::CommonTime beginExp2(beginExpWS2), endExp2(endExpWS2);
   uut.Toe = gpstk::GPSWeekSecond(2059, 603000);
   uut.xmitTime = gpstk::GPSWeekSecond(2059,597600);
   TUCATCH(uut.fixFit());
   TUASSERTE(gpstk::CommonTime, beginExp2, uut.beginFit);
   TUASSERTE(gpstk::CommonTime, endExp2, uut.endFit);
      //uut.dump(std::cerr, gpstk::OrbitDataKepler::Detail::Full);
   TURETURN();
}


unsigned GPSCNav2Eph_T ::
validateTest()
{
   TUDEF("GPSCNav2Data", "validate");
      /// @todo implement some tests when we implement validate()
   TURETURN();
}


int main()
{
   GPSCNav2Eph_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.validateTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
