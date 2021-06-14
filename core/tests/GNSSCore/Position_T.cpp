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

#include "Position.hpp"
#include "TestUtil.hpp"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace gpstk;
using namespace gpstk::StringUtils;

class Position_T
{
public:
   Position_T(){eps = 1e-3;}// Default Constructor, set the precision value
   ~Position_T() {} // Default Desructor
   double eps;

      /* Test will check the transforms of Position Objects.
         There are 4  position types. This test will take a
         position starting in each type and transform it to
         each of the remaining types. This is a one-way transform,
         and comparisons will be performed using the range() function.

         Suppressing print lines from the test. */
   unsigned transformTest()
   {
      TUDEF("Position", "Cartesian transformTo");
      try
      {
         Position c,s,d,g; //An object for each of the Position types.
        
            // test transformTo

            // Start in ECEF (Cartesian)
         c.setECEF(-1575232.0141,-4707872.2332, 3993198.4383);
         Position t(c); // Comparison Object
         t.transformTo(Position::Geodetic);
         TUASSERTFEPS(0, range(t,c), eps);

         t = c; // Reset comparison object
         t.transformTo(Position::Geocentric);
         TUASSERTFEPS(0, range(t,c), eps);

         t = c; // Reset comparison object
         t.transformTo(Position::Spherical);
         TUASSERTFEPS(0, range(t,c), eps); 

            //Start in Geodetic
         TUCSM("Geodetic transformTo");
         d.setGeodetic(39.000004186778,251.499999999370,1400.009066903964);
         t = d;
         t.transformTo(Position::Cartesian);
         TUASSERTFEPS(0, range(t,d), eps);

         t = d;  // Reset comparison object
         t.transformTo(Position::Geocentric);
         TUASSERTFEPS(0, range(t,d), eps); 

         t = d;  // Reset comparison object
         t.transformTo(Position::Spherical);
         TUASSERTFEPS(0, range(t,d), eps); 

            //Start in Geocentric
         TUCSM("Geocentric transformTo");
         g.setGeocentric(38.811958506159,251.499999999370,6371110.627671023800);
         t = g;  // Reset comparison object
         t.transformTo(Position::Cartesian);
         TUASSERTFEPS(0, range(t,g), eps); 

         t = g;  // Reset comparison object
         t.transformTo(Position::Geodetic);
         TUASSERTFEPS(0, range(t,g), eps);

         t = g;  // Reset comparison object
         t.transformTo(Position::Spherical);
         TUASSERTFEPS(0, range(t,g), eps); 

            //Start in Spherical
         TUCSM("Spherical transformTo");
         s.setSpherical(51.188041493841,251.499999999370,6371110.627671023800);
         t = s;
         t.transformTo(Position::Cartesian);
         TUASSERTFEPS(0, range(t,s), eps);

         t = s;
         t.transformTo(Position::Geocentric);
         TUASSERTFEPS(0, range(t,s), eps);

         t = s;
         t.transformTo(Position::Geodetic);
         TUASSERTFEPS(0, range(t,s), eps); 
      }
      catch(...)
      {
         std::cout << "Exception encountered at: " << testFramework.countTests()
                   << std::endl
                   << "Test method failed" << std::endl;
      }
      TURETURN();
   }

