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

#include "TimeSystem.hpp"
#include "TestUtil.hpp"
#include <iostream>
#include <sstream>
#include <cmath>

class TimeSystem_T
{
public:
   unsigned operatorTest();
   unsigned getLeapSecondsTest();
   unsigned getLeapSecondsTestZeroDay();
   unsigned getLeapSecondsTestPre1960s();
   unsigned correctionTest();
   unsigned correctionTestInvalidTimeSystem();
   unsigned asStringTest();
};

unsigned TimeSystem_T::operatorTest()
{
   TUDEF("TimeSystem", "operator<<");
   std::string testMesg;
   gnsstk::TimeSystem compare = gnsstk::TimeSystem::GAL;
   
   //Does operator<< function correctly?
   std::string outputString, compareString;
   std::stringstream outputStream;
   outputStream << compare;
   outputString = outputStream.str();
   compareString = "GAL";
   
   TUASSERTE(std::string, compareString, outputString);
   TURETURN();
}

unsigned TimeSystem_T::getLeapSecondsTest()
{
   TUDEF("TimeSystem", "getLeapSeconds");
   
   //Check leap-second computation prior to 1972 but not before 1960
   //Should be (inputDate - lastRefDate) * rate + leapSeconds
   //4.213170+488*0.002592
   TUASSERTFE(5.478066, gnsstk::getLeapSeconds(1969, 6, 3));
   
   //1.8458580 + 118*0.0011232
   TUASSERTFE(1.9783956, gnsstk::getLeapSeconds(1962, 4,  29));
   
   //3.6401300 + 5*.001296
   TUASSERTFE(3.64661, gnsstk::getLeapSeconds(1965, 3,  6));
      
   //4.2131700 + 295*.002592
   TUASSERTFE(4.97781, gnsstk::getLeapSeconds(1968, 11,  22));
   
   //4.3131700 + 409*.002592
   TUASSERTFE(5.373298, gnsstk::getLeapSeconds(1967, 2,  14));
   
   //Check leap-second computation after 1/1/1972
   TUASSERTFE(29, gnsstk::getLeapSeconds(1995, 10, 13));
   TUASSERTFE(32, gnsstk::getLeapSeconds(2004, 3, 25));
   TUASSERTFE(22, gnsstk::getLeapSeconds(1984, 8, 27));
   TUASSERTFE(10, gnsstk::getLeapSeconds(1972, 5, 8));
   
   TURETURN();
}

unsigned TimeSystem_T::getLeapSecondsTestZeroDay()
{
   TUDEF("TimeSystem", "getLeapSecondsTestZeroDay");

   TUASSERTFE(7.6838579999999998549, gnsstk::getLeapSeconds(1971, 10, 0));

   TURETURN();
}

unsigned TimeSystem_T::getLeapSecondsTestPre1960s()
{
   TUDEF("TimeSystem", "getLeapSecondsTestPre1960s");

   TUASSERTFE(0.0, gnsstk::getLeapSeconds(1955, 8, 21));

   TURETURN();
}

