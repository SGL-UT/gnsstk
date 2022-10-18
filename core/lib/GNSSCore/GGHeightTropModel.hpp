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


#ifndef GG_HEIGHT_TROP_MODEL_HPP
#define GG_HEIGHT_TROP_MODEL_HPP

#include "TropModel.hpp"

namespace gnsstk
{
   //---------------------------------------------------------------------------------
   /** Tropospheric model with heights based on Goad and Goodman(1974),
    * "A Modified Hopfield Tropospheric Refraction Correction Model," Paper
    * presented at the Fall Annual Meeting of the American Geophysical Union,
    * San Francisco, December 1974.
    *
    * (Not the same as GGTropModel because this has height dependence, and the
    * computation of this model does not break cleanly into wet and dry components.)
    *
    * @note This model requires heights, both of the weather parameters,
    * and of the receiver. Thus, usually, caller will set heights at the
    * same time the weather is set:
    *
    * @code
    *    GGHeightTropModel ggh;
    *    ggh.setWeather(T,P,H);
    *    ggh.setHeights(hT,hP,hH);
    * @endcode
    *
    * And when the correction (and/or delay and map) is computed,
    * receiver height is set before the call to correction(elevation):
    *
    * @code
    *    ggh.setReceiverHeight(height);
    *    trop = ggh.correction(elevation);
    * @endcode
    *
    * @note setReceiverHeight(ht) sets the 'weather heights' as well, if they are not
    * already defined.
    */
   class GGHeightTropModel : public TropModel
   {
   public:
         /// Empty constructor
      GGHeightTropModel();

         /** Creates a trop model, with weather observation input
          * @param wx the weather to use for this correction.
          * @throw InvalidParameter
          */
      GGHeightTropModel(const WxObservation& wx);

         /** Create a tropospheric model from explicit weather data
          * @param T temperature in degrees Celsius
          * @param P atmospheric pressure in millibars
          * @param H relative humidity in percent
          * @throw InvalidParameter
          */
      GGHeightTropModel(const double& T,
                        const double& P,
                        const double& H);

         /** Create a valid model from explicit input.
          * @param T temperature in degrees Celsius
          * @param P atmospheric pressure in millibars
          * @param H relative humidity in percent
          * @param hT height at which temperature applies in meters.
          * @param hP height at which atmospheric pressure applies in meters.
          * @param hH height at which relative humidity applies in meters.
          * @throw InvalidParameter
          */
      GGHeightTropModel(const double& T,
                        const double& P,
                        const double& H,
                        const double hT,
                        const double hP,
                        const double hH);

         /// @copydoc TropModel::name()
      virtual std::string name()
      { return std::string("GGHeight"); }

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

         /// @copydoc TropModel::setWeather(const double&,const double&,const double&)
      virtual void setWeather(const double& T,
                              const double& P,
                              const double& H);

         /// @copydoc TropModel::setWeather(const WxObservation&)
      virtual void setWeather(const WxObservation& wx);

         /** Re-define the heights at which the weather parameters apply.
          *  Typically called whenever setWeather is called.
          *  @param hT height at which temperature applies in meters.
          *  @param hP height at which atmospheric pressure applies in meters.
          *  @param hH height at which relative humidity applies in meters.
          */
      void setHeights(const double& hT,
                      const double& hP,
                      const double& hH);

         /// @copydoc TropModel::setReceiverHeight(const double&)
      void setReceiverHeight(const double& ht);

   private:
      double height;                ///< height (m) of the receiver
      double htemp;                 ///< height (m) at which temp applies
      double hpress;                ///< height (m) at which press applies
      double hhumid;                ///< height (m) at which humid applies
      bool validWeather;            ///< flag for valid weather
      bool validHeights;            ///< flag for valid height
      bool validRxHeight;           ///< flag for valid receiver height
   };
}

#endif
