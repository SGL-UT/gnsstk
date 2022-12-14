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
 * @file RangeRange.hpp
 * Computation of geometric ranges from transmission to reception
 */

#ifndef NEW_RAW_RANGE_HPP
#define NEW_RAW_RANGE_HPP

#include <utility>

#include "Triple.hpp"
#include "Position.hpp"
#include "CommonTime.hpp"
#include "NavLibrary.hpp"
#include "NavSatelliteID.hpp"
#include "Position.hpp"
#include "EllipsoidModel.hpp"
#include "SVHealth.hpp"
#include "NavValidityType.hpp"
#include "NavSearchOrder.hpp"

namespace gnsstk
{
      /// @ingroup GNSSEph
      //@{

      /** Compute the raw range (a.k.a. geometric range) from a receiver at
       * at receive time and an SV at transmit time.
       *
       * When working with coordinates in an ECEF frame, the rotation of the
       * frame during the transmission must be accounted for in computing the
       * distance. The range equation can be written as such:
       * @f[
       *    \rho(\tau, t_{sv}) = \biggl|\biggl|\mathbf{R3}\bigl(\tau*\omega\bigl)\cdot r^{sv}(t_{sv}) - r^{rx}\biggl|\biggl|
       * @f]
       *
       * The computeRange() methods perform this exact computation. Note that
       * a simple rotation around the Z axis can be used due to the assumption
       * of a short time of flight (<1s). This will not work for larger
       * time deltas.
       *
       * The above range equation requires knowing the transmit time  and the
       * time of flight (tof) but usually neither is known. There are several
       * RawRange methods for estimating the transmit time and tof depending on
       * the available inputs.
       * - fromSvPos(): use this if an SV position at transmit time is already
       *    known.
       * - fromSvTransmit(): the same as fromSvPos() but will look up the
       *    SV position for you.
       * - fromSvTransmitWithObs(): used with smoothed obs
       * - fromReceive(): only usable if an unbiased receive time is known
       * - fromNominalReceive(): the use case of this methodology is unknown
       * - fromNominalReceiveWithObs(): the most common case that will compute
       *    a good range solution from receiver observations
       *
       * Documentation and code in this class tries to stay consistent in
       * terminology. The table below describes the terminology.
       *
       * Term | Variable Name | Math Term | Explanation
       * ---- | ------------- | --------- | -----------
       * R3   | N/A           | @f$\mathbf{R3}@f$ | A rotation about the Z axis
       * Time of flight (tof)  | tof           | @f$\tau@f$ | The time it takes from signal transmission to signal reception
       * Angular velocity | N/A | @f$\omega@f$ | An ECEF reference frame's angular velocity, typically provided by an ellipsoid model
       * Receiver (RX) position | rxPos | @f$r^{rx}@f$ | A receiver's position in ECEF coordinates
       * SV position | svPos | @f$r^{sv}@f$ | An SV's position in ECEF coordinates
       * Nominal receive time | receiveNominal | @f$\tilde{t}_{rx}@f$ | The receive time in the time frame of the receiver.
       * Receive time | receive | @f$t_{rx}@f$ | The true receive time
       * Nominal transmit time | transmitNominal | @f$\tilde{t}_{sv}@f$ | The transmit time in the time frame of the transmitter (e.g. SV)
       * Transmit time | transmit | @f$t_{sv}@f$ | The true transmit time
       * SV clock offset | N/A | @f$\Delta t_{sv}@f$ | An SV clock's offset from a true time frame (typically GPS)
       * RX relativity correction | @f$\delta t_{sv}^{rel}@f$ | A relativity correction due to the ellipictal orbits of SVs
       *
       *
       */
   class RawRange
   {
   public:

