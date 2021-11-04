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
#include "GPSCNavEph.hpp"
#include "GPSWeekSecond.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   GPSCNavEph ::
   GPSCNavEph()
         : pre11(0),
           preClk(0),
           healthL1(true),
           healthL2(true),
           healthL5(true),
           uraED(-16),
           uraNED0(-16),
           uraNED1(0),
           uraNED2(0),
           alert11(false),
           alertClk(false),
           integStat(false),
           phasingL2C(false),
           deltaA(0.0),
           dOMEGAdot(0.0),
           top(gnsstk::CommonTime::BEGINNING_OF_TIME),
           xmit11(gnsstk::CommonTime::BEGINNING_OF_TIME),
           xmitClk(gnsstk::CommonTime::BEGINNING_OF_TIME)
   {
      signal.messageType = NavMessageType::Ephemeris;
   }


   bool GPSCNavEph ::
   validate() const
   {
      return GPSCNavData::validate() && ((pre11 == 0) || (pre11 == 0x8b)) &&
         ((preClk == 0) || (preClk == 0x8b));
   }


   CommonTime GPSCNavEph ::
   getUserTime() const
   {
      CommonTime mr = std::max({xmitTime, xmit11, xmitClk});
      if (signal.nav == NavType::GPSCNAVL2)
         return mr + 12.0;
      return mr + 6.0;
   }


   void GPSCNavEph ::
   fixFit()
   {
      CommonTime xmit1st = std::min({xmitTime, xmit11, xmitClk});
      GPSWeekSecond xws(xmit1st), toeWS(Toe);
      int xmitWeek = xws.week;
      long xmitSOW = (long) xws.sow;
         /** @todo replace all these magic numbers with named
          * constants or enums, with sensible names, not
          * "NINTY_MINUTES" [sic] */
      bool isNominalToe = (long)toeWS.sow % 7200 == 90*60;
      endFit = Toe + 90*60;

         // If the toe is NOT offset, then the begin valid time can be set
         // to the beginning of the two hour interval.
      if (signal.system==SatelliteSystem::GPS && isNominalToe)
      {
         xmitSOW = xmitSOW - (xmitSOW % 7200);
      }

         // If there IS an offset, all we can assume is that we (hopefully)
         // captured the earliest transmission and set the begin valid time
         // to that value.
         //
         // @note Prior to GPS III, the offset was typically applied
         // to BOTH the first and second data sets following a
         // cutover.  So this means the SECOND data set will NOT be
         // coerced to the top of the even hour start time if it
         // wasn't collected at the top of the hour.
      beginFit = GPSWeekSecond(xmitWeek, xmitSOW, xws.getTimeSystem());
         // If an upload cutover, need some adjustment.
      if (!isNominalToe)
      {
         endFit += 300;
      }
   }


   void GPSCNavEph ::
   dumpSVStatus(std::ostream& s) const
   {
      const ios::fmtflags oldFlags = s.flags();
      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');
      s << "           ACCURACY PARAMETERS"
        << endl
        << endl
        << "ED accuracy index              :  " << setfill(' ')
        << dec << setw(4) << (int) uraED << endl
        << "NED accuracy indices  0, 1, 2  :  " << setfill(' ')
        << dec << setw(4) << (int) uraNED0 << ", "
        << dec << setw(4) << (unsigned) uraNED1 << ", "
        << dec << setw(4) << (unsigned) uraNED2 << endl
        << "Integrity Status Flag          : "
        << (integStat ? "1 (Enhanced)" : "0 (Legacy)")
        << endl << endl << endl
        << "              " << getDumpTimeHdr(DumpDetail::Full) << endl
        << "Predict    :  " << getDumpTime(DumpDetail::Full, top) << endl
        << endl
        << "           SV STATUS"
        << endl
        << endl
        << "Health bits  L1, L2, L5        :     " << setfill('0') << setw(1)
        << healthL1 << ",  " << healthL2 << ",  " << healthL5
        << endl
        << "L2C Phasing                    :     " << setfill(' ')
        << phasingL2C << " (0=quadrature, 1=in-phase)"
        << endl << endl << endl
        << "           TRANSMIT TIMES" << endl << endl
        << "              " << getDumpTimeHdr(DumpDetail::Full) << endl
        << "Message 10:   " << getDumpTime(DumpDetail::Full, xmitTime) << endl
        << "Message 11:   " << getDumpTime(DumpDetail::Full, xmit11) << endl
        << "Clock:        " << getDumpTime(DumpDetail::Full, xmitClk) << endl;
      s.flags(oldFlags);
   }
}
