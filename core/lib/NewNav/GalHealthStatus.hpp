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
#ifndef GNSSTK_GALHEALTHSTATUS_HPP
#define GNSSTK_GALHEALTHSTATUS_HPP

#include <string>
#include "EnumIterator.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Identify different types of SV health states.
   enum class GalHealthStatus
   {
      Unknown = -1,     ///< Health is not known or is uninitialized.
      OK = 0,           ///< Signal OK.
      OutOfService = 1, ///< Signal out of service.
      WillBeOOS = 2,    ///< Signal will be out of service.
      InTest = 3,       ///< Signal Component currently in Test
      Last              ///< Used to create an iterator.
   };

      /** Define an iterator so C++11 can do things like
       * for (GalHealthStatus i : GalHealthStatusIterator()) */
   typedef EnumIterator<GalHealthStatus, GalHealthStatus::Unknown, GalHealthStatus::Last> GalHealthStatusIterator;

   namespace StringUtils
   {
         /// Convert a GalHealthStatus to a whitespace-free string name.
      std::string asString(GalHealthStatus e) noexcept;
         /// Convert a string name to an GalHealthStatus
      GalHealthStatus asGalHealthStatus(const std::string& s) noexcept;
   }

      //@}

}

#endif // GNSSTK_GALHEALTHSTATUS_HPP