         /** Compute geometric range from an SV's known transmitting position.
          *
          * This method estimates a transmit time by holding the SV position as
          * constant and finding the optimal solution to the set of equations:
          *
          * @f[
          *    \rho(\tau) = \biggl|\biggl|\mathbf{R3}\bigl(\tau*\omega\bigl)\cdot r^{sv} - r^{rx}\biggl|\biggl|
          * @f]
          * @f[
          *    \tau = \rho(\tau) / c
          * @f]
          *
          * @param[in] rxPos A receiver's position in ECEF
          * @param[in] svXvt An SV's Xvt
          * @param[in] ellipsoid Ellipsoid model to provide an ECEF rotation rate.
          * @param[in] smallAngleApprox If true, uses the small angle
          *    approximation to skip trigonometric computations.
          *    This can be used to reduce the number of operations but
          *    sacrifice precision.
          * @param[in] seed An initial guess of the time of flight (@f$\tau@f$)
          * @param[in] threshold Convergence threshold
          * @param[in] maxIter Max number of iterations to evaluate
          * @return A tuple of success code, raw range, and rotated SV
          *    coordinates. The bool is true if successful, false if no nav
          *    data was found to compute the Xvt.
          */
      static std::tuple<bool, double, Xvt> fromSvPos(
         const Position& rxPos,
         const Xvt& svXvt,
         const EllipsoidModel& ellipsoid,
         bool smallAngleApprox = false,
         double seed = 0.07,
         double threshold = 1.e-13,
         int maxIter = 5
      );

         /** Compute geometric range from an SV's transmitting position
          *
          * This method is the same as RawRange::fromSvPos() but will look up
          * the SV position for you by the provided time and NavLibrary.
          *
          * @param[in] rxPos A receiver's position in ECEF
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] transmit The transmit time
          * @param[in] ellipsoid Ellipsoid model to provide an ECEF rotation rate.
          * @param[in] smallAngleApprox If true, uses the small angle
          *    approximation to skip trigonometric computations.
          *    This can be used to reduce the number of operations but
          *    sacrifice precision.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] seed An initial guess of the time of flight (@f$\tau@f$)
          * @param[in] threshold Convergence threshold
          * @param[in] maxIter Max number of iterations to evaluate
          * @return A tuple of success code, raw range, and rotated SV
          *    coordinates. The bool is true if successful, false if no nav
          *    data was found to compute the Xvt.
          */
      static std::tuple<bool, double, Xvt> fromSvTransmit(
         const Position& rxPos,
         NavLibrary& navLib,
         const NavSatelliteID& sat,
         const CommonTime& transmit,
         const EllipsoidModel& ellipsoid,
         bool smallAngleApprox = false,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly,
         NavSearchOrder order = NavSearchOrder::User,
         double seed = 0.07,
         double threshold = 1.e-13,
         int maxIter = 5
      );


         /** Compute geometric range at a known receive time
          *
          * @param[in] rxPos A receiver's position in ECEF
          * @param[in] receive The receive time
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] ellipsoid Ellipsoid model to provide an ECEF rotation rate.
          * @param[in] smallAngleApprox If true, uses the small angle
          *    approximation to skip trigonometric computations.
          *    This can be used to reduce the number of operations but
          *    sacrifice precision.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] seed An initial guess of the time of flight (@f$\tau@f$)
          * @param[in] threshold Convergence threshold
          * @param[in] maxIter Max number of iterations to evaluate
          * @return A tuple of success code, raw range, and rotated SV
          *    coordinates. The bool is true if successful, false if no nav
          *    data was found to compute the Xvt.
          */
      static std::tuple<bool, double, Xvt> fromReceive(
         const Position& rxPos,
         const CommonTime& receive,
         NavLibrary& navLib,
         const NavSatelliteID& sat,
         const EllipsoidModel& ellipsoid,
         bool smallAngleApprox = false,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly,
         NavSearchOrder order = NavSearchOrder::User,
         double seed = 0.07,
         double threshold = 1.e-13,
         int maxIter = 5
      );

