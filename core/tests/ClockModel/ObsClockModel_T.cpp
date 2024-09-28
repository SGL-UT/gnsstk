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
#include "ObsClockModel.hpp"

#include "ObsRngDev.hpp"
#include "TestUtil.hpp"
#include <iostream>

#include "NavLibrary.hpp"
#include "RinexNavDataFactory.hpp"

#include "EphemerisRange.hpp"
#include "SimpleTropModel.hpp"

using namespace std;
using namespace gnsstk;

//============================================================
// Declarations for test class
//============================================================

std::string unexpectedError = "An unexpected exception occurred";

namespace gnsstk
{
  class ObsClockModel_Child : public ObsClockModel
  {
    public:
      virtual void addEpoch(const ORDEpoch& re) {}
      virtual double getOffset(const gnsstk::CommonTime& t) const {return(0.0);}
      virtual bool isOffsetValid(const gnsstk::CommonTime& t) const {return false;}

      void simulateSvStatus(const SatID& svid, SvStatus newStatus) {
        this->status[svid] = newStatus;
      }
  };
};


class ObsClockModel_T
{
    public:
        ObsClockModel_T() {};
        ~ObsClockModel_T(){};
        int test_constructor();
        int test_constructor_defaults();
        int test_setAndGetSvModeMap();
        int test_setAndGetSvMode_individual();
        int test_setAndGetSvMode_all();
        int test_getSvModeErrorThrow();
        int test_setAndGetSigmaMultiplier();
        int test_setAndGetElevationMask();
        int test_setAndGetUseWonkyData();
        int test_getSvStatusMapAndGetSvStatus();
        int test_getSvStatusErrorThrow();
        int test_simpleOrdClock();
        int test_dump();

        gnsstk::ObsRngDev makeORD(const double prange, const float elevation, const short health, const unsigned wonky) {

                gnsstk::NavLibrary ephemStore;
                gnsstk::WGS84Ellipsoid em;
                gnsstk::NavDataFactoryPtr ndfp;
                gnsstk::Position receiverPos;
                gnsstk::CommonTime obsTime = gnsstk::CivilTime(2006, 1, 31, 2, 0, 0, gnsstk::TimeSystem::GPS).convertToCommonTime();
                SatID SV = SatID(1, SatelliteSystem::GPS);

                receiverPos.setGeodetic(30.387577, -97.727607, 240);
                std::string path = gnsstk::getPathData() + "/test_input_rinex_nav_ephemerisData.031";
                ndfp = std::make_shared<gnsstk::RinexNavDataFactory>();
                ndfp->addDataSource(path);
                ephemStore.addFactory(ndfp);

                gnsstk::ObsRngDev ord(prange, SV, obsTime, receiverPos, ephemStore, em); 


                ord.elevation =  vfloat(elevation);
                ord.health = vshort(health);
                ord.wonky = wonky;

                return ord;
        }

};

//============================================================
// Definitions for test class
//============================================================


//----------------------------------------
// Constructor tests
//----------------------------------------

int ObsClockModel_T :: test_constructor( void )
{
    TUDEF("ObsClockModel", "constructor");
    // Test Description: ObsClockModel objects are created with default constructor

    try
    {
        ObsClockModel_Child testObsClockModel;
        TUPASS("We have successfully constructed an ObsClockModel");
    }
    catch(...)
    {
        TUFAIL("constructor threw an exception");
    }

    TURETURN();
}


int ObsClockModel_T :: test_constructor_defaults( void )
{
    TUDEF("ObsClockModel", "constructor defaults");
    // Test Description: ObsClockModel constructor with default values


    try
    {
        ObsClockModel_Child testObsClockModel;
        SatID testSatID( 1, SatelliteSystem::GPS );

        TUASSERT(testObsClockModel.getSigmaMultiplier() == 2.0); // Default sigma value is not as expected
        TUASSERT(testObsClockModel.getElevationMask() == 0.0 ); // Default elevation mask value is not as expected
        TUASSERT(testObsClockModel.getSvMode( testSatID ) == ObsClockModel::ALWAYS) // Default SvMode value is not as expected
        TUASSERT(testObsClockModel.getUseWonkyData() == false); // Default useWonkyData value is not as expected

        TUPASS("We have successfully tested default constructor values for ObsClockModel");
    }
    catch(...)
    {
        TUFAIL("constructor threw an exception while testing default data member values");
    }

    TURETURN();
}


