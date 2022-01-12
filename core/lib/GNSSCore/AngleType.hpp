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

#ifndef GNSSTK_ANGLETYPE_HPP
#define GNSSTK_ANGLETYPE_HPP

#include <string>
#include "EnumIterator.hpp"

namespace gnsstk
{
      /// @ingroup Geodetic
      //@{

      /** Because the angle can be initialized via a variety of
       * different values that are the same type, we use this enum
       * to indicate in the constructor what type is being
       * passed. */
   enum class AngleType
   {
      Unknown,    ///< Uninitialized value.
      Rad,        ///< Value is in radians.
      Deg,        ///< Value is in degrees.
      SemiCircle, ///< Value is in semi-circles (aka half-cycles).
      Sin,        ///< Value is the sine of the angle.
      Cos,        ///< Value is the cosine of the angle.
      Last        ///< Used to create an iterator.
   };

      /** Define an iterator so C++11 can do things like
       * for (AngleType i : AngleTypeIterator()) */
   typedef EnumIterator<AngleType, AngleType::Unknown, AngleType::Last> AngleTypeIterator;

   namespace StringUtils
   {
         /// Convert a AngleType to a whitespace-free string name.
      std::string asString(AngleType e) throw();
         /// Convert a string name to an AngleType
      AngleType asAngleType(const std::string& s) throw();
   }

      //@}
}

#endif // GNSSTK_ANGLETYPE_HPP