         /** Compute geometric range from receiver observations
          *
          * This is the common approach to computing raw range when provided
          * receiver observations. It uses estTransmitFromObs to generate
          * a transmit time and then compute raw range.
          *
          * @param[in] rxPos A receiver's position in ECEF
          * @param[in] receiveNominal The receive time in the time frame of
          *    the receiver
          * @param[in] pseudorange Measured pseudorange at the nominal receive
          *    time
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] ellipsoid Ellipsoid model to provide an ECEF rotation rate.
          * @param[in] smallAngleApprox If true, uses the small angle
          *    approximation to skip trigonometric computations.
          *    This can be used to reduce the number of operations but
          *    sacrifice precision.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @return A tuple of success code, raw range, and rotated SV
          *    coordinates. The bool is true if successful, false if no nav
          *    data was found to compute the Xvt.
          */
      static std::tuple<bool, double, Xvt> fromNominalReceiveWithObs(
         const Position& rxPos,
         const CommonTime& receiveNominal,
         double pseudorange,
         NavLibrary& navLib,
         const NavSatelliteID& sat,
         const EllipsoidModel& ellipsoid,
         bool smallAngleApprox = false,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly,
         NavSearchOrder order = NavSearchOrder::User
      );

         /** Compute geometric range using a known transmit time and
          * pseudorange measurement.
          *
          * This is specifically used for smoothed observations which are time
          * stamped by SV transmit time.
          *
          * @param[in] rxPos A receiver's position in ECEF
          * @param[in] pseudorange A measured pseudorange
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] ellipsoid Ellipsoid model to provide an ECEF rotation rate.
          * @param[in] smallAngleApprox If true, uses the small angle
          *    approximation to skip trigonometric computations.
          *    This can be used to reduce the number of operations but
          *    sacrifice precision.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @return A tuple of success code, raw range, and rotated SV
          *    coordinates. The bool is true if successful, false if no nav
          *    data was found to compute the Xvt.
          */
      static std::tuple<bool, double, Xvt> fromSvTransmitWithObs(
         const Position& rxPos,
         double pseudorange,
         NavLibrary& navLib,
         const NavSatelliteID& sat,
         const CommonTime& transmit,
         const EllipsoidModel& ellipsoid,
         bool smallAngleApprox = false,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly,
         NavSearchOrder order = NavSearchOrder::User
      );

         /** Compute geometric range from an SV's transmitting positioning
          *
          * Uses a combination of fromSvTransmit() and fromNominalReceiveWithObs
          * to estimate the range from SV to RX.
          *
          * @param[in] rxPos A receiver's position in ECEF
          * @param[in] receiveNominal The receive time in the time frame of
          *    the receiver
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] ellipsoid Ellipsoid model to provide an ECEF rotation rate.
          * @param[in] smallAngleApprox If true, uses the small angle
          *    approximation to skip trigonometric computations.
          *    This can be used to reduce the number of operations but
          *    sacrifice precision.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] seed An initial guess of the time of flight (@f$\tau@f$)
          * @param[in] threshold Convergence threshold
          * @param[in] maxIter Max number of iterations to evaluate
          * @return A tuple of success code, raw range, and rotated SV
          *    coordinates. The bool is true if successful, false if no nav
          *    data was found to compute the Xvt.
          */
      static std::tuple<bool, double, Xvt> fromNominalReceive(
         const Position& rxPos,
         const CommonTime& receiveNominal,
         NavLibrary& navLib,
         const NavSatelliteID& sat,
         const EllipsoidModel& ellipsoid,
         bool smallAngleApprox = false,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly,
         NavSearchOrder order = NavSearchOrder::User,
         double seed = 0.7,
         double threshold = 1.e-13,
         int maxIter = 5
      );

