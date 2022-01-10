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
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
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
    * @file SunEarthSatGeometry.cpp
    * Module containing various routines related to Sun-Earth-Satellite geometry,
    * including satellite attitude, XYZ->UEN rotation, and (elevation,azimuth)
    * as seen at the satellite. Used by PhaseWindup and PreciseRange, among others.
    * @note Use class SolarSystem or SolarPosition() to get Sun's position as needed.
    */

#include <map>
#include "GPSEllipsoid.hpp"          /// uses angVelocity
#include "SunEarthSatGeometry.hpp"

using namespace std;

namespace gnsstk
{

      /// Same as upEastNorth(), but with rows re-ordered.
   Matrix<double> northEastUp(Position& pos, bool geocentric)
   {
      try {
         Matrix<double> R(3,3);
         pos.transformTo(geocentric ? Position::Geocentric : Position::Geodetic);

         double lat = (geocentric ? pos.getGeocentricLatitude()
                                  : pos.getGeodeticLatitude()) * DEG_TO_RAD;  /// rad N
         double lon = pos.getLongitude() * DEG_TO_RAD;                        /// rad E
         double ca = ::cos(lat);
         double sa = ::sin(lat);
         double co = ::cos(lon);
         double so = ::sin(lon);

            // Rotation matrix that transforms X=(x,y,z) into (R*X)(north,east,up).
         R(0,0) = -sa*co;  R(0,1) = -sa*so;  R(0,2) = ca;
         R(1,0) =    -so;  R(1,1) =     co;  R(1,2) = 0.;
         R(2,0) =  ca*co;  R(2,1) =  ca*so;  R(2,2) = sa;

            // The rows of R are also the unit vectors, in ECEF, of north,east,up;
            // R = (N && E && U) = transpose(N || E || U).
            // Vector<double> N = R.rowCopy(0);
            // Vector<double> E = R.rowCopy(1);
            // Vector<double> U = R.rowCopy(2);

         return R;
      }
      catch(Exception& e) { GNSSTK_RETHROW(e); }
      catch(exception& e) {
         Exception E("std except: " + string(e.what()));
         GNSSTK_THROW(E);
      }
      catch(...)
      {
         Exception e("Unknown exception.");
         GNSSTK_THROW(e);
      }
   }

      /// Same as northEastUp(pos, true).
   Matrix<double> northEastUpGeocentric(Position& pos)
   { return northEastUp(pos, true); }

      /// Same as northEastUp(pos, false).
   Matrix<double> northEastUpGeodetic(Position& pos)
   { return northEastUp(pos, false); }

      /**
       * Given a Position, compute unit (ECEF) vectors in the topocentric Up, East, North
       * directions at that position. Use geodetic coordinates, i.e. 'up' is
       * perpendicular to the geoid. Return the vectors in the form of a
       * 3x3 Matrix<double>, this is in fact the rotation matrix that will take
       * an ECEF vector into a local topocentric or 'up-east-north' vector.
       */
   Matrix<double> upEastNorth(Position& pos, bool geocentric)
   {
      try {
         Matrix<double> R = northEastUp(pos, geocentric);
         for(int i=0; i<3; i++) { double r=R(0,i); R(0,i)=R(2,i); R(2,i)=r; }
         return R;
      }
      catch(Exception& e) { GNSSTK_RETHROW(e); }
      catch(exception& e) {
         Exception E("std except: " + string(e.what()));
         GNSSTK_THROW(E);
      }
      catch(...) { Exception e("Unknown exception."); GNSSTK_THROW(e); }
   }

      /**
       * Same as upEastNorth, but using geocentric coordinates,
       * so that the Up direction will meet the center of Earth.
       */
   Matrix<double> upEastNorthGeocentric(Position& pos)
   { return upEastNorth(pos, true); }

      /// Same as upEastNorth, but using geodetic coordinates.
   Matrix<double> upEastNorthGeodetic(Position& pos)
   { return upEastNorth(pos, false); }

