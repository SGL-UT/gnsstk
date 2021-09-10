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
#include "GPSLNavAlm.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   GPSLNavAlm ::
   GPSLNavAlm()
         : healthBits(0xff),
           deltai(0.0),
           toa(0.0)
   {
      signal.messageType = NavMessageType::Almanac;
      msgLenSec = 6.0;
   }


   bool GPSLNavAlm ::
   validate() const
   {
      return GPSLNavData::validate();
   }


   void GPSLNavAlm ::
   fixFit()
   {
         // Set the fit times to toa-70h through toa+74 hours.  This
         // is an estimate based on IS-GPS-200 Table 20-XIII.
      beginFit = Toe - (70.0 * 3600.0);
      endFit   = Toe + (74.0 * 3600.0);
   }


   void GPSLNavAlm ::
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
