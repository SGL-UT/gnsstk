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


#include "GPSEllipsoid.hpp"
#include "GGHeightTropModel.hpp"

#define THROW_IF_INVALID_DETAILED() {if (!valid) {                      \
         InvalidTropModel e;                                            \
         if(!validWeather) e.addText("Invalid trop model: weather");    \
         if(!validHeights)  e.addText("Invalid trop model: validHeights"); \
         if(!validRxHeight)   e.addText("Invalid trop model: validRxHeight"); \
         GNSSTK_THROW(e);}}

namespace gnsstk
{
   GGHeightTropModel::GGHeightTropModel()
   {
      validWeather = false; //setWeather(20.0,980.0,50.0);
      validHeights = false; //setHeights(0.0,0.0,0.0);
      validRxHeight = false;
   }


   GGHeightTropModel::GGHeightTropModel(const WxObservation& wx)
   {
      valid = validRxHeight = validHeights = false;
      setWeather(wx);
   }


   GGHeightTropModel::GGHeightTropModel(const double& T,
                                        const double& P,
                                        const double& H)
   {
      validRxHeight = validHeights = false;
      setWeather(T,P,H);
   }


   GGHeightTropModel::GGHeightTropModel(const double& T,
                                        const double& P,
                                        const double& H,
                                        const double hT,
                                        const double hP,
                                        const double hH)
   {
      validRxHeight = false;
      setWeather(T,P,H);
      setHeights(hT,hP,hH);
   }

      // re-define this to get the throws
   double GGHeightTropModel::correction(double elevation) const
   {
      THROW_IF_INVALID_DETAILED();

      if(elevation < 0.0) return 0.0;

      return (dry_zenith_delay() * dry_mapping_function(elevation)
            + wet_zenith_delay() * wet_mapping_function(elevation));

   }


   double GGHeightTropModel::correction(const Position& RX,
                                        const Position& SV,
                                        const CommonTime& tt)
   {
      THROW_IF_INVALID_DETAILED();

      // compute height from RX
      setReceiverHeight(RX.getHeight());

      return TropModel::correction(RX.elevation(SV));

   }  // end GGHeightTropModel::correction(RX,SV,TT)

   double GGHeightTropModel::correction(const Xvt& RX,
                                        const Xvt& SV,
                                        const CommonTime& tt)
   {
      Position R(RX),S(SV);
      return GGHeightTropModel::correction(R,S,tt);
   }


   double GGHeightTropModel::dry_zenith_delay() const
   {
      THROW_IF_INVALID_DETAILED();
      double hrate=6.5e-3;
      double Ts=temp+hrate*height;
      double em=978.77/(2.8704e4*hrate);
      double Tp=Ts-hrate*hpress;
      double ps=press*std::pow(Ts/Tp,em)/1000.0;
      double rs=77.624e-3/Ts;
      double ho=11.385/rs;
      rs *= ps;
      double zen=(ho-height)/ho;
      zen = rs*zen*zen*zen*zen;
         // normalize
      zen *= (ho-height)/5;
      return zen;

   }


   double GGHeightTropModel::wet_zenith_delay() const
   {
      THROW_IF_INVALID_DETAILED();

      double hrate=6.5e-3; //   deg K / m
      double Th=temp-273.15-hrate*(hhumid-htemp);
      double Ta=7.5*Th/(237.3+Th);
         // water vapor partial pressure
      double e0=6.11e-5*humid*std::pow(10.0,Ta);
      double Ts=temp+hrate*htemp;
      double em=978.77/(2.8704e4*hrate);
      double Tk=Ts-hrate*hhumid;
      double es=e0*std::pow(Ts/Tk,4.0*em);
      double rs=(371900.0e-3/Ts-12.92e-3)/Ts;
      double ho=11.385*(1255/Ts+0.05)/rs;
      double zen=(ho-height)/ho;
      zen = rs*es*zen*zen*zen*zen;
         //normalize
      zen *= (ho-height)/5;
      return zen;

   }