      /**
       *  Compute the fraction of the Sun covered by the earth as seen from a satellite,
       *  given the angular radii of both, and their angular separation.
       */
      /*
       * Consider the Sun and the Earth as seen from the satellite. Let the Sun be a
       * circle of angular radius r, center in direction s, and the Earth be a (larger)
       * circle of angular radius R, center in direction e. The circles overlap if
       * |e-s| < R+r; complete overlap if |e-s| < R-r.
       * The satellite is in penumbra if R-r < |e-s| < R+r, umbra if |e-s| < R-r.
       * Let L == |e-s|. What is the area of overlap in penumbra: R-r < L < R+r ?
       * Call the two points where the circles intersect p1 and p2. Draw a line from
       * e to s; call the points where this line intersects the two circles r1 and R1,
       * respectively.
       * Draw lines from e to s, e to p1, e to p2, s to p1 and s to p2. Call the angle
       * between e-s and e-p1 alpha, and that between s-e and s-p1, beta.
       * Draw a rectangle with top and bottom parallel to e-s passing through p1 and p2,
       * and with sides passing through s and r1. Similarly for e and R1. Note that the
       * area of intersection lies within the intersection of these two rectangles.
       * Call the area of the rectangle outside the circles A and B.
       * The height H of the rectangles is
       * H = 2Rsin(alpha) = 2rsin(beta)
       * also, L = rcos(beta)+Rcos(alpha)
       * The area A will be the area of the rectangle
       *              minus the area of the wedge formed by the angle 2*alpha
       *              minus the area of the two triangles which meet at s :
       * A = RH - (2alpha/2pi)*pi*R*R - 2*(1/2)*(H/2)Rcos(alpha)
       * Similarly,
       * B = rH - (2beta/2pi)*pi*r*r  - 2*(1/2)*(H/2)rcos(beta)
       * The area of intersection will be the area of the rectangular intersection
       *                            minus the area A
       *                            minus the area B
       * Intersection = H(R+r-L) - A - B
       *          = HR+Hr-HL -HR+alpha*R*R+(H/2)Rcos(alpha) -Hr+beta*r*r+(H/2)rcos(beta)
       * Cancel terms, and substitute for L using above equation L = ..
       *          = -(H/2)rcos(beta)-(H/2)Rcos(alpha)+alpha*R*R+beta*r*r
       * substitute for H/2
       *          = -R*R*sin(alpha)cos(alpha)-r*r*sin(beta)cos(beta)+alpha*R*R+beta*r*r
       * Intersection = R*R*[alpha-sin(alpha)cos(alpha)]+r*r*[beta-sin(beta)cos(beta)]
       * Solve for alpha and beta in terms of R, r and L using H and L relations above
       * (r/R)cos(beta)=(L/R)-cos(alpha)
       * (r/R)sin(beta)=sin(alpha)
       * so
       * (r/R)^2 = (L/R)^2 - (2L/R)cos(alpha) + 1
       * cos(alpha) = (R/2L)(1+(L/R)^2-(r/R)^2)
       * cos(beta) = (L/r) - (R/r)cos(alpha)
       * and 0 <= alpha or beta <= pi
       */
   double shadowFactor(double angRadEarth, double angRadSun, double angSeparation)
   {
      try {
         if(angSeparation >= angRadEarth+angRadSun)
         {
            return 0.0;
         }
         if(angSeparation <= fabs(angRadEarth-angRadSun))
         {
            return 1.0;
         }
         double r=angRadSun, R=angRadEarth, L=angSeparation;
         if(angRadSun > angRadEarth)
         {
            r=angRadEarth; R=angRadSun;
         }
         double cosalpha = (R/L)*(1.0+(L/R)*(L/R)-(r/R)*(r/R))/2.0;
         double cosbeta = (L/r) - (R/r)*cosalpha;
         double sinalpha = ::sqrt(1.0-cosalpha*cosalpha);
         double sinbeta = ::sqrt(1.0-cosbeta*cosbeta);
         double alpha = ::asin(sinalpha);
         double beta = ::asin(sinbeta);
         double shadow = r*r*(beta-sinbeta*cosbeta)+R*R*(alpha-sinalpha*cosalpha);
         shadow /= ::acos(-1.0)*angRadSun*angRadSun;
         return shadow;
      }
      catch(Exception& e) { GNSSTK_RETHROW(e); }
      catch(exception& e) {
         Exception E("std except: " + string(e.what()));
         GNSSTK_THROW(E);
      }
      catch(...)
      {
         Exception e("Unknown exception."); GNSSTK_THROW(e);
      }
   }

