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
#ifndef GPSTK_NEQUICKIONODATA_HPP
#define GPSTK_NEQUICKIONODATA_HPP

#include "IonoData.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to NeQuick ionospheric model.
   class NeQuickIonoData : public IonoData
   {
   public:
         /// Initialize internal data.
      NeQuickIonoData();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override
      { return true; }

         /** Print the contents of this NeQuickIonoData object in a
          * human-readable format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;

         /** Get the ionospheric correction in meters.
          * @param[in] when The time of the observation to correct.
          * @param[in] rxgeo The receiver's geodetic position.
          * @param[in] svgeo The observed satellite's geodetic position.
          * @param[in] band The carrier band of the signal being corrected.
          * @return The ionospheric delay, in meters, on band. */
      double getCorrection(const CommonTime& when,
                           const Position& rxgeo,
                           const Position& svgeo,
                           CarrierBand band) const override;

         // ai terms are solar flux units, solar flux units/degree,
         // solar flux units/degree**2.  Refer to Galileo-OS-SIS-ICD.
      double ai[3]; ///< a_i terms of NeQuick model in solar flux units.
         // These are technically Galileo-specific terms, and they
         // seem to be as yet unused according to the ICD (section
         // 5.1.6, "reserved for future use"), however until such time
         // as we have a situation where we use an NeQuick model that
         // does NOT use this data, I'm leaving it here so it only
         // needs to be defined once.
      bool idf[5]; ///< Ionospheric disturbance flag for regions 1-5 (0-4).

   protected:
         /// Compute l from lambda using eq 6-8, Galileo Ionospheric Model.
      static int getl(double lambda);
         /** get modip from look-up table
          * @param[in] phi Geodetic latitude in degrees.
          * @param[in] lambda Longitude in degrees.
          * @return MODIP mu in degrees. */
      static double getMODIP(double phi, double lambda);
         /** Third order interpolation defined in 2.5.7.1, Galileo
          * Ionospheric Model.
          * @param[in] z1 The data point at -1 in the interpolation.
          * @param[in] z2 The data point at 0 in the interpolation.
          * @param[in] z3 The data point at 1 in the interpolation.
          * @param[in] z4 The data point at 2 in the interpolation.
          * @param[in] x The position to compute the interpolated value [0,1].
          * @return The interpolated value at position x. */
      static double interpolate(double z1, double z2, double z3, double z4,
                                double x);
         /// get slant total electron content
      static double getSTEC();
   };

      //@}

}

#endif // GPSTK_NEQUICKIONODATA_HPP
