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
#include "GLOFNavAlm.hpp"
#include "TimeString.hpp"
#include "YDSTime.hpp"
#include "GLOFNavEph.hpp"

using namespace std;

namespace gnsstk
{
   const PZ90Ellipsoid GLOFNavAlm::ell;
   const double GLOFNavAlm::mu = ell.gm_km();
   const double GLOFNavAlm::ae = ell.a_km();
   const double GLOFNavAlm::icp = 63.0 * gnsstk::PI / 180.0;
   const double GLOFNavAlm::Tcp = 43200.0;
   const double GLOFNavAlm::C20 = -1082.63e-6;
   const double GLOFNavAlm::J = (-3.0/2.0) * C20;
   const double GLOFNavAlm::C20Term = (3.0/2.0) * C20;


   GLOFNavAlm ::
   GLOFNavAlm()
         : healthBits(false),
           tau(std::numeric_limits<double>::quiet_NaN()),
           lambda(std::numeric_limits<double>::quiet_NaN()),
           deltai(std::numeric_limits<double>::quiet_NaN()),
           ecc(std::numeric_limits<double>::quiet_NaN()),
           omega(std::numeric_limits<double>::quiet_NaN()),
           tLambda(std::numeric_limits<double>::quiet_NaN()),
           deltaT(std::numeric_limits<double>::quiet_NaN()),
           deltaTdot(std::numeric_limits<double>::quiet_NaN()),
           freq(-1)
   {
      signal.messageType = NavMessageType::Almanac;
      msgLenSec = 4.0;
   }


   bool GLOFNavAlm ::
   validate() const
   {
         /// @todo implement some checking.
      return true;
   }


   bool GLOFNavAlm ::
   getXvt(const CommonTime& when, Xvt& xvt)
   {
      return nc.getXvt(when, xvt, *this);
   }


   CommonTime GLOFNavAlm ::
   getUserTime() const
   {
      CommonTime mr = std::max({timeStamp, xmit2});
      return mr + 2.0;
   }


   void GLOFNavAlm ::
   fixFit()
   {
         // There is no stated fit interval or period of validity
         // for almanac data.  However, it is generally safe to
         // assume that the data should not be used before
         // the transmit time. 
      beginFit = timeStamp;
         /// @todo get a better end fit interval than this.
      endFit = CommonTime::END_OF_TIME;
      endFit.setTimeSystem(beginFit.getTimeSystem());
         // other computed data
      setSemiMajorAxisIncl();
   }


   void GLOFNavAlm ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      if (dl == DumpDetail::Terse)
      {
         dumpTerse(s);
         return;
      }
      ios::fmtflags oldFlags = s.flags();

      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');

      // s << "****************************************************************"
      //   << "************" << endl
      //   << "GLONASS ORB/CLK (IMMEDIATE) PARAMETERS" << endl << endl
      //   << "SAT : " << signal.sat << endl << endl;
      s << "**************************************************************"
        << endl
        << " GLONASS ORB/CLK (NON-IMMEDIATE) PARAMETERS for GLONASS Slot "
        << signal.sat.id << endl;

         // the rest is full details, so just return if Full is not asked for.
      if (dl != DumpDetail::Full)
         return;

      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');

         /** @bug this shouldn't be here, but I'm putting it here
          * temporarily to make it easier to compare with legacy code
          * output. */
      CommonTime xmit(timeStamp);
      xmit.setTimeSystem(TimeSystem::GLO);
         /* ^^ */
      string tform("%02m/%02d/%Y %03j %02H:%02M:%02S  %7.0s  %P");
      s << endl
        << "              MM/DD/YYYY DOY HH:MM:SS      SOD\n"
        << "Transmit   :  "
        << printTime(xmit,tform) << endl
        << "Orbit Epoch:  "
        << printTime(Toa,tform) << endl
        << endl
        << "Parameter              Value" << endl;

