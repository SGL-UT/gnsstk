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


#ifndef SAAS_TROP_MODEL_HPP
#define SAAS_TROP_MODEL_HPP

#include "TropModel.hpp"

namespace gnsstk
{
      /** Saastamoinen tropospheric model based on Saastamoinen, J.,
       * 'Atmospheric Correction for the Troposphere and Stratosphere
       * in Radio Ranging of Satellites,' Geophysical Monograph 15,
       * American Geophysical Union, 1972, and Ch 9 of McCarthy, D and
       * Petit, G, IERS Conventions (2003), IERS Technical Note 32,
       * IERS, 2004. The mapping functions are from Neill, A.E., 1996,
       * 'Global Mapping Functions for the Atmosphere Delay of Radio
       * Wavelengths,' J. Geophys. Res., 101, pp. 3227-3246 (also see
       * IERS TN 32).
       *
       * This model includes a wet and dry component, and requires
       * input of the geodetic latitude, day of year and height above
       * the ellipsoid of the receiver.
       *
       * Usually, the caller will set the latitude and day of year at the same
       * time the weather is set
       * @code
       *   SaasTropModel stm;
       *   stm.setReceiverLatitude(lat);
       *   stm.setDayOfYear(doy);
       *   stm.setWeather(T,P,H);
       * @endcode
       * Then, when the correction (and/or delay and map) is computed,
       * receiver height should be set before the call to
       * correction(elevation):
       * @code
       *   stm.setReceiverHeight(height);
       *   trop_corr = stm.correction(elevation);
       * @endcode
       *
       * @note in this model, units of 'temp' are degrees Celsius and
       * humid actually stores water vapor partial pressure in mbars
       */
   class SaasTropModel : public TropModel
   {
   public:
         /// Empty constructor
      SaasTropModel();

         /** Create a trop model using the minimum information:
          * latitude and doy.
          * @param lat Latitude of the receiver in degrees.
          * @param day Day of year.
          */
      SaasTropModel(const double& lat,
                    const int& day);

         /** Create a trop model with weather.
          * @param lat Latitude of the receiver in degrees.
          * @param day Day of year.
          * @param wx the weather to use for this correction.
          * @throw InvalidParameter
          */
      SaasTropModel(const double& lat,
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
      SaasTropModel(const double& lat,
                    const int& day,
                    const double& T,
                    const double& P,
                    const double& H);

         /// @copydoc TropModel::name()
      virtual std::string name()
      { return std::string("Saas"); }

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

         /// @copydoc TropModel::wet_mapping_function(double elevation) const
      virtual double wet_mapping_function(double elevation) const;

         /// @copydoc TropModel::setWeather(const WxObservation&)
      virtual void setWeather(const WxObservation& wx);

         /// @copydoc TropModel::setWeather(const double&,const double&,const double&)
      virtual void setWeather(const double& T,
                              const double& P,
                              const double& H);

         /// @copydoc TropModel::setReceiverHeight(const double& ht)
      void setReceiverHeight(const double& ht);

         /// @copydoc TropModel::setReceiverLatitude(const double&)
      void setReceiverLatitude(const double& lat);

         /// @copydoc TropModel::setDayOfYear(const int&)
      void setDayOfYear(const int& d);

   private:
      double height;             ///< height (m) of the receiver above the geoid
      double latitude;           ///< latitude (deg) of receiver
      int doy;                   ///< day of year
      bool validWeather;         ///< flag for valid weather
      bool validRxLatitude;      ///< flag for valid Rx latitude
      bool validRxHeight;        ///< flag for valid Rx longitude
      bool validDOY;             ///< flag for valid day of year
   };

}
#endif
