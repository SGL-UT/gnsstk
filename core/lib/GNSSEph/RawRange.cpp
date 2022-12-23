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

#include <tuple>
#include <limits>
#include "GNSSconstants.hpp"
#include "RawRange.hpp"
#include "Triple.hpp"
#include "CommonTime.hpp"
#include "EllipsoidModel.hpp"
#include "NavLibrary.hpp"
#include "NavSatelliteID.hpp"
#include "EllipsoidModel.hpp"
#include "SVHealth.hpp"
#include "NavValidityType.hpp"
#include "NavSearchOrder.hpp"
#include "Xvt.hpp"
#include "Position.hpp"

namespace gnsstk
{
   std::tuple<bool, double, Xvt> RawRange::fromSvPos(
      const Position& rxPos,
      const Xvt& svXvt,
      const EllipsoidModel& ellipsoid,
      bool smallAngleApprox,
      double seed,
      double threshold,
      int maxIter
   )
   {
      int iter = 0;
      double tof = seed;
      double tof_prev;
      double range;
      Xvt rotatedSvXvt;

         // Optimize `tau = range(tau) / c`
      do {
         tof_prev = tof;

         std::tie(range, rotatedSvXvt) =
            computeRange(rxPos, svXvt, tof, ellipsoid, smallAngleApprox);

         tof = range / ellipsoid.c();

      } while(ABS(tof - tof_prev) > threshold && ++iter < maxIter);

      std::tie(range, rotatedSvXvt) =
         computeRange(rxPos, svXvt, tof, ellipsoid, smallAngleApprox);

      return std::make_tuple(true, range, rotatedSvXvt);
   }

   std::tuple<bool, double, Xvt> RawRange::fromSvTransmit(
      const Position& rxPos,
      NavLibrary& navLib,
      const NavSatelliteID& sat,
      const CommonTime& transmit,
      const EllipsoidModel& ellipsoid,
      bool smallAngleApprox,
      SVHealth xmitHealth,
      NavValidityType valid,
      NavSearchOrder order,
      double seed,
      double threshold,
      int maxIter
   )
   {
      Xvt svXvt;
         // false = always use ephemeris
      if(!navLib.getXvt(sat, transmit, svXvt, false, xmitHealth, valid, order))
      {
         return std::make_tuple(false,
                                std::numeric_limits<double>::quiet_NaN(),
                                svXvt);
      }

      return fromSvPos(rxPos, svXvt, ellipsoid, smallAngleApprox,
                       seed, threshold, maxIter);
   }

   std::tuple<bool, double, Xvt> RawRange::fromSvTransmitWithObs(
      const Position& rxPos,
      double pseudorange,
      NavLibrary& navLib,
      const NavSatelliteID& sat,
      const CommonTime& transmit,
      const EllipsoidModel& ellipsoid,
      bool smallAngleApprox,
      SVHealth xmitHealth,
      NavValidityType valid,
      NavSearchOrder order
   )
   {
      Xvt svXvt;
      if(!navLib.getXvt(sat, transmit, svXvt, false, xmitHealth, valid, order))
      {
         return std::make_tuple(false,
                                std::numeric_limits<double>::quiet_NaN(),
                                svXvt);
      }

      double tof = (pseudorange / ellipsoid.c()) - svXvt.clkbias - svXvt.relcorr;

      double range;
      Xvt rotatedSvXvt;
      std::tie(range, rotatedSvXvt) =
         computeRange(rxPos, svXvt, tof, ellipsoid, smallAngleApprox);

      return std::make_tuple(true, range, rotatedSvXvt);
   }


   std::tuple<bool, double, Xvt> RawRange::fromReceive(
      const Position& rxPos,
      const CommonTime& receive,
      NavLibrary& navLib,
      const NavSatelliteID& sat,
      const EllipsoidModel& ellipsoid,
      bool smallAngleApprox,
      SVHealth xmitHealth,
      NavValidityType valid,
      NavSearchOrder order,
      double seed,
      double threshold,
      int maxIter)
   {
      bool success;
      CommonTime transmitEst;
      Xvt svXvt;

         // Estimate transmit time by iterative optimization
      std::tie(success, transmitEst) =
         estTransmitFromReceive(rxPos, receive, navLib, sat, ellipsoid,
                                 xmitHealth, valid, order, seed,
                                 threshold, maxIter);
      if(!success)
      {
         return std::make_tuple(false,
                                std::numeric_limits<double>::quiet_NaN(),
                                svXvt);
      }

         // Get SV position at estimated transmit time
      success = navLib.getXvt(sat, transmitEst, svXvt, false,
                              xmitHealth, valid, order);
      if(!success)
      {
         return std::make_tuple(false,
                                std::numeric_limits<double>::quiet_NaN(),
                                svXvt);
      }

         // Compute range between SV and RX
      double range;
      Xvt rotatedSvXvt;
      std::tie(range, rotatedSvXvt) = computeRange(rxPos, receive, svXvt,
                                                   transmitEst, ellipsoid,
                                                   smallAngleApprox);
      return std::make_tuple(true, range, rotatedSvXvt);
   }

