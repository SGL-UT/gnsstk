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

#ifndef GNSSTK_TIMESYSTEMCONVERTER_HPP
#define GNSSTK_TIMESYSTEMCONVERTER_HPP

// forward declarations for CommonTime
namespace gnsstk
{
   class TimeSystemConverter;
   class CommonTime;
}

#include "CommonTime.hpp"

namespace gnsstk
{
      /// @ingroup TimeHandling
      //@{

      /** Define an abstract base class for implementing conversions
       * between time systems. */
   class TimeSystemConverter
   {
   public:
         /** Get the offset in seconds between fromSys and toSys.
          * @param[in] fromSys The time system to convert from.
          * @param[in] toSys The time system to convert to.
          * @param[in] t The time at which the offset is being
          *   requested (i.e. the time being converted).
          * @param[out] offs The resulting offset in seconds, 
          *   if available, where when(toSys)=when(fromSys)-offset.
          * @return true if successful, false if unavailable. */
      virtual bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                             const CommonTime& t, double& offs) = 0;
   };

      //@}

}

#endif // GNSSTK_TIMESYSTEMCONVERTER_HPP
