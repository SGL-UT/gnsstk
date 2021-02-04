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

   gpstk::TimeSystemCorrection buildObject(const std::string& str);

private:
   double eps;

   double A0;
   double A1;
   double deltaT;
   gpstk::CommonTime tscRefTime; 
};


unsigned TimeSystemCorr_T ::
initializationTest()
{
   TUDEF("TimeSystemCorr", "Constructor");

   gpstk::TimeSystemCorrection cUnknown;
   TUASSERTE(gpstk::TimeSystemCorrection::CorrType,
             gpstk::TimeSystemCorrection::Unknown, cUnknown.type);

   gpstk::TimeSystemCorrection cIRGP("IRGP");
   TUASSERTE(gpstk::TimeSystemCorrection::CorrType,
             gpstk::TimeSystemCorrection::IRGP, cIRGP.type);
   TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::IRN, cIRGP.frTS);
   TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::GPS, cIRGP.toTS);
   TUASSERTFE(0.0, cIRGP.A0);
   TUASSERTFE(0.0, cIRGP.A1);
   TUASSERTE(long, 0, cIRGP.refWeek);
   TUASSERTE(long, 0, cIRGP.refSOW);
   TUASSERTE(long, 0, cIRGP.refYr);
   TUASSERTE(long, 0, cIRGP.refMon);
   TUASSERTE(long, 0, cIRGP.refDay);
   TUASSERTE(int, 0, cIRGP.geoUTCid);

   TURETURN();
}


unsigned TimeSystemCorr_T ::
operatorTest()
{
   TUDEF("TimeSystemCorr", "operator<");

      // Build a complete set of correction objects
   gpstk::TimeSystemCorrection cUnknown;
   gpstk::TimeSystemCorrection cGPUT("GPUT");
   gpstk::TimeSystemCorrection cGAUT("GAUT");
   gpstk::TimeSystemCorrection cSBUT("SBUT");
   gpstk::TimeSystemCorrection cGLUT("GLUT");
   gpstk::TimeSystemCorrection cGPGA("GPGA");
   gpstk::TimeSystemCorrection cGLGP("GLGP");
   gpstk::TimeSystemCorrection cQZGP("QZGP");
   gpstk::TimeSystemCorrection cQZUT("QZUT");
   gpstk::TimeSystemCorrection cBDUT("BDUT");
   gpstk::TimeSystemCorrection cBDGP("BDGP");
   gpstk::TimeSystemCorrection cIRUT("IRUT");
   gpstk::TimeSystemCorrection cIRGP("IRGP");

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
   tscRefTime = gpstk::CivilTime(2016, 1, 3, 0, 0, 0.0); 

   gpstk::CommonTime timeOfInterest = tscRefTime + deltaT;
   double forwardResult = A0 + A1 * deltaT; 
   forwardResult = forwardResult * -1.0;   // This is a CORRECTION, not an error
   double corrVal = 0.0; 

   gpstk::TimeSystemCorrection tscTest = buildObject("GPUT");
   timeOfInterest.setTimeSystem(gpstk::TimeSystem::GPS);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(gpstk::TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   tscTest = buildObject("GAUT");
   timeOfInterest.setTimeSystem(gpstk::TimeSystem::GAL);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(gpstk::TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   tscTest = buildObject("QZUT");
   timeOfInterest.setTimeSystem(gpstk::TimeSystem::QZS);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(gpstk::TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   tscTest = buildObject("BDUT");
   timeOfInterest.setTimeSystem(gpstk::TimeSystem::BDT);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(gpstk::TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   tscTest = buildObject("IRUT");
   timeOfInterest.setTimeSystem(gpstk::TimeSystem::IRN);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(gpstk::TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

      // GLONASS is a little different in that the A1 term is not used
   A1 = 0;
   forwardResult = A0 + A1 * deltaT; 
   forwardResult = forwardResult * -1.0;   // This is a CORRECTION, not an error
   tscTest = buildObject("GLUT");
   timeOfInterest.setTimeSystem(gpstk::TimeSystem::GLO);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(gpstk::TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   TURETURN();
}

gpstk::TimeSystemCorrection TimeSystemCorr_T ::
buildObject(const std::string& str)
{
   gpstk::TimeSystemCorrection tsc(str);
   tsc.refWeek = static_cast<gpstk::GPSWeekSecond>(tscRefTime).week;
   tsc.refSOW = static_cast<gpstk::GPSWeekSecond>(tscRefTime).sow;
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
