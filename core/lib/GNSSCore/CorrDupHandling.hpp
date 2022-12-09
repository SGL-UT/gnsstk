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
#ifndef GNSSTK_CORRDUPHANDLING_HPP
#define GNSSTK_CORRDUPHANDLING_HPP

#include <string>
#include <set>
#include "EnumIterator.hpp"

namespace gnsstk
{
      /// @ingroup GNSSsolutions
      //@{

      /** Enumeration for defining how duplicat GroupPathCorrector
       * types are handled by GroupPathCorr and CorrectionResults.
       * The final sum will only ever use one correction of a given
       * CorrectorType, and it will depend on the order that the
       * corrector objects are added to GroupPathCorr::calcs.  This
       * enum also will tell GroupPathCorr whether to compute the
       * biases of all of the correctors in calcs, or stop computing,
       * for example, ISCs once the first ISC-type corrector yields a
       * valid correction. */
   enum class CorrDupHandling
   {
      Unknown,      ///< Duplicate handling is uninitialized.
      ComputeFirst, ///< Sum includes first valid correction, no duplication.
      ComputeLast,  ///< Sum includes last valid correction of a type.
      UseFirst,     ///< Sum includes first valid correction of type, all computed.
      Last          ///< Used to create an iterator.
   };

      /** Define an iterator so C++11 can do things like
       * for (CorrDupHandling i : CorrDupHandlingIterator()) */
   typedef EnumIterator<CorrDupHandling, CorrDupHandling::Unknown, CorrDupHandling::Last> CorrDupHandlingIterator;
      /// Set of message types, used by NavLibrary and NavDataFactory.
   typedef std::set<CorrDupHandling> CorrDupHandlingSet;

   namespace StringUtils
   {
         /// Convert a CorrDupHandling to a whitespace-free string name.
      std::string asString(CorrDupHandling e) noexcept;
         /// Convert a string name to an CorrDupHandling
      CorrDupHandling asCorrDupHandling(const std::string& s) noexcept;
   }

      //@}

}

#endif // GNSSTK_CORRDUPHANDLING_HPP
