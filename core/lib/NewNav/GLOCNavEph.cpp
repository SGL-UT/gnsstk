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
#include "GLOCBits.hpp"
#include "GLOCNavEph.hpp"
#include "TimeString.hpp"
#include "PZ90Ellipsoid.hpp"
#include "GLONASSTime.hpp"
#include "YDSTime.hpp"
#include "DebugTrace.hpp"

using namespace std;

namespace gnsstk
{
   const double GLOCNavEph::we = 7.2921151467e-5;


   GLOCNavEph ::
   GLOCNavEph()
         : N4(-1), NT(-1), Mj(GLOCSatType::Unknown), PS(-1), tb(-1),
           EjE(-1), EjT(-1), RjE(GLOCRegime::Unknown), RjT(GLOCRegime::Unknown),
           FjE(-1), FjT(-1),
           clkBias(std::numeric_limits<double>::quiet_NaN()),
           freqBias(std::numeric_limits<double>::quiet_NaN()),
           driftRate(std::numeric_limits<double>::quiet_NaN()),
           tauc(std::numeric_limits<double>::quiet_NaN()),
           taucdot(std::numeric_limits<double>::quiet_NaN()),
           tauDelta(std::numeric_limits<double>::quiet_NaN()),
           tauGPS(std::numeric_limits<double>::quiet_NaN()),
           step(60.0)
   {
      signal.messageType = NavMessageType::Ephemeris;
         // 3x 3 second strings.
      msgLenSec = 9.0;
   }


   bool GLOCNavEph ::
   validate() const
   {
      bool rv = (header.validate() &&
                 header11.validate() &&
                 header12.validate());
      DEBUGTRACE("GLOCNavEph::validate returning " << rv);
      return rv;
   }


   bool GLOCNavEph ::
   getXvt(const CommonTime& when, Xvt& xvt, const ObsID& oid)
   {
      DEBUGTRACE_FUNCTION();
         // If the exact epoch is found, let's return the values
      if (when == Toe)
      {
         xvt.x[0] = pos[0]*1.e3;   // m
         xvt.x[1] = pos[1]*1.e3;   // m
         xvt.x[2] = pos[2]*1.e3;   // m
         xvt.v[0] = vel[0]*1.e3;  // m/sec
         xvt.v[1] = vel[1]*1.e3;  // m/sec
         xvt.v[2] = vel[2]*1.e3;  // m/sec
            // In the GLONASS system, 'clkbias' already includes the
            // relativistic correction, therefore we must substract
            // the latter from the former.
         xvt.relcorr = xvt.computeRelativityCorrection();
            // Added negation here to match the SP3 sign
         xvt.clkbias = -(clkBias + freqBias * (when - Toe) - xvt.relcorr);
         xvt.clkdrift = freqBias;
         xvt.frame = ReferenceFrame::PZ90;
         xvt.health = toXvtHealth(header.health);
         return true;
      }
      bool simplified = (std::fabs(when - Toe) <= 900);
      Vector<double> initialState(6), accel(3), k1(6), k2(6), k3(6),
         k4(6), tempRes(6), a1(3), a23(3), a4(3);
         // Convert broadcast values from km to m, which is what the
         // differential equations use.
      initialState(0)  = pos[0]*1000.0;
      initialState(2)  = pos[1]*1000.0;
      initialState(4)  = pos[2]*1000.0;
      initialState(1)  = vel[0]*1000.0;
      initialState(3)  = vel[1]*1000.0;
      initialState(5)  = vel[2]*1000.0;
      accel(0) = acc[0]*1000.0;
      accel(1) = acc[1]*1000.0;
      accel(2) = acc[2]*1000.0;
         // Integrate satellite state to desired epoch using the given step
      double rkStep(step);
      if ((when - Toe) < 0.0)
      {
         rkStep = step*(-1.0);
      }
      CommonTime workEpoch(Toe);
      double tolerance(1e-9);
      bool done(false);
      while (!done)
      {
            // If we are about to overstep, change the stepsize appropriately
            // to hit our target final time.
         if(rkStep > 0.0)
         {
            if((workEpoch + rkStep) > when)
            {
               rkStep = (when - workEpoch);
            }
         }
         else
         {
            if ((workEpoch + rkStep) < when)
            {
               rkStep = (when - workEpoch);
            }
         }
         if (haveLTDMP())
         {
            double dt = when-Toe;
            if (std::fabs(dt) >= 900)
            {
               dt = workEpoch-Toe;
               a1 = 1000.0 * ltdmp.geta(dt);
               a23 = 1000.0 * ltdmp.geta(dt+(rkStep/2.0));
               a4 = 1000.0 * ltdmp.geta(dt+rkStep);
            }
         }
         k1 = derivative(initialState, accel, a1, simplified);
         tempRes = initialState + k1*rkStep/2.0;
         k2 = derivative(tempRes, accel, a23, simplified);
         tempRes = initialState + k2*rkStep/2.0;
         k3 = derivative(tempRes, accel, a23, simplified);
         tempRes = initialState + k3*rkStep;
         k4 = derivative(tempRes, accel, a4, simplified);
         initialState = initialState +
            (k1/6.0 + k2/3.0 + k3/3.0 + k4/6.0) * rkStep;
            // If we are within tolerance of the target time, we are done.
         workEpoch += rkStep;
         if (std::fabs(when - workEpoch) < tolerance)
         {
            done = true;
         }
      }  // while (!done)
      xvt.x[0] = initialState(0);
      xvt.x[1] = initialState(2);
      xvt.x[2] = initialState(4);
      xvt.v[0] = initialState(1);
      xvt.v[1] = initialState(3);
      xvt.v[2] = initialState(5);
         // In the GLONASS system, 'clkbias' already includes the relativistic
         // correction, therefore we must substract the late from the former.
      xvt.relcorr = xvt.computeRelativityCorrection();
         // Added negation here to match the SP3 sign
      xvt.clkbias = -(clkBias + freqBias * (when - Toe) - xvt.relcorr);
      xvt.clkdrift = freqBias;
      xvt.frame = ReferenceFrame::PZ90;
      xvt.health = toXvtHealth(header.health);
      return true;
   }


