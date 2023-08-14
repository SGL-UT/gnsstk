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

#include "BasicTimeSystemConverter.hpp"
#include "CivilTime.hpp"

namespace gnsstk
{
   bool BasicTimeSystemConverter ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& t, double& offs)
   {
      if (fromSys == toSys)
      {
            // nothing to do
         offs = 0.;
         return true;
      }
         // Make sure to do enum comparison first as that is much less
         // expensive than time comparison.
      if ((fromSys == fromSystem) && (toSys == toSystem) &&
          (t >= fromTimeStamp) && (t <= toTimeStamp))
      {
         offs = toffs;
         return true;
      }
      CivilTime civ(t);
      try
      {
         offs = - getTimeSystemCorrection(fromSys, toSys, civ.year, civ.month,
                                          civ.day);
      }
      catch (gnsstk::Exception& exc)
      {
         std::cerr << exc << std::endl;
         return false;
      }
      return true;
   }


   bool BasicTimeSystemConverter ::
   explore(TimeSystem fromSys, TimeSystem toSys,
           const CommonTime& fromTime, const CommonTime& toTime)
   {
      double offs1, offs2;
      if (getOffset(fromSys, toSys, fromTime, offs1) &&
          getOffset(fromSys, toSys, toTime, offs2))
      {
         if (offs1 == offs2)
         {
            fromSystem = fromSys;
            toSystem = toSys;
            fromTimeStamp = fromTime;
            toTimeStamp = toTime;
            toffs = offs1;
         }
         return true;
      }
      else
      {
         return false;
      }
   }
}
