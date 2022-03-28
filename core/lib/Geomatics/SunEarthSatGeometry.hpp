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
    * @file SunEarthSatGeometry.hpp
    * Include file for various routines related to Sun-Earth-Satellite geometry,
    * including satellite attitude, XYZ->UEN rotation, and (elevation,azimuth) as
    * seen at the satellite. Used by PhaseWindup and PreciseRange, among others.
    * @note Use class SolarSystem or SolarPosition() to get Sun's position as needed.
    */

#ifndef SUN_EARTH_SATELLITE_INCLUDE
#define SUN_EARTH_SATELLITE_INCLUDE

#include "Matrix.hpp"
#include "Position.hpp"

namespace gnsstk
{
      /**
       *  Given a Position, compute unit vectors in ECEF coordinates in the North,
       *  East and Up directions at that position. Use either geodetic coordinates,
       *  i.e. so that 'up' is perpendicular to the geoid, not from center of Earth;
       *  or geocentric coordinates, so that the vertical meets the center of the Earth.
       *  Return the vectors in the form of a 3x3 Matrix<double>. This is in fact the
       *  rotation matrix that will take an ECEF (XYZ) vector into North,East,Up.
       *  Individual unit vectors can be defined from this rotation matrix R by
       *  @code
       *     Vector<double> N = R.rowCopy(0);
       *     Vector<double> E = R.rowCopy(1);
       *     Vector<double> U = R.rowCopy(2);
       *  @endcode
       *  @param[in] pos   Position at which the rotation matrix will be defined.
       *  @param[in] geoc  If true, use geocentric, else geodetic coordinates (default false).
       *  @return          3x3 rotation matrix that will transform an ECEF vector into the
       *                   local topocentric or North,East,Up frame at the position pos.
       *  @throw Exception
       */
   Matrix<double> northEastUp(Position& pos, bool geoc=false);

      /**
       *  Same as northEastUp(pos, true).
       *  @param[in] pos  Position at which the rotation matrix will be defined.
       *  @return         3x3 rotation matrix that will transform an ECEF vector into the
       *                  local topocentric or North,East,Up frame at the position pos.
       *  @throw Exception
       */
   Matrix<double> northEastUpGeocentric(Position& pos);

      /**
       *  Same as northEastUp(pos, false).
       *  @param[in] pos  Position at which the rotation matrix will be defined.
       *  @return         3x3 rotation matrix that will transform an ECEF vector into the
       *                  local topocentric or North,East,Up frame at the position pos.
       *  @throw Exception
       */
   Matrix<double> northEastUpGeodetic(Position& pos);

      /**
       *  Given a Position, compute unit vectors in ECEF coordinates in the Up, East
       *  and North directions at that position. Use either geodetic coordinates,
       *  i.e. so that 'up' is perpendicular to the geoid, not from center of Earth;
       *  or geocentric coordinates, so that the vertical meets the center of the Earth.
       *  Return the vectors in the form of a 3x3 Matrix<double>. This is in fact the
       *  rotation matrix that will take an ECEF (XYZ) vector into 'up,east,north'.
       *  Individual unit vectors can be defined from this rotation matrix R by
       *  @code
       *     Vector<double> U = R.rowCopy(0);
       *     Vector<double> E = R.rowCopy(1);
       *     Vector<double> N = R.rowCopy(2);
       *  @endcode
       *  @param[in] pos   Position at which the rotation matrix will be defined.
       *  @param[in] geoc  If true, use geocentric, else geodetic coordinates (default).
       *  @return          3x3 rotation matrix that will transform an ECEF vector into the
       *                   local topocentric or North,East,Up frame at the position pos.
       *  @throw Exception
       */
   Matrix<double> upEastNorth(Position& pos, bool geoc=false);

      /**
       *  Same as upEastNorth, but using geocentric coordinates, so that the Up
       *  direction will meet the center of Earth.
       *  @param pos  Position at which the rotation matrix will be defined.
       *  @return     3x3 rotation matrix that will transform an ECEF vector into the
       *              local topocentric or North,East,Up frame at the position pos.
       *  @throw Exception
       */
   Matrix<double> upEastNorthGeocentric(Position& pos);

