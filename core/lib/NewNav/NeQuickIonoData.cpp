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
#include "MODIP.hpp"
#include "FreqConv.hpp"

using namespace std;

/// Threshold for solar flux coefficients to consider them zero and unavailable.
constexpr double COEFF_THRESH = 1e-7;
/// Ref Eq.17 of Galileo Ionospheric Model
constexpr double DEFAULT_IONO_LEVEL = 63.7;
constexpr double ABOVE_ELEV_EPSILON = 1e-5;

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
         // throw an exception to prevent the use of this method,
         // which requires the implementation of a TEC integrator.
      GPSTK_THROW(Exception("NeQuickIonoData::getCorrection is incomplete"));
      if ((fabs(ai[0]) < COEFF_THRESH) && (fabs(ai[1]) < COEFF_THRESH) &&
          (fabs(ai[2]) < COEFF_THRESH))
      {
         return DEFAULT_IONO_LEVEL;
      }
         // Obtain estimates of receiver position, satellite position, and time
      double phi1 = rxgeo.getGeodeticLatitude();
      double lambda1 = rxgeo.longitude();
      double h1 = rxgeo.height();
      double phi2 = svgeo.getGeodeticLatitude();
      double lambda2 = svgeo.longitude();
      double h2 = svgeo.height();
         // Obtain receiver MODIP_u
      MODIP modip;
      double modip_u = modip.stModip(phi1,lambda1);
         // Obtain Effective Ionisation Level Azu (also eq.18)
      double azu = ai[0] + ai[1]*modip_u + ai[2]*modip_u*modip_u;       // eq.2
      double tec = 0;
         // The ESA code differences lat and long to determine if a
         // satellite is directly above the receiver, but I think it's
         // better to look at the elevation.
      double elev = rxgeo.elevation(svgeo);
      if (fabs(elev - 90.0) < ABOVE_ELEV_EPSILON)
      {
            // Call NeQuick G VTEC integration routine for path
         tec = getVTEC();
      }
      else
      {
            // Call NeQuick G STEC integration routine for path
         tec = getSTEC();
      }
         // Obtain correction by converting STEC to code delay
      double f = getFrequency(band);
      double d1gr = tec * 40.3/(f*f);                                   // eq.1
      return 0;
   }


   double NeQuickIonoData ::
   getSTEC()
   {
         // for each integration point in the path, call NeQuick
         // routine to obtain electron density
         // Integrate STEC for all points in the path.
      return 0;
   }


   double NeQuickIonoData ::
   getVTEC()
   {
         // for each integration point in the path, call NeQuick
         // routine to obtain electron density
         // Integrate VTEC for all points in the path.
      return 0;
   }
}
