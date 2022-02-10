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

#ifndef GNSSTK_SATMETADATASORT_HPP
#define GNSSTK_SATMETADATASORT_HPP

#include <functional>
#include "SatMetaData.hpp"

namespace gnsstk
{
      /** Provide a sorting algorithm for SatMetaData to be used by
       * SatMetaDataStore and derivatives to minimize the amount of
       * time spent looking for data.
       * @note This sort algorithm assumes that it is sorting
       *   satellites of the same system, which is how they're stored
       *   in SatMetaDataStore. */
   struct SatMetaDataSort
   {
      bool operator()(const SatMetaData& lhs, const SatMetaData& rhs) const
      {
         if (lhs.prn < rhs.prn)
            return true;
         if (lhs.prn > rhs.prn)
            return false;
         if (lhs.chl < rhs.chl)
            return true;
         if (lhs.chl > rhs.chl)
            return false;
         if (lhs.slotID < rhs.slotID)
            return true;
         if (lhs.slotID > rhs.slotID)
            return false;
         if (lhs.startTime < rhs.startTime)
            return true;
         return false;
      }
   };

} // namespace gnsstk

#endif // GNSSTK_SATMETADATASORT_HPP
