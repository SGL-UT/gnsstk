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
#ifndef GNSSTK_GPSLNAVL2CODES_HPP
#define GNSSTK_GPSLNAVL2CODES_HPP

#include <string>
#include "EnumIterator.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Codes on L2 channel, per IS-GPS-200 20.3.3.3.1.2
   enum class GPSLNavL2Codes
   {
      Unknown  =-1, ///< Unknown/Uninitialized value.
      Invalid1 = 0, ///< Not a valid broadcast value.
      Pcode    = 1, ///< P/Y-code is broadcast on L2.
      CAcode   = 2, ///< C/A-code is broadcast on L2.
      Invalid2 = 3, ///< Not a valid broadcast value.
      Last,         ///< Used to verify that all items are described at compile time
   };

      /** Define an iterator so C++11 can do things like
       * for (GPSLNavL2Codes i : GPSLNavL2CodesIterator()) */
   typedef EnumIterator<GPSLNavL2Codes, GPSLNavL2Codes::Unknown, GPSLNavL2Codes::Last> GPSLNavL2CodesIterator;

   namespace StringUtils
   {
         /// Convert GPSLNavL2Codes to a printable string for dump().
      std::string asString(GPSLNavL2Codes e);
   }

} // namespace gnsstk

#endif // GNSSTK_GPSLNAVL2CODES_HPP
