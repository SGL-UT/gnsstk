//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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

namespace gnsstk
{
   KlobucharIonoData ::
   KlobucharIonoData()
         : alpha{0,0,0,0},
           beta{0,0,0,0}
   {
   }


   void KlobucharIonoData ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      if (dl == DumpDetail::OneLine)
      {
         NavData::dump(s,dl);
         return;
      }
         // "header"
      s << "****************************************************************"
        << "************" << endl
        << " Ionospheric correction data"
        << endl
        << endl
        << "PRN : " << setw(2) << signal.sat << " / "
        << "SVN : " << setw(2);
      std::string svn;
      if (getSVN(signal.sat, timeStamp, svn))
      {
         s << svn;
      }
      s << endl << endl;

         // the rest is full details, so just return if Full is not asked for.
      if (dl != DumpDetail::Full)
         return;

      const ios::fmtflags oldFlags = s.flags();

      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');

      std::string timeFmt = weekFmt+dumpTimeFmt;
      s << endl
        << "           TIMES OF INTEREST"
        << endl << endl
        << "              Week(10bt)     SOW     DOW   UTD     SOD"
        << "   MM/DD/YYYY   HH:MM:SS\n"
        << "Transmit:     " << printTime(timeStamp, timeFmt) << endl;

      s.setf(ios::scientific, ios::floatfield);
      s.precision(8);
      s.fill(' ');

      s << endl
        << "           KLOBUCHAR IONO PARAMETERS" << endl
        << "Parameter              Value" << endl
        << "alpha[0]     " << setw(16) << alpha[0] << " sec" << endl
        << "alpha[1]     " << setw(16) << alpha[1] << " sec/semi-circle" << endl
        << "alpha[2]     " << setw(16) << alpha[2] << " sec/semi-circle**2"
        << endl
        << "alpha[3]     " << setw(16) << alpha[3] << " sec/semi-circle**3"
        << endl
        << "beta[0]      " << setw(16) << beta[0] << " sec" << endl
        << "beta[1]      " << setw(16) << beta[1] << " sec/semi-circle" << endl
        << "beta[2]      " << setw(16) << beta[2] << " sec/semi-circle**2"
        << endl
        << "beta[3]      " << setw(16) << beta[3] << " sec/semi-circle**3"
        << endl;

      s.flags(oldFlags);
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
      if (band != gnsstk::CarrierBand::L1)
      {
         double f2 = getFrequency(band);
         t_iono *= (FREQ_GPS_L1/f2)*(FREQ_GPS_L1/f2);
      }
      return t_iono * C_MPS;
   }
}
