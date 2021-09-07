//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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
#ifndef GNSSTK_BDSD1NAVALM_HPP
#define GNSSTK_BDSD1NAVALM_HPP

#include "BDSD1NavData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to BeiDou D1 almanac pages.
   class BDSD1NavAlm : public BDSD1NavData
   {
   public:
         /// Sets the nav message type.
      BDSD1NavAlm();

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

         /** Override dumpHarmonics to hide them in output since BeiDou
          * almanacs don't contain this data. */
      void dumpHarmonics(std::ostream& s) const override
      {}

         /// Fill the beginFit and endFit values for this object.
      void fixFit();

         /** Dump SV status information (e.g. health).
          * @param[in,out] s The stream to write the data to. */
      void dumpSVStatus(std::ostream& s) const override;

      uint8_t pnum;        ///< Almanac page number.
      double deltai;       ///< Inclination in rad relative to 0.3*pi rad.
      double toa;          ///< Convenience storage of unqualified toa.
      uint16_t healthBits; ///< 9 bits of health from sf5,pg7/8.
      bool isDefault;      ///< True if the source page was all 0s for orbit.
   };

      //@}

}

#endif // GNSSTK_BDSD1NAVALM_HPP
