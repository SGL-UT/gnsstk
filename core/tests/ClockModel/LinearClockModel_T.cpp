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

#include "TestUtil.hpp"
#include "LinearClockModel.hpp"

#include "ObsRngDev.hpp"
#include <iostream>

#include "NavLibrary.hpp"
#include "RinexNavDataFactory.hpp"

#include "EphemerisRange.hpp"
#include "SimpleTropModel.hpp"



using namespace std;
using namespace gnsstk;

ObsRngDev generateOrd(double ord, CommonTime time, SatID svid) {
    ObsRngDev newOrd;
    newOrd.ord = ord;
    newOrd.obstime = time;
    newOrd.svid = svid;
    const float azimuth = 0;
    newOrd.azimuth = vfloat(azimuth);
    const float elevation = 15;
    newOrd.elevation = vfloat(elevation);
    const short health = 0;
    newOrd.health = vshort(health);
    const short iodc = 0;
    newOrd.iodc = vshort(iodc);
    const double rho = 0;
    newOrd.rho = vdouble(rho);
    const double iono = 0;
    newOrd.iono = vdouble(iono);
    const double trop = 0;
    newOrd.trop = vdouble(trop);
    return newOrd;
}

ORDEpoch generateEpoch(double base, double deviation, CommonTime time, double deviationMultiplier = 1) {
    ORDEpoch oe;
    
    SatID SV1 = SatID(1, SatelliteSystem::GPS);
    SatID SV2 = SatID(2, SatelliteSystem::GPS);
    SatID SV3 = SatID(3, SatelliteSystem::GPS);
    SatID SV4 = SatID(4, SatelliteSystem::GPS);
    SatID SV5 = SatID(5, SatelliteSystem::GPS);

    oe.ords[SV1] = generateOrd(base + deviation * deviationMultiplier, time, SV1);
    oe.ords[SV2] = generateOrd(base + deviation, time, SV2);
    oe.ords[SV3] = generateOrd(base + deviation, time, SV3);
    oe.ords[SV4] = generateOrd(base + deviation, time, SV4);
    oe.ords[SV5] = generateOrd(base + deviation, time, SV5);

    oe.time = time;
    return oe;
}


class LinearClockModel_T
{
    public:
        LinearClockModel_T() {};
        ~LinearClockModel_T(){};
        int test_constructor();
        int test_isOffsetValid();
        int test_getOffset();
        int test_addEpoch();
        int test_dump();
};

//============================================================
// Definitions for test class
//============================================================


//----------------------------------------
// Constructor tests
//----------------------------------------

int LinearClockModel_T :: test_constructor( void )
{
    TUDEF("LinearClockModel", "constructor");

    try
    {
        LinearClockModel testLinearClockModel;
        TUPASS("We have successfully constructed an LinearClockModel");
    }
    catch(...)
    {
        TUFAIL("constructor threw an exception");
    }

    TURETURN();
}

//----------------------------------------
// Mode test
//----------------------------------------

int LinearClockModel_T :: test_addEpoch( void ) 
{
    TUDEF("LinearClockModel", "addEpoch");
    LinearClockModel testLinearClockModel;

    // Create a start time.
    CommonTime time;
    long day = 245924; // February 1st 2021
    long secondsOfDay = 0;
    double fractionalSeconds = 0.0;
    time.set(day, secondsOfDay, fractionalSeconds, TimeSystem::UTC);

    double timeStep = 100; // Time in seconds at which we add need Epochs

    // Mock 1000 equivalent Epochs over 100000 seconds
    for (int i = 0; i < 1000; i++) {
        time.addSeconds(timeStep);
        ORDEpoch oe = generateEpoch(500, 50, time);
        testLinearClockModel.addEpoch(oe);
    }

    // Expect slope to be zero, and offset to equal base
    std::string linerExpectation = "base: 0245924 00100000 0.000000000000000 UTC, start: 0245924 00100000 0.000000000000000 UTC, end: 0245925 13600000 0.000000000000000 UTC\n"
                                   "Clock: est(end)=550, n=95, b=550, m=0, sigma=0, r=0\n";
    std::ostringstream out;
    testLinearClockModel.dump(out, 0);
    TUASSERTE(string, linerExpectation,  out.str());


    // Mock 1000 Epochs with slowly deviating ords
    for (int i = 0; i < 1000; i++) {
        time.addSeconds(timeStep);
        ORDEpoch oe = generateEpoch(500, 50 + i, time);
        testLinearClockModel.addEpoch(oe);
    }

    // Expect slope, standard deviation, offset and correlation to have changed
    std::string changeInSlopeExpectation = "base: 0245924 00100000 0.000000000000000 UTC, start: 0245924 00100000 0.000000000000000 UTC, end: 0245926 27200000 0.000000000000000 UTC\n"
                                           "Clock: est(end)=1549, n=95, b=-450, m=0.01, sigma=5.50628, r=1\n";
    std::ostringstream out2;
    testLinearClockModel.dump(out2, 0);
    TUASSERTE(string, changeInSlopeExpectation,  out2.str());

    // Add 6 Epochs with 10x deviation 
    for (int i = 0; i < 6; ++i) {
        time.addSeconds(timeStep);
        ORDEpoch oe = generateEpoch(500, 50, time, 10);
        testLinearClockModel.addEpoch(oe);
    }
    
    // Expect a reset after 5+ extreme deviations 
    std::string emptyClockExpectation = "base: 0245924 00100000 0.000000000000000 UTC, start: 3442448 00000000 0.000000000000000 Any, end: 0000000 00000000 0.000000000000000 Any\n"
                                        "Clock: est(end)=0, n=0, b=0, m=0, sigma=0, r=0\n";

    std::ostringstream out4;
    testLinearClockModel.dump(out4, 0);
    TUASSERTE(string, emptyClockExpectation,  out4.str());

    // Add Epochs on both ends of the time spectrum
    testLinearClockModel.addEpoch(generateEpoch(500, 50, time));
    testLinearClockModel.addEpoch(generateEpoch(500, 50, gnsstk::CommonTime::BEGINNING_OF_TIME));
    testLinearClockModel.addEpoch(generateEpoch(500, 50, gnsstk::CommonTime::END_OF_TIME));

    // Expect time range to now expand all of time
    std::string timeRangeChangeExpectation = "base: 0245926 27800000 0.000000000000000 UTC, start: 0000000 00000000 0.000000000000000 Any, end: 3442448 00000000 0.000000000000000 Any\n"
                                             "Clock: est(end)=550, n=5, b=550, m=0, sigma=0, r=0\n";
    std::ostringstream out3;
    testLinearClockModel.dump(out3, 0);
    TUASSERTE(string, timeRangeChangeExpectation,  out3.str());

    TURETURN();
}

