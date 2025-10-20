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

#include "CommonTime.hpp"
#include "SP3NavDataFactory.hpp"
#include "NavDataFactory.hpp"
#include "GlobalTropModel.hpp"
#include "YDSTime.hpp"
#include "NavLibrary.hpp"
#include "PRSolution.hpp"
#include "TestUtil.hpp"
#include "logstream.hpp"
#include "Position.hpp"

using namespace gnsstk;

class PRSolution_T
{
public:
   PRSolution_T() {};

   unsigned raimComputeUnweightedTest();
   unsigned notEnoughSvsTest();
   unsigned noEphemerisTest();
   unsigned wtdAveStatsSetAndGetMessage();
   unsigned wtdAveStatsSetLabel();
   unsigned wtdAveStatsGetSol();
   unsigned wtdAveStatsEmptyDump();
   unsigned prSolutionOutputStringHeader();
   unsigned prSolutionFixAPSolution();
   unsigned prSolutionGetAPV();
   unsigned prSolutionDumpSolution();
   unsigned prSolutionUpdateAPSolutionEdgeCases();
   unsigned prSolutionOSStream();
   unsigned prSolutionOutputPOSString();
   unsigned prSolutionOutputCLKStringEmpty();
   unsigned prSolutionOutputCLKString();
   unsigned prSolutionErrorCodeString();
   unsigned prSolutionConfigString();
   unsigned tropExceptionTest();
};


unsigned PRSolution_T ::
raimComputeUnweightedTest()
{
      // A simple workflow test that exercises the typical usage of PRSolution.
   TUDEF("PRSolution", "RAIMComputeUnweighted");

      // There's a lot of logging code in PRSolution. Turning on all logging
      // at least to check that it doesn't break anything during this test.
   ConfigureLOG::Stream() = &std::cerr;
   ConfigureLOG::ReportingLevel() = ConfigureLOG::Level("DEBUG");

      // Load some example ephemeris data
   std::string navfile = gnsstk::getPathData() + gnsstk::getFileSep() + "test_input_sp3_nav_2015_200.sp3";
   std::shared_ptr<NavDataFactory> navFactory = std::make_shared<SP3NavDataFactory>();
   navFactory->addDataSource(navfile);
   NavLibrary navLib;
   navLib.addFactory(navFactory);

      // PRSolution needs a trop model. Using Global for simplicity.
   GlobalTropModel tropModel;
   
   YDSTime time(2015, 200, 46800);
   time.setTimeSystem(TimeSystem::GPS);
   std::vector<SatID> sats{
      SatID(3, SatelliteSystem::GPS),
      SatID(7, SatelliteSystem::GPS),
      SatID(9, SatelliteSystem::GPS),
      SatID(10, SatelliteSystem::GPS),
      SatID(16, SatelliteSystem::GPS),
      SatID(19, SatelliteSystem::GPS),
      SatID(23, SatelliteSystem::GPS),
      SatID(26, SatelliteSystem::GPS),
      SatID(27, SatelliteSystem::GPS),
      SatID(31, SatelliteSystem::GPS),
      SatID(32, SatelliteSystem::GPS)
   };

       // Ranges to the satellite with some artificial error.
       // Ranges determined based on observing position in Leander, Texas.
       // The order of these pseudoranges match the order of the sats vector.
   std::vector<double> pseudoranges{
      22148868.974 + 15,
      23873407.930 + 7,
      22137165.177 + 2,
      24436197.549 + 8,
      20652840.526 + 1,
      23940601.009 + 12,
      20368218.660 + 15,
      22696632.800 + 4,
      22363097.651 + 14,
      25136058.042 + 5,
      24250681.418 + 7
   };

   PRSolution solver;
       // Shouldn't be valid, nothing has happened yet.
   TUASSERT(!solver.isValid());

      // It will throw an exception until PRSolution::allowedGNSS has been set.
   TUTHROW(solver.RAIMComputeUnweighted(
      time,
      sats,
      pseudoranges,
      navLib,
      &tropModel
   ));
   solver.allowedGNSS.push_back(SatelliteSystem::GPS);
   solver.allowedGNSS.push_back(SatelliteSystem::Glonass);

      // It will throw an exception if there's no trop model.
      // If you need a trop model that is effectively no model at all
      // then use gnsstk::ZeroTropModel
   TUTHROW(solver.RAIMComputeUnweighted(
      time,
      sats,
      pseudoranges,
      navLib,
      nullptr
   ));

      // Should converge on a decent solution.
      // Some of the obs is expected to be rejected.
      // A 10 meter epsilon allows for variation if the algorithm
      // changes slightly. This test is intended to catch large changes.
   int rc = solver.RAIMComputeUnweighted(
      time,
      sats,
      pseudoranges,
      navLib,
      &tropModel
   );
   TUASSERTE(int, 0, rc);
   TUASSERT(solver.isValid());
   TUASSERT(sats.size() > solver.Nsvs);
   TUASSERTFEPS(solver.Solution[0], -751000.0, 10);
   TUASSERTFEPS(solver.Solution[1], -5444353.0, 10);
   TUASSERTFEPS(solver.Solution[2], 3225860, 10);
   TUASSERTFEPS(solver.Solution[3], 68.0, 10);
      // Since PRSolution::hasMemory == true by default then the
      // APSolution field should be automtically populated for the next run.
   TUASSERTFEPS(solver.APSolution[0], -751000.0, 10);
   TUASSERTFEPS(solver.APSolution[1], -5444353.0, 10);
   TUASSERTFEPS(solver.APSolution[2], 3225860, 10);
   TUASSERTFEPS(solver.APSolution[3], 68.0, 10);

      // Advance the time by 10 seconds and rerun the solver with the same observation data.
      // Effectively this is equivalent to having bad observation data at a particular epoch.
      // The solver should complete but return as a "degraded" solution.
   time = YDSTime(2015, 200, 46800+10);
   time.setTimeSystem(TimeSystem::GPS);
   rc = solver.RAIMComputeUnweighted(
      time,
      sats,
      pseudoranges,
      navLib,
      &tropModel
   );
   TUASSERTE(int, 1, rc);
   TUASSERT(solver.isValid());
   TUASSERT(sats.size() > solver.Nsvs);
   
   TURETURN();
}

