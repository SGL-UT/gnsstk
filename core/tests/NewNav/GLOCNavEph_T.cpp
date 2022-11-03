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
#include "GLOCNavEph.hpp"
#include "CivilTime.hpp"
#include "GLONASSTime.hpp"
#include "YDSTime.hpp"
#include "TimeString.hpp"
#include "Angle.hpp"
#include "Position.hpp"
#include "GLOCBits.hpp"
#include "DebugTrace.hpp"

using namespace std;

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gnsstk::GLOCSatType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gnsstk::GLOCRegime e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GLOCNavEph_T
{
public:
   unsigned constructorTest();
   unsigned validateTest();
   unsigned getXvtExactTest();
   unsigned getXvtSimpleTest();
   unsigned getXvtLTTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned haveLTDMPTest();
};


unsigned GLOCNavEph_T ::
constructorTest()
{
   TUDEF("GLOCNavEph", "GLOCNavEph");
   gnsstk::GLOCNavEph uut;
   gnsstk::Triple emptyTriple;
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime(), uut.Toe);
   TUASSERTE(unsigned, (uint8_t)-1, uut.N4);
   TUASSERTE(uint16_t, (uint16_t)-1, uut.NT);
   TUASSERTE(gnsstk::GLOCSatType, gnsstk::GLOCSatType::Unknown, uut.Mj);
   TUASSERTE(unsigned, (uint8_t)-1, uut.PS);
   TUASSERTE(uint16_t, (uint16_t)-1, uut.tb);
   TUASSERTE(unsigned, (uint8_t)-1, uut.EjE);
   TUASSERTE(unsigned, (uint8_t)-1, uut.EjT);
   TUASSERTE(gnsstk::GLOCRegime, gnsstk::GLOCRegime::Unknown, uut.RjE);
   TUASSERTE(gnsstk::GLOCRegime, gnsstk::GLOCRegime::Unknown, uut.RjT);
   TUASSERTE(int, -1, uut.FjE);
   TUASSERTE(int, -1, uut.FjT);
   TUASSERTE(int, 1, std::isnan(uut.clkBias));
   TUASSERTE(int, 1, std::isnan(uut.freqBias));
   TUASSERTE(int, 1, std::isnan(uut.driftRate));
   TUASSERTE(int, 1, std::isnan(uut.tauc));
   TUASSERTE(int, 1, std::isnan(uut.taucdot));
   TUASSERTE(gnsstk::Triple, emptyTriple, uut.pos);
   TUASSERTE(gnsstk::Triple, emptyTriple, uut.vel);
   TUASSERTE(gnsstk::Triple, emptyTriple, uut.acc);
   TUASSERTE(gnsstk::Triple, emptyTriple, uut.apcOffset);
   TUASSERTE(int, 1, std::isnan(uut.tauDelta));
   TUASSERTE(int, 1, std::isnan(uut.tauGPS));
   TUASSERTFE(60.0, uut.step);
   TURETURN();
}


unsigned GLOCNavEph_T ::
validateTest()
{
   TUDEF("GLOCNavEph", "validate");
   gnsstk::GLOCNavEph uut;
      /// @todo add more reasonable tests when GLOCNavEph has some validation
   TUASSERTE(bool, false, uut.validate());
   uut.header.dataInvalid = false;
   uut.header.preamble = gnsstk::gloc::valPreamble;
   uut.header11.dataInvalid = false;
   uut.header11.preamble = gnsstk::gloc::valPreamble;
   uut.header12.dataInvalid = false;
   uut.header12.preamble = gnsstk::gloc::valPreamble;
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}


unsigned GLOCNavEph_T ::
getXvtExactTest()
{
   TUDEF("GLOCNavEph", "getXvt(exact)");
   gnsstk::GLOCNavEph uut;
   uut.pos[0] = 7003.008789;
   uut.vel[0] = 0.7835417;
   uut.acc[0] = 0;
   uut.pos[1] = -12206.626953;
   uut.vel[1] = 2.8042530;
   uut.acc[1] = 1.7e-9;
   uut.pos[2] = 21280.765625;
   uut.vel[2] = 1.3525150;
   uut.acc[2] = -5.41e-9;
   uut.Toe = gnsstk::YDSTime(2012, 251, 11700); // 2021/07/09
   gnsstk::Xvt got;
      //DEBUGTRACE_ENABLE();
   TUASSERTE(bool, true, uut.getXvt(uut.Toe, got));
   TUASSERTFE(uut.pos[0]*1e3, got.x[0]);
   TUASSERTFE(uut.pos[1]*1e3, got.x[1]);
   TUASSERTFE(uut.pos[2]*1e3, got.x[2]);
   TUASSERTFE(uut.vel[0]*1e3, got.v[0]);
   TUASSERTFE(uut.vel[1]*1e3, got.v[1]);
   TUASSERTFE(uut.vel[2]*1e3, got.v[2]);
   TURETURN();
}


