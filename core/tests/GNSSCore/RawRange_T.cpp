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

#include "Triple.hpp"
#include "TestUtil.hpp"
#include "NavDataFactory.hpp"
#include "RinexNavDataFactory.hpp"
#include "NavLibrary.hpp"
#include "EphemerisRange.hpp"
#include "GPSEllipsoid.hpp"
#include "SatID.hpp"
#include "NavSatelliteID.hpp"

#include "RawRange.hpp"

using namespace gnsstk;

class RawRange_T
{
public:
   RawRange_T();

   unsigned testRotateECEFTriple();
   unsigned testRotateECEFPosition();
   unsigned testRotateECEFXvt();
   unsigned testComputeRangePosition();
   unsigned testComputeRangeTriple();
   unsigned testComputeRangeXvt();
   unsigned testEstTransmitFromReceive();
   unsigned testEstTransmitFromObs();
   unsigned testFromSvPos();
   unsigned testFromSvTransmit();
   unsigned testFromSvTransmitWithObs();
   unsigned testFromReceive();
   unsigned testFromNominalReceive();
   unsigned testFromNominalReceiveWithObs();
   NavLibrary navLib;
};


RawRange_T ::
RawRange_T()
      : navLib()
{
   NavDataFactoryPtr
      ndfp(std::make_shared<RinexNavDataFactory>());

   std::string fname = getPathData() + getFileSep() +
      "arlm2000.15n";

   navLib.addFactory(ndfp);

   RinexNavDataFactory *rndfp =
      dynamic_cast<RinexNavDataFactory*>(ndfp.get());

   GNSSTK_ASSERT(rndfp->addDataSource(fname));
}


unsigned RawRange_T ::
testRotateECEFTriple()
{
   TUDEF("RawRange", "rotateECEF(Triple)");
   Triple t(-7.0e5, -5.0e6, 3.0e6);
   double dt = 0.07;
   GPSEllipsoid ellipsoid;
   Triple actual;

   actual = RawRange::rotateECEF(t, dt, ellipsoid);
   TUASSERTFESMRT(-700025.52239389379974, actual[0]);
   TUASSERTFESMRT(-4999996.4267984386533, actual[1]);
   TUASSERTFESMRT(3000000.0, actual[2]);

      // The small angle approximation approach can make a slight difference
   actual = RawRange::rotateECEF(t, dt, ellipsoid, true);
   TUASSERTFESMRT(-700025.52240301342681, actual[0]);
   TUASSERTFESMRT(-4999996.4268635781482, actual[1]);
   TUASSERTFESMRT(3000000.0, actual[2]);

   TURETURN();
}

unsigned RawRange_T ::
testRotateECEFPosition()
{
   TUDEF("RawRange", "rotateECEF(Position)");
      // The geodetic equivalent of ECEF -7.0e5, -5.0e6, 3.0e6
   Position p(30.902726259175704, 262.03038960567716,
                      -499714.8540719012,
                      Position::CoordinateSystem::Geodetic);
   double dt = 0.07;
   GPSEllipsoid ellipsoid;
   Position actual;

      // Return result should still be in the coordinate system as provided
   actual = RawRange::rotateECEF(p, dt, ellipsoid);
   TUASSERTE(Position::CoordinateSystem,
             Position::CoordinateSystem::Geodetic,
             actual.getCoordinateSystem());
   TUASSERTFESMRT(30.902726259174816192, actual[0]);
   TUASSERTFESMRT(262.03009714047612988, actual[1]);
   TUASSERTFESMRT(-499714.85406564455479, actual[2]);

      // Should be the same as the results from testRotateECEFTriple
      // but with additional fudge factor due to geodetic to ECEF conversions
   actual.asECEF();
   TUASSERTFEPS(-700025.52239389379974, actual[0], 1e-4);
   TUASSERTFEPS(-4999996.4267984386533, actual[1], 1e-4);
   TUASSERTFEPS(3000000.0, actual[2], 1e-5);

      // The small angle approximation approach can make a slight difference
   actual = RawRange::rotateECEF(p, dt, ellipsoid, true);
   actual.asECEF();
   TUASSERTFEPS(-700025.52240301342681, actual[0], 1e-4);
   TUASSERTFEPS(-4999996.4268635781482, actual[1], 1e-4);
   TUASSERTFEPS(3000000.0, actual[2], 1e-5);

   TURETURN();
}