      s.setf(ios::scientific, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(6);
      s.fill(' ');

      s << "tau         " << setw(16) << tau  << " sec" << endl
        << "lambda      " << setw(16) << lambda << " rad" << endl
        << "di          " << setw(16) << deltai << " rad" << endl
        << "e           " << setw(16) << ecc << " dimensionless" << endl
        << "omega       " << setw(16) << omega << " rad" << endl
        << "tLambda     " << setw(16) << tLambda << " seconds" << endl
        << "dT          " << setw(16) << deltaT << " sec/orbit" << endl
        << "dTd         " << setw(16) << deltaTdot << " sec/orbit**2" << endl;

      s.setf(ios::fixed, ios::floatfield);
      s.precision(0);
      s << "C           " << setw(16) <<  healthBits << " encoded:";
      if (healthBits == false)
      {
         s << " NON-operational";
      }
      else
      {
         s << " operational";
      }
      s << endl
        << "M           " << setw(16) << static_cast<int>(satType)
        << " encoded: " << StringUtils::asString(satType) << endl
        << "H           " << setw(16) << freq << " freq. offset" << endl
        << "Transmit SV " << setw(16) << signal.xmitSat << endl
        << "l           " << setw(16) << lhealth
        << " Health of transmitting SV" << endl;

      s.flags(oldFlags);
   }


   void GLOFNavAlm ::
   dumpTerse(std::ostream& s) const
   {
      string tform2("%02m/%02d/%4Y %03j %02H:%02M:%02S");
      stringstream ss;
      ss << "  " << setw(2) << signal.sat.id << "  ";
      ss << printTime(beginFit,tform2);
      ss << " ! ";
      ss << printTime(Toa,tform2) << " ! ";
      if (healthBits == true)
      {
         ss << " op";
      }
      else
      {
         ss << "bad";
      }
      ss << "    " << setw(2) << freq;
      ss << "    " << setw(2) << signal.sat.id;
      s << ss.str();
   }


   void GLOFNavAlm ::
   setSemiMajorAxisIncl()
   {
      nc.setSemiMajorAxisIncl(deltaT, deltai, omega, ecc);
   }
   

   GLOFNavAlm::NumberCruncher ::
   NumberCruncher()
         : lambdaBar(std::numeric_limits<double>::quiet_NaN()),
           tau(std::numeric_limits<double>::quiet_NaN()),
           nu(std::numeric_limits<double>::quiet_NaN()),
           Tdeltap(std::numeric_limits<double>::quiet_NaN()),
           n(std::numeric_limits<double>::quiet_NaN()),
           a(std::numeric_limits<double>::quiet_NaN()),
           i(std::numeric_limits<double>::quiet_NaN()),
           coslambdaBar(std::numeric_limits<double>::quiet_NaN()),
           sinlambdaBar(std::numeric_limits<double>::quiet_NaN()),
           cos2lambdaBar(std::numeric_limits<double>::quiet_NaN()),
           sin2lambdaBar(std::numeric_limits<double>::quiet_NaN()),
           cos3lambdaBar(std::numeric_limits<double>::quiet_NaN()),
           sin3lambdaBar(std::numeric_limits<double>::quiet_NaN()),
           cos4lambdaBar(std::numeric_limits<double>::quiet_NaN()),
           sin4lambdaBar(std::numeric_limits<double>::quiet_NaN()),
           earthvs(std::numeric_limits<double>::quiet_NaN()),
           sini(std::numeric_limits<double>::quiet_NaN()),
           sini2(std::numeric_limits<double>::quiet_NaN()),
           cosi(std::numeric_limits<double>::quiet_NaN()),
           cosi2(std::numeric_limits<double>::quiet_NaN()),
           JTerm(std::numeric_limits<double>::quiet_NaN()),
           JsinTerm(std::numeric_limits<double>::quiet_NaN()),
           Jsini2Term(std::numeric_limits<double>::quiet_NaN()),
           Jcosi2Term(std::numeric_limits<double>::quiet_NaN()),
           h(std::numeric_limits<double>::quiet_NaN()),
           l(std::numeric_limits<double>::quiet_NaN()),
           ecc2(std::numeric_limits<double>::quiet_NaN()),
           ecc2obv(std::numeric_limits<double>::quiet_NaN())
   {
   }


