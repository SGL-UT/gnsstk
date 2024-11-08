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

using namespace gnsstk;

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
   unsigned convertTimeSystemToCorrTypeTest();
   unsigned asStringTest();
   unsigned asString4Test();
   unsigned dumpTest();
   unsigned isConverterForTest();
   unsigned fixTimeSystemTest();

   TimeSystemCorrection buildObject(const std::string& str);

private:
   double eps;

   double A0;
   double A1;
   double deltaT;
   CommonTime tscRefTime;
};

unsigned TimeSystemCorr_T ::
initializationTest()
{
   TUDEF("TimeSystemCorr", "Constructor");

   TimeSystemCorrection cUnknown;
   TUASSERTE(TimeSystemCorrection::CorrType,
             TimeSystemCorrection::Unknown, cUnknown.type);
            
         
   TimeSystemCorrection cIRGP("IRGP");
   TUASSERTE(TimeSystemCorrection::CorrType,
             TimeSystemCorrection::IRGP, cIRGP.type);
   TUASSERTE(TimeSystem, TimeSystem::IRN, cIRGP.frTS);
   TUASSERTE(TimeSystem, TimeSystem::GPS, cIRGP.toTS);
   TUASSERTFE(0.0, cIRGP.A0);
   TUASSERTFE(0.0, cIRGP.A1);
   TUASSERTE(CommonTime, CommonTime::BEGINNING_OF_TIME,
             cIRGP.refTime);
   TUASSERTE(int, 0, cIRGP.geoUTCid);

   TURETURN();
}

