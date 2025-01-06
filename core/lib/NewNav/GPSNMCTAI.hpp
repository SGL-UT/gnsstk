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
#ifndef GNSSTK_GPSNMCTAI_HPP
#define GNSSTK_GPSNMCTAI_HPP

#include <cstdint>
#include <string>

#include "EnumIterator.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Identifies values of the Availability Indicator (AI) of GPS LNav NMCT
       * 
       * Explanation of values in IS-GPS-200 section "20.3.3.5.1.9 NMCT"
       */
   enum class GPSNMCTAI : uint8_t
   {
      Unencrypted = 0,
      Encrypted = 1,
      NotAvailable = 2,
      Reserved = 3,
      Last
   };

      /// Define an iterator so C++11 can do range-for loops.
   using GPSNMCTAIIterator =
      gnsstk::EnumIterator<GPSNMCTAI, GPSNMCTAI::Unencrypted, GPSNMCTAI::Last>;

   namespace StringUtils
   {
         /// Convert a GPSNMCTAI to a whitespace-free string name.
      std::string asString(GPSNMCTAI g) noexcept;
         /// Convert a string name to a GPSNMCTAI.
      GPSNMCTAI asGPSNMCTAI(const std::string& s) noexcept;
   }

      //@}

} // namespace gnsstk

#endif // GNSSTK_GPSNMCTAI_HPP
