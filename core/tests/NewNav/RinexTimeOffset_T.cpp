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
#include "RinexTimeOffset.hpp"
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


class RinexTimeOffset_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned validateTest();
   unsigned getUserTimeTest();
   unsigned getOffsetTest();
   unsigned getConversionsTest();
};


unsigned RinexTimeOffset_T ::
constructorTest()
{
   TUDEF("RinexTimeOffset", "RinexTimeOffset");
   gnsstk::RinexTimeOffset uut;
   TUASSERTE(gnsstk::TimeSystemCorrection::CorrType,
             gnsstk::TimeSystemCorrection::CorrType::Unknown, uut.type);
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::Unknown, uut.frTS);
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::Unknown, uut.toTS);
   TUASSERTFE(0.0, uut.A0);
   TUASSERTFE(0.0, uut.A1);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::BEGINNING_OF_TIME,
             uut.refTime);
   TUASSERTE(std::string, "", uut.geoProvider);
   TUASSERTE(int, 0, uut.geoUTCid);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::TimeOffset,
             uut.signal.messageType);
   TURETURN();
}


unsigned RinexTimeOffset_T ::
validateTest()
{
   TUDEF("RinexTimeOffset", "validate");
   gnsstk::RinexTimeOffset offs;
      /** @todo implement some tests for validation when we actually
       * implement RinexTimeOffset::validate() method */
   TURETURN();
}


unsigned RinexTimeOffset_T ::
getUserTimeTest()
{
   TUDEF("RinexTimeOffset", "getUserTime");
   gnsstk::RinexTimeOffset uut;
   gnsstk::CommonTime exp(gnsstk::GPSWeekSecond(2100,135.0));
   uut.timeStamp = gnsstk::GPSWeekSecond(2100,135.0);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned RinexTimeOffset_T ::
getOffsetTest()
{
   TUDEF("RinexTimeOffset", "getOffset");
   gnsstk::RinexTimeOffset uut;
   gnsstk::GPSWeekSecond ws1(2060, 405504.0);
   gnsstk::GPSWeekSecond ws2(2061, 405504.0);
   uut.type = gnsstk::TimeSystemCorrection::GPUT;
   uut.frTS = gnsstk::TimeSystem::GPS;
   uut.toTS = gnsstk::TimeSystem::UTC;
   uut.A0 = 18.0 + 1.9790604711E-09;
   uut.A1 = 7.5495165675E-15;
   uut.refTime = gnsstk::GPSWeekSecond(2060,21600);
   double offset = 666;
      /// @todo Truth values here need to be verified.
   ws1.setTimeSystem(gnsstk::TimeSystem::UTC);
   TUASSERT(uut.getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC, ws1,
                          offset));
   TUASSERTFE(18.000000004877350079, offset);
   ws2.setTimeSystem(gnsstk::TimeSystem::UTC);
   TUASSERT(uut.getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC, ws2,
                          offset));
   TUASSERTFE(18.000000009443297699, offset);
   TURETURN();
}


unsigned RinexTimeOffset_T ::
getConversionsTest()
{
   TUDEF("RinexTimeOffset", "getConversions");
   gnsstk::TimeCvtSet convs;
   gnsstk::RinexTimeOffset uut;
   uut.frTS = gnsstk::TimeSystem::GPS;
   uut.toTS = gnsstk::TimeSystem::Unknown;
   gnsstk::TimeCvtKey key1(gnsstk::TimeSystem::GPS,
                                          gnsstk::TimeSystem::Unknown);
   gnsstk::TimeCvtKey key2(gnsstk::TimeSystem::Unknown,
                                          gnsstk::TimeSystem::GPS);
   TUCATCH(convs = uut.getConversions());
   TUASSERTE(size_t, 2, convs.size());
   TUASSERTE(size_t, 1, convs.count(key1));
   TUASSERTE(size_t, 1, convs.count(key2));
   TURETURN();
}


int main()
{
   RinexTimeOffset_T testClass;
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
