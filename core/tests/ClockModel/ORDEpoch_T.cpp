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

#include "ORDEpoch.hpp"
#include <iostream>
#include <iomanip>

#include "EpochClockModel.hpp"
#include "ObsRngDev.hpp"
#include "TestUtil.hpp"
#include "NavLibrary.hpp"
#include "RinexNavDataFactory.hpp"
#include "EphemerisRange.hpp"
#include "SimpleTropModel.hpp"

using namespace std;
using namespace gnsstk;

class ORDEpoch_T
{
        public:
	   ORDEpoch_T(){}// Default Constructor, set the precision value
           ~ORDEpoch_T() {} // Default Desructor
            int test_removeORD();
            int test_applyClockModel();
            int test_removeOffset();

            gnsstk::ObsRngDev makeORD(const double prange, const float elevation, const short health, const unsigned wonky) {

                gnsstk::NavLibrary ephemStore;
                gnsstk::WGS84Ellipsoid em;
                gnsstk::NavDataFactoryPtr ndfp;
                gnsstk::Position receiverPos;
                gnsstk::CommonTime obsTime = gnsstk::CivilTime(2006, 1, 31, 2, 0, 0, gnsstk::TimeSystem::GPS);
                SatID SV = SatID(1, SatelliteSystem::GPS);

                receiverPos.setGeodetic(30.387577, -97.727607, 240);
                std::string path = gnsstk::getPathData() + gnsstk::getFileSep() + "test_input_rinex_nav_ephemerisData.031";
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


int ORDEpoch_T::test_removeORD()
{
   TUDEF("ORDEpoch", "removeORD");
   // Verify that removeORD correctly removes ord from ords 

   try
   {
      // Initialize ORDEpoch and one ObsRngDev
      ORDEpoch oe;
      gnsstk::ObsRngDev ord1 = this->makeORD(100, 15, 0, 0);
      SatID SV1 = SatID(1, SatelliteSystem::GPS);
      oe.ords[SV1] = ord1;

      // Function call
      oe.removeORD(SV1);
      TUASSERT(oe.ords.size() == 0);
   }
   catch(const gnsstk::Exception& e)
   {
      TUFAIL("Exception: " + std::string(e.what()));
   }
   catch(...)
   {
      TUFAIL("An unexpected error occurred.");
   }

   TURETURN();
}

int ORDEpoch_T::test_applyClockModel() // Also covers removeOffset()
{
   TUDEF("ORDEpoch", "applyClockModel");

   try
   {
      // Initialize an ObsRngDev and a ClockModel
      EpochClockModel cm;
      ORDEpoch oe;
      gnsstk::ObsRngDev ord1 = this->makeORD(100, 15, 0, 0);
      gnsstk::ObsRngDev ord2 = this->makeORD(120, 15, 0, 0);
      gnsstk::ObsRngDev ord3 = this->makeORD(140, 15, 0, 0);

      SatID SV1 = SatID(1, SatelliteSystem::GPS);
      SatID SV2 = SatID(2, SatelliteSystem::GPS);
      SatID SV3 = SatID(3, SatelliteSystem::GPS);

      oe.ords[SV1] = ord1;
      oe.ords[SV2] = ord2;
      oe.ords[SV3] = ord3;

      // isOffsetValid() must return true
      cm.addEpoch(oe);

      // Obtain the old ORD values
      vector<double> oldORDs;
      map<SatID, ObsRngDev>::iterator it;
      for(it = oe.ords.begin(); it != oe.ords.end(); it++)
      {
         oldORDs.push_back((*it).second.getORD());
      }

      // Function call
      oe.applyClockModel(cm);

      // Verify each new ord in the map
      vector<double>::iterator it_old = oldORDs.begin();
      for(it = oe.ords.begin(); it != oe.ords.end(); it++)
      {
         double ordExpected = *it_old - cm.getOffset(oe.time);
         double ordReturned = (*it).second.getORD();
         TUASSERTFEPS(ordReturned, ordExpected, 1e-7);
         it_old++;
      }
   }
   catch(const gnsstk::Exception& e)
   {
      TUFAIL("Exception: " + std::string(e.what()));
   }
   catch(...)
   {
      TUFAIL("An unexpected error occurred.");
   }

   TURETURN();
}

int main() //Main function to initialize and run all tests above
{
   int errorTotal = 0;

   ORDEpoch_T testClass;
   
   errorTotal += testClass.test_removeORD();
   errorTotal += testClass.test_applyClockModel();

   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl; 
   return errorTotal;
}
