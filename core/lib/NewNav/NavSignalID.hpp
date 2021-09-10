//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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
#ifndef GNSSTK_NAVSIGNALID_HPP
#define GNSSTK_NAVSIGNALID_HPP

#include <iostream>
#include <set>
#include "SatelliteSystem.hpp"
#include "ObsID.hpp"
#include "NavType.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Class used to identify navigation data signal types.
   class NavSignalID
   {
   public:
         /// Set all data members to "Unknown"
      NavSignalID();

         /** Initialize all data to specified values.
          * @param[in] sys The GNSS this signal originates from.
          * @param[in] car The carrier band of this signal.
          * @param[in] track The tracking code of this signal.
          * @param[in] nmt The navigation message format of this signal. 
          * @param[in] mcode Data to uniquely identify M-code signal.
          * @param[in] mcodeMask Bitmask for matching mcode. */
      NavSignalID(SatelliteSystem sys, CarrierBand car, TrackingCode track,
                  NavType nmt, uint32_t mcode = 0, uint32_t mcodeMask = -1);

         /** Initialize all data to specified values.
          * @param[in] sys The GNSS this signal originates from.
          * @param[in] oid An ObsID describing the signal (band, code, etc.)
          * @param[in] nmt The navigation message format of this signal. */
      NavSignalID(SatelliteSystem sys, const ObsID& oid, NavType nmt);

         /// Sorting so we can use this class as a map key
      bool operator<(const NavSignalID& right) const
      { return (order(right) < 0); }
         /// Equality check (all data members)
      bool operator==(const NavSignalID& right) const
      { return (order(right) == 0); }
         /// Inequality check (all data members)
      bool operator!=(const NavSignalID& right) const
      { return (order(right) != 0); }

         /// return true if any of the fields are set to match wildcards.
      virtual bool isWild() const;

         // Having the system here may seem redundant but if we're
         // identifying signals across an entire system (i.e. without
         // a specific PRN or other unique satellite ID as used in
         // NavSatelliteID) we need to identify the system as well to
         // differentiate e.g. between GPS L1 C/A and QZSS L1 C/A nav.
      SatelliteSystem system; ///< GNSS for this signal.
      ObsID obs;              ///< Carrier, tracking code, etc.
      NavType nav;            ///< Navigation message structure of this signal.

   protected:
         /** Generic comparison function that will tell us < = >
          * @param[in] right The object to compare with this.
          * @return <0 for this < right, == 0 for this == right, >0
          *   for this > right. */
      int order(const NavSignalID& right) const;
   };

      /// Set of nav data signal identifiers.
   typedef std::set<NavSignalID> NavSignalSet;


   inline std::ostream& operator<<(std::ostream& s, const NavSignalID& nsid)
   {
      s << StringUtils::asString(nsid.system) << " "
        << nsid.obs << " "
        << StringUtils::asString(nsid.nav);
      return s;
   }

      //@}

}

#endif // GNSSTK_NAVSIGNALID_HPP