      /**
       *  Same as UpEastNorth, but using geodetic coordinates.
       *  @param[in] pos  Position at which the rotation matrix will be defined.
       *  @return         3x3 rotation matrix that will transform an ECEF vector into the
       *                  local topocentric or North,East,Up frame at the position pos.
       *  @throw Exception
       */
   Matrix<double> upEastNorthGeodetic(Position& pos);

      /**
       *  Compute the fraction of the Sun covered by the earth as seen from a satellite,
       *  given the angular radii of both, and their angular separation. See code for
       *  more documentation, including derivations.
       *  @param[in] angRadEarth    Angular radius of the earth as seen at the satellite.
       *  @param[in] angRadSun      Angular radius of the sun as seen at the satellite.
       *  @param[in] angSeparation  Angular distance of the sun from the Earth.
       *  @return                   Fraction (0 <= f <= 1) of area of Sun covered by Earth.
       *  @note Units only need be consistent.
       *  @throw Exception
       */
   double shadowFactor(double angRadEarth, double angRadSun, double angSeparation);

      /**
       *  Compute the fraction of the Sun covered by Earth as seen from a satellite,
       *  given Sun and satellite positions. Cf. the other version of this function.
       *  @param[in] sv   Satellite position.
       *  @param[in] sun  Sun position.
       *  @return         Fraction (0 <= f <= 1) of area of sun covered by Earth.
       *  @throw Exception
       */
   double shadowFactor(const Position& sv, const Position& sun);

      /**
       *  Compute the satellite attitude, given the time, the satellite position pos,
       *  and a Sun position. Use either class SolarSystem (high accuracy) or module
       *  SolarPosition (low accuracy) to get the Sun position; however note that
       *  SolarSystem::SatelliteAttitude(tt, sv, shadow, sunSVangle) makes both calls.
       *  Return a 3x3 Matrix that contains, as rows, the unit (ECEF) vectors X,Y,Z
       *  in the body frame of the satellite, namely
       *     Z = along the boresight (i.e. towards Earth center),
       *     Y = perpendicular to both Z and the satellite-Sun direction, and
       *     X = completing the orthonormal triad. X will generally point toward the Sun.
       *  Thus this rotation matrix R transforms an ECEF XYZ vector into the body frame
       *  of the satellite, so R * (ECEF XYZ vector) = components in body frame.
       *  Also, transpose(R) * (sat. body. frame vector) = ECEF XYZ components.
       *  Also return the shadow factor, which is the fraction of the Sun's area not
       *  visible to satellite; thus sf == 1 means the satellite is in eclipse.
       *  @param[in] pos  Satellite position at tt.
       *  @param[in] sun  Sun position at tt.
       *  @return         3x3 rotation matrix from XYZ to satellite body frame.
       *  @throw Exception
       */
   Matrix<double> satelliteAttitude(const Position& pos, const Position& sun);

      /**
       *  Compute the satellite attitude, given the satellite position pos and
       *  velocity vel, assuming an orbit-normal attitude.
       *  Return a 3x3 Matrix which contains, as rows, the unit (ECEF) vectors X,Y,Z
       *  in the body frame of the satellite, namely
       *     Z = along the boresight (i.e. towards Earth center),
       *     Y = perpendicular to orbital plane, pointing opposite the angular momentum vector,
       *     X = along-track (same direction as velocity vector for circular orbit).
       *  Thus this rotation matrix R transforms an ECEF XYZ vector into the body frame
       *  of the satellite, so R * (ECEF XYZ vector) = components in body frame.
       *  Also, R.transpose() * (sat. body. frame vector) = ECEF XYZ components.
       *  @param[in] pos  Satellite position.
       *  @param[in] vel  Satellite velocity.
       *  @return         3x3 rotation matrix from XYZ to Satellite body frame.
       *  @throw Exception
       */
   Matrix<double> orbitNormalAttitude(const Position& pos, const Position& vel);

