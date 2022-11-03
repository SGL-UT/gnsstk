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
#include "MOPSTropModel.hpp"

#define THROW_IF_INVALID_DETAILED() {if (!valid) {                   \
         InvalidTropModel e;                                            \
         if(!validLat) e.addText("Invalid trop model: validLat");       \
         if(!validHeight)  e.addText("Invalid trop model: validHeight"); \
         if(!validTime)   e.addText("Invalid trop model: day of year"); \
         GNSSTK_THROW(e);}}

namespace gnsstk
{
      // Some specific constants
   static const double MOPSg=9.80665;
   static const double MOPSgm=9.784;
   static const double MOPSk1=77.604;
   static const double MOPSk2=382000.0;
   static const double MOPSRd=287.054;


   MOPSTropModel::MOPSTropModel()
   {
      validHeight = false;
      validLat    = false;
      validTime   = false;
      valid       = false;
   }


   MOPSTropModel::MOPSTropModel( const double& ht,
                                 const double& lat,
                                 const int& doy )
   {
      setReceiverHeight(ht);
      setReceiverLatitude(lat);
      setDayOfYear(doy);
   }


   MOPSTropModel::MOPSTropModel(const Position& RX, const CommonTime& time)
   {
      setReceiverHeight(RX.getAltitude());
      setReceiverLatitude(RX.getGeodeticLatitude());
      setDayOfYear(time);
   }


   double MOPSTropModel::correction(double elevation) const
   {
      THROW_IF_INVALID_DETAILED();

      if(elevation < 5.0) return 0.0;

      double map = MOPSTropModel::mapping_function(elevation);

         // Compute tropospheric delay
      double tropDelay = ( MOPSTropModel::dry_zenith_delay() +
                           MOPSTropModel::wet_zenith_delay() ) * map;

      return tropDelay;

   }


   double MOPSTropModel::correction( const Position& RX,
                                     const Position& SV )
   {
      try
      {
         setReceiverHeight( RX.getAltitude() );
         setReceiverLatitude(RX.getGeodeticLatitude());
         setWeather();
      }
      catch(GeometryException& e)
      {
         valid = false;
      }

      if(!valid) throw InvalidTropModel("Invalid model");

      double c;
      try
      {
         c = MOPSTropModel::correction(RX.elevationGeodetic(SV));
      }
      catch(InvalidTropModel& e)
      {
         GNSSTK_RETHROW(e);
      }

      return c;

   }


   double MOPSTropModel::correction( const Position& RX,
                                     const Position& SV,
                                     const CommonTime& tt )
   {
      setDayOfYear(tt);

      return MOPSTropModel::correction(RX,SV);
   }


   double MOPSTropModel::correction( const Position& RX,
                                     const Position& SV,
                                     const int& doy )
   {
      setDayOfYear(doy);

      return MOPSTropModel::correction(RX,SV);
   }


   double MOPSTropModel::correction( const Xvt& RX,
                                     const Xvt& SV )
   {
      Position R(RX),S(SV);

      return MOPSTropModel::correction(R,S);
   }


   double MOPSTropModel::correction( const Xvt& RX,
                                     const Xvt& SV,
                                     const CommonTime& tt )
   {
      setDayOfYear(tt);
      Position R(RX),S(SV);

      return MOPSTropModel::correction(R,S);
   }


   double MOPSTropModel::correction( const Xvt& RX,
                                     const Xvt& SV,
                                     const int& doy )
   {
      setDayOfYear(doy);
      Position R(RX),S(SV);

      return MOPSTropModel::correction(R,S);
   }


   double MOPSTropModel::dry_zenith_delay() const
   {
      THROW_IF_INVALID();

      double ddry, zh_dry, exponent;

         // Set the extra parameters
      double P = MOPSParameters(0);
      double T = MOPSParameters(1);
      double beta = MOPSParameters(3);

         // Zero-altitude dry zenith delay:
      zh_dry = 0.000001*(MOPSk1*MOPSRd)*P/MOPSgm;

         /* Zenith delay terms at MOPSHeight meters of height above mean sea
          * level
          */
      exponent = MOPSg/MOPSRd/beta;
      ddry = zh_dry * std::pow( (1.0 - beta*MOPSHeight/T), exponent );

      return ddry;

   }