   std::tuple<bool, double, Xvt> RawRange::fromNominalReceive(
      const Position& rxPos,
      const CommonTime& receiveNominal,
      NavLibrary& navLib,
      const NavSatelliteID& sat,
      const EllipsoidModel& ellipsoid,
      bool smallAngleApprox,
      SVHealth xmitHealth,
      NavValidityType valid,
      NavSearchOrder order,
      double seed,
      double threshold,
      int maxIter)
   {
      bool success;
      double range;
      Xvt rotatedSvXvt;

         // First estimate the range by using the receiveNominal as the
         // transmit time.
      std::tie(success, range, rotatedSvXvt) =
         fromSvTransmit(rxPos, navLib, sat, receiveNominal, ellipsoid,
                        smallAngleApprox, xmitHealth, valid, order,
                        seed, threshold, maxIter);
      if(!success)
      {
         return std::make_tuple(false,
                                std::numeric_limits<double>::quiet_NaN(),
                                rotatedSvXvt);
      }

         // Create mock pseudorange using the estimated range and clock offsets
      double fakePsuedorange =
         range - (rotatedSvXvt.clkbias + rotatedSvXvt.relcorr) * ellipsoid.c();

         // Compute range by using mock pseudorange
      std::tie(success, range, rotatedSvXvt) =
         fromNominalReceiveWithObs(rxPos, receiveNominal, fakePsuedorange,
                                    navLib, sat, ellipsoid, smallAngleApprox,
                                    xmitHealth, valid, order);
      return std::make_tuple(success, range, rotatedSvXvt);
   }

   std::tuple<bool, double, Xvt> RawRange::fromNominalReceiveWithObs(
      const Position& rxPos,
      const CommonTime& receiveNominal,
      double pseudorange,
      NavLibrary& navLib,
      const NavSatelliteID& sat,
      const EllipsoidModel& ellipsoid,
      bool smallAngleApprox,
      SVHealth xmitHealth,
      NavValidityType valid,
      NavSearchOrder order)
   {
      bool success;
      CommonTime transmitEst;
      Xvt svXvt;

         // Estimate transmit time
      std::tie(success, transmitEst) =
         estTransmitFromObs(receiveNominal, pseudorange, navLib, sat,
                            xmitHealth, valid, order);
      if(!success)
      {
         return std::make_tuple(false,
                                std::numeric_limits<double>::quiet_NaN(),
                                svXvt);
      }

         // Get SV position at transmit time
      success = navLib.getXvt(sat, transmitEst, svXvt, false,
                              xmitHealth, valid, order);
      if(!success)
      {
         return std::make_tuple(false,
                                std::numeric_limits<double>::quiet_NaN(),
                                svXvt);
      }

         // Estimate time of flight (tof)
         // Can't use receiveNominal to estimate the time of flight
         // since receiveNominal might contain a large receiver clock offset.
         // A more consistent approach is to derive tof from the "instantaneous"
         // range from the now known SV pos at transmit time
      double range;
      std::tie(range, std::ignore) =
         computeRange(rxPos, svXvt, 0, ellipsoid, smallAngleApprox);
      double tof = range / ellipsoid.c();

         // Compute range using the estimated tof
      Xvt rotatedSvXvt;
      std::tie(range, rotatedSvXvt) =
         computeRange(rxPos, svXvt, tof, ellipsoid, smallAngleApprox);

      return std::make_tuple(true, range, rotatedSvXvt);
   }


   std::tuple<bool, CommonTime> RawRange::estTransmitFromReceive(
      const Position& rxPos,
      const CommonTime& receive,
      NavLibrary& navLib,
      const NavSatelliteID& sat,
      const EllipsoidModel& ellipsoid,
      SVHealth xmitHealth,
      NavValidityType valid,
      NavSearchOrder order,
      double seed,
      double threshold,
      int maxIter)
   {
      Xvt svXvt;
      CommonTime estTransmit;
      int iter = 0;
      double tof = seed;
      double prevTof;

         // Optimize this pair of functions:
         //    `transmit = receive - tof`
         //    `tof = range(transmit, tof) / c`
      do {
         estTransmit = receive - tof;

         if(!navLib.getXvt(sat, estTransmit, svXvt,
                           false, xmitHealth, valid, order))
         {
            return std::make_tuple(false, CommonTime::BEGINNING_OF_TIME);
         }

         double range;
         std::tie(range, std::ignore) =
               computeRange(rxPos, receive, svXvt, estTransmit, ellipsoid);
         // std::tie(range, std::ignore) =
         //       computeRange(rxPos, svXvt, 0, ellipsoid);
         // std::tie(range, std::ignore) =
         //       computeRange(rxPos, svXvt, range / ellipsoid.c(), ellipsoid);


         prevTof = tof;
         tof = range / ellipsoid.c();

      } while(ABS(tof - prevTof) > threshold && ++iter < maxIter);

      return std::make_tuple(true, estTransmit);
   }


