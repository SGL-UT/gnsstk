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
#include "NavMessageType.hpp"

namespace gnsstk
{
   namespace StringUtils
   {
      std::string asString(NavMessageType e) noexcept
      {
         switch (e)
         {
            case NavMessageType::Unknown:    return "Unknown";
            case NavMessageType::Almanac:    return "Almanac";
            case NavMessageType::Ephemeris:  return "Ephemeris";
            case NavMessageType::TimeOffset: return "TimeOffset";
            case NavMessageType::Health:     return "Health";
            case NavMessageType::Clock:      return "Clock";
            case NavMessageType::Iono:       return "Iono";
            case NavMessageType::ISC:        return "ISC";
            case NavMessageType::System:     return "System";
            default:                         return "???";
         } // switch (e)
      } // asString(NavMessageType)


      NavMessageType asNavMessageType(const std::string& s) noexcept
      {
         if (s == "Unknown")
            return NavMessageType::Unknown;
         if (s == "Almanac")
            return NavMessageType::Almanac;
         if (s == "Ephemeris")
            return NavMessageType::Ephemeris;
         if (s == "TimeOffset")
            return NavMessageType::TimeOffset;
         if (s == "Health")
            return NavMessageType::Health;
         if (s == "Clock")
            return NavMessageType::Clock;
         if (s == "Iono")
            return NavMessageType::Iono;
         if (s == "ISC")
            return NavMessageType::ISC;
         if (s == "System")
            return NavMessageType::System;
         return NavMessageType::Unknown;
      } // asNavMessageType(string)
   } // namespace StringUtils
} // namespace gnsstk
