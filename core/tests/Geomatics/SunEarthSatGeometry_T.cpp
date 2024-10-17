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

#include <iostream>

#include "TestUtil.hpp"
#include "SunEarthSatGeometry.hpp"
#include "Position.hpp"
#include "Matrix.hpp"

using namespace gnsstk;

class SunEarthSatGeometry_T
{
public:
    unsigned testNorthEastUp();
    unsigned testUpEastNorth();
    unsigned testShadowFactor();
    unsigned testSatelliteAttitude();
    unsigned testOrbitNormalAttitude();
    unsigned testSatelliteNadirAzimuthAngles();
    unsigned testSatelliteEarthSunAngle();
    unsigned testSunOrbitAngles();
    unsigned testSatelliteYawAngle();
};


// tests northEastUp functions: northEastUp(), northEastUpGeodetic(), northEastUpGeocentric()
unsigned SunEarthSatGeometry_T :: testNorthEastUp()
{
    TUDEF("SunEarthSatGeometry_T", "testNorthEastUp");

    // station 85408 position
    Position stationPos = Position(-740290.106988, -5457071.675903, 3207245.675845);
    
    // expected rotation matrix for the above position with geocentric latitude
    Matrix<double> ExpRGeocen(3,3);
    ExpRGeocen(0,0) = 0.067651336; ExpRGeocen(0,1) = 0.498693939; ExpRGeocen(0,2) = 0.864134047;
    ExpRGeocen(1,0) = 0.990923668; ExpRGeocen(1,1) = -0.134425756; ExpRGeocen(1,2) = 0.0;
    ExpRGeocen(2,0) = -0.116161873; ExpRGeocen(2,1) = -0.85629088; ExpRGeocen(2,2) = 0.503261709;

    // expected rotation matrix for the above position with geodetic latitude
    Matrix<double> ExpRGeodet(3,3);
    ExpRGeodet(0,0) = 0.067990913; ExpRGeodet(0,1) = 0.501197143; ExpRGeodet(0,2) = 0.862657904;
    ExpRGeodet(1,0) = 0.990923668; ExpRGeodet(1,1) = -0.134425756; ExpRGeodet(1,2) = 0.0;
    ExpRGeodet(2,0) = -0.115963441; ExpRGeodet(2,1) = -0.854828135; ExpRGeodet(2,2) = 0.505787841;

    // run northEastUp for both geodetic and geocentric cases to get gnsstk-calculated matrices
    Matrix<double> RGeocentric(3,3);
    Matrix<double> RGeodetic(3,3);
    try {
       RGeocentric = northEastUpGeocentric(stationPos);
       RGeodetic = northEastUpGeodetic(stationPos);
    }
    catch(Exception& e) {
        TUFAIL("Unexpected exception: " + e.what());
    }

    // compare expected and returned matrices element-wise, with an epsilon of 1E-9
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            TUASSERTFEPS(ExpRGeocen[i][j], RGeocentric[i][j], 1E-9);
            TUASSERTFEPS(ExpRGeodet[i][j], RGeodetic[i][j], 1E-9);
        };
    };
    TURETURN();
}


unsigned SunEarthSatGeometry_T :: testUpEastNorth()
{
    TUDEF("SunEarthSatGeometry_T", "testUpEastNorth");

    // station 85408 position
    Position stationPos = Position(-740290.106988, -5457071.675903, 3207245.675845);
    
    // expected rotation matrix for the above position with geocentric latitude (swap top and bottom rows from northEastUp)
    gnsstk::Matrix<double> ExpRGeocen(3,3);
    ExpRGeocen(0,0) = -0.116161873; ExpRGeocen(0,1) = -0.85629088; ExpRGeocen(0,2) = 0.503261709;
    ExpRGeocen(2,0) = 0.067651336; ExpRGeocen(2,1) = 0.498693939; ExpRGeocen(2,2) = 0.864134047;
    ExpRGeocen(1,0) = 0.990923668; ExpRGeocen(1,1) = -0.134425756; ExpRGeocen(1,2) = 0.0;

    // expected rotation matrix for the above position with geodetic latitude (swap top and bottom rows from northEastUp)
    Matrix<double> ExpRGeodet(3,3);
    ExpRGeodet(0,0) = -0.115963441; ExpRGeodet(0,1) = -0.854828135; ExpRGeodet(0,2) = 0.505787841;
    ExpRGeodet(2,0) = 0.067990913; ExpRGeodet(2,1) = 0.501197143; ExpRGeodet(2,2) = 0.862657904;
    ExpRGeodet(1,0) = 0.990923668; ExpRGeodet(1,1) = -0.134425756; ExpRGeodet(1,2) = 0.0;

    // run UpEastNorth for both geodetic and geocentric cases to get gnsstk-calculated matrices
    Matrix<double> RGeocentric(3,3);
    Matrix<double> RGeodetic(3,3);
    try {
       RGeocentric = upEastNorthGeocentric(stationPos);
       RGeodetic = upEastNorthGeodetic(stationPos);
    }
    catch(Exception& e) {
        TUFAIL("Unexpected exception: " + e.what());
    }

    // compare expected and returned matrices element-wise, with an epsilon of 1E-9
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            TUASSERTFEPS(ExpRGeocen[i][j], RGeocentric[i][j], 1E-9);
            TUASSERTFEPS(ExpRGeodet[i][j], RGeodetic[i][j], 1E-9);
        };
    };
    TURETURN();
}


