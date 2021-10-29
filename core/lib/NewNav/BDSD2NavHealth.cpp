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
#include "BDSD2NavHealth.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   BDSD2NavHealth ::
   BDSD2NavHealth()
         : isAlmHealth(true),
           satH1(true),
           svHealth(0xff)
   {
      weekFmt = "%4D(%4e)";
      msgLenSec = 6.0;
   }


   SVHealth BDSD2NavHealth ::
   getHealth() const
   {
      if (isAlmHealth)
      {
            /** @todo Determine if this is reasonable.  It appears as
             * though the reserved health bits can sometimes be
             * non-zero even if other explicitly defined bits are not,
             * and also different signals can be set unhealthy.  So if
             * we have health data that is tagged as B1I that says B3I
             * is unhealthy, do we indicate that we're unhealthy?
             * Compare that to GPS LNAV which defines health bits in a
             * similar fashion (different bits indicate health on
             * different signals), but it's not really used that way -
             * typically either all bits are set or cleared.  As such,
             * we need to clarify user expectations as much as
             * anything. */
         return (svHealth == 0) ? SVHealth::Healthy : SVHealth::Unhealthy;
      }
      else
      {
         return (satH1 ? SVHealth::Unhealthy : SVHealth::Healthy);
      }
   }

   void BDSD2NavHealth ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      const ios::fmtflags oldFlags = s.flags();

      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');
      switch (dl)
      {
         case DumpDetail::OneLine:
            NavData::dump(s,dl);
            break;
         case DumpDetail::Brief:
            NavData::dump(s,dl);
            s << "svHealth = " << hex << (unsigned)svHealth << dec << "  "
              << StringUtils::asString(getHealth()) << endl;
            break;
         case DumpDetail::Full:
               // "header"
            s << "*************************************************************"
              << "***************" << endl
              << "Satellite Health" << endl << endl
              << "PRN : " << setw(2) << signal.sat << " / "
              << "SVN : " << setw(2);
            std::string svn;
            if (getSVN(signal.sat, timeStamp, svn))
            {
               s << svn;
            }
            s << endl << endl
              << "           TIMES OF INTEREST"
              << endl << endl
              << "              " << getDumpTimeHdr(dl) << endl
              << "Transmit:     " << getDumpTime(dl, timeStamp) << endl
              << endl
              << "           HEALTH DATA" << endl
              << "Bits               0x" << hex << setw(3) << setfill('0')
              << svHealth << endl << endl
              << "Function           Status" << endl
              << "Satellite clock    ";
            if ((svHealth & SatClock) == SatClock)
            {
               if ((svHealth ^ SatClock) == 0)
               {
                  s << "unavailable";
               }
               else if ((svHealth & 0xff) == 0xff)
               {
                  s << "failed";
               }
               else
               {
                  s << "reserved";
               }
            }
            else
            {
               s << "normal";
            }
            s << endl
              << "B1I signal         "
              << (svHealth & B1ISignalWeak ? "weak" : "normal") << endl
              << "B2I signal         "
              << (svHealth & B2ISignalWeak ? "weak" : "normal") << endl
              << "B3I signal         "
              << (svHealth & B3ISignalWeak ? "weak" : "normal") << endl
              << "Reserved (5)       "
              << (svHealth & 0x10 ? "set" : "unset") << endl
              << "Reserved (4)       "
              << (svHealth & 0x08 ? "set" : "unset") << endl
              << "Reserved (3)       "
              << (svHealth & 0x04 ? "set" : "unset") << endl
              << "Navigation message "
              << (svHealth & NavMsgBad ? "invalid" : "valid") << endl
              << "Reserved (1)       "
              << (svHealth & 0x01 ? "set" : "unset") << endl;
            break;
      }
      s.flags(oldFlags);
   }
}
