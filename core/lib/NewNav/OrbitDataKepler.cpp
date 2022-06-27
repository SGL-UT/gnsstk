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
#include <math.h> // trig functions
#include "OrbitDataKepler.hpp"
#include "GPSWeekSecond.hpp"
#include "GPSEllipsoid.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   OrbitDataKepler ::
   OrbitDataKepler()
         : Cuc(0.0), Cus(0.0), Crc(0.0), Crs(0.0), Cic(0.0), Cis(0.0), M0(0.0),
           dn(0.0), dndot(0.0), ecc(0.0), A(0.0), Ahalf(0.0), Adot(0.0),
           OMEGA0(0.0), i0(0.0), w(0.0), OMEGAdot(0.0), idot(0.0), af0(0.0),
           af1(0.0), af2(0.0), health(SVHealth::Unknown),
           frame(ReferenceFrame::WGS84)
   {
   }


   void OrbitDataKepler ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      if (dl == DumpDetail::OneLine)
      {
         s << printTime(timeStamp, dumpTimeFmtBrief) << " " << signal
           << (signal.messageType == NavMessageType::Almanac
               ? " toa: " : " toe: ")
           << printTime(Toe, dumpTimeFmtBrief) << " "
           << gnsstk::StringUtils::asString(health) << std::endl;
         return;
      }
         // "header"
      s << "****************************************************************"
        << "************" << endl
        << "Broadcast " << getDataType() << " (Engineering Units)"
            // - " << getNameLong();
        << endl
        << endl
        << "PRN : " << setw(2) << signal.sat << " / "
        << "SVN : " << setw(2);
      std::string svn;
      if (getSVN(signal.sat, timeStamp, svn))
      {
         s << svn;
      }
      if (signal.messageType == NavMessageType::Almanac)
      {
            // for almanacs, print the transmitting satellite as well.
         s << endl
           << "XMIT: " << setw(2) << signal.xmitSat << " / "
           << "SVN : " << setw(2);
         if (getSVN(signal.xmitSat, timeStamp, svn))
         {
            s << svn;
         }
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

      dumpSVStatus(s);

      s << endl
        << "           TIMES OF INTEREST"
        << endl << endl
        << "              " << getDumpTimeHdr(dl) << endl
        << "Begin Valid:  " << getDumpTime(dl, beginFit) << endl
        << "Clock Epoch:  " << getDumpTime(dl, Toc) << endl
        << (signal.messageType == NavMessageType::Ephemeris ? "Eph" : "Alm")
        << " Epoch:    " << getDumpTime(dl, Toe) << endl
        << "End Valid:    " << getDumpTime(dl, endFit) << endl;

      s.setf(ios::scientific, ios::floatfield);
      s.precision(precision);
      s.fill(' ');

      dumpClock(s);
      dumpOrbit(s);
      dumpHarmonics(s);

      s.flags(oldFlags);
   }


   void OrbitDataKepler ::
   dumpClock(std::ostream& s) const
   {
      s << endl
        << "           CLOCK PARAMETERS"
        << endl
        << endl
        << "Bias T0:     " << setw(fw) << af0 << " sec" << endl
        << "Drift:       " << setw(fw) << af1 << " sec/sec" << endl
        << "Drift rate:  " << setw(fw) << af2 << " sec/(sec**2)" << endl;
   }


   void OrbitDataKepler ::
   dumpOrbit(std::ostream& s) const
   {
      s << endl
        << "           ORBIT PARAMETERS"
        << endl
        << endl
        << "Semi-major axis:       " << setw(fw) <<  A     << " m       "
        << setw(fw) << Adot  << "   m/sec" << endl
        << "Motion correction:     " << setw(fw) <<  dn    << " rad/sec "
        << setw(fw) << dndot << " rad/(sec**2)" << endl
        << "Eccentricity:          " << setw(fw) << ecc << endl
        << "Arg of perigee:        " << setw(fw) << w << " rad" << endl
        << "Mean anomaly at epoch: " << setw(fw) << M0 << " rad" << endl
        << "Right ascension:       " << setw(fw) << OMEGA0 << " rad     "
        << setw(fw) << OMEGAdot << " rad/sec" << endl
        << "Inclination:           " << setw(fw) << i0     << " rad     "
        << setw(fw) << idot << " rad/sec" << endl;
   }


   void OrbitDataKepler ::
   dumpHarmonics(std::ostream& s) const
   {
      s << endl
        << "           HARMONIC CORRECTIONS"
        << endl
        << endl
        << "Radial        Sine: " << setw(fw) << Crs << " m    Cosine: "
        << setw(fw) << Crc << " m" << endl
        << "Inclination   Sine: " << setw(fw) << Cis << " rad  Cosine: "
        << setw(fw) << Cic << " rad" << endl
        << "In-track      Sine: " << setw(fw) << Cus << " rad  Cosine: "
        << setw(fw) << Cuc << " rad" << endl;
   }


   bool OrbitDataKepler ::
   getXvt(const CommonTime& when, const EllipsoidModel& ell, Xvt& xvt,
          const ObsID& oid)
   {
      GPSWeekSecond gpsws = (Toe);
      double ToeSOW = gpsws.sow;
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

      double sqrtgm = SQRT(ell.gm());
      double twoPI = 2.0e0 * PI;
      double lecc;            // eccentricity
      double tdrinc;          // dt inclination

      lecc = ecc;
      tdrinc = idot;

         // Compute time since ephemeris & clock epochs
      elapte = when - Toe;

         // Compute A at time of interest
      double Ak = A + Adot * elapte;

         // Compute mean motion
      double dnA = dn + 0.5 * dndot * elapte;
         // NOT Ak because this equation specifies A0, not Ak.
      amm  = (sqrtgm / (A*Ahalf)) + dnA;

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
      xvt.relcorr = svRelativity(when, ell);
      xvt.clkbias = svClockBias(when);
      xvt.clkdrift = svClockDrift(when);
         // This appears to be only a string for naming
      xvt.frame = ReferenceFrame::WGS84;

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
      R    = Ak*G + dr;
      AINC = i0 + tdrinc * elapte  +  di;

         //  Longitude of ascending node (ANLON)
      ANLON = OMEGA0 + (OMEGAdot - ell.angVelocity()) *
         elapte - ell.angVelocity() * ToeSOW;

         // In plane location
      cosu = ::cos( U );
      sinu = ::sin( U );

      xip  = R * cosu;
      yip  = R * sinu;

         //  Angles for rotation to earth fixed
      can  = ::cos( ANLON );
      san  = ::sin( ANLON );
      cinc = ::cos( AINC  );
      sinc = ::sin( AINC  );

         // Earth fixed - meters
      xef  =  xip*can  -  yip*cinc*san;
      yef  =  xip*san  +  yip*cinc*can;
      zef  =              yip*sinc;

      xvt.x[0] = xef;
      xvt.x[1] = yef;
      xvt.x[2] = zef;

         // Compute velocity of rotation coordinates
      dek = amm / G;
      dlk = amm * q / (G*G);
      div = tdrinc - 2.0e0 * dlk *
         ( Cic  * s2al - Cis * c2al );
      domk = OMEGAdot - ell.angVelocity();
      duv = dlk*(1.e0+ 2.e0 * (Cus*c2al - Cuc*s2al) );
      drv = Ak * lecc * dek * sinea - 2.e0 * dlk *
         ( Crc * s2al - Crs * c2al ) + Adot * G;

      dxp = drv*cosu - R*sinu*duv;
      dyp = drv*sinu + R*cosu*duv;

         // Calculate velocities
      vxef = dxp*can - xip*san*domk - dyp*cinc*san
         + yip*( sinc*san*div - cinc*can*domk);
      vyef = dxp*san + xip*can*domk + dyp*cinc*can
         - yip*( sinc*can*div + cinc*san*domk);
      vzef = dyp*sinc + yip*cinc*div;

         // Move results into output variables
      xvt.v[0] = vxef;
      xvt.v[1] = vyef;
      xvt.v[2] = vzef;
      xvt.health = toXvtHealth(health);
      xvt.frame = frame;
      return true;
   }


   double OrbitDataKepler ::
   svRelativity(const CommonTime& when, const EllipsoidModel& ell) const
   {
      double twoPI  = 2.0e0 * PI;
      double sqrtgm = SQRT(ell.gm());
      double elapte = when - Toe;
      double amm    = (sqrtgm / (A*Ahalf)) + dn;
      double meana,F,G,delea;

      double Ak = A + Adot*elapte;
      meana = M0 + elapte * amm;
      meana = fmod(meana, twoPI);
      double ea = meana + ecc * ::sin(meana);

      int loop_cnt = 1;
      do {
         F     = meana - ( ea - ecc * ::sin(ea));
         G     = 1.0 - ecc * ::cos(ea);
         delea = F/G;
         ea    = ea + delea;
         loop_cnt++;
      } while ( (ABS(delea) > 1.0e-11 ) && (loop_cnt <= 20) );
      double dtr = REL_CONST * ecc * SQRT(Ak) * ::sin(ea);
      return dtr;
   }


   double OrbitDataKepler ::
   svClockBias(const CommonTime& when) const
   {
      double dtc, elaptc;
      elaptc = when - Toc;
      dtc = af0 + elaptc * ( af1 + elaptc * af2 );
      return dtc;
   }


   double OrbitDataKepler ::
   svClockDrift(const CommonTime& when) const
   {
      double drift, elaptc;
      elaptc = when - Toc;
      drift = af1 + elaptc * af2;
      return drift;
   }


   bool OrbitDataKepler ::
   isSameData(const NavDataPtr& right) const
   {
      const std::shared_ptr<OrbitDataKepler> rhs =
         std::dynamic_pointer_cast<OrbitDataKepler>(right);
      if (!rhs)
      {
            // not the same type.
         return false;
      }
      return (NavData::isSameData(right) &&
              (Toe == rhs->Toe) &&
              (Toc == rhs->Toc) &&
              (Cuc == rhs->Cuc) &&
              (Cus == rhs->Cus) &&
              (Crc == rhs->Crc) &&
              (Crs == rhs->Crs) &&
              (Cic == rhs->Cic) &&
              (Cis == rhs->Cis) &&
              (M0 == rhs->M0) &&
              (dn == rhs->dn) &&
              (dndot == rhs->dndot) &&
              (ecc == rhs->ecc) &&
              (A == rhs->A) &&
              (Ahalf == rhs->Ahalf) &&
              (Adot == rhs->Adot) &&
              (OMEGA0 == rhs->OMEGA0) &&
              (i0 == rhs->i0) &&
              (w == rhs->w) &&
              (OMEGAdot == rhs->OMEGAdot) &&
              (idot == rhs->idot) &&
              (af0 == rhs->af0) &&
              (af1 == rhs->af1) &&
              (af2 == rhs->af2));
   }


   std::list<std::string> OrbitDataKepler ::
   compare(const NavDataPtr& right) const
   {
         // OrbitData doesn't have any data, but OrbitData::compare
         // instead throws an exception if called so as to make sure
         // unimplemented children make it clear they're
         // unimplemented.  So jump directly to NavData::compare.
      std::list<std::string> rv = NavData::compare(right);
      const std::shared_ptr<OrbitDataKepler> rhs =
         std::dynamic_pointer_cast<OrbitDataKepler>(right);
      if (!rhs)
      {
            // not the same type.
         rv.push_back("CLASS");
      }
      else
      {
            // old nav implementation clearly didn't check this
            // if (xmitTime != rhs->xmitTime)
            //    rv.push_back("xmitTime");
         if (Toe != rhs->Toe)
            rv.push_back("Toe");
         if (Toc != rhs->Toc)
            rv.push_back("Toc");
         if (health != rhs->health)
            rv.push_back("health");
         if (Cuc != rhs->Cuc)
            rv.push_back("Cuc");
         if (Cus != rhs->Cus)
            rv.push_back("Cus");
         if (Crc != rhs->Crc)
            rv.push_back("Crc");
         if (Crs != rhs->Crs)
            rv.push_back("Crs");
         if (Cic != rhs->Cic)
            rv.push_back("Cic");
         if (Cis != rhs->Cis)
            rv.push_back("Cis");
         if (M0 != rhs->M0)
            rv.push_back("M0");
         if (dn != rhs->dn)
            rv.push_back("dn");
         if (dndot != rhs->dndot)
            rv.push_back("dndot");
         if (ecc != rhs->ecc)
            rv.push_back("ecc");
         if (A != rhs->A)
            rv.push_back("A");
         if (Ahalf != rhs->Ahalf)
            rv.push_back("Ahalf");
         if (Adot != rhs->Adot)
            rv.push_back("Adot");
         if (OMEGA0 != rhs->OMEGA0)
            rv.push_back("OMEGA0");
         if (i0 != rhs->i0)
            rv.push_back("i0");
         if (w != rhs->w)
            rv.push_back("w");
         if (OMEGAdot != rhs->OMEGAdot)
            rv.push_back("OMEGAdot");
         if (idot != rhs->idot)
            rv.push_back("idot");
         if (af0 != rhs->af0)
            rv.push_back("af0");
         if (af1 != rhs->af1)
            rv.push_back("af1");
         if (af2 != rhs->af2)
            rv.push_back("af2");
         if (beginFit != rhs->beginFit)
            rv.push_back("beginFit");
         if (endFit != rhs->endFit)
            rv.push_back("endFit");
      }
      return rv;
   }
}
