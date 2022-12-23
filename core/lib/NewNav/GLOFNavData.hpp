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
#ifndef GNSSTK_GLOFNAVDATA_HPP
#define GNSSTK_GLOFNAVDATA_HPP

#include "OrbitData.hpp"
#include "NavFit.hpp"
#include "SVHealth.hpp"
#include "GLOFNavPCode.hpp"
#include "GLOFNavSatType.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements shared between GLONASS Civil F-Nav
       * ephemerides and almanacs. */
   class GLOFNavData : public OrbitData, public NavFit
   {
   public:
         /// Sets the nav message type and all other data members to 0.
      GLOFNavData();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

      CommonTime xmit2;   ///< Transmit time for string 2 (eph) or odd string.
      GLOFNavSatType satType; ///< Satellite type (M_n: GLONASS or GLONASS-M).
      unsigned slot;      ///< Slot number (n).
      bool lhealth;       ///< Health flag? Different from B_n and C_n?
      SVHealth health;    ///< SV health status.
   };

      //@}

}

#endif // GNSSTK_GLOFNAVDATA_HPP
