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

#include "Epoch.hpp"
#include "TestUtil.hpp"

using namespace std;
using namespace gnsstk;


class Epoch_T
{
   public:
      Epoch_T(){eps = 1e-12;}
      ~Epoch_T() {}
      int initializationTest ();
      int operatorTest ();
      int setFunctionsTest() ;
      int addSecondsTest();


double eps;
};

//initializationTest ensures the constructors set the values properly
int Epoch_T:: initializationTest (void)
{
   TUDEF("Epoch", "Contructor related tests");
   CommonTime ct = CivilTime(2024,11,20, 13,30,15 ).convertToCommonTime();

   Epoch obj1(ct); //Initialize with CommonTime object
  
   TUASSERTE(int, 2024, (obj1.year()));
   TUASSERTE(int, 11, (obj1.month()));
   TUASSERTE(int, 20, (obj1.day()));
   TUASSERTE(int, 13, (obj1.hour()));
   TUASSERTE(int, 30, (obj1.minute()));
   TUASSERTE(double, 15, (obj1.second()));

   GPSWeekSecond ws(1300,13500.,TimeSystem::GPS);
   Epoch obj2(ws,2024); //Initialize with GPSWeekSecond object

   TUASSERTE(int, 13500, obj2.GPSsow());
   TUASSERTE(double, 2024, obj2.year());
   TUASSERTE(double, 2324, obj2.GPSweek());

   GPSWeekZcount gz(1300,13500.,TimeSystem(2));
   Epoch obj3(gz); //Initialize with GPSWeekZcount object
  
   TUASSERTE(double, 13500, obj3.GPSzcount());
   TUASSERTE(double, 1300, obj3.GPSweek());

   TURETURN();
}

int Epoch_T:: operatorTest (void)
{
   TUDEF( "Epoch", "Operator tests");

   Epoch today, yesterday, tomorrow;
   today.set(CivilTime(2024,10,20, 13,30,15 ).convertToCommonTime());
   yesterday.set(CivilTime(2024,10,19, 13,30,15 ).convertToCommonTime());
   tomorrow.set(CivilTime(2024,10,21, 13,30,15 ).convertToCommonTime());

   Epoch test_yesterday, test_tomorrow, orig_today, orig_yesterday;

   test_yesterday = today - SEC_PER_DAY;
   test_tomorrow = today + SEC_PER_DAY;

   TUASSERTE(Epoch, test_yesterday, yesterday);
   TUASSERTE(Epoch, test_tomorrow, tomorrow);

   orig_today = today;
   orig_yesterday = yesterday;

   long seconds = SEC_PER_DAY; //1 day

   today -= seconds;   
   TUASSERTE(Epoch, today, yesterday);
   today += seconds;
   TUASSERTE(Epoch, today, orig_today);

   testFramework.assert(today > yesterday,     "operator >  did not check Epochs greater than function properly ", __LINE__);
   testFramework.assert(yesterday < today,     "operator <  did not check Epochs greater than function properly ", __LINE__);
   testFramework.assert(today >= orig_today,   "operator >=  did not check Epochs greater than function properly ", __LINE__);
   testFramework.assert(yesterday <= orig_yesterday,   "operator <=  did not check Epochs greater than function properly ", __LINE__);

   TURETURN();

}

int Epoch_T:: setFunctionsTest()
{
   TUDEF( "Epoch", "set-function related tests");
   Epoch obj1;
   obj1.set(CivilTime(2024,10,20, 13,30,15 ).convertToCommonTime());

   TUASSERTE(int, 20, obj1.day());
   TUASSERTE(int, 10, obj1.month());
   TUASSERTE(int, 2024, obj1.year());

   // Get the current time
   std::time_t now = std::time(0);
   // Convert to local time
   std::tm* local = std::localtime(&now);
   // Extract day, month, and year
   int day = local->tm_mday;
   int month = local->tm_mon + 1; // tm_mon is 0-indexed
   int year = local->tm_year + 1900; // tm_year is years since 1900

   // Test setLocalTime()
   Epoch obj2;
   obj2.setLocalTime();

   TUASSERTE(int, day, obj2.day());
   TUASSERTE(int, month, obj2.month());
   TUASSERTE(int, year, obj2.year());

   //Test setTolerance()
   obj2.setTolerance(60);
   TUASSERTE(int, 60, obj2.getTolerance());

   // Test setDate()
   obj2.set(CivilTime(2024,10,20, 13,30,15 ).convertToCommonTime());
   obj2.setDate(CivilTime(2023,11,8, 23,10,45 ).convertToCommonTime());

   TUASSERTE(int, 8, obj2.day());
   TUASSERTE(int, 11, obj2.month());
   TUASSERTE(int, 2023, obj2.year());

   //HH:MM:SS should remain unchanged
   TUASSERTE(int, 13, obj2.hour());
   TUASSERTE(int, 30, obj2.minute());
   TUASSERTE(int, 15, obj2.second());

   // Test setTime()
   obj2.setTime(CivilTime(2024,10,20, 12,12,12 ).convertToCommonTime());

   //YY:MM:DD should remain unchanged
   TUASSERTE(int, 8, obj2.day());
   TUASSERTE(int, 11, obj2.month());
   TUASSERTE(int, 2023, obj2.year());
   TUASSERTE(int, 12, obj2.hour());
   TUASSERTE(int, 12, obj2.minute());
   TUASSERTE(int, 12, obj2.second());

   //Set with GPSWeekZcount object
   GPSWeekZcount gz(1300,13500.,TimeSystem(2));
   Epoch obj3;
   obj3.set(gz);
   TUASSERTE(double, 13500, obj3.GPSzcount());
   TUASSERTE(double, 1300, obj3.GPSweek());

   //Set with GPSZcount object
   Epoch obj4;
   GPSZcount gzObj(10, 35);
   obj4.set(gzObj);
   TUASSERTE(int, 10, obj4.GPSweek());
   TUASSERTE(int, 35, obj4.GPSzcount());

   TURETURN();

}
int Epoch_T:: addSecondsTest()
{
   TUDEF( "Epoch", "addseconds tests");
   Epoch today, orig_today;
   Epoch three_days_after, three_days_before;
   today.set(CivilTime(2024,10,20, 13,30,15 ).convertToCommonTime());
   orig_today = today;

   // Test addSeconds()
   long seconds_long =  SEC_PER_DAY * 3 ; //3 days
   three_days_after = today + (3 * SEC_PER_DAY);
   three_days_before = today - (3 * SEC_PER_DAY);
   today = today.addSeconds(seconds_long);
   TUASSERTE(Epoch, today, three_days_after);
   
   // Test addMilliSeconds()
   today = orig_today; 
   long milliseconds = seconds_long * 1000;
   today = today.addMilliSeconds(milliseconds);
   TUASSERTE(Epoch, today, three_days_after);

   // Test addMicroSeconds(). Currently this test fails in windows only. Fix in progress.
   /*
   today = orig_today; 
   long microseconds = 2.592e+11 ;
   today = today.addMicroSeconds(microseconds);
   TUASSERTE(Epoch, today, three_days_after);
   */
   TURETURN();
  

}

int main()
{
   int check, errorCounter = 0;
   Epoch_T testClass;

   errorCounter += testClass.initializationTest();
   errorCounter += testClass.operatorTest();
   errorCounter += testClass.setFunctionsTest();
   errorCounter += testClass.addSecondsTest();

   return errorCounter;
}
