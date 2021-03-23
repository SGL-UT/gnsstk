//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
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
#ifndef GPSTK_GPSCNAVDATA_HPP
#define GPSTK_GPSCNAVDATA_HPP

#include "OrbitDataKepler.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Class that defines nav message data information that is
       * shared between GPS CNav almanac and ephemeris messages. */
   class GPSCNavData : public OrbitDataKepler
   {
   public:
         /// Aref value defined in IS-GPS-200 (meters).
      static const double refAGPS;
         /// OMEGA dot reference value defined in IS-GPS-200 (radians).
      static const double refOMEGAdotGPS;
         /// inclination offset, this + delta i = i0, defined in IS-GPS-200.
      static const double refioffsetGPS;
         /// delta i reference value defined in IS-GPS-200 (radians).
      static const double refdeltaiGPS;
         /// Argument of perigee reference value (radians, guessed at).
      static const double refwGPS;
         /// Reference eccentricity for reduced almanac in IS-GPS-200.
      static const double refEccGPS;

         /// Aref value defined in IS-QZSS (meters).
      static const double refAQZSS;
         /// OMEGA dot reference value for QZSS (radians).
      static const double refOMEGAdotQZSS;
         /// inclination offset, this + delta i = i0, defined in IS-QZSS.
      static const double refioffsetQZSS;
         /// delta i reference value defined in IS-QZSS (radians).
      static const double refdeltaiQZSS;
         /// Argument of perigee reference value in IS-QZSS (radians).
      static const double refwQZSS;
         /// Reference eccentricity for reduced almanac in IS-QZSS-200.
      static const double refEccQZSS;

         /// Initialize data members.
      GPSCNavData();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

      uint32_t pre; ///< The preamble from the start of the subframe.
      bool alert;   ///< Alert flag
   };

      //@}

}

#endif // GPSTK_GPSCNAVDATA_HPP
