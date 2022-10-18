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


#include "YDSTime.hpp"
#include "SaasTropModel.hpp"

#define THROW_IF_INVALID_DETAILED() {if (!valid) {\
         InvalidTropModel e;                                            \
         if(!validWeather)    e.addText("Invalid trop model: Weather"); \
         if(!validRxHeight)   e.addText("Invalid trop model: Rx Height"); \
         if(!validRxLatitude) e.addText("Invalid trop model: Rx Latitude"); \
         if(!validDOY)        e.addText("Invalid trop model: day of year"); \
         GNSSTK_THROW(e);}}

namespace gnsstk
{
   // constants for wet mapping function
   static const double SaasWetA[5]=
     { 0.00058021897, 0.00056794847, 0.00058118019, 0.00059727542, 0.00061641693 };
   static const double SaasWetB[5]=
     { 0.0014275268, 0.0015138625, 0.0014572752, 0.0015007428, 0.0017599082 };
   static const double SaasWetC[5]=
     { 0.043472961, 0.046729510, 0.043908931, 0.044626982, 0.054736038 };

   // constants for dry mapping function
   static const double SaasDryA[5]=
     { 0.0012769934, 0.0012683230, 0.0012465397, 0.0012196049, 0.0012045996 };
   static const double SaasDryB[5]=
     { 0.0029153695, 0.0029152299, 0.0029288445, 0.0029022565, 0.0029024912 };
   static const double SaasDryC[5]=
     { 0.062610505, 0.062837393, 0.063721774, 0.063824265, 0.064258455 };

   static const double SaasDryA1[5]=
     { 0.0, 0.000012709626, 0.000026523662, 0.000034000452, 0.000041202191 };
   static const double SaasDryB1[5]=
     { 0.0, 0.000021414979, 0.000030160779, 0.000072562722, 0.00011723375 };
   static const double SaasDryC1[5]=
     { 0.0, 0.000090128400, 0.000043497037, 0.00084795348, 0.0017037206 };


   SaasTropModel::SaasTropModel()
   {
      validWeather = false;
      validRxLatitude = false;
      validDOY = false;
      validRxHeight = false;
   } // end SaasTropModel::SaasTropModel()


   SaasTropModel::SaasTropModel(const double& lat,
                                const int& day)
   {
      validWeather = false;
      validRxHeight = false;
      SaasTropModel::setReceiverLatitude(lat);
      SaasTropModel::setDayOfYear(day);
   } // end SaasTropModel::SaasTropModel


   SaasTropModel::SaasTropModel(const double& lat,
                                const int& day,
                                const WxObservation& wx)
   {
      validRxHeight = false;
      SaasTropModel::setReceiverLatitude(lat);
      SaasTropModel::setDayOfYear(day);
      SaasTropModel::setWeather(wx);
   }  // end SaasTropModel::SaasTropModel(weather)


   SaasTropModel::SaasTropModel(const double& lat,
                                const int& day,
                                const double& T,
                                const double& P,
                                const double& H)
   {
      validRxHeight = false;
      SaasTropModel::setReceiverLatitude(lat);
      SaasTropModel::setDayOfYear(day);
      SaasTropModel::setWeather(T,P,H);
   } // end SaasTropModel::SaasTropModel()

      // re-define this to get the throws correct
   double SaasTropModel::correction(double elevation) const
   {
      if(!valid) {
         if(!validWeather) GNSSTK_THROW(
            InvalidTropModel("Invalid Saastamoinen trop model: weather"));
         if(!validRxLatitude) GNSSTK_THROW(
            InvalidTropModel("Invalid Saastamoinen trop model: Rx Latitude"));
         if(!validRxHeight) GNSSTK_THROW(
            InvalidTropModel("Invalid Saastamoinen trop model: Rx Height"));
         if(!validDOY) GNSSTK_THROW(
            InvalidTropModel("Invalid Saastamoinen trop model: day of year"));
         GNSSTK_THROW(
            InvalidTropModel("Valid flag corrupted in Saastamoinen trop model"));
      }

      if(elevation < 0.0) return 0.0;

      double corr=0.0;
      try {
         corr = (dry_zenith_delay() * dry_mapping_function(elevation)
            + wet_zenith_delay() * wet_mapping_function(elevation));
      }
      catch(Exception& e) { GNSSTK_RETHROW(e); }

      return corr;

   }  // end SaasTropModel::correction(elevation)


