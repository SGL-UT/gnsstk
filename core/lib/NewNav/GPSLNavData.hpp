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
#ifndef GNSSTK_GPSLNAVDATA_HPP
#define GNSSTK_GPSLNAVDATA_HPP

#include "OrbitDataKepler.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class that defines nav message data information that is
       * shared between GPS LNav almanac and ephemeris messages.
       * @note The data in this class should represent subframe 1's
       *   data in the case of an ephemeris, or the single subframe
       *   for almanac data.
       * @see GPSLNavEph for data from subframes 2 and 3. */
   class GPSLNavData : public OrbitDataKepler
   {
   public:
         /// inclination offset, this + delta i = i0, defined in IS-GPS-200.
      static const double refioffsetGPS;

         /** @todo figure out when to use QZO iref and when to use
          * GEO.  According to table 3.2.1-1 the PRN assignments are
          * mostly defined, but PRN 198 and 202 are in satellite
          * category "QZO/GEO" so it's not 100% clear. */
         /// inclination offset, this + delta i = i0, defined in IS-QZSS.
      static const double refioffsetQZSS;
         /// Reference eccentricity for reduced almanac in IS-QZSS-200.
      static const double refEccQZSS;

         /// Initialize data members.
      GPSLNavData();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

      uint32_t pre; ///< The TLM preamble from word 1 of the subframe.
      uint32_t tlm; ///< The TLM message from word 1 of the subframe.
      bool alert;   ///< Alert flag from HOW
      bool asFlag;  ///< Anti-spoof flag from HOW
   };

      //@}

}

#endif // GNSSTK_GPSLNAVDATA_HPP
