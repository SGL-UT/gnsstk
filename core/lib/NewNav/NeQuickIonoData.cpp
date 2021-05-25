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
#include "NeQuickIonoData.hpp"
#include "TimeString.hpp"
#include "modip.hpp"

using namespace std;

namespace gpstk
{
   NeQuickIonoData ::
   NeQuickIonoData()
         : ai{0,0,0},
           idf{false,false,false,false,false}
   {
   }


   void NeQuickIonoData ::
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
         // std::string svn;
         // if (getSVN(satID, ctToe, svn))
         // {
         //    s << svn;
         // }
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
        << "           NEQUICK IONO PARAMETERS" << endl
        << "Parameter              Value" << endl
        << "a_i0         " << setw(16) << ai[0] << " sfu" << endl
        << "a_i1         " << setw(16) << ai[1] << " sfu/degree" << endl
        << "a_i2         " << setw(16) << ai[2] << " sfu/degree**2" << endl
        << boolalpha
        << "IDF region 1 " << setw(16) << idf[0] << endl
        << "IDF region 2 " << setw(16) << idf[1] << endl
        << "IDF region 3 " << setw(16) << idf[2] << endl
        << "IDF region 4 " << setw(16) << idf[3] << endl
        << "IDF region 5 " << setw(16) << idf[4] << endl;

      s.flags(oldFlags);
   }


   double NeQuickIonoData ::
   getCorrection(const CommonTime& when,
                 const Position& rxgeo,
                 const Position& svgeo,
                 CarrierBand band) const
   {
         // Obtain estimates of receiver position, satellite position, and time
      double phi1 = rxgeo.getGeodeticLatitude();
      double lambda1 = rxgeo.longitude();
      double h1 = rxgeo.height();
      double phi2 = svgeo.getGeodeticLatitude();
      double lambda2 = svgeo.longitude();
      double h2 = svgeo.height();
         // Obtain receiver MODIP_u
      double modip = getMODIP(phi1,lambda1);
         // Obtain Effective Ionisation Level Azu
      double azu = ai[0] + ai[1]*modip + ai[2]*modip*modip;           // eq.2
         // Call NeQuick G STEC integration routine for path
      double stec = getSTEC();
         // Obtain correction by converting STEC to code delay
      double d1gr = 40.3;                                     // eq.1
      return 0;
   }


   int NeQuickIonoData ::
   getl(double lambda)
   {
         // yes, this is supposed to be int, see eq 6, Galileo Ionospheric Model
      int rv = ((lambda + 180) / 10) - 2;  // eq.6
      if (rv < 0)
         rv += 36; // eq.7
      if (rv > 33)
         rv -= 36; // eq.8
      return rv;
   }


   double NeQuickIonoData ::
   getMODIP(double phi, double lambda)
   {
         // Because Galileo's documentation brilliantly uses the z
         // term in one instance as a matrix and in another as a
         // vector, we call the matrix "zM" and the vector "zV".
      double zM[4][4];
      int l = getl(lambda);
      double a = ((phi+90.0)/5.0)+1.0;                                // eq.9
      double x = a - int(a);                                          // eq.10
      int i = int(a) - 2;                                             // eq.11
      for (int k = 1; k <= 4; k++)
      {
         for (int j = 1; j <= 4; j++)
         {
            zM[j-1][k-1] = gnssdata::stModip(i+j, l+k);               // eq.12
         }
      }
      double zV[4];
      for (int k = 0; k < 4; k++)
      {
         zV[k] = interpolate(zM[0][k],zM[1][k],zM[2][k],zM[3][k],x);  // eq.13
      }
      double b = (lambda + 180.0) / 10.0;                             // eq.14
      double y = b - int(b);                                          // eq.15
      return interpolate(zV[0], zV[1], zV[2], zV[3], y);              // eq.16
   }


   double NeQuickIonoData ::
   interpolate(double z1, double z2, double z3, double z4, double x)
   {
      if (fabs(2*x) < 1e-10)
      {
            // if x is really close to zero, just return the point at 0.
         return z2;                             // eq.128
      }
      double sigma = 2*x - 1;                   // eq.129
      double g1 = z3 + z2;                      // eq.130
      double g2 = z3 - z2;                      // eq.131
      double g3 = z4 + z1;                      // eq.132
      double g4 = (z4 - z1) / 3.0;              // eq.133
      double a0 = 9.0*g1 - g3;                  // eq.134
      double a1 = 9.0*g2 - g4;                  // eq.135
      double a2 = g3 - g1;                      // eq.136
      double a3 = g4 - g2;                      // eq.137
         // eq.138
      return (a0 + a1*sigma + a2*sigma*sigma + a3*sigma*sigma*sigma) / 16.0;
   }


   double NeQuickIonoData ::
   getSTEC()
   {
         // for each integration point in the path, call NeQuick
         // routine to obtain electron density
         // Integrate STEC for all points in the path.
      return 0;
   }
}
