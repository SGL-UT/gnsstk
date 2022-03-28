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
#ifndef GNSSTK_NAVSEARCHORDER_HPP
#define GNSSTK_NAVSEARCHORDER_HPP

#include <string>
#include "EnumIterator.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Specify the behavior of nav data searches in NavLibrary/NavDataFactory
   enum class NavSearchOrder
   {
      Unknown,///< Search order is not known or is uninitialized.
      User,   ///< Return the latest message before the search time.
      Nearest,///< Return the message closest to the search time.
      Last    ///< Used to create an iterator.
   };

      /** Define an iterator so C++11 can do things like
       * for (NavSearchOrder i : NavSearchOrderIterator()) */
   typedef EnumIterator<NavSearchOrder, NavSearchOrder::Unknown, NavSearchOrder::Last> NavSearchOrderIterator;

   namespace StringUtils
   {
         /// Convert a NavSearchOrder to a whitespace-free string name.
      std::string asString(NavSearchOrder e);
         /// Convert a string name to an NavSearchOrder
      NavSearchOrder asNavSearchOrder(const std::string& s);
   }

      //@}

}

#endif // GNSSTK_NAVSEARCHORDER_HPP