//----------------------------------------
// Methods
// - setSvModeMap
// - getSvModeMap
// - setSvMode
// - getSvMode
// - setSigmaMultiplier
// - getSigmaMultiplier
// - setElevationMask
// - getElevationMask
// - setUseWonkyData
// - getUseWonkyData
// - getSvStatusMap
// - getSvStatus
// - simpleOrdClock
// - dump
//----------------------------------------

int ObsClockModel_T::test_setAndGetSvModeMap()
{
    TUDEF("ObsClockModel", "setAndGetSvModeMap");
    // Verify that setSvModeMap correctly sets the SV mode map and getSvModeMap correctly retrieves SV mode map

    try
    {
        ObsClockModel_Child testObsClockModel;
        ObsClockModel::SvModeMap newModes;

        // All mode types
        newModes[SatID(1, SatelliteSystem::GPS)] = ObsClockModel::ALWAYS;
        newModes[SatID(2, SatelliteSystem::GPS)] = ObsClockModel::HEALTHY;
        newModes[SatID(3, SatelliteSystem::GPS)] = ObsClockModel::IGNORE;

        testObsClockModel.setSvModeMap(newModes);

        const auto& modesSet = testObsClockModel.getSvModeMap();
        for (int prn = 1; prn <= gnsstk::MAX_PRN; prn++)
        {
            SatID svid(prn, SatelliteSystem::GPS);
            ObsClockModel::SvMode expectedMode = ObsClockModel::IGNORE;

            if (newModes.find(svid) != newModes.end())
            {
                expectedMode = newModes[svid];
            }

            TUASSERT(modesSet.at(svid) == expectedMode);
        }
    }
    catch (const std::exception& e)
    {
        TUFAIL("Exception: " + std::string(e.what()));
    }
    catch (...)
    {
        TUFAIL("Unknown exception occurred");
    }

    TURETURN();
}

int ObsClockModel_T::test_setAndGetSvMode_individual()
{
    TUDEF("ObsClockModel", "setAndGetSvMode_individual");
    // Verify that setSvMode can set a individuals svMode and getSvMode can retrieve it
    try
    {
        ObsClockModel_Child testObsClockModel;
        SatID testSvid(1, SatelliteSystem::GPS);
        ObsClockModel::SvMode expectedMode = ObsClockModel::ALWAYS;

        testObsClockModel.setSvMode(testSvid, expectedMode);
        ObsClockModel::SvMode returnedMode = testObsClockModel.getSvMode(testSvid);

        TUASSERT(returnedMode == expectedMode);
    }
    catch (const gnsstk::Exception& e)
    {
        TUFAIL("Exception: " + std::string(e.what()));
    }
    catch (...)
    {
        TUFAIL("Unknown exception occurred");
    }

    TURETURN();
}


int ObsClockModel_T::test_setAndGetSvMode_all()
{
    TUDEF("ObsClockModel", "setAndGetSvMode_all");
    // Verify that setSvMode can set a all svModes when not given an individual sv to set

    try
    {
        ObsClockModel_Child testObsClockModel;
        ObsClockModel::SvMode expectedMode = ObsClockModel::HEALTHY;

        testObsClockModel.setSvMode(expectedMode);

        bool allMatch = true;
        for (int prn = 1; prn <= gnsstk::MAX_PRN; prn++)
        {
            SatID svid(prn, SatelliteSystem::GPS);
            if (testObsClockModel.getSvMode(svid) != expectedMode)
            {
                allMatch = false;
            }
        }

        TUASSERT(allMatch);
    }
    catch (const gnsstk::Exception& e)
    {
        TUFAIL("Exception: " + std::string(e.what()));
    }
    catch (...)
    {
        TUFAIL("Unknown exception occurred");
    }

    TURETURN();
}


