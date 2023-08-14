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
#include "GPSSVConfig.hpp"

#include <string>

namespace gnsstk
{
   namespace StringUtils
   {
      std::string asString(GPSSVConfig g) noexcept
      {
         switch (g)
         {
            case GPSSVConfig::noInfo:    return "NoInfo";
            case GPSSVConfig::blockIIR:  return "BlockIIR";
            case GPSSVConfig::blockIIRM: return "BlockIIRM";
            case GPSSVConfig::blockIIF:  return "BlockIIF";
            case GPSSVConfig::blockIII:  return "BlockIII";
            case GPSSVConfig::blockIIIF: return "BlockIIIF";
            default:                     return "???";
         } // switch (g)
      } // asString(GPSSVConfig)


      GPSSVConfig asGPSSVConfig(const std::string& s) noexcept
      {
         if (s == "NoInfo")
            return GPSSVConfig::noInfo;
         if (s == "BlockIIR")
            return GPSSVConfig::blockIIR;
         if (s == "BlockIIRM")
            return GPSSVConfig::blockIIRM;
         if (s == "BlockIIF")
            return GPSSVConfig::blockIIF;
         if (s == "BlockIII")
            return GPSSVConfig::blockIII;
         if (s == "BlockIIIF")
            return GPSSVConfig::blockIIIF;
         return GPSSVConfig::noInfo;
      } // asGPSSVConfig(string)
   } // namespace StringUtils
} // namespace gnsstk
