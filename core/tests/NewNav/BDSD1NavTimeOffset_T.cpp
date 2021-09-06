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
#include "BDSD1NavTimeOffset.hpp"
#include "TestUtil.hpp"
#include "BDSWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class BDSD1NavTimeOffset_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned validateTest();
   unsigned getUserTimeTest();
   unsigned getOffsetTest();
   unsigned getConversionsTest();
};


unsigned BDSD1NavTimeOffset_T ::
constructorTest()
{
   TUDEF("BDSD1NavTimeOffset", "BDSD1NavTimeOffset");
   gpstk::BDSD1NavTimeOffset uut;
   TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::Unknown, uut.tgt);
   TUASSERTFE(0.0, uut.a0);
   TUASSERTFE(0.0, uut.a1);
   TUASSERTFE(0.0, uut.a2);
   TUASSERTFE(0.0, uut.deltatLS);
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime(), uut.refTime);
   TUASSERTFE(0.0, uut.tot);
   TUASSERTE(unsigned, 0, uut.wnot);
   TUASSERTE(unsigned, 0, uut.wnLSF);
   TUASSERTE(unsigned, 0, uut.dn);
   TUASSERTFE(0.0, uut.deltatLSF);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::TimeOffset,
             uut.signal.messageType);
   TURETURN();
}


unsigned BDSD1NavTimeOffset_T ::
validateTest()
{
   TUDEF("BDSD1NavTimeOffset", "validate");
   gpstk::BDSD1NavTimeOffset offs;
   TUASSERTE(bool, false, offs.validate());
   offs.a0 = 1;
   TUASSERTE(bool, false, offs.validate());
   offs.a1 = 1;
   TUASSERTE(bool, true, offs.validate());
   offs.dn = 6;
   TUASSERTE(bool, true, offs.validate());
   offs.dn = 7;
   TUASSERTE(bool, false, offs.validate());
   TURETURN();
}


unsigned BDSD1NavTimeOffset_T ::
getUserTimeTest()
{
   TUDEF("BDSD1NavTimeOffset", "getUserTime");
   gpstk::BDSD1NavTimeOffset uut;
   gpstk::CommonTime exp(gpstk::BDSWeekSecond(2100,141.0));
   uut.timeStamp = gpstk::BDSWeekSecond(2100,135.0);
   TUASSERTE(gpstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned BDSD1NavTimeOffset_T ::
getOffsetTest()
{
   TUDEF("BDSD1NavTimeOffset", "getOffset");
   gpstk::BDSD1NavTimeOffset uut;
   gpstk::BDSWeekSecond ws1(2060, 405504.0);
   gpstk::BDSWeekSecond ws2(2061, 405504.0);
   uut.tgt = gpstk::TimeSystem::UTC;
   uut.a0 = 1.9790604711E-09;
   uut.a1 = 7.5495165675E-15;
   uut.a2 = 0;
   uut.deltatLS = 18.0;
   uut.tot = 21600.0;
   uut.wnot = 2060;
   uut.refTime = gpstk::BDSWeekSecond(uut.wnot,uut.tot);
   double offset;
      /// @todo Truth values here need to be verified.
   TUASSERT(uut.getOffset(gpstk::TimeSystem::BDT, gpstk::TimeSystem::UTC, ws1,
                          offset));
   TUASSERTFE(18.000000004877350079, offset);
   TUASSERT(uut.getOffset(gpstk::TimeSystem::BDT, gpstk::TimeSystem::UTC, ws2,
                          offset));
   TUASSERTFE(18.000000009443297699, offset);
   TURETURN();
}


unsigned BDSD1NavTimeOffset_T ::
getConversionsTest()
{
   TUDEF("BDSD1NavTimeOffset", "getConversions");
   gpstk::TimeCvtSet convs;
   gpstk::BDSD1NavTimeOffset offs;
      // This looks a bit weird, but basically getConversions is
      // expected to return a set containing one key pair for BDS to
      // the target time system, which by default is Unknown.
   gpstk::TimeCvtKey key1(gpstk::TimeSystem::BDT,
                                          gpstk::TimeSystem::Unknown);
   gpstk::TimeCvtKey key2(gpstk::TimeSystem::Unknown,
                                          gpstk::TimeSystem::BDT);
   TUCATCH(convs = offs.getConversions());
   TUASSERTE(size_t, 2, convs.size());
   TUASSERTE(size_t, 1, convs.count(key1));
   TUASSERTE(size_t, 1, convs.count(key2));
   TURETURN();
}


int main()
{
   BDSD1NavTimeOffset_T testClass;
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
