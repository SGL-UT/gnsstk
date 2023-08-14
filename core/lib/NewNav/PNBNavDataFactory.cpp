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
#include "PNBNavDataFactory.hpp"
#include "NavMessageType.hpp"

using namespace std;

namespace gnsstk
{
   PNBNavDataFactory ::
   PNBNavDataFactory()
         : navValidity(NavValidityType::Any)
   {
      setTypeFilter(allNavMessageTypes);
   }


   void PNBNavDataFactory ::
   setTypeFilter(const NavMessageTypeSet& nmts)
   {
         // We use boolean values instead of a set so that we're not
         // checking a set every time a new subframe is added.
      processEph  = nmts.count(gnsstk::NavMessageType::Ephemeris) > 0;
      processAlm  = nmts.count(gnsstk::NavMessageType::Almanac) > 0;
      processHea  = nmts.count(gnsstk::NavMessageType::Health) > 0;
      processTim  = nmts.count(gnsstk::NavMessageType::TimeOffset) > 0;
      processIono = nmts.count(gnsstk::NavMessageType::Iono) > 0;
      processISC  = nmts.count(gnsstk::NavMessageType::ISC) > 0;
      processSys  = nmts.count(gnsstk::NavMessageType::System) > 0;
   }

}
