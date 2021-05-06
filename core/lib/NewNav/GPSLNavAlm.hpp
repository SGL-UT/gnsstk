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
#ifndef GPSTK_GPSLNAVALM_HPP
#define GPSTK_GPSLNAVALM_HPP

#include "GPSLNavData.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to GPS LNav almanac pages.
   class GPSLNavAlm : public GPSLNavData
   {
   public:
         /// Sets the nav message type.
      GPSLNavAlm();

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
          * LNav almanacs don't contain this data. */
      void dumpHarmonics(std::ostream& s) const override
      {}

         /// Fill the beginFit and endFit values for this object.
      void fixFit();

         /** Dump SV status information (e.g. health).
          * @param[in,out] s The stream to write the data to. */
      void dumpSVStatus(std::ostream& s) const override;

      uint8_t healthBits; ///< 8 SV health bits.
      double deltai;      ///< Inclination in rad relative to 0.3*pi rad.
      double toa;         ///< Convenience storage of unqualified toa.
   };

      //@}

}

#endif // GPSTK_GPSLNAVALM_HPP