unsigned PRSolution_T ::
notEnoughSvsTest()
{
      // A simple workflow test that exercises the typical usage of PRSolution.
      // PRSolution requires at least 4 SVs (sometimes more) of data. If there
      // is not enough data then it should return with an error.
   TUDEF("PRSolution", "RAIMComputeUnweighted");

   std::string navfile = gnsstk::getPathData() + gnsstk::getFileSep() + "test_input_sp3_nav_2015_200.sp3";
   std::shared_ptr<NavDataFactory> navFactory = std::make_shared<SP3NavDataFactory>();
   navFactory->addDataSource(navfile);

   NavLibrary navLib;
   navLib.addFactory(navFactory);

   GlobalTropModel tropModel;
   
   YDSTime time(2015, 200, 46800);
   time.setTimeSystem(TimeSystem::GPS);
   std::vector<SatID> sats{
      SatID(3, SatelliteSystem::GPS),
      SatID(7, SatelliteSystem::GPS),
      SatID(9, SatelliteSystem::GPS)
   };

       // Ranges to the satellite with some artificial error.
       // Ranges determined based on observing position in Leander, Texas.
       // The order of these pseudoranges match the order of the sats vector.
   std::vector<double> pseudoranges{
      22148868.974 + 15,
      23873407.930 + 7,
      22137165.177 + 2
   };

   PRSolution solver;
       // Shouldn't be valid, nothing has happened yet.
   TUASSERT(!solver.isValid());
   solver.allowedGNSS.push_back(SatelliteSystem::GPS);

   int rc = solver.RAIMComputeUnweighted(
      time,
      sats,
      pseudoranges,
      navLib,
      &tropModel
   );

   TUASSERTE(int, -3, rc);
   TUASSERT(!solver.isValid());

   TURETURN();
}

