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
#ifndef GPSTK_GPSCNAV2ALM_HPP
#define GPSTK_GPSCNAV2ALM_HPP

#include "OrbitDataKepler.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to GPS CNav2 midi almanac.
   class GPSCNav2Alm : public OrbitDataKepler
   {
   public:
         /** Midi almanac inclination offset, this + delta i = i0,
          * defined in IS-GPS-800. */
      static const double refi0GPS;
         /** Midi almanac inclination offset, this + delta i = i0,
          * defined in IS-QZSS-PNT-004. */
      static const double refi0QZSS;

         /// Sets the nav message type.
      GPSCNav2Alm();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** This is just a method for making the dump output say
          * "Almanac" vs "Ephemeris" when appropriate. */
      std::string getDataType() const override
      { return "Almanac"; }
         /** Override dumpHarmonics to hide them in output since GPS
          * CNav2 almanacs don't contain this data. */
      void dumpHarmonics(std::ostream& s) const override
      {}

         /// Fill the beginFit and endFit values for this object.
      void fixFit();

         /// @note The health flags are true if unhealthy.
      bool healthL1;      ///< L1 signal health.
      bool healthL2;      ///< L2 signal health.
      bool healthL5;      ///< L5 signal health.
      double deltai;      ///< Inclination in rad relative to 0.3*pi rad.
      unsigned wna;       ///< Reference week for toa.
      double toa;         ///< Convenience storage of unqualified toa.
   };

      //@}

}

#endif // GPSTK_GPSCNAV2ALM_HPP
