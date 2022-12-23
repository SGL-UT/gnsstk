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

#include "CorrectionResults.hpp"

namespace gnsstk
{
   void CorrectionResults ::
   addResult(const CorrectionResult& res)
   {
      results.push_back(res);
   }


   const CorrectionResultList& CorrectionResults ::
   getResults() const
   {
      return results;
   }


   double CorrectionResults ::
   getCorrSum(CorrDupHandling dups) const
   {
      double rv = 0.0;
         // Collected results (intermediate stage for dealing with duplicates)
      std::map<CorrectorType, double> coll;
      for (const auto& res : results)
      {
         switch (dups)
         {
               // Computation is already done at this point, so treat
               // ComputeFirst and UseFirst the same here.
            case CorrDupHandling::ComputeFirst:
            case CorrDupHandling::UseFirst:
               if (coll.find(res.source->corrType) == coll.end())
               {
                     // This is the first result of this type, so add it.
                  coll[res.source->corrType] = res.result;
               }
               break;
            case CorrDupHandling::ComputeLast:
                  // Just add the result, and it will automatically
                  // end up with the last result in the map.
               coll[res.source->corrType] = res.result;
               break;
            default:
            {
               InvalidParameter exc("Invalid CorrDupHandling value: " +
                                    StringUtils::asString((int)dups));
               GNSSTK_THROW(exc);
            }
         }
      }
      if (coll.empty())
      {
            // no data, so return nan.
         return std::numeric_limits<double>::quiet_NaN();
      }
      for (const auto& ci : coll)
      {
         rv += ci.second;
      }
      return rv;
   }


   void CorrectionResults ::
   clear()
   {
      results.clear();
   }

} // namespace gnsstk
