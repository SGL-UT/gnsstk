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
#include "GPSCNavHealth.hpp"

using namespace std;

namespace gnsstk
{
   CommonTime GPSCNavHealth ::
   getUserTime() const
   {
      if (signal.nav == NavType::GPSCNAVL2)
         return timeStamp + 12.0;
      return timeStamp + 6.0;
   }


   void GPSCNavHealth ::
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
            s << "health = " << hex << (unsigned)health << dec << "  "
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
              << "Bits               0x" << hex << setw(1) << setfill('0')
              << (unsigned)health << endl
              << "Status             " << StringUtils::asString(getHealth())
              << endl;
            break;
      }
      s.flags(oldFlags);
   }
}
