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
#include <cmath>
#include <string.h>
#include "NeQuickIonoData.hpp"
#include "TimeString.hpp"
#include "MODIP.hpp"
#include "FreqConv.hpp"
#include "DebugTrace.hpp"

using namespace std;

/*
 * ALL EQUATION AND SECTION REFERENCES ARE TO THE DOCUMENT 
 * "Ionospheric Correction Algorithm for Galileo Single Frequency Users"
 * aka "Galileo Ionospheric Model"
 * UNLESS OTHERWISE STATED
 *
 * Apologies for all the magic numbers.  They've been replaced with
 * meaningful labels when possible, otherwise this is how it appears
 * in the source document.
 *
 * See the NeQuickIonoData class documentation for additional references.
 */

/// Threshold for solar flux coefficients to consider them zero and unavailable.
constexpr double COEFF_THRESH = 1e-7;
/// Threshold for critical frequency to consider it zero.
constexpr double CRIT_FREQ_THRESH = 1e-6;
/// E layer threshold for critical frequency to consider it zero per eq.82.
constexpr double CRIT_FREQ_THRESH_E = 1e-30;
/// Ref Eq.17 of Galileo Ionospheric Model
constexpr double DEFAULT_IONO_LEVEL = 63.7;
constexpr double ABOVE_ELEV_EPSILON = 1e-5;
/// Relative difference between integration steps per 2.5.8.2.8
constexpr double INTEG_EPSILON = 1e-3;
/// tangent of 30 degrees, used in the Gauss numerical integration
constexpr double tan30 = 0.5773502691896;
/// Used to calculate associated electron density from critical frequency.
constexpr double FREQ2NE_D = 0.124;
/// E layer maximum density height in km.
constexpr double hmE = 120.0;                                           //eq.78
/// E layer bottom-side thickness in km.
constexpr double BEbot = 5.0;                                           //eq.89

   /** Define constants using enums, which avoids the complications of
    * using precompilter macros and also avoids the use of memory that
    * a static const elicits. */
