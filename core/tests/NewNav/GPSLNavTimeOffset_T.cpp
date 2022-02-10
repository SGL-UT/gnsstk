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
#include "GPSLNavTimeOffset.hpp"
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


class GPSLNavTimeOffset_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned validateTest();
   unsigned getUserTimeTest();
   unsigned getOffsetTest();
   unsigned getConversionsTest();
};


unsigned GPSLNavTimeOffset_T ::
constructorTest()
{
   TUDEF("GPSLNavTimeOffset", "GPSLNavTimeOffset");
   gnsstk::GPSLNavTimeOffset obj;
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GPS, obj.src);
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC, obj.tgt);
   TUASSERTFE(0.0, obj.a0);
   TUASSERTFE(0.0, obj.a1);
   TUASSERTFE(0.0, obj.a2);
   TUASSERTFE(0.0, obj.deltatLS);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime(), obj.refTime);
   TUASSERTFE(0.0, obj.tot);
   TUASSERTE(unsigned, 0, obj.wnot);
   TUASSERTE(unsigned, 0, obj.wnLSF);
   TUASSERTE(unsigned, 0, obj.dn);
   TUASSERTFE(0.0, obj.deltatLSF);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::TimeOffset,
             obj.signal.messageType);
   TURETURN();
}


unsigned GPSLNavTimeOffset_T ::
validateTest()
{
   TUDEF("GPSLNavTimeOffset", "validate");
   gnsstk::GPSLNavTimeOffset offs;
   TUASSERTE(bool, true, offs.validate());
   offs.tot = 602112.0;
   TUASSERTE(bool, true, offs.validate());
   offs.dn = 7;
   TUASSERTE(bool, true, offs.validate());
   offs.dn = 8;
   TUASSERTE(bool, false, offs.validate());
   offs.dn = 7;
   offs.tot = -0.001;
   TUASSERTE(bool, false, offs.validate());
   offs.tot = 602112.1;
   TUASSERTE(bool, false, offs.validate());
   TURETURN();
}


unsigned GPSLNavTimeOffset_T ::
getUserTimeTest()
{
   TUDEF("GPSLNavTimeOffset", "getUserTime");
   gnsstk::GPSLNavTimeOffset obj;
   obj.timeStamp = gnsstk::GPSWeekSecond(2100,135.0);
   gnsstk::CommonTime exp(gnsstk::GPSWeekSecond(2100,135.0));
      // 1x 6s subframe
   exp = exp + 6.0;
   TUASSERTE(gnsstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}


unsigned GPSLNavTimeOffset_T ::
getOffsetTest()
{
   TUDEF("GPSLNavTimeOffset", "getOffset");
   gnsstk::GPSLNavTimeOffset obj;
   gnsstk::GPSWeekSecond ws1(2092, 405504.0);
   gnsstk::GPSWeekSecond ws2(2093, 405504.0);
   obj.deltatLS = 18.0;
   obj.a0 = -2.79396772E-09;
   obj.a1 = -7.10542736E-15;
   obj.tot = 405504.0;
   obj.wnot = 2092;
   obj.refTime = gnsstk::GPSWeekSecond(obj.wnot, obj.tot);
   double offset;
      /// @todo Truth values here need to be verified.
   TUASSERT(obj.getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC, ws1,
                          offset));
   TUASSERTFE(17.99999999720603227615, offset);
   TUASSERT(obj.getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC, ws2,
                          offset));
   TUASSERTFE(17.99999999290866981028, offset);
   TURETURN();
}


unsigned GPSLNavTimeOffset_T ::
getConversionsTest()
{
   TUDEF("GPSLNavTimeOffset", "getConversions");
   gnsstk::TimeCvtSet convs;
   gnsstk::GPSLNavTimeOffset offs;
   gnsstk::TimeCvtKey key1(gnsstk::TimeSystem::GPS,
                                          gnsstk::TimeSystem::UTC);
   gnsstk::TimeCvtKey key2(gnsstk::TimeSystem::UTC,
                                          gnsstk::TimeSystem::GPS);
   TUCATCH(convs = offs.getConversions());
   TUASSERTE(size_t, 2, convs.size());
   TUASSERTE(size_t, 1, convs.count(key1));
   TUASSERTE(size_t, 1, convs.count(key2));
   TURETURN();
}


int main()
{
   GPSLNavTimeOffset_T testClass;
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