int ObsClockModel_T::test_getSvModeErrorThrow() {
    TUDEF("ObsClockModel", "getSvModeErrorThrow");
    // Verify that an error is thrown when attempting to get a SV mode that does nto exist

    try {
        ObsClockModel_Child testObsClockModel;

        SatID SV1;

        TUTHROW(testObsClockModel.getSvMode(SV1));

    } catch (...) {

        TUFAIL("Unexpected getSvMode did not throw error when look for a no existant SV");

    }

    TURETURN();
}


int ObsClockModel_T::test_setAndGetSigmaMultiplier()
{
    TUDEF("ObsClockModel", "setAndGetSigmaMultiplier");
    // Verify that setSigmaMultiplier can set the sigma multiplier and getSigmaMultiplier can retrieve it.

    try
    {
        ObsClockModel_Child testObsClockModel;

        double defaultSigmaMultiplier = 2.0;
        double initialSigmaMultiplier = testObsClockModel.getSigmaMultiplier();
        TUASSERT(defaultSigmaMultiplier == initialSigmaMultiplier) // Default sigma multiplier is incorrect

        double expectedSigmaMultiplier = 3.5;
        testObsClockModel.setSigmaMultiplier(expectedSigmaMultiplier);

        double returnedSigmaMultiplier = testObsClockModel.getSigmaMultiplier();

        TUASSERT(expectedSigmaMultiplier == returnedSigmaMultiplier);
    }
    catch (const gnsstk::Exception& e)
    {
        TUFAIL("Exception: " + std::string(e.what()));
    }
    catch (...)
    {
        TUFAIL(unexpectedError);
    }

    TURETURN();
}

int ObsClockModel_T::test_setAndGetElevationMask()
{
    TUDEF("ObsClockModel", "setAndGetElevationMask");
    // Verify that setElevationMask correctly sets the elevation mask angle and setElevationMask correctly retrieves

    try
    {
        ObsClockModel_Child testObsClockModel;

        double defaultElevationMask = 0.0;
        double initialElevationMask = testObsClockModel.getElevationMask();
        TUASSERT(defaultElevationMask == initialElevationMask) // Default elevation mask is incorrect

        double expectedElevationMask = 15.0;
        testObsClockModel.setElevationMask(expectedElevationMask);

        double returnedElevationMask = testObsClockModel.getElevationMask();

        TUASSERT(expectedElevationMask == returnedElevationMask);
    }
    catch (const gnsstk::Exception& e)
    {
        TUFAIL("Exception: " + std::string(e.what()));
    }
    catch (...)
    {
        TUFAIL(unexpectedError);
    }

    TURETURN();
}

int ObsClockModel_T::test_setAndGetUseWonkyData()
{
    TUDEF("ObsClockModel", "setAndGetUseWonkyData");
    // Verify that setUseWonkyData can set useWonky data to false and that getUseWonkyData can retrieve it.
    try
    {
        ObsClockModel_Child testObsClockModel;

        bool defaultUseWonkyData = false;
        bool initialUseWonkyData = testObsClockModel.getUseWonkyData();
        TUASSERT(defaultUseWonkyData == initialUseWonkyData);

        testObsClockModel.setUseWonkyData(true);
        bool returnedUseWonkyData = testObsClockModel.getUseWonkyData();
        TUASSERT(returnedUseWonkyData == true);

        testObsClockModel.setUseWonkyData(false);
        returnedUseWonkyData = testObsClockModel.getUseWonkyData();
        TUASSERT(returnedUseWonkyData == false);
    }
    catch (const gnsstk::Exception& e)
    {
        TUFAIL("Exception: " + std::string(e.what()));
    }
    catch (...)
    {
        TUFAIL(unexpectedError);
    }

    TURETURN();
}