   CommonTime GLOCNavEph ::
   getUserTime() const
   {
         // I would normally use max, but ltdmp.header31/32 may not be
         // set, leaving their time system set to Unknown.  So we do
         // this the clunky way.
      CommonTime mr;
      mr.setTimeSystem(gnsstk::TimeSystem::Any);
      if ((header.xmit.getTimeSystem() != TimeSystem::Unknown) &&
          (header.xmit > mr))
      {
         mr = header.xmit;
      }
      if ((header11.xmit.getTimeSystem() != TimeSystem::Unknown) &&
          (header11.xmit > mr))
      {
         mr = header11.xmit;
      }
      if ((header12.xmit.getTimeSystem() != TimeSystem::Unknown) &&
          (header12.xmit > mr))
      {
         mr = header12.xmit;
      }
      if ((ltdmp.header31.xmit.getTimeSystem() != TimeSystem::Unknown) &&
          (ltdmp.header31.xmit > mr))
      {
         mr = ltdmp.header31.xmit;
      }
      if ((ltdmp.header32.xmit.getTimeSystem() != TimeSystem::Unknown) &&
          (ltdmp.header32.xmit > mr))
      {
         mr = ltdmp.header32.xmit;
      }
      return mr + 3.0;
   }


   void GLOCNavEph ::
   fixFit()
   {
      DEBUGTRACE_FUNCTION();
      beginFit = timeStamp;
      if (haveLTDMP() && ltdmp.validate())
      {
            // four hour fit based on the comments in Appendix J
         endFit = Toe + 4.0 * 3600.0;
      }
      else
      {
            // no long-term parameters available, we're limited to
            // 15 minutes.
         endFit = Toe + 900.0;
      }
   }


   void GLOCNavEph ::
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

