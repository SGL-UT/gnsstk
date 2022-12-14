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

#include <utility>

#include "EllipsoidModel.hpp"
#include "EphemerisRange.hpp"
#include "MiscMath.hpp"
#include "GNSSconstants.hpp"
#include "GPSLNavEph.hpp"
#include "TimeString.hpp"
#include "RawRange.hpp"

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
      NavValidityType valid,
      const EllipsoidModel& ellipsoid)
   {
      try {
         int nit;
         double tof,tof_old;
         bool success;
         std::tie(success, rawrange, svPosVel) =
            RawRange::fromReceive(Rx, trNom, navLib,
                                  NavSatelliteID(sat), ellipsoid);

         if(!success)
         {
            InvalidRequest ir("getXvt failed");
            GNSSTK_THROW(ir);
         }

         updateCER(Rx);
         return (rawrange-svclkbias-relativity);
      }
      catch(gnsstk::Exception& e) {
         GNSSTK_RETHROW(e);
      }
   }


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
      NavValidityType valid,
      const EllipsoidModel& ellipsoid)
   {
      try {
         bool success;
         std::tie(success, rawrange, svPosVel) =
            RawRange::fromNominalReceiveWithObs(Rx, trNom, pr, navLib,
                                                NavSatelliteID(sat), ellipsoid);
         if(!success)
         {
            InvalidRequest ir("getXvt failed");
            GNSSTK_THROW(ir);
         }

         updateCER(Rx);
         return (rawrange-svclkbias-relativity);
      }
      catch(gnsstk::Exception& e) {
         GNSSTK_RETHROW(e);
      }
   }


   double CorrectedEphemerisRange::ComputeAtTransmitTime(
      const CommonTime& trNom,
      const Position& Rx,
      const SatID sat,
      NavLibrary& navLib,
      NavSearchOrder order,
      SVHealth xmitHealth,
      NavValidityType valid,
      const EllipsoidModel& ellipsoid)
   {
      try {
         bool success;
         std::tie(success, rawrange, svPosVel) =
            RawRange::fromNominalReceive(Rx, trNom, navLib,
                                         NavSatelliteID(sat), ellipsoid);

         if(!success)
         {
            InvalidRequest ir("getXvt failed");
            GNSSTK_THROW(ir);
         }

         updateCER(Rx);

         return (rawrange - svclkbias - relativity);
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
      NavValidityType valid,
      const EllipsoidModel& ellipsoid)
   {
      try
      {
         bool success;
         std::tie(success, rawrange, svPosVel)
            = RawRange::fromSvTransmitWithObs(rx, pr, navLib,
                                              NavSatelliteID(sat), ttNom,
                                              ellipsoid, true);

         if(!success)
         {
            InvalidRequest ir("getXvt failed");
            GNSSTK_THROW(ir);
         }

         updateCER(rx);

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
