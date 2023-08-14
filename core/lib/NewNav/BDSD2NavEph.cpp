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
#include "BDSD2NavEph.hpp"
#include "BDSWeekSecond.hpp"
#include "TimeString.hpp"
#include "GPS_URA.hpp"
#include "Matrix.hpp"
#include "CGCS2000Ellipsoid.hpp"

using namespace std;

namespace gnsstk
{
   BDSD2NavEph ::
   BDSD2NavEph()
         : satH1(true),
           aodc(0xff),
           aode(0xff),
           uraIndex(15),
           tgd1(std::numeric_limits<double>::quiet_NaN()),
           tgd2(std::numeric_limits<double>::quiet_NaN())
   {
      signal.messageType = NavMessageType::Ephemeris;
   }


   bool BDSD2NavEph ::
   getXvt(const CommonTime& when, Xvt& xvt, const ObsID& oid)
   {
      CGCS2000Ellipsoid ell;

      if ((signal.sat.id >= MIN_MEO_BDS) && (signal.sat.id <= MAX_MEO_BDS))
      {
            // use the standard getXvt for MEO/IGSO satellites
         return OrbitDataKepler::getXvt(when, ell, xvt);
      }
      BDSWeekSecond bdsws = Toe;
      double ToeSOW = bdsws.sow;
      double ea;              // eccentric anomaly
      double delea;           // delta eccentric anomaly during iteration
      double elapte;          // elapsed time since Toe
      double q,sinea,cosea;
      double GSTA,GCTA;
      double amm;
      double meana;           // mean anomaly
      double F,G;             // temporary real variables
      double alat,talat,c2al,s2al,du,dr,di,U,R,truea,AINC;
      double ANLON,cosu,sinu,xip,yip,can,san,cinc,sinc;
      double xef,yef,zef,dek,dlk,div,domk,duv,drv;
      double dxp,dyp,vxef,vyef,vzef;
      double xGK,yGK,zGK;

         // Compute time since ephemeris & clock epochs
      elapte = when - Toe;
      double sqrtgm = SQRT(ell.gm());

         // Compute A at time of interest
      double Ak = A + Adot * elapte;

      double twoPI = 2.0e0 * PI;
      double lecc;               // eccentricity
      double tdrinc;            // dt inclination

      lecc = ecc;
      tdrinc = idot;

         // Compute mean motion
      double dnA = dn + 0.5 * dndot * elapte;
      double Ahalf = SQRT(A);
      amm  = (sqrtgm / (A*Ahalf)) + dnA;  // NOT Ak because this equation
                                          // specifies A0, not Ak.

         // In-plane angles
         //     meana - Mean anomaly
         //     ea    - Eccentric anomaly
         //     truea - True anomaly

      meana = M0 + elapte * amm;
      meana = fmod(meana, twoPI);

      ea = meana + lecc * ::sin(meana);

      int loop_cnt = 1;
      do  {
         F = meana - ( ea - lecc * ::sin(ea));
         G = 1.0 - lecc * ::cos(ea);
         delea = F/G;
         ea = ea + delea;
         loop_cnt++;
      } while ( (fabs(delea) > 1.0e-11 ) && (loop_cnt <= 20) );

         // Compute clock corrections
      xvt.relcorr = svRelativity(when);
      xvt.clkbias = svClockBias(when);
      xvt.clkdrift = svClockDrift(when);
      xvt.frame = RefFrame(frame, when);

         // Compute true anomaly
      q     = SQRT( 1.0e0 - lecc*lecc);
      sinea = ::sin(ea);
      cosea = ::cos(ea);
      G     = 1.0e0 - lecc * cosea;

         //  G*SIN(TA) AND G*COS(TA)
      GSTA  = q * sinea;
      GCTA  = cosea - lecc;

         //  True anomaly
      truea = atan2 ( GSTA, GCTA );

         // Argument of lat and correction terms (2nd harmonic)
      alat  = truea + w;
      talat = 2.0e0 * alat;
      c2al  = ::cos( talat );
      s2al  = ::sin( talat );

      du  = c2al * Cuc +  s2al * Cus;
      dr  = c2al * Crc +  s2al * Crs;
      di  = c2al * Cic +  s2al * Cis;

         // U = updated argument of lat, R = radius, AINC = inclination
      U    = alat + du;
      R    = Ak*G  + dr;
      AINC = i0 + tdrinc * elapte  +  di;
         // At this point, the ICD formulation diverges to something
         // different.
         //  Longitude of ascending node (ANLON)
      ANLON = OMEGA0 + OMEGAdot * elapte
         - ell.angVelocity() * ToeSOW;

         // In plane location
      cosu = ::cos(U);
      sinu = ::sin(U);
      xip  = R * cosu;
      yip  = R * sinu;

         //  Angles for rotation
      can  = ::cos(ANLON);
      san  = ::sin(ANLON);
      cinc = ::cos(AINC);
      sinc = ::sin(AINC);

         // GEO satellite coordinates in user-defined inertial system
      xGK  =  xip*can  -  yip*cinc*san;
      yGK  =  xip*san  +  yip*cinc*can;
      zGK  =              yip*sinc;

         // Rz matrix
      double angleZ = ell.angVelocity() * elapte;
      double cosZ = ::cos(angleZ);
      double sinZ = ::sin(angleZ);

         // Initiailize 3X3 with all 0.0
      gnsstk::Matrix<double> matZ(3,3);
         // Row,Col
      matZ(0,0) =  cosZ;
      matZ(0,1) =  sinZ;
      matZ(0,2) =   0.0;
      matZ(1,0) = -sinZ;
      matZ(1,1) =  cosZ;
      matZ(1,2) =   0.0;
      matZ(2,0) =   0.0;
      matZ(2,1) =   0.0;
      matZ(2,2) =   1.0;

         // Rx matrix
      const double angleX = -5.0 * PI/180.0;
      double cosX = ::cos(angleX);
      double sinX = ::sin(angleX);
      gnsstk::Matrix<double> matX(3,3);
      matX(0,0) =   1.0;
      matX(0,1) =   0.0;
      matX(0,2) =   0.0;
      matX(1,0) =   0.0;
      matX(1,1) =  cosX;
      matX(1,2) =  sinX;
      matX(2,0) =   0.0;
      matX(2,1) = -sinX;
      matX(2,2) =  cosX;

         // Matrix (single column) of xGK, yGK, zGK
      gnsstk::Matrix<double> inertialPos(3,1);
      inertialPos(0,0) = xGK;
      inertialPos(1,0) = yGK;
      inertialPos(2,0) = zGK;

      gnsstk::Matrix<double> result(3,1);
      result = matZ * matX * inertialPos;

      xvt.x[0] = result(0,0);
      xvt.x[1] = result(1,0);
      xvt.x[2] = result(2,0);

         // derivatives of true anamoly and arg of latitude
      dek = amm / G;
      dlk =  amm * q / (G*G);

         // in-plane, cross-plane, and radial derivatives
      div = tdrinc - 2.0e0 * dlk * (Cic * s2al - Cis  * c2al);
      duv = dlk*(1.e0+ 2.e0 * (Cus*c2al - Cuc*s2al));
      drv = A * lecc * dek * sinea + 2.e0 * dlk * (Crs * c2al - Crc * s2al) +
         Adot * G;

         //
      dxp = drv*cosu - R*sinu*duv;
      dyp = drv*sinu + R*cosu*duv;

         // Time-derivative of Rz matrix
      gnsstk::Matrix<double> dmatZ(3,3);
         // Row,Col
      dmatZ(0,0) =  sinZ * -ell.angVelocity();
      dmatZ(0,1) = -cosZ * -ell.angVelocity();
      dmatZ(0,2) =   0.0;
      dmatZ(1,0) =  cosZ * -ell.angVelocity();
      dmatZ(1,1) =  sinZ * -ell.angVelocity();
      dmatZ(1,2) =   0.0;
      dmatZ(2,0) =   0.0;
      dmatZ(2,1) =   0.0;
      dmatZ(2,2) =   0.0;

         // Time-derivative of X,Y,Z in interial form
      gnsstk::Matrix<double> dIntPos(3,1);
      dIntPos(0,0) = - xip * san * OMEGAdot
         + dxp * can
         - yip * (cinc * can * OMEGAdot
                  -sinc * san * div )
         - dyp * cinc * san;
      dIntPos(1,0) =   xip * can * OMEGAdot
         + dxp * san
         - yip * (cinc * san * OMEGAdot
                  +sinc * can * div )
         + dyp * cinc * can;
      dIntPos(2,0) = yip * cinc * div + dyp * sinc;

      gnsstk::Matrix<double> vresult(3,1);
      vresult =  matZ * matX * dIntPos +
         dmatZ * matX * inertialPos;

         // Move results into output variables
      xvt.v[0] = vresult(0,0);
      xvt.v[1] = vresult(1,0);
      xvt.v[2] = vresult(2,0);
      switch (health)
      {
         case SVHealth::Unknown:
            xvt.health = Xvt::Unknown;
            break;
         case SVHealth::Healthy:
            xvt.health = Xvt::Healthy;
            break;
         case SVHealth::Unhealthy:
            xvt.health = Xvt::Unhealthy;
            break;
         case SVHealth::Degraded:
            xvt.health = Xvt::Degraded;
            break;
         default:
            xvt.health = Xvt::Uninitialized;
            break;
      }

      return true;
   }


