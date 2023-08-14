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
//  Copyright 2004-2023, The Board of Regents of The University of Texas System
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
#ifndef GNSSTK_GPSNavConfig_HPP
#define GNSSTK_GPSNavConfig_HPP

#include <memory>
#include <ostream>

#include "DumpDetail.hpp"
#include "GPSSVConfig.hpp"
#include "SystemNavData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Wrapper for the 4-bit A-S status and SV configuration in GPS LNav
       * subframe 4, page 25. */
   class GPSNavConfig : public SystemNavData
   {
   public:
         /// Initialize to ICD-defined "no info" values.
      GPSNavConfig();

         /// Create a deep copy of this object.
      NavDataPtr clone() const override
      {
         return std::make_shared<GPSNavConfig>(*this);
      }

         /** Checks the contents of this message against known validity rules
          * as defined in the appropriate ICD.
          * @return true always as there is nothing to check in this class.
          */
      bool validate() const override
      { return true; }

         /** Print the contents of this object in a human-readable format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream &s, DumpDetail dl) const override;

         /** Defines the config of NavData::signal, specifically sat
          * (not xmitSat).
          * @return SVConfig value; noInfo if an invalid value is present. */
      GPSSVConfig getSVConfig() const noexcept;

         /// Whether Anti-Spoof is on for this satellite.
      bool antispoofOn{false};
         /// The 3-bit SV configuration (defaults to noInfo).
      uint8_t svConfig{0};
};

      //@}
} // namespace gnsstk

#endif // GNSSTK_GPSNavConfig_HPP
