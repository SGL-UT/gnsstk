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
#include "GLOFNavEph.hpp"
#include "TimeString.hpp"
#include "PZ90Ellipsoid.hpp"
#include "YDSTime.hpp"

using namespace std;

namespace gnsstk
{
   GLOFNavEph ::
   GLOFNavEph()
         : clkBias(std::numeric_limits<double>::quiet_NaN()),
           freqBias(std::numeric_limits<double>::quiet_NaN()),
           healthBits(-1),
           tb(-1),
           P1(-1), P2(-1), P3(-1), P4(-1),
           interval(-1),
           opStatus(PCode::Unknown),
           tauDelta(std::numeric_limits<double>::quiet_NaN()),
           aod(-1),
           accIndex(-1),
           dayCount(-1),
           step(1.0)
   {
      signal.messageType = NavMessageType::Ephemeris;
      msgLenSec = 8.0;
   }


   bool GLOFNavEph ::
   validate() const
   {
         /// @todo implement some checking.
      return true;
   }


   bool GLOFNavEph ::
   getXvt(const CommonTime& when, Xvt& xvt, const ObsID& oid)
   {
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
         xvt.clkbias = clkBias + freqBias * (when - Toe) - xvt.relcorr;
         xvt.clkdrift = freqBias;
         xvt.frame = ReferenceFrame::PZ90;
         xvt.health = toXvtHealth(health);
         return true;
      }
      double px(pos[0]);   // X coordinate (km)
      double vx(vel[0]);   // X velocity   (km/s)
      double ax(acc[0]);   // X acceleration (km/s^2)
      double py(pos[1]);   // Y coordinate
      double vy(vel[1]);   // Y velocity
      double ay(acc[1]);   // Y acceleration
      double pz(pos[2]);   // Z coordinate
      double vz(vel[2]);   // Z velocity
      double az(acc[2]);   // Z acceleration
         // We will need some PZ-90 ellipsoid parameters
      PZ90Ellipsoid pz90;
      double we(pz90.angVelocity());
         // Get sidereal time at Greenwich at 0 hours UT
      double gst(getSiderealTime(Toe));
      double s0(gst*PI/12.0);
      YDSTime ytime(Toe);
      double numSeconds(ytime.sod);
      double s(s0 + we*numSeconds);
      double cs(std::cos(s));
      double ss(std::sin(s));
         // Initial state matrix
      Vector<double> initialState(6), accel(3), dxt1(6), dxt2(6), dxt3(6),
                     dxt4(6), tempRes(6);
         // Get the reference state out of GloEphemeris object data. Values
         // must be rotated from PZ-90 to an absolute coordinate system
         // Initial x coordinate (m)
      initialState(0)  = (px*cs - py*ss);
         // Initial y coordinate
      initialState(2)  = (px*ss + py*cs);
         // Initial z coordinate
      initialState(4)  = pz;
         // Initial x velocity   (m/s)
      initialState(1)  = (vx*cs - vy*ss - we*initialState(2));
         // Initial y velocity
      initialState(3)  = (vx*ss + vy*cs + we*initialState(0));
         // Initial z velocity
      initialState(5)  = vz;
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
         numSeconds += rkStep;
         s = s0 + we*(numSeconds);
         cs = std::cos(s);
         ss = std::sin(s);
            // Accelerations are computed once per iteration
         accel(0) = ax*cs - ay*ss;
         accel(1) = ax*ss + ay*cs;
         accel(2) = az;
         dxt1 = derivative(initialState, accel);
         for(int j = 0; j < 6; ++j)
         {
            tempRes(j) = initialState(j) + rkStep*dxt1(j)/2.0;
         }
         dxt2 = derivative(tempRes, accel);
         for(int j = 0; j < 6; ++j)
         {
            tempRes(j) = initialState(j) + rkStep*dxt2(j)/2.0;
         }
         dxt3 = derivative(tempRes, accel);
         for(int j = 0; j < 6; ++j)
         {
            tempRes(j) = initialState(j) + rkStep*dxt3(j);
         }
         dxt4 = derivative(tempRes, accel);
         for(int j = 0; j < 6; ++j)
         {
            initialState(j) = initialState(j) + rkStep *
               (dxt1(j) + 2.0 * (dxt2(j) + dxt3(j)) + dxt4(j)) / 6.0;
         }
            // If we are within tolerance of the target time, we are done.
         workEpoch += rkStep;
         if (std::fabs(when - workEpoch) < tolerance)
         {
            done = true;
         }
      }  // while (!done)
      px = initialState(0);
      py = initialState(2);
      pz = initialState(4);
      vx = initialState(1);
      vy = initialState(3);
      vz = initialState(5);
      xvt.x[0] = 1e3*(px*cs + py*ss);         // X coordinate
      xvt.x[1] = 1e3*(-px*ss + py*cs);          // Y coordinate
      xvt.x[2] = 1e3*pz;                        // Z coordinate
      xvt.v[0] = 1e3*(vx*cs + vy*ss + we*(xvt.x[1]/1e3)); // X velocity
      xvt.v[1] = 1e3*(-vx*ss + vy*cs - we*(xvt.x[0]/1e3)); // Y velocity
      xvt.v[2] = 1e3*vz;                        // Z velocity
         // In the GLONASS system, 'clkbias' already includes the relativistic
         // correction, therefore we must substract the late from the former.
      xvt.relcorr = xvt.computeRelativityCorrection();
      xvt.clkbias = clkBias + freqBias * (when - Toe) - xvt.relcorr;
      xvt.clkdrift = freqBias;
      xvt.frame = ReferenceFrame::PZ90;
      xvt.health = toXvtHealth(health);
      return true;
   }


   CommonTime GLOFNavEph ::
   getUserTime() const
   {
      CommonTime mr = std::max({timeStamp, xmit2, xmit3, xmit4});
      return mr + 2.0;
   }


   void GLOFNavEph ::
   fixFit()
   {
      beginFit = timeStamp;
         // See PNBGLOFNavDataFactory::processEph for more info
      unsigned kludge = (interval > 0 ? interval : 30);
         // half the interval in seconds = interval*60/2 = interval*30
      endFit = Toe + (kludge*30.0 + 30.0);
   }


   void GLOFNavEph ::
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
        << "SAT : " << signal.sat << endl << endl;

         // the rest is full details, so just return if Full is not asked for.
      if (dl != DumpDetail::Full)
         return;

      string tform("%02m/%02d/%Y %03j %02H:%02M:%02S  %7.0s  %P");
      s << "           TIMES OF INTEREST" << endl
        << "              MM/DD/YYYY DOY HH:MM:SS      SOD" << endl
        << "Begin Valid:  " << printTime(beginFit,tform) << endl
        << "Orbit Epoch:  " << printTime(Toe,tform) << endl
        << "End Valid:    " << printTime(endFit,tform) << endl
        << endl
        << "Parameter              Value" << endl;

      s.setf(ios::scientific, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(6);
      s.fill(' ');

      s << "X, X', X''  "
        << setw(16) << pos[0] << " m, "
        << setw(16) << vel[0] << " m/s, "
        << setw(16) << acc[0] << " m/s**2"
        << endl
        << "Y, Y', Y''  "
        << setw(16) << pos[1] << " m, "
        << setw(16) << vel[1] << " m/s, "
        << setw(16) << acc[1] << " m/s**2"
        << endl
        << "Z, Z', Z''  "
        << setw(16) << pos[2] << " m, "
        << setw(16) << vel[2] << " m/s, "
        << setw(16) << acc[2] << " m/s**2"
        << endl
        << "tau         " << setw(16) << clkBias  << " sec" << endl
        << "gamma       " << setw(16) << freqBias << " sec/sec" << endl
        << "tauDelta    " << setw(16) << tauDelta << " sec (interfreq bias)"
        << endl;

      s.setf(ios::fixed, ios::floatfield);
      s.precision(0);
      s << "t_b         " << setw(16) << tb << " increments" << endl
        << "P           " << setw(16) << static_cast<int>(opStatus)
        << " encoded: " << StringUtils::asString(opStatus) << endl
        << "P1          " << setw(16) <<  P1 << " encoded:"
        << " Time interval = " << interval << " minutes"
        << endl
        << "P2          " << setw(16) <<  P2 << " encoded:";
      switch (P2)
      {
         case 0:
            s << " even epoch time";
            break;
         case 1:
            s << " odd epoch time";
            break;
         default:
            s << " ?????";
            break;
      }
      s << endl
        << "P3          " << setw(16) <<  P3 << " encoded: ";
      switch (P3)
      {
         case 0:
            s << "4 satellites in almanac strings";
            break;
         case 1:
            s << "5 satellites in almanac strings";
            break;
         default:
            s << "?????";
            break;
      }
      s << endl
        << "P4          " << setw(16) << P4 << " data set change flag" << endl
        << "slot        " << setw(16) << slot << endl
        << "F_T         " << setw(16) << accIndex << " encoded, "
        << setw(5) << getAccuracy() << " m" << endl
        << "N_t         " << setw(16) << dayCount << " day of quadrennial"
        << endl
        << "E           " << setw(16) << aod << " age of info in days" << endl
        << "M           " << setw(16) << static_cast<int>(satType)
        << " encoded: " << StringUtils::asString(satType) << endl
        << "B           " << setw(16) << (unsigned)healthBits << " encoded: ";
      if (healthBits & 0x0004)
      {
         s << " MALFUNCTION";
      }
      else
      {
         s << " Functional";
      }
      s << endl
        << "l (String3) " << setw(16) << lhealth << " encoded: ";
      if (lhealth == 0)
      {
         s << " healthy";
      }
      else
      {
         s << " unhealthy";
      }
      s << endl
        << "Health (B&l)" << setw(16) << StringUtils::asString(health) << endl;
      if (!signal.obs.freqOffsWild)
      {
         s << "H           " << setw(16) << signal.obs.freqOffs
           <<" frequency number";
      }
      else
      {
         s << "H           " << "         unknown"
           <<" frequency number";
      }
      s << endl;

      s.flags(oldFlags);
   }


   void GLOFNavEph ::
   dumpTerse(std::ostream& s) const
   {
      string tform("%02H:%02M:%02S");
      string tform2("%02m/%02d/%4Y %03j %02H:%02M:%02S");
      s << "  " << setw(2) << signal.sat.id << "  "
        << printTime(beginFit,tform) << " ! "
        << printTime(Toe,tform2) << " ! "
        << printTime(endFit,tform) << "       "
        << lhealth << "     "
        << healthBits << "  "
        << endl;
   }


   double GLOFNavEph ::
   getAccuracy() const
   {
      double retVal = 0.0;
      switch (accIndex)
      {
        case  0: retVal =   1.0; break;
        case  1: retVal =   2.0; break;
        case  2: retVal =   2.5; break;
        case  3: retVal =   4.0; break;
        case  4: retVal =   5.0; break;
        case  5: retVal =   7.0; break;
        case  6: retVal =  10.0; break;
        case  7: retVal =  12.0; break;
        case  8: retVal =  14.0; break;
        case  9: retVal =  16.0; break;
        case 10: retVal =  32.0; break;
        case 11: retVal =  64.0; break;
        case 12: retVal = 128.0; break;
        case 13: retVal = 256.0; break;
        case 14: retVal = 512.0; break;
        case 15: 
        default:
           retVal = 0.0; 
      }
      return retVal; 
   }


   double GLOFNavEph ::
   getSiderealTime(const CommonTime& time)
   {
         // The following algorithm is based on the paper:
         // Aoki, S., Guinot,B., Kaplan, G. H., Kinoshita, H., McCarthy, D. D.
         //    and P.K. Seidelmann. 'The New Definition of Universal Time'.
         //    Astronomy and Astrophysics, 105, 359-361, 1982.
         // Get the Julian Day at 0 hours UT (jd)
      YDSTime ytime(time);
      double year(static_cast<double>(ytime.year));
      int doy(ytime.doy);
      int temp(static_cast<int>(floor(365.25 * (year - 1.0))) + doy);
      double jd(static_cast<double>(temp)+ 1721409.5);
         // Compute the Julian centuries (36525 days)
      double jc((jd - 2451545.0)/36525.0);
         // Compute the sidereal time, in seconds
      double sid(24110.54841 + jc * (8640184.812866
                                     + jc * (0.093104 - jc * 0.0000062)));
      sid = sid / 3600.0;
      sid = fmod(sid, 24.0);
      if(sid < 0.0)
      {
         sid = sid + 24.0;
      }
      return sid;
   } // getSidTime()


   Vector<double> GLOFNavEph ::
   derivative(const Vector<double>& inState, const Vector<double>& accel)
      const
   {
         // We will need some important PZ90 ellipsoid values
      PZ90Ellipsoid pz90;
      const double j20(pz90.j20());
      const double mu(pz90.gm_km());
      const double ae(pz90.a_km());
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
      double k1(j20*xmu*1.5*rho*rho);
      double  cm(k1*(1.0-5.0*zr2));
      double cmz(k1*(3.0-5.0*zr2));
      double k2(cm-xmu);
      double gloAx(k2*xr + accel(0));
      double gloAy(k2*yr + accel(1));
      double gloAz((cmz-xmu)*zr + accel(2));
      Vector<double> dxt(6, 0.0);
         // Let's insert data related to X coordinates
      dxt(0) = inState(1);       // Set X'  = Vx
      dxt(1) = gloAx;            // Set Vx' = gloAx
         // Let's insert data related to Y coordinates
      dxt(2) = inState(3);       // Set Y'  = Vy
      dxt(3) = gloAy;            // Set Vy' = gloAy
         // Let's insert data related to Z coordinates
      dxt(4) = inState(5);       // Set Z'  = Vz
      dxt(5) = gloAz;            // Set Vz' = gloAz
      return dxt;
   }  // derivative()
}
