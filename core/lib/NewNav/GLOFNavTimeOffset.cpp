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
#include "CivilTime.hpp"
#include "GLOFNavTimeOffset.hpp"
#include "GPSWeekSecond.hpp"
#include "TimeSystem.hpp"

using namespace std;

namespace gnsstk
{
   GLOFNavTimeOffset ::
   GLOFNavTimeOffset()
   {
      msgLenSec = 2.0;
         // The ICD uses the opposite sign convention from what
         // StdNavTimeOffset uses, so just swap the src and tgt from
         // what you would normally expect.
      src = TimeSystem::GPS;
      tgt = TimeSystem::GLO;
      weekFmt = "";
   }


   bool GLOFNavTimeOffset ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, double& offset) const
   {
      bool result = StdNavTimeOffset::getOffset(fromSys, toSys, when, offset);

      if (!result)
      {
         return result;
      }

      // GLO is 3h in advance of UTC. 
      // StdNavTimeOffset does not account for this bulk offset.
      CivilTime civilWhen{when};
      double offsetUTC = getTimeSystemCorrection(TimeSystem::GLO, 
                                                 TimeSystem::UTC,
                                                 civilWhen.year, 
                                                 civilWhen.month, 
                                                 civilWhen.day);
      if (fromSys == tgt)
      {
         offsetUTC = -offsetUTC;
      }
      offset += offsetUTC;

      return result;
   }

   bool GLOFNavTimeOffset ::
   validate() const
   {
         /// @todo add some checks.
      return true;
   }
}