int LinearClockModel_T :: test_dump( void )
{
    TUDEF("LinearClockModel", "dump");
    LinearClockModel testLinearClockModel;

    // Create a start time.
    CommonTime time;
    long day = 245924; // February 1st 2021
    long secondsOfDay = 0;
    double fractionalSeconds = 0.0;
    time.set(day, secondsOfDay, fractionalSeconds, TimeSystem::UTC);

    double timeStep = 100; // Time in seconds at which we add need Epochs

    // Mock 1000 equivalent Epochs over 100000 seconds
    for (int i = 0; i < 1000; i++) {
        time.addSeconds(timeStep);
        ORDEpoch oe = generateEpoch(500, 50, time);
        testLinearClockModel.addEpoch(oe);
    }

    // Expect slope to be zero, and offset to equal base
    std::string dumpExpectation = "base: 0245924 00100000 0.000000000000000 UTC, start: 0245924 00100000 0.000000000000000 UTC, end: 0245925 13600000 0.000000000000000 UTC\n"
                                   "Clock: est(end)=550, n=95, b=550, m=0, sigma=0, r=0\n"
                                   "min elev: 0, max sigma: 2\n"
                                   "GPS 1/0 GPS 2/0 GPS 3/0 GPS 4/0 GPS 5/0 \n";
    
    std::ostringstream out;
    testLinearClockModel.dump(out, 1);
    TUASSERTE(string, dumpExpectation,  out.str());

    TURETURN();
}


int LinearClockModel_T :: test_isOffsetValid( void )
{
    TUDEF("LinearClockModel", "isOffsetValid");

    LinearClockModel testLinearClockModel;

    // Create a start time.
    CommonTime time;
    long day = 245924; // February 1st 2021
    long secondsOfDay = 0;
    double fractionalSeconds = 0.0;
    time.set(day, secondsOfDay, fractionalSeconds, TimeSystem::UTC);

    double timeStep = 100; // Time in seconds at which we add need Epochs

    // Clock with no Epochs should be invalid
    TUASSERT(!testLinearClockModel.isOffsetValid(time));

    // Mock 1000 equivalent Epochs over 100000 seconds
    for (int i = 0; i < 1000; i++) {
        time.addSeconds(timeStep);
        ORDEpoch oe = generateEpoch(500, 50, time);
        testLinearClockModel.addEpoch(oe);
    }

    // The last time added should be valid
    TUASSERT(testLinearClockModel.isOffsetValid(time));

    CommonTime timeBeforeEpochs;
    timeBeforeEpochs.set(0, 0, 0, TimeSystem::UTC);

    // A time before any of the added Epochs should be invalid
    TUASSERT(!testLinearClockModel.isOffsetValid(timeBeforeEpochs));

    CommonTime timeAfterEpochs;
    timeAfterEpochs.set(day + 3, 0, 0, TimeSystem::UTC);

    // A time after all the added Epochs should be invalid
    TUASSERT(!testLinearClockModel.isOffsetValid(timeAfterEpochs));

    TURETURN();
}

int LinearClockModel_T :: test_getOffset( void )
{
    TUDEF("LinearClockModel", "getOffset");

    LinearClockModel testLinearClockModel;

    // Create a start time.
    CommonTime time;
    long day = 245924; // February 1st 2021
    long secondsOfDay = 0;
    double fractionalSeconds = 0.0;
    time.set(day, secondsOfDay, fractionalSeconds, TimeSystem::UTC);

    double timeStep = 100; // Time in seconds at which we add need Epochs

    // Clock with no Epochs should be invalid
    TUASSERT(!testLinearClockModel.isOffsetValid(time));

    // Mock 1000 equivalent Epochs over 100000 seconds
    for (int i = 0; i < 1000; i++) {
        time.addSeconds(timeStep);
        ORDEpoch oe = generateEpoch(500, 50, time);
        testLinearClockModel.addEpoch(oe);
    }

    TUASSERTFE(550, testLinearClockModel.getOffset(time));

    double oneDay = 86400;
    time.addSeconds(oneDay);

    TUASSERTFE(0, testLinearClockModel.getOffset(time));

    TURETURN();
}



//============================================================
// main() for test application to be executed by CMake/CTest
//============================================================

int main( )
{
   int  errorTotal = 0;

   LinearClockModel_T  testClass;

   errorTotal += testClass.test_constructor();
   errorTotal += testClass.test_addEpoch();
   errorTotal += testClass.test_dump();
   errorTotal += testClass.test_isOffsetValid();
   errorTotal += testClass.test_getOffset();
   errorTotal += testClass.test_dump();
   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;

   return( errorTotal );
}

//============================================================
// The End
//============================================================
