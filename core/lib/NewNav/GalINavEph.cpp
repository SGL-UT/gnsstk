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
#include "GalINavEph.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   GalINavEph ::
   GalINavEph()
         : bgdE5aE1(0.0),
           bgdE5bE1(0.0),
           sisaIndex(255),
           svid(0),
           iodnav1(0),
           iodnav2(0),
           iodnav3(0),
           iodnav4(0),
           hsE5b(GalHealthStatus::Unknown),
           hsE1B(GalHealthStatus::Unknown),
           dvsE5b(GalDataValid::Unknown),
           dvsE1B(GalDataValid::Unknown)
   {
      signal.messageType = NavMessageType::Ephemeris;
      weekFmt = "%4L(%4l)";
         // Not recommended for use - getUserTime() should be used
         // instead as there is no guarantee that the two word types
         // making up this ephemeris are consecutive.
      msgLenSec = 2.0;
      frame = RefFrameSys::ITRF;
   }


   bool GalINavEph ::
   validate() const
   {
      return true;
   }


   CommonTime GalINavEph ::
   getUserTime() const
   {
      const gnsstk::CommonTime unset;
      gnsstk::CommonTime rv = xmitTime;
         // If any xmit time is unset, make a best guess at what it
         // should be by adding 2 seconds (the transmit time of an
         // I/NAV page) to the return value.
         // This is mostly to handle formats like RINEX where we don't
         // have the transmit time of each of the pages.
      if (xmit2 != unset)
         rv = std::max(rv, xmit2);
      else
         rv += 2.0;
      if (xmit3 != unset)
         rv = std::max(rv, xmit3);
      else
         rv += 2.0;
      if (xmit4 != unset)
         rv = std::max(rv, xmit4);
      else
         rv += 2.0;
      if (xmit5 != unset)
         rv = std::max(rv, xmit5);
      else
         rv += 2.0;
         // Galileo I/NAV nominal page transmit time is 2 seconds per
         // OS-SIS-ICD figure 15.
      return rv + 2.0;
   }


   void GalINavEph ::
   fixFit()
   {
      beginFit = xmitTime;
      endFit = Toe + (3600*4); // four hours
   }


   void GalINavEph ::
   dumpSVStatus(std::ostream& s) const
   {
      const ios::fmtflags oldFlags = s.flags();
      s.setf(ios::scientific, ios::floatfield);
      const std::string poFmt("TOW :  " + weekFmt +
                              "  %6.0g  %3a-%1w:%02H:%02M:%02S");
      s << "           PAGE OVERHEAD" << endl
        << endl
        << "              Week(10bt)     SOW    DOW:HH:MM:SS      IOD_NAV"
        << endl
        << printTime(xmitTime,"Word 1 "+poFmt) << setw(13) << iodnav1 << endl
        << printTime(xmit2,"Word 2 "+poFmt) << setw(13) << iodnav2 << endl
        << printTime(xmit3,"Word 3 "+poFmt) << setw(13) << iodnav3 << endl
        << printTime(xmit4,"Word 4 "+poFmt) << setw(13) << iodnav4 << endl
        << printTime(xmit5,"Word 5 "+poFmt)
        << endl << endl
        << "           SV STATUS" << endl
        << endl
        << "E5b_DVS             : " << setw(9) << static_cast<int>(dvsE5b)
        << " (" << gnsstk::StringUtils::asString(dvsE5b) << ")" << endl
        << "E5b_HS              : " << setw(9) << static_cast<int>(hsE5b)
        << " (" << gnsstk::StringUtils::asString(hsE5b) << ")" << endl
        << "E1B_DVS             : " << setw(9) << static_cast<int>(dvsE1B)
        << " (" << gnsstk::StringUtils::asString(dvsE1B) << ")" << endl
        << "E1B_HS              : " << setw(9) << static_cast<int>(hsE1B)
        << " (" << gnsstk::StringUtils::asString(hsE1B) << ")" << endl
        << "SISA                : " << setw(9) << (unsigned)sisaIndex << endl
        << "Health              : " << setw(9)
        << gnsstk::StringUtils::asString(health) << endl
        << setprecision(6)
        << "Bgd(E1-E5a)         : " << setw(13) << bgdE5aE1 << " sec" << endl
        << "Bgd(E1-E5b)         : " << setw(13) << bgdE5bE1 << " sec" << endl;
      s.flags(oldFlags);
   }
}