   double shadowFactor(const Position& sv, const Position& sun)
   {
      try {
         Position z(sv);
            // z points from satellite to Earth center along the antenna boresight.
         z.transformTo(Position::Cartesian);
         double svrange = z.mag();
         double d = -1.0/svrange;
         z = d * z;                              // reverse and normalize z

            // Find distance to Sun, and unit vector Earth to Sun.
         Position sSun(sun);
         sSun.transformTo(Position::Cartesian);
         double dSun = sSun.radius();            // distance to Sun
         d = 1.0/dSun;
         sSun = d * sSun;                        // unit vector Earth-to-Sun

            // Sun-Earth-satellite angle = angular separation sat and Sun seen at Earth;
            // same as satelliteEarthSunAngle(SV,Sun).
         double sesa = ::acos(-z.dot(sSun));

            // Apparent angular radius (deg) of Sun = 0.2666/(distance in AU)/
         double angRadSun = DEG_TO_RAD * 0.2666 / (dSun/149598.0e6);
            // Angular radius of Earth at sattelite.
         double angRadEarth = ::asin(6378137.0/svrange);

         return shadowFactor(angRadEarth, angRadSun, sesa);
      }
      catch(Exception& e)
      {
         GNSSTK_RETHROW(e);
      }
   }

      /**
       * Compute the satellite attitude, given the time and position sv.
       * Use either class SolarSystem (high accuracy) or module SolarPosition
       * (low accuracy) to get the Sun position.
       * Return a 3x3 Matrix that contains, as rows, the unit (ECEF) vectors X,Y,Z
       * in the satellite body frame.
       * @param[in] sat  Satellite position.
       * @param[in] sun  Sun position.
       * @return Matrix<double>(3,3)  Rotation matrix from XYZ to Satellite body frame.
       */
   Matrix<double> satelliteAttitude(const Position& sat, const Position& sun)
   {
      try {
         Position pSun(sun), pSat(sat);
         Triple sSun(pSun.asECEF()), sSat(pSat.asECEF());  // must be Cartesian

            // Make orthonormal triad parallel to satellite body axes.
         Triple X,Y,Z,T;

            // Z points from satellite to Earth center along the antenna boresight.
         Z = sSat;
         double d = -1.0/sSat.mag();    // reverse Z and normalize
         Z = d * Z;

            // Let T point from satellite to Sun.
         T = sSun - sSat;               // sat-to-Sun = (Earth-to-Sun) - (Earth-to-sat)
         d = 1.0/T.mag();
         T = d * T;                     // normalize

            // Y is perpendicular to Z and T ...
         Y = Z.cross(T);
         d = 1.0/Y.mag();
         Y = d * Y;                     // normalize Y

            // ... such that X points generally in the direction of the Sun.
         X = Y.cross(Z);                // X will be unit vector since Y and Z are

         if(X.dot(T) < 0)
         {
            // Need to reverse X, hence Y also.
            X = -1.0 * X;
            Y = -1.0 * Y;
         }

            // Fill the rotation matrix: rows are X, Y and Z,
            // so R*V(ECEF XYZ) = V(body frame components) = (V dot X,Y,Z).
         Matrix<double> R(3,3);
         for(int i=0; i<3; i++) {
            R(0,i) = X[i];
            R(1,i) = Y[i];
            R(2,i) = Z[i];
         }

         return R;
      }
      catch(Exception& e) { GNSSTK_RETHROW(e); }
      catch(exception& e)
      {
         Exception E("std except: " + string(e.what()));
         GNSSTK_THROW(E);
      }
      catch(...)
      {
         Exception e("Unknown exception.");
         GNSSTK_THROW(e);
      }
   }

      /**
       * Compute the satellite attitude, given its position pos and velocity vel,
       * assuming an orbit-normal attitude.
       * Return a 3x3 Matrix which contains, as rows, the unit (ECEF) vectors X,Y,Z
       * in the satellite body frame.
       */
   Matrix<double> orbitNormalAttitude(const Position& pos, const Position& vel)
   {
      try {
         int i;
         double svrange, angmom;
         Position X,Y,Z;
         Matrix<double> R(3,3);

            // Z points from satellite to Earth center along the antenna boresight.
         svrange = pos.mag();
         Z = pos*(-1.0/svrange); // reverse and normalize Z
         Z.transformTo(Position::Cartesian);

            // Y points opposite the angular momentum vector.
         Y = pos.cross(vel);
         angmom = Y.mag();
         Y = Y*(-1.0/angmom);  /// reverse and normalize Y
         Y.transformTo(Position::Cartesian);

            // X completes the right-handed system.
         X = Y.cross(Z);

            // Fill the matrix and return it.
         for(i=0; i<3; i++) {
	        R(0,i) = X[i];
	        R(1,i) = Y[i];
	        R(2,i) = Z[i];
         }

         return R;
      }
      catch(Exception& e) { GNSSTK_RETHROW(e); }
      catch(std::exception& e) {Exception E("std except: "+string(e.what())); GNSSTK_THROW(E);}
      catch(...) { Exception e("Unknown exception."); GNSSTK_THROW(e); }
   }

