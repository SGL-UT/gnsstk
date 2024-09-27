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

#include "SP3NavDataFactory.hpp"
#include "NavDataFactory.hpp"
#include "GlobalTropModel.hpp"
#include "YDSTime.hpp"
#include "NavLibrary.hpp"
#include "PRSolution.hpp"
#include "TestUtil.hpp"
#include "logstream.hpp"

using namespace gnsstk;

class PRSolution_T
{
public:
   PRSolution_T() {};

   unsigned raimComputeUnweightedTest();
   unsigned notEnoughSvsTest();
   unsigned noEphemerisTest();
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

int main()
{
   PRSolution_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.raimComputeUnweightedTest();
   errorTotal += testClass.notEnoughSvsTest();
   errorTotal += testClass.noEphemerisTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
            << std::endl;

   return errorTotal;
}