unsigned TimeSystem_T::correctionTest()
{
   TUDEF("TimeSystem", "getTimeSystemCorrection");
   //Check conversion from any given time system to UTC and back
   TUASSERTFE(6, gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::GPS, 1990, 11, 6));
   TUASSERTFE(-13, gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC, 2004, 11, 16));
   TUASSERTFE(10800, gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::GLO, 1992, 10, 3));
   TUASSERTFE(-10800, gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::GLO, gnsstk::TimeSystem::UTC, 1995, 5, 10));
   TUASSERTFE(12, gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::GAL, 1997, 7, 25));
   TUASSERTFE(-14, gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::GAL, gnsstk::TimeSystem::UTC, 2008, 6, 5));
   
   // QZSS can't be converted
   //testMesg = "Conversion from UTC time to QZS time was incorrect";
   //testFramework.assert(std::abs(gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::QZS, 1985, 8, 10) - 4) < eps, testMesg, __LINE__);
   //testMesg = "Conversion from QZS time to UTC time was incorrect";
   //testFramework.assert(std::abs(gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::QZS, gnsstk::TimeSystem::UTC, 2010, 2, 14) - 15) < eps, testMesg, __LINE__);
   TUASSERTFE(0, gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::BDT, 2006, 9, 21));
   TUASSERTFE(-2, gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::BDT, gnsstk::TimeSystem::UTC, 2012, 8, 27));
   TUASSERTFE(13, gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::IRN, 2004, 11, 16));
   TUASSERTFE(-13, gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::IRN, gnsstk::TimeSystem::UTC, 2004, 11, 16));
   TUASSERTFE(35, gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::TAI, 2014, 6, 1));
   TUASSERTFE(-35, gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::TAI, gnsstk::TimeSystem::UTC, 2015, 1, 1));
   TUASSERTFE(13 + 51.184, gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::UTC,  gnsstk::TimeSystem::TT, 2005, 4, 31));
   TUASSERTFE(-(6 + 51.184), gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::TT, gnsstk::TimeSystem::UTC, 1990, 7, 21));
   //reference section B of astronomical almanac for TDB conversion
   
   TUASSERTFE(65.1840299405112091335467994213104248046875,
              gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::UTC,  gnsstk::TimeSystem::TDB, 2007, 12, 25));
   TUASSERTFE(-58.1838658094272460630236309953033924102783203125,
              gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::TDB, gnsstk::TimeSystem::UTC, 1991, 4, 25));
   
   TURETURN();
}

unsigned TimeSystem_T::correctionTestInvalidTimeSystem()
{
   TUDEF("TimeSystem", "correctionTestInvalidTimeSystem");

   // Invalid In TimeSystem
   TUTHROW(gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::Last, gnsstk::TimeSystem::UTC, 1991, 4, 25));

   // Invalid Out TimeSystem
   TUTHROW(gnsstk::getTimeSystemCorrection(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::Last, 1991, 4, 25));

   TURETURN();
}

unsigned TimeSystem_T::asStringTest()
{
   TUDEF("TimeSystem", "asStringTest");

   TUASSERTE(std::string, "UNK", gnsstk::StringUtils::asString(gnsstk::TimeSystem::Unknown));
   TUASSERTE(std::string, "Any", gnsstk::StringUtils::asString(gnsstk::TimeSystem::Any));
   TUASSERTE(std::string, "GPS", gnsstk::StringUtils::asString(gnsstk::TimeSystem::GPS));
   TUASSERTE(std::string, "GLO", gnsstk::StringUtils::asString(gnsstk::TimeSystem::GLO));
   TUASSERTE(std::string, "GAL", gnsstk::StringUtils::asString(gnsstk::TimeSystem::GAL));
   TUASSERTE(std::string, "QZS", gnsstk::StringUtils::asString(gnsstk::TimeSystem::QZS));
   TUASSERTE(std::string, "BDT", gnsstk::StringUtils::asString(gnsstk::TimeSystem::BDT));
   TUASSERTE(std::string, "IRN", gnsstk::StringUtils::asString(gnsstk::TimeSystem::IRN));
   TUASSERTE(std::string, "UTC", gnsstk::StringUtils::asString(gnsstk::TimeSystem::UTC));
   TUASSERTE(std::string, "TAI", gnsstk::StringUtils::asString(gnsstk::TimeSystem::TAI));
   TUASSERTE(std::string, "TT", gnsstk::StringUtils::asString(gnsstk::TimeSystem::TT));
   TUASSERTE(std::string, "TDB", gnsstk::StringUtils::asString(gnsstk::TimeSystem::TDB));
   TUASSERTE(std::string, "???", gnsstk::StringUtils::asString(gnsstk::TimeSystem::Last));

   TURETURN();
}


int main() //Main function to initialize and run all tests above
{
   TimeSystem_T testClass;
   unsigned errorCounter = 0;

   errorCounter += testClass.operatorTest();
   errorCounter += testClass.getLeapSecondsTest();
   errorCounter += testClass.getLeapSecondsTestZeroDay();
   errorCounter += testClass.getLeapSecondsTestPre1960s();
   errorCounter += testClass.correctionTest();
   errorCounter += testClass.correctionTestInvalidTimeSystem();
   errorCounter += testClass.asStringTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorCounter
             << std::endl;

   return errorCounter; //Return the total number of errors
}
