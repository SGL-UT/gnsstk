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
#ifndef GNSSTK_GLOFNAVPCODE_HPP
#define GNSSTK_GLOFNAVPCODE_HPP

#include <string>
#include "EnumIterator.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Values for GLONASS FDMA nav message, Word P.  The values
       * indicate whether the tau_c parameter and tau_GPS parameter
       * are relayed from the control segment or calculated
       * on-board the GLONASS-M satellite.
       * @note This seems to imply P has no meaning on legacy
       *   GLONASS, only in GLONASS-M. */
   enum class GLOFNavPCode
   {
      Unknown      =-1, ///< Unknown/Uninitialized value.
      CRelGPSRel   = 0, ///< C parameter relayed, GPS parameter relayed
      CRelGPSCalc  = 1, ///< C parameter relayed, GPS parameter calculated
      CCalcGPSRel  = 2, ///< C parameter calculated, GPS parameter relayed
      CCalcGPSCalc = 3, ///< C parameter calculated, GPS parameter calculated
      Last,             ///< Used to verify that all items are described at compile time
   };

      /** Define an iterator so C++11 can do things like
       * for (GLOFNavPCode i : GLOFNavPCodeIterator()) */
   typedef EnumIterator<GLOFNavPCode, GLOFNavPCode::Unknown, GLOFNavPCode::Last> GLOFNavPCodeIterator;

   namespace StringUtils
   {
         /// Convert GLOFNavPCode to a printable string for dump().
      std::string asString(GLOFNavPCode e);
   }

      //@}

} // namespace gnsstk

#endif // GNSSTK_GLOFNAVPCODE_HPP