unsigned RawRange_T ::
testRotateECEFXvt()
{
   TUDEF("RawRange", "rotateECEF(Triple)");
   Xvt xvt;
   xvt.x = Triple(-7.0e5, -5.0e6, 3.0e6);
   xvt.v = Triple(100, 200, 300);
   xvt.clkbias = 0.3;
   xvt.relcorr = 0.002;
   double dt = 0.07;
   GPSEllipsoid ellipsoid;
   Xvt actual;

   actual = RawRange::rotateECEF(xvt, dt, ellipsoid);
   TUASSERTFESMRT(-700025.52239389379974, actual.x[0]);
   TUASSERTFESMRT(-4999996.4267984386533, actual.x[1]);
   TUASSERTFESMRT(3000000.0, actual.x[2]);
   TUASSERTFESMRT(100.00102089481774215, actual.v[0]);
   TUASSERTFESMRT(199.99948954933415735, actual.v[1]);
   TUASSERTFESMRT(300.0, actual.v[2]);
      // Other Xvt member variables should be the same
   TUASSERTFE(0.3, xvt.clkbias);
   TUASSERTFE(0.002, xvt.relcorr);

      // The small angle approximation approach can make a slight difference
   actual = RawRange::rotateECEF(xvt, dt, ellipsoid, true);
   TUASSERTFESMRT(-700025.52240301342681, actual.x[0]);
   TUASSERTFESMRT(-4999996.4268635781482, actual.x[1]);
   TUASSERTFESMRT(3000000.0, actual.x[2]);
   TUASSERTFESMRT(100.00102089612053646, actual.v[0]);
   TUASSERTFESMRT(199.99948955193971756, actual.v[1]);
   TUASSERTFESMRT(300.0, actual.v[2]);

   TURETURN();
}


unsigned RawRange_T ::
testComputeRangePosition()
{
   TUDEF("RawRange", "computeRange(Position)");
       // Should be able to handle Positions in non-ECEF coordinates
   Position loc1(-7.0e5, -5.0e6, 3.0e6);
   loc1.transformTo(Position::CoordinateSystem::Geodetic);
   Position loc2(-7.0e6, -5.0e7, 3.0e7);
   loc2.transformTo(Position::CoordinateSystem::Geodetic);
   
   double dt = 0.07;
   CommonTime t1 = CommonTime::BEGINNING_OF_TIME;
   GPSEllipsoid ellipsoid;

   double range;
   Position rotatedLoc2;
   std::tie(range, rotatedLoc2) =
      RawRange::computeRange(loc1, loc2, dt, ellipsoid);
   TUASSERTFEPS(52855368.696156509221, range, 1.0e-4);
      // Returned triple should be the rotated position of loc2
   rotatedLoc2.transformTo(Position::CoordinateSystem::Cartesian);
   TUASSERTFEPS(-7000255.2239389382303, rotatedLoc2[0], 1.0e-4);
   TUASSERTFEPS(-49999964.267984382808, rotatedLoc2[1], 1.0e-4);
   TUASSERTFEPS(30000000.0, rotatedLoc2[2], 1.0e-4);

      // Again with small angle approximation
   std::tie(range, rotatedLoc2) =
      RawRange::computeRange(loc1, loc2, dt, ellipsoid, true);
   TUASSERTFEPS(52855368.696721956134, range, 1.0e-4);
      // Returned triple should be the rotated position of loc2
   rotatedLoc2.transformTo(Position::CoordinateSystem::Cartesian);
   TUASSERTFEPS(-7000255.2240301342681, rotatedLoc2[0], 1.0e-4);
   TUASSERTFEPS(-49999964.268635779619, rotatedLoc2[1], 1.0e-4);
   TUASSERTFEPS(30000000.0, rotatedLoc2[2], 1.0e-4);

      // Again with time stamp version
   std::tie(range, rotatedLoc2) =
      RawRange::computeRange(loc1, t1 + dt, loc2, t1, ellipsoid);
   TUASSERTFEPS(52855368.696156509221, range, 1.0e-4);
      // Returned triple should be the rotated position of loc2
   rotatedLoc2.transformTo(Position::CoordinateSystem::Cartesian);
   TUASSERTFEPS(-7000255.2239389382303, rotatedLoc2[0], 1.0e-4);
   TUASSERTFEPS(-49999964.267984382808, rotatedLoc2[1], 1.0e-4);
   TUASSERTFEPS(30000000.0, rotatedLoc2[2], 1.0e-4);

      // Again with time stamp version and small angle approximation
   std::tie(range, rotatedLoc2) =
      RawRange::computeRange(loc1, t1 + dt, loc2, t1, ellipsoid, true);
   TUASSERTFEPS(52855368.696721956134, range, 1.0e-4);
      // Returned triple should be the rotated position of loc2
   rotatedLoc2.transformTo(Position::CoordinateSystem::Cartesian);
   TUASSERTFEPS(-7000255.2240301342681, rotatedLoc2[0], 1.0e-4);
   TUASSERTFEPS(-49999964.268635779619, rotatedLoc2[1], 1.0e-4);
   TUASSERTFEPS(30000000.0, rotatedLoc2[2], 1.0e-4);

   TURETURN();
}

