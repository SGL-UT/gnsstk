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

#include "TimeTag.hpp"
#include "ANSITime.hpp"
#include "CivilTime.hpp"
#include "MJD.hpp"
#include "JulianDate.hpp"
#include "GPSWeekSecond.hpp"
#include "GPSWeekZcount.hpp"
#include "UnixTime.hpp"
#include "YDSTime.hpp"
#include "TestUtil.hpp"
#include "BasicTimeSystemConverter.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>

using namespace gnsstk;
using namespace std;

// This test file will contain a series of scanf checks for each of
// the directly tested TimeTag classes.

class TimeTag_T
{
public:
   TimeTag_T() {}
   ~TimeTag_T() {}

//	ANSITime scanf Test
   unsigned scanfANSITime()
   {
      TUDEF("TimeTag", "scanf(ANSITime)");


         //Set a hardcoded time
      ANSITime hardCodedTime(13500000,TimeSystem(2));
         //Provide a format string
      std::string formatString = "%08K %03P";
         //Print the time using that format
      std::string timeString = hardCodedTime.printf(formatString);
      ANSITime scannedTime;

         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(ANSITime, hardCodedTime, scannedTime);

      TURETURN();
   }

//	CivilTime scanf Test
   unsigned scanfCivilTime()
   {
      TUDEF("TimeTag", "scanf(CivilTime)");


         //Set a hardcoded time
      CivilTime hardCodedTime(2008,8,21,13,30,15.,TimeSystem::UTC);
         //Provide a format string
      std::string formatString = "%04Y %02m %02d %02H %02M %02S %03P";
         //Print the time using that format
      std::string timeString = hardCodedTime.printf(formatString);
      CivilTime scannedTime;

         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(CivilTime, hardCodedTime, scannedTime);

         //Reset the time for a new format to be scanned
      scannedTime.reset();


         //Provide a format string
      formatString = "%02y %02m %02d %02H %02M %02S %03P";
         //Print the time using that format
      timeString = hardCodedTime.printf(formatString);
         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(CivilTime, hardCodedTime, scannedTime);

         //Reset the time for a new format to be scanned
      scannedTime.reset();

         //Provide a format string
      formatString = "%04Y %03b %02d %02H %02M %02S %03P";
         //Print the time using that format
      timeString = hardCodedTime.printf(formatString);
         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(CivilTime, hardCodedTime, scannedTime);

         //Reset the time for a new format to be scanned
      scannedTime.reset();

         //Provide a format string
      formatString = "%04Y %03b %02d %02H %02M %5.2f %03P";
         //Print the time using that format
      timeString = hardCodedTime.printf(formatString);
         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(CivilTime, hardCodedTime, scannedTime);

         //Reset the time for a new format to be scanned
      scannedTime.reset();

         //Provide a format string
      formatString = "%02y %03b %02d %02H %02M %5.2f %03P";
         //Print the time using that format
      timeString = hardCodedTime.printf(formatString);
         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(CivilTime, hardCodedTime, scannedTime);

         //Reset the time for a new format to be scanned
      scannedTime.reset();

      TURETURN();
   }



//	GPSWeekSecond scanf Test
   unsigned scanfGPSWeekSecond()
   {
      TUDEF("TimeTag", "scanf(GPSWeekSecond)");


         //Set a hardcoded time
      GPSWeekSecond hardCodedTime(1300,13500.,TimeSystem::GPS);
         //Provide a format string
      std::string formatString = "%04F %8.2g %03P";
         //Print the time using that format
      std::string timeString = hardCodedTime.printf(formatString);
      GPSWeekSecond scannedTime;

         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(GPSWeekSecond, hardCodedTime, scannedTime);

      TURETURN();
   }

//	GPSWeekZcount scanf Test
   unsigned scanfGPSWeekZcount()
   {
      TUDEF("TimeTag", "scanf(GPSWeekZcount)");


         //Set a hardcoded time
      GPSWeekZcount hardCodedTime(1300,13500.,TimeSystem(2));
         //Provide a format string
      std::string formatString = "%04F %05z %03P";
         //Print the time using that format
      std::string timeString = hardCodedTime.printf(formatString);
      GPSWeekZcount scannedTime;

         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(GPSWeekZcount, hardCodedTime, scannedTime);

         //Reset the time for a new format to be scanned
      scannedTime.reset();


         //Provide a format string
      formatString = "%04F %10C %03P";
         //Print the time using that format
      timeString = hardCodedTime.printf(formatString);
         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(GPSWeekZcount, hardCodedTime, scannedTime);

         //Reset the time for a new format to be scanned
      scannedTime.reset();


         //Provide a format string
      formatString = "%04F %10c %03P";
         //Print the time using that format
      timeString = hardCodedTime.printf(formatString);
         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(GPSWeekZcount, hardCodedTime, scannedTime);

      TURETURN();
   }

//	JulianDate scanf Test
   unsigned scanfJulianDate()
   {
      TUDEF("TimeTag", "scanf(JulianDate)");


         //Set a hardcoded time
      JulianDate hardCodedTime(1234567,TimeSystem(2));
         //Provide a format string
      std::string formatString = "%10.2J %03P";
         //Print the time using that format
      std::string timeString = hardCodedTime.printf(formatString);
      JulianDate scannedTime;

         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(JulianDate, hardCodedTime, scannedTime);

      TURETURN();
   }

//	MJD scanf Test
   unsigned scanfMJD()
   {
      TUDEF("TimeTag", "scanf(MJD)");


         //Set a hardcoded time
      MJD hardCodedTime(123456,TimeSystem(1));
         //Provide a format string
      std::string formatString = "%08Q %03P";
         //Print the time using that format
      std::string timeString = hardCodedTime.printf(formatString);
      MJD scannedTime;

         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(MJD, hardCodedTime, scannedTime);

      TURETURN();
   }

//	UnixTime scanf Test
   unsigned scanfUnixTime()
   {
      TUDEF("TimeTag", "scanf(UnixTime)");


         //Set a hardcoded time
      UnixTime hardCodedTime(1654321,10,TimeSystem(5));
         //Provide a format string
      std::string formatString = "%07U %02u %03P";
         //Print the time using that format
      std::string timeString = hardCodedTime.printf(formatString);
      UnixTime scannedTime;

         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(UnixTime, hardCodedTime, scannedTime);

      TURETURN();
   }


//	YDSTime scanf Test
   unsigned scanfYDSTime()
   {
      TUDEF("TimeTag", "scanf(YDSTime)");


         //Set a hardcoded time
      YDSTime hardCodedTime(2008,200,1000,TimeSystem::GPS);
         //Provide a format string
      std::string formatString = "%04Y %03j %7.2s %03P";
         //Print the time using that format
      std::string timeString = hardCodedTime.printf(formatString);
      YDSTime scannedTime;

         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(YDSTime, hardCodedTime, scannedTime);
         //Reset the time for a new format to be scanned
      scannedTime.reset();


         //Provide a format string
      formatString = "%02y %03j %7.2s %03P";
         //Print the time using that format
      timeString = hardCodedTime.printf(formatString);
         //Read the formatted string back into a new time variable
      scannedTime.scanf(timeString, formatString);

         //Verify the scanned time is the same as the hardcoded time
      TUASSERTE(YDSTime, hardCodedTime, scannedTime);

      TURETURN();
   }