int ObsClockModel_T::test_getSvStatusMapAndGetSvStatus()
{
    TUDEF("ObsClockModel", "getSvStatusMap");
    // Simulate the setting of multiple SV statuses then verifying the retrievd map by getSvStatusMap reflexes that 
    try
    {
        ObsClockModel_Child testObsClockModel;

        ObsClockModel::SvStatusMap initialStatusMap = testObsClockModel.getSvStatusMap();
        TUASSERT(initialStatusMap.empty() == true); // Initial SvStatusMap should be empty

        SatID SV1 = SatID(1, SatelliteSystem::GPS);
        SatID SV2 = SatID(2, SatelliteSystem::GPS);
        SatID SV3 = SatID(3, SatelliteSystem::GPS);
        SatID SV4 = SatID(4, SatelliteSystem::GPS);
        SatID SV5 = SatID(5, SatelliteSystem::GPS);
        SatID SV6 = SatID(6, SatelliteSystem::GPS);

        testObsClockModel.simulateSvStatus(SV1, ObsClockModel::SvStatus::USED);
        testObsClockModel.simulateSvStatus(SV2, ObsClockModel::SvStatus::MANUAL);
        testObsClockModel.simulateSvStatus(SV3, ObsClockModel::SvStatus::SVHEALTH);
        testObsClockModel.simulateSvStatus(SV4, ObsClockModel::SvStatus::ELEVATION);
        testObsClockModel.simulateSvStatus(SV5, ObsClockModel::SvStatus::WONKY);
        testObsClockModel.simulateSvStatus(SV6, ObsClockModel::SvStatus::SIGMA);

        ObsClockModel::SvStatusMap modifiedStatusMap = testObsClockModel.getSvStatusMap();

        TUASSERT(modifiedStatusMap.size() == 6); // SvStatusMap should contain six entries

        TUASSERT(testObsClockModel.getSvStatus(SV1) == ObsClockModel::SvStatus::USED); // SV1 status should be 'USED'
        TUASSERT(testObsClockModel.getSvStatus(SV2) == ObsClockModel::SvStatus::MANUAL); // SV2 status should be 'MANUAL'
        TUASSERT(testObsClockModel.getSvStatus(SV3) == ObsClockModel::SvStatus::SVHEALTH); // SV3 status should be 'SVHEALTH'
        TUASSERT(testObsClockModel.getSvStatus(SV4) == ObsClockModel::SvStatus::ELEVATION); // SV4 status should be 'ELEVATION'
        TUASSERT(testObsClockModel.getSvStatus(SV5) == ObsClockModel::SvStatus::WONKY); // SV5 status should be 'WONKY'
        TUASSERT(testObsClockModel.getSvStatus(SV6) == ObsClockModel::SvStatus::SIGMA); // SV6 status should be 'SIGMA'

    }
    catch (const gnsstk::Exception& e)
    {
        TUFAIL("Exception: " + std::string(e.what()));
    }
    catch (...)
    {
        TUFAIL(unexpectedError);
    }

    TURETURN();
}

int ObsClockModel_T::test_getSvStatusErrorThrow() {
    TUDEF("ObsClockModel", "getSvStatusErrorThrow");
    // Verify that an error is thrown when the SV status of a SV that is not in the map is requested.]

    try {
        ObsClockModel_Child testObsClockModel;

        SatID SV1 = SatID(1, SatelliteSystem::GPS);

        TUTHROW(testObsClockModel.getSvStatus(SV1));

    } catch (...) {

        TUFAIL("Unexpected getSvStatus did not throw error when look for a no existant SV");

    }

    TURETURN();
}