unsigned RawRange_T ::
testComputeRangeTriple()
{
   TUDEF("RawRange", "computeRange(Triple)");
   Triple loc1(-7.0e5, -5.0e6, 3.0e6);
   Triple loc2(-7.0e6, -5.0e7, 3.0e7);
   double dt = 0.07;
   CommonTime t1 = CommonTime::BEGINNING_OF_TIME;
   GPSEllipsoid ellipsoid;

   double range;
   Triple rotatedLoc2;
   std::tie(range, rotatedLoc2) =
      RawRange::computeRange(loc1, loc2, dt, ellipsoid);
   TUASSERTFESMRT(52855368.696156509221, range);
      // Returned triple should be the rotated position of loc2
   TUASSERTFESMRT(-7000255.2239389382303, rotatedLoc2[0]);
   TUASSERTFESMRT(-49999964.267984382808, rotatedLoc2[1]);
   TUASSERTFESMRT(30000000.0, rotatedLoc2[2]);

      // Again with small angle approximation
   std::tie(range, rotatedLoc2) =
      RawRange::computeRange(loc1, loc2, dt, ellipsoid, true);
   TUASSERTFESMRT(52855368.696721956134, range);
      // Returned triple should be the rotated position of loc2
   TUASSERTFESMRT(-7000255.2240301342681, rotatedLoc2[0]);
   TUASSERTFESMRT(-49999964.268635779619, rotatedLoc2[1]);
   TUASSERTFESMRT(30000000.0, rotatedLoc2[2]);

      // Again with time stamp version
   std::tie(range, rotatedLoc2) =
      RawRange::computeRange(loc1, t1 + dt, loc2, t1, ellipsoid);
   TUASSERTFESMRT(52855368.696156509221, range);
      // Returned triple should be the rotated position of loc2
   TUASSERTFESMRT(-7000255.2239389382303, rotatedLoc2[0]);
   TUASSERTFESMRT(-49999964.267984382808, rotatedLoc2[1]);
   TUASSERTFESMRT(30000000.0, rotatedLoc2[2]);

      // Again with time stamp version and small angle approximation
   std::tie(range, rotatedLoc2) =
      RawRange::computeRange(loc1, t1 + dt, loc2, t1, ellipsoid, true);
   TUASSERTFESMRT(52855368.696721956134, range);
      // Returned triple should be the rotated position of loc2
   TUASSERTFESMRT(-7000255.2240301342681, rotatedLoc2[0]);
   TUASSERTFESMRT(-49999964.268635779619, rotatedLoc2[1]);
   TUASSERTFESMRT(30000000.0, rotatedLoc2[2]);

   TURETURN();
}