      /* Test will check the formatted printing of Position objects. */
   unsigned printfTest()
   {
      TUDEF("Position", "printf");

      try
      {
         Position c;
         c.setECEF(-1575232.0141,-4707872.2332, 3993198.4383);

         TUASSERTE(std::string,
                   "-1575232.0141     X() (meters)",
                   c.printf("%13.4x     X() (meters)"));
         TUASSERTE(std::string,
                   "-4707872.2332     Y() (meters)",
                   c.printf("%13.4y     Y() (meters)"));
         TUASSERTE(std::string,
                   " 3993198.4383     Z() (meters)",
                   c.printf("%13.4z     Z() (meters)"));
         TUASSERTE(std::string,
                   "   -1575.2320     X()/1000 (kilometers)",
                   c.printf("%13.4X     X()/1000 (kilometers)"));
         TUASSERTE(std::string,
                   "   -4707.8722     Y()/1000 (kilometers)",
                   c.printf("%13.4Y     Y()/1000 (kilometers)"));
         TUASSERTE(std::string,
                   "    3993.1984     Z()/1000 (kilometers)",
                   c.printf("%13.4Z     Z()/1000 (kilometers)"));
         TUASSERTE(std::string,
                   "      39.000004   geodeticLatitude() (degrees North)",
                   c.printf("%15.6A   geodeticLatitude() (degrees North)"));
         TUASSERTE(std::string,
                   "      38.811959   geocentricLatitude() (degrees North)",
                   c.printf("%15.6a   geocentricLatitude() (degrees North)"));
         TUASSERTE(std::string,
                   "     251.500000   longitude() (degrees East)",
                   c.printf("%15.6L   longitude() (degrees East)"));
         TUASSERTE(std::string,
                   "     251.500000   longitude() (degrees East)",
                   c.printf("%15.6l   longitude() (degrees East)"));
         TUASSERTE(std::string,
                   "     108.500000   longitude() (degrees West)",
                   c.printf("%15.6w   longitude() (degrees West)"));
         TUASSERTE(std::string,
                   "     108.500000   longitude() (degrees West)",
                   c.printf("%15.6W   longitude() (degrees West)"));
         TUASSERTE(std::string,
                   "      51.188041   theta() (degrees)",
                   c.printf("%15.6t   theta() (degrees)"));
         TUASSERTE(std::string,
                   "       0.893400   theta() (radians)",
                   c.printf("%15.6T   theta() (radians)"));
         TUASSERTE(std::string,
                   "     251.500000   phi() (degrees)",
                   c.printf("%15.6p   phi() (degrees)"));
         TUASSERTE(std::string,
                   "       4.389503   phi() (radians)",
                   c.printf("%15.6P   phi() (radians)"));
         TUASSERTE(std::string,
                   " 6371110.6277     radius() meters",
                   c.printf("%13.4r     radius() meters"));
         TUASSERTE(std::string,
                   "    6371.1106     radius()/1000 kilometers",
                   c.printf("%13.4R     radius()/1000 kilometers"));
         TUASSERTE(std::string,
                   "    1400.0091     height() meters",
                   c.printf("%13.4h     height() meters"));
         TUASSERTE(std::string,
                   "       1.4000     height()/1000 kilometers",
                   c.printf("%13.4H     height()/1000 kilometers"));
      }
      catch(...)
      {
         std::cout << "Exception encountered at: " << testFramework.countTests()
                   << std::endl
                   << "Test method failed" << std::endl;
      }
      TURETURN();
   }

      /*      Test for scanning strings
              Additional print lines are commented out. */
   unsigned scanTest()
   {
      TUDEF("Position", "scan");

      try
      {
         Position c; // Initial position
         c.setECEF(-1575232.0141,-4707872.2332, 3993198.4383);
         string fmt[5]={ //Various string formats
            "", //This one is left empty but is skipped in the loop
            "%A degN %L degE %h m",
            "%a degN %L degE %r m",
            "%x m %y m %z m",
            "%t deg %p deg %r M"};
         for(int i=1; i<5; i++) 
         {
            string str;
               // A dummy Position initialized at c and another
               // Position for comparison
            Position t(c),tt;
            t.transformTo(static_cast<Position::CoordinateSystem>(i));
            {
               ostringstream o;
               o << t; //Output the Position object to stream
               str = o.str(); //Store that output as a string
            }
               //Set the comparison object using the output string
            tt.setToString(str,fmt[i]);

            TUASSERTFEPS(0, range(tt,t), eps);
         }
      }
      catch(...)
      {
         std::cout << "Exception encountered at: " << testFramework.countTests()
                   << std::endl
                   << "Test method failed" << std::endl;
      }
      TURETURN();
   }