int ObsClockModel_T::test_simpleOrdClock() {
    TUDEF("ObsClockModel", "simpleOrdClock");
    // Test simple clock by generating ORDs for all possible use cases and making sure simple clock sets the correct status and return the correct stats

    try {
        ObsClockModel_Child testObsClockModel;

        testObsClockModel.setElevationMask(10.0);
        testObsClockModel.setSigmaMultiplier(3.0);
        testObsClockModel.setUseWonkyData(false);

        SatID SV1 = SatID(1, SatelliteSystem::GPS);
        SatID SV2 = SatID(2, SatelliteSystem::GPS);
        SatID SV3 = SatID(3, SatelliteSystem::GPS);
        SatID SV4 = SatID(4, SatelliteSystem::GPS);
        SatID SV5 = SatID(5, SatelliteSystem::GPS);
        SatID SV6 = SatID(6, SatelliteSystem::GPS);
        SatID SV7 = SatID(7, SatelliteSystem::GPS);
        SatID SV8 = SatID(8, SatelliteSystem::GPS);
        SatID SV9 = SatID(9, SatelliteSystem::GPS);

        // Three identical ords to make average easy
        gnsstk::ObsRngDev ord1 = this->makeORD(100, 15, 0, 0);
        gnsstk::ObsRngDev ord2 = this->makeORD(102, 15, 0, 0);
        gnsstk::ObsRngDev ord3 = this->makeORD(104, 15, 0, 0);
        testObsClockModel.setSvMode(SV1, ObsClockModel::ALWAYS);
        testObsClockModel.setSvMode(SV2, ObsClockModel::ALWAYS);
        testObsClockModel.setSvMode(SV3, ObsClockModel::ALWAYS);

        // One healthy non-wonky, above elevation threshold set to ignore
        gnsstk::ObsRngDev ord4 = this->makeORD(106, 15, 0, 0);
        testObsClockModel.setSvMode(SV4, ObsClockModel::IGNORE);

        // One set to heathy 
        gnsstk::ObsRngDev ord5 = this->makeORD(106, 15, 0, 0);
        testObsClockModel.setSvMode(SV5, ObsClockModel::HEALTHY);

        // One set to un-heathy
        short s = 0;
        s |= 0x20;
        gnsstk::ObsRngDev ord6 = this->makeORD(110, 15, s, 0);
        testObsClockModel.setSvMode(SV6, ObsClockModel::HEALTHY);

        // One set to wonky
        gnsstk::ObsRngDev ord7 = this->makeORD(112, 15, 0, 1);
        testObsClockModel.setSvMode(SV7, ObsClockModel::ALWAYS);

        // One set to under elevation threshold
        gnsstk::ObsRngDev ord8 = this->makeORD(150, 0, 0, 0);
        testObsClockModel.setSvMode(SV8, ObsClockModel::ALWAYS);

        ORDEpoch oe;
        oe.ords[SV1] = ord1;
        oe.ords[SV2] = ord2;
        oe.ords[SV3] = ord3;
        oe.ords[SV4] = ord4;
        oe.ords[SV5] = ord5;
        oe.ords[SV6] = ord6;
        oe.ords[SV7] = ord7;
        oe.ords[SV8] = ord8;
        Stats<double> result = testObsClockModel.simpleOrdClock(oe);

        double average = result.Average();
        double expectedAverage = -24805702.776356488466;

        TUASSERTFEPS(average, expectedAverage, 1e-7); // Average of ORDs should have the predetermine amount
        TUASSERT(result.N() == 8); // There should only be 8 ords in the data set

        TUASSERT(testObsClockModel.getSvStatus(SV4) == ObsClockModel::SvStatus::MANUAL) // SV4 is set to IGNORE and should have status MANUAL
        TUASSERT(testObsClockModel.getSvStatus(SV5) == ObsClockModel::SvStatus::USED); // SV5 is set to healthy and should have status of USED
        TUASSERT(testObsClockModel.getSvStatus(SV6) == ObsClockModel::SvStatus::SVHEALTH); // SV6 is set to unhealthy and should have status of SVHEALTH
        TUASSERT(testObsClockModel.getSvStatus(SV7) == ObsClockModel::SvStatus::WONKY) // SV7 is set to wonky and should have status of WONKY
        TUASSERT(testObsClockModel.getSvStatus(SV8) == ObsClockModel::SvStatus::ELEVATION) // SV8 is under elevation threshold and should be set to ELEVATION"

        // Trigger sigma ord
        testObsClockModel.setSigmaMultiplier(1.0);
        gnsstk::ObsRngDev ord9 = this->makeORD(120, 15, 0, 0);
        testObsClockModel.setSvMode(SV9, ObsClockModel::ALWAYS);
        oe.ords[SV9] = ord9;

        testObsClockModel.simpleOrdClock(oe);

        TUASSERT(testObsClockModel.getSvStatus(SV9) == ObsClockModel::SvStatus::SIGMA); // Ord value greater than standard deviation should have been marked as SIGMA
    } 
    catch (const gnsstk::Exception& e)
    {
        TUFAIL("Exception: " + std::string(e.what()));
    }
    catch (...)
    {
        TUFAIL(unexpectedError);
    }
    TURETURN();
}