unsigned RawRange_T ::
testComputeRangeXvt()
{
   TUDEF("RawRange", "computeRange(Xvt)");
   Triple loc1(-7.0e5, -5.0e6, 3.0e6);

   Xvt xvt;
   xvt.x = Triple(-7.0e6, -5.0e7, 3.0e7);
   xvt.v = Triple(100, 200, 300);

   double dt = 0.07;
   CommonTime t1 = CommonTime::BEGINNING_OF_TIME;
   GPSEllipsoid ellipsoid;

   double range;
   Xvt rotatedXvt;
   std::tie(range, rotatedXvt) =
      RawRange::computeRange(loc1, xvt, dt, ellipsoid);
   TUASSERTFESMRT(52855368.696156509221, range);
      // Returned xvt should be the rotated position and velocity
   TUASSERTFESMRT(-7000255.2239389382303, rotatedXvt.x[0]);
   TUASSERTFESMRT(-49999964.267984382808, rotatedXvt.x[1]);
   TUASSERTFESMRT(30000000.0, rotatedXvt.x[2]);
   TUASSERTFESMRT(100.00102089481774215, rotatedXvt.v[0]);
   TUASSERTFESMRT(199.99948954933415735, rotatedXvt.v[1]);
   TUASSERTFESMRT(300.0, rotatedXvt.v[2]);

      // Again with small angle approximation
   std::tie(range, rotatedXvt) =
      RawRange::computeRange(loc1, xvt, dt, ellipsoid, true);
   TUASSERTFESMRT(52855368.696721956134, range);
      // Returned xvt should be the rotated position and velocity
   TUASSERTFESMRT(-7000255.2240301342681, rotatedXvt.x[0]);
   TUASSERTFESMRT(-49999964.268635779619, rotatedXvt.x[1]);
   TUASSERTFESMRT(30000000.0, rotatedXvt.x[2]);
   TUASSERTFESMRT(100.00102089612053646, rotatedXvt.v[0]);
   TUASSERTFESMRT(199.99948955193971756, rotatedXvt.v[1]);
   TUASSERTFESMRT(300.0, rotatedXvt.v[2]);

      // Again with time stamp version
   std::tie(range, rotatedXvt) =
      RawRange::computeRange(loc1, t1 + dt, xvt, t1, ellipsoid);
   TUASSERTFESMRT(52855368.696156509221, range);
      // Returned xvt should be the rotated position and velocity
   TUASSERTFESMRT(-7000255.2239389382303, rotatedXvt.x[0]);
   TUASSERTFESMRT(-49999964.267984382808, rotatedXvt.x[1]);
   TUASSERTFESMRT(30000000.0, rotatedXvt.x[2]);
   TUASSERTFESMRT(100.00102089481774215, rotatedXvt.v[0]);
   TUASSERTFESMRT(199.99948954933415735, rotatedXvt.v[1]);
   TUASSERTFESMRT(300.0, rotatedXvt.v[2]);

      // Again with time stamp version and small angle approximation
   std::tie(range, rotatedXvt) =
      RawRange::computeRange(loc1, t1 + dt, xvt, t1, ellipsoid, true);
   TUASSERTFESMRT(52855368.696721956134, range);
      // Returned xvt should be the rotated position and velocity
   TUASSERTFESMRT(-7000255.2240301342681, rotatedXvt.x[0]);
   TUASSERTFESMRT(-49999964.268635779619, rotatedXvt.x[1]);
   TUASSERTFESMRT(30000000.0, rotatedXvt.x[2]);
   TUASSERTFESMRT(100.00102089612053646, rotatedXvt.v[0]);
   TUASSERTFESMRT(199.99948955193971756, rotatedXvt.v[1]);
   TUASSERTFESMRT(300.0, rotatedXvt.v[2]);

   TURETURN();
}

