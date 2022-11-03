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

#include "GLONASSTime.hpp"
#include "TimeTag.hpp"
#include "TimeString.hpp"
#include "CivilTime.hpp"
#include "TestUtil.hpp"
#include <iostream>
#include <fstream>


class GLONASSTime_T
{
public:
   unsigned constructorTest();
   unsigned convertTest();
   unsigned compareTest();
   unsigned resetTest();
   unsigned diffTest();
};


unsigned GLONASSTime_T ::
constructorTest()
{
   TUDEF("GLONASSTime", "GLONASSTime()");
   gnsstk::GLONASSTime uut1;
   TUASSERTE(unsigned, 0, uut1.epoch);
   TUASSERTE(unsigned, 0, uut1.day);
   TUASSERTFE(0., uut1.sod);
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GLO, uut1.getTimeSystem());
   TUCSM("GLONASSTime(...)");
   gnsstk::GLONASSTime uut2(23, 32, 1234., gnsstk::TimeSystem::UTC);
   TUASSERTE(unsigned, 23, uut2.epoch);
   TUASSERTE(unsigned, 32, uut2.day);
   TUASSERTFE(1234., uut2.sod);
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC, uut2.getTimeSystem());
   TUCSM("GLONASSTime(GLONASSTime)");
   gnsstk::GLONASSTime uut3(uut2);
   TUASSERTE(unsigned, 23, uut3.epoch);
   TUASSERTE(unsigned, 32, uut3.day);
   TUASSERTFE(1234., uut3.sod);
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC, uut3.getTimeSystem());
   TURETURN();
}


unsigned GLONASSTime_T ::
convertTest()
{
   TUDEF("GLONASSTime", "convertToCommonTime");
      /** @note this test truth data comes from ICD-GLONASS-CDMA
       * General Edition, Appendix K. */
   gnsstk::GLONASSTime uut1(5,251,0);
   gnsstk::CommonTime exp(gnsstk::CivilTime(2012, 9, 7, 0, 0, 0,
                                            gnsstk::TimeSystem::GLO));
   TUASSERTE(gnsstk::CommonTime, exp, uut1);

   TUCSM("convertFromCommonTime");
   gnsstk::GLONASSTime uut2;
   TUCATCH(uut2.convertFromCommonTime(exp));
   TUASSERTE(unsigned, 5, uut2.epoch);
   TUASSERTE(unsigned, 251, uut2.day);
   TUASSERTFE(0., uut2.sod);
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GLO, uut2.getTimeSystem());

      // round trip tests.
   TUCSM("convertFrom/ToCommonTime");
   gnsstk::GLONASSTime uut3(7, 367, 12345);
   gnsstk::CommonTime uut3ct(uut3);
   gnsstk::GLONASSTime uut3b(uut3ct);
   TUASSERTE(unsigned, uut3.epoch, uut3b.epoch);
   TUASSERTE(unsigned, uut3.day, uut3b.day);
   TUASSERTFE(uut3.sod, uut3b.sod);

   gnsstk::GLONASSTime uut4(7, 732, 12345);
   gnsstk::CommonTime uut4ct(uut4);
   gnsstk::GLONASSTime uut4b(uut4ct);
   TUASSERTE(unsigned, uut4.epoch, uut4b.epoch);
   TUASSERTE(unsigned, uut4.day, uut4b.day);
   TUASSERTFE(uut4.sod, uut4b.sod);

   gnsstk::GLONASSTime uut5(7, 1097, 12345);
   gnsstk::CommonTime uut5ct(uut5);
   gnsstk::GLONASSTime uut5b(uut5ct);
   TUASSERTE(unsigned, uut5.epoch, uut5b.epoch);
   TUASSERTE(unsigned, uut5.day, uut5b.day);
   TUASSERTFE(uut5.sod, uut5b.sod);

      // make sure it works for the year 2100.
   gnsstk::GLONASSTime uut6(27, 3, 12345);
   gnsstk::CommonTime uut6ct(uut6);
   gnsstk::GLONASSTime uut6b(uut6ct);
   TUASSERTE(unsigned, uut6.epoch, uut6b.epoch);
   TUASSERTE(unsigned, uut6.day, uut6b.day);
   TUASSERTFE(uut6.sod, uut6b.sod);

   gnsstk::GLONASSTime uut7(27, 367, 12345);
   gnsstk::CommonTime uut7ct(uut7);
   gnsstk::GLONASSTime uut7b(uut7ct);
   TUASSERTE(unsigned, uut7.epoch, uut7b.epoch);
   TUASSERTE(unsigned, uut7.day, uut7b.day);
   TUASSERTFE(uut7.sod, uut7b.sod);

   gnsstk::GLONASSTime uut8(27, 731, 12345);
   gnsstk::CommonTime uut8ct(uut8);
   gnsstk::GLONASSTime uut8b(uut8ct);
   TUASSERTE(unsigned, uut8.epoch, uut8b.epoch);
   TUASSERTE(unsigned, uut8.day, uut8b.day);
   TUASSERTFE(uut8.sod, uut8b.sod);

   gnsstk::GLONASSTime uut9(27, 1096, 12345);
   gnsstk::CommonTime uut9ct(uut9);
   gnsstk::GLONASSTime uut9b(uut9ct);
   TUASSERTE(unsigned, uut9.epoch, uut9b.epoch);
   TUASSERTE(unsigned, uut9.day, uut9b.day);
   TUASSERTFE(uut9.sod, uut9b.sod);
   TURETURN();
}