unsigned PRSolution_T ::
noEphemerisTest()
{
      // A simple workflow test that exercises the typical usage of PRSolution.
      // PRSolution ephemeris data for the SVs. If there is not enough ephemeris
      // data then it should return with an error.
   TUDEF("PRSolution", "RAIMComputeUnweighted");

   NavLibrary navLib;
   GlobalTropModel tropModel;
   
   YDSTime time(2015, 200, 46800);
   time.setTimeSystem(TimeSystem::GPS);
   std::vector<SatID> sats{
      SatID(3, SatelliteSystem::GPS),
      SatID(7, SatelliteSystem::GPS),
      SatID(9, SatelliteSystem::GPS),
      SatID(10, SatelliteSystem::GPS),
      SatID(16, SatelliteSystem::GPS),
      SatID(19, SatelliteSystem::GPS),
      SatID(23, SatelliteSystem::GPS),
      SatID(26, SatelliteSystem::GPS),
      SatID(27, SatelliteSystem::GPS),
      SatID(31, SatelliteSystem::GPS),
      SatID(32, SatelliteSystem::GPS)
   };

       // Ranges to the satellite with some artificial error.
       // Ranges determined based on observing position in Leander, Texas.
       // The order of these pseudoranges match the order of the sats vector.
   std::vector<double> pseudoranges{
      22148868.974 + 15,
      23873407.930 + 7,
      22137165.177 + 2,
      24436197.549 + 8,
      20652840.526 + 1,
      23940601.009 + 12,
      20368218.660 + 15,
      22696632.800 + 4,
      22363097.651 + 14,
      25136058.042 + 5,
      24250681.418 + 7
   };

   PRSolution solver;
       // Shouldn't be valid, nothing has happened yet.
   TUASSERT(!solver.isValid());
   solver.allowedGNSS.push_back(SatelliteSystem::GPS);

   int rc = solver.RAIMComputeUnweighted(
      time,
      sats,
      pseudoranges,
      navLib,
      &tropModel
   );

   TUASSERTE(int, -4, rc);
   TUASSERT(!solver.isValid());

   TURETURN();
}

// WtdAveStats UNIT Tests

unsigned PRSolution_T::wtdAveStatsSetAndGetMessage()
{
   TUDEF("PRSolution", "wtdAveStatsSetMessage");

   gnsstk::WtdAveStats testAveStats;
   std::string testString = "test string";
   testAveStats.setMessage(testString);

   TUASSERTE(std::string, testString, testAveStats.getMessage());

   TURETURN();

}

unsigned PRSolution_T::wtdAveStatsSetLabel()
{
   TUDEF("PRSolution", "wtdAveStatsSetLabel");

   gnsstk::WtdAveStats testAveStats;

   // Add some data so dump as something to dump
   gnsstk::Vector<double> vector(3);
   vector[0] = 1.1;
   vector[1] = 2.2;
   vector[2] = 3.3;

   gnsstk::Matrix<double> matrix(3, 3);
   matrix(0, 0) = 1.0; matrix(0, 1) = 2.0; matrix(0, 2) = 3.0;
   matrix(1, 0) = 4.0; matrix(1, 1) = 5.0; matrix(1, 2) = 6.0;
   matrix(2, 0) = 7.0; matrix(2, 1) = 8.0; matrix(2, 2) = 9.0;

   testAveStats.add(vector, matrix);

   // Modify labels
   std::string testStringOne = "test string one";
   std::string testStringTwo = "test string two";
   std::string testStringThree = "test string three";
   testAveStats.setLabels(testStringOne, testStringTwo, testStringThree);

   // Get dump output
   std::ostringstream oss;
   testAveStats.dump(oss);

   std::string expectedOutput = "Simple statistics on \n"
                               "  test string one N: 1 Ave: 1.1000 Std: 0.0000 Min: 1.1000 Max: 1.1000\n"
                               "  test string two N: 1 Ave: 2.2000 Std: 0.0000 Min: 2.2000 Max: 2.2000\n"
                               "  test string three N: 1 Ave: 3.3000 Std: 0.0000 Min: 3.3000 Max: 3.3000\n"
                               "Weighted average \n"
                               "         1.1000         2.2000         3.3000    1";

   TUASSERTE(std::string, expectedOutput, oss.str());

   TURETURN();
}