unsigned RawRange_T ::
testEstTransmitFromReceive()
{
   TUDEF("RawRange", "estTransmitFromReceive()");

   Triple rxPos(-7.0e5, -5.0e6, 3.0e6);
   NavSatelliteID sat(SatID(5, SatelliteSystem::GPS));
   CommonTime receive(CivilTime(2015, 7, 19, 2, 0, 0.0, TimeSystem::GPS));
   GPSEllipsoid ellipsoid;

   bool success;
   CommonTime time;
   std::tie(success, time) = RawRange::estTransmitFromReceive(
      rxPos,
      receive,
      navLib,
      sat,
      ellipsoid);

   TUASSERT(success);
   CommonTime exp(CivilTime(2015, 7, 19, 1, 59, 59.92565104107503, TimeSystem::GPS));
   TUASSERT(abs(exp - time) < 1e-9);

   TURETURN();
}

unsigned RawRange_T ::
testEstTransmitFromObs()
{
   TUDEF("RawRange", "estTransmitFromObs()");

   Triple rxPos(-7.0e5, -5.0e6, 3.0e6);
   NavSatelliteID sat(SatID(5, SatelliteSystem::GPS));
   CommonTime receive(CivilTime(2015, 7, 19, 2, 0, 0.0, TimeSystem::GPS));
   double pseudorange = 2e7;
   GPSEllipsoid ellipsoid;

   bool success;
   CommonTime time;
   std::tie(success, time) = RawRange::estTransmitFromObs(
      receive,
      pseudorange,
      navLib,
      sat);

   TUASSERT(success);
   CommonTime exp(CivilTime(2015, 7, 19, 1, 59, 59.93350360018778, TimeSystem::GPS));
   TUASSERT(abs(exp - time) < 1e-9);

   TURETURN();
}

unsigned RawRange_T ::
testFromReceive()
{
   TUDEF("RawRange", "fromReceive()");

   Position rxLocation(-7.0e5, -5.0e6, 3.0e6);
   NavSatelliteID satId(SatID(5, SatelliteSystem::GPS));
   CommonTime time(CivilTime(2015,7,19,2,0,0.0,TimeSystem::GPS));
   GPSEllipsoid ellipsoid;

   Xvt xvt;
   double range;
   bool success;
   std::tie(success, range, xvt) =
      RawRange::fromReceive(rxLocation, time, navLib, satId, ellipsoid);

      // @note These fields are slightly different than the Xvt tagged
      // at the given time. RawRange tries to determine the position of the SV
      // at time of transmit but rotates the coordinates into the ECEF frame
      // at time of receive.
   TUASSERT(success);
   TUASSERTFESMRT(9272533.3762740660459,  xvt.x[0]);
   TUASSERTFESMRT(-12471159.893898375332, xvt.x[1]);
   TUASSERTFESMRT(21480836.886172864586,  xvt.x[2]);
   TUASSERTFESMRT(2077.3531590469665389,  xvt.v[0]);
   TUASSERTFESMRT(1796.0597201257969573,  xvt.v[1]);
   TUASSERTFESMRT(164.40771415613258455,  xvt.v[2]);
   TUASSERTFESMRT(-0.00021641042654059759786, xvt.clkbias);
   TUASSERTFESMRT(4.3200998334200003381e-12,  xvt.clkdrift);
   TUASSERTFESMRT(-8.8008986915573278037e-09, xvt.relcorr);
   TUASSERTFESMRT(22289257.145863413811, range);
   TURETURN();
}


