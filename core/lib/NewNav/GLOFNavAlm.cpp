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
#include "GLOFNavAlm.hpp"
#include "TimeString.hpp"
#include "YDSTime.hpp"
#include "GLOFNavEph.hpp"
#include "DebugTrace.hpp"

using namespace std;

namespace gnsstk
{
   const PZ90Ellipsoid GLOFNavAlm::ell;
   const double GLOFNavAlm::mu = ell.gm_km();
   const double GLOFNavAlm::ae = ell.a_km();
   const double GLOFNavAlm::icp = 63.0 * gnsstk::PI / 180.0;
   const double GLOFNavAlm::Tcp = 43200.0;
   const double GLOFNavAlm::C20 = -1082.62575e-6;
   const double GLOFNavAlm::J = (-3.0/2.0) * C20;
   const double GLOFNavAlm::C20Term = (3.0/2.0) * C20;


   GLOFNavAlm ::
   GLOFNavAlm()
         : healthBits(false),
           taunA(std::numeric_limits<double>::quiet_NaN()),
           lambdanA(std::numeric_limits<double>::quiet_NaN()),
           deltainA(std::numeric_limits<double>::quiet_NaN()),
           eccnA(std::numeric_limits<double>::quiet_NaN()),
           omeganA(std::numeric_limits<double>::quiet_NaN()),
           tLambdanA(std::numeric_limits<double>::quiet_NaN()),
           deltaTnA(std::numeric_limits<double>::quiet_NaN()),
           deltaTdotnA(std::numeric_limits<double>::quiet_NaN()),
           freqnA(-1)
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
   getXvt(const CommonTime& when, Xvt& xvt, const ObsID& oid)
   {
      bool rv = math.getXvt(when, xvt, *this);
      xvt.health = (healthBits ? Xvt::Healthy : Xvt::Unhealthy);
      return rv;
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

      s << "**************************************************************"
        << endl
        << " GLONASS ORB/CLK (NON-IMMEDIATE) PARAMETERS"
        << endl
        << endl
        << getSignalString() << endl;

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

      s << "tau         " << setw(16) << taunA  << " sec" << endl
        << "lambda      " << setw(16) << lambdanA << " rad" << endl
        << "di          " << setw(16) << deltainA << " rad" << endl
        << "e           " << setw(16) << eccnA << " dimensionless" << endl
        << "omega       " << setw(16) << omeganA << " rad" << endl
        << "tLambda     " << setw(16) << tLambdanA << " seconds" << endl
        << "dT          " << setw(16) << deltaTnA << " sec/orbit" << endl
        << "dTd         " << setw(16) << deltaTdotnA << " sec/orbit**2" << endl;

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
        << "H           " << setw(16) << freqnA << " freq. offset" << endl
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
      ss << "    " << setw(2) << freqnA;
      ss << "    " << setw(2) << signal.sat.id;
      s << ss.str();
   }


   void GLOFNavAlm ::
   setSemiMajorAxisIncl()
   {
      DEBUGTRACE_FUNCTION();
      math.setSemiMajorAxisIncl(deltaTnA, deltainA, omeganA, eccnA);
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
      DEBUGTRACE_FUNCTION();
      h = ecc * std::sin(omega);
      l = ecc * std::cos(omega);
      ecc2 = ecc * ecc;
      ecc2obv = 1.0 - ecc2;
      DEBUGTRACE("ecc = " << ecc);
      DEBUGTRACE("omega = " << omega);
      DEBUGTRACE("h = " << h);
      DEBUGTRACE("l = " << l);
      DEBUGTRACE("ecc2 = " << ecc2);
      DEBUGTRACE("ecc2obv = " << ecc2obv);
   }