         /** Estimate a transmit time using a known receive time and
          * geometric constraints.
          *
          * This method estimates a transmit time by finding the optimal
          * solution to the set of equations:
          *
          * @f[
          *    t_{sv} = t_{rx} - \tau
          * @f]
          * @f[
          *    \rho(\tau, t_{sv}) = \biggl|\biggl|\mathbf{R3}\bigl(\tau*\omega\bigl)\cdot r^{sv}(t_{sv}) - r^{rx}\biggl|\biggl|
          * @f]
          * @f[
          *    \tau = \rho(\tau, t_{sv}) / c
          * @f]
          *
          * The above equations are evaluated iteratively until an end condition
          * is met.
          *
          * @param[in] rxPos A receiver's position in ECEF
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] ellipsoid Ellipsoid model to provide an ECEF rotation rate.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] seed An initial guess of the time of flight (@f$\tau@f$)
          * @param[in] threshold Convergence threshold
          * @param[in] maxIter Max number of iterations to evaluate
          * @return A tuple of success code and estimated transmit time.
          *    The bool is true if successful, false if no nav data was found
          *    to compute the Xvt
          */
      static std::tuple<bool, CommonTime> estTransmitFromReceive(
         const Position& rxPos,
         const CommonTime& receive,
         NavLibrary& navLib,
         const NavSatelliteID& sat,
         const EllipsoidModel& ellipsoid,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly,
         NavSearchOrder order = NavSearchOrder::User,
         double seed = 0.07,
         double threshold = 1.e-13,
         int maxIter = 5
      );

         /** Estimate signal transmission time using receiver observations
          * and ephemeris.
          *
          * A pseudorange is typically formed as the difference between
          * nominal receive time and nominal transmit time, multiplied
          * by the speed of light.
          *
          * @f[
          *    p = c * (\tilde{t}_{rx} - \tilde{t}_{sv})
          * @f]
          *
          * The nominal transmit time consists of the actual transmit time
          * plus clock offsets at the time of broadcast. Clock offsets
          * are provided by SV ephemeris broadcasts.
          *
          * @f[
          *    p = c * (\tilde{t}_{rx} - t_{sv} - \Delta t_{sv} - \delta t_{sv}^{rel})
          * @f]
          *
          * Solving for transmit time, we get:
          *
          * @f[
          *    t_{sv} = \tilde{t}_{rx} - \frac{p}{c} - \Delta t_{sv} - \delta t_{sv}^{rel}
          * @f]
          *
          * @note Other biases like ISCs and second order relativity are not
          *    accounted for here. Additional corrections can be applied to the
          *    output by subtracting any additional known biases.
          *
          * @param[in] receiveNominal The receive time in the time frame of
          *    the receiver
          * @param[in] pseudorange Measured pseudorange at the nominal receive
          *    time
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @return A tuple of success code and estimated transmit time.
          *    The bool is true if successful, false if no nav data was found
          *    to compute the Xvt
          */
      static std::tuple<bool, CommonTime> estTransmitFromObs(
         const CommonTime& receiveNominal,
         double pseudorange,
         NavLibrary& navLib,
         const NavSatelliteID& sat,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly,
         NavSearchOrder order = NavSearchOrder::User
      );

         /** Compute geometric range from an SV's transmitting position
          * to a receiver's position and accounting for the rotation
          * of the ECEF frame during the time of flight.
          *
          * The computation is equiavalent to
          * @f[
          *    \rho = \biggl|\biggl|\mathbf{R3}\bigl((t_{rx} - t_{tx})*\omega)\bigl)\cdot r^{sv} - r^{rx}\biggl|\biggl|
          * @f]
          *
          * @note An approximation of ECEF rotation is used that is
          *    applicable for small time deltas (<1s). Do not use this
          *    method for larger rotations.
          *
          * @param[in] rxPos (@f$r^rx@f$) The receiver's position in ECEF
          *    coordinates
          * @param[in] receive (@f$t_{rx}@f$) The time of signal receive
          * @param[in] svPos (@f$r^sv@f$) The SV's position in ECEF coordinates
          * @param[in] transmit (@f$t_{tx}@f$) The time of signal transmit
          * @param[in] ellipsoid Used to provide an angular rate of rotation
          *    of the ECEF (@f$\omega@f$)
          * @param[in] smallAngleApprox If true, uses the small angle
          *    approximation to skip trigonometric computations.
          *    This can be used to reduce the number of operations but
          *    sacrifice precision.
          * @return A tuple of the range (@f$\rho@f$) and the rotated SV
          *    coordinates (@f$\mathbf{R3}(\tau*\omega)\cdot r^{sv}@f$)
          */
      static std::tuple<double, Xvt> computeRange(
         const Position& rxPos,
         const CommonTime& receive,
         const Xvt& svXvt,
         const CommonTime& transmit,
         const EllipsoidModel& ellipsoid,
         bool smallAngleApprox=false
      );