   double GGHeightTropModel::dry_mapping_function(double elevation) const
   {
      THROW_IF_INVALID_DETAILED();

      if(elevation < 0.0) return 0.0;

      double hrate=6.5e-3;
      double Ts=temp+hrate*htemp;
      double ho=(11.385/77.624e-3)*Ts;
      double se=std::sin(elevation*DEG_TO_RAD);
      if(se < 0.0) se=0.0;

      GPSEllipsoid ell;
      double rt,a,b,rn[8],al[8],er=ell.a();
      rt = (er+ho)/(er+height);
      rt = rt*rt - (1.0-se*se);
      if(rt < 0) rt=0.0;
      rt = (er+height)*(SQRT(rt)-se);
      a = -se/(ho-height);
      b = -(1.0-se*se)/(2.0*er*(ho-height));
      rn[0] = rt*rt;
      for(int j=1; j<8; j++) rn[j]=rn[j-1]*rt;
      al[0] = 2*a;
      al[1] = 2*a*a+4*b/3;
      al[2] = a*(a*a+3*b);
      al[3] = a*a*a*a/5+2.4*a*a*b+1.2*b*b;
      al[4] = 2*a*b*(a*a+3*b)/3;
      al[5] = b*b*(6*a*a+4*b)*0.1428571;
      if(b*b > 1.0e-35) {
         al[6] = a*b*b*b/2;
         al[7] = b*b*b*b/9;
      } else {
         al[6] = 0.0;
         al[7] = 0.0;
      }
      double map=rt;
      for(int k=0; k<8; k++) map += al[k]*rn[k];
         // normalize
      double norm=(ho-height)/5;
      return map/norm;
   }


   double GGHeightTropModel::wet_mapping_function(double elevation) const
   {
      THROW_IF_INVALID_DETAILED();
      if(elevation < 0.0) return 0.0;

      double hrate=6.5e-3;
      double Ts=temp+hrate*htemp;
      double rs=(371900.0e-3/Ts-12.92e-3)/Ts;
      double ho=11.385*(1255/Ts+0.05)/rs;
      double se=std::sin(elevation*DEG_TO_RAD);
      if(se < 0.0) se=0.0;

      GPSEllipsoid ell;
      double rt,a,b,rn[8],al[8],er=ell.a();
      rt = (er+ho)/(er+height);
      rt = rt*rt - (1.0-se*se);
      if(rt < 0) rt=0.0;
      rt = (er+height)*(SQRT(rt)-se);
      a = -se/(ho-height);
      b = -(1.0-se*se)/(2.0*er*(ho-height));
      rn[0] = rt*rt;
      for(int i=1; i<8; i++) rn[i]=rn[i-1]*rt;
      al[0] = 2*a;
      al[1] = 2*a*a+4*b/3;
      al[2] = a*(a*a+3*b);
      al[3] = a*a*a*a/5+2.4*a*a*b+1.2*b*b;
      al[4] = 2*a*b*(a*a+3*b)/3;
      al[5] = b*b*(6*a*a+4*b)*0.1428571;
      if(b*b > 1.0e-35) {
         al[6] = a*b*b*b/2;
         al[7] = b*b*b*b/9;
      } else {
         al[6] = 0.0;
         al[7] = 0.0;
      }
      double map=rt;
      for(int j=0; j<8; j++) map += al[j]*rn[j];
         // normalize map function
      double norm=(ho-height)/5;
      return map/norm;

   }


   void GGHeightTropModel::setWeather(const double& T,
                                      const double& P,
                                      const double& H)
   {
      try
      {
         TropModel::setWeather(T,P,H);
         validWeather = true;
         valid = validWeather && validHeights && validRxHeight;
      }
      catch(InvalidParameter& e)
      {
         valid = validWeather = false;
         GNSSTK_RETHROW(e);
      }
   }


   void GGHeightTropModel::setWeather(const WxObservation& wx)
   {
      try
      {
         TropModel::setWeather(wx);
         validWeather = true;
         valid = validWeather && validHeights && validRxHeight;
      }
      catch(InvalidParameter& e)
      {
         valid = validWeather = false;
         GNSSTK_RETHROW(e);
      }
   }


   void GGHeightTropModel::setHeights(const double& hT,
                                      const double& hP,
                                      const double& hH)
   {
      htemp = hT;                 // height (m) at which temp applies
      hpress = hP;                // height (m) at which press applies
      hhumid = hH;                // height (m) at which humid applies
      validHeights = true;
      valid = validWeather && validHeights && validRxHeight;
   }


   void GGHeightTropModel::setReceiverHeight(const double& ht)
   {
      height = ht;
      validRxHeight = true;
      if(!validHeights) {
         htemp = hpress = hhumid = ht;
         validHeights = true;
      }
      valid = validWeather && validHeights && validRxHeight;
   }

}