   double MOPSTropModel::wet_zenith_delay() const
   {
      THROW_IF_INVALID();

      double dwet, zh_wet, exponent;

         // Set the extra parameters
      double T = MOPSParameters(1);
      double e = MOPSParameters(2);
      double beta = MOPSParameters(3);
      double lambda = MOPSParameters(4);

         // Zero-altitude wet zenith delay:
      zh_wet = (0.000001*MOPSk2)*MOPSRd/(MOPSgm*(lambda+1.0)-beta*MOPSRd)*e/T;

         /* Zenith delay terms at MOPSHeight meters of height above mean sea
          * level
          */
      exponent = ( (lambda+1.0)*MOPSg/MOPSRd/beta)-1.0;
      dwet= zh_wet * std::pow( (1.0 - beta*MOPSHeight/T), exponent );

      return dwet;

   }  // end MOPSTropModel::wet_zenith_delay()


   void MOPSTropModel::setWeather()
   {
      if(!validLat)
      {
         valid = false;
         InvalidTropModel e(
            "MOPSTropModel must have Rx latitude before computing weather");
         GNSSTK_THROW(e);
      }

      if(!validTime)
      {
         valid = false;
         InvalidTropModel e(
            "MOPSTropModel must have day of year before computing weather");
         GNSSTK_THROW(e);
      }

         /* In order to compute tropospheric delay we need to compute some
          * extra parameters
          */
      try
      {
         prepareParameters();
      }
      catch(InvalidTropModel& e)
      {
         GNSSTK_RETHROW(e);
      }

      valid = validHeight && validLat && validTime;
   }


   void MOPSTropModel::setReceiverHeight(const double& ht)
   {
      MOPSHeight = ht;
      validHeight = true;

         // Change the value of field "valid" if everything is already set
      valid = validHeight && validLat && validTime;

         // If model is valid, set the appropriate parameters
      if (valid) setWeather();
   }


   void MOPSTropModel::setReceiverLatitude(const double& lat)
   {
      MOPSLat = lat;
      validLat = true;

         // Change the value of field "valid" if everything is already set
      valid = validHeight && validLat && validTime;

         // If model is valid, set the appropriate parameters
      if (valid) setWeather();
   }


   void MOPSTropModel::setDayOfYear(const int& doy)
   {

      if ( (doy>=1) && (doy<=366))
      {
         validTime = true;
      }
      else
      {
         validTime = false;
      }

      MOPSTime = doy;

         // Change the value of field "valid" if everything is already set
      valid = validHeight && validLat && validTime;

         // If model is valid, set the appropriate parameters
      if (valid) setWeather();
   }


   void MOPSTropModel::setDayOfYear(const CommonTime& time)
   {
      MOPSTime = (int)(static_cast<YDSTime>(time)).doy;
      validTime = true;

         // Change the value of field "valid" if everything is already set
      valid = validHeight && validLat && validTime;

         // If model is valid, set the appropriate parameters
      if (valid) setWeather();
   }


   void MOPSTropModel::setAllParameters( const CommonTime& time,
                                         const Position& rxPos )
   {
      MOPSTime = (int)(static_cast<YDSTime>(time)).doy;
      validTime = true;
      MOPSLat = rxPos.getGeodeticLatitude();
      validLat = true;
      MOPSHeight = rxPos.getHeight();
      validHeight = true;

         // Change the value of field "valid" if everything is already set
      valid = validHeight && validLat && validTime;

         // If model is valid, set the appropriate parameters
      if (valid) setWeather();
   }


   double MOPSTropModel::MOPSsigma2(double elevation)
   {
      double map_f;

         /* If elevation is below bounds, fail in a sensible way returning a
          * very big sigma value
          */
      if(elevation < 5.0)
      {
         return 9.9e9;
      }
      else
      {
         map_f = MOPSTropModel::mapping_function(elevation);
      }

         // Compute residual error for tropospheric delay
      double MOPSsigma2trop = (0.12*map_f)*(0.12*map_f);

      return MOPSsigma2trop;
   }