         /** Compute geometric range from an SV's transmitting position
          * to a receiver's position and accounting for the rotation
          * of the ECEF frame during the time of flight.
          *
          * The computation is equiavalent to
          * @f[
          *    \rho = \biggl|\biggl|\mathbf{R3}\bigl(\tau*\omega\bigl)\cdot r^{sv} - r^{rx}\biggl|\biggl|
          * @f]
          *
          * @note An approximation of ECEF rotation is used that is
          *    applicable for small time deltas (<1s). Do not use this
          *    method for larger rotations.
          *
          * @param[in] rxPos (@f$r^rx@f$) The receiver's position in ECEF
          *    coordinates
          * @param[in] svXvt The SV's position (@f$r^sv@f$) and velocity in
          *    ECEF coordinates
          * @param[in] tof (@f$\tau@f$) The time of flight between transmission
          *    and reception
          * @param[in] ellipsoid Used to provide an angular rate of rotation
          *    of the ECEF (@f$\omega@f$)
          * @param[in] smallAngleApprox If true, uses the small angle
          *    approximation to skip trigonometric computations.
          *    This can be used to reduce the number of operations but
          *    sacrifice precision.
          * @return A tuple of the range (@f$\rho@f$) and the rotated SV
          *    position and velocity
          */
      static std::tuple<double, Xvt> computeRange(
         const Position& rxPos,
         const Xvt& svXvt,
         double tof,
         const EllipsoidModel& ellipsoid,
         bool smallAngleApprox=false
      );

         /** Compute geometric range from an SV's transmitting position
          * to a receiver's position and accounting for the rotation
          * of the ECEF frame during the time of flight.
          *
          * The computation is equiavalent to
          * @f[
          *    \rho = \biggl|\biggl|\mathbf{R3}\bigl((t_{rx} - t_{tx})*\omega)\bigl)\cdot r^{sv} - r^{rx}\biggl|\biggl|
          * @f]
          *
          * @note An approximation of ECEF rotation is used that is
          *    applicable for small time deltas (<1s). Do not use this
          *    method for larger rotations.
          *
          * @param[in] rxPos (@f$r^rx@f$) The receiver's position in ECEF
          *    coordinates
          * @param[in] receive (@f$t_{rx}@f$) The time of signal receive
          * @param[in] svPos (@f$r^sv@f$) The SV's position in ECEF coordinates
          * @param[in] transmit (@f$t_{tx}@f$) The time of signal transmit
          * @param[in] ellipsoid Used to provide an angular rate of rotation
          *    of the ECEF (@f$\omega@f$)
          * @param[in] smallAngleApprox If true, uses the small angle
          *    approximation to skip trigonometric computations.
          *    This can be used to reduce the number of operations but
          *    sacrifice precision.
          * @return A tuple of the range (@f$\rho@f$) and the rotated SV
          *    coordinates (@f$\mathbf{R3}(\tau*\omega)\cdot r^{sv}@f$)
          */
      static std::tuple<double, Position> computeRange(
         const Position& rxPos,
         const CommonTime& receive,
         const Position& svPos,
         const CommonTime& transmit,
         const EllipsoidModel& ellipsoid,
         bool smallAngleApprox=false
      );

