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
#ifndef GNSSTK_GLOFNAVUT1TIMEOFFSET_HPP
#define GNSSTK_GLOFNAVUT1TIMEOFFSET_HPP

#include "TimeOffsetData.hpp"
#include "TimeSystem.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Defines the class that provides the ability to convert
       * GLONASS to/from UT1 (UTC Grenwich, to distinguish it from
       * UTC(SU)), using data extracted from GLONASS navigation
       * messages.
       * The algorithm is sufficiently different from what's in
       * StdNavTimeOffset (in that the time difference is between the
       * ephemeris and almanac times rather than some reference time
       * and the desired time) that it's implemented apart from
       * StdNavTimeOffset. */
   class GLOFNavUT1TimeOffset : public TimeOffsetData
   {
   public:
         /// Initialize all data to 0.
      GLOFNavUT1TimeOffset();

         /** Print the contents of this object in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;

         /** Print the contents of this object in a terse format.
          * @param[in,out] s The stream to write the data to. */
      void dumpTerse(std::ostream& s) const;

         /** Get the offset, in seconds, to apply to times when
          * converting them from fromSys to toSys.
          * @param[in] fromSys The time system to convert from.
          * @param[in] toSys The time system to convert to.
          * @param[in] when The time being converted, in the same time
          *   system as fromSys.
          * @param[out] offset The offset in seconds where
          *   when(toSys)=when(fromSys)-offset.
          * @throw AssertionFailure if when's time system is not fromSys.
          * @return true if an offset is available, false if not. */
      bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                     const CommonTime& when, double& offset)
         const override;

         /** The set of time system conversions this class is capable of making.
          * @return a set of supported time system conversion to/from pairs. */
      TimeCvtSet getConversions() const override;

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;


         /// @todo document this stuff as well as you can
      double tauc; ///< Non-integer correction between UTC(SU) and GLONASS.
      double B1;   ///< Time bias in seconds.
      double B2;   ///< Time drift in s/s.
      unsigned KP; ///< Leap second indicator.
      unsigned NT; ///< Ephemeris days since leap year.
      unsigned NA; ///< Almanac days since leap year.
   };

      //@}

}

#endif // GNSSTK_GLOFNAVUT1TIMEOFFSET_HPP