   void GLOFNavAlm::NumberCruncher ::
   setSemiMajorAxisIncl(double deltaT, double deltai, double omega, double ecc)
   {
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("deltaT = " << deltaT);
      DEBUGTRACE("deltai = " << deltai);
      DEBUGTRACE("omega = " << omega);
      DEBUGTRACE("ecc = " << ecc);
         // from GLONASS ICD appendix 3.2.2, Algorithm of calculation
      double aCurr = -9999999999999, aPrev = -9999999999999;
      setEccArgOfPerigee(ecc, omega);
      Tdeltap = Tcp + deltaT;
      n = 2.0*gnsstk::PI / Tdeltap;
      i = icp + deltai;
      sini = sin(i);
      sini2 = sini * sini;
      cosi = std::cos(i);
      cosi2 = cosi * cosi;
      double ninv = Tdeltap / (2.0*gnsstk::PI);
      DEBUGTRACE(setprecision(15) << "icp = " << icp);
      DEBUGTRACE("Delta i = " << deltai);
      DEBUGTRACE("i = " << i);
      DEBUGTRACE("sini2 = " << sini2);
      DEBUGTRACE("ninv = " << ninv);
      DEBUGTRACE("mu = " << mu);
      DEBUGTRACE("ecc2obv = " << ecc2obv);
      nu = -omega;
      double nuTerm = std::pow(1.0 + ecc * cos(nu), 3.0);
      double omegaTerm = std::pow(1.0 + ecc * cos(omega), 2.0);
      double eTerm = std::pow(ecc2obv, 3.0/2.0);
      double siniTerm = (2.0 - (5.0/2.0)*sini2);
      double bigTerm =
         (siniTerm * (eTerm / omegaTerm) + (nuTerm / ecc2obv));
      DEBUGTRACE("bigTerm = " << siniTerm << " * (" << eTerm << " / "
                 << omegaTerm << ") + (" << nuTerm << " / " << ecc2obv << ")");
         // initial approximation a^(0)
      aPrev = std::pow(ninv * ninv * mu, 1.0/3.0);
      DEBUGTRACE("a^(0) = " << aPrev);
      DEBUGTRACE("Tock_b = " << nuTerm);
      DEBUGTRACE("Tock_c = " << (nuTerm / ecc2obv));
      DEBUGTRACE("Tock_d = " << eTerm);
      DEBUGTRACE("Tock_e = " << C20Term);
      DEBUGTRACE("Tock_f = " << omegaTerm);
      DEBUGTRACE("Tock_g = " << (eTerm / omegaTerm));
      DEBUGTRACE("Tock_h = " << siniTerm);
      DEBUGTRACE("Tock_i = " << bigTerm);
      unsigned iteration = 0;
      while (fabs(aCurr-aPrev) >= 1e-3)
      {
         double pn = aPrev * ecc2obv;
            // note that I use Tdeltap / .... to match Tdeltap * ....^-1
         double tock_k = (1.0 + C20Term * (ae/pn) * (ae/pn) * bigTerm);
         double Tock = Tdeltap / tock_k;
         DEBUGTRACE("Tock_k = " << tock_k);
         DEBUGTRACE("p(" << iteration << ") = " << pn);
         DEBUGTRACE("Tock(" << (iteration+1) << ") = " << Tock);
         if (iteration > 0)
            aPrev = aCurr;
         aCurr = std::pow(std::pow(Tock/(2.0*gnsstk::PI),2.0) * mu, 1.0/3.0);
         DEBUGTRACE("a^(" << iteration << ") = " << aCurr);
         DEBUGTRACE("diff = " << fabs(aCurr-aPrev));
         iteration++;
      }
      DEBUGTRACE("a = " << aCurr << " km");
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
      DEBUGTRACE_FUNCTION();
      lambdaBar = M + omega + n * tau;
      DEBUGTRACE("M = " << M);
      DEBUGTRACE("omega = " << omega);
      DEBUGTRACE("n = " << n);
      DEBUGTRACE("tau = " << tau);
      DEBUGTRACE("n*tau = " << (n*tau));
      DEBUGTRACE("lambdaBar = " << lambdaBar);
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
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("M = " << M);
      DEBUGTRACE("omega = " << omega);
      DEBUGTRACE("a = " << a);
      DEBUGTRACE("dt = " << dt);
      Deltas m1, m2;
         // lambda Bar MUST be set just prior to calling setDeltas in
         // order to get the proper value for tau, and thus the proper
         // values for lambdaBar and derived values in place.
      setLambdaBar(M, omega, 0, n);
      m1.setDeltas(*this, a, 0, n); // tau is 0 for m=1
      setLambdaBar(M, omega, dt, n);
      m2.setDeltas(*this, a, dt, n);
      deltas = m2 - m1;
      DEBUGTRACE("deltas:");
      DEBUGTRACE("delta a = " << deltas.deltaa);
      DEBUGTRACE("delta h = " << deltas.deltah);
      DEBUGTRACE("delta l = " << deltas.deltal);
      DEBUGTRACE("delta OMEGA = " << deltas.deltaOMEGA);
      DEBUGTRACE("delta i = " << deltas.deltai);
      DEBUGTRACE("delta lambda* = " << deltas.deltalambdaBar);
   }