unsigned SunEarthSatGeometry_T :: testShadowFactor()
{
    TUDEF("SunEarthSatGeometry_T", "testShadowFactor");

    // about 17 degrees, in radians:
    double EarthAngRad = 0.296706;
    // about 15 arcminutes, in radians:
    double SunAngRad = 0.00436332;

    // check trivial cases first: sun not covered, sun totally occluded. Then check non-trivial case
    double shadowFactorZero;
    double shadowFactorFull;
    double shadowFactorRads;
    double shadowFactorPos;
    try {
        // trivial cases
        shadowFactorZero = shadowFactor(EarthAngRad, SunAngRad, 0.4);
        shadowFactorFull = shadowFactor(EarthAngRad, SunAngRad, 0.2);

        // (EarthAngRad + SunAngRad) > angular separation > abs(EathAngRad - sunAngRad)
        shadowFactorRads = shadowFactor(EarthAngRad, SunAngRad, 0.293);

        // utilize SV pos/sat pos to find shadowFactor-- i.e. exercise function to convert positions to angles
        // this sat/sun position corresponds to 7:30pm on 20240101. The SV used here is GPS PRN 5
        Position SVPos = Position(-9196813.8994, 11914068.5772, 21714222.4978);
        Position solPos = Position(-49920869890.0995, -125878274575.0792, -57455321740.7313);
        shadowFactorPos = shadowFactor(SVPos, solPos);
    }
    catch(Exception& e) {
      TUFAIL("Unexpected exception: " + e.what());
    }

    TUASSERTFE(0.0, shadowFactorZero);
    TUASSERTFE(1.0, shadowFactorFull);
    TUASSERTFEPS(0.321956211, shadowFactorRads, 1E-9);
    TUASSERTFE(0.0, shadowFactorPos);
    
    TURETURN();
}

unsigned SunEarthSatGeometry_T :: testSatelliteAttitude()
{
    TUDEF("SunEarthSatGeometry_T", "testSatelliteAttitude");

    // SV/sol position from 7:30pm on 20240101. The SV used here is GPS PRN 5
    Position SVPos = Position(-9196813.8994, 11914068.5772, 21714222.4978);
    Position solPos = Position(-49920869890.0995, -125878274575.0792, -57455321740.7313);
    
    // hand-calculated rotation matrix
    Matrix<double> expAttitude(3,3);
    expAttitude(0,0) = -0.673402114; expAttitude(0,1) = -0.730208310; expAttitude(0,2) = 0.115435768;
    expAttitude(1,0) = -0.652195294; expAttitude(1,1) = 0.513269959; expAttitude(1,2) = -0.557848766;
    expAttitude(2,0) = 0.348096093; expAttitude(2,1) = -0.450943203; expAttitude(2,2) = -0.821875499;

    Matrix<double> Attitude(3,3);
    try {
        Attitude = satelliteAttitude(SVPos, solPos);
    }
    catch(Exception& e) {
      TUFAIL("Unexpected exception: " + e.what());
    }

    // check rotation matrix element-wise
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            TUASSERTFEPS(expAttitude[i][j], Attitude[i][j], 1E-9);
        };
    };

    TURETURN();
}


unsigned SunEarthSatGeometry_T :: testOrbitNormalAttitude()
{
    TUDEF("SunEarthSatGeometry_T", "testOrbitNormalAttitude");

    // position of: GPS PRN 5 at 7:30pm on 20240101
    Position SVpos = Position(-9196813.8994, 11914068.5772, 21714222.4978);
    Position SVvel = Position(-2422.933692676207, -1395.3579676087809, -249.57031542254785);
    
    // manually ECEF unit vectors in SV body frame
    Matrix<double> expOrbitNormalR(3,3);
    expOrbitNormalR(0,0) = -0.862018616; expOrbitNormalR(0,1) = -0.498538082; expOrbitNormalR(0,2) = -0.091562472;
    expOrbitNormalR(1,0) = -0.368446761; expOrbitNormalR(1,1) = 0.740344520; expOrbitNormalR(1,2) = -0.562260595;
    expOrbitNormalR(2,0) = 0.348096093; expOrbitNormalR(2,1) = -0.450943204; expOrbitNormalR(2,2) = -0.821875500;

    Matrix<double> orbitNormalR(3,3);
    try {
        orbitNormalR = orbitNormalAttitude(SVpos, SVvel);
    }
    catch(Exception& e) {
      TUFAIL("Unexpected exception: " + e.what());
    }

    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            TUASSERTFEPS(expOrbitNormalR[i][j], orbitNormalR[i][j], 1E-9);
        };
    };

    TURETURN();
}


