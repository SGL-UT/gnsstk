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
#include "GPSCNav2TimeOffset.hpp"
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


class GPSCNav2TimeOffset_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned validateTest();
   unsigned getUserTimeTest();
   unsigned getOffsetTest();
   unsigned getConversionsTest();
};


unsigned GPSCNav2TimeOffset_T ::
constructorTest()
{
   TUDEF("GPSCNav2TimeOffset", "GPSCNav2TimeOffset");
   gpstk::GPSCNav2TimeOffset uut;
   TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::Unknown, uut.tgt);
   TUASSERTFE(0.0, uut.a0);
   TUASSERTFE(0.0, uut.a1);
   TUASSERTFE(0.0, uut.a2);
   TUASSERTFE(0.0, uut.deltatLS);
   TUASSERTFE(0.0, uut.tot);
   TUASSERTE(unsigned, 0, uut.wnot);
   TUASSERTE(unsigned, 0, uut.wnLSF);
   TUASSERTE(unsigned, 0, uut.dn);
   TUASSERTFE(0.0, uut.deltatLSF);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::TimeOffset,
             uut.signal.messageType);
   TURETURN();
}


unsigned GPSCNav2TimeOffset_T ::
validateTest()
{
   TUDEF("GPSCNav2TimeOffset", "validate");
   gpstk::GPSCNav2TimeOffset offs;
   TUASSERTE(bool, true, offs.validate());
   offs.tot = 604784.0;
   TUASSERTE(bool, true, offs.validate());
   offs.dn = 7;
   TUASSERTE(bool, true, offs.validate());
   offs.dn = 8;
   TUASSERTE(bool, false, offs.validate());
   offs.dn = 7;
   offs.tot = -0.001;
   TUASSERTE(bool, false, offs.validate());
   offs.tot = 604784.1;
   TUASSERTE(bool, false, offs.validate());
   TURETURN();
}


unsigned GPSCNav2TimeOffset_T ::
getUserTimeTest()
{
   TUDEF("GPSCNav2TimeOffset", "getUserTime");
   gpstk::GPSCNav2TimeOffset uut;
   uut.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime exp = uut.timeStamp + 5.48;
   TUASSERTE(gpstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNav2TimeOffset_T ::
getOffsetTest()
{
   TUDEF("GPSCNav2TimeOffset", "getOffset");
   gpstk::GPSCNav2TimeOffset uut;
   gpstk::GPSWeekSecond ws1(2060, 405504.0);
   gpstk::GPSWeekSecond ws2(2061, 405504.0);
   uut.tgt = gpstk::TimeSystem::UTC;
   uut.a0 = 1.9790604711E-09;
   uut.a1 = 7.5495165675E-15;
   uut.a2 = 0;
   uut.deltatLS = 18.0;
   uut.tot = 21600.0;
   uut.wnot = 2060;
   double offset;
      /// @todo Truth values here need to be verified.
   TUASSERT(uut.getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC, ws1,
                          offset));
   TUASSERTFE(18.000000004877350079, offset);
   TUASSERT(uut.getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC, ws2,
                          offset));
   TUASSERTFE(18.000000009443297699, offset);
   TURETURN();
}


unsigned GPSCNav2TimeOffset_T ::
getConversionsTest()
{
   TUDEF("GPSCNav2TimeOffset", "getConversions");
   gpstk::TimeOffsetData::TimeCvtSet convs;
   gpstk::GPSCNav2TimeOffset offs;
      // This looks a bit weird, but basically getConversions is
      // expected to return a set containing one key pair for GPS to
      // the target time system, which by default is Unknown.
   gpstk::TimeOffsetData::TimeCvtKey key1(gpstk::TimeSystem::GPS,
                                          gpstk::TimeSystem::Unknown);
   gpstk::TimeOffsetData::TimeCvtKey key2(gpstk::TimeSystem::Unknown,
                                          gpstk::TimeSystem::GPS);
   TUCATCH(convs = offs.getConversions());
   TUASSERTE(size_t, 2, convs.size());
   TUASSERTE(size_t, 1, convs.count(key1));
   TUASSERTE(size_t, 1, convs.count(key2));
   TURETURN();
}


int main()
{
   GPSCNav2TimeOffset_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.validateTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getOffsetTest();
   errorTotal += testClass.getConversionsTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
