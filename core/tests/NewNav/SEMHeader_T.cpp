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
#include "SEMHeader.hpp"
#include "TestUtil.hpp"

uint32_t testGPSWeekDisambiguation()
{
   TUDEF("SEMHeader", "Disambiguation");

   std::vector<int32_t> refEpochWeekArray = {1, 2, 511, 512, 513, 1023, gnsstk::GPS_WEEK_PER_EPOCH,
      1025, 1535, 1536, 1537, 2047, 2048, 2049};

   std::vector<int32_t> inputWeekArray =  {0, 10, 20, 400, 500, 511, 513, 600, 900, 1022, 1023, gnsstk::GPS_WEEK_PER_EPOCH,
      1025, 1500, 1535, 1537, 2046, 2047, 2048, 2049};
   
   for (int i = 0; i < inputWeekArray.size(); ++i) {
      for (int j = 0; j < refEpochWeekArray.size(); ++j) {
         int32_t disambiguated_week = gnsstk::detail::getDisAmbiguatedNearbyWeek(refEpochWeekArray[j], inputWeekArray[i]);

         TUASSERTE(int32_t, disambiguated_week & (gnsstk::GPS_WEEK_PER_EPOCH - 1),
            inputWeekArray[i] & (gnsstk::GPS_WEEK_PER_EPOCH - 1));
         
         if (inputWeekArray[i] > (gnsstk::GPS_WEEK_PER_EPOCH - 1) || refEpochWeekArray[j] - (gnsstk::GPS_WEEK_PER_EPOCH / 2) <= 0)
         {
            TUASSERTE(int32_t, inputWeekArray[i], disambiguated_week);
         }
         else
         {
            TUASSERT(disambiguated_week >= refEpochWeekArray[j] - (gnsstk::GPS_WEEK_PER_EPOCH / 2)
               && disambiguated_week <= refEpochWeekArray[j] + (gnsstk::GPS_WEEK_PER_EPOCH / 2));
         }
         // printf("ref:%4d, alm:%4d, disamb_2:%4d \n", refEpochWeekArray[j], inputWeekArray[i], disambiguated_week);
      }
   }
   TURETURN();
}

int main()
{
   unsigned errorTotal = 0;

   errorTotal += testGPSWeekDisambiguation();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}

