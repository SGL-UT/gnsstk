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
#include "GLOFNavHealth.hpp"

using namespace std;

namespace gnsstk
{
   GLOFNavHealth ::
   GLOFNavHealth()
   {
         // Allow the data fields to default to invalid via ValidType.
      msgLenSec = 2.0;
   }


   bool GLOFNavHealth ::
   validate() const
   {
      return (healthBits.is_valid() || ln.is_valid() || Cn.is_valid());
   }


   void GLOFNavHealth ::
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
            if (healthBits.is_valid())
            {
               s << "  Bn = " << hex << (unsigned)healthBits;
            }
            if (ln.is_valid())
            {
               s << "  ln = " << hex << (unsigned)ln;
            }
            if (Cn.is_valid())
            {
               s << "  Cn = " << hex << (unsigned)Cn;
            }
            s << endl;
            break;
         case DumpDetail::Full:
               // "header"
            s << "*************************************************************"
              << "***************" << endl
              << "Satellite Health"
              << endl
              << endl
              << getSignalString() << endl
              << "           TIMES OF INTEREST"
              << endl << endl
              << "              " << getDumpTimeHdr(dl) << endl
              << "Transmit:     " << getDumpTime(dl, timeStamp) << endl
              << endl
              << "           HEALTH DATA" << endl;
            if (healthBits.is_valid())
            {
               s << "Bn               0x" << hex << setw(1) << setfill('0')
                 << (unsigned)healthBits << endl;
            }
            if (ln.is_valid())
            {
               s << "ln               0x" << hex << setw(1) << setfill('0')
                 << (unsigned)ln << endl;
            }
            if (Cn.is_valid())
            {
               s << "Cn               0x" << hex << setw(1) << setfill('0')
                 << (unsigned)Cn << endl;
            }
            s << "Status             " << StringUtils::asString(getHealth())
              << endl;
            break;
      }
      s.flags(oldFlags);
   }


   SVHealth GLOFNavHealth ::
   getHealth() const
   {
         // healthBits + ln = ephemeris
      if (healthBits.is_valid() && ln.is_valid())
      {
         return ((ln.get_value() != false) || (healthBits & 0x04)
                 ? SVHealth::Unhealthy
                 : SVHealth::Healthy);
      }
         // Cn + ln = almanac (we ignore ln in this case, though)
      if (Cn.is_valid())
      {
         return (Cn ? SVHealth::Healthy : SVHealth::Unhealthy);
      }
         // ln on its own = string 5
      if (ln.is_valid())
      {
         return (ln ? SVHealth::Healthy : SVHealth::Unhealthy);
      }
      return SVHealth::Unknown;
   }

}