   void GLOFNavAlm::NumberCruncher ::
   setEccArgOfPerigee(double ecc, double omega)
   {
      h = ecc * std::sin(omega);
      l = ecc * std::cos(omega);
      ecc2 = ecc * ecc;
      ecc2obv = 1.0 - ecc2;
   }


   void GLOFNavAlm::NumberCruncher ::
   setSemiMajorAxisIncl(double deltaT, double deltai, double omega, double ecc)
   {
         // from GLONASS ICD appendix 3.2.2, Algorithm of calculation
      double aCurr = 0, aPrev = 9999999;
      setEccArgOfPerigee(ecc, omega);
      Tdeltap = Tcp + deltaT;
      n = 2.0*gnsstk::PI / Tdeltap;
      i = icp + deltai;
      sini = sin(i);
      sini2 = sini * sini;
      cosi = std::cos(i);
      cosi2 = cosi * cosi;
      double ninv = Tdeltap / (2.0*gnsstk::PI);
      cerr << setprecision(15) << "icp = " << icp << endl << "Delta i = " << deltai << endl << "i = " << i << endl << "sini2 = " << sini2 << endl << "ninv = " << ninv << endl << "mu = " << mu << endl << "ecc2obv = "
           << ecc2obv << endl;
      nu = -omega;
      double nuTerm = std::pow(1.0 + ecc * cos(nu), 3.0);
      double omegaTerm = std::pow(1.0 + ecc * cos(omega), 2.0);
      double eTerm = std::pow(ecc2obv, 3.0/2.0);
      double bigTerm =
         ((2.0 - (5.0/2.0)*sini2) * (eTerm / omegaTerm) + (nuTerm / ecc2obv));
         // initial approximation a^(0)
      aCurr = std::pow(ninv * ninv * mu, 1.0/3.0);
      cerr << "a^(0) = " << aCurr << endl;
      cerr << "  Tock_b = " << nuTerm << endl;
      cerr << "  Tock_c = " << (nuTerm / ecc2obv) << endl;
      cerr << "  Tock_d = " << eTerm << endl;
      cerr << "  Tock_f = " << omegaTerm << endl;
      cerr << "  Tock_g = " << (eTerm / omegaTerm) << endl;
      cerr << "  Tock_h = " << (2.0 - (5.0/2.0)*sini2) << endl;
      cerr << "  Tock_i = " << bigTerm << endl;
      unsigned iteration = 0;
      while (fabs(aCurr-aPrev) >= 1e-3)
      {
         cerr << "a^(" << iteration << ") = " << aPrev << endl;
         aPrev = aCurr;
         cerr << "   now a^(" << iteration << ") = " << aPrev << endl;
         double p = aPrev * ecc2obv;
            // note that I use Tdeltap / .... to match Tdeltap * ....^-1
         double tock_k = (1.0 + C20Term * (ae/p) * (ae/p) * bigTerm);
         cerr << "  Tock_k = " << tock_k << endl
              << "  pn = " << p << endl;
         double Tock = Tdeltap / tock_k;
         aCurr = std::pow(std::pow(Tock/(2.0*gnsstk::PI),2.0) * mu, 1.0/3.0);
         cerr << "a^(" << iteration++ << "+1) = " << aCurr << endl
              << "  diff = " << fabs(aCurr-aPrev) << endl;
      }
      a = aCurr;
      earthvs = std::pow(ae/a, 2.0);
      JTerm = J * earthvs;
      JsinTerm = JTerm * (1.0-(3.0/2.0)*sini2);
      Jsini2Term = JTerm * sini2;
      Jcosi2Term = JTerm * cosi2;
   }


   void GLOFNavAlm::NumberCruncher ::
   setLambdaBar(double M, double omega, double tau, double n)
   {
      lambdaBar = M + omega + n * tau;
      cerr << "setLambdaBar:" << endl
           << "  M = " << M << endl
           << "  omega = " << omega << endl
           << "  n = " << n << endl
           << "  tau = " << tau << endl
           << "  n*tau = " << (n*tau) << endl
           << "  lambdaBar = " << lambdaBar << endl;
      coslambdaBar = std::cos(lambdaBar);
      sinlambdaBar = std::sin(lambdaBar);
      cos2lambdaBar = std::cos(2.0*lambdaBar);
      sin2lambdaBar = std::sin(2.0*lambdaBar);
      cos3lambdaBar = std::cos(3.0*lambdaBar);
      sin3lambdaBar = std::sin(3.0*lambdaBar);
      cos4lambdaBar = std::cos(4.0*lambdaBar);
      sin4lambdaBar = std::sin(4.0*lambdaBar);
   }