unsigned TimeSystemCorr_T ::
operatorTest()
{
   TUDEF("TimeSystemCorr", "operator<");

      // Build a complete set of correction objects
   TimeSystemCorrection cUnknown;
   TimeSystemCorrection cGPUT("GPUT");
   TimeSystemCorrection cGAUT("GAUT");
   TimeSystemCorrection cSBUT("SBUT");
   TimeSystemCorrection cGLUT("GLUT");
   TimeSystemCorrection cGPGA("GPGA");
   TimeSystemCorrection cGLGP("GLGP");
   TimeSystemCorrection cQZGP("QZGP");
   TimeSystemCorrection cQZUT("QZUT");
   TimeSystemCorrection cBDUT("BDUT");
   TimeSystemCorrection cBDGP("BDGP");
   TimeSystemCorrection cIRUT("IRUT");
   TimeSystemCorrection cIRGP("IRGP");

   // Test invalid sting init
   TUTHROW(TimeSystemCorrection cIRGP("Invalid Time Correction")); 

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
   tscRefTime = CivilTime(2016, 1, 3, 0, 0, 0.0);

   CommonTime timeOfInterest = tscRefTime + deltaT;
   double forwardResult = A0 + A1 * deltaT;
   forwardResult = forwardResult * -1.0;   // This is a CORRECTION, not an error
   double corrVal = 0.0;

   // GPUT corrections
   TimeSystemCorrection tscTest = buildObject("GPUT");
   timeOfInterest.setTimeSystem(TimeSystem::GPS);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::GAL); // Anything but GPS or UTC
   TUTHROW(tscTest.Correction(timeOfInterest));

   // GAUT corrections
   tscTest = buildObject("GAUT");
   timeOfInterest.setTimeSystem(TimeSystem::GAL);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::GPS); // Anything but GAL or UTC
   TUTHROW(tscTest.Correction(timeOfInterest));

   // SBUT corrections
   tscTest = buildObject("SBUT");
   TUTHROW(tscTest.Correction(timeOfInterest));

   // GPGA corrections
   tscTest = buildObject("GPGA");
   timeOfInterest.setTimeSystem(TimeSystem::GAL);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::GPS);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::UTC); // Anything but GAL GPS
   TUTHROW(tscTest.Correction(timeOfInterest));
   
   // GLGP corrections
   tscTest = buildObject("GLGP");
   timeOfInterest.setTimeSystem(TimeSystem::GLO);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(tscTest.A0, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::GPS);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-tscTest.A0, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::UTC); // Anything but GLO GPS
   TUTHROW(tscTest.Correction(timeOfInterest));

   // QZGP corrections
   tscTest = buildObject("QZGP");
   timeOfInterest.setTimeSystem(TimeSystem::QZS);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(0.0, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::GPS);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(0.0, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::UTC); // Anything but QZS GPS
   TUTHROW(tscTest.Correction(timeOfInterest));

   // QZUT corrections
   tscTest = buildObject("QZUT");
   timeOfInterest.setTimeSystem(TimeSystem::QZS);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::GPS); // Anything but QZS UTC
   TUTHROW(tscTest.Correction(timeOfInterest));

   // BDUT corrections
   tscTest = buildObject("BDUT");
   timeOfInterest.setTimeSystem(TimeSystem::BDT);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::GPS); // Anything but BDT UTC
   TUTHROW(tscTest.Correction(timeOfInterest));

   // BDGP corrections
   tscTest = buildObject("BDGP");
   timeOfInterest.setTimeSystem(TimeSystem::BDT);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(tscTest.A0, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::GPS);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-tscTest.A0, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::UTC); // Anything but BDT GPS
   TUTHROW(tscTest.Correction(timeOfInterest));

   // IRUT corrections
   tscTest = buildObject("IRUT");
   timeOfInterest.setTimeSystem(TimeSystem::IRN);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::GPS); // Anything but IRN UTC
   TUTHROW(tscTest.Correction(timeOfInterest));

   // IRGP corrections
   tscTest = buildObject("IRGP");
   timeOfInterest.setTimeSystem(TimeSystem::IRN);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::GPS);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::UTC); // Anything but IRN GPS
   TUTHROW(tscTest.Correction(timeOfInterest));

   // GLUT corrections
      // GLONASS is a little different in that the A1 term is not used
   A1 = 0;
   forwardResult = A0 + A1 * deltaT;
   forwardResult = forwardResult * -1.0;   // This is a CORRECTION, not an error
   tscTest = buildObject("GLUT");
   timeOfInterest.setTimeSystem(TimeSystem::GLO);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::UTC);
   corrVal = tscTest.Correction(timeOfInterest);
   TUASSERTFEPS(-forwardResult, corrVal, eps);

   timeOfInterest.setTimeSystem(TimeSystem::GPS); // Anything but GLO or UTC
   TUTHROW(tscTest.Correction(timeOfInterest));

   // Default 
   TimeSystemCorrection defaultTSC;
   TUTHROW(defaultTSC.Correction(timeOfInterest));

   TURETURN();
}

