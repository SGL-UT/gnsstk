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
#include "SystemTime.hpp"
#include "ANSITime.hpp"
#include "CommonTime.hpp"
#include "UnixTime.hpp"
#include <iostream>
#include <ctime>

using namespace std;
using namespace gnsstk;

class SystemTime_T
{
public:
   SystemTime_T(){}// Default Constructor, set the precision value
   ~SystemTime_T() {} // Default Destructor
      /** SystemTime inherits from UnixTime. The only added ability is
       * to read the time from the machine.  This one test will read
       * the system time and compare it with the ctime value. However,
       * since this test will need to read the time twice no
       * guarantees can be made for strict tolerances.  In fact, the
       * tolerance will be set so that the time will be accurate to
       * the second only. */
   int obtainTimeFromSystemTest ()
   {
      TUDEF("SystemTime", "SystemTime()");

      unsigned failcount = 0, succcount = 0;
         // Run the test at most 5 times, or until we have more
         // successes than failures.  We do this because we sometimes
         // get failures because the seconds of day don't always
         // exactly match when the system time queries being compared
         // don't happen at the exact same time.
      while (((failcount + succcount) < 5) && (succcount <= failcount))
      {
         long day, day2;
         long sod, sod2;
         double fsod, fsod2;

         SystemTime timeFromSystemTime;
         time_t t;
         time(&t);
         ANSITime timeFromctime(t);

         CommonTime commonTimeSystemTime(timeFromSystemTime);
         CommonTime commonTimectime(timeFromctime);

         commonTimeSystemTime.get(day,sod,fsod);
         commonTimectime.get(day2,sod2,fsod2);
            // Was the time obtained properly?
         if (day != day2)
         {
            cerr << "day mismatch" << endl;
            failcount++;
         }
         else if (sod != sod2)
         {
            cerr << "second mismatch" << endl;
            failcount++;
         }
         else if (TimeSystem::UTC != commonTimeSystemTime.getTimeSystem())
         {
            cerr << "time system mismatch" << endl;
            failcount++;
         }
         else
         {
            succcount++;
         }
      }
      if (failcount >= succcount)
      {
         TUFAIL("too many mismatches");
      }
      TURETURN();
   }

};


int main()
{
   unsigned errorCounter = 0;
   SystemTime_T testClass;

   errorCounter += testClass.obtainTimeFromSystemTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorCounter
             << std::endl;

   return errorCounter;
}
