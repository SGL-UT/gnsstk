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

#ifndef GNSSTK_XMITANT_HPP
#define GNSSTK_XMITANT_HPP

#include <string>
#include "EnumIterator.hpp"

namespace gnsstk
{
      /** Enumerate satellite (pseudolite, etc.) transmitting antenna
       * types.  Most will be "Standard", while a few satellites will
       * have secondary "Regional" transmitters.  The secondary,
       * "Regional" transmitter should NOT be confused with the
       * primary transmitter for a regional system such as QZSS or
       * NavIC. */
   enum class XmitAnt
   {
      Unknown,   ///< Unitialized value.  Should always be first.
      Any,       ///< When making comparisons in ObsID, matches any enumeration.
      Standard,  ///< Transmitting antenna is the primary for that signal.
      Regional,  ///< Transmitting antenna is secondary, regional coverage.
      Last       ///< Used to verify that all items are described at compile time
   }; // enum class XmitAnt

      /** Define an iterator so C++11 can do things like
       * for (XmitAnt i : XmitAntIterator()) */
   typedef EnumIterator<XmitAnt, XmitAnt::Unknown, XmitAnt::Last> XmitAntIterator;

   namespace StringUtils
   {
         /// Convert a XmitAnt to a whitespace-free string name.
      std::string asString(XmitAnt e) throw();
         /// Convert a string name to an XmitAnt
      XmitAnt asXmitAnt(const std::string& s) throw();
   }
} // namespace gnsstk

#endif // GNSSTK_XMITANT_HPP