unsigned RawRange_T ::
testFromNominalReceiveWithObs()
{
   TUDEF("RawRange", "testFromNominalReceiveWithObs");

   Position rxLocation(-7.0e5, -5.0e6, 3.0e6);
   NavSatelliteID satId(SatID(5, SatelliteSystem::GPS));
   CommonTime
      time(CivilTime(2015,7,19,2,0,0.0,TimeSystem::GPS));
   double pseduorange = 2e7;
   GPSEllipsoid ellipsoid;

   Xvt xvt;
   double range;
   bool success;
   std::tie(success, range, xvt) =
      RawRange::fromNominalReceiveWithObs(rxLocation, time, pseduorange,
                                          navLib, satId, ellipsoid);

      // @note These fields are slightly different than the Xvt tagged
      // at the given time. RawRange tries to determine the position of the SV
      // at time of transmit but rotates the coordinates into the ECEF frame
      // at time of receive.
   TUASSERT(success);
   TUASSERTFESMRT(9272549.688764328137,   xvt.x[0]);
   TUASSERTFESMRT(-12471145.790274851024, xvt.x[1]);
   TUASSERTFESMRT(21480838.177179995924,  xvt.x[2]);
   TUASSERTFESMRT(2077.3540461841112119,  xvt.v[0]);
   TUASSERTFESMRT(1796.058914385570688,  xvt.v[1]);
   TUASSERTFESMRT(164.40410655412986785,  xvt.v[2]);
   TUASSERTFESMRT(-0.000216410426506673752147, xvt.clkbias);
   TUASSERTFESMRT(4.3200998334200003381e-12,  xvt.clkdrift);
   TUASSERTFESMRT(-8.8009032843852035206e-09, xvt.relcorr);
   TUASSERTFESMRT(22289260.787328250706, range);
   TURETURN();
}


unsigned RawRange_T ::
testFromSvTransmitWithObs()
{
   TUDEF("RawRange", "fromSvTransmitWithObs");

   Position rxLocation(-7.0e5, -5.0e6, 3.0e6);
   NavSatelliteID satId(SatID(5, SatelliteSystem::GPS));
   CommonTime
      time(CivilTime(2015,7,19,2,0,0.0,TimeSystem::GPS));
   double pseduorange = 2e7;
   GPSEllipsoid ellipsoid;

   Xvt xvt;
   double range;
   bool success;
   std::tie(success, range, xvt) =
      RawRange::fromSvTransmitWithObs(rxLocation, pseduorange, navLib,
                                      satId, time, ellipsoid);

      // @note These fields are slightly different than the Xvt tagged
      // at the given time. RawRange tries to determine the position of the SV
      // at time of transmit but rotates the coordinates into the ECEF frame
      // at time of receive.
   TUASSERT(success);
   TUASSERTFESMRT(9272694.5731354933232,  xvt.x[0]);
   TUASSERTFESMRT(-12471021.341979622841, xvt.x[1]);
   TUASSERTFESMRT(21480849.108445473015,  xvt.x[2]);
   TUASSERTFESMRT(2077.3605866147809138,  xvt.v[0]);
   TUASSERTFESMRT(1796.0532152160560599,  xvt.v[1]);
   TUASSERTFESMRT(164.37355694668559636,  xvt.v[2]);
   TUASSERTFESMRT(-0.00021641042621940267715, xvt.clkbias);
   TUASSERTFESMRT(4.3200998334200003381e-12,  xvt.clkdrift);
   TUASSERTFESMRT(-8.8009421765298224418e-09, xvt.relcorr);
   TUASSERTFESMRT(22289292.961206533015, range);
   TURETURN();
}


unsigned RawRange_T ::
testFromNominalReceive()
{
   TUDEF("RawRange", "fromNominalReceive");

   Position rxLocation(-7.0e5, -5.0e6, 3.0e6);
   NavSatelliteID satId(SatID(5, SatelliteSystem::GPS));
   CommonTime
      time(CivilTime(2015,7,19,2,0,0.0,TimeSystem::GPS));
   GPSEllipsoid ellipsoid;

   Xvt xvt;
   double range;
   bool success;
   std::tie(success, range, xvt) =
      RawRange::fromNominalReceive(rxLocation, time, navLib, satId, ellipsoid);

      // @note These fields are slightly different than the Xvt tagged
      // at the given time. RawRange tries to determine the position of the SV
      // at time of transmit but rotates the coordinates into the ECEF frame
      // at time of receive.
   TUASSERT(success);
   TUASSERTFESMRT(9272533.3759945072234, xvt.x[0]);
   TUASSERTFESMRT(-12471159.894135156646, xvt.x[1]);
   TUASSERTFESMRT(21480836.886153958738, xvt.x[2]);
   TUASSERTFESMRT(2077.353159039827915, xvt.v[0]);
   TUASSERTFESMRT(1796.0597201308269177, xvt.v[1]);
   TUASSERTFESMRT(164.40771420896862764, xvt.v[2]);
   TUASSERTFESMRT(-0.00021641042654059808575, xvt.clkbias);
   TUASSERTFESMRT(4.3200998334200003381e-12,  xvt.clkdrift);
   TUASSERTFESMRT(-8.8008986914900614763e-09, xvt.relcorr);
   TUASSERTFESMRT(22289257.145802032202, range);
   TURETURN();
}

