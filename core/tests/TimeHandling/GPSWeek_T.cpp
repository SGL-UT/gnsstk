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


#include "TestUtil.hpp"
#include "GPSWeek.hpp"

using namespace gnsstk;
using namespace std;


class TestGPSWeek : public GPSWeek {
public:
    TestGPSWeek(int w = 0, TimeSystem ts = TimeSystem::Unknown) : GPSWeek(w, ts) {}

    ~TestGPSWeek() {

    }

    unsigned int getDayOfWeek() const override {
        return week % 7;
    }
    
    CommonTime convertToCommonTime() const override {
        return CommonTime();
    }

    void convertFromCommonTime(const CommonTime& ct) override {
    }
};


class GPSWeek_T
{
public:
   GPSWeek_T(){

   };
   ~GPSWeek_T(){

   };
   
   unsigned initializeTest();
   unsigned operatorTest();
   unsigned setFromInfoTest(); 
   unsigned getEpochTest();
   unsigned getWeek10Test();
   unsigned getEpochWeek10Test();
   unsigned setEpochWeek10Test();
   unsigned getPrintCharsTest();
   unsigned getDefaultFormatTest();
   unsigned isValidTest();
};

unsigned GPSWeek_T::initializeTest(void)
{
   TUDEF("GPSWeek", "initialize");

   TestGPSWeek gpsWeek;
   TUASSERTE(int, 0, gpsWeek.week);
   TUASSERT(gpsWeek.getTimeSystem() == TimeSystem::Unknown);

   TestGPSWeek gpsWeekSetWeek(100);
   TUASSERTE(int, 100, gpsWeekSetWeek.week);
   TUASSERT(gpsWeek.getTimeSystem() == TimeSystem::Unknown);

    TestGPSWeek gpsWeekSetTimeSystem(100, TimeSystem::GPS);
   TUASSERTE(int, 100, gpsWeekSetTimeSystem.week);
   TUASSERT(gpsWeekSetTimeSystem.getTimeSystem() == TimeSystem::GPS);

   TURETURN();

}

unsigned GPSWeek_T::operatorTest(void)
{
   TUDEF("GPSWeek", "operator");

   // Test equal weeks
   TestGPSWeek compareWeekEqualOne(1000, TimeSystem::GPS);
   TestGPSWeek compareWeekEqualTwo(1000, TimeSystem::GPS);
   TUASSERT(compareWeekEqualOne == compareWeekEqualTwo);

   TestGPSWeek diffTimeSystemAny(1000, TimeSystem::Any);
   TUASSERT(compareWeekEqualOne == diffTimeSystemAny);
   TUASSERT(diffTimeSystemAny == compareWeekEqualOne);

   // Test un-equal weeks
   TestGPSWeek diffWeekOne(1000, TimeSystem::GPS);
   TestGPSWeek diffWeekTwo(2000, TimeSystem::GPS);
   TUASSERT(diffWeekOne != diffWeekTwo);
   TUASSERT(diffWeekTwo != diffWeekOne);

   TestGPSWeek diffTimeSystemOne(1000, TimeSystem::GPS);
   TestGPSWeek diffTimeSystemTwo(1000, TimeSystem::QZS);
   TUASSERT(!(diffTimeSystemOne == diffTimeSystemTwo));
   TUASSERT(!(diffTimeSystemTwo == diffTimeSystemOne));
   TUTHROW(diffTimeSystemOne != diffTimeSystemTwo);
   TUTHROW(diffTimeSystemTwo != diffTimeSystemOne);

   // Test less than than
   TestGPSWeek lessThan(1000, TimeSystem::GPS);
   TestGPSWeek greaterThan(2000, TimeSystem::GPS);
   TUASSERT(lessThan < greaterThan);
   TUASSERT(!(greaterThan < lessThan)); 
   TUTHROW(diffTimeSystemOne < diffTimeSystemTwo);
   TUTHROW(diffTimeSystemTwo < diffTimeSystemOne);
   TUASSERT(lessThan <= lessThan);
   TUASSERT(lessThan <= greaterThan);
   TUASSERT(!(greaterThan <= lessThan)); 
   TUTHROW(diffTimeSystemOne <= diffTimeSystemTwo);
   TUTHROW(diffTimeSystemTwo <= diffTimeSystemOne);


   // Test greater than
   TUASSERT(greaterThan > lessThan);
   TUASSERT(!(lessThan > greaterThan)); 
   TUTHROW(diffTimeSystemOne > diffTimeSystemTwo);
   TUTHROW(diffTimeSystemTwo > diffTimeSystemOne);
   TUASSERT(lessThan >= lessThan);
   TUASSERT(greaterThan >= lessThan);
   TUASSERT(!(lessThan >= greaterThan)); 
   TUTHROW(diffTimeSystemOne >= diffTimeSystemTwo);
   TUTHROW(diffTimeSystemTwo >= diffTimeSystemOne);


   TURETURN();

}