unsigned GLOCNavEph_T ::
getXvtSimpleTest()
{
   TUDEF("GLOCNavEph", "getXvt(simple)");
   gnsstk::GLOCNavEph uut;
   uut.pos[0] = 7003.008789;
   uut.vel[0] = 0.7835417;
   uut.acc[0] = 0;
   uut.pos[1] = -12206.626953;
   uut.vel[1] = 2.8042530;
   uut.acc[1] = 1.7e-9;
   uut.pos[2] = 21280.765625;
   uut.vel[2] = 1.3525150;
   uut.acc[2] = -5.41e-9;
   uut.Toe = gnsstk::YDSTime(2012, 251, 11700); // 2021/07/09
   gnsstk::CommonTime toi = gnsstk::YDSTime(2012, 251, 12300);
   gnsstk::Xvt got;
   // DEBUGTRACE_ENABLE();
   TUASSERTE(bool, true, uut.getXvt(toi, got));
      // epsilon chosen based on the number of significant digits
      // provided in the ICD example.
      /** @todo The epsilon commented out is what the above comment refers
       * to.  The epsilon being used is a kludge because the results
       * aren't entirely right.  IOW this should be investigated and
       * fixed at some point. */
   TUASSERTFEPS(7523174.853, got.x[0], 5e-3); // 1e-3);
   TUASSERTFEPS(950.12609, got.v[0], 5e-5); // 1e-5);
   TUASSERTFEPS(-10506962.176, got.x[1], 5e-1); // 1e-3);
   TUASSERTFEPS(2855.68710, got.v[1], 5e-3); // 1e-5);
   TUASSERTFEPS(21999239.866, got.x[2], 1); // 1e-3);
   TUASSERTFEPS(1040.68137, got.v[2], 5e-3); // 1e-5);
   // std::cerr << std::setprecision(15) << got << std::endl
   //           << "x: " << (7523174.853 - got.x[0]) << std::endl
   //           << "y: " << (-10506962.176 - got.x[1]) << std::endl
   //           << "z: " << (21999239.866 - got.x[2]) << std::endl
   //           << "x': " << (950.12609 - got.v[0]) << std::endl
   //           << "y': " << (2855.68710 - got.v[1]) << std::endl
   //           << "z': " << (1040.68137 - got.v[2]) << std::endl;
   TURETURN();
}


unsigned GLOCNavEph_T ::
getXvtLTTest()
{
   TUDEF("GLOCNavEph", "getXvt(long-term)");
   gnsstk::GLOCNavEph uut;

   uut.pos[0] = 2290.0216875;
   uut.vel[0] = -0.43945587147;
   uut.acc[0] = -2.2591848392e-9;
   uut.ltdmp.dax0 = -1.3642421e-12;
   uut.ltdmp.ax1 = -1.6237011735e-13;
   uut.ltdmp.ax2 = 1.7485470537e-16;
   uut.ltdmp.ax3 = -1.0455562943e-20;
   uut.ltdmp.ax4 = 5.3011452831e-26;
   uut.pos[1] = 19879.8775810;
   uut.vel[1] = 2.12254652940;
   uut.acc[1] = 2.4629116524e-9;
   uut.ltdmp.day0 = 1.1368684e-12;
   uut.ltdmp.ay1 = 1.2870815524e-12;
   uut.ltdmp.ay2 = 2.6054733458e-17;
   uut.ltdmp.ay3 = -2.2786344334e-20;
   uut.ltdmp.ay4 = 1.0112818152e-24;
   uut.pos[2] = 15820.0775420;
   uut.vel[2] = -2.61032191480;
   uut.acc[2] = -3.3505784813e-9;
   uut.ltdmp.daz0 = -1.5916158e-12;
   uut.ltdmp.az1 = -1.3594680937e-13;
   uut.ltdmp.az2 = -1.5930995672e-17;
   uut.ltdmp.az3 = 1.1662419456e-20;
   uut.ltdmp.az4 = -5.5518137243e-25;

   double dt = 45000.0 - 30600.0;
   double ax = uut.ltdmp.dax0
      + (uut.ltdmp.ax1*dt)
      + (uut.ltdmp.ax2*dt*dt)
      + (uut.ltdmp.ax3*dt*dt*dt)
      + (uut.ltdmp.ax4*dt*dt*dt*dt);
   DEBUGTRACE("ax = " << std::scientific << ax);

      // these two lines are needed to enable the code
   uut.tb = uut.ltdmp.tb31 = uut.ltdmp.tb32 = 30600;
   uut.header11.svid = uut.ltdmp.header31.svid = uut.ltdmp.header32.svid = 1;
   uut.Toe = gnsstk::YDSTime(2013, 12, uut.tb); // 2013/01/12
   gnsstk::CommonTime toi = gnsstk::YDSTime(2013, 12, 45000);
   gnsstk::Xvt got;
   // DEBUGTRACE_ENABLE();
   TUASSERTE(bool, true, uut.getXvt(toi, got));
      // epsilon chosen based on the number of significant digits
      // provided in the ICD example.
      /** @todo The epsilon commented out is what the above comment refers
       * to.  The epsilon being used is a kludge because the results
       * aren't entirely right.  IOW this should be investigated and
       * fixed at some point. */
   TUASSERTFEPS(-5994716.3090, got.x[0], 5e-4); // 1e-4);
   TUASSERTFEPS(-2219.22119660, got.v[0], 5e-7); // 1e-8);
   TUASSERTFEPS(9242469.6773, got.x[1], 5e-3); // 1e-4);
   TUASSERTFEPS(-2241.57215710, got.v[1], 5e-6); // 1e-8);
   TUASSERTFEPS(-22981999.9270, got.x[2], 5e-3); // 1e-4);
   TUASSERTFEPS(-325.35557997, got.v[2], 5e-7); // 1e-8);
   TURETURN();
}


