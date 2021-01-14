//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//  
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, const SatelliteSystem sys)
   {
      s << gpstk::StringUtils::asString(sys);
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

      gpstk::SatID compare1(5, gpstk::SatelliteSystem (1));
      TUASSERTE(int, 5, compare1.id);
      TUASSERTE(gpstk::SatelliteSystem,
                gpstk::SatelliteSystem(1),
                compare1.system);

      gpstk::SatID compare2(0, gpstk::SatelliteSystem (12));
      TUASSERTE(int, 0, compare2.id);
      TUASSERTE(gpstk::SatelliteSystem,
                gpstk::SatelliteSystem(12),
                compare2.system);

      gpstk::SatID compare3(-1, gpstk::SatelliteSystem (-1));
      TUASSERTE(int, -1, compare3.id);
      TUASSERTE(gpstk::SatelliteSystem,
                gpstk::SatelliteSystem(-1),
                compare3.system);

      gpstk::SatID ws1;
      TUASSERTE(bool, false, ws1.wildId);
      TUASSERTE(bool, false, ws1.wildSys);
      gpstk::SatID ws2(99);
      TUASSERTE(int, 99, ws2.id);
      TUASSERTE(bool, false, ws2.wildId);
      TUASSERTE(bool, true, ws2.wildSys);
      gpstk::SatID ws3(gpstk::SatelliteSystem::Mixed);
      TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::Mixed,
                ws3.system);
      TUASSERTE(bool, true, ws3.wildId);
      TUASSERTE(bool, false, ws3.wildSys);
      gpstk::SatID ws4(-65, gpstk::SatelliteSystem::LEO);
      TUASSERTE(int, -65, ws4.id);
      TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::LEO,
                ws4.system);
      TUASSERTE(bool, false, ws4.wildId);
      TUASSERTE(bool, false, ws4.wildSys);
      gpstk::SatID foo(25, gpstk::SatelliteSystem::QZSS);
      gpstk::SatID ws5(foo);
      TUASSERTE(int, 25, ws5.id);
      TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::QZSS,
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
      gpstk::SatID sat1(5, gpstk::SatelliteSystem (1));
      std::string outputString1, compareString1;
      std::stringstream outputStream1;

      sat1.dump(outputStream1);
      outputString1 = outputStream1.str();
      compareString1 = "GPS 5";
      TUASSERTE(std::string, compareString1, outputString1);

         //---------------------------------------------------------------------
         //Output for invalid UserDefined satellite and triple digit ID
         //---------------------------------------------------------------------
      gpstk::SatID sat2(110, gpstk::SatelliteSystem (11));
      std::string outputString2, compareString2;
      std::stringstream outputStream2;

      sat2.dump(outputStream2);
      outputString2 = outputStream2.str();
      compareString2 = "UserDefined 110";
      TUASSERTE(std::string, compareString2, outputString2);

         //---------------------------------------------------------------------
         //Output for invalid satellite and negative ID
         //---------------------------------------------------------------------
      gpstk::SatID sat3(-10, gpstk::SatelliteSystem (50));
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
      gpstk::SatID sat1(5, gpstk::SatelliteSystem (1));
      compareString1 = "GPS 5";
      TUASSERTE(std::string, compareString1,gpstk::StringUtils::asString(sat1));

         //---------------------------------------------------------------------
         //Output for invalid UserDefined satellite and triple digit ID
         //---------------------------------------------------------------------
      gpstk::SatID sat2(110, gpstk::SatelliteSystem (11));
      compareString2 = "UserDefined 110";
      TUASSERTE(std::string, compareString2,gpstk::StringUtils::asString(sat2));

         //---------------------------------------------------------------------
         //Output for invalid satellite and negative ID
         //---------------------------------------------------------------------
      gpstk::SatID sat3(-10, gpstk::SatelliteSystem (50));
      compareString3 = "??? -10";
      TUASSERTE(std::string, compareString3,gpstk::StringUtils::asString(sat3));

      TURETURN();
   }


      /// verify the various operators of the SatID class
   unsigned operatorTest()
   {
      TUDEF("SatID", "OperatorEquivalence");

      gpstk::SatID compare    (5, gpstk::SatelliteSystem(2));
      gpstk::SatID equivalent (5, gpstk::SatelliteSystem(2));
      gpstk::SatID lessThanID (2, gpstk::SatelliteSystem(2));
      gpstk::SatID diffSatSys (5, gpstk::SatelliteSystem(5));
      gpstk::SatID diffEvery  (2, gpstk::SatelliteSystem(5));
      gpstk::SatID diffEvery2 (7, gpstk::SatelliteSystem(1));
      gpstk::SatID redirected (6, gpstk::SatelliteSystem(1));

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
      gpstk::SatID ws1(1, gpstk::SatelliteSystem::GPS);
      gpstk::SatID ws2(1, gpstk::SatelliteSystem::GPS);
      gpstk::SatID ws3(3, gpstk::SatelliteSystem::GPS);
      gpstk::SatID ws4(1, gpstk::SatelliteSystem::QZSS);
      gpstk::SatID ws5(1); // wildcard system
      gpstk::SatID ws6(gpstk::SatelliteSystem::GPS); // wildcard satellite
      gpstk::SatID ws7(gpstk::SatelliteSystem::QZSS); // wildcard satellite
      gpstk::SatID ws8; // wildcard sat and sys
         // we have to set wildcard flags explicitly for ws8
      ws8.makeWild();
      gpstk::SatID ws9(2); // wildcard system
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
      gpstk::SatID ws1(1, gpstk::SatelliteSystem::GPS);
      gpstk::SatID ws2(1, gpstk::SatelliteSystem::GPS);
      gpstk::SatID ws3(3, gpstk::SatelliteSystem::GPS);
      gpstk::SatID ws4(1, gpstk::SatelliteSystem::QZSS);
      gpstk::SatID ws5(1); // wildcard system
      gpstk::SatID ws6(gpstk::SatelliteSystem::GPS); // wildcard satellite
      gpstk::SatID ws7(gpstk::SatelliteSystem::QZSS); // wildcard satellite
      gpstk::SatID ws8; // wildcard sat and sys
         // we have to set wildcard flags explicitly for ws8
      ws8.makeWild();
      gpstk::SatID ws9(2); // wildcard system
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

      gpstk::SatID compare1(5  , gpstk::SatelliteSystem(1));
      gpstk::SatID compare2(1  , gpstk::SatelliteSystem(15));
      gpstk::SatID compare3(-1 , gpstk::SatelliteSystem(-1));
      gpstk::SatID compare4(100, gpstk::SatelliteSystem(-1));
      gpstk::SatID compare5(0  , gpstk::SatelliteSystem(1));
      gpstk::SatID compare6(32 , gpstk::SatelliteSystem(1));
      gpstk::SatID compare7(50 , gpstk::SatelliteSystem(1));
      gpstk::SatID compare8(0  , gpstk::SatelliteSystem(1));
      gpstk::SatID compare9(-3 , gpstk::SatelliteSystem(1));

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
      std::map<gpstk::SatelliteSystem, std::string> testVals =
         {
            { gpstk::SatelliteSystem::GPS, "GPS" },
            { gpstk::SatelliteSystem::Galileo, "Galileo" },
            { gpstk::SatelliteSystem::Glonass, "GLONASS" },
            { gpstk::SatelliteSystem::Geosync, "Geostationary" },
            { gpstk::SatelliteSystem::LEO, "LEO" },
            { gpstk::SatelliteSystem::Transit, "Transit" },
            { gpstk::SatelliteSystem::BeiDou, "BeiDou" },
            { gpstk::SatelliteSystem::QZSS, "QZSS" },
            { gpstk::SatelliteSystem::IRNSS, "IRNSS" },
            { gpstk::SatelliteSystem::Mixed, "Mixed" },
            { gpstk::SatelliteSystem::UserDefined, "UserDefined" },
            { gpstk::SatelliteSystem::Unknown, "Unknown" }
         };

      for (const auto& tvi : testVals)
      {
         TUCSM("convertSatelliteSystemToString");
         TUASSERTE(std::string, tvi.second,
                   gpstk::convertSatelliteSystemToString(tvi.first));
         TUCSM("convertStringToSatelliteSystem");
         TUASSERTE(gpstk::SatelliteSystem, tvi.first,
                   gpstk::convertStringToSatelliteSystem(tvi.second));
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
