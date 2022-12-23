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
#ifndef GNSSTK_RINEXTIMEOFFSETDATA_HPP
#define GNSSTK_RINEXTIMEOFFSETDATA_HPP

#include "TimeOffsetData.hpp"
#include "TimeSystemCorr.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Defines the class used to store time system offset
       * information as stored in RINEX nav headers. */
   class RinexTimeOffset : public TimeOffsetData, public TimeSystemCorrection
   {
   public:
         /// Initialize all data to 0.
      RinexTimeOffset();
         /// Create a deep copy of this object.
      NavDataPtr clone() const override
      { return std::make_shared<RinexTimeOffset>(*this); }

         /** Construct from a pre-existing TimeSystemCorrection object
          * and leap seconds.
          * @param[in] right The TimeSystemCorrection obtained from a
          *   RINEX NAV header.
          * @param[in] leapSec The number of leap seconds from the
          *   same RINEX NAV header. */
      RinexTimeOffset(const TimeSystemCorrection& right, double leapSec);

         /// Obligatory virtual destructor.
      virtual ~RinexTimeOffset()
      {}

         /** Checks the contents of the data against known validity
          * rules.
          * @return true if this message is valid according to criteria.
          */
      bool validate() const override;

         /** Returns the time when the time offset data would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.
          * @note This information is sketchy at best in RINEX.
          * @return transmit time + 12s or 6s, depending on whether
          *   this is CNAV on L5 or on L2.
          */
      CommonTime getUserTime() const override;

         /** Print the contents of this object in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;

         /** Get the offset, in seconds, to apply to times when
          * converting them from fromSys to toSys.
          * @param[in] fromSys The time system to convert from.
          * @param[in] toSys The time system to convert to.
          * @param[in] when The time being converted, in the GPS time
          *   system.
          * @param[out] offset The offset in seconds where
          *   when(toSys)=when(fromSys)-offset.
          * @return true if an offset is available, false if not. */
      bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                     const CommonTime& when, double& offset)
         const override;

         /** The set of time system conversions this class is capable of making.
          * @return a set of supported time system conversion to/from pairs. */
      TimeCvtSet getConversions() const override;

      double deltatLS;  ///< Current or past leap second count (UTC only)
   };

      //@}
} // namespace gnsstk

#endif // GNSSTK_RINEXTIMEOFFSETDATA_HPP