      /**
       * Compute the azimuth and nadir angle, in the satellite body frame,
       * of receiver Position RX as seen at the satellite Position SV.
       * The nadir angle is measured from the Z axis, which points to Earth center,
       * and azimuth is measured from the X axis.
       * @param[in]  sv       Satellite position.
       * @param[in]  rx       Receiver position.
       * @param[in]  rot      Rotation matrix (3,3), output of satelliteAttitude.
       * @param[out] nadir    Nadir angle in degrees.
       * @param[out] azimuth  Azimuth angle in degrees.
       */
   void satelliteNadirAzimuthAngles(const Position& sv,
                                    const Position& rx,
                                    const Matrix<double>& rot,
                                    double& nadir,
                                    double& azimuth)
   {
      try {
         if(rot.rows() != 3 || rot.cols() != 3) {
            Exception e("Rotation matrix invalid.");
            GNSSTK_THROW(e);
         }

         double d;
         Position RmS;
            // RmS points from satellite to receiver.
         RmS = rx - sv;
         RmS.transformTo(Position::Cartesian);
         d = RmS.mag();
         if(d == 0.0) {
            Exception e("Satellite and receiver positions identical.");
            GNSSTK_THROW(e);
         }
         RmS = (1.0/d) * RmS;

         Vector<double> xyz(3),body(3);
         xyz(0) = RmS.X();
         xyz(1) = RmS.Y();
         xyz(2) = RmS.Z();
         body = rot * xyz;

         nadir = ::acos(body(2)) * RAD_TO_DEG;
         azimuth = ::atan2(body(1),body(0)) * RAD_TO_DEG;
         if(azimuth < 0.0) azimuth += 360.;
      }
      catch(Exception& e) { GNSSTK_RETHROW(e); }
      catch(exception& e) {
         Exception E("std except: " + string(e.what()));
         GNSSTK_THROW(E);
      }
      catch(...) { Exception e("Unknown exception."); GNSSTK_THROW(e); }
   }

      /**
       * Compute the angle from satellite to Earth to Sun; that is the angular
       * separation of the satellite and the Sun, as seen from the center of Earth.
       * @param[in] sat  Satellite position
       * @param[in] sun  Sun position
       * @return angle   Angle (rad) for satellite-Earth-Sun.
       */
   double satelliteEarthSunAngle(const Position& sat, const Position& sun)
   {
      try {
         Position pSun(sun), pSat(sat);
         Triple sSun(pSun.asECEF()), sSat(pSat.asECEF());

            // Normalize the vectors.
         double d;
         d = 1.0/sSun.mag(); sSun = sSun * d;
         d = 1.0/sSat.mag(); sSat = sSat * d;

            // Compute the angle.
         return (::acos(sSat.dot(sSun)));
      }
      catch(Exception& e) { GNSSTK_RETHROW(e); }
      catch(exception& e) {
         Exception E("std except: " + string(e.what()));
         GNSSTK_THROW(E);
      }
      catch(...) { Exception e("Unknown exception."); GNSSTK_THROW(e); }
   }

