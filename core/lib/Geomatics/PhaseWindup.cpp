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
 * @file PhaseWindup.cpp
 * Implement computations of phase windup, solar ephemeris, satellite attitude
 * and eclipse at the satellite.
 */

// -----------------------------------------------------------------------------------
// GNSSTk includes
#include "Matrix.hpp"
#include "GNSSconstants.hpp"             // DEG_TO_RAD
// geomatics
#include "PhaseWindup.hpp"
#include "SunEarthSatGeometry.hpp"
#include "SolarPosition.hpp"
#include <math.h>

using namespace std;

namespace gnsstk
{

// -----------------------------------------------------------------------------------
// Version without JPL solar system ephemeris - uses a lower quality solar position.
// Compute the phase windup, in cycles, given the time, the unit vector from receiver
// to transmitter, and the west and north unit vectors at the receiver, all in ECEF.
// YR is the West unit vector, XR is the North unit vector, at the receiver.
double PhaseWindup(double prev,       // previous return value
                   CommonTime& tt,        // epoch of interest
                   Position& SV,       // satellite position
                   Position& Rx2Tx,    // unit vector from receiver to satellite
                   Position& YR,       // west unit vector at receiver
                   Position& XR,       // north unit vector at receiver
                   bool isBlockR)      // true for Block IIR satellites
{
try {
   double d,windup=0.0;
   Position DR,DT;
   Position TR = -1.0 * Rx2Tx;         // transmitter to receiver

   // get satellite attitude
   Position XT,YT,ZT,Sun;
   double AR;
   Sun = solarPosition(tt,AR);
   Matrix<double> Att = satelliteAttitude(SV,Sun);
   XT = Position(Att(0,0),Att(0,1),Att(0,2));      // Cartesian is default
   YT = Position(Att(1,0),Att(1,1),Att(1,2));
   ZT = Position(Att(2,0),Att(2,1),Att(2,2));

   // NB. Block IIR has X (ie the effective dipole orientation) in the -XT direction.
   // Ref. Kouba(2009) GPS Solutions 13, pp1-12.
   if(isBlockR) XT = Position(-Att(0,0),-Att(0,1),-Att(0,2));

   // compute effective dipoles at receiver and transmitter Ref Kouba(2009)
   DR = XR - TR * TR.dot(XR) + Position(TR.cross(YR));
   DT = XT - TR * TR.dot(XT) - Position(TR.cross(YT));

   // normalize
   d  = 1.0/DR.mag();
   DR = d * DR;
   d  = 1.0/DT.mag();
   DT = d * DT;

   windup = ::acos(DT.dot(DR)) / TWO_PI;
   if (TR.dot(DT.cross(DR)) < 0.) windup *= -1.0;

   // adjust by 2pi if necessary
   d = windup-prev;
   windup -= int(d + (d < 0.0 ? -0.5 : 0.5));

   return windup;
}
catch(Exception& e) { GNSSTK_RETHROW(e); }
catch(std::exception& e) { Exception E("std except: "+string(e.what())); GNSSTK_THROW(E); }
catch(...) { Exception e("Unknown exception"); GNSSTK_THROW(e); }
}

double PhaseWindup(double prev,       // previous return value
                   CommonTime& tt,          // epoch of interest
                   Position& SV,         // satellite position
                   Position& Rx2Tx,      // unit vector from receiver to satellite
                   Position& YR,         // west unit vector at receiver
                   Position& XR,         // north unit vector at receiver
                   SolarSystem& SSEph)   // solar system ephemeris
{
try {
   double d,windup;
   Position DR,DT;
   Position TR = -1.0 * Rx2Tx;         // transmitter to receiver

   // get satellite attitude
   Position XT,YT,ZT;
   Matrix<double> Att = SSEph.satelliteAttitude(tt, SV);
   XT = Position(Att(0,0),Att(0,1),Att(0,2));      // Cartesian is default
   YT = Position(Att(1,0),Att(1,1),Att(1,2));
   ZT = Position(Att(2,0),Att(2,1),Att(2,2));

   // compute effective dipoles at receiver and transmitter Ref Kouba(2009) 
   DR = XR - TR * TR.dot(XR) + Position(TR.cross(YR));
   DT = XT - TR * TR.dot(XT) - Position(TR.cross(YT));

   // normalize
   d  = 1.0/DR.mag();
   DR = d * DR;
   d  = 1.0/DT.mag();
   DT = d * DT;

   windup = ::acos(DT.dot(DR)) / TWO_PI;             // cycles
   if (TR.dot(DT.cross(DR)) < 0.) windup *= -1.0;

   // adjust by 2pi if necessary
   d = windup-prev;
   windup -= int(d + (d < 0.0 ? -0.5 : 0.5));
 
   return windup;
}
catch(Exception& e) { GNSSTK_RETHROW(e); }
catch(std::exception& e) { Exception E("std except: "+string(e.what())); GNSSTK_THROW(E); }
catch(...) { Exception e("Unknown exception"); GNSSTK_THROW(e); }
}

double PhaseWindup(double prev,       // previous return value
                   CommonTime& tt,          // epoch of interest
                   Position& SV,         // satellite position
                   Position& Rx2Tx,      // unit vector from receiver to satellite
                   Position& YR,         // west unit vector at receiver
                   Position& XR,         // north unit vector at receiver
                   SolarSystem& SSEph,   // solar system ephemeris
                   bool isBlockR)      // true for Block IIR satellites
{
try {
   double d,windup;
   Position DR,DT;
   Position TR = -1.0 * Rx2Tx;         // transmitter to receiver

   // get satellite attitude
   Position XT,YT,ZT;
   Matrix<double> Att = SSEph.satelliteAttitude(tt, SV);
   XT = Position(Att(0,0),Att(0,1),Att(0,2));      // Cartesian is default
   YT = Position(Att(1,0),Att(1,1),Att(1,2));
   ZT = Position(Att(2,0),Att(2,1),Att(2,2));

   // NB. Block IIR has X (ie the effective dipole orientation) in the -XT direction.
   // Ref. Kouba(2009) GPS Solutions 13, pp1-12.
   if(isBlockR) XT = Position(-Att(0,0),-Att(0,1),-Att(0,2));
   
   // compute effective dipoles at receiver and transmitter Ref Kouba (2009) 
   DR = XR - TR * TR.dot(XR) + Position(TR.cross(YR));
   DT = XT - TR * TR.dot(XT) - Position(TR.cross(YT));

   // normalize
   d  = 1.0/DR.mag();
   DR = d * DR;
   d  = 1.0/DT.mag();
   DT = d * DT;

   windup = ::acos(DT.dot(DR)) / TWO_PI;             // cycles
   if (TR.dot(DT.cross(DR)) < 0.) windup *= -1.0;

   // adjust by 2pi if necessary
   d = windup-prev;
   windup -= int(d + (d < 0.0 ? -0.5 : 0.5));
 
   return windup;
}
catch(Exception& e) { GNSSTK_RETHROW(e); }
catch(std::exception& e) { Exception E("std except: "+string(e.what())); GNSSTK_THROW(E); }
catch(...) { Exception e("Unknown exception"); GNSSTK_THROW(e); }
}

} // end namespace gnsstk
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//

