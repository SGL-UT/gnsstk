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


#ifndef GG_TROP_MODEL_HPP
#define GG_TROP_MODEL_HPP

#include "TropModel.hpp"

namespace gnsstk
{
   /** Tropospheric model based on Goad and Goodman(1974),
    * "A Modified Hopfield Tropospheric Refraction Correction Model," Paper
    * presented at the Fall Annual Meeting of the American Geophysical Union,
    * San Francisco, December 1974, as presented in Leick, "GPS Satellite Surveying,"
    * Wiley, NY, 1990, Chapter 9 (note particularly Table 9.1).
    */
   class GGTropModel : public TropModel
   {
   public:
         /// Empty constructor
      GGTropModel();

         /** Creates a trop model, with weather observation input
          * @param wx the weather to use for this correction.
          * @throw InvalidParameter
          */
      GGTropModel(const WxObservation& wx);

         /** Create a tropospheric model from explicit weather data
          * @param T temperature in degrees Celsius
          * @param P atmospheric pressure in millibars
          * @param H relative humidity in percent
          * @throw InvalidParameter
          */
      GGTropModel(const double& T,
                  const double& P,
                  const double& H);

         /// @copydoc TropModel::name()
      virtual std::string name()
      { return std::string("GG"); }

         /// @copydoc TropModel::dry_zenith_delay() const
      virtual double dry_zenith_delay() const;

         /// @copydoc TropModel::wet_zenith_delay() const
      virtual double wet_zenith_delay() const;

         /// @copydoc TropModel::dry_mapping_function(double) const
      virtual double dry_mapping_function(double elevation) const;

         /// @copydoc TropModel::wet_mapping_function(double) const
      virtual double wet_mapping_function(double elevation) const;

         /// @copydoc TropModel::setWeather(const double&,const double&,const double&)
      virtual void setWeather(const double& T,
                              const double& P,
                              const double& H);

         /// @copydoc TropModel::setWeather(const WxObservation&)
      virtual void setWeather(const WxObservation& wx);

   private:
      double Cdrydelay;
      double Cwetdelay;
      double Cdrymap;
      double Cwetmap;

   };
}
#endif
