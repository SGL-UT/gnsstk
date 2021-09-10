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
#include "GalFNavAlm.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   const double GalFNavAlm::refA = 29600000;
   const double GalFNavAlm::refioffset = (56.0 * PI / 180.0);

   GalFNavAlm ::
   GalFNavAlm()
         : dAhalf(0.0),
           deltai(0.0),
           wna(0),
           t0a(0.0),
           ioda5(0),
           ioda6(0),
           hsE5a(GalHealthStatus::Unknown)
   {
      signal.messageType = NavMessageType::Almanac;
      weekFmt = "%4L(%4l)";
         // Galileo F/NAV nominal page transmit time is 10 seconds per
         // OS-SIS-ICD figure 14.
      msgLenSec = 10.0;
   }


   bool GalFNavAlm ::
   validate() const
   {
      return true;
   }


   CommonTime GalFNavAlm ::
   getUserTime() const
   {
      return std::max(xmitTime, xmit2) + msgLenSec;
   }


   void GalFNavAlm ::
   fixFit()
   {
         /// @todo set the fit interval correctly
         // Set the fit times to transmit time through toa+74 hours.
      beginFit = xmitTime;
      endFit   = Toe + (74.0 * 3600.0);
   }


   void GalFNavAlm ::
   fixHealth()
   {
      switch (hsE5a)
      {
         case GalHealthStatus::OK:
            health = SVHealth::Healthy;
            break;
         case GalHealthStatus::OutOfService:
         case GalHealthStatus::InTest:
            health = SVHealth::Unhealthy;
            break;
         case GalHealthStatus::WillBeOOS:
            health = SVHealth::Degraded;
            break;
         default:
            health = SVHealth::Unknown;
            break;
      }
   }


   void GalFNavAlm ::
   dumpSVStatus(std::ostream& s) const
   {
      const ios::fmtflags oldFlags = s.flags();
      s.setf(ios::scientific, ios::floatfield);
      s << "           PAGE OVERHEAD" << endl
        << endl
        << "                SOW    DOW:HH:MM:SS      IODa" << endl
        << printTime(xmitTime,"Part 1 TOW : %6.0g  %3a-%1w:%02H:%02M:%02S")
        << setw(13) << (unsigned)ioda5 << endl
        << printTime(xmit2,"Part 2 TOW : %6.0g  %3a-%1w:%02H:%02M:%02S")
        << setw(13) << (unsigned)ioda6 << endl
        << endl
        << "           SV STATUS" << endl
        << endl
        << "E5a_HS              : " << setw(9) << static_cast<int>(hsE5a)
        << " (" << gnsstk::StringUtils::asString(hsE5a) << ")" << endl;
      s.flags(oldFlags);
   }
}
