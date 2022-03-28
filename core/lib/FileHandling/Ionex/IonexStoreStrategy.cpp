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
#include "IonexStoreStrategy.hpp"
#include "StringUtils.hpp"

namespace gnsstk
{
   namespace StringUtils
   {
      std::string asString(IonexStoreStrategy e)
      {
         switch (e)
         {
            case IonexStoreStrategy::Unknown:     return "Unknown";
            case IonexStoreStrategy::Nearest:     return "Nearest";
            case IonexStoreStrategy::Consecutive: return "Consecutive";
            case IonexStoreStrategy::ConsRot:     return "ConsRot";
            case IonexStoreStrategy::Rotated:     return "Rotated";
            default:                              return "???";
         } // switch (e)
      } // asString(IonexStoreStrategy)


      IonexStoreStrategy asIonexStoreStrategy(const std::string& s)
      {
         std::string lower(s);
         StringUtils::lowerCase(lower);
         if (lower == "nearest")
            return IonexStoreStrategy::Nearest;
         if (lower == "consecutive")
            return IonexStoreStrategy::Consecutive;
         if (lower == "consrot")
            return IonexStoreStrategy::ConsRot;
         if (lower == "rotated")
            return IonexStoreStrategy::Rotated;
         return IonexStoreStrategy::Unknown;
      } // asIonexStoreStrategy(string)
   } // namespace StringUtils
} // namespace gnsstk
