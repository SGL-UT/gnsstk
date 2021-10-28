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
#include "GPSCNav2Eph.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
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
   gnsstk::GPSCNav2Eph uut;
   TUASSERTE(bool, true, uut.healthL1C);
   TUASSERTE(int8_t, -16, uut.uraED);
   TUASSERTE(int8_t, -16, uut.uraNED0);
   TUASSERTE(uint8_t, 0, uut.uraNED1);
   TUASSERTE(uint8_t, 0, uut.uraNED2);
   TUASSERTE(bool, false, uut.integStat);
   TUASSERTFE(0, uut.deltaA);
   TUASSERTFE(0, uut.dOMEGAdot);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::BEGINNING_OF_TIME, uut.top);
   TUASSERTFE(0, uut.tgd);
   TUASSERTFE(0, uut.iscL1CP);
   TUASSERTFE(0, uut.iscL1CD);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Ephemeris,
             uut.signal.messageType);
   TURETURN();
}


unsigned GPSCNav2Eph_T ::
getUserTimeTest()
{
   TUDEF("GPSCNav2Eph", "getUserTime");
   gnsstk::GPSCNav2Eph uut;
      // just using made-up numbers
   uut.timeStamp = gnsstk::GPSWeekSecond(2100,135.0);
   uut.xmitTime = gnsstk::GPSWeekSecond(2100,139.0);
      // Note that the transmit time and timestamp are typically the
      // same, but getUserTime() by default uses the timeStamp, which
      // is present in NavData while xmitTime is not.  Therefore, the
      // expected user time is timeStamp+12 and not xmitTime+12.
   gnsstk::CommonTime exp(gnsstk::GPSWeekSecond(2100,147.0));
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::Ephemeris);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNav2Eph_T ::
fixFitTest()
{
   TUDEF("GPSCNav2Eph", "fixFit");
   gnsstk::GPSCNav2Eph uut;
   gnsstk::GPSWeekSecond beginExpWS2(2059, 597600), endExpWS2(2060, 3600);
   gnsstk::CommonTime beginExp2(beginExpWS2), endExp2(endExpWS2);
   uut.Toe = gnsstk::GPSWeekSecond(2059, 603000);
   uut.xmitTime = gnsstk::GPSWeekSecond(2059,597600);
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, beginExp2, uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, endExp2, uut.endFit);
      //uut.dump(std::cerr, gnsstk::OrbitDataKepler::Detail::Full);
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
