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
#include "BDSD1NavEph.hpp"
#include "BDSWeekSecond.hpp"
#include "TimeString.hpp"
#include "GPS_URA.hpp"
#include "BDSconsts.hpp"

using namespace std;

namespace gnsstk
{
   BDSD1NavEph ::
   BDSD1NavEph()
         : pre2(0),
           pre3(0),
           rev2(0),
           rev3(0),
           sow2(0),
           sow3(0),
           satH1(true),
           aodc(0xff),
           aode(0xff),
           uraIndex(15),
           xmit2(gnsstk::CommonTime::BEGINNING_OF_TIME),
           xmit3(gnsstk::CommonTime::BEGINNING_OF_TIME),
           tgd1(std::numeric_limits<double>::quiet_NaN()),
           tgd2(std::numeric_limits<double>::quiet_NaN())
   {
      signal.messageType = NavMessageType::Ephemeris;
   }


   bool BDSD1NavEph ::
   validate() const
   {
      return BDSD1NavData::validate() &&
         ((pre2 == 0) || (pre2 == bds::Preamble)) &&
         ((pre2 == 0) || (pre3 == bds::Preamble));
   }


   CommonTime BDSD1NavEph ::
   getUserTime() const
   {
      return std::max({xmitTime, xmit2, xmit3}) + 6.0;
   }


// The BeiDou navigation message does not specify the fit interval. 
// From empirical studies, we know that the fit interval for healthy BeiDou
// navigation message data appears extend at least 2 hours after the
// t-sub-oe, which is typically aligned with the beginning of transmit time. 
//
// Therefore for healthy data in the "operationally nominal" case in which 
// the messages have data set cutovers every hour on the hour, the end-of-fit
// can be somewhere around two hours after beginning of transmit or t-sub-oe.
//
// However, there are a couple of problems with unhealthy data sets. 
//    - In some cases, an unhealthy SV continues to broadcast a given data
//      set for a long period (greater than a day). In such a case, the 
//      fit interval "expires" and the message selection process returns 
//      "no data" where the actual answer is "SV is transmitting an unhealthy indication".
//    - In some cases, an unhealthy SV broadcasts "all-zeros" following the
//      unhealthy indication.  This means the t-sub-oe is zero (which can be
//      a valid value, but only near the beginning of the week).  This leads
//      to a end-of-first aligned with 7200 SOW of the current week, 
//      regardless of where in the week the data are actually broadcast. 
//      This means a selection based on fit interval windows will NEVER select
//      this message as the begin-of-fit is after the end-of-fit. The end 
//      result is that processes are "blind" to unhealthy indications and
//      continue to "coast" on the prior message until its end-of-fit is reached.  
//      This leads to misleading results in analysis; especially in the case in 
//      which a healthy SV transitions to an unhealthy state. 
//
// There are no "good" answers to this issue. After discussion, we are going to try the following:
//    - Set the end-of-fit to the "beginning of transmit" time + SEC_PER_DAY + 30.0.  
//   
// It is hoped that this will achieve the following goals:
//    - For healthy messages in the nominal pattern of data set cutovers on the hour, the 
//      "use most recently transmitted" find algorithm (user option) will still pick 
//      the most recently transmitted, even though the preceding data set may have a fit
//      interval 23 hours later. 
//    - For unhealthy messages with invalid t-sub-oe, the condition 
//          begin-of-fit < end-of-fit will still be true.
//      Therefore, these messages will once again be visible to the find algorithm. 
//    - For unhealthy messages that are broadcast for long periods, there two cases:
//       - For a process working in near-real-time, it should always be using the 
//         most recently collected data set.  There may need to be checks to verify 
//         that the end-of-fit has not artificially expired. 
//       - For post-processing users (the largest percentage of cases), there 
//         is an interaction with how our file stores work. Most of our file stores
//         are organized by day. Therefore, if a message is broadcast continuously
//         across a day boundary, there will be a fresh copy at the beginning of 
//         each daily file.  That's the rationale for using "SEC_PER_DAY + 30.0" as 
//         opposed to "END_OF_TIME".  No one was comfortable with a default 
//         END_OF_TIME for end-of-fit.  
//
//THIS IS ONLY BEING IMPLEMENTED FOR BeiDou (both D1 and D2).
//
   void BDSD1NavEph ::
   fixFit()
   {
      beginFit = Toe - 7200.0;  // Default case

         // If elements were updated during the hour, then
         // we want to use the later time.
      if (xmitTime > Toe)
         beginFit = xmitTime;

      endFit = xmitTime + SEC_PER_DAY + 30.0; 
   }


   void BDSD1NavEph ::
   dumpSVStatus(std::ostream& s) const
   {
      const ios::fmtflags oldFlags = s.flags();
      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');
         // Use these to print subframe SOW in a formatted fashion.
         // We're not printing the week so setting the week to 0 is
         // fine.
      BDSWeekSecond ws1(0,sow), ws2(0,sow2), ws3(0,sow3);
      s << "           SV STATUS" << endl << endl
        << "Health bit (SatH1)  :    0x" << hex << (unsigned)satH1 << dec
        << endl
        << "AODC                : " << setw(6) << getAOD(aodc) << " hours ("
        << (unsigned)aodc << ")" << endl
        << "AODE                : " << setw(6) << getAOD(aode) << " hours ("
        << (unsigned)aode << ")" << endl
        << "URA index           : " << setw(6) << (unsigned)uraIndex << endl
        << "URA (nominal)       : " << setw(6) << fixed
        << SV_ACCURACY_GPS_NOMINAL_INDEX[uraIndex] << " m" << endl
        << "Health              : " << setw(9)
        << gnsstk::StringUtils::asString(health) << endl
        << "Tgd1                : " << setw(13) << setprecision(6)
        << scientific << tgd1 << " sec" << endl
        << "Tgd2                : " << setw(13) << setprecision(6)
        << scientific << tgd2 << " sec" << endl << endl
        << "           SUBFRAME OVERHEAD" << endl << endl
        << "               SOW    DOW:HH:MM:SS" << endl
        << printTime(ws1, "SF1 SOW:    %6.0g  %3a-%w:%02H:%02M:%02S\n")
        << printTime(ws2, "SF2 SOW:    %6.0g  %3a-%w:%02H:%02M:%02S\n")
        << printTime(ws3, "SF3 SOW:    %6.0g  %3a-%w:%02H:%02M:%02S\n");
      s.flags(oldFlags);
   }

   unsigned BDSD1NavEph ::
   getAOD(uint8_t aod)
   {
      if (aod < 25)
         return aod;
      if (aod < 32)
         return 24 * (aod-23);
      return (unsigned)-1;
   }
}
