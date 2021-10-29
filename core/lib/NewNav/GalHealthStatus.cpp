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
#include "GalHealthStatus.hpp"

namespace gnsstk
{
   namespace StringUtils
   {
      std::string asString(GalHealthStatus e) throw()
      {
         switch (e)
         {
            case GalHealthStatus::Unknown:      return "Unknown";
            case GalHealthStatus::OK:           return "OK";
            case GalHealthStatus::OutOfService: return "OutOfService";
            case GalHealthStatus::WillBeOOS:    return "WillBeOOS";
            case GalHealthStatus::InTest:       return "InTest";
            default:                            return "???";
         } // switch (e)
      } // asString(GalHealthStatus)


      GalHealthStatus asGalHealthStatus(const std::string& s) throw()
      {
         if (s == "Unknown")
            return GalHealthStatus::Unknown;
         if (s == "OK")
            return GalHealthStatus::OK;
         if (s == "OutOfService")
            return GalHealthStatus::OutOfService;
         if (s == "WillBeOOS")
            return GalHealthStatus::WillBeOOS;
         if (s == "InTest")
            return GalHealthStatus::InTest;
         return GalHealthStatus::Unknown;
      } // asGalHealthStatus(string)
   } // namespace StringUtils
} // namespace gnsstk
