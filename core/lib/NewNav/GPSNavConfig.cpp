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
//  Copyright 2004-2023, The Board of Regents of The University of Texas System
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
#include "GPSNavConfig.hpp"

#include <iomanip>
#include <ios>
#include <ostream>
#include <type_traits>

#include "DumpDetail.hpp"
#include "GPSSVConfig.hpp"

namespace gnsstk
{
   GPSNavConfig ::
   GPSNavConfig()
   {
      msgLenSec = 6.0;
   }

   void GPSNavConfig ::
   dump(std::ostream &s, DumpDetail dl) const
   {
      const std::ios::fmtflags oldFlags{s.flags()};
      s.setf(std::ios::fixed, std::ios::floatfield);
      s.setf(std::ios::right, std::ios::adjustfield);
      s.setf(std::ios::uppercase);
      s.precision(0);
      s.fill(' ');

      switch (dl)
      {
      case DumpDetail::OneLine:
         NavData::dump(s, dl);
         break;
      case DumpDetail::Brief:
         NavData::dump(s, dl);
            // clang-format off
         s << "antispoof " << (antispoofOn ? "on" : "off") << '\n'
           << "svConfig = " << std::hex << static_cast<int>(svConfig) << std::dec << "  " << StringUtils::asString(svConfig) << '\n';
            // clang-format on         
         break;
      case DumpDetail::Full:
            // "header"
            // clang-format off
         s << "****************************************************************************\n"
           << "Anti-Spoof and SV configuration\n\n"
           << getSignalString() << '\n'
           << "TIMES OF INTEREST\n\n"
           << "          " << getDumpTimeHdr(dl) << '\n'
           << "Transmit: " << getDumpTime(dl, timeStamp) << "\n\n"
           << "Anti-Spoof flag: " << (antispoofOn ? '1' : '0') << '\n'
           << "SV config bits:  0x" << std::hex << std::setw(3) << std::setfill('0') << static_cast<int>(svConfig) << std::dec << '\n'
           << "SV config:       " << StringUtils::asString(getSVConfig()) << '\n';
            // clang-format on
         break;
      }
      s.flags(oldFlags);
   }

   GPSSVConfig GPSNavConfig ::
   getSVConfig() const noexcept
   {
      for (const auto &svConfigVal : GPSSVConfigIterator())
      {
         if (svConfig ==
             static_cast<std::underlying_type<GPSSVConfig>::type>(svConfigVal))
         {
            return svConfigVal;
         }
      }
      return GPSSVConfig::noInfo;
   }
} // namespace gnsstk