   void GLOFNavAlm::NumberCruncher ::
   setDeltas(double M, double omega, double a, double dt)
   {
      Deltas m1, m2;
         // lambda Bar MUST be set just prior to calling setDeltas in
         // order to get the proper value for tau, and thus the proper
         // values for lambdaBar and derived values in place.
      setLambdaBar(M, omega, 0, n);
      m1.setDeltas(*this, a, 0, n); // tau is 0 for m=1
      setLambdaBar(M, omega, dt, n);
      m2.setDeltas(*this, a, dt, n);
      deltas = m2 - m1;
      cerr << "deltas:" << endl
           << "delta a = " << deltas.deltaa << endl
           << "delta h = " << deltas.deltah << endl
           << "delta l = " << deltas.deltal << endl
           << "delta OMEGA = " << deltas.deltaOMEGA << endl
           << "delta i = " << deltas.deltai << endl
           << "delta lambda* = " << deltas.deltalambdaBar << endl;
   }


   double GLOFNavAlm::NumberCruncher ::
   getomegai(double hi, double li, double epsi)
   {
      if (epsi == 0)
      {
         return 0;
      }
      else if (li != 0)
      {
         return std::atan(hi/li);
      }
      else if (hi == epsi)
      {
         return gnsstk::PI/2.0;
      }
      else if (hi == -epsi)
      {
         return -gnsstk::PI/2.0;
      }
         // The above conditions match everything in the ICD, but I
         // can't help but wonder if epsi and hi will ever not match
         // at all and still be non-zero, so I'm sticking this here
         // just in case.
      AssertionFailure exc("omega_i conditions not met");
      GNSSTK_THROW(exc);
   }


   double GLOFNavAlm::NumberCruncher ::
   integrateEin(double Mi, double epsi)
   {
      double Ein = Mi, lastEin = -99999;
      while (fabs(Ein-lastEin) >= 1e-8)
      {
         lastEin = Ein;
         Ein = Mi + epsi * std::sin(lastEin);
      }
      return Ein;
   }


