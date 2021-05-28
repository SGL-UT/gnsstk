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
#include "StdNavTimeOffset.hpp"
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


/// Implement pure virtual methods so we can test.
class TestClass : public gpstk::StdNavTimeOffset
{
public:
   bool validate() const override
   { return true; }
};

class StdNavTimeOffset_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getOffsetTest();
   unsigned getConversionsTest();
};


unsigned StdNavTimeOffset_T ::
constructorTest()
{
   TUDEF("StdNavTimeOffset", "StdNavTimeOffset");
   TestClass uut;
   TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::Unknown, uut.src);
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


unsigned StdNavTimeOffset_T ::
getOffsetTest()
{
   TUDEF("StdNavTimeOffset", "getOffset");
   TestClass uut;
   gpstk::GPSWeekSecond ws1(2060, 405504.0);
   gpstk::GPSWeekSecond ws2(2061, 405504.0);
   uut.src = gpstk::TimeSystem::GPS;
   uut.tgt = gpstk::TimeSystem::UTC;
   uut.a0 = 1.9790604711E-09;
   uut.a1 = 7.5495165675E-15;
   uut.a2 = 0;
   uut.deltatLS = 18.0;
   uut.tot = 21600.0;
   uut.wnot = 2060;
   uut.refTime = gpstk::GPSWeekSecond(uut.wnot,uut.tot);
   double offset;
      /// @todo Truth values here need to be verified.
   TUASSERT(uut.getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC, ws1,
                          offset));
   TUASSERTFE(18.000000004877350079, offset);
   TUASSERT(uut.getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC, ws2,
                          offset));
   TUASSERTFE(18.000000009443297699, offset);
   gpstk::CommonTime utc2(ws2);
   utc2.setTimeSystem(gpstk::TimeSystem::UTC);
   TUASSERT(uut.getOffset(gpstk::TimeSystem::UTC, gpstk::TimeSystem::GPS, utc2,
                          offset));
   TUASSERTFE(-18.000000009443297699, offset);
   gpstk::CommonTime failTime(utc2);
   failTime.setTimeSystem(gpstk::TimeSystem::GLO);
   try
   {
      uut.getOffset(gpstk::TimeSystem::UTC, gpstk::TimeSystem::GPS, failTime,
                    offset);
      TUFAIL("Did not throw an exception when expected");
   }
   catch (gpstk::AssertionFailure)
   {
      TUPASS("");
   }
   catch (gpstk::Exception& exc)
   {
      std::cerr << exc << std::endl;
      TUFAIL("Not the expected exception");
   }
   TURETURN();
}


unsigned StdNavTimeOffset_T ::
getConversionsTest()
{
   TUDEF("StdNavTimeOffset", "getConversions");
   gpstk::TimeCvtSet convs;
   TestClass offs;
   offs.src = gpstk::TimeSystem::GAL;
   offs.tgt = gpstk::TimeSystem::GPS;
   gpstk::TimeCvtKey key1(gpstk::TimeSystem::GPS, gpstk::TimeSystem::GAL);
   gpstk::TimeCvtKey key2(gpstk::TimeSystem::GAL, gpstk::TimeSystem::GPS);
   TUCATCH(convs = offs.getConversions());
   TUASSERTE(size_t, 2, convs.size());
   TUASSERTE(size_t, 1, convs.count(key1));
   TUASSERTE(size_t, 1, convs.count(key2));
   TURETURN();
}


int main()
{
   StdNavTimeOffset_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getOffsetTest();
   errorTotal += testClass.getConversionsTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
