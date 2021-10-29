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

#include "OrbitEphStore.hpp"
#include "CivilTime.hpp"
#include "TimeString.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

using namespace std;

class OrbitEphStore_T
{
public:
      /** This tests the behavior of OrbitEphStore when the store is
       * empty.  Under normal circumstances the map for a given
       * satellite would not be empty, but that can't be guaranteed as
       * soon as edit() is used. */
   unsigned doFindEphEmptyTests()
   {
      TUDEF("OrbitEphStore","findUserOrbitEph");
      try
      {
         gnsstk::OrbitEphStore store;
            // this eph will be removed by edit()
         gnsstk::OrbitEph baleted;
         gnsstk::SatID sat(11, gnsstk::SatelliteSystem::GPS);
         gnsstk::ObsID obsID(gnsstk::ObservationType::NavMsg,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::CA);
         baleted.dataLoadedFlag = true;
         baleted.satID = sat;
         baleted.obsID = obsID;
         baleted.ctToe = gnsstk::GPSWeekSecond(1917, 576000);
         baleted.ctToc = gnsstk::GPSWeekSecond(1917, 576000);
         baleted.beginValid = baleted.ctToe - 3600;
         baleted.endValid = baleted.ctToe + 3600;

         gnsstk::CommonTime searchTime(baleted.ctToe);

         TUCSM("addEphemeris");
         store.addEphemeris(&baleted);

            // make sure the ephemeris is in the store
         TUCSM("size");
         TUASSERTE(unsigned, 1, store.size());

            // make sure we can find it
         TUCSM("findNearOrbitEph");
         TUASSERT(store.findNearOrbitEph(sat, searchTime) != NULL);

         TUCSM("findUserOrbitEph");
         TUASSERT(store.findUserOrbitEph(sat, searchTime) != NULL);

            // remove the ephemeris
         TUCSM("edit");
         store.edit(baleted.endValid + 604800);

            // make sure the ephemeris has been removed
         TUCSM("size");
         TUASSERTE(unsigned, 0, store.size());

            // make sure we can't find it and don't seg fault
         TUCSM("findNearOrbitEph");
         TUASSERT(store.findNearOrbitEph(sat, searchTime) == NULL);

         TUCSM("findUserOrbitEph");
         TUASSERT(store.findUserOrbitEph(sat, searchTime) == NULL);
      }
      catch (gnsstk::Exception &exc)
      {
         cerr << exc << endl;
         TUFAIL("Unexpected exception");
      }
      catch (...)
      {
         TUFAIL("Unexpected exception");
      }
      TURETURN();
   }


