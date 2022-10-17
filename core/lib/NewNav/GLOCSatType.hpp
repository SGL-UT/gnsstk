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
#ifndef GNSSTK_GLOCSATTYPE_HPP
#define GNSSTK_GLOCSATTYPE_HPP

#include <string>

namespace gnsstk
{
      /// Values for Word M in the ephemeris (immediate) and almanac data.
   enum class GLOCSatType
   {
      Unknown = -1,   ///< Unknown/Uninitialized value.
      GLONASS_M = 0,  ///< GLONASS-M with L3 nav payload transmits L3 CDMA.
      GLONASS_K1 = 1, ///< GLONASS-K1 transmits L3 CDMA.
      GLONASS_K2 = 2, ///< GLONASS-K2 transmits L1, L2 and L3 CDMA.
      GLONASS_K1b= 3, ///< GLONASS-K1 transmits L2 and L3 CDMA.
   };


   namespace StringUtils
   {
      std::string asString(GLOCSatType e);
   }
}

#endif // GNSSTK_GLOCSATTYPE_HPP
