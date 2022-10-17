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

#include "NavTimeSystemConverter.hpp"
#include "CivilTime.hpp"
#include "DebugTrace.hpp"

namespace gnsstk
{
   bool NavTimeSystemConverter ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& t, double& offs)
   {
      DEBUGTRACE_FUNCTION();
      if (fromSys == toSys)
      {
            // nothing to do
         offs = 0.;
         DEBUGTRACE("same system, 0 offset");
         return true;
      }
      if (navLib.get() == nullptr)
      {
         DEBUGTRACE("null navLib");
         return false;
      }
         // try direct offset first
         /** @note We use Any health because if someone loads the
          * NavLibrary with only TimeOffset data, there won't be any
          * health to match, resulting in a failure. */
      if (navLib->getOffset(fromSys, toSys, t, offs, SVHealth::Any))
      {
         DEBUGTRACE("successful direct conversion, offs="
                    << std::setprecision(15) << std::scientific << offs);
         return true;
      }
      double offs1, offs2;
         // that didn't work, try converting fromSys->UTC->toSys
      if (navLib->getOffset(fromSys, TimeSystem::UTC, t, offs1,
                            SVHealth::Healthy) &&
          navLib->getOffset(TimeSystem::UTC, toSys, t, offs2,
                            SVHealth::Healthy))
      {
         offs = offs1 + offs2;
         DEBUGTRACE("successful indirect conversion, offs="
                    << std::setprecision(15) << std::scientific << offs);
         return true;
      }
      DEBUGTRACE("unable to get time offset");
      return false;
   }
}