unsigned TimeSystemCorr_T ::
convertTimeSystemToCorrTypeTest()
{
   TUDEF("TimeSystemCorr", "convertTimeSystemToCorrTypeTest");

   TimeSystemCorrection tscTest = buildObject("GPUT");

   // BDT UTC => BDUT
   TimeSystemCorrection::CorrType ctBDUT;
   TUASSERTE(bool, true, tscTest.convertTimeSystemToCorrType(TimeSystem::BDT, TimeSystem::UTC, ctBDUT));
   TUASSERT(ctBDUT == TimeSystemCorrection::BDUT);
   
   // BDT GPS => BDGP
   TimeSystemCorrection::CorrType ctBDGP;
   TUASSERTE(bool, true, tscTest.convertTimeSystemToCorrType(TimeSystem::BDT, TimeSystem::GPS, ctBDGP));
   TUASSERT(ctBDGP == TimeSystemCorrection::BDGP);

   // BDT uses invalid system of QZS gets set to Unknown
   TimeSystemCorrection::CorrType ctBDTInvalid;
   TUASSERTE(bool, false, tscTest.convertTimeSystemToCorrType(TimeSystem::BDT, TimeSystem::QZS, ctBDTInvalid));
   TUASSERT(ctBDTInvalid == TimeSystemCorrection::Unknown);

   // GAL UTC => GAUT
   TimeSystemCorrection::CorrType ctGAUT;
   TUASSERTE(bool, true, tscTest.convertTimeSystemToCorrType(TimeSystem::GAL, TimeSystem::UTC, ctGAUT));
   TUASSERT(ctGAUT == TimeSystemCorrection::GAUT);

   // GAL GPS => GAGP
   TimeSystemCorrection::CorrType ctGAGP;
   TUASSERTE(bool, true, tscTest.convertTimeSystemToCorrType(TimeSystem::GAL, TimeSystem::GPS, ctGAGP));
   TUASSERT(ctGAGP == TimeSystemCorrection::GAGP);

   // GAL uses invalid system of QZS gets set to Unknown
   TimeSystemCorrection::CorrType ctGALInvalid;
   TUASSERTE(bool, false, tscTest.convertTimeSystemToCorrType(TimeSystem::GAL, TimeSystem::QZS, ctGALInvalid));
   TUASSERT(ctGALInvalid == TimeSystemCorrection::Unknown);
   
   // GLO UTC => GLUT
   TimeSystemCorrection::CorrType ctGLUT;
   TUASSERTE(bool, true, tscTest.convertTimeSystemToCorrType(TimeSystem::GLO, TimeSystem::UTC, ctGLUT));
   TUASSERT(ctGLUT == TimeSystemCorrection::GLUT);

   // GLO GPS => GLGP
   TimeSystemCorrection::CorrType ctGLGP;
   TUASSERTE(bool, true, tscTest.convertTimeSystemToCorrType(TimeSystem::GLO, TimeSystem::GPS, ctGLGP));
   TUASSERT(ctGLGP == TimeSystemCorrection::GLGP);

   // GLO uses invalid system of QZS gets set to Unknown
   TimeSystemCorrection::CorrType ctGLOInvalid;
   TUASSERTE(bool, false, tscTest.convertTimeSystemToCorrType(TimeSystem::GLO, TimeSystem::QZS, ctGLOInvalid));
   TUASSERT(ctGLOInvalid == TimeSystemCorrection::Unknown);

   // GPS UTC => GPUT
   TimeSystemCorrection::CorrType ctGPUT;
   TUASSERTE(bool, true, tscTest.convertTimeSystemToCorrType(TimeSystem::GPS, TimeSystem::UTC, ctGPUT));
   TUASSERT(ctGPUT == TimeSystemCorrection::GPUT);

   // GPS GAL => GPGA
   TimeSystemCorrection::CorrType ctGPGA;
   TUASSERTE(bool, true, tscTest.convertTimeSystemToCorrType(TimeSystem::GPS, TimeSystem::GAL, ctGPGA));
   TUASSERT(ctGPGA == TimeSystemCorrection::GPGA);

   // GLO uses invalid system of QZS gets set to Unknown
   TimeSystemCorrection::CorrType ctGPSInvalid;
   TUASSERTE(bool, false, tscTest.convertTimeSystemToCorrType(TimeSystem::GPS, TimeSystem::QZS, ctGPSInvalid));
   TUASSERT(ctGPSInvalid == TimeSystemCorrection::Unknown);

   // IRN UTC => IRUT
   TimeSystemCorrection::CorrType ctIRUT;
   TUASSERTE(bool, true, tscTest.convertTimeSystemToCorrType(TimeSystem::IRN, TimeSystem::UTC, ctIRUT));
   TUASSERT(ctIRUT == TimeSystemCorrection::IRUT);

   // IRN GPS => IRGP
   TimeSystemCorrection::CorrType ctIRGP;
   TUASSERTE(bool, true, tscTest.convertTimeSystemToCorrType(TimeSystem::IRN, TimeSystem::GPS, ctIRGP));
   TUASSERT(ctIRGP == TimeSystemCorrection::IRGP);

   // IRN uses invalid system of QZS gets set to Unknown
   TimeSystemCorrection::CorrType ctIRNInvalid;
   TUASSERTE(bool, false, tscTest.convertTimeSystemToCorrType(TimeSystem::IRN, TimeSystem::QZS, ctIRNInvalid));
   TUASSERT(ctIRNInvalid == TimeSystemCorrection::Unknown);

   // QZS UTC => QZUT
   TimeSystemCorrection::CorrType ctQZUT;
   TUASSERTE(bool, true, tscTest.convertTimeSystemToCorrType(TimeSystem::QZS, TimeSystem::UTC, ctQZUT));
   TUASSERT(ctQZUT == TimeSystemCorrection::QZUT);

   // QZS GPS => QZGP
   TimeSystemCorrection::CorrType ctQZGP;
   TUASSERTE(bool, true, tscTest.convertTimeSystemToCorrType(TimeSystem::QZS, TimeSystem::GPS, ctQZGP));
   TUASSERT(ctQZGP == TimeSystemCorrection::QZGP);

   // QZS uses invalid system of QZS gets set to Unknown
   TimeSystemCorrection::CorrType ctQZSInvalid;
   TUASSERTE(bool, false, tscTest.convertTimeSystemToCorrType(TimeSystem::QZS, TimeSystem::QZS, ctQZSInvalid));
   TUASSERT(ctQZSInvalid == TimeSystemCorrection::Unknown);

   // Fails when using invalid SRC TimeSystem TT
   TimeSystemCorrection::CorrType ctInvalid;
   TUASSERTE(bool, false, tscTest.convertTimeSystemToCorrType(TimeSystem::TT, TimeSystem::QZS, ctInvalid));
   TUASSERT(ctInvalid == TimeSystemCorrection::Unknown);

   TURETURN();
}

