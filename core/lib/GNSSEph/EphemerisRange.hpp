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
 * @file EphemerisRange.hpp
 * Computation of range and associated quantities from NavLibrary
 */

#ifndef NEW_EPHEMERIS_RANGE_HPP
#define NEW_EPHEMERIS_RANGE_HPP

#include "EllipsoidModel.hpp"
#include "GPSEllipsoid.hpp"
#include "CommonTime.hpp"
#include "SatID.hpp"
#include "Position.hpp"
#include "NavLibrary.hpp"
#include "ValidType.hpp"

namespace gnsstk
{
      /// @ingroup GNSSEph
      //@{

      /** Compute the corrected range from receiver at position Rx, to
       * the GPS satellite given by SatID sat, as well as azimuth,
       * elevation, etc., given a nominal timetag (either received or
       * transmitted time) and a NavLibrary.
       *
       * The corrected range is a combination of the raw range (geometric range)
       * minus SV clock offsets. The raw range can be written as:
       * @f[
       *    \rho_{rx}^{sv} = \biggl|\biggl|\mathbf{R}\bigl(-(t_r - t_t)\bigl)\cdot\mathbf{r}^{sv}(t_t) - \mathbf{r}^{rx}(t_r)\biggl|\biggl|
       * @f]
       * where, <br>
       *    <center>
       *    @f$\mathbf{R}(\cdot)@f$ = ECEF frame rotation over a given time interval <br>
       *    @f$\mathbf{r}^{sv}(t_t)@f$ = ECEF position of SV at time of signal transmission (@f$t_t@f$) <br>
       *    @f$\mathbf{r}^{rx}(t_r)@f$ = ECEF position of RX at time of signal receive (@f$t_r@f$)
       *    </center>
       *
       * The different methods in this class represent different methodologies
       * for solving the above equation depending on the available input. See
       * each method for the background in solving for geometric range.
       * \li ComputeAtReceiveTime() if only a nominal receive time is known
       * \li ComputeAtTransmitTime() if a pseudorange and nominal receive time is known
       * \li ComputeAtTransmitSvTime() if a pseudorange and nominal transmit time is known
       */
   class CorrectedEphemerisRange
   {
   public:
         /// Default constructor.
      CorrectedEphemerisRange() {}

         /** Compute the corrected range at RECEIVE time (receiver time frame),
          * from rx to sat at trNom.
          *
          * @note This does not account for a RX clock bias that might be
          *    present in trNom. If a RX clock bias is known, adjust the input
          *    timestamp accordingly before calling this method.
          *
          * This method uses an iterative solution solving for the geometric range
          * and the time of flight. An initial guess is used for the time of flight,
          * a geometric range is computed, and a new time of flight is computed
          * with the relationship of @f$\tau = \frac{\rho_{rx}^{sv}}{c}@f$
          *
          * @param[in] trNom Nominal receive time (@f$\widetilde{t}_r^{rx}@f$).
          * @param[in] rx Receiver position (@f$\mathbf{r}^{sv}(t_t)@f$).
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] ellipsoid Ellipsoid model to provide an ECEF rotation rate.
          * @return The corrected range from rx to sat at trNom. */
      double ComputeAtReceiveTime(
         const CommonTime& trNom,
         const Position& rx,
         const SatID sat,
         NavLibrary& navLib,
         NavSearchOrder order = NavSearchOrder::User,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly,
         const EllipsoidModel& ellipsoid = GPSEllipsoid());

         /** Compute the corrected range at TRANSMIT time (receiver time frame)
          * from rx to sat at trNom.
          *
          * @note The name of the method is slightly misleading. This uses a
          *    RX's nominal receive time to compute the distance between a RX
          *    at nominal receive time and an SV at <em>transmit</em> time.

          * This uses a simple rearrangement of the RX pseudorange equation
          * to determine a nominal time of signal transmission (@f$t_t@f$).
          * @f{eqnarray*}{
          *    p &=& c(\widetilde{t}_r^{rx} - \widetilde{t}_t^{sv}) \\
          * \Rightarrow \widetilde{t}_t^{sv} &=& \widetilde{t}_r^{rx} - p/c
          * @f}
          *
          * The nominal transmission time is corrected with SV clock offsets to
          * compute the true transmission time.
          * @f{eqnarray*}{
          *    t_t &=& \widetilde{t}_t^{sv} - dt^{sv}(\widetilde{t}_t^{sv}) \\
          *        &=& \widetilde{t}_r^{rx} - p/c - dt^{sv}(\widetilde{t}_t^{sv})
          * @f}
          *
          * Note that any RX clock bias drops out as the bias is present in both
          * the RX's timestamp (@f$\widetilde{t}_r^{rx}@f$) and in the pseudorange.
          * With the transmit time computed, a geometric range is computed using
          * the equation described in class description.
          *
          * @param[in] trNom Nominal recieve time (@f$\widetilde{t}_r^{rx}@f$).
          * @param[in] pr Measured pseudorange to initialize time-of-flight (@f$p@f$).
          * @param[in] rx Receiver position (@f$\mathbf{r}^{sv}(t_t)@f$).
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] ellipsoid Ellipsoid model to provide an ECEF rotation rate.
          * @return The corrected range from rx to sat at trNom. */
      double ComputeAtTransmitTime(
         const CommonTime& trNom,
         const double& pr,
         const Position& rx,
         const SatID sat,
         NavLibrary& navLib,
         NavSearchOrder order = NavSearchOrder::User,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly,
         const EllipsoidModel& ellipsoid = GPSEllipsoid());

