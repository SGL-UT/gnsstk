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
       */
   class CorrectedEphemerisRange
   {
   public:
         /// Default constructor.
      CorrectedEphemerisRange() {}

         /** Compute the corrected range at RECEIVE time, from rx to
          * sat at trNom.
          * @param[in] trNom Nominal receive time.
          * @param[in] rx Receiver position.
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @return The corrected range from rx to sat at trNom. */
      double ComputeAtReceiveTime(
         const CommonTime& trNom,
         const Position& rx,
         const SatID sat,
         NavLibrary& navLib,
         NavSearchOrder order = NavSearchOrder::User,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly);

         /** Compute the corrected range at TRANSMIT time (receiver
          * time frame), from rx to sat at trNom.
          * @param[in] trNom Nominal transmit time.
          * @param[in] pr Measured pseudorange to initialize time-of-flight.
          * @param[in] rx Receiver position.
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @return The corrected range from rx to sat at trNom. */
      double ComputeAtTransmitTime(
         const CommonTime& trNom,
         const double& pr,
         const Position& rx,
         const SatID sat,
         NavLibrary& navLib,
         NavSearchOrder order = NavSearchOrder::User,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly);

         /** Compute the corrected range at TRANSMIT time (receiver
          * time frame), from rx to sat at trNom.
          * @note This doesn't use a pseudorange to initialize the
          *   time-of-flight computation; however note that this could
          *   be problematic since the measured pseudorange includes
          *   the rx clock bias while this does not; prefer the
          *   version with measured pseudorange input.
          * @param[in] trNom Nominal transmit time.
          * @param[in] rx Receiver position.
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @return The corrected range from rx to sat at trNom. */
      double ComputeAtTransmitTime(
         const CommonTime& trNom,
         const Position& rx,
         const SatID sat,
         NavLibrary& navLib,
         NavSearchOrder order = NavSearchOrder::User,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly);

         /** Compute the corrected range at TRANSMIT time (SV time
          * frame), from rx to sat at trNom.
          * @param[in] ttNom Nominal transmit time.
          * @param[in] pr Measured pseudorange to initialize time-of-flight.
          * @param[in] rx Receiver position.
          * @param[in] sat Satellite ID to get the position for.
          * @param[in] navLib The navigation data library to use for
          *   looking up satellite XVT.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @param[in] xmitHealth The desired health status of the
          *   satellite transmitting the nav data.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @return The corrected range from rx to sat at ttNom. */
      double ComputeAtTransmitSvTime(
         const CommonTime& ttNom,
         const double& pr,
         const Position& rx,
         const SatID sat,
         NavLibrary& navLib,
         NavSearchOrder order = NavSearchOrder::User,
         SVHealth xmitHealth = SVHealth::Any,
         NavValidityType valid = NavValidityType::ValidOnly);

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
      void rotateEarth(const Position& rx);
      bool getXvt(NavLibrary& navLib, const NavSatelliteID& sat,
                  const CommonTime& when,
                  NavSearchOrder order,
                  SVHealth xmitHealth,
                  NavValidityType valid);

   }; // end class CorrectedEphemerisRange

      /// Compute relativity correction (sec.s) from the satellite
      /// position and velocity
   double RelativityCorrection(const Xvt& svPosVel);

      //@}

}  // namespace gnsstk

#endif