unsigned GPSWeek_T::setFromInfoTest(void)
{
   TUDEF("GPSWeek", "setFromInfo");
   
   TestGPSWeek gpsWeek1;
   TimeTag::IdToValue id1;
   id1['E'] = "2000";
   gpsWeek1.setFromInfo(id1);
   TUASSERTE(int, 2048000, gpsWeek1.week);
   
   TestGPSWeek gpsWeek2;
   TimeTag::IdToValue id2;
   id2['F'] = "2000";
   gpsWeek2.setFromInfo(id2);
   TUASSERTE(int, 2000, gpsWeek2.week);
   
   TestGPSWeek gpsWeek3;
   TimeTag::IdToValue id3;
   id3['G'] = "2000";
   gpsWeek3.setFromInfo(id3);
   TUASSERTE(int, 976, gpsWeek3.week);

   TestGPSWeek gpsWeek4(0, TimeSystem::GPS);
   TimeTag::IdToValue id4;
   id4['P'] = "2000";
   gpsWeek4.setFromInfo(id4);
   TUASSERTE(int, 0, gpsWeek4.week);
   TUASSERT(gpsWeek4.getTimeSystem() == TimeSystem::Unknown);


   TestGPSWeek gpsWeek5;
   TimeTag::IdToValue id5;
   id5['X'] = "2000"; // not a valid option
   gpsWeek5.setFromInfo(id5);
   TUASSERTE(int, 0, gpsWeek5.week);

   TURETURN();

}

