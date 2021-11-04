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
#include <iterator>
#include "NavDataFactory.hpp"
#include "TimeString.hpp"

/// debug time string
static const std::string dts("%Y/%03j/%02H:%02M:%02S %P");

namespace gnsstk
{
   bool NavDataFactory :: isPresent(const NavMessageID& nmid,
                                    const CommonTime& fromTime,
                                    const CommonTime& toTime)
   {
      CommonTime t = fromTime;
      NavDataPtr junk;
         // std::cerr << __PRETTY_FUNCTION__ << " "
         //           << gnsstk::printTime(fromTime, dts) << " "
         //           << gnsstk::printTime(toTime, dts)
         //           << std::endl;
      while (true)
      {
            // std::cerr << "isPresent calling find @ " << gnsstk::printTime(t, dts)
            //           << std::endl;
         if (find(nmid, t, junk, SVHealth::Any, NavValidityType::Any,
                  NavSearchOrder::User))
         {
            return true;
         }
            // Jump forward a somewhat arbitrarily chosen 1 hour in
            // time until we hit toTime to see if there are any other
            // possibilities given that this time failed to turn up
            // anything.  The logic is used to include toTime but not
            // get stuck in an infinite loop.
         if (t < toTime)
         {
            t += 3600.0;
            t = std::min(t, toTime);
         }
         else
         {
            break;
         }
      }
      return false;
   }


   std::set<SatID> NavDataFactory ::
   getIndexSet(const CommonTime& fromTime,
               const CommonTime& toTime) const
   {
      NavSatelliteIDSet fullSatSet = getAvailableSats(fromTime,toTime);
      std::set<SatID> rv;
      for (const auto& fssi : fullSatSet)
      {
         rv.insert(fssi.sat);
      }
      return rv;
   }


   std::set<SatID> NavDataFactory ::
   getIndexSet(NavMessageType nmt,
               const CommonTime& fromTime,
               const CommonTime& toTime) const
   {
      NavSatelliteIDSet fullSatSet = getAvailableSats(nmt,fromTime,toTime);
      std::set<SatID> rv;
      for (const auto& fssi : fullSatSet)
      {
         rv.insert(fssi.sat);
      }
      return rv;
   }
}
