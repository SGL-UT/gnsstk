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

#ifndef GNSSTK_REFERENCE_FRAME_HPP
#define GNSSTK_REFERENCE_FRAME_HPP

#include <iostream>
#include <string>
#include "EnumIterator.hpp"

namespace gnsstk
{

      /** ECEF reference systems or frames.
       * @deprecated This enum is deprecated as of Dec 2022 and should
       *   not be used.  Use RefFrame instead. */
   enum class ReferenceFrame
   {
         // add new frames BEFORE count, then add to
         // asStrings/asReferenceFrame in ReferenceFrame.cpp and make
         // parallel to this enum.

         // Unknown MUST BE FIRST, and MUST = 0
      Unknown=0,  ///< unknown frame
      WGS84,      ///< WGS84, assumed to be the latest version
      WGS84G730,  ///< WGS84, GPS week 730 version
      WGS84G873,  ///< WGS84, GPS week 873 version
      WGS84G1150, ///< WGS84, GPS week 1150 version
      WGS84G1674, ///< WGS84, GPS week 1674 version
      WGS84G1762, ///< WGS84, GPS week 1762 version
      ITRF,       ///< ITRF, assumed to be the latest version
      ITRF94,     ///< ITRF, 1994 version
      ITRF96,     ///< ITRF, 1996 version
      ITRF97,     ///< ITRF, 1997 version
      ITRF2000,   ///< ITRF, 2000 version
      ITRF2005,   ///< ITRF, 2005 version
      ITRF2008,   ///< ITRF, 2008 version
      ITRF2014,   ///< ITRF, 2014 version
      PZ90,       ///< PZ90 (GLONASS)
      PZ90KGS,    ///< PZ90 the "original"
      CGCS2000,   ///< CGCS200 (BDS)
         // Last MUST BE LAST
      Last        ///< Used to verify that all items are described at compile time
   };

      /** Define an iterator so C++11 can do things like
       * for (ReferenceFrame i : ReferenceFrameIterator()) */
   typedef EnumIterator<ReferenceFrame, ReferenceFrame::Unknown, ReferenceFrame::Last> ReferenceFrameIterator;

   namespace StringUtils
   {
         /// Convert a ReferenceFrame enum to its string representation.
      std::string asString(ReferenceFrame e);
         /// Convert a string representation of ReferenceFrame to an enum.
      ReferenceFrame asReferenceFrame(const std::string& s);
   }

      /** Write name (asString()) of a ReferenceFrame to an output stream.
       * @param[in,out] os the output stream
       * @param[in] f the ReferenceFrame to be written
       * @return The reference to the ostream passed to this operator. */
   inline std::ostream& operator<<(std::ostream& os, ReferenceFrame f)
   { return os << StringUtils::asString(f); }

}   // end of gnsstk namespace

#endif // GNSSTK_REFERENCE_FRAME_HPP
