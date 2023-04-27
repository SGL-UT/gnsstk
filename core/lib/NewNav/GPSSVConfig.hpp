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
#ifndef GNSSTK_GPSSVCONFIG_HPP
#define GNSSTK_GPSSVCONFIG_HPP

#include <cstdint>
#include <string>

#include "EnumIterator.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Identify GPS SV configuration states.
   enum class GPSSVConfig : uint8_t
   {
      noInfo = 0,
      blockIIR = 1,
      blockIIRM = 2,
      blockIIF = 3,
      blockIII = 4,
      blockIIIF = 5,
      last
   };

      /// Define an iterator so C++11 can do range-for loops.
   using GPSSVConfigIterator =
      gnsstk::EnumIterator<GPSSVConfig, GPSSVConfig::noInfo, GPSSVConfig::last>;

   namespace StringUtils
   {
         /// Convert a GPSSVConfig to a whitespace-free string name.
      std::string asString(GPSSVConfig g) noexcept;
         /// Convert a string name to a GPSSVConfig.
      GPSSVConfig asGPSSVConfig(const std::string& s) noexcept;
   }

      //@}

} // namespace gnsstk

#endif // GNSSTK_GPSSVCONFIG_HPP
