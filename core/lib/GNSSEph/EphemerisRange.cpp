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

/**
 * @file EphemerisRange.cpp
 * Computation of range and associated quantities from EphemerisStore,
 * given receiver position and time.
 */

#include "EphemerisRange.hpp"
#include "MiscMath.hpp"
#include "GPSEllipsoid.hpp"
#include "GNSSconstants.hpp"
#include "GPSLNavEph.hpp"
#include "TimeString.hpp"

using namespace std;
using namespace gnsstk;

namespace gnsstk
{
   // Compute the corrected range at RECEIVE time, from receiver at position Rx,
   // to the GPS satellite given by SatID sat, as well as all the CER quantities,
   // given the nominal receive time trNom and an EphemerisStore. Note that this
   // routine does not intrinsicly account for the receiver clock error
   // like the ComputeAtTransmitTime routine does.
   double CorrectedEphemerisRange::ComputeAtReceiveTime(
      const CommonTime& trNom,
      const Position& Rx,
      const SatID sat,
      NavLibrary& navLib,
      NavSearchOrder order,
      SVHealth xmitHealth,
      NavValidityType valid)
   {
      try {
         int nit;
         double tof,tof_old;
         GPSEllipsoid ellipsoid;

         nit = 0;
         tof = 0.07;       // initial guess 70ms
         do {
            // best estimate of transmit time
            transmit = trNom;
            transmit -= tof;
            tof_old = tof;
            // get SV position
            try {
                  /** @todo getXvt was expected to throw an exception on
                   * failure in the past.  This assert more or less mimics
                   * that behavior.  Refactoring is needed.  */
               GNSSTK_ASSERT(getXvt(navLib, NavSatelliteID(sat), transmit,
                                    order, xmitHealth, valid));
            }
            catch(AssertionFailure& e) {
               GNSSTK_RETHROW(e);
            }

            rotateEarth(Rx);
            // update raw range and time of flight
            rawrange = RSS(svPosVel.x[0]-Rx.X(),
                           svPosVel.x[1]-Rx.Y(),
                           svPosVel.x[2]-Rx.Z());
            tof = rawrange/ellipsoid.c();

         } while(ABS(tof-tof_old)>1.e-13 && ++nit<5);

         updateCER(Rx);

         return (rawrange-svclkbias-relativity);
      }
      catch(gnsstk::Exception& e) {
         GNSSTK_RETHROW(e);
      }
   }  // end CorrectedEphemerisRange::ComputeAtReceiveTime


      // Compute the corrected range at TRANSMIT time, from receiver at position Rx,
      // to the GPS satellite given by SatID sat, as well as all the CER quantities,
      // given the nominal receive time trNom and an EphemerisStore, as well as
      // the raw measured pseudorange.
   double CorrectedEphemerisRange::ComputeAtTransmitTime(
      const CommonTime& trNom,
      const double& pr,
      const Position& Rx,
      const SatID sat,
      NavLibrary& navLib,
      NavSearchOrder order,
      SVHealth xmitHealth,
      NavValidityType valid)
   {
      try {
         CommonTime tt;

         // 0-th order estimate of transmit time = receiver - pseudorange/c
         transmit = trNom;
         transmit -= pr/C_MPS;
         tt = transmit;

         // correct for SV clock
         for(int i=0; i<2; i++) {
            // get SV position
            try {
                  /** @todo getXvt was expected to throw an exception on
                   * failure in the past.  This assert more or less mimics
                   * that behavior.  Refactoring is needed.  */
               GNSSTK_ASSERT(getXvt(navLib, NavSatelliteID(sat), tt,
                                    order, xmitHealth, valid));
            }
            catch(InvalidRequest& e) {
               GNSSTK_RETHROW(e);
            }
            tt = transmit;
            // remove clock bias and relativity correction
            tt -= (svPosVel.clkbias + svPosVel.relcorr);
         }

         rotateEarth(Rx);
         // raw range
         rawrange = RSS(svPosVel.x[0]-Rx.X(),
                        svPosVel.x[1]-Rx.Y(),
                        svPosVel.x[2]-Rx.Z());

         updateCER(Rx);

         return (rawrange-svclkbias-relativity);
      }
      catch(gnsstk::Exception& e) {
         GNSSTK_RETHROW(e);
      }
   }  // end CorrectedEphemerisRange::ComputeAtTransmitTime


   double CorrectedEphemerisRange::ComputeAtTransmitTime(
      const CommonTime& trNom,
      const Position& Rx,
      const SatID sat,
      NavLibrary& navLib,
      NavSearchOrder order,
      SVHealth xmitHealth,
      NavValidityType valid)
   {
      try {
         gnsstk::GPSEllipsoid gm;
            /** @todo getXvt was expected to throw an exception on
             * failure in the past.  This assert more or less mimics
             * that behavior.  Refactoring is needed.  */
         GNSSTK_ASSERT(getXvt(navLib, NavSatelliteID(sat), trNom,
                              order, xmitHealth, valid));
         double pr = svPosVel.preciseRho(Rx, gm);
         return ComputeAtTransmitTime(trNom, pr, Rx, sat, navLib);
      }
      catch(gnsstk::Exception& e) {
         GNSSTK_RETHROW(e);
      }
   }