      /*      Elevation and Azimuth tests
              Comparing these calculations from the ones in Triple
              (which are tested in the Triple tests) */
   unsigned elevationAzimuthTest()
   {
      TUDEF("Position", "elevationAzimuth");
      try
      {
         Position c,s;
         c.setECEF(-1575232.0141,-4707872.2332, 3993198.4383);
         s.setECEF(3*6371110.62767,0,0);
         TUASSERTFEPS(c.elvAngle(s), c.elevation(s), eps);
         TUASSERTFEPS(c.azAngle(s), c.azimuth(s), eps);
      }
      catch(...)
      {
         std::cout << "Exception encountered at: " << testFramework.countTests()
                   << std::endl
                   << "Test method failed" << std::endl;
      }
      TURETURN();
   }

      /*      Transform tests at a pole. The pole is a unique location
              which may cause the transforms to break. */
   unsigned poleTransformTest()
   {
      TUDEF("Position", "poleTransform");
      try
      {
         Position c,t;
            //cout << "Try to break it at the pole\n";
         c.setECEF(0,0,6371110.6277);
            //c.setECEF(0,0,0.0001);         // this will break it
         t = c;

            // Code below tests every possible conversion from one
            // coordinate system to the next at the pole.
            // i.e.    ECEF -> Geodetic
            //         ECEF -> Geocentric
            //         ECEF -> Spherical
            //         Spherical -> Geodetic
            //         Spherical -> Geocentric
            //         Spherical -> ECEF
            //         etc...

         t.transformTo(Position::Geodetic);
         TUASSERTFEPS(0, range(t,c), eps);
         t.transformTo(Position::Geocentric);
         TUASSERTFEPS(0, range(t,c), eps); 
         t.transformTo(Position::Spherical);
         TUASSERTFEPS(0, range(t,c), eps);
         t.transformTo(Position::Cartesian);
         TUASSERTFEPS(0, range(t,c), eps); 
         t.transformTo(Position::Geodetic);
         TUASSERTFEPS(0, range(t,c), eps); 
         t.transformTo(Position::Cartesian);
         TUASSERTFEPS(0, range(t,c), eps); 
         t.transformTo(Position::Spherical);
         TUASSERTFEPS(0, range(t,c), eps); 
         t.transformTo(Position::Geodetic);
         TUASSERTFEPS(0, range(t,c), eps); 
         t.transformTo(Position::Spherical);
         TUASSERTFEPS(0, range(t,c), eps);
         t.transformTo(Position::Geocentric);
         TUASSERTFEPS(0, range(t,c), eps); 
         t.transformTo(Position::Cartesian);
         TUASSERTFEPS(0, range(t,c), eps);
      }
      catch(...)
      {
         std::cout << "Exception encountered at: " << testFramework.countTests()
                   << std::endl
                   << "Test method failed" << std::endl;
      }
      TURETURN();
   }

      /*      Many of the tests above use the range() function to
              measure the distances between two positions. It in turn 
              needs to be tested to ensure that it works. */
   unsigned rangeTest()
   {
      TUDEF("Position", "range()");
      try
      {
         Position c,t;
         c.setECEF(0,0,6371110.6277);
         t.setECEF(20,0,6371110.6277);
         TUASSERTFEPS(20, range(c,t), eps);
         t.setECEF(0,-20,6371110.6277);
         TUASSERTFEPS(20, range(c,t), eps);
         t.setECEF(0,0,6371210.6277);
         TUASSERTFEPS(100, range(c,t), eps);
         t.setECEF(300,400,6371610.6277);
         TUASSERTFEPS(sqrt(500000.0), range(c,t), eps);
      }
      catch(...)
      {
         std::cout << "Exception encountered at: " << testFramework.countTests()
                   << std::endl
                   << "Test method failed" << std::endl;
      }
      TURETURN();
   }
};

int main() //Main function to initialize and run all tests above
{
   unsigned errorTotal = 0;
   Position_T testClass;

   errorTotal += testClass.rangeTest();
   errorTotal += testClass.transformTest();
   errorTotal += testClass.printfTest();
   errorTotal += testClass.scanTest();
   errorTotal += testClass.elevationAzimuthTest();
   errorTotal += testClass.poleTransformTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal; //Return the total number of errors
}
