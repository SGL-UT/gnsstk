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
#include "BasicTimeSystemConverter.hpp"
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
/// How close an elevation angle needs to be to +/- 90 to be considered polar.
constexpr double ABOVE_ELEV_EPSILON = 1e-5;
/// Relative difference between integration steps per 2.5.8.2.8
constexpr double INTEG_EPSILON = 1e-3;
/// Scalar for bottomside electron density per eq.115 and eq.121
constexpr double ELEC_DEN_SCALING = 1.0e11;
/// Scalar from integral to TEC per eq.151 and eq.202
constexpr double TEC_SCALE_FACTOR = 1.0e-13;
/// Scalar from TEC Units to electrons per square meter.
constexpr double TECU_SCALE_FACTOR = 1.0e16;
/// Topside electron density approximation epsilon per eq.127
constexpr double TOP_APPROX_EPSILON = 1.0e11;
/// TEC numerical integration first point in km point per 2.5.8.2.7
constexpr double INTEGRATION_FIRST_POINT = 1000;
/// TEC numerical integration second point in km point per 2.5.8.2.7
constexpr double INTEGRATION_SECOND_POINT = 2000;
/// Integration epsilon (tolerance) for the s1 to sa interval per 2.5.8.2.7
constexpr double INTEG_EPSILON_S1_SA = 0.001;
/// Integration epsilon (tolerance) for the sa to sb/s2 interval per 2.5.8.2.7
constexpr double INTEG_EPSILON_SA_S2 = 0.01;
/// Minimum return value of neExp()
constexpr double NEEXP_MIN_VALUE = 1.8049e-35;
/// Maximum return value of neExp()
constexpr double NEEXP_MAX_VALUE = 5.5406e34;
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
   RecursionMax = 50,           ///< Maximum integrateGaussKronrod recursion.
   ExponentMin = -80,           ///< Minimum exponent for neExp().
   ExponentMax = 80,            ///< Maximum exponent for neExp().
};


