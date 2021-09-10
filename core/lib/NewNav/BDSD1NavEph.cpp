//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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


   void BDSD1NavEph ::
   fixFit()
   {
      beginFit = Toe - 7200.0;  // Default case
         // If elements were updated during the hour, then
         // we want to use the later time.
         /// @todo Determine if this is an appropriate adjustment
      if (xmitTime > Toe)
         beginFit = xmitTime;
      endFit = Toe + 7200;
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
        << "Health bit (SatH1)  :      0x" << hex << (unsigned)satH1 << dec
        << endl
        << "AODC                : " << setw(6) << getAOD(aodc) << " hours ("
        << (unsigned)aodc << ")" << endl
        << "AODE                : " << setw(6) << getAOD(aode) << " hours ("
        << (unsigned)aode << ")" << endl
        << "URA index           : " << setw(9) << (unsigned)uraIndex << endl
            //                     1234567890123
        << "URA (nominal)       : " << setw(6) << fixed
        << SV_ACCURACY_GPS_NOMINAL_INDEX[uraIndex] << " m" << endl
        << endl
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
