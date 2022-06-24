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
#ifndef GNSSTK_BDSD2NAVHEALTH_HPP
#define GNSSTK_BDSD2NAVHEALTH_HPP

#include "NavHealthData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Wrapper for the 9-bit health status in BeiDou D2NAV subframe
       * 5, page 7-8. */
   class BDSD2NavHealth : public NavHealthData
   {
   public:
         /** Enumeration for defining the D2 health bits meaning.
          * Intentionally not an enum class, which would require
          * having to cast the values to an integer before doing bit
          * math e.g. "svHealth & SatClock" vs. "svHealth &
          * static_cast<uint16_t>(BDSD2HealthBits::SatClock)" */
      enum BDSD2HealthBits
      {
            /** Per table 5-15: "the satellite clock is unavailable if
             * the other 8 bits are all “0”; the satellite is in
             * failure or permanently shut off if the last 8bits are
             * all “1”; the definition is reserved if the other 8 bits
             * are in other values." */
         SatClock = 0x100,
         B1ISignalWeak = 0x80, ///< B1I signal power 10dB+ lower than nominal.
         B2ISignalWeak = 0x40, ///< B2I signal power 10dB+ lower than nominal.
         B3ISignalWeak = 0x20, ///< B3I signal power 10dB+ lower than nominal.
         NavMsgBad = 0x02,     ///< IOD over limit
            // remaining bits are currently marked reserved
      };

         /// Initialize to unhealthy using a value typically not seen in health.
      BDSD2NavHealth();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true always as there is nothing to check in this class.
          */
      bool validate() const override
      { return true; }

         /** Print the contents of this object in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;

         /** Defines the status of NavData::signal, specifically sat
          * (not xmitSat).
          * @return Healthy if no health bits are set. */
      SVHealth getHealth() const override;

         /// If true, svHealth is representative, otherwise satH1 is.
      bool isAlmHealth;
         /// Ephemeris health (autonomous satellite health flag).
      bool satH1;
         /// 9-bit health from subframes 4.
      uint16_t svHealth;
   };

      //@}

}

#endif // GNSSTK_BDSD2NAVHEALTH_HPP