      /**
       *  Compute the azimuth and nadir angle, in the satellite body frame,
       *  of receiver Position rx as seen at the satellite Position sv. The nadir angle
       *  is measured from the Z axis, which points to Earth center, and azimuth is
       *  measured from the X axis.
       *  @param[in]  sv       Satellite position.
       *  @param[in]  rx       Receiver position.
       *  @param[in]  rot      Rotation matrix (3,3), output of satelliteAttitude.
       *  @param[out] nadir    Nadir angle in degrees.
       *  @param[out] azimuth  Azimuth angle in degrees.
       *  @throw Exception if rotation matrix has wrong dimensions.
       */
   void satelliteNadirAzimuthAngles(const Position& sv,
                                    const Position& rx,
                                    const Matrix<double>& rot,
                                    double& nadir,
                                    double& azimuth);

      /**
       *  Compute the angle from satellite to Earth to Sun; that is the angular
       *  separation of the satellite and the Sun, as seen from the center of Earth.
       *  This angle lies between zero and pi, and it reaches zero (pi)
       *  only when the Sun lies exactly in the orbit plane at noon (midnight).
       *  Use either class SolarSystem (high accuracy) or module SolarPosition
       *  (low accuracy) to get the Sun position. Return the angle in radians.
       *  @param[in] sv   Satellite position.
       *  @param[in] sun  Sun position at tt.
       *  @return         Angle in radians satellite-Earth-Sun.
       *  @throw Exception
       */
   double satelliteEarthSunAngle(const Position& sv, const Position& sun);

      /**
       *  Compute the angle between the Sun and the satellite orbit plane,
       *  given the satellite position and velocity and Sun position.
       *  Return the angle in radians; domain +-Pi/2 and with the sign of RxV.
       *  That is, the angle is positive if the Sun is out of the orbit plane in the
       *  direction of R cross V; then Sun "sees" the orbit motion as counter-clockwise.
       *  Also return phi, the angle, in the orbit plane, from midnight to the
       *  satellite; domain 0 to 2Pi, increasing in the direction of vel.
       *  Use either class SolarSystem (high accuracy) or module SolarPosition
       *  (lower accuracy) to get the Sun position.
       *  @param[in]  pos   Satellite position at time of interest.
       *  @param[in]  vel   Satellite velocity at time of interest.
       *  @param[in]  sun   Sun position at time of interest.
       *  @param[out] phi   Return angle in orbit plane, midnight to satellite (rad).
       *  @param[out] beta  Return angle sun to plane of satellite orbit (rad).
       *  @note phi, beta and sesa, the satellite-earth-sun angle, form a right spherical
       *  triangle with sesa opposite the right angle. Thus, cos(sesa)=cos(beta)*cos(phi).
       *  @throw Exception
       */
   void sunOrbitAngles(const Position& pos, const Position& vel, const Position& sun,
                       double& beta, double& phi);

      /**
       *  Compute the nominal yaw angle of the satellite given the satellite position and
       *  velocity and the Sun position at the given time, plus a flag for GPS Block IIR
       *  and IIF satellites. Return the nominal yaw angle in radians, and the yaw rate
       *  in radians/second.
       *  @param[in]  pos      Satellite position at time of interest.
       *  @param[in]  vel      Satellite velocity at time of interest (Cartesian, m/s).
       *  @param[in]  sun      Sun position at time of interest.
       *  @param[in]  blkIIRF  True if the satellite is GPS block IIR or IIF.
       *  @param[out] yawrate  Return yaw rate in rad/second.
       *  @return              Satellite yaw angle in radians
       *  @throw Exception
       */
   double satelliteYawAngle(const Position& pos, const Position& vel, const Position& sun,
                            const bool& blkIIRF, double& yawrate);

}  // end namespace gnsstk

#endif // SUN_EARTH_SATELLITE_INCLUDE
