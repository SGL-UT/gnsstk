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

#include "RefFrameSys.hpp"

using namespace std;

namespace gnsstk
{
   namespace StringUtils
   {
      std::string asString(RefFrameSys e)
         noexcept
      {
         switch (e)
         {
            case RefFrameSys::Unknown:    return "Unknown";
            case RefFrameSys::WGS84:      return "WGS84";
            case RefFrameSys::ITRF:       return "ITRF";
            case RefFrameSys::PZ90:       return "PZ90";
            case RefFrameSys::CGCS2000:   return "CGCS2000";
            default:                      return "???";
         }
      }


      RefFrameSys asRefFrameSys(const std::string& s)
         noexcept
      {
         if (s == "Unknown")      return RefFrameSys::Unknown;
         if (s == "WGS84")        return RefFrameSys::WGS84;
         if (s == "ITRF")         return RefFrameSys::ITRF;
         if (s == "PZ90")         return RefFrameSys::PZ90;
         if (s == "CGCS2000")     return RefFrameSys::CGCS2000;
         return RefFrameSys::Unknown;
      }
   }
}   // end namespace