   double GLOFNavAlm::NumberCruncher ::
   getomegai(double hi, double li, double epsi)
   {
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE_FUNCTION();
         // expressions computed once and reused multiple times are
         // marked "optimize"
         // I think this is sufficient - the original formula appears
         // to be getting the time difference between the requested
         // and reference times, in seconds.
         // This may also be the only place where NA is implicitly used.
      DEBUGTRACE("when = " << printTime(when, "%Y/%02m/%02d %02H:%02M:%05.2f"));
      DEBUGTRACE("toa = " << printTime(alm.Toa, "%Y/%02m/%02d %02H:%02M:%05.2f"));
      double tstar = when - alm.Toa;
      double Wk = tstar / Tdeltap;
         // I'm definitely not sure about this one.
      double W;
      std::modf(Wk,&W); // W is the integer part of Wk
      double tscale = Tdeltap*W + alm.deltaTdotnA*W*W;       // optimize
      double tLambdakBar = alm.tLambdanA + tscale;
      double tLambdak = std::fmod(tLambdakBar, 86400.0);
      double OMEGAdot = C20Term * n * earthvs * cosi * std::sqrt(ecc2obv);
      double lambdak = alm.lambdanA + (OMEGAdot - omega3) * tscale;
      DEBUGTRACE("tstar = " << tstar);
      DEBUGTRACE("Wk = " << Wk);
      DEBUGTRACE("W = " << W);
      DEBUGTRACE("tlambdakBar = " << tLambdakBar);
      DEBUGTRACE("tlambdak = " << tLambdak);
      DEBUGTRACE("n = " << n);
      DEBUGTRACE("OMEGAdot = " << OMEGAdot);
      DEBUGTRACE("lambdak = " << lambdak);
      double gst = GLOFNavEph::getSiderealTime(when);
      DEBUGTRACE("gst = " << gst);
      double S0 = gst*PI/12.0;
      DEBUGTRACE("S0 = " << S0);
      double S = S0 + omega3 * (tLambdak - 10800);
      double OMEGA = lambdak + S;
      DEBUGTRACE("OMEGA = " << OMEGA);
      DEBUGTRACE("S = " << S);
      DEBUGTRACE("S0 = " << S0);
      DEBUGTRACE("C20Term = " << C20Term);
      DEBUGTRACE("earthvs = " << earthvs);
      DEBUGTRACE("ecc2obv = " << ecc2obv);
         ///////////////////////////////////////////////////////////////////////
      double tanEdiv2 = std::sqrt((1.0-alm.eccnA)/(1.0+alm.eccnA)) *
         std::tan(nu/2.0);
      double E = 2.0 * std::atan(tanEdiv2);
      double M = E - alm.eccnA * std::sin(E);
      DEBUGTRACE("tan(E/2) = " << tanEdiv2);
      DEBUGTRACE("E = " << E);
      DEBUGTRACE("M = " << M);
      setDeltas(M, alm.omeganA, a, tstar);
         ///////////////////////////////////////////////////////////////////////
      double ai = a + deltas.deltaa;
      double hi = h + deltas.deltah;
      double li = l + deltas.deltal;
      double ii = i + deltas.deltai;
      DEBUGTRACE("OMEGA = " << OMEGA);
      DEBUGTRACE("deltaOMEGA = " << deltas.deltaOMEGA);
      double OMEGAi = OMEGA + deltas.deltaOMEGA;
      double sinOMEGAi = std::sin(OMEGAi);
      double cosOMEGAi = std::cos(OMEGAi);
      double epsi = std::sqrt(hi*hi+li*li);
      double omegai = getomegai(hi, li, epsi);
      double lambdaStar = M + alm.omeganA + n*tstar + deltas.deltalambdaBar;
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
      DEBUGTRACE("hi = " << hi);
      DEBUGTRACE("li = " << li);
      DEBUGTRACE("epsi = " << epsi);
      DEBUGTRACE("omegai = " << omegai);
      DEBUGTRACE("ai = " << ai);
      DEBUGTRACE("ii = " << ii);
      DEBUGTRACE("OMEGAi = " << OMEGAi);
      DEBUGTRACE("Mi = " << Mi);
      DEBUGTRACE("lambda* = " << lambdaStar);
      DEBUGTRACE("Ein = " << Ein);
      DEBUGTRACE("nui = " << nui);
      DEBUGTRACE("ui = " << ui);
      DEBUGTRACE("ri = " << ri);
      DEBUGTRACE("Vri = " << Vri);
      DEBUGTRACE("Vui = " << Vui);
      xvt.x[0] = ri * ((cosui*cosOMEGAi) - (sinui*sinOMEGAi*cosii));
      xvt.x[1] = ri * ((cosui*sinOMEGAi) + (sinui*cosOMEGAi*cosii));
      xvt.x[2] = ri * sinui * sinii;
         // change km to m
      xvt.x[0] *= 1000.0;
      xvt.x[1] *= 1000.0;
      xvt.x[2] *= 1000.0;

      DEBUGTRACE("             sin                  cos");
      DEBUGTRACE("ii          " << sinii << "    " << cosii);
      DEBUGTRACE("ui          " << sinui << "    " << cosui);
      DEBUGTRACE("OMEGAi      " << sinOMEGAi << "    " << cosOMEGAi);
      xvt.v[0] = Vri * (cosui*cosOMEGAi - sinui*sinOMEGAi*cosii) -
         Vui * (sinui*cosOMEGAi + cosui*sinOMEGAi*cosii);
      xvt.v[1] = Vri * (cosui*sinOMEGAi + sinui*cosOMEGAi*cosii) -
         Vui * (sinui*sinOMEGAi - cosui*cosOMEGAi*cosii);
         /* debugging code
      double vyaa = (cosui*sinOMEGAi);
      double vyab = (sinui*cosOMEGAi*cosii);
      double vya = Vri * (vyaa + vyab);
      double vyba = (sinui*sinOMEGAi);
      double vybb = (-cosui*cosOMEGAi*cosii);
      double vyb = -Vui * (vyba + vybb);
      DEBUGTRACE("vyaa = " << vyaa);
      DEBUGTRACE("vyab = " << vyab);
      DEBUGTRACE("vya = " << vya);
      DEBUGTRACE("vyba = " << vyba);
      DEBUGTRACE("vybb = " << vybb);
      DEBUGTRACE("vyb = " << vyb);
         */
      xvt.v[2] = Vri * sinui * sinii + Vui * cosui * sinii;
         // change km/s to m/s
      xvt.v[0] *= 1000.0;
      xvt.v[1] *= 1000.0;
      xvt.v[2] *= 1000.0;
      xvt.frame = ReferenceFrame::PZ90;
         // clock bias and drift are not available (?).
      xvt.relcorr = xvt.computeRelativityCorrection();
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
   setDeltas(const NumberCruncher& math, double a, double tau, double n)
   {
      DEBUGTRACE_FUNCTION();
         // fractions used repeatedly.
      static constexpr double OH = 0.5;      // one half
      static constexpr double TH = 3.0/2.0;  // three halves
      static constexpr double FH = 5.0/2.0;  // five halves
      static constexpr double SH = 7.0/2.0;  // seven halves
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
      double a_a = 2.0 * math.JsinTerm * (math.l * math.coslambdaBar
                                        + math.h * math.sinlambdaBar)
         + math.Jsini2Term * (OH * math.h * math.sinlambdaBar
                            - OH * math.l * math.coslambdaBar
                            + math.cos2lambdaBar
                            + SH * math.l * math.cos3lambdaBar
                            + SH * math.h * math.sin3lambdaBar);
         // The ICD defines the abbove expression as delta a(m) / a,
         // so multiply the whole thing by a to get delta a(m)
      deltaa = a_a * a;
      deltah = math.JsinTerm * (math.l * n * tau
                              + math.sinlambdaBar
                              + TH * math.l * math.sin2lambdaBar
                              - TH * math.h * math.cos2lambdaBar)
         - OQ * math.Jsini2Term * (math.sinlambdaBar
                                 - ST * math.sin3lambdaBar
                                 + 5.0 * math.l * math.sin2lambdaBar
                                 - STH * math.l * math.sin4lambdaBar
                                 + STH * math.h * math.cos4lambdaBar
                                 + math.h * math.cos2lambdaBar)
         + math.Jcosi2Term * (math.l * n * tau
                            - OH * math.l * math.sin2lambdaBar);
      deltal = math.JsinTerm * (-math.h * n * tau
                              + math.coslambdaBar
                              + TH * math.l * math.cos2lambdaBar
                              + TH * math.h * math.sin2lambdaBar)
         - OQ * math.Jsini2Term * (-math.coslambdaBar
                                 - ST*math.cos3lambdaBar
                                 - 5.0 * math.h * math.sin2lambdaBar
                                 - STH * math.l * math.cos4lambdaBar
                                 - STH * math.h * math.sin4lambdaBar
                                 + math.l * math.cos2lambdaBar)
         + math.Jcosi2Term * (-math.h * n * tau
                            + OH * math.h * math.sin2lambdaBar);
      deltaOMEGA = -math.JTerm * math.cosi * (n * tau
                                          + SH * math.l * math.sinlambdaBar
                                          - FH * math.h * math.coslambdaBar
                                          - OH * math.sin2lambdaBar
                                          - SS * math.l * math.sin3lambdaBar
                                          + SS * math.h * math.cos3lambdaBar);
      deltai = OH * math.JTerm * math.sini * math.cosi *
         (-math.l * math.coslambdaBar
          + math.h * math.sinlambdaBar
          + math.cos2lambdaBar
          + ST * math.l * math.cos3lambdaBar
          + ST * math.h * math.sin3lambdaBar);
      deltalambdaBar = (2.0 * math.JsinTerm * (n * tau
                                             + SQ * math.l * math.sinlambdaBar
                                             - SQ * math.h * math.coslambdaBar))
         + (3.0 * math.Jsini2Term * (-STF * math.h * math.coslambdaBar
                                   - STF * math.l * math.sinlambdaBar
                                   - FNSS * math.h * math.cos3lambdaBar
                                   + FNSS * math.l * math.sin3lambdaBar
                                   + OQ * math.sin2lambdaBar))
         + (math.Jcosi2Term * (n * tau
                             + SH * math.l * math.sinlambdaBar
                             - FH * math.h * math.coslambdaBar
                             - OH * math.sin2lambdaBar
                             - SS * math.l * math.sin3lambdaBar
                             + SS * math.h * math.cos3lambdaBar));
/* Unused code, unknown purpose.
      double K81 = (2.0 * math.JsinTerm * (n * tau +
                                         SQ * math.l * math.sinlambdaBar -
                                         SQ * math.h * math.coslambdaBar));
      double Q81 = (3.0 * math.Jsini2Term * (-STF * math.h * math.coslambdaBar -
                                           STF * math.l * math.sinlambdaBar -
                                           FNSS * math.h * math.cos3lambdaBar +
                                           FNSS * math.l * math.sin3lambdaBar +
                                           OQ * math.sin2lambdaBar));
      double W81 = (math.Jcosi2Term * (n * tau
                                     + SH * math.l * math.sinlambdaBar
                                     - FH * math.h * math.coslambdaBar
                                     - OH * math.sin2lambdaBar
                                     - SS * math.l * math.sin3lambdaBar
                                     + SS * math.h * math.cos3lambdaBar));
*/
      int m = (tau == 0) ? 1 : 2;
      DEBUGTRACE("deltaa(m=" << m << ")/a = " << a_a);
      DEBUGTRACE("deltah(m=" << m << ") = " << deltah);
      DEBUGTRACE("deltal(m=" << m << ") = " << deltal);
      DEBUGTRACE("deltaOMEGA(m=" << m << ") = " << deltaOMEGA);
      DEBUGTRACE("deltai(m=" << m << ") = " << deltai);
      DEBUGTRACE("deltalambdaBar(m=" << m << ") = " << deltalambdaBar);
      DEBUGTRACE("lambdaBar(m=" << m << ") = " << math.lambdaBar);
      DEBUGTRACE("h(m=" << m << ") = " << math.h);
      DEBUGTRACE("l(m=" << m << ") = " << math.l);
      DEBUGTRACE("tau(m=" << m << ") = " << tau);
      DEBUGTRACE("J(m=" << m << ") = " << J);
      DEBUGTRACE("e^2(m=" << m << ") = " << math.ecc2);
      DEBUGTRACE("1-e^2(m=" << m << ") = " << math.ecc2obv);
      DEBUGTRACE("JTerm(m=" << m << ") = " << math.JTerm);
      DEBUGTRACE("JsinTerm(m=" << m << ") = " << math.JsinTerm);
      DEBUGTRACE("a(m=" << m << ") = " << a);
   }
}