   bool GLOFNavAlm::NumberCruncher ::
   getXvt(const CommonTime& when, Xvt& xvt, const GLOFNavAlm& alm)
   {
         // expressions computed once and reused multiple times are
         // marked "optimize"
         // I think this is sufficient - the original formula appears
         // to be getting the time difference between the requested
         // and reference times, in seconds.
         // This may also be the only place where NA is implicitly used.
      double tstar = when - alm.Toa;
      double Wk = tstar / Tdeltap;
         // I'm definitely not sure about this one.
      double W;
      std::modf(Wk,&W); // W is the integer part of Wk
      double tscale = Tdeltap*W + alm.deltaTdot*W*W;       // optimize
      double tLambdakBar = alm.tLambda + tscale;
      double tLambdak = std::fmod(tLambdakBar, 86400.0);
      double OMEGAdot = C20Term * n * earthvs * cosi * std::sqrt(ecc2obv);
      cerr << "OMEGAdot = " << OMEGAdot << endl
           << "n = " << n << endl;
      double lambdak = alm.lambda + (OMEGAdot - omega3) * tscale;
      double gst = GLOFNavEph::getSiderealTime(alm.Toa);
      double S0 = gst*PI/12.0;
      double S = S0 + omega3 * (tLambdak - 10800);
      double OMEGA = lambdak + S;
      cerr << "OMEGA = " << OMEGA << endl
           << "S = " << S << endl
           << "S0 = " << S0 << endl
           << "C20Term = " << C20Term << endl
           << "earthvs = " << earthvs << endl
           << "ecc2obv = " << ecc2obv << endl;
         ///////////////////////////////////////////////////////////////////////
      double tanEdiv2 = std::sqrt((1.0-alm.ecc)/(1.0+alm.ecc)) *
         std::tan(nu/2.0);
      double E = 2.0 * std::atan(tanEdiv2);
      double M = E - alm.ecc * std::sin(E);
      cerr << "tan(E/2) = " << tanEdiv2 << endl
           << "E = " << E << endl
           << "M = " << M << endl;
      setDeltas(M, alm.omega, a, tstar);
         ///////////////////////////////////////////////////////////////////////
      double ai = a + deltas.deltaa;
      double hi = h + deltas.deltah;
      double li = l + deltas.deltal;
      double ii = i + deltas.deltai;
      double OMEGAi = OMEGA + deltas.deltaOMEGA;
      double sinOMEGAi = std::sin(OMEGAi);
      double cosOMEGAi = std::cos(OMEGAi);
      double epsi = std::sqrt(hi*hi+li*li);
      double omegai = getomegai(hi, li, epsi);
      double lambdaStar = M + alm.omega + n*tstar + deltas.deltalambdaBar;
      double Mi = lambdaStar - omegai;
      double Ein = integrateEin(Mi, epsi);
      double tannuidiv2 = std::sqrt((1.0+epsi)/(1.0-epsi)) * std::tan(Ein/2.0);
      double nui = 2.0 * std::atan(tannuidiv2);
      double sinnui = std::sin(nui);
      double cosnui = std::cos(nui);
      double cosii = std::cos(ii);
      double sinii = std::sin(ii);
      double muTerm = std::sqrt(mu/ai);
      double epsiTerm = std::sqrt(1-(epsi*epsi));
      double ui = nui + omegai;
      double cosui = std::cos(ui);
      double sinui = std::sin(ui);
      double ri = ai * (1-(epsi * cos(Ein)));
      double Vri = muTerm * ((epsi * sinnui) / epsiTerm);
      double Vui = muTerm * ((1 + epsi * cosnui) / epsiTerm);
      cerr << "hi = " << hi << endl
           << "li = " << li << endl
           << "epsi = " << epsi << endl
           << "omegai = " << omegai << endl
           << "ai = " << ai << endl
           << "ii = " << ii << endl
           << "OMEGAi = " << OMEGAi << endl
           << "Mi = " << Mi << endl
           << "lambda* = " << lambdaStar << endl
           << "Ein = " << Ein << endl
           << "nui = " << nui << endl
           << "ui = " << ui << endl
           << "ri = " << ri << endl
           << "Vri = " << Vri << endl
           << "Vui = " << Vui << endl;
      xvt.x[0] = ri * (cosui*cosOMEGAi - sinui*sinOMEGAi*cosii);
      xvt.x[1] = ri * (cosui*sinOMEGAi + sinui*cosOMEGAi*cosii);
      xvt.x[2] = ri * sinui * sinii;

      cerr << "             sin                  cos" << endl
           << "ii          " << sinii << "    " << cosii << endl
           << "ui          " << sinui << "    " << cosui << endl
           << "OMEGAi      " << sinOMEGAi << "    " << cosOMEGAi << endl;
      xvt.v[0] = Vri * (cosui*cosOMEGAi - sinui*sinOMEGAi*cosii) -
         Vui * (sinui*cosOMEGAi + cosui*sinOMEGAi*cosii);
      xvt.v[1] = Vri * (cosui*sinOMEGAi + sinui*cosOMEGAi*cosii) -
         Vui * (sinui*sinOMEGAi - cosui*cosOMEGAi*cosii);
      double vyaa = (cosui*sinOMEGAi);
      double vyab = (sinui*cosOMEGAi*cosii);
      double vya = Vri * (vyaa + vyab);
      double vyba = (sinui*sinOMEGAi);
      double vybb = (-cosui*cosOMEGAi*cosii);
      double vyb = -Vui * (vyba + vybb);
      cerr << "vyaa = " << vyaa << endl
           << "vyab = " << vyab << endl
           << "vya = " << vya << endl
           << "vyba = " << vyba << endl
           << "vybb = " << vybb << endl
           << "vyb = " << vyb << endl;
      xvt.v[2] = Vri * sinui * sinii + Vui * cosui * sinii;
      return true;
   }