unsigned TimeSystemCorr_T ::
asStringTest()
{
   TUDEF("TimeSystemCorr", "asStringTest");

   TimeSystemCorrection tsc;
   TUASSERTE(std::string, "ERROR", tsc.asString());

   TUASSERTE(std::string, "GPS to UTC", buildObject("GPUT").asString());
   TUASSERTE(std::string, "GAL to UTC", buildObject("GAUT").asString());
   TUASSERTE(std::string, "SBAS to UTC",buildObject("SBUT").asString());
   TUASSERTE(std::string, "GLO to UTC", buildObject("GLUT").asString());
   TUASSERTE(std::string, "GPS to GAL", buildObject("GPGA").asString());
   TUASSERTE(std::string, "GPS to GAL", buildObject("GAGP").asString());
   TUASSERTE(std::string, "GLO to GPS", buildObject("GLGP").asString());
   TUASSERTE(std::string, "QZS to GPS", buildObject("QZGP").asString());
   TUASSERTE(std::string, "QZS to UTC", buildObject("QZUT").asString());
   TUASSERTE(std::string, "BDT to UTC", buildObject("BDUT").asString());
   TUASSERTE(std::string, "BDT to GPS", buildObject("BDGP").asString());
   TUASSERTE(std::string, "IRN to UTC", buildObject("IRUT").asString());
   TUASSERTE(std::string, "IRN to GPS", buildObject("IRGP").asString());

   TURETURN();
}

unsigned TimeSystemCorr_T ::
asString4Test()
{
   TUDEF("TimeSystemCorr", "asString4Test");

   TimeSystemCorrection tsc;
   TUASSERTE(std::string, "ERROR", tsc.asString4());

   TUASSERTE(std::string, "GPUT", buildObject("GPUT").asString4());
   TUASSERTE(std::string, "GAUT", buildObject("GAUT").asString4());
   TUASSERTE(std::string, "SBUT", buildObject("SBUT").asString4());
   TUASSERTE(std::string, "GLUT", buildObject("GLUT").asString4());
   TUASSERTE(std::string, "GPGA", buildObject("GPGA").asString4());
   TUASSERTE(std::string, "GLGP", buildObject("GLGP").asString4());
   TUASSERTE(std::string, "QZGP", buildObject("QZGP").asString4());
   TUASSERTE(std::string, "QZUT", buildObject("QZUT").asString4());
   TUASSERTE(std::string, "BDUT", buildObject("BDUT").asString4());
   TUASSERTE(std::string, "BDGP", buildObject("BDGP").asString4());
   TUASSERTE(std::string, "IRUT", buildObject("IRUT").asString4());
   TUASSERTE(std::string, "IRGP", buildObject("IRGP").asString4());

   TURETURN();
}

