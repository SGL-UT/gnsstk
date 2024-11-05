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
#include "Week.hpp"

using namespace gnsstk;
using namespace std;

class TestWeek : public gnsstk::Week {
public:
    TestWeek(int w = 0, TimeSystem ts = TimeSystem::Unknown)
        : Week(w, ts) {}

    int bitmask() const override {
        return 0x3FF; //1024
    }

   int Nbits() const override {
        return 10;
    }

   long MJDEpoch() const override {
        return GPS_EPOCH_MJD; // 1979/1/1
    }

    // From here down these methods are unused by the class and here strictly to meet override requirements

   unsigned int getDayOfWeek() const override {
        return week % 7;
    }
    
   CommonTime convertToCommonTime() const override {
        return CommonTime();
    }

   void convertFromCommonTime(const CommonTime& ct) override {
    }

   std::string printf(const std::string& fmt) const override {
        return "SimpleTimeTag output";
    }

   std::string printError(const std::string& fmt) const override {
        return "Error in SimpleTimeTag";
    }

   bool setFromInfo(const IdToValue& info) override {
        return true;
    }
};


class Week_T
{
public:
   Week_T(){

   };
   ~Week_T(){

   };
   
   unsigned rolloverTest();
   unsigned operatorsTest();
   unsigned getWeekTest();
   unsigned getModWeekTest();
   unsigned getEpochTest();
   unsigned getEpochModWeekTest();
   unsigned setEpochTest();
   unsigned setModWeekTest();
   unsigned setEpochModWeekTest();
   unsigned adjustToYearTest();
   unsigned getPrintCharsTest();
   unsigned getDefaultFormatTest();
   unsigned isValidTest();
};

unsigned Week_T::rolloverTest(void)
{
   TUDEF("Week", "rollover");

   TestWeek week(1000, TimeSystem::GPS);
   TUASSERTE(int, 1024, week.rollover()); // returns roll over point

   TURETURN();

}

