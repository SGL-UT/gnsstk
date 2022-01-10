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
#include "AngleType.hpp"

namespace gnsstk
{
   namespace StringUtils
   {
      std::string asString(AngleType e) throw()
      {
         switch (e)
         {
            case AngleType::Unknown:    return "Unknown";
            case AngleType::Rad:        return "radians";
            case AngleType::Deg:        return "degrees";
            case AngleType::SemiCircle: return "semi-circles";
            case AngleType::Sin:        return "sin";
            case AngleType::Cos:        return "cos";
            default:                    return "???";
         }
      }


      AngleType asAngleType(const std::string& s) throw()
      {
         if (s == "Unknown")
            return AngleType::Unknown;
         if (s == "radians")
            return AngleType::Rad;
         if (s == "degrees")
            return AngleType::Deg;
         if (s == "semi-circles")
            return AngleType::SemiCircle;
         if (s == "sin")
            return AngleType::Sin;
         if (s == "cos")
            return AngleType::Cos;
         return AngleType::Unknown;
      }
   } // namespace StringUtils
} // namespace gnsstk
