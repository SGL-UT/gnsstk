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
#include "GPSLNavEph.hpp"
#include "GPSWeekSecond.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   GPSLNavEph ::
   GPSLNavEph()
         : pre2(0),
           pre3(0),
           tlm2(0),
           tlm3(0),
           iodc(0),
           iode(0),
           fitIntFlag(0),
           healthBits(0xff),
           uraIndex(0),
           tgd(0.0),
           asFlag2(false),
           asFlag3(false),
           alert2(false),
           alert3(false),
           codesL2(L2Codes::Invalid1),
           L2Pdata(false)
   {
      signal.messageType = NavMessageType::Ephemeris;
      msgLenSec = 18.0;
   }


   bool GPSLNavEph ::
   validate() const
   {
      return GPSLNavData::validate() && ((pre2 == 0) || (pre2 == 0x8b)) &&
         ((pre3 == 0) || (pre3 == 0x8b));
   }


   void GPSLNavEph ::
   fixFit()
   {
      GPSWeekSecond xws(xmitTime), toeWS(Toe);
      int xmitWeek = xws.week;
      long xmitSOW = (long) xws.sow;
      bool isNominalToe = (long)toeWS.sow % 7200 == 0;
      double fitSeconds = 3600.0 * getLegacyFitInterval(iodc, fitIntFlag);
      endFit = Toe + (fitSeconds/2.0);

         // If the toe is NOT offset, then the begin valid time can be set
         // to the beginning of the two hour interval.
         // NOTE: This is only true for GPS.   We can't do this
         // for QZSS, even though it also broadcasts the LNAV message format.
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
         // Calculate the SOW aligned with the mid point and then
         // calculate the number of seconds the toe is SHORT
         // of that value.   That's how far the endValid needs
         // to be adjusted.
      if (!isNominalToe)
      {
         long sow = (long) toeWS.sow;
         long num900secIntervals = sow / 900;
         long midPointSOW = (num900secIntervals+1) * 900;
         double adjustUp = (double) (midPointSOW - sow);
         endFit += adjustUp;
      }
   }


   void GPSLNavEph ::
   dumpSVStatus(std::ostream& s) const
   {
      const ios::fmtflags oldFlags = s.flags();
         /** @note we add 6 seconds to xmitTime because we're printing
          * the HOW time not the transmit time.  Also we're really
          * just guessing about the SF2 and SF3 HOW times. */
      unsigned health = healthBits;
      s << "           SUBFRAME OVERHEAD" << endl << endl
        << "               SOW    DOW:HH:MM:SS     IOD    ALERT   A-S" << endl
        << "SF1 HOW:   "
        << gnsstk::printTime(xmitTime+6, "%7.0g  %3a-%1w:%02H:%02M:%02S")
        << "   0x" << hex << setw(3) << internal << setfill('0')
        << nouppercase << iodc << dec << setfill(' ') << "      "
        << noboolalpha << alert << "     " << (asFlag ? " on" : "off") << endl
        << "SF2 HOW:   "
        << gnsstk::printTime(xmitTime+12, "%7.0g  %3a-%1w:%02H:%02M:%02S")
        << "    0x" << hex << setw(2) << internal << setfill('0')
        << nouppercase << iode << dec << setfill(' ') << "      "
        << alert2 << "     " << (asFlag2 ? " on" : "off") << endl
        << "SF3 HOW:   "
        << gnsstk::printTime(xmitTime+18, "%7.0g  %3a-%1w:%02H:%02M:%02S")
        << "    0x" << hex << setw(2) << internal << setfill('0')
        << nouppercase << iode << dec << setfill(' ') << "      "
        << alert3 << "     " << (asFlag3 ? " on" : "off") << endl
        << endl
        << "           SV STATUS" << endl << endl
        << "Health bits         :      0x" << setw(2)
        << internal << setfill('0') << hex << health << dec
        << ", " << setfill(' ') << health << endl
        << "Fit interval flag   :" << setw(10) << (unsigned)fitIntFlag << endl
        << "URA index           :" << setw(10) << (unsigned)uraIndex << endl
        << "Code on L2          :" << setw(10) << StringUtils::asString(codesL2)
        << endl
        << "L2 P Nav data       :" << "       " << (L2Pdata ? "off" : " on")
        << endl << right
        << "Tgd                 :" << setw(14) << setprecision(6) << scientific
        << tgd << " sec" << endl;
      s.flags(oldFlags);
   }


   namespace StringUtils
   {
      std::string asString(GPSLNavEph::L2Codes e)
      {
         switch (e)
         {
            case GPSLNavEph::L2Codes::Invalid1: return "Invalid1";
            case GPSLNavEph::L2Codes::Pcode:    return "P only";
            case GPSLNavEph::L2Codes::CAcode:   return "C/A only";
            case GPSLNavEph::L2Codes::Invalid2: return "Invalid2";
            default:                            return "Unknown";
         }
      }
   }
}