   double CorrectedEphemerisRange::ComputeAtTransmitSvTime(
      const CommonTime& ttNom,
      const double& pr,
      const Position& rx,
      const SatID sat,
      NavLibrary& navLib,
      NavSearchOrder order,
      SVHealth xmitHealth,
      NavValidityType valid)
   {
      try
      {
         Position trx(rx);
         trx.asECEF();

            /** @todo getXvt was expected to throw an exception on
             * failure in the past.  This assert more or less mimics
             * that behavior.  Refactoring is needed.  */
         GNSSTK_ASSERT(getXvt(navLib, NavSatelliteID(sat), ttNom,
                              order, xmitHealth, valid));

         // compute rotation angle in the time of signal transit

         // While this is quite similiar to rotateEarth, its not the same
         // and jcl doesn't know which is really correct
         // BWT this uses the measured pseudorange, corrected for SV clock and
         // relativity, to compute the time of flight; rotateEarth uses the value
         // computed from the receiver position and the ephemeris. They should be
         // very nearly the same, and multiplying by angVel/c should make the angle
         // of rotation very nearly identical.
         GPSEllipsoid ell;
         double range(pr/ell.c() - svPosVel.clkbias - svPosVel.relcorr);
         double rotation_angle = -ell.angVelocity() * range;
         svPosVel.x[0] = svPosVel.x[0] - svPosVel.x[1] * rotation_angle;
         svPosVel.x[1] = svPosVel.x[1] + svPosVel.x[0] * rotation_angle;
         svPosVel.x[2] = svPosVel.x[2];

         rawrange = trx.slantRange(svPosVel.x);
         updateCER(trx);

         return rawrange - svclkbias - relativity;
      }
      catch (Exception& e) {
         GNSSTK_RETHROW(e);
      }
   }


   void CorrectedEphemerisRange::updateCER(const Position& Rx)
   {
      relativity = svPosVel.computeRelativityCorrection() * C_MPS;

      svclkbias = svPosVel.clkbias * C_MPS;
      svclkdrift = svPosVel.clkdrift * C_MPS;

      cosines[0] = (Rx.X()-svPosVel.x[0])/rawrange;
      cosines[1] = (Rx.Y()-svPosVel.x[1])/rawrange;
      cosines[2] = (Rx.Z()-svPosVel.x[2])/rawrange;

      Position SV(svPosVel);
      elevation = Rx.elevation(SV);
      azimuth = Rx.azimuth(SV);
      elevationGeodetic = Rx.elevationGeodetic(SV);
      azimuthGeodetic = Rx.azimuthGeodetic(SV);
   }


   void CorrectedEphemerisRange::rotateEarth(const Position& Rx)
   {
      GPSEllipsoid ellipsoid;
      double tof = RSS(svPosVel.x[0]-Rx.X(),
                       svPosVel.x[1]-Rx.Y(),
                       svPosVel.x[2]-Rx.Z())/ellipsoid.c();
      double wt = ellipsoid.angVelocity()*tof;
      double sx =  ::cos(wt)*svPosVel.x[0] + ::sin(wt)*svPosVel.x[1];
      double sy = -::sin(wt)*svPosVel.x[0] + ::cos(wt)*svPosVel.x[1];
      svPosVel.x[0] = sx;
      svPosVel.x[1] = sy;
      sx =  ::cos(wt)*svPosVel.v[0] + ::sin(wt)*svPosVel.v[1];
      sy = -::sin(wt)*svPosVel.v[0] + ::cos(wt)*svPosVel.v[1];
      svPosVel.v[0] = sx;
      svPosVel.v[1] = sy;
   }


   bool CorrectedEphemerisRange ::
   getXvt(NavLibrary& navLib, const NavSatelliteID& sat, const CommonTime& when,
          NavSearchOrder order,
          SVHealth xmitHealth,
          NavValidityType valid)
   {
      NavMessageID nmid(sat, NavMessageType::Ephemeris);
      NavDataPtr ndp;
      std::shared_ptr<OrbitData> od;
      std::shared_ptr<GPSLNavEph> ephLNav;
      if (!navLib.find(nmid, when, ndp, xmitHealth, valid, order))
      {
         return false;
      }
      if (od = std::dynamic_pointer_cast<OrbitData>(ndp))
      {
         if (!od->getXvt(when, svPosVel))
            return false;
      }
      else
      {
            // Not orbit data? How?
         return false;
      }
      if (ephLNav = std::dynamic_pointer_cast<GPSLNavEph>(ndp))
      {
         iodc = ephLNav->iodc;
         health = ephLNav->healthBits;
      }
      return true;
   }


   double RelativityCorrection(const Xvt& svPosVel)
   {
      // relativity correction
      // dtr = -2*dot(R,V)/(c*c) = -4.4428e-10(s/sqrt(m)) * ecc * sqrt(A(m)) * sinE
      // compute it separately here, in units seconds.
      double dtr = ( -2.0 *( svPosVel.x[0] * svPosVel.v[0]
                             + svPosVel.x[1] * svPosVel.v[1]
                             + svPosVel.x[2] * svPosVel.v[2] ) / C_MPS ) / C_MPS;
      return dtr;
   }

}  // namespace gnsstk