      /**
       * Compute the angle between the Sun and the satellite orbit plane.
       * Return the angle in radians; domain +-pi/2 and with the sign of RxV.
       * That is, the angle is positive if the Sun is out of the orbit plane in the
       * direction of R cross V; then Sun "sees" the orbit motion as counter-clockwise.
       * Also return, in phi, the angle, in the plane of the orbit, from midnight to the
       * satellite; domain 0 to 2pi, increasing in the direction of Vel.
       * Use either class SolarSystem (high accuracy) or module SolarPosition
       * (lower accuracy) to get the Sun position.
       * @param[in]  Position pos   Satellite position at time of interest.
       * @param[in]  Position vel   Satellite velocity at time of interest.
       * @param[in]  Position sun   Sun position at time of interest.
       * @param[out] double beta    Angle sun to plane of satellite orbit (rad).
       * @param[out] double phi     Angle in orbit plane, midn to satellite (rad).
       * @note phi, beta and sesa -- the sat-Earth-Sun angle -- form a right spherical
       * triangle with sesa opposite the right angle. Thus, cos(sesa)=cos(beta)*cos(phi).
       */
   void sunOrbitAngles(const Position& pos, const Position& vel, const Position& sun,
                       double& beta, double& phi)
   {
      try {
         Position satR(pos),satV(vel),pSun(sun);
         GPSEllipsoid ellips;
         double omega(ellips.angVelocity());  // 7.292115e-5 rad/sec

            // Compute inertial velocity.
         Position inertialV;
         inertialV.setECEF(satV.X()-omega*satR.Y(),satV.Y()+omega*satR.X(),satV.Z());

            // Use Triples.
         Triple sSun(pSun.asECEF()),sSat(satR.asECEF());
         Triple sVel(inertialV.asECEF()), u, w;

            // Normalize the vectors.
         double d;
         d = 1.0/sSun.mag(); sSun = sSun * d;
         d = 1.0/sSat.mag(); sSat = d * sSat;
         d = 1.0/sVel.mag(); sVel = d * sVel;

            // u is R cross V -- normal to the satellite orbit plane.
         u = sSat.cross(sVel);

            // Compute the angle beta: u dot sun = sin(beta) = cos(PI/2-beta)
         double udotsun(u.dot(sSun));
         beta = PI/2.0 - ::acos(udotsun);

         /**
          * Compute phi, angle in orbit plane between Sun and sat.
          * Zero at midnight, increasing with satellite motion.
          * w = sSun - u*sin(beta) lies in the orbit plane but ~points to Sun,
          * but if beta==pi/2, u==sSun and w==zero, phi is indeterminate.
          */
         w = sSun - udotsun*u;
         d = w.mag();
         if(d > 1.e-14) {
            d = 1.0/d;
            w = d * w;                      // normalize w
            phi = ::acos(sSat.dot(w));      // zero at noon where sat||w and dot=1
            if(sSat.dot(u.cross(w)) < 0.0)  // make phi zero at midnight
               phi = PI - phi;
            else
               phi += PI;
         }
         else
            phi = 0.0;
      }
      catch(Exception& e) { GNSSTK_RETHROW(e); }
      catch(exception& e) {
         Exception E("std except: " + string(e.what()));
         GNSSTK_THROW(E);
      }
      catch(...) { Exception e("Unknown exception."); GNSSTK_THROW(e); }
   }

      /**
       * Compute the nominal yaw angle of the satellite given the satellite position and
       * velocity and the Sun position at the given time, plus a flag for GPS Block IIR
       * and IIF satellites. Return the nominal yaw angle in radians, and the yaw rate
       * in radians/second.
       * @param[in]  Position pos    Satellite position at time of interest.
       * @param[in]  Position vel    Satellite velocity at time of interest (Cartesian, m/s).
       * @param[in]  Position sun    Sun position at time of interest.
       * @param[in]  bool blkIIRF    True if the satellite is GPS block IIR or IIF.
       * @param[out] double yawrate  Yaw rate in rad/s.
       * @return     double yaw      Satellite yaw angle (rad).
       */
   double satelliteYawAngle(const Position& pos, const Position& vel,
                            const Position& sun, const bool& blkIIRF, double& yawrate)
   {
      try {
            // Get orbit tilt beta, and in-plane orbit angle from midnight, mu.
         double beta, mu;
         sunOrbitAngles(pos, vel, sun, beta, mu);
         double sinmu = ::sin(mu);  // mu is the in-orbit-plane "azimuth" from midnight,
         double cosmu = ::cos(mu);  // increasing in direction of satellite motion.

            // Nominal yaw angle - cf. Kouba(2009).
         double tanb(::tan(beta)), yaw;
         if(blkIIRF)
            yaw = ::atan2(tanb, -sinmu);
         else
            yaw = ::atan2(-tanb, sinmu);

            // Orbit velocity (rad/s).
         double orbv = TWO_PI * vel.mag() / pos.mag();

            // Nominal yaw rate.
         yawrate = orbv * tanb * cosmu / (sinmu*sinmu+tanb*tanb);

         return yaw;
      }
      catch(Exception& e) { GNSSTK_RETHROW(e); }
   }

}  // end namespace