      s << "****************************************************************"
        << "************" << endl
        << "GLONASS ORB/CLK (IMMEDIATE) PARAMETERS" << endl << endl
        << "SAT : " << signal.sat << endl << "      "
        << StringUtils::asString(Mj) << endl << endl;

         // the rest is full details, so just return if Full is not asked for.
      if (dl != DumpDetail::Full)
      {
         return;
      }

      header.dumpOverHeader(s);
      header.dumpStrOverhead("STR10:", s);
      header11.dumpStrOverhead("STR11:", s);
      header12.dumpStrOverhead("STR12:", s);
      if (haveLTDMP())
      {
         ltdmp.header31.dumpStrOverhead("STR31:", s);
         ltdmp.header32.dumpStrOverhead("STR32:", s);
      }

      string tform("%02m/%02d/%Y %03j %02H:%02M:%02S  %7.0s  %P");
      s << endl
        << "           TIMES OF INTEREST" << endl << endl
        << "              MM/DD/YYYY DOY HH:MM:SS      SOD" << endl
        << "Begin Valid:  " << printTime(beginFit,tform) << endl
        << "Eph Epoch:    " << printTime(Toe,tform) << endl
        << "End Valid:    " << printTime(endFit,tform) << endl
        << endl;

      s.setf(ios::scientific, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(6);
      s.fill(' ');

      s << "           CLOCK PARAMETERS" << endl << endl
        << "Bias:       " << setw(16) << clkBias  << " sec (τ)" << endl
        << "Drift:      " << setw(16) << freqBias << " sec/sec (γ)" << endl
        << "Drift rate: " << setw(16) << driftRate << " sec/(sec**2) (β)"
        << endl << endl
        << "           EPHEMERIS/CLOCK STATUS" << endl
        << endl
        << "                AGE  REGIME       ACCURACY (σ, m)" << endl
        << "Ephemeris:" << setw(9) << (unsigned)EjE << "  " << setw(10)
        << StringUtils::asString(RjE) << "  " << setw(8) << (int)FjE << " ("
        << setw(0) << setprecision(1) << fixed << factorToSigma(FjE) << ")"
        << endl
        << "Clock:    " << setw(9) << (unsigned)EjT << "  " << setw(10)
        << StringUtils::asString(RjT) << "  " << setw(8) << (int)FjT << " ("
        << setw(0) << setprecision(1) << fixed << factorToSigma(FjT) << ")"
        << endl << endl
        << scientific << setprecision(6)
        << "           ORBIT PARAMETERS" << endl << endl
        << "                           "
        << "X                  Y                  Z"
        << endl
        << "Position:   " << setw(16) << pos[0] << ",  " << setw(16) << pos[1]
        << ",  " << setw(16) << pos[2] << " km" << endl
        << "Velocity:   " << setw(16) << vel[0] << ",  " << setw(16) << vel[1]
        << ",  " << setw(16) << vel[2] << " km/sec" << endl
        << "Accel:      " << setw(16) << acc[0] << ",  " << setw(16) << acc[1]
        << ",  " << setw(16) << acc[2] << " km/sec**2" << endl
        << "Ant Offset: " << setw(16) << apcOffset[0] << ",  " << setw(16)
        << apcOffset[1] << ",  " << setw(16) << apcOffset[2] << endl
        << endl
        << "Code Bias:  " << setw(16) << tauDelta
        << " sec (Δτ, L3OCp time to L3OCd time)" << endl
        << "Time corr:  " << setw(16) << tauc << " sec  " << setw(16) << taucdot
        << " sec/sec (GLONASS to MT)" << endl
        << "Time offset:" << setw(16) << tauGPS << " sec (from GPS)" << endl
        << "Next Eph:   " << setw(16) << (unsigned)PS << " strings" << endl;

      s.setf(ios::fixed, ios::floatfield);
      s.precision(0);
      s << "t_b:        " << setw(16) << tb << " seconds (MT)" << endl
        << "Health (H): " << setw(16) << StringUtils::asString(header.health)
        << endl << endl;

      if (haveLTDMP())
      {
         s << endl << endl;
         ltdmp.dump(s);
         GLONASSTime ref31(N4, NT, ltdmp.tb31);
         GLONASSTime ref32(N4, NT, ltdmp.tb32);
         s << endl
           << "tb (31):    " << printTime(ref31,tform) << endl
           << "tb (32):    " << printTime(ref32,tform) << endl;
      }

      s.flags(oldFlags);
   }


