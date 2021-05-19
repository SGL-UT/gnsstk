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
#include "KlobucharIonoData.hpp"
#include "TimeString.hpp"
#include "YDSTime.hpp"
#include "FreqConv.hpp"

using namespace std;

namespace gpstk
{
   KlobucharIonoData ::
   KlobucharIonoData()
         : alpha{0,0,0,0},
           beta{0,0,0,0}
   {
   }


   double KlobucharIonoData ::
   getCorrection(const CommonTime& when,
                 const Position& rxgeo,
                 const Position& svgeo,
                 CarrierBand band) const
   {
         // style guide strongly discourages using underscores, but
         // I'm using them here to denote subscripts, latex style.
      double el = rxgeo.elevationGeodetic(svgeo) / 180.0;
      double az = rxgeo.azimuthGeodetic(svgeo) / 180.0;
      double phi_u = rxgeo.getGeodeticLatitude() / 180.0;
      double lambda_u = rxgeo.getLongitude() / 180.0;
         // constant values defined in Figure 20-4, IS-GPS-200
      double psi = (0.0137 / (el+0.11)) - 0.022;
      double phi_i = phi_u + psi * ::cos(az * PI);
      if (phi_i > 0.416)
         phi_i = 0.416;
      if (phi_i < -0.416)
         phi_i = -0.416;
      double lambda_i = lambda_u + (psi * ::sin(az * PI) / ::cos(phi_i * PI));
      double t = 43200 * lambda_i + YDSTime(when).sod;
      double phi_m = phi_i + 0.064 * ::cos((lambda_i-1.617) * PI);
      double iAMP = alpha[0]+phi_m*(alpha[1]+phi_m*(alpha[2]+phi_m*alpha[3]));
      double iPER =  beta[0]+phi_m*( beta[1]+phi_m*( beta[2]+phi_m* beta[3]));
      if (iAMP < 0.0)
         iAMP = 0.0;
      if (iPER < 72000.0)
         iPER = 72000.0;
      double x = TWO_PI * (t - 50400.0) / iPER;
      double iF = 1.0 + 16.0 * (0.53 - el)*(0.53 - el)*(0.53 - el);
      double t_iono = 0.0;
      if (fabs(x) < 1.57)
      {
         t_iono = iF * (5.0e-9 + iAMP * (1 + x*x * (-0.5 + x*x/24.0)));
      }
      else
      {
         t_iono = iF * 5.0e-9;
      }
         /* Klobuchar models used in GPS and QZSS are referenced to
          * L1.  If other systems use this model, this may need to
          * change. */
      if (band != gpstk::CarrierBand::L1)
      {
         double f2 = getFrequency(band);
         t_iono *= (FREQ_GPS_L1/f2)*(FREQ_GPS_L1/f2);
      }
      return t_iono * C_MPS;
   }
}