   bool BDSD2NavEph ::
   validate() const
   {
      return BDSD2NavData::validate();
   }


   CommonTime BDSD2NavEph ::
   getUserTime() const
   {
         // 10 pages, 3s between = 30s
      return xmitTime + 30.0;
   }

// The BeiDou navigation message does not specify the fit interval. 
// From empirical studies, we know that the fit interval for healthy BeiDou
// navigation message data appears extend at least 2 hours after the
// t-sub-oe, which is typically aligned with the beginning of transmit time. 
//
// Therefore for healthy data in the "operationally nominal" case in which 
// the messages have data set cutovers every hour on the hour, the end-of-fit
// can be somewhere around two hours after beginning of transmit or t-sub-oe.
//
// However, there are a couple of problems with unhealthy data sets. 
//    - In some cases, an unhealthy SV continues to broadcast a given data
//      set for a long period (greater than a day). In such a case, the 
//      fit interval "expires" and the message selection process returns 
//      "no data" where the actual answer is "SV is transmitting an unhealthy indication".
//    - In some cases, an unhealthy SV broadcasts "all-zeros" following the
//      unhealthy indication.  This means the t-sub-oe is zero (which can be
//      a valid value, but only near the beginning of the week).  This leads
//      to a end-of-first aligned with 7200 SOW of the current week, 
//      regardless of where in the week the data are actually broadcast. 
//      This means a selection based on fit interval windows will NEVER select
//      this message as the begin-of-fit is after the end-of-fit. The end 
//      result is that processes are "blind" to unhealthy indications and
//      continue to "coast" on the prior message until its end-of-fit is reached.  
//      This leads to misleading results in analysis; especially in the case in 
//      which a healthy SV transitions to an unhealthy state. 
//
// There are no "good" answers to this issue. After discussion, we are going to try the following:
//    - Set the end-of-fit to the "beginning of transmit" time + SEC_PER_DAY + 30.0.  
//   
// It is hoped that this will achieve the following goals:
//    - For healthy messages in the nominal pattern of data set cutovers on the hour, the 
//      "use most recently transmitted" find algorithm (user option) will still pick 
//      the most recently transmitted, even though the preceding data set may have a fit
//      interval 23 hours later. 
//    - For unhealthy messages with invalid t-sub-oe, the condition 
//          begin-of-fit < end-of-fit will still be true.
//      Therefore, these messages will once again be visible to the find algorithm. 
//    - For unhealthy messages that are broadcast for long periods, there two cases:
//       - For a process working in near-real-time, it should always be using the 
//         most recently collected data set.  There may need to be checks to verify 
//         that the end-of-fit has not artificially expired. 
//       - For post-processing users (the largest percentage of cases), there 
//         is an interaction with how our file stores work. Most of our file stores
//         are organized by day. Therefore, if a message is broadcast continuously
//         across a day boundary, there will be a fresh copy at the beginning of 
//         each daily file.  That's the rationale for using "SEC_PER_DAY + 30.0" as 
//         opposed to "END_OF_TIME".  No one was comfortable with a default 
//         END_OF_TIME for end-of-fit.  
//
//THIS IS ONLY BEING IMPLEMENTED FOR BeiDou (both D1 and D2).
//
   void BDSD2NavEph ::
   fixFit()
   {
      beginFit = Toe - 7200.0;  // Default case

         // If elements were updated during the hour, then
         // we want to use the later time.
      if (xmitTime > Toe)
         beginFit = xmitTime;

      endFit = xmitTime + SEC_PER_DAY + 30.0; 
   }