unsigned TimeSystemCorr_T ::
dumpTest()
{
   TUDEF("TimeSystemCorr", "dumpTest");

   TimeSystemCorrection tsc;
   std::ostringstream oss;
   tsc.dump(oss);
   std::string expectedDump = "Time system correction for ERROR: ERROR";
   TUASSERTE(std::string, expectedDump, oss.str());

   std::ostringstream GPUTstream;
   std::string expectedGPUTDumpOutput = "Time system correction for GPUT: GPS to UTC, A0 = 1.000000000000e-07, A1 = 0.000000000000e+00, RefTime = week/sow 1878/0";
   buildObject("GPUT").dump(GPUTstream);
   TUASSERTE(std::string, expectedGPUTDumpOutput, GPUTstream.str());

   std::ostringstream GAUTstream;
   std::string expectedGAUTDumpOutput = "Time system correction for GAUT: GAL to UTC, A0 = 1.000000000000e-07, A1 = 0.000000000000e+00, RefTime = week/sow 1878/0";
   buildObject("GAUT").dump(GAUTstream);
   TUASSERTE(std::string, expectedGAUTDumpOutput, GAUTstream.str());
    
   std::ostringstream SBUTstream;
   std::string expectedSBUTDumpOutput = "Time system correction for SBUT: SBAS to UTC, A0 = 1.000000000000e-07, A1 = 0.000000000000e+00, RefTime = week/sow 1878/0, provider , UTC ID = 0";
   buildObject("SBUT").dump(SBUTstream);
   TUASSERTE(std::string, expectedSBUTDumpOutput, SBUTstream.str());
    
   std::ostringstream GLUTstream;
   std::string expectedGLUTDumpOutput = "Time system correction for GLUT: GLO to UTC, -TauC = 1.000000000000e-07, RefTime = week/sow 1878/0";
   buildObject("GLUT").dump(GLUTstream);
   TUASSERTE(std::string, expectedGLUTDumpOutput, GLUTstream.str());
    
   std::ostringstream GPGAstream;
   std::string expectedGPGADumpOutput = "Time system correction for GPGA: GPS to GAL, A0G = 1.000000000000e-07, A1G = 0.000000000000e+00, RefTime = week/sow 1878/0";
   buildObject("GPGA").dump(GPGAstream);
   TUASSERTE(std::string, expectedGPGADumpOutput, GPGAstream.str());
    
   std::ostringstream GLGPstream;
   std::string expectedGLGPDumpOutput = "Time system correction for GLGP: GLO to GPS, TauGPS = 1.000000000000e-07 sec, RefTime = yr/mon/day 2016/1/3";
   buildObject("GLGP").dump(GLGPstream);
   TUASSERTE(std::string, expectedGLGPDumpOutput, GLGPstream.str());
    
   std::ostringstream QZGPstream;
   std::string expectedQZGPDumpOutput = "Time system correction for QZGP: QZS to GPS, A0 = 1.000000000000e-07, A1 = 0.000000000000e+00, RefTime = week/sow 1878/0";
   buildObject("QZGP").dump(QZGPstream);
   TUASSERTE(std::string, expectedQZGPDumpOutput, QZGPstream.str());
    
   std::ostringstream QZUTstream;
   std::string expectedQZUTDumpOutput = "Time system correction for QZUT: QZS to UTC, A0 = 1.000000000000e-07, A1 = 0.000000000000e+00, RefTime = week/sow 1878/0";
   buildObject("QZUT").dump(QZUTstream);
   TUASSERTE(std::string, expectedQZUTDumpOutput, QZUTstream.str());
    
   std::ostringstream BDUTstream;
   std::string expectedBDUTDumpOutput = "Time system correction for BDUT: BDT to UTC, A0 = 1.000000000000e-07, A1 = 0.000000000000e+00, RefTime = week/sow 522/0";
   buildObject("BDUT").dump(BDUTstream);
   TUASSERTE(std::string, expectedBDUTDumpOutput, BDUTstream.str());
    
   std::ostringstream BDGPstream;
   std::string expectedBDGPDumpOutput = "Time system correction for BDGP: BDT to GPS, A0 = 1.000000000000e-07, A1 = 0.000000000000e+00, RefTime = week/sow 522/0";
   buildObject("BDGP").dump(BDGPstream);
   TUASSERTE(std::string, expectedBDGPDumpOutput, BDGPstream.str());
    
   std::ostringstream IRUTstream;
   std::string expectedIRUTDumpOutput = "Time system correction for IRUT: IRN to UTC, A0 = 1.000000000000e-07, A1 = 0.000000000000e+00, RefTime = week/sow 1878/0";
   buildObject("IRUT").dump(IRUTstream);
   TUASSERTE(std::string, expectedIRUTDumpOutput, IRUTstream.str());
    
   std::ostringstream IRGPstream;
   std::string expectedIRGPDumpOutput = "Time system correction for IRGP: IRN to GPS, A0 = 1.000000000000e-07, A1 = 0.000000000000e+00, RefTime = week/sow 1878/0";
   buildObject("IRGP").dump(IRGPstream);
   TUASSERTE(std::string, expectedIRGPDumpOutput, IRGPstream.str());

   TURETURN();
}

