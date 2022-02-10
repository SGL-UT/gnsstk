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

#include "SatelliteSystem.hpp"
#include "SatID.hpp"

#include "TestUtil.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <map>

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, const SatelliteSystem sys)
   {
      s << gnsstk::StringUtils::asString(sys);
      return s;
   }
}

class SatID_T
{
public:
   SatID_T() {} // Default Constructor, set the precision value
   ~SatID_T() {} // Default Desructor


      /// ensure the constructors set the values properly
   unsigned initializationTest()
   {
      TUDEF("SatID", "Constructor");

      gnsstk::SatID compare1(5, gnsstk::SatelliteSystem (1));
      TUASSERTE(int, 5, compare1.id);
      TUASSERTE(gnsstk::SatelliteSystem,
                gnsstk::SatelliteSystem(1),
                compare1.system);

      gnsstk::SatID compare2(0, gnsstk::SatelliteSystem (12));
      TUASSERTE(int, 0, compare2.id);
      TUASSERTE(gnsstk::SatelliteSystem,
                gnsstk::SatelliteSystem(12),
                compare2.system);

      gnsstk::SatID compare3(-1, gnsstk::SatelliteSystem (-1));
      TUASSERTE(int, -1, compare3.id);
      TUASSERTE(gnsstk::SatelliteSystem,
                gnsstk::SatelliteSystem(-1),
                compare3.system);

      gnsstk::SatID ws1;
      TUASSERTE(bool, false, ws1.wildId);
      TUASSERTE(bool, false, ws1.wildSys);
      gnsstk::SatID ws2(99);
      TUASSERTE(int, 99, ws2.id);
      TUASSERTE(bool, false, ws2.wildId);
      TUASSERTE(bool, true, ws2.wildSys);
      gnsstk::SatID ws3(gnsstk::SatelliteSystem::Mixed);
      TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::Mixed,
                ws3.system);
      TUASSERTE(bool, true, ws3.wildId);
      TUASSERTE(bool, false, ws3.wildSys);
      gnsstk::SatID ws4(-65, gnsstk::SatelliteSystem::LEO);
      TUASSERTE(int, -65, ws4.id);
      TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::LEO,
                ws4.system);
      TUASSERTE(bool, false, ws4.wildId);
      TUASSERTE(bool, false, ws4.wildSys);
      gnsstk::SatID foo(25, gnsstk::SatelliteSystem::QZSS);
      gnsstk::SatID ws5(foo);
      TUASSERTE(int, 25, ws5.id);
      TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::QZSS,
                ws5.system);
      TUASSERTE(bool, false, ws5.wildId);
      TUASSERTE(bool, false, ws5.wildSys);

      TURETURN();
   }


      /// check the output from SatID::dump meets its expectations
   unsigned dumpTest()
   {
      TUDEF("SatID", "dump(std::stream)");

         //---------------------------------------------------------------------
         //Output for GPS satellite and single digit ID
         //---------------------------------------------------------------------
      gnsstk::SatID sat1(5, gnsstk::SatelliteSystem (1));
      std::string outputString1, compareString1;
      std::stringstream outputStream1;

      sat1.dump(outputStream1);
      outputString1 = outputStream1.str();
      compareString1 = "GPS 5";
      TUASSERTE(std::string, compareString1, outputString1);

         //---------------------------------------------------------------------
         //Output for invalid UserDefined satellite and triple digit ID
         //---------------------------------------------------------------------
      gnsstk::SatID sat2(110, gnsstk::SatelliteSystem (11));
      std::string outputString2, compareString2;
      std::stringstream outputStream2;

      sat2.dump(outputStream2);
      outputString2 = outputStream2.str();
      compareString2 = "UserDefined 110";
      TUASSERTE(std::string, compareString2, outputString2);

         //---------------------------------------------------------------------
         //Output for invalid satellite and negative ID
         //---------------------------------------------------------------------
      gnsstk::SatID sat3(-10, gnsstk::SatelliteSystem (50));
      std::string outputString3, compareString3;
      std::stringstream outputStream3;

      sat3.dump(outputStream3);
      outputString3 = outputStream3.str();
      compareString3 = "??? -10";
      TUASSERTE(std::string, compareString3, outputString3);

      TURETURN();

   }


      /// check that a SatID object can be reported as a string
   unsigned asStringTest()
   {
      TUDEF("SatID", "asStringTest");

      std::string compareString1,compareString2,compareString3;

         //---------------------------------------------------------------------
         //Output for GPS satellite and single digit ID
         //---------------------------------------------------------------------
      gnsstk::SatID sat1(5, gnsstk::SatelliteSystem (1));
      compareString1 = "GPS 5";
      TUASSERTE(std::string, compareString1,gnsstk::StringUtils::asString(sat1));

         //---------------------------------------------------------------------
         //Output for invalid UserDefined satellite and triple digit ID
         //---------------------------------------------------------------------
      gnsstk::SatID sat2(110, gnsstk::SatelliteSystem (11));
      compareString2 = "UserDefined 110";
      TUASSERTE(std::string, compareString2,gnsstk::StringUtils::asString(sat2));

         //---------------------------------------------------------------------
         //Output for invalid satellite and negative ID
         //---------------------------------------------------------------------
      gnsstk::SatID sat3(-10, gnsstk::SatelliteSystem (50));
      compareString3 = "??? -10";
      TUASSERTE(std::string, compareString3,gnsstk::StringUtils::asString(sat3));

      TURETURN();
   }


      /// verify the various operators of the SatID class
   unsigned operatorTest()
   {
      TUDEF("SatID", "OperatorEquivalence");

      gnsstk::SatID compare    (5, gnsstk::SatelliteSystem(2));
      gnsstk::SatID equivalent (5, gnsstk::SatelliteSystem(2));
      gnsstk::SatID lessThanID (2, gnsstk::SatelliteSystem(2));
      gnsstk::SatID diffSatSys (5, gnsstk::SatelliteSystem(5));
      gnsstk::SatID diffEvery  (2, gnsstk::SatelliteSystem(5));
      gnsstk::SatID diffEvery2 (7, gnsstk::SatelliteSystem(1));
      gnsstk::SatID redirected (6, gnsstk::SatelliteSystem(1));

         //---------------------------------------------------------------------
         //Does the == Operator function?
         //---------------------------------------------------------------------
      TUASSERT( compare == equivalent);
      TUASSERT(!(compare == lessThanID));
      TUASSERT(!(compare == diffSatSys));

      TUCSM("operator!=");
         //---------------------------------------------------------------------
         //Does the != Operator function?
         //---------------------------------------------------------------------
      TUASSERT(!(compare != equivalent));
      TUASSERT( compare != lessThanID);
      TUASSERT( compare != diffSatSys);

      TUCSM("operator<");
         //---------------------------------------------------------------------
         //Does the < Operator function?
         //---------------------------------------------------------------------

         //ID only comparisons
      TUASSERT(!(compare < lessThanID));
      TUASSERT( lessThanID < compare);
      TUASSERT(!(compare < equivalent));

         //SatelliteSystem only comparisons
      TUASSERT( compare < diffSatSys);
      TUASSERT(!(diffSatSys < compare));

         //Completely different comparisons
      TUASSERT( compare < diffEvery);
      TUASSERT(!(diffEvery < compare));
      TUASSERT(!(compare < diffEvery2));
      TUASSERT( diffEvery2 < compare);

      TUCSM("operator>");
         //---------------------------------------------------------------------
         //Does the > Operator function?
         //---------------------------------------------------------------------

         //ID only comparisons
      TUASSERT((compare > lessThanID));
      TUASSERT(!(lessThanID > compare));
      TUASSERT(!(compare > equivalent));

         //SatelliteSystem only comparisons
      TUASSERT(!(compare > diffSatSys));
      TUASSERT((diffSatSys > compare));

         //Completely different comparisons
      TUASSERT(!(compare > diffEvery));
      TUASSERT((diffEvery > compare));
      TUASSERT((compare > diffEvery2));
      TUASSERT(!(diffEvery2 > compare));

      TUCSM("operator<=");
         //---------------------------------------------------------------------
         //Does the <= Operator function?
         //---------------------------------------------------------------------

         //ID only comparisons
      TUASSERT(!(compare <= lessThanID));
      TUASSERT( lessThanID <= compare);
      TUASSERT((compare <= equivalent));

         //SatelliteSystem only comparisons
      TUASSERT( compare <= diffSatSys);
      TUASSERT(!(diffSatSys <= compare));

         //Completely different comparisons
      TUASSERT( compare <= diffEvery);
      TUASSERT(!(diffEvery <= compare));
      TUASSERT(!(compare <= diffEvery2));
      TUASSERT( diffEvery2 <= compare);

      TUCSM("operator>=");
         //---------------------------------------------------------------------
         //Does the >= Operator function?
         //---------------------------------------------------------------------

         //ID only comparisons
      TUASSERT((compare >= lessThanID));
      TUASSERT(!(lessThanID >= compare));
      TUASSERT((compare >= equivalent));

         //SatelliteSystem only comparisons
      TUASSERT(!(compare >= diffSatSys));
      TUASSERT((diffSatSys >= compare));

         //Completely different comparisons
      TUASSERT(!(compare >= diffEvery));
      TUASSERT((diffEvery >= compare));
      TUASSERT((compare >= diffEvery2));
      TUASSERT(!(diffEvery2 >= compare));

      TUCSM("operator==");
      gnsstk::SatID ws1(1, gnsstk::SatelliteSystem::GPS);
      gnsstk::SatID ws2(1, gnsstk::SatelliteSystem::GPS);
      gnsstk::SatID ws3(3, gnsstk::SatelliteSystem::GPS);
      gnsstk::SatID ws4(1, gnsstk::SatelliteSystem::QZSS);
      gnsstk::SatID ws5(1); // wildcard system
      gnsstk::SatID ws6(gnsstk::SatelliteSystem::GPS); // wildcard satellite
      gnsstk::SatID ws7(gnsstk::SatelliteSystem::QZSS); // wildcard satellite
      gnsstk::SatID ws8; // wildcard sat and sys
         // we have to set wildcard flags explicitly for ws8
      ws8.makeWild();
      gnsstk::SatID ws9(2); // wildcard system
         // sanity checks
      TUASSERTE(bool, false, ws5.wildId);
      TUASSERTE(bool, true,  ws5.wildSys);
      TUASSERTE(bool, true,  ws6.wildId);
      TUASSERTE(bool, false, ws6.wildSys);
      TUASSERTE(bool, true,  ws7.wildId);
      TUASSERTE(bool, false, ws7.wildSys);
      TUASSERTE(bool, true,  ws8.wildId);
      TUASSERTE(bool, true,  ws8.wildSys);
      TUASSERTE(bool, false, ws9.wildId);
      TUASSERTE(bool, true,  ws9.wildSys);
         // actual tests
      TUASSERTE(bool, true,  ws1 == ws2);
      TUASSERTE(bool, false, ws1 == ws3);
      TUASSERTE(bool, false, ws1 == ws4);
      TUASSERTE(bool, true,  ws1 == ws5);
      TUASSERTE(bool, true,  ws1 == ws6);
      TUASSERTE(bool, false, ws1 == ws7);
      TUASSERTE(bool, true,  ws1 == ws8);
      TUASSERTE(bool, false, ws1 == ws9);

      TUCSM("operator<<");
         //---------------------------------------------------------------------
         //Does the << Operator function?
         //---------------------------------------------------------------------

      std::string outputString, compareString;
      std::stringstream outputStream;
      outputStream << redirected;
      outputString = outputStream.str();
      compareString = "GPS 6";

      TUASSERTE(std::string, compareString, outputString);

      TURETURN();
   }


   unsigned lessThanTest()
   {
      TUDEF("SatID", "operator<");
      gnsstk::SatID ws1(1, gnsstk::SatelliteSystem::GPS);
      gnsstk::SatID ws2(1, gnsstk::SatelliteSystem::GPS);
      gnsstk::SatID ws3(3, gnsstk::SatelliteSystem::GPS);
      gnsstk::SatID ws4(1, gnsstk::SatelliteSystem::QZSS);
      gnsstk::SatID ws5(1); // wildcard system
      gnsstk::SatID ws6(gnsstk::SatelliteSystem::GPS); // wildcard satellite
      gnsstk::SatID ws7(gnsstk::SatelliteSystem::QZSS); // wildcard satellite
      gnsstk::SatID ws8; // wildcard sat and sys
         // we have to set wildcard flags explicitly for ws8
      ws8.makeWild();
      gnsstk::SatID ws9(2); // wildcard system
         // sanity checks
      TUASSERTE(bool, false, ws5.wildId);
      TUASSERTE(bool, true,  ws5.wildSys);
      TUASSERTE(bool, true,  ws6.wildId);
      TUASSERTE(bool, false, ws6.wildSys);
      TUASSERTE(bool, true,  ws7.wildId);
      TUASSERTE(bool, false, ws7.wildSys);
      TUASSERTE(bool, true,  ws8.wildId);
      TUASSERTE(bool, true,  ws8.wildSys);
      TUASSERTE(bool, false, ws9.wildId);
      TUASSERTE(bool, true,  ws9.wildSys);
         // actual tests
      TUASSERTE(bool, false, ws1 < ws2);
      TUASSERTE(bool, false, ws2 < ws1);
      TUASSERTE(bool, true,  ws1 < ws3);
      TUASSERTE(bool, false, ws3 < ws1);
      TUASSERTE(bool, true,  ws1 < ws4);
      TUASSERTE(bool, false, ws4 < ws1);
      TUASSERTE(bool, false, ws1 < ws5);
      TUASSERTE(bool, false, ws5 < ws1);
      TUASSERTE(bool, false, ws1 < ws6);
      TUASSERTE(bool, false, ws6 < ws1);
      TUASSERTE(bool, true,  ws1 < ws7);
      TUASSERTE(bool, false, ws7 < ws1);
      TUASSERTE(bool, false, ws1 < ws8);
      TUASSERTE(bool, false, ws8 < ws1);
      TUASSERTE(bool, true,  ws1 < ws9);
      TUASSERTE(bool, false, ws9 < ws1);
      TURETURN();
   }


      /// check that the isValid method returns the proper value
   unsigned isValidTest()
   {
      TUDEF("SatID", "isValid()");

      gnsstk::SatID compare1(5  , gnsstk::SatelliteSystem(1));
      gnsstk::SatID compare2(1  , gnsstk::SatelliteSystem(15));
      gnsstk::SatID compare3(-1 , gnsstk::SatelliteSystem(-1));
      gnsstk::SatID compare4(100, gnsstk::SatelliteSystem(-1));
      gnsstk::SatID compare5(0  , gnsstk::SatelliteSystem(1));
      gnsstk::SatID compare6(32 , gnsstk::SatelliteSystem(1));
      gnsstk::SatID compare7(50 , gnsstk::SatelliteSystem(1));
      gnsstk::SatID compare8(0  , gnsstk::SatelliteSystem(1));
      gnsstk::SatID compare9(-3 , gnsstk::SatelliteSystem(1));

      TUASSERT(compare1.isValid());
      TUASSERT(compare2.isValid());
      TUASSERT(!compare3.isValid());
      TUASSERT(!compare4.isValid());
      TUASSERT(!compare5.isValid());
      TUASSERT(compare6.isValid());
      TUASSERT(!compare7.isValid());
      TUASSERT(!compare8.isValid());
      TUASSERT(!compare9.isValid());

      TURETURN();
   }

      /// Regression testing for string <-> enum translation
   unsigned stringConvertTest()
   {
      TUDEF("SatID", "convertSatelliteSystemToString");
      std::map<gnsstk::SatelliteSystem, std::string> testVals =
         {
            { gnsstk::SatelliteSystem::GPS, "GPS" },
            { gnsstk::SatelliteSystem::Galileo, "Galileo" },
            { gnsstk::SatelliteSystem::Glonass, "GLONASS" },
            { gnsstk::SatelliteSystem::Geosync, "Geostationary" },
            { gnsstk::SatelliteSystem::LEO, "LEO" },
            { gnsstk::SatelliteSystem::Transit, "Transit" },
            { gnsstk::SatelliteSystem::BeiDou, "BeiDou" },
            { gnsstk::SatelliteSystem::QZSS, "QZSS" },
            { gnsstk::SatelliteSystem::IRNSS, "IRNSS" },
            { gnsstk::SatelliteSystem::Mixed, "Mixed" },
            { gnsstk::SatelliteSystem::UserDefined, "UserDefined" },
            { gnsstk::SatelliteSystem::Unknown, "Unknown" }
         };

      for (const auto& tvi : testVals)
      {
         TUCSM("convertSatelliteSystemToString");
         TUASSERTE(std::string, tvi.second,
                   gnsstk::convertSatelliteSystemToString(tvi.first));
         TUCSM("convertStringToSatelliteSystem");
         TUASSERTE(gnsstk::SatelliteSystem, tvi.first,
                   gnsstk::convertStringToSatelliteSystem(tvi.second));
      }

      TURETURN();
   }
};


int main() //Main function to initialize and run all tests above
{
   SatID_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.initializationTest();
   errorTotal += testClass.dumpTest();
   errorTotal += testClass.operatorTest();
   errorTotal += testClass.lessThanTest();
   errorTotal += testClass.isValidTest();
   errorTotal += testClass.stringConvertTest();
   errorTotal += testClass.asStringTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal; //Return the total number of errors
}