         /** Compute geometric range from an SV's transmitting position
          * to a receiver's position and accounting for the rotation
          * of the ECEF frame during the time of flight.
          *
          * The computation is equiavalent to
          * @f[
          *    \rho = \biggl|\biggl|\mathbf{R3}\bigl(\tau*\omega\bigl)\cdot r^{sv} - r^{rx}\biggl|\biggl|
          * @f]
          *
          * @note An approximation of ECEF rotation is used that is
          *    applicable for small time deltas (<1s). Do not use this
          *    method for larger rotations.
          *
          * @param[in] rxPos (@f$r^rx@f$) The receiver's position in ECEF
          *    coordinates
          * @param[in] svPos (@f$r^sv@f$) The SV's position in ECEF coordinates
          * @param[in] tof (@f$\tau@f$) The time of flight between transmission
          *    and reception
          * @param[in] ellipsoid Used to provide an angular rate of rotation
          *    of the ECEF (@f$\omega@f$)
          * @param[in] smallAngleApprox If true, uses the small angle
          *    approximation to skip trigonometric computations.
          *    This can be used to reduce the number of operations but
          *    sacrifice precision.
          * @return A tuple of the range (@f$\rho@f$) and the rotated SV
          *    coordinates (@f$\mathbf{R3}(\tau*\omega)\cdotr^{sv}@f$)
          */
      static std::tuple<double, Position> computeRange(
         const Position& rxPos,
         const Position& svPos,
         double tof,
         const EllipsoidModel& ellipsoid,
         bool smallAngleApprox=false
      );

         /** Transform ECEF coordinates into rotated ECEF frame
          *
          * The transformation is equiavalent to
          * @f[
          *    v\prime=\mathbf{R3}(dt*\omega)\cdot v
          * @f]
          *
          * @note An approximation of ECEF rotation is used that is
          *    applicable for small time deltas (<1s). Do not use this
          *    method for larger rotations.
          *
          * @param[in] vec An ECEF vector
          * @param[in] dt Time of rotation in seconds
          * @param[in] ellipsoid Provides the angular rate of the ECEF
          * @param[in] smallAngleApprox If true, uses the small angle
          *    approximation to skip trigonometric computations.
          *    This can be used to reduce the number of operations but
          *    sacrifice precision.
          * @return A copy of \a vec but with coordinates in the rotated frame
          */
      static Triple rotateECEF(
         const Triple& vec,
         double dt,
         const EllipsoidModel& ellipsoid,
         bool smallAngleApprox=false
      );

         /** Transform ECEF coordinates into rotated ECEF frame
          *
          * The transformation is equiavalent to
          * @f[
          *    v\prime=\mathbf{R3}(dt*\omega)\cdot v
          * @f]
          *
          * @note An approximation of ECEF rotation is used that is
          *    applicable for small time deltas (<1s). Do not use this
          *    method for larger rotations.
          *
          * @param[in] vec A position. If it is not in ECEF it will first
          *    be converted to ECEF before performing rotation.
          * @param[in] dt Time of rotation in seconds
          * @param[in] ellipsoid Provides the angular rate of the ECEF
          * @param[in] smallAngleApprox If true, uses the small angle
          *    approximation to skip trigonometric computations.
          *    This can be used to reduce the number of operations but
          *    sacrifice precision.
          * @return A copy \a vec but with coordinates in the rotated frame.
          *    The output Position will be in the same coordinate system
          *    as the input \a vec.
          */
      static Position rotateECEF(
         const Position& vec,
         double dt,
         const EllipsoidModel& ellipsoid,
         bool smallAngleApprox=false
      );

         /** Transform ECEF coordinates into rotated ECEF frame
          *
          * This method will transform both the \a xvt position and velocity
          * vectors to keep them consistent.
          *
          * The transformation is equiavalent to
          * @f[
          *    v\prime=\mathbf{R3}(dt*\omega)\cdot v
          * @f]
          *
          * @note An approximation of ECEF rotation is used that is
          *    applicable for small time deltas (<1s). Do not use this
          *    method for larger rotations.
          *
          * @param[in] xvt An ECEF position and velocity
          * @param[in] dt Time of rotation in seconds
          * @param[in] ellipsoid Provides the angular rate of the ECEF
          * @param[in] smallAngleApprox If true, uses the small angle
          *    approximation to skip trigonometric computations.
          *    This can be used to reduce the number of operations but
          *    sacrifice precision.
          * @return A copy of the provided Xvt but with the position and
          *    velocity vectors in the rotated ECEF frame
          */
      static Xvt rotateECEF(
         const Xvt& xvt,
         double dt,
         const EllipsoidModel& ellipsoid,
         bool smallAngleApprox=false
      );

   }; // end class RawRange

   //@}

}  // namespace gnsstk

#endif