   void MOPSTropModel::prepareParameters()
   {
      THROW_IF_INVALID();

      try
      {
            // We need to read some data
         prepareTables();

            // Declare some variables
         int idmin, j, index;
         double fact, axfi;
         Vector<double> avr0(5);
         Vector<double> svr0(5);

            // Resize MOPSParameters as appropriate
         MOPSParameters.resize(5);

         if (MOPSLat >= 0.0)
         {
            idmin = 28;
         }
         else
         {
            idmin = 211;
         }

            // Fraction of the year in radians
         fact = 2.0*PI*((double)(MOPSTime-idmin))/365.25;

         axfi = ABS(MOPSLat);

         if ( axfi <= 15.0 )                    index=0;
         if ( (axfi > 15.0) && (axfi <= 30.0) ) index=1;
         if ( (axfi > 30.0) && (axfi <= 45.0) ) index=2;
         if ( (axfi > 45.0) && (axfi <= 60.0) ) index=3;
         if ( (axfi > 60.0) && (axfi <  75.0) ) index=4;
         if ( axfi >= 75.0 )                     index=5;

         for (j=0; j<5; j++)
         {
            if (index == 0) {
               avr0(j)=avr(index,j);
               svr0(j)=svr(index,j);
            }
            else
            {
               if (index < 5)
               {
                  avr0(j) = avr(index-1,j) + (avr(index,j)-avr(index-1,j)) *
                            (axfi-fi0(index-1))/(fi0( index)-fi0(index-1));

                  svr0(j) = svr(index-1,j) + (svr(index,j)-svr(index-1,j)) *
                            (axfi-fi0(index-1))/(fi0( index)-fi0(index-1));
               }
               else
               {
                  avr0(j) = avr(index-1,j);
                  svr0(j) = svr(index-1,j);
               }
            }

            MOPSParameters(j) = avr0(j)-svr0(j)*std::cos(fact);
         }

      } // end try
      catch (...)
      {
         InvalidTropModel e("Problem computing extra MOPS parameters.");
         GNSSTK_RETHROW(e);
      }
   }


   void MOPSTropModel::prepareTables()
   {
      avr.resize(5,5);
      svr.resize(5,5);
      fi0.resize(5);


         // Table avr (Average):

      avr(0,0) = 1013.25; avr(0,1) = 299.65; avr(0,2) = 26.31;
         avr(0,3) = 0.0063; avr(0,4) = 2.77;

      avr(1,0) = 1017.25; avr(1,1) = 294.15; avr(1,2) = 21.79;
         avr(1,3) = 0.00605; avr(1,4) = 3.15;

      avr(2,0) = 1015.75; avr(2,1) = 283.15; avr(2,2) = 11.66;
         avr(2,3) = 0.00558; avr(2,4) = 2.57;

      avr(3,0) = 1011.75; avr(3,1) = 272.15; avr(3,2) = 6.78;
         avr(3,3) = 0.00539; avr(3,4) = 1.81;

      avr(4,0) = 1013.00; avr(4,1) = 263.65; avr(4,2) = 4.11;
         avr(4,3) = 0.00453; avr(4,4) = 1.55;


         // Table svr (Seasonal Variation):

      svr(0,0) = 0.00; svr(0,1) = 0.00; svr(0,2) = 0.00;
         svr(0,3) = 0.00000; svr(0,4) = 0.00;

      svr(1,0) = -3.75; svr(1,1) = 7.00; svr(1,2) = 8.85;
         svr(1,3) = 0.00025; svr(1,4) = 0.33;

      svr(2,0) = -2.25; svr(2,1) = 11.00; svr(2,2) = 7.24;
         svr(2,3) = 0.00032; svr(2,4) = 0.46;

      svr(3,0) = -1.75; svr(3,1) = 15.00; svr(3,2) = 5.36;
         svr(3,3) = 0.00081; svr(3,4) = 0.74;

      svr(4,0) = -0.50; svr(4,1) = 14.50; svr(4,2) = 3.39;
         svr(4,3) = 0.00062; svr(4,4) = 0.30;


         // Table fi0 (Latitude bands):

      fi0(0) = 15.0; fi0(1) = 30.0; fi0(2) = 45.0;
         fi0(3) = 60.0; fi0(4) = 75.0;
   }
}