   double SaasTropModel::correction(const Position& RX,
                                    const Position& SV,
                                    const CommonTime& tt)
   {
      SaasTropModel::setReceiverHeight(RX.getHeight());
      SaasTropModel::setReceiverLatitude(RX.getGeodeticLatitude());
      SaasTropModel::setDayOfYear(int((static_cast<YDSTime>(tt).doy)));

      if(!valid) {
         if(!validWeather) GNSSTK_THROW(
            InvalidTropModel("Invalid Saastamoinen trop model: weather"));
         if(!validRxLatitude) GNSSTK_THROW(
            InvalidTropModel("Invalid Saastamoinen trop model: Rx Latitude"));
         if(!validRxHeight) GNSSTK_THROW(
            InvalidTropModel("Invalid Saastamoinen trop model: Rx Height"));
         if(!validDOY) GNSSTK_THROW(
            InvalidTropModel("Invalid Saastamoinen trop model: day of year"));
         valid = true;
      }

      double corr=0.0;
      try {
         corr = SaasTropModel::correction(RX.elevation(SV));
      }
      catch(Exception& e) { GNSSTK_RETHROW(e); }

      return corr;

   }  // end SaasTropModel::correction(RX,SV,TT)


   double SaasTropModel::correction(const Xvt& RX,
                                    const Xvt& SV,
                                    const CommonTime& tt)
   {
      Position R(RX),S(SV);
      return SaasTropModel::correction(R,S,tt);
   }


   double SaasTropModel::dry_zenith_delay() const
   {
      THROW_IF_INVALID_DETAILED();

      //return (0.0022768*pr/(1-0.00266*::cos(2*lat*DEG_TO_RAD)-0.00028*ht/1000.));
      return SaasDryDelay(press,latitude,height);

   }  // end SaasTropModel::dry_zenith_delay()


   double SaasTropModel::wet_zenith_delay() const
   {
      THROW_IF_INVALID_DETAILED();

      double T = temp+CELSIUS_TO_KELVIN;

      // partial pressure due to water vapor. Leick 4th ed 8.2.4
      double pwv = 0.01 * humid * ::exp(-37.2465+0.213166*T-0.000256908*T*T);
      /* IERS2003 Ch 9 pg 99 - very similar to Leick above
       *double pwv = 0.01*humid
       *      * 0.01*::exp(33.93711047-1.9121316e-2*T+1.2378847e-5*T*T-6.3431645e3/T)
       *      * (1.00062+3.14e-6*press+5.6e-7*temp);
       */

      /* Saastamoinen 1973 Atmospheric correction for the troposphere and
       * stratosphere in radio ranging of satellites. The use of artificial
       * satellites for geodesy, Geophys. Monogr. Ser. 15, Amer. Geophys. Union,
       * pp. 274-251, 1972, modified for gravity as in Davis etal 1985
       */
      return ( 0.002277*((1255/T)+0.05)*pwv /
                  (1-0.00266*::cos(2*latitude*DEG_TO_RAD)-0.00028*height/1000.) );

   }  // end SaasTropModel::wet_zenith_delay()


