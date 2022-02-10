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

#include "CivilTime.hpp"
#include "CommonTime.hpp"
#include "TimeSystemCorr.hpp"
#include "TestUtil.hpp"
#include <iostream>
#include <sstream>
#include <cmath>

class TimeSystemCorr_T
{
public:
      // Default Constructor, set the precision value
   TimeSystemCorr_T()
   { eps = 1E-12; }

      /** Ensures the constructors set the values properly.  Get
       * methods are tested implicitly */
   unsigned initializationTest();

      /** operator==() and operator()< only exist in order to support
       * use of TimeSystemCorr objects as map keys.  In this role, only the
       * CorrType is checked.   Therefore, it is sufficient
       * to see that the ordering is maintained without change. */
   unsigned operatorTest();

   unsigned correctionTest();

   gnsstk::TimeSystemCorrection buildObject(const std::string& str);

private:
   double eps;

   double A0;
   double A1;
   double deltaT;
   gnsstk::CommonTime tscRefTime;
};


unsigned TimeSystemCorr_T ::
initializationTest()
{
   TUDEF("TimeSystemCorr", "Constructor");

   gnsstk::TimeSystemCorrection cUnknown;
   TUASSERTE(gnsstk::TimeSystemCorrection::CorrType,
             gnsstk::TimeSystemCorrection::Unknown, cUnknown.type);

   gnsstk::TimeSystemCorrection cIRGP("IRGP");
   TUASSERTE(gnsstk::TimeSystemCorrection::CorrType,
             gnsstk::TimeSystemCorrection::IRGP, cIRGP.type);
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::IRN, cIRGP.frTS);
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GPS, cIRGP.toTS);
   TUASSERTFE(0.0, cIRGP.A0);
   TUASSERTFE(0.0, cIRGP.A1);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::BEGINNING_OF_TIME,
             cIRGP.refTime);
   TUASSERTE(int, 0, cIRGP.geoUTCid);

   TURETURN();
}


unsigned TimeSystemCorr_T ::
operatorTest()
{
   TUDEF("TimeSystemCorr", "operator<");

      // Build a complete set of correction objects
   gnsstk::TimeSystemCorrection cUnknown;
   gnsstk::TimeSystemCorrection cGPUT("GPUT");
   gnsstk::TimeSystemCorrection cGAUT("GAUT");
   gnsstk::TimeSystemCorrection cSBUT("SBUT");
   gnsstk::TimeSystemCorrection cGLUT("GLUT");
   gnsstk::TimeSystemCorrection cGPGA("GPGA");
   gnsstk::TimeSystemCorrection cGLGP("GLGP");
   gnsstk::TimeSystemCorrection cQZGP("QZGP");
   gnsstk::TimeSystemCorrection cQZUT("QZUT");
   gnsstk::TimeSystemCorrection cBDUT("BDUT");
   gnsstk::TimeSystemCorrection cBDGP("BDGP");
   gnsstk::TimeSystemCorrection cIRUT("IRUT");
   gnsstk::TimeSystemCorrection cIRGP("IRGP");

   TUCSM("operator==");
   TUASSERT( !(cUnknown==cGPUT));
   TUASSERT( !(cGPUT==cGAUT));
   TUASSERT( !(cGAUT==cSBUT));
   TUASSERT( !(cSBUT==cGLUT));
   TUASSERT( !(cGLUT==cGPGA));
   TUASSERT( !(cGPGA==cGLGP));
   TUASSERT( !(cGLGP==cQZGP));
   TUASSERT( !(cQZGP==cQZUT));
   TUASSERT( !(cQZUT==cBDUT));
   TUASSERT( !(cBDUT==cBDGP));
   TUASSERT( !(cBDGP==cIRUT));
   TUASSERT( !(cIRUT==cIRGP));

   TUCSM("operator<");
   TUASSERT(cUnknown < cGPUT);
   TUASSERT(cGPUT < cGAUT);
   TUASSERT(cGAUT < cSBUT);
   TUASSERT(cSBUT < cGLUT);
   TUASSERT(cGLUT < cGPGA);
   TUASSERT(cGPGA < cGLGP);
   TUASSERT(cGLGP < cQZGP);
   TUASSERT(cQZGP < cQZUT);
   TUASSERT(cQZUT < cBDUT);
   TUASSERT(cBDUT < cBDGP);
   TUASSERT(cBDGP < cIRUT);
   TUASSERT(cIRUT < cIRGP);

   TURETURN();
}


unsigned TimeSystemCorr_T ::
correctionTest()
{
   TUDEF("TimeSystemCorr", "Correction");

      // Define a representative A0 and A1.  Define an epoch time and
      // a reasonable refernce time.
      // Use these values throughout the set of tests.
      // Test at least one conversion for each conversion type.
   A0 = 1.0e-7;
   A1 = 1.0e-12;
   deltaT = -10000;    // 10,000 sec is a nice round number for prpogation
   tscRefTime = gnsstk::CivilTime(2016, 1, 3, 0, 0, 0.0);

   gnsstk::CommonTime timeOfInterest = tscRefTime + deltaT;
   double forwardResult = A0 + A1 * deltaT;
   forwardResult = forwardResult * -1.0;   // This is a CORRECTION, not an error
   double corrVal = 0.0;

   gnsstk::TimeSystemCorrection tscTest = buildObject("GPUT");
   timeOfInterest.setTimeSystem(gnsstk::TimeSystem::GPS);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(gnsstk::TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   tscTest = buildObject("GAUT");
   timeOfInterest.setTimeSystem(gnsstk::TimeSystem::GAL);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(gnsstk::TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   tscTest = buildObject("QZUT");
   timeOfInterest.setTimeSystem(gnsstk::TimeSystem::QZS);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(gnsstk::TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   tscTest = buildObject("BDUT");
   timeOfInterest.setTimeSystem(gnsstk::TimeSystem::BDT);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(gnsstk::TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   tscTest = buildObject("IRUT");
   timeOfInterest.setTimeSystem(gnsstk::TimeSystem::IRN);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(gnsstk::TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

      // GLONASS is a little different in that the A1 term is not used
   A1 = 0;
   forwardResult = A0 + A1 * deltaT;
   forwardResult = forwardResult * -1.0;   // This is a CORRECTION, not an error
   tscTest = buildObject("GLUT");
   timeOfInterest.setTimeSystem(gnsstk::TimeSystem::GLO);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(gnsstk::TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   TURETURN();
}

gnsstk::TimeSystemCorrection TimeSystemCorr_T ::
buildObject(const std::string& str)
{
   gnsstk::TimeSystemCorrection tsc(str);
   tsc.refTime = tscRefTime;
   tsc.A0 = A0;
   tsc.A1 = A1;

   return tsc;
}


int main() //Main function to initialize and run all tests above
{
   TimeSystemCorr_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.initializationTest();
   errorTotal += testClass.operatorTest();
   errorTotal += testClass.correctionTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal; //Return the total number of errors
}