unsigned SunEarthSatGeometry_T :: testSatelliteNadirAzimuthAngles()
{
    TUDEF("SunEarthSatGeometry_T", "testSateliteNadirAzimuthAngles");

    // position of: GPS PRN 5 at 7:30pm on 20240101
    Position SVPos = Position(-9196813.8994, 11914068.5772, 21714222.4978);
    Position solPos = Position(-49920869890.0995, -125878274575.0792, -57455321740.7313);
    
    // position of station 85408 (ARL)
    Position stationPos = Position(-740290.106988, -5457071.675903, 3207245.675845);
    
    // initialize nadir and azimuth variables
    double nadir;
    double azimuth;

    try {
        Matrix<double> R(3,3); 
        R = satelliteAttitude(SVPos, solPos);
        satelliteNadirAzimuthAngles(SVPos, stationPos, R, nadir, azimuth);
    }
    catch(gnsstk::Exception& e) {
      TUFAIL("Unexpected exception: " + e.what());
    }

    TUASSERTFEPS(13.748127863, nadir, 1E-9);
    TUASSERTFEPS(319.760537130, azimuth, 1E-9);

    TURETURN();
}


unsigned SunEarthSatGeometry_T :: testSatelliteEarthSunAngle()
{
    TUDEF("SunEarthSatGeometry_T", "testSatelliteEarthSunAngle");

    // position of: GPS PRN 5 at 7:30pm on 20240101
    Position SVPos = Position(-9196813.8994, 11914068.5772, 21714222.4978);
    Position solPos = Position(-49920869890.0995, -125878274575.0792, -57455321740.7313);

    double angle;
    try {
        angle = satelliteEarthSunAngle(SVPos, solPos);
    }
    catch(gnsstk::Exception& e) {
      TUFAIL("Unexpected exception: " + e.what());
    }

    TUASSERTFEPS(2.200328172946, angle, 1E-9);

    TURETURN();
}


unsigned SunEarthSatGeometry_T :: testSunOrbitAngles()
{
    TUDEF("SunEarthSatGeometry_T", "testSunOrbitAngles");

    // position of: GPS PRN 5 at 7:30pm on 20240101
    Position SVPos = Position(-9196813.8994, 11914068.5772, 21714222.4978);
    Position SVvel = Position(-2422.933692676207, -1395.3579676087809, -249.57031542254785);
    Position solPos = Position(-49920869890.0995, -125878274575.0792, -57455321740.7313);

    double beta;
    double phi;
    try {
        sunOrbitAngles(SVPos, SVvel, solPos, beta, phi);
    }
    catch(gnsstk::Exception& e) {
      TUFAIL("Unexpected exception: " + e.what());
    }

    TUASSERTFEPS(0.257219826684, beta, 1E-9);
    TUASSERTFEPS(0.916255167058, phi, 1E-9);

    TURETURN();
}


unsigned SunEarthSatGeometry_T :: testSatelliteYawAngle()
{
    TUDEF("SunEarthSatGeometry_T", "testSatelliteYawAngle");

    // position of: GPS PRN 5 at 7:30pm on 20240101
    Position SVPos = Position(-9196813.8994, 11914068.5772, 21714222.4978);
    Position SVvel = Position(-2422.933692676207, -1395.3579676087809, -249.57031542254785);
    Position solPos = Position(-49920869890.0995, -125878274575.0792, -57455321740.7313);

    double yaw;
    double yawrate;
    double yawBlockIIRF;
    double yawrateBlockIIRF;

    try {
        yaw = satelliteYawAngle(SVPos, SVvel, solPos, false, yawrate);
        yawBlockIIRF = satelliteYawAngle(SVPos, SVvel, solPos, true, yawrateBlockIIRF);
    }
    catch(gnsstk::Exception& e) {
      TUFAIL("Unexpected exception: " + e.what());
    }

    TUASSERTFEPS(-0.320166423809, yaw, 1E-9);
    TUASSERTFEPS(2.821426229780, yawBlockIIRF, 1E-9);
    TUASSERTFEPS(0.000153038912, yawrate, 1E-9);
    TUASSERTFE(yawrate, yawrateBlockIIRF); // yawrate should be indepentent of the blkIIRF parameter

    TURETURN();
}


int main()
{
    SunEarthSatGeometry_T testClass;
    unsigned errorTotal = 0;

    errorTotal += testClass.testNorthEastUp();
    errorTotal += testClass.testUpEastNorth();
    errorTotal += testClass.testShadowFactor();
    errorTotal += testClass.testSatelliteAttitude();
    errorTotal += testClass.testOrbitNormalAttitude();
    errorTotal += testClass.testSatelliteNadirAzimuthAngles();
    errorTotal += testClass.testSatelliteEarthSunAngle();
    errorTotal += testClass.testSunOrbitAngles();
    errorTotal += testClass.testSatelliteYawAngle();

    std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;

    return errorTotal;
}