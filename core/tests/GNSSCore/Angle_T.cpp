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

#include <math.h>
#include "TestUtil.hpp"
#include "Angle.hpp"
#include "GNSSconstants.hpp"

class Angle_T
{
public:
   Angle_T();
   unsigned constructorTest();
   unsigned setValueTest();
   static const double epsilon;

   class TestData
   {
   public:
      TestData(double val, gnsstk::AngleType type, double radians,
               double degrees, double sine, double cosine, double semicirc,
               double tangent)
            : v(val), t(type), rad(radians), deg(degrees), sin(sine),
              cos(cosine), sc(semicirc), tan(tangent)
      {}
      double v;
      gnsstk::AngleType t;
      double rad;
      double deg;
      double sc;
      double sin;
      double cos;
      double tan;
   };
};


const double Angle_T::epsilon = 1e-9;

Angle_T ::
Angle_T()
{
}


unsigned Angle_T ::
constructorTest()
{
   TUDEF("Angle", "Angle");
   const double radians = 0.52359877559829881566;
   const double degrees = 30.0;
   const double semicirc = 0.16666666666666666666;
   const double sine = ::sin(radians);
   const double cosine = ::cos(radians);
   const double tangent = ::tan(radians);
      // test default constructor
   gnsstk::Angle uut1;
   TUASSERTE(bool, true, isnan(uut1.rad()));
   TUASSERTE(bool, true, isnan(uut1.deg()));
   TUASSERTE(bool, true, isnan(uut1.semicirc()));
   TUASSERTE(bool, true, isnan(uut1.sin()));
   TUASSERTE(bool, true, isnan(uut1.cos()));
   TUASSERTE(bool, true, isnan(uut1.tan()));
      // test radians constructor
   gnsstk::Angle uut2(radians, gnsstk::AngleType::Rad);
   TUASSERTFEPS(radians, uut2.rad(), epsilon);
   TUASSERTFEPS(degrees, uut2.deg(), epsilon);
   TUASSERTFEPS(semicirc,uut2.semicirc(), epsilon);
   TUASSERTFEPS(sine,    uut2.sin(), epsilon);
   TUASSERTFEPS(cosine,  uut2.cos(), epsilon);
   TUASSERTFEPS(tangent, uut2.tan(), epsilon);
      // test degrees constructor
   gnsstk::Angle uut3(degrees, gnsstk::AngleType::Deg);
   TUASSERTFEPS(radians, uut3.rad(), epsilon);
   TUASSERTFEPS(degrees, uut3.deg(), epsilon);
   TUASSERTFEPS(semicirc,uut3.semicirc(), epsilon);
   TUASSERTFEPS(sine,    uut3.sin(), epsilon);
   TUASSERTFEPS(cosine,  uut3.cos(), epsilon);
   TUASSERTFEPS(tangent, uut3.tan(), epsilon);
      // test sin constructor
   gnsstk::Angle uut4(sine, gnsstk::AngleType::Sin);
   TUASSERTFEPS(radians, uut4.rad(), epsilon);
   TUASSERTFEPS(degrees, uut4.deg(), epsilon);
   TUASSERTFEPS(semicirc,uut4.semicirc(), epsilon);
   TUASSERTFEPS(sine,    uut4.sin(), epsilon);
   TUASSERTFEPS(cosine,  uut4.cos(), epsilon);
   TUASSERTFEPS(tangent, uut4.tan(), epsilon);
      // test cos constructor
   gnsstk::Angle uut5(cosine, gnsstk::AngleType::Cos);
   TUASSERTFEPS(radians, uut5.rad(), epsilon);
   TUASSERTFEPS(degrees, uut5.deg(), epsilon);
   TUASSERTFEPS(semicirc,uut5.semicirc(), epsilon);
   TUASSERTFEPS(sine,    uut5.sin(), epsilon);
   TUASSERTFEPS(cosine,  uut5.cos(), epsilon);
   TUASSERTFEPS(tangent, uut5.tan(), epsilon);
      // test sin/cos constructor
   gnsstk::Angle uut6(sine, cosine);
   TUASSERTFEPS(radians, uut6.rad(), epsilon);
   TUASSERTFEPS(degrees, uut6.deg(), epsilon);
   TUASSERTFEPS(semicirc,uut6.semicirc(), epsilon);
   TUASSERTFEPS(sine,    uut6.sin(), epsilon);
   TUASSERTFEPS(cosine,  uut6.cos(), epsilon);
   TUASSERTFEPS(tangent, uut6.tan(), epsilon);
      // test semi-circles constructor
   gnsstk::Angle uut7(semicirc, gnsstk::AngleType::SemiCircle);
   TUASSERTFEPS(radians, uut7.rad(), epsilon);
   TUASSERTFEPS(degrees, uut7.deg(), epsilon);
   TUASSERTFEPS(semicirc,uut7.semicirc(), epsilon);
   TUASSERTFEPS(sine,    uut7.sin(), epsilon);
   TUASSERTFEPS(cosine,  uut7.cos(), epsilon);
   TUASSERTFEPS(tangent, uut7.tan(), epsilon);
   TURETURN();
}


unsigned Angle_T ::
setValueTest()
{
   TUDEF("Angle", "setValue");
   using gnsstk::DEG2RAD;
   using gnsstk::RAD2DEG;
   static const TestData testData[] =
      {
         { 0.0,  gnsstk::AngleType::Rad, 0.0, 0.0, 0.0,  1.0, 0.0, 0.0 },
         { 1.0,  gnsstk::AngleType::Deg, 1.0*DEG2RAD, 1.0, ::sin(1.0*DEG2RAD),
           ::cos(1.0*DEG2RAD), 1.0/180.0, ::tan(1.0*DEG2RAD) },
         { 0.34, gnsstk::AngleType::Sin, ::asin(0.34), ::asin(0.34)*RAD2DEG,
           0.34, ::cos(::asin(0.34)), ::asin(0.34)/gnsstk::PI,
           ::tan(::asin(0.34)) },
         { 0.78, gnsstk::AngleType::Cos, ::acos(0.78), ::acos(0.78)*RAD2DEG,
           ::sin(::acos(0.78)),  0.78, ::acos(0.78)/gnsstk::PI,
           ::tan(::acos(0.78)) },
      };
   unsigned numTests = sizeof(testData) / sizeof(testData[0]);
   for (unsigned testNum = 0; testNum < numTests; testNum++)
   {
      const TestData& td(testData[testNum]);
      gnsstk::Angle uut;
      uut.setValue(td.v, td.t);
      TUASSERTFEPS(td.rad, uut.rad(), epsilon);
      TUASSERTFEPS(td.deg, uut.deg(), epsilon);
      TUASSERTFEPS(td.sc,  uut.semicirc(), epsilon);
      TUASSERTFEPS(td.sin, uut.sin(), epsilon);
      TUASSERTFEPS(td.cos, uut.cos(), epsilon);
      TUASSERTFEPS(td.tan, uut.tan(), epsilon);
   }
   TURETURN();
}


int main()
{
   unsigned errorTotal = 0;
   Angle_T testClass;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.setValueTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
