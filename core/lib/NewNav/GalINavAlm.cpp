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
#include "GalINavAlm.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   const double GalINavAlm::refA = 29600000;
   const double GalINavAlm::refioffset = (56.0 * PI / 180.0);

   GalINavAlm ::
   GalINavAlm()
         : dAhalf(0.0),
           deltai(0.0),
           wna(0),
           t0a(0.0),
           ioda1(0),
           ioda2(0),
           hsE5b(GalHealthStatus::Unknown),
           hsE1B(GalHealthStatus::Unknown)
   {
      signal.messageType = NavMessageType::Almanac;
      weekFmt = "%4L(%4l)";
         // Not recommended for use - getUserTime() should be used
         // instead as there is no guarantee that the two word types
         // making up this almanac are consecutive.
      msgLenSec = 2.0;
   }


   bool GalINavAlm ::
   validate() const
   {
      return true;
   }


   CommonTime GalINavAlm ::
   getUserTime() const
   {
         // Galileo I/NAV nominal page transmit time is 2 seconds per
         // OS-SIS-ICD figure 15.
      return std::max(xmitTime, xmit2) + 2.0;
   }


   void GalINavAlm ::
   fixFit()
   {
         /// @todo set the fit interval correctly
         // Set the fit times to transmit time through toa+74 hours.
      beginFit = xmitTime;
      endFit   = Toe + (74.0 * 3600.0);
   }


   void GalINavAlm ::
   fixHealth()
   {
         /** @todo determine if this educated guess about health
          * status is correct or at least reasonable. */
      if ((hsE5b == GalHealthStatus::OK) && (hsE1B == GalHealthStatus::OK))
      {
         health = SVHealth::Healthy;
      }
      else if ((hsE5b == GalHealthStatus::OutOfService) &&
               (hsE1B == GalHealthStatus::OutOfService))
      {
         health = SVHealth::Unhealthy;
      }
      else
      {
         health = SVHealth::Degraded;
      }
   }


   void GalINavAlm ::
   dumpSVStatus(std::ostream& s) const
   {
      const ios::fmtflags oldFlags = s.flags();
      s.setf(ios::scientific, ios::floatfield);
      s << "           PAGE OVERHEAD" << endl
        << endl
        << "                SOW    DOW:HH:MM:SS         IODa" << endl
        << printTime(xmitTime,"Part 1 TOW : %6.0g  %3a-%1w:%02H:%02M:%02S")
        << setw(13) << (unsigned)ioda1 << endl
        << printTime(xmit2,"Part 2 TOW : %6.0g  %3a-%1w:%02H:%02M:%02S")
        << setw(13) << (unsigned)ioda2 << endl
        << endl
        << "           SV STATUS" << endl
        << endl
        << "E5b_HS              : " << setw(9) << static_cast<int>(hsE5b)
        << " (" << gnsstk::StringUtils::asString(hsE5b) << ")" << endl
        << "E1B_HS              : " << setw(9) << static_cast<int>(hsE1B)
        << " (" << gnsstk::StringUtils::asString(hsE1B) << ")" << endl;
      s.flags(oldFlags);
   }
}