unsigned RawRange_T ::
testFromSvPos()
{
   TUDEF("RawRange","fromSvPos");

   Position rxPos(-7.0e5, -5.0e6, 3.0e6);
   Xvt xvt;
   xvt.x = Triple(-9e5, -2e7, 9e6);
   xvt.v = Triple(6e2, 1e3, 2e4);
   xvt.clkbias = -0.3;
   xvt.clkdrift = 0;
   xvt.relcorr = -3.5e-6;
   GPSEllipsoid ellipsoid;

   double range;
   bool success;
   Xvt returnXvt;
   std::tie(success, range, returnXvt) =
      RawRange::fromSvPos(rxPos, xvt, ellipsoid);
   TUASSERT(success);
   TUASSERTFESMRT(16156730.032176425681, range);
   TUASSERTFESMRT(-900078.59885879501235, returnXvt.x[0]);
   TUASSERTFESMRT(-19999996.462896592915, returnXvt.x[1]);
   TUASSERTFESMRT(9000000.0, returnXvt.x[2]);
   TUASSERTFESMRT(600.00392993865398239, returnXvt.v[0]);
   TUASSERTFESMRT(999.99764202630535692, returnXvt.v[1]);
   TUASSERTFESMRT(20000.0, returnXvt.v[2]);

   TURETURN();
}

unsigned RawRange_T ::
testFromSvTransmit()
{
   TUDEF("RawRange","fromSvTransmit");

   Position rxPos(-7.0e5, -5.0e6, 3.0e6);
   NavSatelliteID satId(SatID(5, SatelliteSystem::GPS));
   CommonTime
      time(CivilTime(2015,7,19,2,0,0.0,TimeSystem::GPS));
   GPSEllipsoid ellipsoid;

   double range;
   bool success;
   Xvt returnXvt;
   std::tie(success, range, returnXvt) =
      RawRange::fromSvTransmit(rxPos, navLib, satId, time, ellipsoid);
   TUASSERT(success);
   TUASSERTFESMRT(22289291.623843517154, range);
   TUASSERTFESMRT(9272687.8255264088511, returnXvt.x[0]);
   TUASSERTFESMRT(-12471026.359089374542, returnXvt.x[1]);
   TUASSERTFESMRT(21480849.108445473015, returnXvt.x[2]);
   TUASSERTFESMRT(2077.361558392346069, returnXvt.v[0]);
   TUASSERTFESMRT(1796.0520912329243401, returnXvt.v[1]);
   TUASSERTFESMRT(164.37355694668559636, returnXvt.v[2]);

   TURETURN();
}


int main()
{
   RawRange_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.testRotateECEFTriple();
   errorTotal += testClass.testRotateECEFPosition();
   errorTotal += testClass.testRotateECEFXvt();
   errorTotal += testClass.testComputeRangePosition();
   errorTotal += testClass.testComputeRangeTriple();
   errorTotal += testClass.testComputeRangeXvt();
   errorTotal += testClass.testEstTransmitFromReceive();
   errorTotal += testClass.testEstTransmitFromObs();
   errorTotal += testClass.testFromSvPos();
   errorTotal += testClass.testFromSvTransmit();
   errorTotal += testClass.testFromSvTransmitWithObs();
   errorTotal += testClass.testFromReceive();
   errorTotal += testClass.testFromNominalReceive();
   errorTotal += testClass.testFromNominalReceiveWithObs();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;

   return errorTotal;
}
