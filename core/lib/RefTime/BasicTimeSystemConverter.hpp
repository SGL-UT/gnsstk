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

#ifndef GNSSTK_BASICTIMESYSTEMCONVERTER_HPP
#define GNSSTK_BASICTIMESYSTEMCONVERTER_HPP

#include "TimeSystemConverter.hpp"

namespace gnsstk
{
      /// @ingroup TimeHandling
      //@{

      /** Define a class for doing time system conversions using only
       * a hard-coded table of second-resolution offsets from TAI. */
   class BasicTimeSystemConverter : public TimeSystemConverter
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
      bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                     const CommonTime& t, double& offs) override;

         /** Attempt to "prime" internal data over a desired time
          * range so that when doing a large number of conversions
          * over a time range, if the offset is the same across that
          * time range, we don't do a conversion every single call.
          * If the offset at fromTime is the same as at toTime, any
          * calls to getOffset after calling explore will use the
          * internally stored offset as opposed to attempting to do
          * conversions.  This is a performance enhancement and is not
          * strictly necessary.
          * @param[in] fromSys The time system to convert from.
          * @param[in] toSys The time system to convert to.
          * @param[in] fromTime The start of the time range of interest.
          * @param[in] toTime The end of the time range of interest.
          * @return false if unable to determine the offset.
          */
      bool explore(TimeSystem fromSys, TimeSystem toSys,
                   const CommonTime& fromTime, const CommonTime& toTime);

   protected:
         /// Time system being converted from using toffs.
      TimeSystem fromSystem;
         /// Time system being converted to using toffs.
      TimeSystem toSystem;
         /// First time that toffs is known to be valid.
      CommonTime fromTimeStamp;
         /// Final time that toffs is known to be valid.
      CommonTime toTimeStamp;
         /// Stored offset for time system conversion.
      double toffs;
   };
}
      //@}

#endif // GNSSTK_BASICTIMESYSTEMCONVERTER_HPP