   GLOFNavAlm::NumberCruncher::Deltas ::
   Deltas()
         : deltaa(std::numeric_limits<double>::quiet_NaN()),
           deltah(std::numeric_limits<double>::quiet_NaN()),
           deltal(std::numeric_limits<double>::quiet_NaN()),
           deltaOMEGA(std::numeric_limits<double>::quiet_NaN()),
           deltai(std::numeric_limits<double>::quiet_NaN()),
           deltalambdaBar(std::numeric_limits<double>::quiet_NaN())
   {
   }


   GLOFNavAlm::NumberCruncher::Deltas GLOFNavAlm::NumberCruncher::Deltas ::
   operator-(const Deltas& right) const
   {
      Deltas rv;
      rv.deltaa = deltaa-right.deltaa;
      rv.deltah = deltah-right.deltah;
      rv.deltal = deltal-right.deltal;
      rv.deltaOMEGA = deltaOMEGA-right.deltaOMEGA;
      rv.deltai = deltai-right.deltai;
         // the ICD actually writes this ONE FORMULA as
         // delta lambda * = delta lambda bar (2) - delta lambda bar (1)
         // so it's not strictly accuracte to call it lambdaBar after
         // subtracting, I guess, but whatever.
      rv.deltalambdaBar = deltalambdaBar-right.deltalambdaBar;
      return rv;
   }