   double SaasTropModel::dry_mapping_function(double elevation) const
   {
      THROW_IF_INVALID_DETAILED();
      if(elevation < 0.0) return 0.0;

      double lat,t,ct;
      lat = fabs(latitude);         // degrees
      t = doy - 28.;                // mid-winter
      if(latitude < 0)              // southern hemisphere
         t += 365.25/2.;
      t *= 360.0/365.25;            // convert to degrees
      ct = ::cos(t*DEG_TO_RAD);

      double a,b,c;
      if(lat < 15.) {
         a = SaasDryA[0];
         b = SaasDryB[0];
         c = SaasDryC[0];
      }
      else if(lat < 75.) {          // coefficients are for 15,30,45,60,75 deg
         int i=int(lat/15.0)-1;
         double frac=(lat-15.*(i+1))/15.;
         a = SaasDryA[i] + frac*(SaasDryA[i+1]-SaasDryA[i]);
         b = SaasDryB[i] + frac*(SaasDryB[i+1]-SaasDryB[i]);
         c = SaasDryC[i] + frac*(SaasDryC[i+1]-SaasDryC[i]);

         a -= ct * (SaasDryA1[i] + frac*(SaasDryA1[i+1]-SaasDryA1[i]));
         b -= ct * (SaasDryB1[i] + frac*(SaasDryB1[i+1]-SaasDryB1[i]));
         c -= ct * (SaasDryC1[i] + frac*(SaasDryC1[i+1]-SaasDryC1[i]));
      }
      else {
         a = SaasDryA[4] - ct * SaasDryA1[4];
         b = SaasDryB[4] - ct * SaasDryB1[4];
         c = SaasDryC[4] - ct * SaasDryC1[4];
      }

      double se = ::sin(elevation*DEG_TO_RAD);
      double map = (1.+a/(1.+b/(1.+c)))/(se+a/(se+b/(se+c)));

      a = 0.0000253;
      b = 0.00549;
      c = 0.00114;
      map += (height/1000.0)*(1./se-(1+a/(1.+b/(1.+c)))/(se+a/(se+b/(se+c))));

      return map;

   }  // end SaasTropModel::dry_mapping_function()


   double SaasTropModel::wet_mapping_function(double elevation) const
   {
      THROW_IF_INVALID_DETAILED();
      if(elevation < 0.0) return 0.0;

      double a,b,c,lat;
      lat = fabs(latitude);         // degrees
      if(lat < 15.) {
         a = SaasWetA[0];
         b = SaasWetB[0];
         c = SaasWetC[0];
      }
      else if(lat < 75.) {          // coefficients are for 15,30,45,60,75 deg
         int i=int(lat/15.0)-1;
         double frac=(lat-15.*(i+1))/15.;
         a = SaasWetA[i] + frac*(SaasWetA[i+1]-SaasWetA[i]);
         b = SaasWetB[i] + frac*(SaasWetB[i+1]-SaasWetB[i]);
         c = SaasWetC[i] + frac*(SaasWetC[i+1]-SaasWetC[i]);
      }
      else {
         a = SaasWetA[4];
         b = SaasWetB[4];
         c = SaasWetC[4];
      }

      double se = ::sin(elevation*DEG_TO_RAD);
      double map = (1.+a/(1.+b/(1.+c)))/(se+a/(se+b/(se+c)));

      return map;

   }


   void SaasTropModel::setWeather(const double& T,
                                  const double& P,
                                  const double& H)
   {
      temp = T;
      press = P;
         // humid actually stores water vapor partial pressure
      double exp=7.5*T/(T+237.3);
      humid = 6.11 * (H/100.) * std::pow(10.0,exp);

      validWeather = true;
      valid = (validWeather && validRxHeight && validRxLatitude && validDOY);

   }


   void SaasTropModel::setWeather(const WxObservation& wx)
   {
      try
      {
         SaasTropModel::setWeather(wx.temperature,wx.pressure,wx.humidity);
      }
      catch(InvalidParameter& e)
      {
         valid = validWeather = false;
         GNSSTK_RETHROW(e);
      }
   }


   void SaasTropModel::setReceiverHeight(const double& ht)
   {
      height = ht;
      validRxHeight = true;
      valid = (validWeather && validRxHeight && validRxLatitude && validDOY);
   }


   void SaasTropModel::setReceiverLatitude(const double& lat)
   {
      latitude = lat;
      validRxLatitude = true;
      valid = (validWeather && validRxHeight && validRxLatitude && validDOY);
   }


   void SaasTropModel::setDayOfYear(const int& d)
   {
      doy = d;
      if(doy > 0 && doy < 367) validDOY=true; else validDOY = false;
      valid = (validWeather && validRxHeight && validRxLatitude && validDOY);
   }
}