unsigned PRSolution_T::wtdAveStatsGetSol()
{
   TUDEF("PRSolution", "wtdAveStatsGetSol");

   gnsstk::WtdAveStats testAveStats;

   // Add some data so dump as something to dump
   gnsstk::Vector<double> vector(3);
   vector[0] = 1.1;
   vector[1] = 2.2;
   vector[2] = 3.3;

   gnsstk::Matrix<double> matrix(3, 3);
   matrix(0, 0) = 1.0; matrix(0, 1) = 2.0; matrix(0, 2) = 3.0;
   matrix(1, 0) = 4.0; matrix(1, 1) = 5.0; matrix(1, 2) = 6.0;
   matrix(2, 0) = 7.0; matrix(2, 1) = 8.0; matrix(2, 2) = 9.0;

   testAveStats.add(vector, matrix);

   // Generate Solution
   gnsstk::Vector<double> solutionVector = testAveStats.getSol();

   TUASSERTE(double, 1.1000, solutionVector[0]);
   TUASSERTE(double, 2.2000, solutionVector[1]);
   TUASSERTE(double, 3.3000, solutionVector[2]);

   TURETURN();
}

unsigned PRSolution_T::wtdAveStatsEmptyDump()
{
   TUDEF("PRSolution", "wtdAveStatsEmptyDump");
   
   gnsstk::WtdAveStats testAveStats;
   testAveStats.reset();
   std::ostringstream oss;
   testAveStats.dump(oss);

   std::string expectedOutput = "Simple statistics on \n"
                                " No data!";
   
   TUASSERTE(std::string, expectedOutput, oss.str());

   TURETURN();
}

// PRSolution UNIT Tests

unsigned PRSolution_T::prSolutionOutputStringHeader()
{
   TUDEF("PRSolution", "prSolutionOutputStringHeader");

   PRSolution testSolver;
   std::string testTag = "test tag";
   std::string header = testSolver.outputStringHeader(testTag);

   std::string expectedHeader = "#test tag NAV       time     Sol/Resid:X(m)     Sol/Resid:Y(m)     Sol/Resid:Z(m)          sys clock [sys clock ...]   Valid/Not\n"
                                "#test tag RMS    time Ngood    resid    TDOP    PDOP    GDOP Slope nit converge sats(-rej)... (ret code) Valid/Not";
   
   TUASSERTE(std::string, expectedHeader, header);

   TURETURN();
}

unsigned PRSolution_T::prSolutionFixAPSolution()
{
   TUDEF("PRSolution", "prSolutionFixAPSolution");

   PRSolution testSolver;

   double testX = 1;
   double testY = 2;
   double testZ = 3;

   testSolver.hasMemory = true;
   testSolver.fixAPSolution(testX, testY, testZ);

   gnsstk::Vector<double> testAPSolution = testSolver.APSolution;

   TUASSERTE(double, 1, testAPSolution[0]);
   TUASSERTE(double, 2, testAPSolution[1]);
   TUASSERTE(double, 3, testAPSolution[2]);

   TURETURN();
}

unsigned PRSolution_T::prSolutionGetAPV()
{
   TUDEF("PRSolution", "prSolutionGetAPV");

   PRSolution testSolver;

   TUASSERTE(double, 0.0, testSolver.getAPV());

   testSolver.APV = 2;
   testSolver.ndof = 1;

   TUASSERTE(double, 2, testSolver.getAPV());

   TURETURN();
}


