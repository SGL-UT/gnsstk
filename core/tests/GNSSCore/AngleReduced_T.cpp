//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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

#include <math.h>
#include "TestUtil.hpp"
#include "AngleReduced.hpp"
#include "GNSSconstants.hpp"

class AngleReduced_T
{
public:
   AngleReduced_T();
   unsigned constructorTest();
   unsigned setValueTest();
   static const double epsilon;

   class TestData
   {
   public:
      TestData(double val, gnsstk::AngleReduced::Type type, double radians,
               double degrees, double sine, double cosine)
            : v(val), t(type), rad(radians), deg(degrees), sin(sine),
              cos(cosine)
      {}
      double v;
      gnsstk::AngleReduced::Type t;
      double rad;
      double deg;
      double sin;
      double cos;
   };
};


const double AngleReduced_T::epsilon = 1e-9;

AngleReduced_T ::
AngleReduced_T()
{
}


unsigned AngleReduced_T ::
constructorTest()
{
   TUDEF("AngleReduced", "AngleReduced");
   const double radians = 0.52359877559829881566;
   const double degrees = 30.0;
   const double sine = ::sin(radians);
   const double cosine = ::cos(radians);
      // test default constructor
   gnsstk::AngleReduced uut1;
   TUASSERTE(bool, true, isnan(uut1.sin()));
   TUASSERTE(bool, true, isnan(uut1.cos()));
      // test radians constructor
   gnsstk::AngleReduced uut2(radians, gnsstk::AngleReduced::Rad);
   TUASSERTFEPS(sine,    uut2.sin(), epsilon);
   TUASSERTFEPS(cosine,  uut2.cos(), epsilon);
      // test degrees constructor
   gnsstk::AngleReduced uut3(degrees, gnsstk::AngleReduced::Deg);
   TUASSERTFEPS(sine,    uut3.sin(), epsilon);
   TUASSERTFEPS(cosine,  uut3.cos(), epsilon);
      // test sin constructor
   gnsstk::AngleReduced uut4(sine, gnsstk::AngleReduced::Sin);
   TUASSERTFEPS(sine,    uut4.sin(), epsilon);
   TUASSERTFEPS(cosine,  uut4.cos(), epsilon);
      // test cos constructor
   gnsstk::AngleReduced uut5(cosine, gnsstk::AngleReduced::Cos);
   TUASSERTFEPS(sine,    uut5.sin(), epsilon);
   TUASSERTFEPS(cosine,  uut5.cos(), epsilon);
      // test sin/cos constructor
   gnsstk::AngleReduced uut6(sine, cosine);
   TUASSERTFEPS(sine,    uut6.sin(), epsilon);
   TUASSERTFEPS(cosine,  uut6.cos(), epsilon);
   TURETURN();
}


unsigned AngleReduced_T ::
setValueTest()
{
   TUDEF("AngleReduced", "setValue");
   using gnsstk::DEG2RAD;
   using gnsstk::RAD2DEG;
   static const TestData testData[] =
      {
         { 0.0,  gnsstk::AngleReduced::Rad, 0.0, 0.0, 0.0,  1.0 },
         { 1.0,  gnsstk::AngleReduced::Deg, 1.0*DEG2RAD, 1.0, ::sin(1.0*DEG2RAD),
           ::cos(1.0*DEG2RAD) },
         { 0.34, gnsstk::AngleReduced::Sin, ::asin(0.34), ::asin(0.34)*RAD2DEG,
           0.34, ::cos(::asin(0.34)) },
         { 0.78, gnsstk::AngleReduced::Cos, ::acos(0.78), ::acos(0.78)*RAD2DEG,
           ::sin(::acos(0.78)),  0.78 },
      };
   unsigned numTests = sizeof(testData) / sizeof(testData[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestData& td(testData[testNum]);
      gnsstk::AngleReduced uut;
      uut.setValue(td.v, td.t);
      TUASSERTFEPS(td.sin, uut.sin(), epsilon);
      TUASSERTFEPS(td.cos, uut.cos(), epsilon);
   }
   TURETURN();
}


int main()
{
   unsigned errorTotal = 0;
   AngleReduced_T testClass;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.setValueTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
