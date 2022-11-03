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


#ifndef NB_TROP_MODEL_HPP
#define NB_TROP_MODEL_HPP

#include "TropModel.hpp"

namespace gnsstk
{
      /** Tropospheric model developed by University of New Brunswick and
       * described in "A Tropospheric Delay Model for the User of the
       * Wide Area Augmentation System," J. Paul Collins and Richard
       * B. Langley, Technical Report No. 187, Dept. of Geodesy and
       * Geomatics Engineering, University of New Brunswick, 1997. See
       * particularly Appendix C.
       *
       * This model includes a wet and dry component, and was designed
       * for the user without access to measurements of temperature,
       * pressure and relative humidity at ground level. Input of the
       * receiver latitude, day of year and height above the ellipsoid
       * are required, because the mapping functions depend on these
       * quantities. In addition, if the weather (T,P,H) are not
       * explicitly provided, this model interpolates a table of values,
       * using latitude and day of year, to get the ground level weather
       * parameters.
       *
       * Usually, the caller will set the latitude and day of year at the same
       * time the weather is set (if weather is available):
       *
       * @code
       *   NBTropModel nb;
       *   nb.setReceiverLatitude(lat);
       *   nb.setDayOfYear(doy);
       *   nb.setWeather(T,P,H);       // OPTIONAL
       * @endcode
       *
       * Then, when the correction (and/or delay and map) is computed,
       * receiver height should be set before the call to
       * correction(elevation):
       *
       * @code
       *   nb.setReceiverHeight(height);
       *   trop = nb.correction(elevation);
       * @endcode
       *
       * @note In this model, units of 'temp' are degrees Kelvin, and 'humid'
       * is the water vapor partial pressure.
       */
   class NBTropModel : public TropModel
   {
   public:
         /// Empty constructor
      NBTropModel();

         /** Create a trop model using the minimum information:
          * latitude and doy.
          * Interpolate the weather unless setWeather (optional) is called.
          * @param lat Latitude of the receiver in degrees.
          * @param day Day of year.
          * @throw InvalidParameter
          */
      NBTropModel(const double& lat,
                  const int& day);

         /** Create a trop model with weather.
          * @param lat Latitude of the receiver in degrees.
          * @param day Day of year.
          * @param wx the weather to use for this correction.
          * @throw InvalidParameter
          */
      NBTropModel(const double& lat,
                  const int& day,
                  const WxObservation& wx);

         /** Create a tropospheric model from explicit weather data
          * @param lat Latitude of the receiver in degrees.
          * @param day Day of year.
          * @param T temperature in degrees Celsius
          * @param P atmospheric pressure in millibars
          * @param H relative humidity in percent
          * @throw InvalidParameter
          */
      NBTropModel(const double& lat,
                  const int& day,
                  const double& T,
                  const double& P,
                  const double& H);

         /** Create a valid model from explicit input
          * (weather will be estimated internally by this model).
          * @param ht Height of the receiver in meters.
          * @param lat Latitude of the receiver in degrees.
          * @param day Day of year.
          * @throw InvalidParameter
          */
      NBTropModel(const double& ht,
                  const double& lat,
                  const int& day);

         /// @copydoc TropModel::name()
      virtual std::string name()
      { return std::string("NB"); }

         /// @copydoc TropModel::correction(double) const
      virtual double correction(double elevation) const;

         /// @copydoc TropModel::correction(const Position&,const Position&,const CommonTime&)
      virtual double correction(const Position& RX,
                                const Position& SV,
                                const CommonTime& tt);

         /// @copydoc TropModel::correction(const Xvt&,const Xvt&,const CommonTime&)
      virtual double correction(const Xvt& RX,
                                const Xvt& SV,
                                const CommonTime& tt);

         /// @copydoc TropModel::dry_zenith_delay() const
      virtual double dry_zenith_delay() const;

         /// @copydoc TropModel::wet_zenith_delay() const
      virtual double wet_zenith_delay() const;

         /// @copydoc TropModel::dry_mapping_function(double) const
      virtual double dry_mapping_function(double elevation) const;

         /// @copydoc TropModel::wet_mapping_function(double) const
      virtual double wet_mapping_function(double elevation) const;

         /// @copydoc TropModel::setWeather(const WxObservation&)
      virtual void setWeather(const WxObservation& wx);

         /// @copydoc TropModel::setWeather(const double&,const double&,const double&)
      virtual void setWeather(const double& T,
                              const double& P,
                              const double& H);

         /** Configure the model to estimate the weather using lat and doy
          * @throw InvalidTropModel
          */
      void setWeather();

         /// @copydoc TropModel::setReceiverHeight(const double&)
      void setReceiverHeight(const double& ht);

         /// @copydoc TropModel::setReceiverLatitude(const double& lat)
      void setReceiverLatitude(const double& lat);

         /// @copydoc TropModel::setDayOfYear(const int& d)
      void setDayOfYear(const int& d);

   private:
      bool interpolateWeather;      // if true, compute T,P,H from latitude,doy
      double height;                // height (m) of the receiver
      double latitude;              // latitude (deg) of receiver
      int doy;                      // day of year
      bool validWeather;
      bool validRxLatitude;
      bool validRxHeight;
      bool validDOY;
   };

}
#endif
