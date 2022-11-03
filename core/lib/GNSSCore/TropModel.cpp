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

#include "TropModel.hpp"
#include "EphemerisRange.hpp"         // for Elevation()
#include "MathBase.hpp"               // SQRT
#include "GNSSconstants.hpp"          // DEG_TO_RAD
#include "GPSEllipsoid.hpp"           // ell.a() = R earth
#include "GNSSconstants.hpp"          // TWO_PI
#include "YDSTime.hpp"


namespace gnsstk
{
      // for temperature conversion from Celcius to Kelvin
   const double TropModel::CELSIUS_TO_KELVIN = 273.15;

   double TropModel::correction(double elevation) const
   {
      THROW_IF_INVALID();

      if(elevation < 0.0)
         return 0.0;

      return (dry_zenith_delay() * dry_mapping_function(elevation)
            + wet_zenith_delay() * wet_mapping_function(elevation));

   }  // end TropModel::correction(elevation)


   double TropModel::correction(const Position& RX,
                                const Position& SV,
                                const CommonTime& tt)
   {
      THROW_IF_INVALID();

      double c;
      try
      {
         c = correction(RX.elevation(SV));
      }
      catch(InvalidTropModel& e)
      {
         GNSSTK_RETHROW(e);
      }
      return c;
   }  // end TropModel::correction(RX,SV,TT)


   void TropModel::setWeather(const double& T,
                              const double& P,
                              const double& H)
   {
      temp = T + CELSIUS_TO_KELVIN;
      press = P;
      humid = H;
      if (temp < 0.0)
      {
         valid = false;
         InvalidParameter e("Invalid temperature.");
         GNSSTK_THROW(e);
      }
      if (press < 0.0)
      {
         valid = false;
         InvalidParameter e("Invalid pressure.");
         GNSSTK_THROW(e);
      }
      if (humid < 0.0 || humid > 105.0)
      {
         valid = false;
         InvalidParameter e("Invalid humidity.");
         GNSSTK_THROW(e);
      }
         /* truncates humidity to 100 if between 105 and 100.
          * models cannot handle supersaturation.
          */
      if (humid > 100)
      {
         humid = 100.0;
      }
   }  // end TropModel::setWeather(T,P,H)


   void TropModel::setWeather(const WxObservation& wx)
   {
      if (wx.isAllValid())
      {
         try
         {
            setWeather(wx.temperature, wx.pressure, wx.humidity);
            valid = true;
         }
         catch(InvalidParameter& e)
         {
            valid = false;
            GNSSTK_RETHROW(e);
         }
      }
      else
      {
         valid = false;
         InvalidParameter e("Invalid weather data.");
         GNSSTK_THROW(e);
      }
   }


   void TropModel::weatherByStandardAtmosphereModel(const double& ht, double& T, double& P, double& H)
   {
         // reference height and it's relate weather(T P H)
      const double h0  = 0.0;			   // meter
      const double Tr  = +18.0;	      // Celsius
      const double pr  = 1013.25;		// millibarc
      const double Hr  = 50;			   // humidity

      T = Tr-0.0065*(ht-h0);
      P = pr * std::pow((1 - 0.0000226 * (ht - h0)), 5.225);
      H = Hr * std::exp(-0.0006396 * (ht - h0));

   }
}
