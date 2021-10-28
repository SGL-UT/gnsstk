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
#include "BDSD2NavAlm.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   BDSD2NavAlm ::
   BDSD2NavAlm()
         : pnum(0xff),
           deltai(0.0),
           toa(0.0),
           healthBits(0xffff),
           isDefault(false)
   {
      signal.messageType = NavMessageType::Almanac;
      msgLenSec = 6.0;
   }


   bool BDSD2NavAlm ::
   validate() const
   {
      return !isDefault && BDSD2NavData::validate();
   }


   void BDSD2NavAlm ::
   fixFit()
   {
      beginFit = xmitTime;
      endFit   = CommonTime::END_OF_TIME;
      endFit.setTimeSystem(TimeSystem::BDT);
   }


   void BDSD2NavAlm ::
   dumpSVStatus(std::ostream& s) const
   {
      const ios::fmtflags oldFlags = s.flags();
      s.setf(ios::scientific, ios::floatfield);
      s << "           PAGE OVERHEAD" << endl
        << endl
        << "                SOW    DOW:HH:MM:SS" << endl
        << printTime(xmitTime,"Transmit   : %6.0g  %3a-%1w:%02H:%02M:%02S")
        << endl
        << endl
        << "           SV STATUS" << endl
        << endl
        << "Health              : " << setw(9)
        << static_cast<unsigned>(healthBits)
        << " (" << gnsstk::StringUtils::asString(health) << ")" << endl;
      s.flags(oldFlags);
   }
}