   void BDSD2NavEph ::
   dumpSVStatus(std::ostream& s) const
   {
      const ios::fmtflags oldFlags = s.flags();
      s.setf(ios::fixed, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(0);
      s.fill(' ');
         // Use these to print subframe SOW in a formatted fashion.
         // We're not printing the week so setting the week to 0 is
         // fine.
      BDSWeekSecond ws1(0,sow);
      s << "           SV STATUS" << endl << endl
        << "Health bit (SatH1)  :    0x" << hex << (unsigned)satH1 << dec
        << endl
        << "AODC                : " << setw(6) << getAOD(aodc) << " hours ("
        << (unsigned)aodc << ")" << endl
        << "AODE                : " << setw(6) << getAOD(aode) << " hours ("
        << (unsigned)aode << ")" << endl
        << "URA index           : " << setw(6) << (unsigned)uraIndex << endl
        << "URA (nominal)       : " << setw(6) << fixed
        << SV_ACCURACY_GPS_NOMINAL_INDEX[uraIndex] << " m" << endl
        << "Health              : " << setw(9)
        << gnsstk::StringUtils::asString(health) << endl
        << "Tgd1                : " << setw(13) << setprecision(6)
        << scientific << tgd1 << " sec" << endl
        << "Tgd2                : " << setw(13) << setprecision(6)
        << scientific << tgd2 << " sec" << endl << endl
        << "           SUBFRAME OVERHEAD" << endl << endl
        << "               SOW    DOW:HH:MM:SS" << endl
        << printTime(ws1, "Pg1 SOW:    %6.0g  %3a-%w:%02H:%02M:%02S\n");
      s.flags(oldFlags);
   }

   unsigned BDSD2NavEph ::
   getAOD(uint8_t aod)
   {
      if (aod < 25)
         return aod;
      if (aod < 32)
         return 24 * (aod-23);
      return (unsigned)-1;
   }
}
