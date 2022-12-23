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
#ifndef GNSSTK_CORRECTIONRESULTS_HPP
#define GNSSTK_CORRECTIONRESULTS_HPP

#include "CorrectionResult.hpp"
#include "CorrDupHandling.hpp"

namespace gnsstk
{
      /// @ingroup GNSSsolutions
      //@{

      /** Class for containing a series of computed biases from the
       * GroupPathCorrector classes. */
   class CorrectionResults
   {
   public:
         /// Declaring it explicitly so people don't complain.
      CorrectionResults() = default;
         /** Add a result to the ordered list.
          * @param[in] res The result to be added. */
      void addResult(const CorrectionResult& res);
         /// Empty the contents of #results.
      void clear();
         /// Get the ordered list of #results.
      const CorrectionResultList& getResults() const;
         /** Get the sum of results, filtering duplicates as indicated.
          * @param[in] dups How duplicates are filtered.
          *    * ComputeFirst means the first result of any given type
          *      (e.g. Iono) that is contained within #results will be
          *      incorporated into the final sum, while others of that
          *      type will not.
          *    * UseFirst behaves the same as ComputeFirst
          *    * ComputeLast means the last result of any given type
          *      that is contained within #results will be
          *      incorporated into the final sum, while others of that
          *      type will not.
          * @return The sum of all corrections of unique types. */
      double getCorrSum(CorrDupHandling dups) const;
   private:
      CorrectionResultList results; ///< The computed biases.
   };

      //@}

} // namespace gnsstk

#endif // GNSSTK_CORRECTIONRESULTS_HPP