   void GLOCNavEph ::
   dumpTerse(std::ostream& s) const
   {
      string tform("%02H:%02M:%02S");
      string tform2("%02m/%02d/%4Y %03j %02H:%02M:%02S");
      s << "  " << setw(2) << signal.sat.id << "  "
        << printTime(beginFit,tform) << " ! "
        << printTime(Toe,tform2) << " ! "
        << printTime(endFit,tform) << "       "
        << header.dataInvalid << "     "
        << header.svUnhealthy << "  "
        << endl;
   }


   Vector<double> GLOCNavEph ::
   derivative(const Vector<double>& inState, const Vector<double>& accel,
              const Vector<double>& lt, bool simplified)
      const
   {
         // We will need some important PZ90 ellipsoid values
      PZ90Ellipsoid pz90;
      const double mu = pz90.gm();          // 398600.44e9;
      const double ae = pz90.a();           // 6378136
      const double j02 = -pz90.j20();       // 1082625.7e-9
         /** @todo figure out a reasonable way to handle the fact that
          * FDMA nav uses a less precise value for omega_e.  Maybe a
          * different PZ90Ellipsoid class? */
      const double we = 7.2921151467e-5;
         // Let's start getting the current satellite position and velocity
      double  x(inState(0));          // X coordinate
      double  y(inState(2));          // Y coordinate
      double  z(inState(4));          // Z coordinate
      double r2(x*x + y*y + z*z);
      double r(std::sqrt(r2));
      double xmu(mu/r2);
      double rho(ae/r);
      double xr(x/r);
      double yr(y/r);
      double zr(z/r);
      double zr2(zr*zr);
      double k1(-1.5*j02*xmu*rho*rho);
      double  cm(k1*(1.0-5.0*zr2));
      double cmz(k1*(3.0-5.0*zr2));
      double k2(cm-xmu);
      double gloAx(k2*xr + we*we*x + 2.0*we*inState(3) + accel(0));
      double gloAy(k2*yr + we*we*y - 2.0*we*inState(1) + accel(1));
      double gloAz((cmz-xmu)*zr + accel(2));
      if (!simplified)
      {
         gloAx += lt[0];
         gloAy += lt[1];
         gloAz += lt[2];
      }
      Vector<double> dxt({
            inState(1), gloAx,
            inState(3), gloAy,
            inState(5), gloAz });
      return dxt;
   }  // derivative()


   double GLOCNavEph ::
   factorToSigma(int8_t factor)
   {
      switch (factor)
      {
         case -15: return 0.01;
         case -14: return 0.02;
         case -13: return 0.03;
         case -12: return 0.04;
         case -11: return 0.06;
         case -10: return 0.08;
         case -9:  return 0.1;
         case -8:  return 0.15;
         case -7:  return 0.2;
         case -6:  return 0.3;
         case -5:  return 0.4;
         case -4:  return 0.6;
         case -3:  return 0.7;
         case -2:  return 0.8;
         case -1:  return 0.9;
         case 0:   return 1;
         case 1:   return 2;
         case 2:   return 2.5;
         case 3:   return 4;
         case 4:   return 5;
         case 5:   return 7;
         case 6:   return 10;
         case 7:   return 12;
         case 8:   return 14;
         case 9:   return 16;
         case 10:  return 32;
         case 11:  return 64;
         case 12:  return 128;
         case 13:  return 256;
         case 14:  return 512;
         default:  return std::numeric_limits<double>::quiet_NaN();
      }
   }
}