   unsigned basicTests()
   {
      TUDEF("OrbitEphStore","Basic Access Tests");
      try
      {
         gnsstk::OrbitEphStore store;

            // Create a small number of OrbElemBase object with
            // specific characteristics.

         gnsstk::OrbitEph to1;
         gnsstk::SatID sat1(1, gnsstk::SatelliteSystem::GPS);
         gnsstk::ObsID obsID(gnsstk::ObservationType::NavMsg,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::CA);
         to1.dataLoadedFlag = true;
         to1.satID = sat1;
         to1.obsID = obsID;
         to1.ctToe = gnsstk::GPSWeekSecond(2000, 7200);   // 0200
         to1.ctToc = to1.ctToe;
         to1.beginValid = to1.ctToe - 7200;
         to1.endValid = to1.ctToe + 7200;

         gnsstk::OrbitEph to2;
         gnsstk::SatID sat2(32, gnsstk::SatelliteSystem::GPS);
         to2.dataLoadedFlag = true;
         to2.satID = sat2;
         to2.obsID = obsID;
         to2.ctToe = gnsstk::GPSWeekSecond(2000, 79200);    // 2200
         to2.ctToc = to2.ctToe;
         to2.beginValid = to2.ctToe - 7200;
         to2.endValid = to2.ctToe + 7200;

         gnsstk::OrbitEph to3;
         gnsstk::SatID sat3(16, gnsstk::SatelliteSystem::GPS);
         to3.dataLoadedFlag = true;
         to3.satID = sat3;
         to3.obsID = obsID;
         to3.ctToe = gnsstk::GPSWeekSecond(2000, 43200);    // 1200
         to3.ctToc = to3.ctToe;
         to3.beginValid = to3.ctToe - 7200;
         to3.endValid = to3.ctToe + 7200;

         store.addEphemeris(&to1);
         store.addEphemeris(&to2);
         store.addEphemeris(&to3);

            // make sure the ephemeris is in the store
         TUCSM("size");
         TUASSERTE(unsigned, 3, store.size());

         TUCSM("getIndexSet");
         set<gnsstk::SatID> testSet = store.getIndexSet();
         if (testSet.find(sat1)==testSet.end())
         {
            stringstream ss;
            ss << "Did not find expected SV ";
            ss << sat1;
            ss << " in the store.";
            TUFAIL(ss.str());
         }
         if (testSet.find(sat2)==testSet.end())
         {
            stringstream ss;
            ss << "Did not find expected SV ";
            ss << sat2;
            ss << " in the store.";
            TUFAIL(ss.str());
         }
         if (testSet.find(sat3)==testSet.end())
         {
            stringstream ss;
            ss << "Did not find expected SV ";
            ss << sat3;
            ss << " in the store.";
            TUFAIL(ss.str());
         }

         TUCSM("computeXvt");
         gnsstk::Xvt xvt;
         TUCATCH(xvt = store.computeXvt(to1.satID, to1.ctToe));
         TUASSERTE(gnsstk::Xvt::HealthStatus,
                   gnsstk::Xvt::HealthStatus::Healthy, xvt.health);
         TUCATCH(xvt = store.computeXvt(to2.satID, to2.ctToe));
         TUASSERTE(gnsstk::Xvt::HealthStatus,
                   gnsstk::Xvt::HealthStatus::Healthy, xvt.health);
         TUCATCH(xvt = store.computeXvt(to3.satID, to3.ctToe));
         TUASSERTE(gnsstk::Xvt::HealthStatus,
                   gnsstk::Xvt::HealthStatus::Healthy, xvt.health);
         gnsstk::SatID bogus(33, gnsstk::SatelliteSystem::GPS);
         TUCATCH(xvt = store.computeXvt(bogus, to3.ctToe));
         TUASSERTE(gnsstk::Xvt::HealthStatus,
                   gnsstk::Xvt::HealthStatus::Unavailable, xvt.health);

         TUCSM("getSVHealth");
         gnsstk::Xvt::HealthStatus health;
         TUCATCH(health = store.getSVHealth(to1.satID, to1.ctToe));
         TUASSERTE(gnsstk::Xvt::HealthStatus,
                   gnsstk::Xvt::HealthStatus::Healthy, health);
         TUCATCH(health = store.getSVHealth(to2.satID, to2.ctToe));
         TUASSERTE(gnsstk::Xvt::HealthStatus,
                   gnsstk::Xvt::HealthStatus::Healthy, health);
         TUCATCH(health = store.getSVHealth(to3.satID, to3.ctToe));
         TUASSERTE(gnsstk::Xvt::HealthStatus,
                   gnsstk::Xvt::HealthStatus::Healthy, health);
         TUCATCH(health = store.getSVHealth(bogus, to3.ctToe));
         TUASSERTE(gnsstk::Xvt::HealthStatus,
                   gnsstk::Xvt::HealthStatus::Unavailable, health);

         TUCSM("getInitialTime");
         TUASSERTE(gnsstk::CommonTime, to1.beginValid, store.getInitialTime());

         TUCSM("getFinalTime");
         TUASSERTE(gnsstk::CommonTime, to2.endValid, store.getFinalTime());
      }
      catch (gnsstk::Exception &exc)
      {
         cerr << exc << endl;
         TUFAIL("Unexpected exception");
      }
      catch (...)
      {
         TUFAIL("Unexpected exception");
      }
      TURETURN();
   }
};


int main(int argc, char *argv[])
{
   unsigned total = 0;
   OrbitEphStore_T testClass;
   total += testClass.doFindEphEmptyTests();
   total += testClass.basicTests();

   cout << "Total Failures for " << __FILE__ << ": " << total << endl;
   return total;
}
