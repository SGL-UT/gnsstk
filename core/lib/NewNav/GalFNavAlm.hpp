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
#ifndef GNSSTK_GALFNAVALM_HPP
#define GNSSTK_GALFNAVALM_HPP

#include "OrbitDataGal.hpp"
#include "GalHealthStatus.hpp"
#include "gnsstk_export.h"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to Galileo F/NAV almanacs.
   class GalFNavAlm : public OrbitDataGal
   {
   public:
         /// A ref value defined in OS-SIS-ICD Table 75 (meters).
      GNSSTK_EXPORT static const double refA;
         /// inclination offset, refioffset + delta i = i0, defined in OS-SIS-ICD.
      GNSSTK_EXPORT static const double refioffset;

         /// Sets the nav message type.
      GalFNavAlm();
         /// Create a deep copy of this object.
      NavDataPtr clone() const override
      { return std::make_shared<GalFNavAlm>(*this); }

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
          * @return transmit time of most recent message type of the pair + 10s.
          */
      CommonTime getUserTime() const override;

         /** Override dumpHarmonics to hide them in output since
          * Galileo F/NAV almanacs don't contain this data. */
      void dumpHarmonics(std::ostream& s) const override
      {}

         /// Fill the beginFit and endFit values for this object.
      void fixFit();

         /** Set the SVHealth value "health" according to the health
          * status variable hsE5a. */
      void fixHealth();

         /** Dump SV status information (e.g. health).
          * @param[in,out] s The stream to write the data to. */
      void dumpSVStatus(std::ostream& s) const override;

         /** Transmit time of the second page type used in constructing
          * the almanac, or the same as xmitTime if not split.  That is:
          * SVID | xmitTime page type | xmit2 page type
          * ---- | ------------------ | ---------------
          * 1    | 5                  | 5
          * 2    | 5                  | 6
          * 3    | 6                  | 6
          */
      CommonTime xmit2;
      double dAhalf;         ///< delta sqrt(A)
      double deltai;         ///< Inclination in rad relative to 0.3*pi rad.
      unsigned wna;          ///< Reference week for t0a.
      double t0a;            ///< Convenience storage of unqualified t0a.
      uint8_t ioda5;         ///< IODa for page type 5.
      uint8_t ioda6;         ///< IODa for page type 6.
      GalHealthStatus hsE5a; ///< Health status for E5a.
   };

      //@}

}

#endif // GNSSTK_GALFNAVALM_HPP
