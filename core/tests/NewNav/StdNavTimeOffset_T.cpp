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
#include "TimeString.hpp"
using namespace std;
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
      // Test using real data and exercise the scheduled leap second
   TestClass uut2;
   uut2.src = gpstk::TimeSystem::GPS;
   uut2.tgt = gpstk::TimeSystem::UTC;
   uut2.a0 = -3.72529030E-09;
   uut2.a1 = -9.76996262E-15;
   uut2.a2 = 0;
   uut2.deltatLS = 17.0;
   uut2.tot = 147456.0;
   uut2.wnot = 1929;
   uut2.refTime = gpstk::GPSWeekSecond(uut2.wnot,uut2.tot);
   uut2.wnLSF = 1929;
   uut2.dn = 7;
   uut2.deltatLSF = 18.0;
   uut2.effTime = gpstk::GPSWeekSecond(uut2.wnLSF, (uut2.dn-1)*86400);
      // WNLSF/DN is not in the past relative to user time and not
      // within 6 hours
   gpstk::CommonTime test2time1(gpstk::GPSWeekSecond(1929,496799));
   TUASSERTE(bool, true, uut2.getOffset(uut2.src,uut2.tgt,test2time1,offset));
      // verified by hand
   TUASSERTFE(16.99999999286164253931, offset);
      // WNLSF/DN is within 6 hours
   gpstk::CommonTime test2time2(gpstk::GPSWeekSecond(1929,496800));
   TUASSERTE(bool, true, uut2.getOffset(uut2.src,uut2.tgt,test2time2,offset));
      // tE == WNLSF/DN
   gpstk::CommonTime test2time3(gpstk::GPSWeekSecond(1929,518400));
   TUASSERTE(bool, true, uut2.getOffset(uut2.src,uut2.tgt,test2time3,offset));
      // verified by hand
   TUASSERTFE(17.99999999265060068865, offset);
      // Not testing uut2 with a time > 6 hours after the effectivity
      // time because it would yield invalid results (offset of ~17
      // seconds).
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