unsigned GPSWeek_T::getEpochTest(void)
{
   TUDEF("GPSWeek", "getEpoch");
   
   TestGPSWeek underRolloverWeek(1000, TimeSystem::GPS);
   underRolloverWeek.setEpoch(0);
   TUASSERTE(int, 1000, underRolloverWeek.getWeek10());

   TestGPSWeek atRolloverWeek(1024, TimeSystem::GPS);
   atRolloverWeek.setEpoch(1);
   TUASSERTE(int, 0, atRolloverWeek.getWeek10());

   TestGPSWeek overRolloverWeek(2000, TimeSystem::GPS);
   overRolloverWeek.setEpoch(1);
   TUASSERTE(int, 976, overRolloverWeek.getWeek10());

   TestGPSWeek multiRolloverWeek(4000, TimeSystem::GPS);
   multiRolloverWeek.setEpoch(3);
   TUASSERTE(int, 928, multiRolloverWeek.getWeek10());

   TURETURN();
}
unsigned GPSWeek_T::getWeek10Test(void)
{
   TUDEF("GPSWeek", "getWeek10");

   TestGPSWeek underRolloverWeek(1000, TimeSystem::GPS);
   TUASSERTE(int, 1000, underRolloverWeek.getWeek10());

   TestGPSWeek atRolloverWeek(1024, TimeSystem::GPS);
   TUASSERTE(int, 0, atRolloverWeek.getWeek10());

   TestGPSWeek overRolloverWeek(2000, TimeSystem::GPS);
   TUASSERTE(int, 976, overRolloverWeek.getWeek10());

   TestGPSWeek multiRolloverWeek(4000, TimeSystem::GPS);
   TUASSERTE(int, 928, multiRolloverWeek.getWeek10());
   
   TURETURN();
}
unsigned GPSWeek_T::getEpochWeek10Test(void)
{
   TUDEF("GPSWeek", "getEpochWeek10");

   unsigned int e;
   unsigned int w;

   TestGPSWeek underRolloverWeek(1000, TimeSystem::GPS);
   underRolloverWeek.getEpochWeek10(e, w);
   TUASSERTE(int, underRolloverWeek.getEpoch(), e);
   TUASSERTE(int, underRolloverWeek.getWeek10(), w);

   TestGPSWeek atRolloverWeek(1024, TimeSystem::GPS);
   atRolloverWeek.getEpochWeek10(e, w);
   TUASSERTE(int, atRolloverWeek.getEpoch(), e);
   TUASSERTE(int, atRolloverWeek.getWeek10(), w);

   TestGPSWeek overRolloverWeek(2000, TimeSystem::GPS);
   overRolloverWeek.getEpochWeek10(e, w);
   TUASSERTE(int, overRolloverWeek.getEpoch(), e);
   TUASSERTE(int, overRolloverWeek.getWeek10(), w);

   TestGPSWeek multiRolloverWeek(4000, TimeSystem::GPS);
   multiRolloverWeek.getEpochWeek10(e, w);
   TUASSERTE(int, multiRolloverWeek.getEpoch(), e);
   TUASSERTE(int, multiRolloverWeek.getWeek10(), w);
   
   TURETURN();
}
unsigned GPSWeek_T::setEpochWeek10Test(void)
{
   TUDEF("GPSWeek", "setEpochWeek10");

   unsigned int e;
   unsigned int w;

   TestGPSWeek underRolloverWeek(1000, TimeSystem::GPS);
   underRolloverWeek.setEpochWeek10(0, 0);
   underRolloverWeek.getEpochWeek10(e, w);
   TUASSERTE(int, 0, e);
   TUASSERTE(int, 0, w);

   TestGPSWeek atRolloverWeek(1024, TimeSystem::GPS);
   atRolloverWeek.setEpochWeek10(1, 10);
   atRolloverWeek.getEpochWeek10(e, w);
   TUASSERTE(int, 1, e);
   TUASSERTE(int, 10, w);

   TestGPSWeek overRolloverWeek(2000, TimeSystem::GPS);
   overRolloverWeek.setEpochWeek10(1, 20);
   overRolloverWeek.getEpochWeek10(e, w);
   TUASSERTE(int, 1, e);
   TUASSERTE(int, 20, w);

   TestGPSWeek multiRolloverWeek(4000, TimeSystem::GPS);
   multiRolloverWeek.setEpochWeek10(3, 200);
   multiRolloverWeek.getEpochWeek10(e, w);
   TUASSERTE(int, 3, e);
   TUASSERTE(int, 200, w);
   
   TURETURN();
}


unsigned GPSWeek_T::getPrintCharsTest(void) 
{
   TUDEF("GPSWeek", "getPrintChars");
   
   TestGPSWeek gpsWeek;
   TUASSERTE(string, "EFGP", gpsWeek.getPrintChars());
   
   TURETURN();
}
unsigned GPSWeek_T::getDefaultFormatTest(void) 
{
   TUDEF("GPSWeek", "getDefaultFormat");
   
   TestGPSWeek gpsWeek;
   TUASSERTE(string, "%04F", gpsWeek.getDefaultFormat());
   
   TURETURN(); 
}
unsigned GPSWeek_T::isValidTest(void) 
{
   TUDEF("GPSWeek", "isValid");

   TestGPSWeek negativeWeek(-100, TimeSystem::GPS);
   TUASSERTE(bool, false, negativeWeek.isValid());

   TestGPSWeek toLargeWeek(142601, TimeSystem::GPS); // 1 more than max week
   TUASSERTE(bool, false, toLargeWeek.isValid());

   TestGPSWeek normalWeek(3000, TimeSystem::GPS);
   TUASSERTE(bool, true, normalWeek.isValid());


   TURETURN();
}

int main() //Main function to initialize and run all tests above
{
   GPSWeek_T testClass;

   unsigned errorTotal = 0;

   errorTotal += testClass.initializeTest();
   errorTotal += testClass.operatorTest();
   errorTotal += testClass.getEpochTest();
   errorTotal += testClass.getWeek10Test();
   errorTotal += testClass.getEpochWeek10Test();
   errorTotal += testClass.setEpochWeek10Test();
   errorTotal += testClass.setFromInfoTest();
   errorTotal += testClass.getPrintCharsTest();
   errorTotal += testClass.getDefaultFormatTest();
   errorTotal += testClass.isValidTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal; //Return the total number of errors
}