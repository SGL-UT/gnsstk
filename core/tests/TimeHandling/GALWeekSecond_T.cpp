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

#include "GALWeekSecond.hpp"
#include "CommonTime.hpp"
#include "TestUtil.hpp"

using namespace gnsstk;
using namespace std;

class GALWeekSecond_T
{
public:
   GALWeekSecond_T() {}
   ~GALWeekSecond_T() {}

   unsigned initializationTest();
   unsigned nBitsTest();
   unsigned bitmaskTest();
   unsigned getPrintCharsTest();
   unsigned getDefaultFormatTest();
   unsigned printfTest();
   unsigned printErrorTest();
   unsigned setFromInfoTest();
   unsigned resetTest();
   unsigned weekRolloverAdjTest();


};

unsigned GALWeekSecond_T::initializationTest()
{
    TUDEF("GALWeekSecond", "Constructor");

    GALWeekSecond compare(1300,13500.,TimeSystem::GAL); //Initialize an object
       //--------------------------------------------------------------------
       //Were the attributes set to expectation with the explicit
       //constructor?
       //--------------------------------------------------------------------
    TUASSERTE(int, 1300, compare.getWeek());
    TUASSERTFE(13500, compare.getSOW());
    TUASSERTE(TimeSystem, TimeSystem::GAL, compare.getTimeSystem());
    
    testFramework.changeSourceMethod("ConstructorCopy");
    GALWeekSecond copy(compare); // Initialize with copy constructor
       //--------------------------------------------------------------------
       //Were the attributes set to expectation with the copy constructor?
       //--------------------------------------------------------------------
    TUASSERTE(int, 1300, copy.getWeek());
    TUASSERTFE(13500, copy.getSOW());
    TUASSERTE(TimeSystem, TimeSystem::GAL, copy.getTimeSystem());
    
    testFramework.changeSourceMethod("operator=");
    GALWeekSecond assigned;
    assigned = compare;
       //--------------------------------------------------------------------
       //Were the attributes set to expectation with the Set operator?
       //--------------------------------------------------------------------
    TUASSERTE(int, 1300, assigned.getWeek());
    TUASSERTFE(13500, assigned.getSOW());
    TUASSERTE(TimeSystem, TimeSystem::GAL, assigned.getTimeSystem());

    TURETURN();

}

unsigned GALWeekSecond_T::nBitsTest()
{
    TUDEF("GALWeekSecond", "nBits");
    GALWeekSecond galWeekSecond;
    TUASSERTE(int, 12, galWeekSecond.Nbits())
    TURETURN();

}

unsigned GALWeekSecond_T::bitmaskTest()
{
    TUDEF("GALWeekSecond", "bitmask");
    GALWeekSecond galWeekSecond;
    TUASSERTE(int, 0xFFF, galWeekSecond.bitmask())
    TURETURN();

}

unsigned GALWeekSecond_T::getPrintCharsTest()
{
    TUDEF("GALWeekSecond", "getPrintChars");
    GALWeekSecond galWeekSecond;
    TUASSERTE(string, "TLlwgP", galWeekSecond.getPrintChars())
    TURETURN();

}

unsigned GALWeekSecond_T::getDefaultFormatTest()
{
    TUDEF("GALWeekSecond", "getDefaultFormat");
    GALWeekSecond galWeekSecond;
    TUASSERTE(string, "%L %g %P", galWeekSecond.getDefaultFormat())
    TURETURN();

}

unsigned GALWeekSecond_T::printfTest()
{
    TUDEF("GALWeekSecond", "printf");

    GALWeekSecond galWeekSecondEmpty;
    TUASSERTE(string, "0 0 0 0.000000 GAL", galWeekSecondEmpty.printf("%L %l %w %g %P"));

    TURETURN();

}

unsigned GALWeekSecond_T::printErrorTest()
{
    TUDEF("GALWeekSecond", "printError");

    GALWeekSecond galWeekSecondEmpty;
    TUASSERTE(string, "BadGALfweek 0.000000 BadGALsys", galWeekSecondEmpty.printError("%L %g %P"));

    TURETURN();

}