enum NeQuickIonoDataConsts
{
   F2LayerMODIPCoeffCount = 12, ///< Legendre coefficient count for mod dip lat.
   F2LayerLongCoeffCount = 9,   ///< Legendre coefficient count for longitude.
   F2TransFactorCoeffCount = 7, ///< Legendre coefficient count for transmission factor.
};


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
         // Get the time in civil units for later math
      CivilTime civ(when);
         // Obtain receiver modified dip latitude
      MODIP modip;
      double modip_u = modip.stModip(rxgeo);
      double azu = getEffIonoLevel(modip_u);
      double tec = 0;
         // The ESA code differences lat and long to determine if a
         // satellite is directly above the receiver, but I think it's
         // better to look at the elevation.
      double elev = rxgeo.elevation(svgeo);
      CCIR ccir;
      ModelParameters modelParam(modip_u, rxgeo, azu, ccir, civ);
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
   getEffIonoLevel(double modip_u)
      const
   {
         // Obtain Effective Ionisation Level Azu (also eq.2)
      double azu = ai[0] + ai[1]*modip_u + ai[2]*modip_u*modip_u;       // eq.18
      if ((fabs(ai[0]) < COEFF_THRESH) &&                               // eq.17
          (fabs(ai[1]) < COEFF_THRESH) &&
          (fabs(ai[2]) < COEFF_THRESH))
      {
         azu = DEFAULT_IONO_LEVEL;
      }
         // Clamp Azu to [0,400] per section 3.3.
      if (azu < 0)
         azu = 0;
      else if (azu > 400)
         azu = 400;
      return azu;
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


   NeQuickIonoData::ModelParameters ::
   ModelParameters(double modip_u, const Position& pos, double az,
                   CCIR& ccirData, const CivilTime& when)
         : fXeff(effSolarZenithAngle(pos,when)),
           ffoF1(0.0), // default to 0, see eq.37
           ccir(ccirData)
   {
      int seas;
         // get the effective sunspot number
      fAzr = sqrt(167273+(az-DEFAULT_IONO_LEVEL)*1123.6)-408.99;        // eq.19
      switch (when.month)
      {
         case 1:
         case 2:
         case 11:
         case 12:
            seas = -1;                                                  //eq.30
            break;
         case 3:
         case 4:
         case 9:
         case 10:
            seas = 0;                                                   //eq.31
            break;
         case 5:
         case 6:
         case 7:
         case 8:
            seas = 1;                                                   //eq.32
            break;
         default:
            GPSTK_THROW(Exception("Invalid month"));
            break;
      }
      double phi = pos.geodeticLatitude();
      double lambda = pos.longitude();
      double ee = exp(0.3 * phi);                                       //eq.33
      double seasp = seas * (ee-1)/(ee+1);                              //eq.34
      double term1 = (1.112-0.019*seasp);
      double term2 = sqrt(az);
      double term3 = pow(cos(fXeff*DEG2RAD), 0.6);
      ffoE = sqrt(term1*term1*term2*term3 + 0.49);                      //eq.35
      fNmE = FREQ2NE_D * ffoE * ffoE;                                   //eq.36
      if (ffoE >= 2.0)                                                  //eq.37
      {
         ffoF1 = 1.4 * ffoE;
      }
         /// @todo figure out what is meant by "too close to foF2"
      if (ffoF1 < CRIT_FREQ_THRESH)
      {
         ffoF1 = 0.0;
      }
      if ((ffoF1 <= 0) && (ffoE > 2.0))
      {
         fNmF1 = FREQ2NE_D * (ffoE + 0.5) * (ffoE + 0.5);               //eq.38
      }
      else
      {
         fNmF1 = FREQ2NE_D * ffoF1 * ffoF1;                             //eq.39
      }
         // Compute the fourier time series for foF2 and M(3000)F2
      ccir.fourier(when, az);
      legendre(modip_u, pos);
      fNmF2 = FREQ2NE_D * ffoF2 * ffoF2;                                //eq.77
         // Compute peak electron density height for each layer
      height();
         // Compute thickness parameters for each layer
      thickness();
      exosphereAdjust(when.month);
   }


   NeQuickIonoData::ModelParameters ::
   ModelParameters(CCIR& ccirData)
         : ccir(ccirData),
           fAzr(std::numeric_limits<double>::quiet_NaN()),
           ffoE(std::numeric_limits<double>::quiet_NaN()),
           fNmE(std::numeric_limits<double>::quiet_NaN()),
           fBEtop(std::numeric_limits<double>::quiet_NaN()),
           fA3(std::numeric_limits<double>::quiet_NaN()),
           ffoF1(std::numeric_limits<double>::quiet_NaN()),
           fNmF1(std::numeric_limits<double>::quiet_NaN()),
           fhmF1(std::numeric_limits<double>::quiet_NaN()),
           fB1top(std::numeric_limits<double>::quiet_NaN()),
           fB1bot(std::numeric_limits<double>::quiet_NaN()),
           fA2(std::numeric_limits<double>::quiet_NaN()),
           ffoF2(std::numeric_limits<double>::quiet_NaN()),
           fNmF2(std::numeric_limits<double>::quiet_NaN()),
           fhmF2(std::numeric_limits<double>::quiet_NaN()),
           fB2bot(std::numeric_limits<double>::quiet_NaN()),
           fA1(std::numeric_limits<double>::quiet_NaN()),
           fM3000F2(std::numeric_limits<double>::quiet_NaN()),
           fH0(std::numeric_limits<double>::quiet_NaN()),
           fXeff(std::numeric_limits<double>::quiet_NaN())
   {
   }


   void NeQuickIonoData::ModelParameters ::
   solarDeclination(const CivilTime& when, double& sdsin,
                    double& sdcos)
   {
      GPSTK_ASSERT(when.getTimeSystem() == gpstk::TimeSystem::UTC);
         // compute day of year at the middle of the month
      double dy = 30.5 * when.month - 15;                               //eq.20
         // compute time in days
      double t = dy + (18.0-when.getUTHour())/24.0;                     //eq.21
         // compute the argument
      double am = (0.9856 * t - 3.289) * DEG2RAD;                       //eq.22
      double al = am + (1.916*sin(am) + 0.020*sin(2*am) + 282.634) *    //eq.23
         DEG2RAD;
      sdsin = 0.39782*sin(al);                                          //eq.24
      sdcos = sqrt(1-sdsin*sdsin);                                      //eq.25
   }


   double NeQuickIonoData::ModelParameters ::
   solarZenithAngle(const Position& pos, const CivilTime& when)
   {
      double phiRad = pos.geodeticLatitude() * DEG2RAD;
      double lambda = pos.longitude();
      double sdsin, sdcos;
      solarDeclination(when, sdsin, sdcos);
         // leave the UTC check up to solarDeclination
      double lt = when.getUTHour() + (lambda / 15.0);                   //eq.4
         // X is really chi.
      double cosX=sin(phiRad) * sdsin +                                 //eq.26
         cos(phiRad) * sdcos * cos(PI/12*(12-lt));
      double rv = RAD2DEG * atan2(sqrt(1-cosX*cosX),cosX);              //eq.27
      return rv;
   }


   double NeQuickIonoData::ModelParameters ::
   effSolarZenithAngle(const Position& pos, const CivilTime& when)
   {
         // x is really chi.
      static const double x0 = 86.23292796211615;                       //eq.28
      double x = solarZenithAngle(pos, when);
      double exp2 = exp(12*(x-x0));                                     //eq.29
      double rv = (x+(90-0.24*exp(20-0.2*x))*exp2) / (1+exp2);
      return rv;
   }


   void NeQuickIonoData::ModelParameters ::
   legendre(double modip_u, const Position& pos)
   {
         // sine modified dip latitude coefficients
      double M[F2LayerMODIPCoeffCount];                                 //eq.52
         // cosine latitude coefficents
      double P[F2LayerLongCoeffCount];                                  //eq.53
         // sine and cosine longitude coefficients
      double S[F2LayerLongCoeffCount];                                  //eq.54
      double C[F2LayerLongCoeffCount];                                  //eq.55
         // Legendre grades for F2 critical frequency
      const unsigned Q[] {12,12,9,5,2,1,1,1,1};                         //eq.63
      const int K[] {-12, 12, 36, 54, 64, 68, 70, 72, 74};              //eq.66
      M[0] = 1.0;                                                       //eq.56
         // Legendre grades for F2 transmission factor M(3000)F2
      const unsigned R[] {7,8,6,3,2,1,1};                               //eq.71
      const int H[] {-7,7,23,35,41,45,47};                              //eq.74
      double modip_uRad = modip_u * DEG2RAD;
      double phiRad = pos.geodeticLatitude() * DEG2RAD;
      double lambdaRad = pos.longitude() * DEG2RAD;
         // compute sine modififed dip latitude coefficients
      for (unsigned k = 1; k<F2LayerMODIPCoeffCount; k++)
      {
         M[k] = M[k-1] * sin(modip_uRad);                               //eq.57
      }
      P[0] = 1.0; // not used except for initialization convenience
         // compute cos lat, sin long, cos long coefficients
      for (unsigned n = 1; n < F2LayerLongCoeffCount; n++)
      {
         P[n] = P[n-1] * cos(phiRad);                                   //eq.58
         S[n] = sin(n * lambdaRad);                                     //eq.59
         C[n] = cos(n * lambdaRad);                                     //eq.60
      }
         // initialize higher order terms to zero before summation
      ffoF2 = 0;
         // compute foF2 order 0 term
      for (unsigned k = 0; k<F2LayerMODIPCoeffCount; k++)
      {
         ffoF2 += ccir.getCF2(k) * M[k];                                //eq.61
      }
         // compute foF2 higher order terms 2-9 
      for (unsigned n = 1; n<F2LayerLongCoeffCount; n++)
      {
         for (unsigned k=0; k<Q[n]; k++)
         {
            ffoF2 += (ccir.getCF2(K[n]+2*k) * C[n] +                    //eq.67
                      ccir.getCF2(K[n]+2*k+1) * S[n]) * M[k] * P[n];
         }
      }
         // initialize higher order terms to zero before summation
      fM3000F2 = 0;
         // compute M(3000)F2 order 0 term
      for (unsigned k=0; k<F2TransFactorCoeffCount; k++)
      {
         fM3000F2 += ccir.getCM3(k) * M[k];                             //eq.69
      }
         // compute M(3000)F2 higher order terms 2-7
      for (unsigned n = 1; n<F2TransFactorCoeffCount; n++)
      {
         for (unsigned k=0; k<R[n]; k++)
         {
            fM3000F2 += (ccir.getCM3(H[n]+2*k) * C[n] +                 //eq.75
                         ccir.getCM3(H[n]+2*k+1) *S[n]) * M[k] * P[n];
         }
      }
   }


   void NeQuickIonoData::ModelParameters ::
   height()
   {
         // A lot of constants in these equations that I don't really
         // know the context of.  The EU code refers to them as
         // DUDENEY.
      double cfRatio = ffoF2 / ffoE;                                    //eq.84
      double eTerm = exp(20*(cfRatio-1.75));
      double rho = (cfRatio*eTerm + 1.75) / (eTerm + 1);
      double M = fM3000F2;                                              //eq.81
      double dM = (ffoE<CRIT_FREQ_THRESH_E ? -0.012 :                   //eq.82
                   (0.253/(rho-1.215))-0.012);                          //eq.83
      fhmF2 = ((1490 * M * sqrt((0.0196*M*M+1)/(1.2967*M*M-1))) /       //eq.80
               (M + dM)) - 176.0;
      fhmF1 = (fhmF2+hmE) / 2.0;                                        //eq.79
   }


   void NeQuickIonoData::ModelParameters ::
   thickness()
   {
      fB2bot = ((0.385*fNmF2) /                                         //eq.85
                (0.01*exp(-3.467+0.857*log(ffoF2*ffoF2) +
                          2.02 * log(fM3000F2))));
      fB1top = 0.3 * (fhmF2-fhmF1);                                     //eq.86
      fB1bot = 0.5 * (fhmF1 - hmE);                                     //eq.87
      fBEtop = std::max(fB1bot, 7.0);                                   //eq.88
   }


   void NeQuickIonoData::ModelParameters ::
   exosphereAdjust(unsigned month)
   {
      DEBUGTRACE_FUNCTION();
      double k = 0;
      double ka = 0;
      switch (month)
      {
         case 4:
         case 5:
         case 6:
         case 7:
         case 8:
         case 9:
            ka = 6.705 - 0.014*fAzr - 0.008*fhmF2;                      //eq.99
            break;
         case 1:
         case 2:
         case 3:
         case 10:
         case 11:
         case 12:
            ka = fhmF2/fB2bot;
            ka = -7.77 + 0.097*ka*ka + 0.153*fNmF2;                     //eq.100
            break;
         default:
            GPSTK_THROW(Exception("Invalid month"));
            break;
      }
      double kb = exp(ka-2);
      kb = (ka * kb + 2) / (1 + kb);                                    //eq.101
      k = exp(kb-8);
      k = (8 * k + kb) / (1 + k);                                       //eq.102
      double Ha = k * fB2bot;                                           //eq.103
      double x = (Ha - 150.0) / 100.0;                                  //eq.104
      double v = (0.041163*x - 0.183981)*x + 1.424472;                  //eq.105
      DEBUGTRACE("auxiliary_param_x = " << x);
      DEBUGTRACE("auxiliary_param_v = " << v);
      fH0 = Ha / v;                                                     //eq.106
      DEBUGTRACE("top-side thickness parameter = " << fH0);
   }


   void NeQuickIonoData::ModelParameters ::
   peakAmplitudes()
   {
      fA1 = 4 * fNmF2;                                                  //eq.90
      if (ffoF1 < 0.5)
      {
         fA2 = 0;                                                       //eq.91
         fA3 = 4.0 * (fNmE - epstein(fA1,fhmF2,fB2bot,hmE));            //eq.92
      }
      else
      {
         double A3a = 4.0 * fNmE;                                       //eq.93
         double A2a = 0;
            // section 2.5.5.9 simply says to repeat these equations 5 times.
         for (unsigned i = 0; i < 5; i++)
         {
            A2a = 4.0 * (fNmF1 - epstein(fA1,fhmF2,fB2bot,fhmF1) -      //eq.94
                         epstein(A3a,hmE,fBEtop,fhmF1));
            double tmp = exp(A2a - 0.80 * fNmF1);
            A2a = (A2a * tmp + 0.80 * fNmF1) / (1+tmp);                 //eq.95
            A3a = 4.0 * (fNmE - epstein(A2a,fhmF1,fB1bot,hmE) -         //eq.96
                         epstein(fA1,fhmF2,fB2bot,hmE));
         }
         fA2 = A2a;                                                     //eq.97
         double tmp = exp(60.0*(A3a-0.005));
         fA3 = (A3a * tmp + 0.05) / (1+tmp);                            //eq.98
      }
   }


   double NeQuickIonoData ::
   integrateGauss(double g1, double g2)
   {
      DEBUGTRACE_FUNCTION();
      double gn1 = 0.0, gn2 = 0.0;
         // Integrate TEC for all points in the path using the Gauss
         // algorithm as described in section 2.5.8.2.8 of Galileo
         // Ionospheric Model.
      int n = 8;                                                        //eq.194
      bool done = false;
      while (!done)
      {
         double dn = (g2 - g1) / n;                                     //eq.195
         double g = tan30 * dn;                                         //eq.196
         double y = g1 + ((dn - g) / 2.0);                              //eq.197
         gn2 = 0.0;
         for (unsigned i = 0; i < n; i++)                               //eq.198
         {
               /// @todo change sqrt below to the actual function
            gn2 += sqrt(y + i*dn) + sqrt(y + i*dn + g);
         }
         gn2 *= (dn/2.0);
         if (fabs(gn1-gn2) > (INTEG_EPSILON * fabs(gn1)))               //eq.201
         {
            n *= 2.0;                                                   //eq.199
            gn1 = gn2;                                                  //eq.200
         }
      }
      double rv = (gn2 + ((gn2-gn1)/15)) * 1e-13;                       //eq.202
      DEBUGTRACE("rv=" << rv);
      return rv;
   }
}