unsigned Week_T::operatorsTest(void)
{
   TUDEF("Week", "operators");

   // Test equal weeks
   TestWeek compareWeekEqualOne(1000, TimeSystem::GPS);
   TestWeek compareWeekEqualTwo(1000, TimeSystem::GPS);
   TUASSERT(compareWeekEqualOne == compareWeekEqualTwo);

   TestWeek diffTimeSystemAny(1000, TimeSystem::Any);
   TUASSERT(compareWeekEqualOne == diffTimeSystemAny);
   TUASSERT(diffTimeSystemAny == compareWeekEqualOne);

   // Test un-equal weeks
   TestWeek diffWeekOne(1000, TimeSystem::GPS);
   TestWeek diffWeekTwo(2000, TimeSystem::GPS);
   TUASSERT(diffWeekOne != diffWeekTwo);
   TUASSERT(diffWeekTwo != diffWeekOne);

   TestWeek diffTimeSystemOne(1000, TimeSystem::GPS);
   TestWeek diffTimeSystemTwo(1000, TimeSystem::QZS);
   TUASSERT(!(diffTimeSystemOne == diffTimeSystemTwo));
   TUASSERT(!(diffTimeSystemTwo == diffTimeSystemOne));
   TUTHROW(diffTimeSystemOne != diffTimeSystemTwo);
   TUTHROW(diffTimeSystemTwo != diffTimeSystemOne);

   // Test less than than
   TestWeek lessThan(1000, TimeSystem::GPS);
   TestWeek greaterThan(2000, TimeSystem::GPS);
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

unsigned Week_T::getWeekTest(void)
{
   TUDEF("Week", "getWeek");

   TestWeek week(1000, TimeSystem::GPS);
   TUASSERTE(int, 1000, week.getWeek());

   TURETURN();

}

unsigned Week_T::getModWeekTest(void)
{
   TUDEF("Week", "getModWeek");

   TestWeek underRolloverWeek(1000, TimeSystem::GPS);
   TUASSERTE(unsigned int, 1000, underRolloverWeek.getModWeek());

   TestWeek atRolloverWeek(1024, TimeSystem::GPS);
   TUASSERTE(unsigned int, 0, atRolloverWeek.getModWeek());

   TestWeek overRolloverWeek(2000, TimeSystem::GPS);
   TUASSERTE(unsigned int, 976, overRolloverWeek.getModWeek());

   TestWeek multiRolloverWeek(4000, TimeSystem::GPS);
   TUASSERTE(unsigned int, 928, multiRolloverWeek.getModWeek());

   TURETURN();

}

unsigned Week_T::getEpochTest(void)
{
   TUDEF("Week", "getEpoch");

   TestWeek underRolloverWeek(1000, TimeSystem::GPS);
   TUASSERTE(unsigned int, 0, underRolloverWeek.getEpoch());

   TestWeek atRolloverWeek(1024, TimeSystem::GPS);
   TUASSERTE(unsigned int, 1, atRolloverWeek.getEpoch());

   TestWeek overRolloverWeek(2000, TimeSystem::GPS);
   TUASSERTE(unsigned int, 1, overRolloverWeek.getEpoch());

   TestWeek multiRolloverWeek(4000, TimeSystem::GPS);
   TUASSERTE(unsigned int, 3, multiRolloverWeek.getEpoch());

   TURETURN();

}

unsigned Week_T::getEpochModWeekTest(void)
{
   TUDEF("Week", "getEpochModWeek");

   unsigned int e;
   unsigned int w;
   TestWeek underRolloverWeek(1000, TimeSystem::GPS);
   underRolloverWeek.getEpochModWeek(e, w);
   TUASSERTE(unsigned int, 0, e);
   TUASSERTE(unsigned int, 1000, w);

   TestWeek atRolloverWeek(1024, TimeSystem::GPS);
   atRolloverWeek.getEpochModWeek(e, w);
   TUASSERTE(unsigned int, 1, e);
   TUASSERTE(unsigned int, 0, w);

   TestWeek overRolloverWeek(2000, TimeSystem::GPS);
   overRolloverWeek.getEpochModWeek(e, w);
   TUASSERTE(unsigned int, 1, e);
   TUASSERTE(unsigned int, 976, w);

   TestWeek multiRolloverWeek(4000, TimeSystem::GPS);
   multiRolloverWeek.getEpochModWeek(e, w);
   TUASSERTE(unsigned int, 3, e);
   TUASSERTE(unsigned int, 928, w);
   TURETURN();

}

unsigned Week_T::setEpochTest(void)
{
   TUDEF("Week", "setEpoch");

   TestWeek underRolloverWeek(1000, TimeSystem::GPS);
   underRolloverWeek.setEpoch(0);
   TUASSERTE(int, 1000, underRolloverWeek.getWeek());

   TestWeek atRolloverWeek(1024, TimeSystem::GPS);
   atRolloverWeek.setEpoch(1);
   TUASSERTE(int, 1024, atRolloverWeek.getWeek());

   TestWeek overRolloverWeek(2000, TimeSystem::GPS);
   overRolloverWeek.setEpoch(1);
   TUASSERTE(int, 2000, overRolloverWeek.getWeek());

   TestWeek multiRolloverWeek(4000, TimeSystem::GPS);
   multiRolloverWeek.setEpoch(3);
   TUASSERTE(int, 4000, multiRolloverWeek.getWeek());

   TestWeek mixMatchWeek(2000, TimeSystem::GPS);
   mixMatchWeek.setEpoch(0);
   TUASSERTE(int, 976, mixMatchWeek.getWeek());

   TURETURN();

}

unsigned Week_T::setModWeekTest(void)
{
   TUDEF("Week", "setModWeek");

   TestWeek underRolloverWeek(1000, TimeSystem::GPS);
   underRolloverWeek.setModWeek(100);
   TUASSERTE(int, 100, underRolloverWeek.getWeek());

   TestWeek atRolloverWeek(1024, TimeSystem::GPS);
   atRolloverWeek.setModWeek(100);
   TUASSERTE(int, 1124, atRolloverWeek.getWeek());

   TestWeek overRolloverWeek(2000, TimeSystem::GPS);
   overRolloverWeek.setModWeek(100);
   TUASSERTE(int, 1124, overRolloverWeek.getWeek());

   TestWeek multiRolloverWeek(4000, TimeSystem::GPS);
   multiRolloverWeek.setModWeek(100);
   TUASSERTE(int, 3172, multiRolloverWeek.getWeek());


   TURETURN();

}

unsigned Week_T::setEpochModWeekTest(void)
{
   TUDEF("Week", "setEpochModWeek");

   TestWeek underRolloverWeek(1000, TimeSystem::GPS);
   underRolloverWeek.setEpochModWeek(0, 100);
   TUASSERTE(int, 100, underRolloverWeek.getWeek());

   TestWeek atRolloverWeek(1024, TimeSystem::GPS);
   atRolloverWeek.setEpochModWeek(1, 100);
   TUASSERTE(int, 1124, atRolloverWeek.getWeek());

   TestWeek overRolloverWeek(2000, TimeSystem::GPS);
   overRolloverWeek.setEpochModWeek(1, 100);
   TUASSERTE(int, 1124, overRolloverWeek.getWeek());

   TestWeek multiRolloverWeek(4000, TimeSystem::GPS);
   multiRolloverWeek.setEpochModWeek(3, 100);
   TUASSERTE(int, 3172, multiRolloverWeek.getWeek());

   TURETURN();

}

unsigned Week_T::adjustToYearTest(void)
{
   TUDEF("Week", "adjustToYear");

   // Leap Year
   TestWeek leapYearWeek(3000, TimeSystem::GPS);
   leapYearWeek.adjustToYear(2020);
   TUASSERTE(int, 3000, leapYearWeek.getWeek());

   TURETURN();

}

unsigned Week_T::getPrintCharsTest(void)
{
   TUDEF("Week", "getPrintChars");

   TestWeek week(1000, TimeSystem::GPS);
   TUASSERTE(std::string, "EFGP", week.getPrintChars());

   TURETURN();

}

unsigned Week_T::getDefaultFormatTest(void)
{
   TUDEF("Week", "getDefaultFormat");

   TestWeek week(1000, TimeSystem::GPS);
   TUASSERTE(std::string, "%04F", week.getDefaultFormat());

   TURETURN();

}

unsigned Week_T::isValidTest(void)
{
   TUDEF("Week", "isValid");

   TestWeek negativeWeek(-100, TimeSystem::GPS);
   TUASSERTE(bool, false, negativeWeek.isValid());

   TestWeek toLargeWeek(negativeWeek.MAXWEEK() + 1, TimeSystem::GPS);
   TUASSERTE(bool, false, toLargeWeek.isValid());

   TestWeek normalWeek(3000, TimeSystem::GPS);
   TUASSERTE(bool, true, normalWeek.isValid());

   TURETURN();

}


int main() //Main function to initialize and run all tests above
{
   Week_T testClass;

   unsigned errorTotal = 0;

   errorTotal += testClass.rolloverTest();
   errorTotal += testClass.operatorsTest();
   errorTotal += testClass.getWeekTest();
   errorTotal += testClass.getModWeekTest();
   errorTotal += testClass.getEpochTest();
   errorTotal += testClass.getEpochModWeekTest();
   errorTotal += testClass.setEpochTest();
   errorTotal += testClass.setModWeekTest();
   errorTotal += testClass.setEpochModWeekTest();
   errorTotal += testClass.adjustToYearTest();
   errorTotal += testClass.getPrintCharsTest();
   errorTotal += testClass.getDefaultFormatTest();
   errorTotal += testClass.isValidTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal; //Return the total number of errors
}