   unsigned changeTimeSystemTest();
};


unsigned TimeTag_T ::
changeTimeSystemTest()
{
   TUDEF("TimeTag", "changeTimeSystem");
   std::shared_ptr<TimeSystemConverter> btscShared =
      make_shared<gnsstk::BasicTimeSystemConverter>();
   gnsstk::BasicTimeSystemConverter *btsc =
      dynamic_cast<gnsstk::BasicTimeSystemConverter *>(btscShared.get());
      // can't use TimeTag directly as it's abstract, so use CivilTime instead
   gnsstk::CivilTime uut, exp;

      //Check conversion from any given time system to UTC and back
   uut = gnsstk::CivilTime(1990,11,6,0,0,0,gnsstk::TimeSystem::UTC);
   exp = gnsstk::CivilTime(1990,11,6,0,0,6,gnsstk::TimeSystem::GPS);
   TUASSERTE(bool, true, uut.changeTimeSystem(gnsstk::TimeSystem::GPS,btsc));
   TUASSERTE(gnsstk::CommonTime, uut, exp);

   uut = gnsstk::CivilTime(2004,11,16,0,0,0,gnsstk::TimeSystem::GPS);
   exp = gnsstk::CivilTime(2004,11,15,23,59,47,gnsstk::TimeSystem::UTC);
   TUASSERTE(bool, true, uut.changeTimeSystem(gnsstk::TimeSystem::UTC,btsc));
   TUASSERTE(gnsstk::CommonTime, uut, exp);

   uut = gnsstk::CivilTime(1992,10,3,0,0,0,gnsstk::TimeSystem::UTC);
   exp = gnsstk::CivilTime(1992,10,3,0,0,0,gnsstk::TimeSystem::GLO);
   TUASSERTE(bool, true, uut.changeTimeSystem(gnsstk::TimeSystem::GLO,btsc));
   TUASSERTE(gnsstk::CommonTime, uut, exp);

   uut = gnsstk::CivilTime(1995,5,10,0,0,0,gnsstk::TimeSystem::GLO);
   exp = gnsstk::CivilTime(1995,5,10,0,0,0,gnsstk::TimeSystem::UTC);
   TUASSERTE(bool, true, uut.changeTimeSystem(gnsstk::TimeSystem::UTC,btsc));
   TUASSERTE(gnsstk::CommonTime, uut, exp);

   uut = gnsstk::CivilTime(1995,5,10,0,0,0,gnsstk::TimeSystem::GLO);
   exp = gnsstk::CivilTime(1995,5,10,0,0,0,gnsstk::TimeSystem::GLO);
   TUASSERTE(bool, true, uut.changeTimeSystem(gnsstk::TimeSystem::GLO,btsc));
   TUASSERTE(gnsstk::CommonTime, uut, exp);

   uut = gnsstk::CivilTime(2020,1,1,0,0,0,gnsstk::TimeSystem::GPS);
   exp = gnsstk::CivilTime(2019,12,31,23,59,42,gnsstk::TimeSystem::GLO);
   TUASSERTE(bool, true, uut.changeTimeSystem(gnsstk::TimeSystem::GLO,btsc));
   TUASSERTE(gnsstk::CommonTime, uut, exp);

      // conversion using static TimeSystemConverter
   gnsstk::CommonTime::tsConv = btscShared;
   uut = gnsstk::CivilTime(1990,11,6,0,0,0,gnsstk::TimeSystem::UTC);
   exp = gnsstk::CivilTime(1990,11,6,0,0,6,gnsstk::TimeSystem::GPS);
   TUASSERTE(bool, true, uut.changeTimeSystem(gnsstk::TimeSystem::GPS));
   TUASSERTE(gnsstk::CommonTime, uut, exp);

   uut = gnsstk::CivilTime(2004,11,16,0,0,0,gnsstk::TimeSystem::GPS);
   exp = gnsstk::CivilTime(2004,11,15,23,59,47,gnsstk::TimeSystem::UTC);
   TUASSERTE(bool, true, uut.changeTimeSystem(gnsstk::TimeSystem::UTC));
   TUASSERTE(gnsstk::CommonTime, uut, exp);

   uut = gnsstk::CivilTime(1992,10,3,0,0,0,gnsstk::TimeSystem::UTC);
   exp = gnsstk::CivilTime(1992,10,3,0,0,0,gnsstk::TimeSystem::GLO);
   TUASSERTE(bool, true, uut.changeTimeSystem(gnsstk::TimeSystem::GLO));
   TUASSERTE(gnsstk::CommonTime, uut, exp);

   uut = gnsstk::CivilTime(1995,5,10,0,0,0,gnsstk::TimeSystem::GLO);
   exp = gnsstk::CivilTime(1995,5,10,0,0,0,gnsstk::TimeSystem::UTC);
   TUASSERTE(bool, true, uut.changeTimeSystem(gnsstk::TimeSystem::UTC));
   TUASSERTE(gnsstk::CommonTime, uut, exp);

   uut = gnsstk::CivilTime(1995,5,10,0,0,0,gnsstk::TimeSystem::GLO);
   exp = gnsstk::CivilTime(1995,5,10,0,0,0,gnsstk::TimeSystem::GLO);
   TUASSERTE(bool, true, uut.changeTimeSystem(gnsstk::TimeSystem::GLO));
   TUASSERTE(gnsstk::CommonTime, uut, exp);

   uut = gnsstk::CivilTime(2020,1,1,0,0,0,gnsstk::TimeSystem::GPS);
   exp = gnsstk::CivilTime(2019,12,31,23,59,42,gnsstk::TimeSystem::GLO);
   TUASSERTE(bool, true, uut.changeTimeSystem(gnsstk::TimeSystem::GLO));
   TUASSERTE(gnsstk::CommonTime, uut, exp);

   TURETURN();
}


int main() //Main function to initialize and run all tests above
{
   unsigned errorTotal = 0;
   TimeTag_T testClass;

   errorTotal += testClass.scanfANSITime();
   errorTotal += testClass.scanfCivilTime();
   errorTotal += testClass.scanfGPSWeekSecond();
   errorTotal += testClass.scanfGPSWeekZcount();
   errorTotal += testClass.scanfJulianDate();
   errorTotal += testClass.scanfMJD();
   errorTotal += testClass.scanfUnixTime();
   errorTotal += testClass.scanfYDSTime();
   errorTotal += testClass.changeTimeSystemTest();

   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;

      //Return the total number of errors
   return errorTotal;
}