int ObsClockModel_T::test_dump() {
    TUDEF("ObsClockModel", "dump");
    // Verify the dump print out matches the data in the clock

    try {

        ObsClockModel_Child testObsClockModel;
        testObsClockModel.setElevationMask(10.0);
        testObsClockModel.setSigmaMultiplier(3.0);

        SatID SV1 = SatID(1, SatelliteSystem::GPS);
        SatID SV2 = SatID(2, SatelliteSystem::GPS);
        SatID SV3 = SatID(3, SatelliteSystem::GPS);
        SatID SV4 = SatID(4, SatelliteSystem::GPS);
        SatID SV5 = SatID(5, SatelliteSystem::GPS);
        SatID SV6 = SatID(6, SatelliteSystem::GPS);

        testObsClockModel.simulateSvStatus(SV1, ObsClockModel::SvStatus::USED);
        testObsClockModel.simulateSvStatus(SV2, ObsClockModel::SvStatus::MANUAL);
        testObsClockModel.simulateSvStatus(SV3, ObsClockModel::SvStatus::SVHEALTH);
        testObsClockModel.simulateSvStatus(SV4, ObsClockModel::SvStatus::ELEVATION);
        testObsClockModel.simulateSvStatus(SV5, ObsClockModel::SvStatus::WONKY);
        testObsClockModel.simulateSvStatus(SV6, ObsClockModel::SvStatus::SIGMA);    

        std::ostringstream out;

        testObsClockModel.dump(out, 1);

        std::string expected = "min elev:10, max sigma:3, prn/status: GPS 1/0 GPS 2/1 GPS 3/2 GPS 4/3 GPS 5/4 GPS 6/5 ";
        TUASSERT(out.str() == expected); // dump print out should match the set data

    } 
    catch (const gnsstk::Exception& e)
    {
        TUFAIL("Exception: " + std::string(e.what()));
    }
    catch (...)
    {
        TUFAIL(unexpectedError);
    }

    TURETURN();
}

//============================================================
// main() for test application to be executed by CMake/CTest
//============================================================

int main( )
{
   int  errorTotal = 0;

   ObsClockModel_T  testClass;

   errorTotal += testClass.test_constructor();
   errorTotal += testClass.test_constructor_defaults();
   errorTotal += testClass.test_setAndGetSvModeMap();
   errorTotal += testClass.test_setAndGetSvMode_individual();
   errorTotal += testClass.test_setAndGetSvMode_all();
   errorTotal += testClass.test_setAndGetSigmaMultiplier();
   errorTotal += testClass.test_setAndGetElevationMask();
   errorTotal += testClass.test_setAndGetUseWonkyData();
   errorTotal += testClass.test_getSvStatusMapAndGetSvStatus();
   errorTotal += testClass.test_getSvStatusErrorThrow();
   errorTotal += testClass.test_getSvModeErrorThrow();
   errorTotal += testClass.test_simpleOrdClock();
   errorTotal += testClass.test_dump();
   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;

   return( errorTotal );
}

//============================================================
// The End
//============================================================