unsigned PRSolution_T::prSolutionDumpSolution()
{
   TUDEF("PRSolution", "prSolutionDumpSolution");

   PRSolution testSolver;

   gnsstk::WtdAveStats testAveStats;

   // Add some data so dump as something to dump
   gnsstk::Vector<double> vector(3);
   vector[0] = 1;
   vector[1] = 1;
   vector[2] = 1;

   gnsstk::Matrix<double> matrix(3, 3);
   matrix(0, 0) = 1; matrix(0, 1) = 1; matrix(0, 2) = 1;
   matrix(1, 0) = 1; matrix(1, 1) = 1; matrix(1, 2) = 1;
   matrix(2, 0) = 1; matrix(2, 1) = 1; matrix(2, 2) = 1;

   testAveStats.add(vector, matrix);

   testSolver.was = testAveStats;

   std::ostringstream oss;
   testSolver.dumpSolution(oss);

   std::string expectedOutput = "Simple statistics on PRS\n"
                                "  ECEF_X N: 1 Ave: 1.0000 Std: 0.0000 Min: 1.0000 Max: 1.0000\n"
                                "  ECEF_Y N: 1 Ave: 1.0000 Std: 0.0000 Min: 1.0000 Max: 1.0000\n"
                                "  ECEF_Z N: 1 Ave: 1.0000 Std: 0.0000 Min: 1.0000 Max: 1.0000\n"
                                "Weighted average PRS\n"
                                "         1.0000         1.0000         1.0000    1\n"
                                "Covariance: PRS\n"
                                "                        ECEF_X         ECEF_Y         ECEF_Z\n"
                                "         ECEF_X      1.000e+00\n"
                                "         ECEF_Y      1.000e+00      1.000e+00\n"
                                "         ECEF_Z      1.000e+00      1.000e+00      1.000e+00\n"
                                "APV: PRS sigma = 0.000 meters with 0 degrees of freedom.\n";
   
   TUASSERTE(std::string, expectedOutput, oss.str());

   TURETURN();
}


unsigned PRSolution_T::prSolutionUpdateAPSolutionEdgeCases()
{
   TUDEF("PRSolution", "prSolutionUpdateAPSolutionEdgeCases");

   gnsstk::Vector<double> vector(3);
   vector[0] = 1.1;
   vector[1] = 2.2;
   vector[2] = 3.3;

   PRSolution testSolver;

   testSolver.APSolution.resize(0);

   testSolver.updateAPSolution(vector);

   gnsstk::Vector<double> apSolution =testSolver.APSolution;

   TUASSERTE(double, 1.1, apSolution[0]);
   TUASSERTE(double, 2.2, apSolution[1]);
   TUASSERTE(double, 3.3, apSolution[2]);

   TURETURN();
}

unsigned PRSolution_T::prSolutionOSStream()
{
   TUDEF("PRSolution", "prSolutionOSStream");

   WtdAveStats was;

   std::stringstream ss;
   ss << was;

   TUASSERTE(std::string, "Simple statistics on \n No data!", ss.str());

   TURETURN();
}

unsigned PRSolution_T::prSolutionOutputPOSString()
{
   TUDEF("PRSolution", "prSolutionOutputPOSString");

   PRSolution testSolver;

   std::string testString = "test string";
   int iretFlag = -999;
   gnsstk::Vector<double> vector(3);
   vector[0] = 1.1;
   vector[1] = 2.2;
   vector[2] = 3.3;

   std::string output = testSolver.outputPOSString(testString, iretFlag, vector);
   std::string expectedOutput = "#test string POS    time         Sol-X(m)         Sol-Y(m)         Sol-Z(m) (ret code) Valid/Not";

   TUASSERTE(std::string, expectedOutput, output);

   TURETURN();
}

unsigned PRSolution_T::prSolutionOutputCLKStringEmpty()
{
   TUDEF("PRSolution", "prSolutionOutputCLKStringEmpty");

   PRSolution testSolver;

   std::string testString = "test string";
   int iretFlag = -999;

   std::string output = testSolver.outputCLKString(testString, iretFlag);
   std::string expectedOutput = "#test string CLK    time sys       clock ...";
   TUASSERTE(std::string, expectedOutput, output);

   TURETURN();
}

unsigned PRSolution_T::prSolutionOutputCLKString()
{
   TUDEF("PRSolution", "prSolutionOutputCLKString");

   PRSolution testSolver;

   std::string testString = "test string";
   int iretFlag = 0;

   std::string output = testSolver.outputCLKString(testString, iretFlag);
   std::string expectedOutput = "test string CLK %4F %10.3g (0 ok) NV";
   TUASSERTE(std::string, expectedOutput, output);

   TURETURN();
}