unsigned GLONASSTime_T ::
compareTest()
{
   TUDEF("GLONASSTime", "operator==");
   gnsstk::GLONASSTime uut1(5, 251, 60., gnsstk::TimeSystem::GLO);
   gnsstk::GLONASSTime uut2(5, 251, 60., gnsstk::TimeSystem::GLO);
   gnsstk::GLONASSTime uut3(4, 251, 60., gnsstk::TimeSystem::GLO);
   gnsstk::GLONASSTime uut4(6, 251, 60., gnsstk::TimeSystem::GLO);
   gnsstk::GLONASSTime uut5(5, 250, 60., gnsstk::TimeSystem::GLO);
   gnsstk::GLONASSTime uut6(5, 252, 60., gnsstk::TimeSystem::GLO);
   gnsstk::GLONASSTime uut7(5, 251, 59., gnsstk::TimeSystem::GLO);
   gnsstk::GLONASSTime uut8(5, 251, 61., gnsstk::TimeSystem::GLO);

   TUASSERTE(bool, true,  uut1 == uut2);
   TUASSERTE(bool, false, uut1 == uut3);
   TUASSERTE(bool, false, uut1 == uut4);
   TUASSERTE(bool, false, uut1 == uut5);
   TUASSERTE(bool, false, uut1 == uut6);
   TUASSERTE(bool, false, uut1 == uut7);
   TUASSERTE(bool, false, uut1 == uut8);

   TUCSM("operator!=");
   TUASSERTE(bool, false, uut1 != uut2);
   TUASSERTE(bool, true,  uut1 != uut3);
   TUASSERTE(bool, true,  uut1 != uut4);
   TUASSERTE(bool, true,  uut1 != uut5);
   TUASSERTE(bool, true,  uut1 != uut6);
   TUASSERTE(bool, true,  uut1 != uut7);
   TUASSERTE(bool, true,  uut1 != uut8);

   TUCSM("operator<");
   TUASSERTE(bool, false, uut1 < uut2);
   TUASSERTE(bool, false, uut1 < uut3);
   TUASSERTE(bool, true,  uut1 < uut4);
   TUASSERTE(bool, false, uut1 < uut5);
   TUASSERTE(bool, true,  uut1 < uut6);
   TUASSERTE(bool, false, uut1 < uut7);
   TUASSERTE(bool, true,  uut1 < uut8);

   TUCSM("operator>");
   TUASSERTE(bool, false, uut1 > uut2);
   TUASSERTE(bool, true,  uut1 > uut3);
   TUASSERTE(bool, false, uut1 > uut4);
   TUASSERTE(bool, true,  uut1 > uut5);
   TUASSERTE(bool, false, uut1 > uut6);
   TUASSERTE(bool, true,  uut1 > uut7);
   TUASSERTE(bool, false, uut1 > uut8);

   TUCSM("operator<=");
   TUASSERTE(bool, true,  uut1 <= uut2);
   TUASSERTE(bool, false, uut1 <= uut3);
   TUASSERTE(bool, true,  uut1 <= uut4);
   TUASSERTE(bool, false, uut1 <= uut5);
   TUASSERTE(bool, true,  uut1 <= uut6);
   TUASSERTE(bool, false, uut1 <= uut7);
   TUASSERTE(bool, true,  uut1 <= uut8);

   TUCSM("operator>=");
   TUASSERTE(bool, true,  uut1 >= uut2);
   TUASSERTE(bool, true,  uut1 >= uut3);
   TUASSERTE(bool, false, uut1 >= uut4);
   TUASSERTE(bool, true,  uut1 >= uut5);
   TUASSERTE(bool, false, uut1 >= uut6);
   TUASSERTE(bool, true,  uut1 >= uut7);
   TUASSERTE(bool, false, uut1 >= uut8);

   TURETURN();
}


unsigned GLONASSTime_T ::
resetTest()
{
   TUDEF("GLONASSTime", "reset");
   gnsstk::GLONASSTime uut(23, 32, 1234., gnsstk::TimeSystem::UTC);
   TUASSERTE(unsigned, 23, uut.epoch);
   TUASSERTE(unsigned, 32, uut.day);
   TUASSERTFE(1234., uut.sod);
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC, uut.getTimeSystem());
   TUCATCH(uut.reset());
   TUASSERTE(unsigned, 0, uut.epoch);
   TUASSERTE(unsigned, 0, uut.day);
   TUASSERTFE(0., uut.sod);
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GLO, uut.getTimeSystem());
   TURETURN();
}


unsigned GLONASSTime_T ::
diffTest()
{
   TUDEF("GLONASSTime", "convertToCommonTime");
      // GLONASSTime doesn't have its own operator-, but we want to
      // make sure that the difference corresponds to the test data
      // in, e.g. ICD GLONASS CDMA General Description, appendix M2
   gnsstk::GLONASSTime almTime(0,1452,33571.625);
   gnsstk::GLONASSTime intTime(0,1453, 51300.0);
   gnsstk::CommonTime almTimeCT(almTime);
   gnsstk::CommonTime intTimeCT(intTime);
   double diff = intTimeCT - almTimeCT;
   TUASSERTFE(104128.375, diff);
   TURETURN();
}


int main()
{
   unsigned errorTotal = 0;
   GLONASSTime_T testClass;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.convertTest();
   errorTotal += testClass.compareTest();
   errorTotal += testClass.resetTest();
   errorTotal += testClass.diffTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
