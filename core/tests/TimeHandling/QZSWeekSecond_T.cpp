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
#include "QZSWeekSecond.hpp"

using namespace std;
using namespace gnsstk;

class QZSWeekSecond_T
{
public:
   QZSWeekSecond_T(){}
   ~QZSWeekSecond_T() {} 

   unsigned printTest ()
   {
      TUDEF("QZSWeekSecond", "Print");
      QZSWeekSecond test(3, 200);

      //----------------------------------------
      // getPrintChars
      //----------------------------------------
      TUASSERTE(string, "VhiwgP", test.getPrintChars());

      //----------------------------------------
      // getDefaultFormat
      //----------------------------------------
      TUASSERTE(string, "%h %g %P", test.getDefaultFormat());

      //----------------------------------------
      // printError
      //----------------------------------------
      TUASSERTE(string, "", test.printError(""));

      TURETURN();
   }

   unsigned setObjectTest () 
   {
      TUDEF("QZSWeekSecond", "Set");

      //----------------------------------------
      // setFromInfo
      //----------------------------------------
      QZSWeekSecond test(3, 200);

      // Values set from constructor
      TUASSERTE(unsigned int, 3, test.getWeek());
      TUASSERTFE(200, test.getSOW());
      TUASSERTE(TimeSystem, TimeSystem::QZS, test.getTimeSystem());
      test.reset();
      
      TimeTag::IdToValue info;

      // Set week using weeks, set SOW using seconds
      info['h'] = "50"; // weeks
      info['g'] = "5000"; // seconds
      test.setFromInfo(info);
      TUASSERTE(unsigned int, 50, test.getWeek());
      TUASSERTFE(5000, test.getSOW());
      info.erase('h');
      info.erase('g');
      test.reset();

      // Set week using weeks, set SOW using days
      info['h'] = "300"; // weeks
      info['w'] = "4"; // days
      test.setFromInfo(info);
      TUASSERTE(unsigned int, 300, test.getWeek());
      TUASSERTFE(4 * SEC_PER_DAY, test.getSOW());
      info.erase('h');
      info.erase('w');
      test.reset();

      // Set week using epochs, set SOW using seconds
      info['V'] = "1"; // epoch
      info['g'] = "2000"; // seconds
      test.setFromInfo(info); 
      TUASSERTE(unsigned int, 65536, test.getWeek());
      TUASSERTFE(2000, test.getSOW());
      info.erase('V');
      info.erase('g');
      test.reset();

      // Set week with epochs and MOD week
      info['V'] = "3"; // epoch
      info['i'] = "30"; // mod week
      test.setFromInfo(info);
      TUASSERTE(unsigned int, 3, test.getEpoch());
      TUASSERTE(unsigned int, 196638, test.getWeek()); // 3 * 65536 + 30
      TUASSERTE(unsigned int, 30, test.getModWeek());
      info.erase('V');
      info.erase('i');
      test.reset();

      // Set TimeSystem
      info['P'] = "GPS"; // time system
      info['F'] = "0"; // value to be ignored, testing default in setFromInfo's switch logic
      test.setFromInfo(info);
      TUASSERTE(TimeSystem, TimeSystem::GPS, test.getTimeSystem());
      
      TURETURN();
   }

};


int main()
{
   unsigned errorCounter = 0;
   QZSWeekSecond_T testClass;

   errorCounter += testClass.printTest();
   errorCounter += testClass.setObjectTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorCounter
             << std::endl;

   return errorCounter;
}