unsigned PRSolution_T::prSolutionErrorCodeString()
{
   TUDEF("PRSolution", "prSolutionErrorCodeString");

   PRSolution testSolver;

   TUASSERTE(std::string, "unknown", testSolver.errorCodeString(777));
   TUASSERTE(std::string, "ok but perhaps degraded", testSolver.errorCodeString(1));
   TUASSERTE(std::string, "ok", testSolver.errorCodeString(0));
   TUASSERTE(std::string, "failed to converge", testSolver.errorCodeString(-1));
   TUASSERTE(std::string, "singular solution", testSolver.errorCodeString(-2));
   TUASSERTE(std::string, "not enough satellites", testSolver.errorCodeString(-3));
   TUASSERTE(std::string, "not any ephemeris", testSolver.errorCodeString(-4));

   TURETURN();
}

unsigned PRSolution_T::prSolutionConfigString()
{
   TUDEF("PRSolution", "prSolutionConfigString");

   PRSolution testSolver;

   std::string testTag = "test tag";
   std::string configString = testSolver.configString(testTag);

   std::string expectedConfigString = "test tag\n"
                                      "   iterations 10\n"
                                      "   convergence 3.00e-07\n"
                                      "   RMS residual limit 6.50\n"
                                      "   RAIM slope limit 1000.00 meters\n"
                                      "   Maximum number of satellites to reject is -1\n"
                                      "   Memory information IS stored";
   
   TUASSERTE(std::string, expectedConfigString, configString);

   TURETURN();
}


   /* PRSolution should catch exceptions from GlobalTropModel regardless
    * if the receiver height is below the trop model height limit.
    *
    * GlobalTropModel's height limit is not a hard limit and can actually
    * fluctuate a few meters depending on the receiver's latitude and longitude.
    *
    * PRSolution used to only check the receiver position against the "constant"
    * height limit before computing the trop correction. This would occasionally
    * fail when the actual computed height limit is lower than initial "constant"
    * height limit causing an exception to bubble up through PRSolution.
    *
    * This test ensures that PRSolution can gracefully continue despite
    * the edge case of GlobalTropModel.
    */
unsigned PRSolution_T ::
tropExceptionTest()
{
   TUDEF("PRSolution", "computeTropDelay");
   gnsstk::GlobalTropModel tropModel{};
   double earth_equitorial_radius = 6378137;
   double trop_height = 44243;
      // Create a dummy RX position that is just under
      // GlobalTropModel's "constant" height limit.
   gnsstk::Position rxPosition{1, earth_equitorial_radius + trop_height - 1, 1};
   TUASSERT(rxPosition.getHeight() < tropModel.getHeightLimit());
      // Dummy SV position ensure's > 3 degree elevation 
      // so that trop model is executed
   gnsstk::Position svPosition{17308210, 20473271, 0.0};
      // Time doesn't matter 
   gnsstk::CommonTime time{};

   gnsstk::PRSolution prs{};
   TUCATCH(prs.computeTropDelay(svPosition, rxPosition, &tropModel, time));
   TURETURN();
}


int main()
{
   PRSolution_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.raimComputeUnweightedTest();
   errorTotal += testClass.notEnoughSvsTest();
   errorTotal += testClass.noEphemerisTest();
   errorTotal += testClass.wtdAveStatsSetAndGetMessage();
   errorTotal += testClass.wtdAveStatsSetLabel();
   errorTotal += testClass.wtdAveStatsGetSol();
   errorTotal += testClass.wtdAveStatsEmptyDump();
   errorTotal += testClass.prSolutionOutputStringHeader();
   errorTotal += testClass.prSolutionFixAPSolution();
   errorTotal += testClass.prSolutionGetAPV();
   errorTotal += testClass.prSolutionDumpSolution();
   errorTotal += testClass.prSolutionUpdateAPSolutionEdgeCases();
   errorTotal += testClass.prSolutionOSStream();
   errorTotal += testClass.prSolutionOutputPOSString();
   errorTotal += testClass.prSolutionOutputCLKStringEmpty();
   errorTotal += testClass.prSolutionOutputCLKString();
   errorTotal += testClass.prSolutionErrorCodeString();
   errorTotal += testClass.prSolutionConfigString();
   errorTotal += testClass.tropExceptionTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;

   return errorTotal;
}