unsigned TimeSystemCorr_T ::
isConverterForTest()
{
   TUDEF("TimeSystemCorr", "isConverterForTest");

   TimeSystemCorrection tsc;
   // Throw error for coverting identical Time Systems
   TUTHROW(tsc.isConverterFor(TimeSystem::GPS, TimeSystem::GPS));
   
   // Throw error for TimeSystem 1 being Unknown
   TUTHROW(tsc.isConverterFor(TimeSystem::Unknown, TimeSystem::GPS));
   
   // Throw error for TimeSystem 2 being Unknown
   TUTHROW(tsc.isConverterFor(TimeSystem::GPS, TimeSystem::Unknown));

   // Return false when neither TimeSystems match toTS or fTS
   TUASSERTE(bool, false, tsc.isConverterFor(TimeSystem::GPS, TimeSystem::UTC));

   // Works when TimeSystems are matching
   tsc.frTS = TimeSystem::GPS;
   tsc.toTS = TimeSystem::UTC;
   TUASSERTE(bool, true, tsc.isConverterFor(TimeSystem::GPS, TimeSystem::UTC));

   // Works in reverse
   TUASSERTE(bool, true, tsc.isConverterFor(TimeSystem::UTC, TimeSystem::GPS));

   TURETURN();
}

unsigned TimeSystemCorr_T ::
fixTimeSystemTest()
{
   TUDEF("TimeSystemCorr", "fixTimeSystemTest");

   TimeSystemCorrection tsc;
   TUASSERT(tsc.refTime.getTimeSystem() == TimeSystem::Any);
   tsc.fixTimeSystem();
   TUASSERT(tsc.refTime.getTimeSystem() == TimeSystem::Unknown);

   TimeSystemCorrection tscGPUT = buildObject("GPUT");
   TUASSERT(tscGPUT.refTime.getTimeSystem() == TimeSystem::Unknown);
   tscGPUT.fixTimeSystem();
   TUASSERT(tscGPUT.refTime.getTimeSystem() == TimeSystem::GPS);

   TimeSystemCorrection tscGLUT = buildObject("GLUT");
   TUASSERT(tscGLUT.refTime.getTimeSystem() == TimeSystem::Unknown);
   tscGLUT.fixTimeSystem();
   TUASSERT(tscGLUT.refTime.getTimeSystem() == TimeSystem::GLO);

   TimeSystemCorrection tscGAUT = buildObject("GAUT");
   TUASSERT(tscGAUT.refTime.getTimeSystem() == TimeSystem::Unknown);
   tscGAUT.fixTimeSystem();
   TUASSERT(tscGAUT.refTime.getTimeSystem() == TimeSystem::GAL);

   TimeSystemCorrection tscBDUT = buildObject("BDUT");
   TUASSERT(tscBDUT.refTime.getTimeSystem() == TimeSystem::Unknown);
   tscBDUT.fixTimeSystem();
   TUASSERT(tscBDUT.refTime.getTimeSystem() == TimeSystem::BDT);

   TimeSystemCorrection tscQZUT = buildObject("QZUT");
   TUASSERT(tscQZUT.refTime.getTimeSystem() == TimeSystem::Unknown);
   tscQZUT.fixTimeSystem();
   TUASSERT(tscQZUT.refTime.getTimeSystem() == TimeSystem::QZS);

   TimeSystemCorrection tscIRUT = buildObject("IRUT");
   TUASSERT(tscIRUT.refTime.getTimeSystem() == TimeSystem::Unknown);
   tscIRUT.fixTimeSystem();
   TUASSERT(tscIRUT.refTime.getTimeSystem() == TimeSystem::IRN);

   TimeSystemCorrection tscSBUT = buildObject("SBUT");
   TUASSERT(tscSBUT.refTime.getTimeSystem() == TimeSystem::Unknown);
   tscSBUT.fixTimeSystem();
   TUASSERT(tscSBUT.refTime.getTimeSystem() == TimeSystem::Unknown);

   TimeSystemCorrection tscGLGP = buildObject("GLGP");
   TUASSERT(tscGLGP.refTime.getTimeSystem() == TimeSystem::Unknown);
   tscGLGP.fixTimeSystem();
   TUASSERT(tscGLGP.refTime.getTimeSystem() == TimeSystem::GLO);

   TimeSystemCorrection tscGAGP = buildObject("GAGP");
   // Manual type change here becuase when initilizing a TimeSystemCorrection by string, both GAGP and GPGA map to GPGA.
   tscGAGP.type = TimeSystemCorrection::GAGP;
   TUASSERT(tscGAGP.refTime.getTimeSystem() == TimeSystem::Unknown);
   tscGAGP.fixTimeSystem();
   TUASSERT(tscGAGP.refTime.getTimeSystem() == TimeSystem::GAL); // should be GAL

   TimeSystemCorrection tscQZGP = buildObject("QZGP");
   TUASSERT(tscQZGP.refTime.getTimeSystem() == TimeSystem::Unknown);
   tscQZGP.fixTimeSystem();
   TUASSERT(tscQZGP.refTime.getTimeSystem() == TimeSystem::QZS);

   TimeSystemCorrection tscIRGP = buildObject("IRGP");
   TUASSERT(tscIRGP.refTime.getTimeSystem() == TimeSystem::Unknown);
   tscIRGP.fixTimeSystem();
   TUASSERT(tscIRGP.refTime.getTimeSystem() == TimeSystem::IRN);
   
   TimeSystemCorrection tscGPGA = buildObject("GPGA");
   TUASSERT(tscGPGA.refTime.getTimeSystem() == TimeSystem::Unknown);
   tscGPGA.fixTimeSystem();
   TUASSERT(tscGPGA.refTime.getTimeSystem() == TimeSystem::GPS);

   TimeSystemCorrection tscBDGP = buildObject("BDGP");
   TUASSERT(tscBDGP.refTime.getTimeSystem() == TimeSystem::Unknown);
   tscBDGP.fixTimeSystem();
   TUASSERT(tscBDGP.refTime.getTimeSystem() == TimeSystem::BDT);
   
   TURETURN();
}

TimeSystemCorrection TimeSystemCorr_T ::
buildObject(const std::string& str)
{
   TimeSystemCorrection tsc(str);
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
   errorTotal += testClass.convertTimeSystemToCorrTypeTest();
   errorTotal += testClass.asStringTest();
   errorTotal += testClass.asString4Test();
   errorTotal += testClass.dumpTest();
   errorTotal += testClass.isConverterForTest();
   errorTotal += testClass.fixTimeSystemTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal; //Return the total number of errors
}
