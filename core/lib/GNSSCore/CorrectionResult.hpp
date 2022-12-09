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
#ifndef GNSSTK_CORRECTIONRESULT_HPP
#define GNSSTK_CORRECTIONRESULT_HPP

#include <list>
#include <limits>
#include "GroupPathCorrector.hpp"

namespace gnsstk
{
      /// @ingroup GNSSsolutions
      //@{

      /** Class for containing a single computed bias from one of the
       * GroupPathCorrector classes.  This pairs the actual bias with
       * the corrector for the purpose of tracking bias
       * contributions. */
   class CorrectionResult
   {
   public:
         /// Initialize #result to NaN and source to null.
      CorrectionResult()
            : result(std::numeric_limits<double>::quiet_NaN())
      {}
         /** Fully initialize the object with real values.
          * @param[in] r The computed correction value.
          * @param[in] s The GroupPathCorrector that computed this result. */
      CorrectionResult(double r, const GroupPathCorrectorPtr& s)
            : result(r), source(s)
      {}
      double result;                ///< Computed bias in meters.
      GroupPathCorrectorPtr source; ///< Corrector that computed this bias.
   };

      /// Just an ordered list of group path correction results.
   typedef std::list<CorrectionResult> CorrectionResultList;

      //@}

} // namespace gnsstk

#endif // GNSSTK_CORRECTIONRESULT_HPP
