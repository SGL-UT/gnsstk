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
#ifndef GPSTK_NAVMESSAGEID_HPP
#define GPSTK_NAVMESSAGEID_HPP

#include <iostream>
#include "NavSatelliteID.hpp"
#include "NavMessageType.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Class used to identify/categorize navigation message data.
   class NavMessageID : public NavSatelliteID
   {
   public:
         /// Initialize message type to Unknown
      NavMessageID()
            : messageType(NavMessageType::Unknown)
      {}
         /// Convenience constructor from NavSatelliteID
      NavMessageID(const NavSatelliteID& sat, NavMessageType nmt)
            : NavSatelliteID(sat), messageType(nmt)
      {}
         /// Ordering operator.
      bool operator<(const NavMessageID& right) const
      {
         if (messageType < right.messageType)
            return true;
         if (messageType > right.messageType)
            return false;
         return NavSatelliteID::operator<(right);
      }
         /// Other ordering operator.
      bool operator>(const NavMessageID& right) const
      {
         if (messageType > right.messageType)
            return true;
         if (messageType < right.messageType)
            return false;
         return (!NavSatelliteID::operator<(right) &&
                 NavSatelliteID::operator!=(right));
      }
         /// Comparison operator, obv.
      bool operator==(const NavMessageID& right) const
      {
         // std::cerr << __PRETTY_FUNCTION__ << std::endl;
         return ((messageType == right.messageType) &&
                 NavSatelliteID::operator==(right));
      }
         /// Implicit != not available
      bool operator!=(const NavMessageID& right) const
      {
         return ((messageType != right.messageType) ||
                 NavSatelliteID::operator!=(right));
      }
         /** Indicates whether a nav message is orbital elements,
          * health data or time offset information. */
      NavMessageType messageType;
   };


   inline std::ostream& operator<<(std::ostream& s, const NavMessageID& nmid)
   {
      s << StringUtils::asString(nmid.messageType) << " "
        << static_cast<NavSatelliteID>(nmid);
      return s;
   }

      //@}

}

#endif // GPSTK_NAVMESSAGEID_HPP