namespace gnsstk
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
      DEBUGTRACE_FUNCTION();
      double tec = getTEC(when, rxgeo, svgeo);
         // Obtain correction by converting STEC to code delay
      double f = getFrequency(band);
      double d1gr = tec * TECU_SCALE_FACTOR * 40.3/(f*f);               // eq.1
      return d1gr;
   }


   double NeQuickIonoData ::
   getTEC(const CommonTime& when,
          const Position& rxgeo,
          const Position& svgeo)
      const
   {
      DEBUGTRACE_FUNCTION();
         // Get the time in civil units for later math
      CivilTime civ(when);
         // Obtain receiver modified dip latitude
      MODIP modip;
      CCIR ccir;
         // pre-determine in a somewhat clumsy, but probably faster
         // method than elevation() if the satellite is directly above
         // the station.
      bool vertical =
         ((fabs(svgeo.geodeticLatitude()-rxgeo.geodeticLatitude()) <
           ABOVE_ELEV_EPSILON) &&
          (fabs(svgeo.longitude()-rxgeo.longitude()) < ABOVE_ELEV_EPSILON));
      Position Pp(rxgeo.getRayPerigee(svgeo));
      IntegrationParameters ip(rxgeo, svgeo, Pp, vertical);
      DEBUGTRACE("computing azu");
      double modip_u = modip.stModip(rxgeo);
      double azu = getEffIonoLevel(modip_u);
      DEBUGTRACE("azu = " << azu);
      DEBUGTRACE("vertical=" << vertical);
      DEBUGTRACE("rxgeo.geodeticLatitude()=" << rxgeo.geodeticLatitude());
      DEBUGTRACE("rxgeo.longitude()=" << rxgeo.longitude());
      DEBUGTRACE("svgeo.geodeticLatitude()=" << svgeo.geodeticLatitude());
      DEBUGTRACE("svgeo.longitude()=" << svgeo.longitude());
      DEBUGTRACE("svgeo.height()=" << svgeo.height());
      Angle debugAngle(rxgeo.geodeticLatitude(),Angle::Deg);
      DEBUGTRACE("pPosition->latitude.rad=" << scientific << debugAngle.rad());
      DEBUGTRACE("pPosition->latitude.degree=" << scientific << debugAngle.deg());
      DEBUGTRACE("pPosition->latitude.sin=" << scientific << debugAngle.sin());
      DEBUGTRACE("pPosition->latitude.cos=" << scientific << debugAngle.cos());
      debugAngle = Angle(rxgeo.longitude(), Angle::Deg);
      DEBUGTRACE("pPosition->longitude.rad=" << scientific << debugAngle.rad());
      DEBUGTRACE("pPosition->longitude.degree=" << scientific << debugAngle.deg());
      DEBUGTRACE("pPosition->longitude.sin=" << scientific << debugAngle.sin());
      DEBUGTRACE("pPosition->longitude.cos=" << scientific << debugAngle.cos());
      DEBUGTRACE("pPosition->height()=" << scientific << (rxgeo.height() / 1000.0));
      DEBUGTRACE("pPosition->radius_km()=" << scientific << (rxgeo.radius() / 1000.0));
      DEBUGTRACE("# pContext->modip_degree=" << scientific << modip_u);
      DEBUGTRACE("# a_sfu[0]=" << scientific << ai[0]);
      DEBUGTRACE("# a_sfu[1]=" << scientific << ai[1]);
      DEBUGTRACE("# a_sfu[2]=" << scientific << ai[2]);
      double rv = 0;
         // must have at least two slant heights to make an interval...
      if (ip.integHeights.size() > 1)
      {
         for (unsigned i = 1; i < ip.integHeights.size(); i++)
         {
            rv += integrateGaussKronrod(ip.integHeights[i-1],
                                        ip.integHeights[i],
                                        rxgeo, svgeo, modip, modip_u, ccir, civ,
                                        azu, ip.intThresh[i-1], vertical);
         }
      }
         // scale as per eq.151 and eq.202
      rv *= TEC_SCALE_FACTOR;
      return rv;
   }


   double NeQuickIonoData ::
   getEffIonoLevel(double modip_u)
      const
   {
      DEBUGTRACE_FUNCTION();
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
   getSED(double dist, const Position& rxgeo, const Position& svgeo,
          const MODIP& modip, CCIR& ccirData, const CivilTime& when,
          double azu)
      const
   {
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("height_km=" << setprecision(15) << dist);
      Position current(rxgeo.getRayPosition(dist * 1000.0, svgeo));
      double modip_u = modip.stModip(current);
      DEBUGTRACE("constructing SED iono");
      ModelParameters iono(modip_u, current, azu, ccirData, when);
      double electronDensity = iono.electronDensity(current);
      DEBUGTRACE("electron density=" << setprecision(15) << scientific
                 << electronDensity);
      return electronDensity;
   }


   double NeQuickIonoData ::
   getVED(double dist, const Position& rxgeo, const Position& svgeo,
          double modip_u, CCIR& ccirData, const CivilTime& when,
          double azu)
      const
   {
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("height_km=" << setprecision(15) << dist);
         // remember that dist is a height for VED, and that it's in km
      Position current(rxgeo.geocentricLatitude(), rxgeo.longitude(), dist*1000,
                       Position::Geodetic, &elModel);
      DEBUGTRACE("constructing VED iono");
      ModelParameters iono(modip_u, current, azu, ccirData, when);
      double electronDensity = iono.electronDensity(current);
      return electronDensity;
   }


   double NeQuickIonoData ::
   neExp(double x)
   {
      if (x > ExponentMax)
      {
         return NEEXP_MAX_VALUE;
      }
      else if (x < ExponentMin)
      {
         return NEEXP_MIN_VALUE;
      }
      return exp(x);
   }


   NeQuickIonoData::ModelParameters ::
   ModelParameters(double modip_u, const Position& pos, double az,
                   CCIR& ccirData, const CivilTime& when)
         : fXeff(effSolarZenithAngle(pos,when)),
           ffoF1(0.0), // default to 0, see eq.37
           ccir(ccirData)
   {
      DEBUGTRACE_FUNCTION();
      int seas;
      DEBUGTRACE("pos = " << pos);
      DEBUGTRACE("solar_12_month_running_mean_of_2800_MHZ_noise_flux=" << az);
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
            GNSSTK_THROW(Exception("Invalid month"));
            break;
      }
      double phi = pos.geodeticLatitude();
      double lambda = pos.longitude();
      DEBUGTRACE("power=" << (0.3 * phi));
      DEBUGTRACE("# pSolar_activity->effective_ionisation_level_sfu="
                 << scientific << az);
      double ee = neExp(0.3 * phi);                                     //eq.33
      double seasp = (seas * (ee-1))/(ee+1);                            //eq.34
      double term1 = (1.112-0.019*seasp);
      double term2 = sqrt(az);
      double term3 = pow(cos(fXeff), 0.6);
      DEBUGTRACE("cos(fXeff)=" << cos(fXeff));
      DEBUGTRACE("pow(cos(fXeff),0.6)=" << pow(cos(fXeff), 0.6));
      DEBUGTRACE("pow(cos(fXeff),0.3)=" << pow(cos(fXeff), 0.3));
      DEBUGTRACE("pow(cos(fXeff),0.3) (alt)=" << exp(log(cos(fXeff))*0.3));
      ffoE = sqrt(term1*term1*term2*term3 + 0.49);                      //eq.35
      DEBUGTRACE("ffoE=" << ffoE);
      fNmE = FREQ2NE_D * ffoE * ffoE;                                   //eq.36
      if (ffoE >= 2.0)                                                  //eq.37
      {
         ffoF1 = 1.4 * ffoE;
      }
         /// @todo figure out what is meant by "too close to foF2" in eq.37
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
      DEBUGTRACE("latitude=" << phi);
      DEBUGTRACE("seas=" << seas);
      DEBUGTRACE("ee=" << scientific << ee);
      DEBUGTRACE("seasp=" << seasp);
         // Compute the fourier time series for foF2 and M(3000)F2
      ccir.fourier(when, fAzr);
      legendre(modip_u, pos);
      fNmF2 = FREQ2NE_D * ffoF2 * ffoF2;                                //eq.77
         // Compute peak electron density height for each layer
      height();
         // Compute thickness parameters for each layer
      thickness();
      exosphereAdjust(when.month);
      peakAmplitudes();
   }


   NeQuickIonoData::ModelParameters ::
   ModelParameters(CCIR& ccirData)
         : ccir(ccirData),
           fAzr(std::numeric_limits<double>::quiet_NaN()),
           ffoE(std::numeric_limits<double>::quiet_NaN()),
           fNmE(std::numeric_limits<double>::quiet_NaN()),
           fBEtop(std::numeric_limits<double>::quiet_NaN()),
           fA{std::numeric_limits<double>::quiet_NaN(),
              std::numeric_limits<double>::quiet_NaN(),
              std::numeric_limits<double>::quiet_NaN()},
           ffoF1(std::numeric_limits<double>::quiet_NaN()),
           fNmF1(std::numeric_limits<double>::quiet_NaN()),
           fhmF1(std::numeric_limits<double>::quiet_NaN()),
           fB1top(std::numeric_limits<double>::quiet_NaN()),
           fB1bot(std::numeric_limits<double>::quiet_NaN()),
           ffoF2(std::numeric_limits<double>::quiet_NaN()),
           fNmF2(std::numeric_limits<double>::quiet_NaN()),
           fhmF2(std::numeric_limits<double>::quiet_NaN()),
           fB2bot(std::numeric_limits<double>::quiet_NaN()),
           fM3000F2(std::numeric_limits<double>::quiet_NaN()),
           fH0(std::numeric_limits<double>::quiet_NaN()),
           fXeff()
   {
      DEBUGTRACE_FUNCTION();
   }


   AngleReduced NeQuickIonoData::ModelParameters ::
   solarDeclination(const CivilTime& when)
   {
      DEBUGTRACE_FUNCTION();
      gnsstk::BasicTimeSystemConverter btsc;
      CivilTime whenUTC(when);
      if (whenUTC.getTimeSystem() != gnsstk::TimeSystem::UTC)
      {
         GNSSTK_ASSERT(whenUTC.changeTimeSystem(gnsstk::TimeSystem::UTC,&btsc));
      }
         // compute day of year at the middle of the month
      double dy = 30.5 * whenUTC.month - 15;                            //eq.20
         // compute time in days
      double t = dy + (18.0-whenUTC.getUTHour())/24.0;                  //eq.21
         // compute the argument
      double am = (0.9856 * t - 3.289) * DEG2RAD;                       //eq.22
      double al = am + (1.916*sin(am) + 0.020*sin(2*am) + 282.634) *    //eq.23
         DEG2RAD;
      return AngleReduced(0.39782*sin(al),AngleReduced::Sin);           //eq.24
   }


   Angle NeQuickIonoData::ModelParameters ::
   solarZenithAngle(const Position& pos, const CivilTime& when)
   {
      DEBUGTRACE_FUNCTION();
      double phiRad = pos.geodeticLatitude() * DEG2RAD;
      double lambda = pos.longitude();
      AngleReduced deltaSun = solarDeclination(when);
         // leave the UTC check up to solarDeclination
      double lt = when.getUTHour() + (lambda / 15.0);                   //eq.4
         // X is really chi.
      double cosX=sin(phiRad) * sin(deltaSun) +                         //eq.26
         cos(phiRad) * cos(deltaSun) * cos(PI/12*(12-lt));
      return Angle(atan2(sqrt(1-cosX*cosX),cosX),Angle::Rad);           //eq.27
   }


   Angle NeQuickIonoData::ModelParameters ::
   effSolarZenithAngle(const Position& pos, const CivilTime& when)
   {
      DEBUGTRACE_FUNCTION();
         // x is really chi.
      static const double x0 = 86.23292796211615;                       //eq.28
      Angle x = solarZenithAngle(pos, when);
      double exp2 = neExp(12*(x.deg()-x0));                             //eq.29
      return Angle((x.deg()+(90-0.24*neExp(20-0.2*x.deg()))*exp2) / (1+exp2),
                   Angle::Deg);
   }


   void NeQuickIonoData::ModelParameters ::
   legendre(double modip_u, const Position& pos)
   {
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE("lambdaRad = " << scientific << lambdaRad);
      DEBUGTRACE("lat.rad = " << scientific << phiRad);
      DEBUGTRACE("lat.deg = " << scientific << pos.geodeticLatitude());
      DEBUGTRACE("cos_lat = " << scientific << cos(phiRad));
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
         S[n-1] = sin(n * lambdaRad);                                   //eq.59
         C[n-1] = cos(n * lambdaRad);                                   //eq.60
         DEBUGTRACE("lambda[" << n << "]=" << scientific << (n*lambdaRad));
         DEBUGTRACE("S[" << n << "]=" << scientific << S[n]);
         DEBUGTRACE("C[" << n << "]=" << scientific << C[n]);
      }
         // initialize higher order terms to zero before summation
      ffoF2 = 0;
         // compute foF2 order 0 term
      for (unsigned k = 0; k<F2LayerMODIPCoeffCount; k++)
      {
         ffoF2 += ccir.getCF2(k) * M[k];                                //eq.61
         DEBUGTRACE("CF2[" << k << "]=" << scientific << ccir.getCF2(k)
                    << "  M[" << k << "]=" << M[k] << "  parameter=" << ffoF2);
      }
         // compute foF2 higher order terms 2-9
      for (unsigned n = 1; n<F2LayerLongCoeffCount; n++)
      {
         for (unsigned k=0; k<Q[n]; k++)
         {
            ffoF2 += (ccir.getCF2(K[n]+2*k) * C[n-1] +                  //eq.67
                      ccir.getCF2(K[n]+2*k+1) * S[n-1]) * M[k] * P[n];
            DEBUGTRACE("M[" << k << "]=" << scientific << M[k]
                       << "  lat_coeff=" << P[n]
                       << "  Fourier_coeff[" << (K[n]+2*k) << "]="
                       << ccir.getCF2(K[n]+2*k)
                       << "  C[" << (n-1) << "]=" << C[n-1]
                       << "  Fourier_coeff[" << (K[n]+2*k+1) << "]="
                       << ccir.getCF2(K[n]+2*k+1)
                       << "  S[" << (n-1) << "]=" << S[n-1]
                       << "  parameter=" << ffoF2);
         }
      }
         // initialize higher order terms to zero before summation
      fM3000F2 = 0;
         // compute M(3000)F2 order 0 term
      DEBUGTRACE("M(3000)F2");
      for (unsigned k=0; k<F2TransFactorCoeffCount; k++)
      {
         fM3000F2 += ccir.getCM3(k) * M[k];                             //eq.69
         DEBUGTRACE("CM3[" << k << "]=" << scientific << ccir.getCM3(k)
                    << "  M[" << k << "]=" << M[k] << "  parameter="
                    << fM3000F2);
      }
         // compute M(3000)F2 higher order terms 2-7
      for (unsigned n = 1; n<F2TransFactorCoeffCount; n++)
      {
         for (unsigned k=0; k<R[n]; k++)
         {
            fM3000F2 += (ccir.getCM3(H[n]+2*k) * C[n-1] +               //eq.75
                         ccir.getCM3(H[n]+2*k+1) * S[n-1]) * M[k] * P[n];
            DEBUGTRACE("M[" << k << "]=" << scientific << M[k]
                       << "  lat_coeff=" << P[n]
                       << "  Fourier_coeff[" << (H[n]+2*k) << "]="
                       << ccir.getCM3(H[n]+2*k)
                       << "  C[" << (n-1) << "]=" << C[n-1]
                       << "  Fourier_coeff[" << (H[n]+2*k+1) << "]="
                       << ccir.getCM3(H[n]+2*k+1)
                       << "  S[" << (n-1) << "]=" << S[n-1]
                       << "  parameter=" << fM3000F2);
         }
      }
   }


   void NeQuickIonoData::ModelParameters ::
   height()
   {
      DEBUGTRACE_FUNCTION();
         // A lot of constants in these equations that I don't really
         // know the context of.  The EU code refers to them as
         // DUDENEY.
      double cfRatio = ffoF2 / ffoE;                                    //eq.84
      double eTerm = neExp(20*(cfRatio-1.75));
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
      DEBUGTRACE_FUNCTION();
         // note that the EU code uses the standard exp here too
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
      DEBUGTRACE("pTime->month=" << month);
      DEBUGTRACE("# pSolar_activity->effective_sun_spot_count=" << scientific << fAzr);
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
            GNSSTK_THROW(Exception("Invalid month"));
            break;
      }
      double kb = neExp(ka-2);
      kb = (ka * kb + 2) / (1 + kb);                                    //eq.101
      k = neExp(kb-8);
      k = (8 * k + kb) / (1 + k);                                       //eq.102
      double Ha = k * fB2bot;                                           //eq.103
      double x = (Ha - 150.0) / 100.0;                                  //eq.104
      double v = (0.041163*x - 0.183981)*x + 1.424472;                  //eq.105
      DEBUGTRACE("shape_factor = " << k);
      DEBUGTRACE("auxiliary_param_x = " << x);
      DEBUGTRACE("auxiliary_param_v = " << v);
      fH0 = Ha / v;                                                     //eq.106
      DEBUGTRACE("top-side thickness parameter = " << fH0);
   }


   void NeQuickIonoData::ModelParameters ::
   peakAmplitudes()
   {
      DEBUGTRACE_FUNCTION();
         // F2 layer amplitude A1
      fA[0] = 4 * fNmF2;                                                //eq.90
      double A3a = 0, A2a = 0;
      if (ffoF1 < 0.5)
      {
         DEBUGTRACE("option 1");
            // Eq.91 and Eq.92 appear to assign A2 and A3 directly,
            // but the EU code makes it appear that they should
            // actually be assigning to A2a and A3a instead, with
            // Eq.97 and Eq.98 being applied regardless of the value
            // of foF1.
         A2a = 0;                                                       //eq.91
         A3a = 4.0 * (fNmE - epstein(fA[0],fhmF2,fB2bot,hmE));          //eq.92
      }
      else
      {
         DEBUGTRACE("option 2");
         A3a = 4.0 * fNmE;                                              //eq.93
         A2a = 0;
            // section 2.5.5.9 simply says to repeat these equations 5 times.
         for (unsigned i = 0; i < 5; i++)
         {
            A2a = 4.0 * (fNmF1 - epstein(fA[0],fhmF2,fB2bot,fhmF1) -    //eq.94
                         epstein(A3a,hmE,fBEtop,fhmF1));
            double tmp = neExp(A2a - 0.80 * fNmF1);
            A2a = (A2a * tmp + 0.80 * fNmF1) / (1+tmp);                 //eq.95
            A3a = 4.0 * (fNmE - epstein(A2a,fhmF1,fB1bot,hmE) -         //eq.96
                         epstein(fA[0],fhmF2,fB2bot,hmE));
         }
      }
         // F1 layer amplitude A2
      fA[1] = A2a;                                                      //eq.97
      double tmp = neExp(60.0*(A3a-0.005));
         // E layer amplitude A3
      fA[2] = (A3a * tmp + 0.05) / (1+tmp);                             //eq.98
   }


   double NeQuickIonoData::ModelParameters ::
   electronDensity(const Position& pos)
   {
      DEBUGTRACE_FUNCTION();
      double rv = 0;
         // must convert height from m to km first
      if ((pos.height() / 1000.0) <= fhmF2)
      {
         rv = electronDensityBottom(pos);
      }
      else
      {
         rv = electronDensityTop(pos);
      }
      DEBUGTRACE("electron density=" << scientific << rv);
      return rv;
   }


   double NeQuickIonoData::ModelParameters ::
   electronDensityTop(const Position& pos)
   {
      DEBUGTRACE_FUNCTION();
      static constexpr double g = 0.125;                                //eq.122
      static constexpr double r = 100;                                  //eq.123
      double h = pos.height() / 1000.0; // height in km
      double deltah = h - fhmF2;                                        //eq.124
      double z = deltah / (fH0*(1+(r*g*deltah)/(r*fH0+g*deltah)));      //eq.125
      double ea = neExp(z);                                             //eq.126
      double rv;
      if (ea > TOP_APPROX_EPSILON)
      {
         rv = 4 * fNmF2 / ea;                                           //eq.127
      }
      else
      {
         rv = 4 * fNmF2 * ea / ((1+ea)*(1+ea));                         //eq.127
      }
      return rv * ELEC_DEN_SCALING;
   }


   double NeQuickIonoData::ModelParameters ::
   electronDensityBottom(const Position& pos)
   {
      DEBUGTRACE_FUNCTION();
      double h = pos.height() / 1000.0; // height in km
      double BE = (h > hmE) ? fBEtop : BEbot;                           //eq.109
      double BF1 = (h > fhmF1) ? fB1top : fB1bot;                       //eq.110
      double mh = std::max(h, 100.0); // see note after eq.113
         // note that the EU code uses the standard exp here too
      double t2 = exp(10/(1+fabs(mh-fhmF2)));
      double alpha[3];
      alpha[0] = (mh - fhmF2) / fB2bot;                                 //eq.111
      alpha[1] = (mh - fhmF1) / BF1 * t2;                               //eq.112
      alpha[2] = (mh - hmE) / BE * t2;                                  //eq.113
         // ea = exp(alpha)
      double ea[3] { neExp(alpha[0]), neExp(alpha[1]), neExp(alpha[2]) };
      double s[3];
      for (unsigned i = 0; i < 3; i++)
      {
         if (fabs(alpha[i]) > 25)
         {
            s[i] = 0;
         }
         else
         {
            s[i] =  fA[i] * ea[i]/((1+ea[i])*(1+ea[i]));                //eq.114
         }
      }
      double rv;
      if (h >= 100.0)
      {
         rv = s[0] + s[1] + s[2];                                       //eq.115
      }
      else
      {
         double ds[3];
         ds[0] = fabs(alpha[0]) > 25 ? 0                                //eq.116
            : (1/fB2bot)*((1-ea[0])/(1+ea[0]));
         ds[1] = fabs(alpha[1]) > 25 ? 0                                //eq.117
            : (1/BF1)*((1-ea[1])/(1+ea[1]));
         ds[2] = fabs(alpha[2]) > 25 ? 0                                //eq.118
            : (1/BE)*((1-ea[2])/(1+ea[2]));
         double sumNum = s[0]*ds[0]+s[1]*ds[1]+s[2]*ds[2];
         double sumDenom = s[0]+s[1]+s[2];
         double BC = 1 - 10*sumNum/sumDenom;                            //eq.119
         double z = (h-100)/10;                                         //eq.120
         rv = sumDenom * neExp(1-BC*z-neExp(-z));                       //eq.121
      }
      return rv * ELEC_DEN_SCALING;
   }


   NeQuickIonoData::IntegrationParameters ::
   IntegrationParameters(const Position& rx, const Position& sv,
                         const Position& Pp, bool vertical)
   {
      DEBUGTRACE_FUNCTION();
      GalileoIonoEllipsoid elModel;
      if (vertical)
      {
            // minimum height of station is 0.
         double h1 = std::max(0.0, rx.height() / 1000.0);
         double ha = INTEGRATION_FIRST_POINT;
         double hb = INTEGRATION_SECOND_POINT;
         double h2 = sv.height() / 1000.0;
            // populate the vector with all the heights that will be
            // used as integration intervals.
         integHeights.push_back(h1);
         if ((ha > h1) && (ha < h2))
         {
            integHeights.push_back(ha);
            intThresh.push_back(INTEG_EPSILON_S1_SA);
         }
         if ((hb > h1) && (hb < h2))
         {
            integHeights.push_back(hb);
            intThresh.push_back(INTEG_EPSILON_SA_S2);
         }
         integHeights.push_back(h2);
            // If intThresh is empty at this point, we know h1 and h2 are
            // both below ha, so use the higher-precision epsilon.
            // Otherwise, the h1 to h2 ray crosses one or both of ha,hb,
            // so use the lower-precision epsilon.
         if (intThresh.empty())
         {
            intThresh.push_back(INTEG_EPSILON_S1_SA);
         }
         else
         {
            intThresh.push_back(INTEG_EPSILON_SA_S2);
         }
         DEBUGTRACE("h1 = " << h1);
         DEBUGTRACE("ha = " << ha);
         DEBUGTRACE("hb = " << hb);
         DEBUGTRACE("h2 = " << h2);
      }
      else
      {
         double rp = Pp.radius() / 1000.0; // ray perigee radius in km
         double ip1 = elModel.a_km() + INTEGRATION_FIRST_POINT;
         double ip2 = elModel.a_km() + INTEGRATION_SECOND_POINT;
            // minimum radius of the station must be the surface of
            // the "ellipsoid" radius is converted from m to km
         double r1 = std::max(elModel.a_km(), rx.radius() / 1000.0);
            // same as above, but for satellite, which we assume isn't
            // underground
         double r2 = sv.radius() / 1000.0;
         double s1 = sqrt(fabs(r1*r1 - rp*rp));
         double sa = sqrt(fabs(ip1*ip1 - rp*rp));                       //eq.188
         double sb = sqrt(fabs(ip2*ip2 - rp*rp));                       //eq.189
         double s2 = sqrt(fabs(r2*r2 - rp*rp));
            // populate the vector with all the slant heights that will
            // be used as integration intervals.
         integHeights.push_back(s1);
         if ((sa > s1) && (sa < s2))
         {
            integHeights.push_back(sa);
            intThresh.push_back(INTEG_EPSILON_S1_SA);
         }
         if ((sb > s1) && (sb < s2))
         {
            integHeights.push_back(sb);
            intThresh.push_back(INTEG_EPSILON_SA_S2);
         }
         integHeights.push_back(s2);
            // If intThresh is empty at this point, we know s1 and s2 are
            // both below sa, so use the higher-precision epsilon.
            // Otherwise, the s1 to s2 ray crosses one or both of sa,sb,
            // so use the lower-precision epsilon.
         if (intThresh.empty())
         {
            intThresh.push_back(INTEG_EPSILON_S1_SA);
         }
         else
         {
            intThresh.push_back(INTEG_EPSILON_SA_S2);
         }
         DEBUGTRACE("s1 = " << s1);
         DEBUGTRACE("sa = " << sa);
         DEBUGTRACE("sb = " << sb);
         DEBUGTRACE("s2 = " << s2);
      }
      DEBUGTRACE("integHeights.size() = " << integHeights.size());
      DEBUGTRACE("intThresh.size() = " << intThresh.size());
   }


   double NeQuickIonoData ::
   integrateGaussKronrod(double heightPt1, double heightPt2,
                         const Position& rxgeo, const Position& svgeo,
                         const MODIP& modip, double modipSta, CCIR& ccirData,
                         const CivilTime& when, double azu, double tolerance,
                         bool vertical, unsigned recursionLevel)
      const
   {
      DEBUGTRACE_FUNCTION();
      double rv = 0;
      DEBUGTRACE("pResult(1) = " << scientific << rv);
         // This code and the constants originate from section F.2.6.1
         // \cite galileo:iono
         /** @note This code is based on pseudocode in F.2.6.1; there
          * are no formulae references in the document. */
         // weights for K15 sample points
      static const double wi[] = {
         0.022935322010529224963732008058970,
         0.063092092629978553290700663189204,
         0.104790010322250183839876322541518,
         0.140653259715525918745189590510238,
         0.169004726639267902826583426598550,
         0.190350578064785409913256402421014,
         0.204432940075298892414161999234649,
         0.209482141084727828012999174891714,
         0.204432940075298892414161999234649,
         0.190350578064785409913256402421014,
         0.169004726639267902826583426598550,
         0.140653259715525918745189590510238,
         0.104790010322250183839876322541518,
         0.063092092629978553290700663189204,
         0.022935322010529224963732008058970
      };
         // weights for G7 sample points
      static const double wig[] = {
         0.129484966168869693270611432679082,
         0.279705391489276667901467771423780,
         0.381830050505118944950369775488975,
         0.417959183673469387755102040816327,
         0.381830050505118944950369775488975,
         0.279705391489276667901467771423780,
         0.129484966168869693270611432679082
      };
         // at what points the samples are used in integration process
         // note that points 0-7 are the negative of points 9-15, in reverse.
      static const double xi[] = {
         -0.991455371120812639206854697526329,
         -0.949107912342758524526189684047851,
         -0.864864423359769072789712788640926,
         -0.741531185599394439863864773280788,
         -0.586087235467691130294144838258730,
         -0.405845151377397166906606412076961,
         -0.207784955007898467600689403773245,
         0,
         0.207784955007898467600689403773245,
         0.405845151377397166906606412076961,
         0.586087235467691130294144838258730,
         0.741531185599394439863864773280788,
         0.864864423359769072789712788640926,
         0.949107912342758524526189684047851,
         0.991455371120812639206854697526329
      };
         // half-difference
      double h2 = (heightPt2 - heightPt1) / 2.0;
         // mid-point
      double hh = (heightPt2 + heightPt1) / 2.0;
         // K15 integration results
      double intk = 0.0;
         // G7 integration results
      double intg = 0.0;
         // G7 counter/index
      unsigned gind = 0;
      DEBUGTRACE("mid_point=" << hh);
      DEBUGTRACE("half_diff=" << h2);
         // Iterating over K15, which is defined to have 15 values
      for (unsigned i = 0; i < 15; i++)
      {
         double x = h2 * xi[i] + hh;
         double y = 0;
         DEBUGTRACE("i=" << i << "  x=" << x);
         if (vertical)
         {
            y = getVED(x, rxgeo, svgeo, modipSta, ccirData, when, azu);
         }
         else
         {
            y = getSED(x, rxgeo, svgeo, modip, ccirData, when, azu);
         }
         DEBUGTRACE("GKI ED = " << scientific << y);
            // Accumulate on to the k15 total
         intk += y * wi[i];
         if (i % 2)
         {
            intg += y * wig[gind++];
         }
      }
         // Complete the calculation of the integration results
      intk = intk * h2;
      intg = intg * h2;
         // Check if the result is within tolerance
      if (((fabs(intk - intg) / intk) <= tolerance) ||
          (fabs(intk - intg) <= tolerance))
      {
            // Result is within tolerance, so return intk
         DEBUGTRACE("pResult(2) = " << scientific << intk);
         return intk;
      }
      else if (recursionLevel >= RecursionMax)
      {
            // No further integration allowed, return intk as best guess.
         DEBUGTRACE("pResult(3) = " << scientific << intk);
         return intk;
      }
      else
      {
            // Result is not within tolerance.  Split portion into
            // equal halves and recurse.
         rv = integrateGaussKronrod(heightPt1, heightPt1 + h2, rxgeo, svgeo,
                                    modip, modipSta, ccirData, when, azu,
                                    tolerance, vertical, recursionLevel+1);
         DEBUGTRACE("pResult(4) = " << scientific << rv);
         rv += integrateGaussKronrod(heightPt1 + h2, heightPt2, rxgeo, svgeo,
                                     modip, modipSta, ccirData, when, azu,
                                     tolerance, vertical, recursionLevel+1);
         DEBUGTRACE("pResult(5) = " << scientific << rv);
      }
      return rv;
   }
}