   std::tuple<bool, CommonTime> RawRange::estTransmitFromObs(
      const CommonTime& nominalReceive,
      double pseudorange,
      NavLibrary& navLib,
      const NavSatelliteID& sat,
      SVHealth xmitHealth,
      NavValidityType valid,
      NavSearchOrder order
   )
   {
      CommonTime nominalTransmit = nominalReceive - (pseudorange / C_MPS);

      Xvt svXvt;
      if(!navLib.getXvt(sat, nominalTransmit, svXvt, false,
                        xmitHealth, valid, order))
      {
         return std::make_tuple(false, CommonTime::BEGINNING_OF_TIME);
      }

      CommonTime transmit = nominalTransmit - (svXvt.clkbias + svXvt.relcorr);
      return std::make_tuple(true, transmit);
   }


   std::tuple<double, Xvt> RawRange::computeRange(
      const Position& rxPos,
      const CommonTime& receive,
      const Xvt& svXvt,
      const CommonTime& transmit,
      const EllipsoidModel& ellipsoid,
      bool smallAngleApprox)
   {
      double tof = receive - transmit;
      return computeRange(rxPos, svXvt, tof, ellipsoid, smallAngleApprox);
   }

   std::tuple<double, Xvt> RawRange::computeRange(
      const Position& rxPos,
      const Xvt& svXvt,
      double tof,
      const EllipsoidModel& ellipsoid,
      bool smallAngleApprox)
   {
      Xvt svXvtRotated = rotateECEF(svXvt, tof, ellipsoid, smallAngleApprox);
      double dist = range(rxPos, svXvtRotated.x);
      return std::make_tuple(dist, svXvtRotated);
   }

   std::tuple<double, Position> RawRange::computeRange(
      const Position& rxPos,
      const CommonTime& receive,
      const Position& svPos,
      const CommonTime& transmit,
      const EllipsoidModel& ellipsoid,
      bool smallAngleApprox)
   {
      double tof = receive - transmit;
      return computeRange(rxPos, svPos, tof, ellipsoid, smallAngleApprox);
   }

   std::tuple<double, Position> RawRange::computeRange(
      const Position& rxPos,
      const Position& svPos,
      double tof,
      const EllipsoidModel& ellipsoid,
      bool smallAngleApprox
   )
   {
      Position svPosRotated = rotateECEF(svPos, tof, ellipsoid, smallAngleApprox);
      double dist = range(rxPos, svPosRotated);
      return std::make_tuple(dist, svPosRotated);
   }

   Triple RawRange::rotateECEF(
      const Triple& vec,
      double dt,
      const EllipsoidModel& ellipsoid,
      bool smallAngleApprox)
   {
      double rotate = dt * ellipsoid.angVelocity();

      Triple vecRotated(vec);

      if(smallAngleApprox)
      {
         vecRotated[0] = vec[0] + vec[1] * rotate;
         vecRotated[1] = vec[1] - vec[0] * rotate;
         vecRotated[2] = vec[2];
      }
      else
      {
         vecRotated = vec.R3(rotate * RAD_TO_DEG);
      }

      return vecRotated;
   }

   Position RawRange::rotateECEF(
      const Position& vec,
      double dt,
      const EllipsoidModel& ellipsoid,
      bool smallAngleApprox)
   {
      Position pos(vec);
      Position::CoordinateSystem sys = pos.getCoordinateSystem();
      pos.asECEF();

      Triple rotatedVec = rotateECEF(static_cast<Triple>(pos), dt,
                                     ellipsoid, smallAngleApprox);
      pos.setECEF(rotatedVec);
      pos.transformTo(sys);

      return pos;
   }

   Xvt RawRange::rotateECEF(
      const Xvt& xvt,
      double dt,
      const EllipsoidModel& ellipsoid,
      bool smallAngleApprox)
   {
      Xvt returnXvt(xvt);
      returnXvt.x = rotateECEF(xvt.x, dt, ellipsoid, smallAngleApprox);
      returnXvt.v = rotateECEF(xvt.v, dt, ellipsoid, smallAngleApprox);
      return returnXvt;
   }


}  // namespace gnsstk
