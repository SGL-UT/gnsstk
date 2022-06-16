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
#ifndef GNSSTK_GLOFNAVHEALTH_HPP
#define GNSSTK_GLOFNAVHEALTH_HPP

#include "ValidType.hpp"
#include "NavHealthData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Wrapper for the health status data scattered throughout the
       * GLONASS Civil F NAV data. */
   class GLOFNavHealth : public NavHealthData
   {
   public:
         /// Initialize to unhealthy using a value typically not seen in health.
      GLOFNavHealth();
         /// Create a deep copy of this object.
      NavDataPtr clone() const override
      { return std::make_shared<GLOFNavHealth>(*this); }

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if any of the data are usable.
          */
      bool validate() const override;

         /** Print the contents of this object in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;

         /** Defines the status of NavData::signal, specifically sat
          * (not xmitSat).
          * @return Healthy if no health bits are set. */
      SVHealth getHealth() const override;

         // We use ValidType to account for the fact that the health
         // data can be a variety of combinations.
      ValidType<uint8_t> healthBits; ///< The 3-bit B_n value.
      ValidType<bool> ln; ///< The l_n health bit in strings 3,5,7,9,11,13,15.
      ValidType<bool> Cn; ///< The C_n health bit in strings 6,8,10,12,14.
   };

      //@}

}

#endif // GNSSTK_GLOFNAVHEALTH_HPP
