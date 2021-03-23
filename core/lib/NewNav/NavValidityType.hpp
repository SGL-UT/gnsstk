//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
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
#ifndef GPSTK_NAVVALIDITYTYPE_HPP
#define GPSTK_NAVVALIDITYTYPE_HPP

#include <string>
#include "EnumIterator.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Enumeration used to determine how to handle valid and
       * invalid navigation data.  Validity is defined in the
       * individual ICDs, where ranges and such are defined. */
   enum class NavValidityType
   {
      Unknown,     ///< Only used as an initial value.
      ValidOnly,   ///< Only load/find nav messages that pass validity checks.
      InvalidOnly, ///< Only load/find nav messages that fail validity checks.
      Any,         ///< Load/find nav messages regardless of validity checks.
      Last         ///< Used to create an iterator.
   };

      /** Define an iterator so C++11 can do things like
       * for (NavValidityType i : NavValidityTypeIterator()) */
   typedef EnumIterator<NavValidityType, NavValidityType::Unknown, NavValidityType::Last> NavValidityTypeIterator;

   namespace StringUtils
   {
         /// Convert a NavValidityType to a whitespace-free string name.
      std::string asString(NavValidityType e) throw();
         /// Convert a string name to an NavValidityType
      NavValidityType asNavValidityType(const std::string& s) throw();
   }

      //@}

}

#endif // GPSTK_NAVVALIDITYTYPE_HPP
