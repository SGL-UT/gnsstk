//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//  
//  This software was developed by Applied Research Laboratories at the 
//  University of Texas at Austin.
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
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

#ifndef GPSTK_CCIR_HPP
#define GPSTK_CCIR_HPP

#include <limits>
#include <vector>
#include "CommonTime.hpp"

// test class forward declaration for friend.
class CCIR_T;

namespace gpstk
{
      /** Wrap the coefficients for the foF2 and M(3000)F2 models
       * recommended by the Comite' Consultatif International des
       * Radiocommunications (CCIR).
       * @note, The CCIR became the ITU-R (International
       * Telecommunication Union Radiocommunication Sector) in 1992,
       * but the data files from which this code was generated are
       * named ccir, so we keep that name for consistency. */
   class CCIR
   {
   public:
         /** Define constants using enums, which avoids the
          * complications of using precompilter macros and also avoids
          * the use of memory that a static const elicits. */
      enum CCIRConsts
      {
         LowSolarActIdx = 0,  ///< Array index for low solar activity condition.
         HighSolarActIdx = 1, ///< Array index for low solar activity condition.
            // dimensions of the CCIR data arrays
         F2SolarActCond = 2,  ///< F2 layer solar activity conditions count.
         F2MaxDegree = 76,    ///< F2 coefficients maximum degree.
         F2MaxOrder = 13,     ///< F2 coefficients maximum order.
         FM3MaxDegree = 49,   ///< FM3 coefficients maximum degree.
         FM3MaxOrder = 9,     ///< FM3 coefficients maximum order.
      };

         /** Initialize internal data to nonsense values to clearly
          * indicate the state of initialization. */
      CCIR();

      void interpolate(const CommonTime& when, double effSunSpots);

         /** Compute Fourier time series for foF2 and M(3000)F2, per
          * section 2.5.5.3.3.
          * @param[in] when The time at which to interpolate the F2
          *   layer coefficients.
          * @param[in] effSunSpots Effective sun spot number, aka Azr. */
      void fourier(const CommonTime& when, double effSunSpots);

      double getCF2(unsigned degree) const
      {
         return (cacheFourierGood && degree < cacheCF2.size()
                 ? cacheCF2[degree]
                 : std::numeric_limits<double>::quiet_NaN());
      }

      double getCM3(unsigned degree) const
      {
         return (cacheFourierGood && degree < cacheCM3.size()
                 ? cacheCM3[degree]
                 : std::numeric_limits<double>::quiet_NaN());
      }

   private:
      void validateCache(const CommonTime& when, double effSunSpots);
         /**
          * @param[in] when The time at which to interpolate the F2
          *   layer coefficients.
          * @param[in] effSunSpots Effective sun spot number, aka Azr. */
      void interpolateF2(const CommonTime& when, double effSunSpots);
      void interpolateFm3(const CommonTime& when, double effSunSpots);

      double ccirF2(unsigned month, int cond, int deg, int ord);
      double ccirFm3(unsigned month, int cond, int deg, int ord);
      double ccir11F2(int cond, int deg, int ord);
      double ccir11Fm3(int cond, int deg, int ord);
      double ccir12F2(int cond, int deg, int ord);
      double ccir12Fm3(int cond, int deg, int ord);
      double ccir13F2(int cond, int deg, int ord);
      double ccir13Fm3(int cond, int deg, int ord);
      double ccir14F2(int cond, int deg, int ord);
      double ccir14Fm3(int cond, int deg, int ord);
      double ccir15F2(int cond, int deg, int ord);
      double ccir15Fm3(int cond, int deg, int ord);
      double ccir16F2(int cond, int deg, int ord);
      double ccir16Fm3(int cond, int deg, int ord);
      double ccir17F2(int cond, int deg, int ord);
      double ccir17Fm3(int cond, int deg, int ord);
      double ccir18F2(int cond, int deg, int ord);
      double ccir18Fm3(int cond, int deg, int ord);
      double ccir19F2(int cond, int deg, int ord);
      double ccir19Fm3(int cond, int deg, int ord);
      double ccir20F2(int cond, int deg, int ord);
      double ccir20Fm3(int cond, int deg, int ord);
      double ccir21F2(int cond, int deg, int ord);
      double ccir21Fm3(int cond, int deg, int ord);
      double ccir22F2(int cond, int deg, int ord);
      double ccir22Fm3(int cond, int deg, int ord);

         /** Month of the interpolated data in cacheF2 so we know when
          * to update. */
      unsigned cacheMonth;
         /** UT hour of the interpolated data in cacheF2 so we know when
          * to update. */
      double cacheHour;
         /** Effective sun spots (Azr) value for data in cacheF2 so we
          * know when to update. */
      double cacheSunSpots;
         /// Cached interpolated F2 coefficients (AF2).
      std::vector<double> cacheF2;
         /// Cached interpolated FM3 coefficients (Am3).
      std::vector<double> cacheFM3;
         /// Cached interpolated CF2 coefficients.
      std::vector<double> cacheCF2;
         /// Cached interpolated Cm3 coefficients.
      std::vector<double> cacheCM3;
         /// Cache state information for interpolated (non-Fourier) data.
      bool cacheGood;
         /// Cache state information for Fourier data.
      bool cacheFourierGood;

      friend class ::CCIR_T;
   };
}

#endif // GPSTK_CCIR_HPP