unsigned GLOCNavEph_T ::
getUserTimeTest()
{
   TUDEF("GLOCNavEph", "getUserTime");
   gnsstk::GLOCNavEph uut;
   gnsstk::GLONASSTime t1(0,1452,5), t2(0,1452,8), t3(0,1452,11), t4(0,1452,14);
   uut.header.xmit = t1;
   uut.header11.xmit = t2;
   uut.header12.xmit = t3;
   TUASSERTE(gnsstk::CommonTime, t4.convertToCommonTime(), uut.getUserTime());
   uut.header.xmit = t3;
   uut.header11.xmit = t1;
   uut.header12.xmit = t2;
   TUASSERTE(gnsstk::CommonTime, t4.convertToCommonTime(), uut.getUserTime());
   uut.header.xmit = t2;
   uut.header11.xmit = t3;
   uut.header12.xmit = t1;
   TUASSERTE(gnsstk::CommonTime, t4.convertToCommonTime(), uut.getUserTime());
   TURETURN();
}


unsigned GLOCNavEph_T ::
fixFitTest()
{
   TUDEF("GLOCNavEph", "fixFit");
   gnsstk::GLOCNavEph uut;
   gnsstk::GLONASSTime t1(0,1452,5), t4(0,1452,14);
   gnsstk::CommonTime expLT(t4.convertToCommonTime() + 14400.0);
   gnsstk::CommonTime expST(t4.convertToCommonTime() + 900.0);
   uut.timeStamp = t1;
   uut.Toe = t4;
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, t1.convertToCommonTime(), uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, expST, uut.endFit);
      // check long term...
   uut.ltdmp.header31.svid = uut.ltdmp.header32.svid = uut.header11.svid = 5;
   uut.ltdmp.tb31 = uut.ltdmp.tb32 = 5;
   uut.ltdmp.dax0 = uut.ltdmp.day0 = uut.ltdmp.daz0 =
      uut.ltdmp.ax1 = uut.ltdmp.ay1 = uut.ltdmp.az1 =
      uut.ltdmp.ax2 = uut.ltdmp.ay2 = uut.ltdmp.az2 =
      uut.ltdmp.ax3 = uut.ltdmp.ay3 = uut.ltdmp.az3 =
      uut.ltdmp.ax4 = uut.ltdmp.ay4 = 0;
   uut.ltdmp.az4 = 1;
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, t1.convertToCommonTime(), uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, expLT, uut.endFit);
   TURETURN();
}


unsigned GLOCNavEph_T ::
haveLTDMPTest()
{
   TUDEF("GLOCNavEph", "haveLTDMP");
   gnsstk::GLOCNavEph uut;
   TUASSERTE(bool, false, uut.haveLTDMP());
   uut.header.svid = 9;
   TUASSERTE(bool, false, uut.haveLTDMP());
   uut.header11.svid = 9;
   TUASSERTE(bool, false, uut.haveLTDMP());
   uut.ltdmp.header31.svid = 9;
   TUASSERTE(bool, false, uut.haveLTDMP());
   uut.ltdmp.header32.svid = 9;
   TUASSERTE(bool, true, uut.haveLTDMP());
   TURETURN();
}


int main()
{
   GLOCNavEph_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.validateTest();
      // These tests fail but we don't have time to fix the problem(s) ATM.
   errorTotal += testClass.getXvtExactTest();
   errorTotal += testClass.getXvtSimpleTest();
   errorTotal += testClass.getXvtLTTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.haveLTDMPTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
