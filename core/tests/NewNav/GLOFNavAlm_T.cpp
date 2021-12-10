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
#include "GLOFNavAlm.hpp"
#include "CivilTime.hpp"
#include "YDSTime.hpp"
#include "TimeString.hpp"
#include "GLOFNavEph.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GLOFNavAlm_T
{
public:
   unsigned constructorTest();
   unsigned validateTest();
   unsigned getXvtTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
};


unsigned GLOFNavAlm_T ::
constructorTest()
{
   TUDEF("GLOFNavAlm", "GLOFNavAlm()");
   gnsstk::GLOFNavAlm uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Almanac,
             uut.signal.messageType);
   TUASSERTE(bool, false, uut.healthBits);
   TUASSERTE(int, 1, isnan(uut.tau));
   TUASSERTE(int, 1, isnan(uut.lambda));
   TUASSERTE(int, 1, isnan(uut.deltai));
   TUASSERTE(int, 1, isnan(uut.ecc));
   TUASSERTE(int, 1, isnan(uut.omega));
   TUASSERTE(int, 1, isnan(uut.tLambda));
   TUASSERTE(int, 1, isnan(uut.deltaT));
   TUASSERTE(int, 1, isnan(uut.deltaTdot));
   TUASSERTE(int, -1, uut.freq);
   TURETURN();
}


unsigned GLOFNavAlm_T ::
validateTest()
{
   TUDEF("GLOFNavAlm", "validate()");
   gnsstk::GLOFNavAlm uut;
      /// @todo Implement some testing when the function has some meat to it.
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}


unsigned GLOFNavAlm_T ::
getXvtTest()
{
   TUDEF("GLOFNavAlm", "getXvt()");
   gnsstk::GLOFNavAlm uut;
      // 2001/09/06
   gnsstk::YDSTime toi(2001, 249, 33300, gnsstk::TimeSystem::GLO);
      // taken from ICD appendix A.3.2.3
   uut.lambda = -0.189986229 * gnsstk::PI;
   uut.tLambda = 27122.09375;
   uut.deltai = 0.011929512 * gnsstk::PI;
   uut.deltaT = -2655.76171875;
   uut.deltaTdot = 0.000549316;
   uut.ecc = 0.001482010;
   uut.omega = 0.440277100 * gnsstk::PI;
   uut.Toa = gnsstk::YDSTime(2001, 249, uut.tLambda, gnsstk::TimeSystem::GLO);
      // Normally called by fixFit, but we don't care about the fit
      // interval for this test.
   uut.setSemiMajorAxisIncl();
   gnsstk::Xvt xvt;
   TUASSERTE(bool, true, uut.getXvt(toi, xvt));
   std::cout << "xvt = " << std::setprecision(17) << xvt << std::endl;
   gnsstk::CommonTime x(gnsstk::YDSTime(2001,249,0,gnsstk::TimeSystem::GLO));
   unsigned i = 0;
   double st = 0;
   while (st <= 6.02401539573)
   {
      gnsstk::CommonTime y = x+(double)i;
      st = (gnsstk::GLOFNavEph::getSiderealTime(y)*gnsstk::PI/12.0);
      std::cout << gnsstk::printTime(y, "%Y %j %s %m %d = ")
                << std::setprecision(15) << st << std::endl;
      i += 86400;
   }
   TURETURN();
}


unsigned GLOFNavAlm_T ::
getUserTimeTest()
{
   TUDEF("GLOFNavAlm", "getUserTime()");
   gnsstk::GLOFNavAlm uut;
   uut.timeStamp = gnsstk::CivilTime(2006, 10, 01, 0, 15, 0,
                                     gnsstk::TimeSystem::GLO);
   uut.xmit2 = gnsstk::CivilTime(2006, 10, 01, 0, 11, 0,
                                     gnsstk::TimeSystem::GLO);
   gnsstk::CommonTime exp(uut.timeStamp + 2.0);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GLOFNavAlm_T ::
fixFitTest()
{
   TUDEF("GLOFNavAlm", "fixFit()");
   gnsstk::GLOFNavAlm uut;
   uut.timeStamp = gnsstk::CivilTime(2006, 10, 01, 0, 15, 0,
                                     gnsstk::TimeSystem::GLO);
   gnsstk::CommonTime expBegin(uut.timeStamp),
      expEnd(gnsstk::CommonTime::END_OF_TIME);
   expEnd.setTimeSystem(gnsstk::TimeSystem::GLO);
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, expBegin, uut.beginFit);
      // This is not a good value but it's the only one we have for now
   TUASSERTE(gnsstk::CommonTime, expEnd, uut.endFit);
   TURETURN();
}


int main()
{
   GLOFNavAlm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.validateTest();
   errorTotal += testClass.getXvtTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
