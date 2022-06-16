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
#ifndef GNSSTK_GPSCNAVALM_HPP
#define GNSSTK_GPSCNAVALM_HPP

#include "GPSCNavData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to GPS CNav midi almanac.
   class GPSCNavAlm : public GPSCNavData
   {
   public:
         /// Sets the nav message type.
      GPSCNavAlm();
         /// Create a deep copy of this object.
      NavDataPtr clone() const override
      { return std::make_shared<GPSCNavAlm>(*this); }

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.
          * @return transmit time + 12s or 6s, depending on whether
          *   this is CNAV on L5 or on L2.
          */
      CommonTime getUserTime() const override;

         /** Override dumpHarmonics to hide them in output since GPS
          * CNav almanacs don't contain this data. */
      void dumpHarmonics(std::ostream& s) const override
      {}

         /// Fill the beginFit and endFit values for this object.
      void fixFit();

         /// @note The health flags are true if unhealthy.
      bool healthL1;      ///< L1 signal health from message type 10.
      bool healthL2;      ///< L2 signal health from message type 10.
      bool healthL5;      ///< L5 signal health from message type 10.
      double deltai;      ///< Inclination in rad relative to 0.3*pi rad.
      unsigned wna;       ///< Reference week for toa.
      double toa;         ///< Convenience storage of unqualified toa.
   };

      //@}

}

#endif // GNSSTK_GPSCNAVALM_HPP