         /** Compute the corrected range at TRANSMIT time (receiver
          * time frame), from rx to sat at trNom.
          *
          * @parblock
          * @note The name of the method is slightly misleading. This uses a
          *    RX's apparent receive time to compute the distance between a RX
          *    at apparent receive time and an SV at <em>transmit</em> time.
          * @endparblock
          *
          * @parblock
          * @note This doesn't use a pseudorange to initialize the
          *   time-of-flight computation; however note that this could
          *   be problematic since the measured pseudorange includes
          *   the rx clock bias while this does not; prefer the
          *   version with measured pseudorange input.
          * @endparblock
          *
          * This uses the same methodology as ComputeAtTransmitTime with the
          * pseudorange input, but instead of a using a direct pseudorange
          * obseravation one is generated using the provided receiver position
          * and the SV's position at the nominal receive time.
          *
          * @param[in] trNom Nominal receive time (@f$\widetilde{t}_r^{rx}@f$).
          * @param[in] rx Receiver position (@f$\mathbf{r}^{sv}(t_t)@f$).
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] ellipsoid Ellipsoid model to provide an ECEF rotation rate.
          * @return The corrected range from rx to sat at trNom. */
      double ComputeAtTransmitTime(
         const CommonTime& trNom,
         const Position& rx,
         const SatID sat,
         NavLibrary& navLib,
         NavSearchOrder order = NavSearchOrder::User,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly,
         const EllipsoidModel& ellipsoid = GPSEllipsoid());

         /** Compute the corrected range at TRANSMIT time (SV time
          * frame), from rx to sat at ttNom.
          *
          * A similar methodology to ComputeAtTransmitTime but uses the SV's
          * nominal transmit time and a pseudorange to estimate the receive
          * time then geometric range.
          *
          * @param[in] ttNom Nominal transmit time (@f$\widetilde{t}_t^{sv}@f$).
          * @param[in] pr Measured pseudorange to initialize time-of-flight (@f$p@f$).
          * @param[in] rx Receiver position (@f$\mathbf{r}^{sv}(t_t)@f$).
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] ellipsoid Ellipsoid model to provide an ECEF rotation rate.
          * @return The corrected range from rx to sat at ttNom. */
      double ComputeAtTransmitSvTime(
         const CommonTime& ttNom,
         const double& pr,
         const Position& rx,
         const SatID sat,
         NavLibrary& navLib,
         NavSearchOrder order = NavSearchOrder::User,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly,
         const EllipsoidModel& ellipsoid = GPSEllipsoid());

         /// The computed raw (geometric) range in meters.
      double rawrange;
         /// The satellite clock bias in meters.
      double svclkbias;
         /// The satellite clock drift in m/s.
      double svclkdrift;
         /// The relativity correction in meters.
      double relativity;
         /// The satellite elevation (spheroidal), as seen at the
         /// receiver, in degrees.
      double elevation;
         /// The satellite azimuth (spheroidal), as seen at the
         /// receiver, in degrees.
      double azimuth;
         /// The satellite elevation (geodetic), as seen at the
         /// receiver, in degrees.
      double elevationGeodetic;
         /// The satellite azimuth (geodetic), as seen at the
         /// receiver, in degrees.
      double azimuthGeodetic;
         /// The computed transmit time of the signal.
      CommonTime transmit;
         /// The direction cosines of the satellite, as seen at the
         /// receiver (XYZ).
      Triple cosines;
         /// The satellite position (m) and velocity (m/s) in ECEF coordinates.
      Xvt svPosVel;
         /// The IODC of the GPS LNAV ephemeris, invalid for other GNSSes
      vshort iodc;
         /** The health bits from the GPS LNAV ephemeris, invalid for
          * other GNSSes */
      vshort health;

   private:
         // These are just helper functions to keep from repeating code
      void updateCER(const Position& rx);

   }; // end class CorrectedEphemerisRange

      /// Compute relativity correction (sec.s) from the satellite
      /// position and velocity
   double RelativityCorrection(const Xvt& svPosVel);

      //@}

}  // namespace gnsstk

#endif