unsigned GALWeekSecond_T::setFromInfoTest()
{
    TUDEF("GALWeekSecond", "setFromInfo");

    GALWeekSecond test1;
    TimeTag::IdToValue settingInfo;

    // Add dummy value to settingInfo and and check it does nothing 
    TUASSERTE(int, 0, test1.getWeek());
    TUASSERTE(double, 0, test1.getSOW());
    TUASSERTE(TimeSystem, TimeSystem::GAL, test1.getTimeSystem());
    settingInfo['Q'] = "not a value";
    test1.setFromInfo(settingInfo);
    TUASSERTE(int, 0, test1.getWeek());
    TUASSERTE(double, 0, test1.getSOW());
    TUASSERTE(TimeSystem, TimeSystem::GAL, test1.getTimeSystem());
    settingInfo.erase('Q');

    // Set week and SOW with setFromInfo
    settingInfo['L'] = "500";
    settingInfo['g'] = "10000";
    test1.setFromInfo(settingInfo);
    TUASSERTE(int, 500, test1.getWeek());
    TUASSERTE(double, 10000, test1.getSOW());
    TUASSERTE(TimeSystem, TimeSystem::GAL, test1.getTimeSystem());
    settingInfo.erase('L');
    settingInfo.erase('g');

    // Set time system with setFromInfo
    settingInfo['P'] = "GPS";
    test1.setFromInfo(settingInfo);
    TUASSERTE(TimeSystem, TimeSystem::GPS, test1.getTimeSystem());
    settingInfo.erase('P');

    GALWeekSecond test2;
    TUASSERTE(int, 0, test2.getWeek());
    TUASSERTE(double, 0, test2.getSOW());
    TUASSERTE(TimeSystem, TimeSystem::GAL, test2.getTimeSystem());

    // Set week by Epoch and MOD
    settingInfo['T'] = "2";
    settingInfo['l'] = "50";
    test2.setFromInfo(settingInfo);
    // 12bit max number 4096
    // 2 * 4096 + 50 = 8242 
    TUASSERTE(int, 8242, test2.getWeek());
    TUASSERTE(double, 0, test2.getSOW());
    TUASSERTE(TimeSystem, TimeSystem::GAL, test2.getTimeSystem());
    settingInfo.erase('T');
    settingInfo.erase('l');

    // set sow by days
    settingInfo['w'] = "4";
    test2.setFromInfo(settingInfo);
    // 86400 seconds in a day * 4 = 345600
    TUASSERTE(double, 345600, test2.getSOW());
    TURETURN();

}

unsigned GALWeekSecond_T::resetTest()
{
   TUDEF("GALWeekSecond", "reset");
   
   GALWeekSecond testReset(1300, 12000,TimeSystem::GPS);
   TUASSERTE(int, 1300, testReset.getWeek());
   TUASSERTE(double, 12000, testReset.getSOW());
   TUASSERTE(TimeSystem, TimeSystem::GPS, testReset.getTimeSystem());

   testReset.reset();

   TUASSERTE(int, 0, testReset.getWeek());
   TUASSERTE(double, 0, testReset.getSOW());
   TUASSERTE(TimeSystem, TimeSystem::GPS, testReset.getTimeSystem()); // reset does not reset time system

   TURETURN();
}

unsigned GALWeekSecond_T::weekRolloverAdjTest()
{
   TUDEF("GALWeekSecond", "weekRolloverAdj");

   GALWeekSecond sameOne(1300,0,TimeSystem::GPS);
   GALWeekSecond sameTwo(sameOne);
   TUASSERTE(GALWeekSecond, sameOne, sameTwo.weekRolloverAdj(sameOne));

   GALWeekSecond halfPlusWeek(1300,604700,TimeSystem::GPS);
   GALWeekSecond zeroWeek(1300,0,TimeSystem::GPS);
   GALWeekSecond oneWeekLess(1299,604700,TimeSystem::GPS);
   TUASSERTE(GALWeekSecond, oneWeekLess, halfPlusWeek.weekRolloverAdj(zeroWeek));

   GALWeekSecond testNegativeDiff(zeroWeek);
   GALWeekSecond oneWeekMore(1300,0,TimeSystem::GPS);
   TUASSERTE(GALWeekSecond, oneWeekMore, testNegativeDiff.weekRolloverAdj(halfPlusWeek));

   TURETURN();
}



int main() //Main function to initialize and run all tests above
{
   unsigned errorTotal = 0;
   GALWeekSecond_T testClass;

   errorTotal += testClass.initializationTest();
   errorTotal += testClass.nBitsTest();
   errorTotal += testClass.bitmaskTest();
   errorTotal += testClass.getPrintCharsTest();
   errorTotal += testClass.getDefaultFormatTest();
   errorTotal += testClass.printfTest();
   errorTotal += testClass.printErrorTest();
   errorTotal += testClass.setFromInfoTest();
   errorTotal += testClass.resetTest();
   errorTotal += testClass.weekRolloverAdjTest();

   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;

   return errorTotal; //Return the total number of errors
}

