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
#include "NavSignalID.hpp"

namespace gnsstk
{
   NavSignalID ::
   NavSignalID()
         : system(SatelliteSystem::Unknown),
           nav(NavType::Unknown)
   {
   }


   NavSignalID ::
   NavSignalID(SatelliteSystem sys, CarrierBand car, TrackingCode track,
               NavType nmt, uint32_t mcode, uint32_t mcodeMask)
         : system(sys),
           obs(gnsstk::ObservationType::NavMsg, car, track),
           nav(nmt)
   {
      obs.setMcodeBits(mcode, mcodeMask);
   }


   NavSignalID ::
   NavSignalID(SatelliteSystem sys, const ObsID& oid, NavType nmt)
         : system(sys),
           obs(oid),
           nav(nmt)
   {
   }


// Use this macro in operator<< to figure out why things fail
#if 0
#define ORDERRET(RV) {                                                  \
      std::cerr << "order() returning " << RV << " @ " << __LINE__      \
                << std::endl;                                           \
      return RV;                                                        \
   }
#else
#define ORDERRET(RV) return RV;
#endif

   int NavSignalID ::
   order(const NavSignalID& right) const
   {
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
      if (system < right.system) ORDERRET(-1);
      if (system > right.system) ORDERRET(1);
      if (obs < right.obs) ORDERRET(-1);
      if (right.obs < obs) ORDERRET(1);
      if ((nav != NavType::Any) && (right.nav != NavType::Any))
      {
         if (nav < right.nav) ORDERRET(-1);
         if (nav > right.nav) ORDERRET(1);
      }
      ORDERRET(0);
   }


   bool NavSignalID ::
   isWild() const
   {
      return (obs.isWild() || (nav == NavType::Any));
   }
}