   void GLOFNavAlm::NumberCruncher::Deltas ::
      setDeltas(const NumberCruncher& nc, double a, double tau, double n)
   {
         // fractions used repeatedly.
      static constexpr double OH = 0.5;      // one half
      static constexpr double TH = 3.0/2.0;  // three halves
      static constexpr double FH = 5.0/2.0;  // five halves
      static constexpr double SH = 7.0/2.0;  // five halves
      static constexpr double STH = 17.0/2.0; // seventeen halves
      static constexpr double ST = 7.0/3.0;  // seven thirds
      static constexpr double OQ = 1.0/4.0;  // one quarter
      static constexpr double SQ = 7.0/4.0;  // seven quarters
      static constexpr double SS = 7.0/6.0;  // seven sixths
      static constexpr double STF = 7.0/24.0;// seven twenty-fourths
      static constexpr double FNSS = 49.0/72.0; // guess.
         // note that lambda bar is defined in two different ways, one
         // without adding n*tau, and one with, where the latter is
         // used when computing corrections due to effects of C20.
      double a_a = 2.0 * nc.JsinTerm * (nc.l * nc.coslambdaBar
                                        + nc.h * nc.sinlambdaBar)
         + nc.Jsini2Term * (OH * nc.h * nc.sinlambdaBar
                            - OH * nc.l * nc.coslambdaBar
                            + nc.cos2lambdaBar
                            + SH * nc.l * nc.cos3lambdaBar
                            + SH * nc.h * nc.sin3lambdaBar);
         // The ICD defines the abbove expression as delta a(m) / a,
         // so multiply the whole thing by a to get delta a(m)
      deltaa = a_a * a;
      deltah = nc.JsinTerm * (nc.l * n * tau
                              + nc.sinlambdaBar
                              + TH * nc.l * nc.sin2lambdaBar
                              - TH * nc.h * nc.cos2lambdaBar)
         - OQ * nc.Jsini2Term * (nc.sinlambdaBar
                                 - ST * nc.sin3lambdaBar
                                 + 5.0 * nc.l * nc.sin2lambdaBar
                                 - STH * nc.l * nc.sin4lambdaBar
                                 + STH * nc.h * nc.cos4lambdaBar
                                 + nc.h * nc.cos2lambdaBar)
         + nc.Jcosi2Term * (nc.l * n * tau
                            - OH * nc.l * nc.sin2lambdaBar);
      deltal = nc.JsinTerm * (-nc.h * n * tau
                              + nc.coslambdaBar
                              + TH * nc.l * nc.cos2lambdaBar
                              + TH * nc.h * nc.sin2lambdaBar)
         - OQ * nc.Jsini2Term * (-nc.coslambdaBar
                                 - ST*nc.cos3lambdaBar
                                 - 5.0 * nc.h * nc.sin2lambdaBar
                                 - STH * nc.l * nc.cos4lambdaBar
                                 - STH * nc.h * nc.sin4lambdaBar
                                 + nc.l * nc.cos2lambdaBar)
         + nc.Jcosi2Term * (-nc.h * n * tau
                            + OH * nc.h * nc.sin2lambdaBar);
      deltaOMEGA = -nc.JTerm * nc.cosi * (n * tau
                                          + SH * nc.l * nc.sinlambdaBar
                                          - FH * nc.h * nc.coslambdaBar
                                          - OH * nc.sin2lambdaBar
                                          - SS * nc.l * nc.sin3lambdaBar
                                          + SS * nc.h * nc.cos3lambdaBar);
      deltai = OH * nc.JTerm * nc.sini * nc.cosi *
         (-nc.l * nc.coslambdaBar
          + nc.h * nc.sinlambdaBar
          + nc.cos2lambdaBar
          + ST * nc.l * nc.cos3lambdaBar
          + ST * nc.h * nc.sin3lambdaBar);
      deltalambdaBar = (2.0 * nc.JsinTerm * (n * tau
                                             + SQ * nc.l * nc.sinlambdaBar
                                             - SQ * nc.h * nc.coslambdaBar))
         + (3.0 * nc.Jsini2Term * (-STF * nc.h * nc.coslambdaBar
                                   - STF * nc.l * nc.sinlambdaBar
                                   - FNSS * nc.h * nc.cos3lambdaBar
                                   + FNSS * nc.l * nc.sin3lambdaBar
                                   + OQ * nc.sin2lambdaBar))
         + (nc.Jcosi2Term * (n * tau
                             + SH * nc.l * nc.sinlambdaBar
                             - FH * nc.h * nc.coslambdaBar
                             - OH * nc.sin2lambdaBar
                             - SS * nc.l * nc.sin3lambdaBar
                             + SS * nc.h * nc.cos3lambdaBar));
      double K81 = (2.0 * nc.JsinTerm * (n * tau +
                                         SQ * nc.l * nc.sinlambdaBar -
                                         SQ * nc.h * nc.coslambdaBar));
      double Q81 = (3.0 * nc.Jsini2Term * (-STF * nc.h * nc.coslambdaBar -
                                           STF * nc.l * nc.sinlambdaBar -
                                           FNSS * nc.h * nc.cos3lambdaBar +
                                           FNSS * nc.l * nc.sin3lambdaBar +
                                           OQ * nc.sin2lambdaBar));
      double W81 = (nc.Jcosi2Term * (n * tau
                                     + SH * nc.l * nc.sinlambdaBar
                                     - FH * nc.h * nc.coslambdaBar
                                     - OH * nc.sin2lambdaBar
                                     - SS * nc.l * nc.sin3lambdaBar
                                     + SS * nc.h * nc.cos3lambdaBar));
      int m = (tau == 0) ? 1 : 2;
      cerr << "deltaa(m=" << m << ")/a = " << a_a << endl
           << "deltah(m=" << m << ") = " << deltah << endl
           << "deltal(m=" << m << ") = " << deltal << endl
           << "deltaOMEGA(m=" << m << ") = " << deltaOMEGA << endl
           << "deltai(m=" << m << ") = " << deltai << endl
           << "deltalambdaBar(m=" << m << ") = " << deltalambdaBar << endl
           << "lambdaBar(m=" << m << ") = " << nc.lambdaBar << endl
           << "h(m=" << m << ") = " << nc.h << endl
           << "l(m=" << m << ") = " << nc.l << endl
           << "tau(m=" << m << ") = " << tau << endl
           << "J(m=" << m << ") = " << J << endl
           << "e^2(m=" << m << ") = " << nc.ecc2 << endl
           << "1-e^2(m=" << m << ") = " << nc.ecc2obv << endl
           << "JTerm(m=" << m << ") = " << nc.JTerm << endl
           << "JsinTerm(m=" << m << ") = " << nc.JsinTerm << endl
           << "a(m=" << m << ") = " << a << endl;
   }
}
