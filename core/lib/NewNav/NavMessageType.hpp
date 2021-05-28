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
#ifndef GPSTK_NAVMESSAGETYPE_HPP
#define GPSTK_NAVMESSAGETYPE_HPP

#include <string>
#include <set>
#include "EnumIterator.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Identify different types of navigation message data.
   enum class NavMessageType
   {
      Unknown,    ///< Message type is not known or is uninitialized.
      Almanac,    ///< Low-precision orbits for other than the transmitting SV.
      Ephemeris,  ///< Precision orbits for the transmitting SV.
      TimeOffset, ///< Message containing information about time system offsets
      Health,     ///< SV health status information message.
      Clock,      ///< SV Clock offset data.  Currently only used by SP3.
      Iono,       ///< Ionospheric correction data.
      Last        ///< Used to create an iterator.
   };

      /** Define an iterator so C++11 can do things like
       * for (NavMessageType i : NavMessageTypeIterator()) */
   typedef EnumIterator<NavMessageType, NavMessageType::Unknown, NavMessageType::Last> NavMessageTypeIterator;
      /// Set of message types, used by NavLibrary and NavDataFactory.
   typedef std::set<NavMessageType> NavMessageTypeSet;

      /** Convenient set of all valid (non-meta) message types.
       * @note This is defined in NavStatic.cpp */
   extern const NavMessageTypeSet allNavMessageTypes;

   namespace StringUtils
   {
         /// Convert a NavMessageType to a whitespace-free string name.
      std::string asString(NavMessageType e) throw();
         /// Convert a string name to an NavMessageType
      NavMessageType asNavMessageType(const std::string& s) throw();
   }

      //@}

}

#endif // GPSTK_NAVMESSAGETYPE_HPP
