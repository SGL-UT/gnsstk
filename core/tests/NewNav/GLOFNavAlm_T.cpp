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
   TUASSERTE(int, 1, isnan(uut.tEpoch));
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
      /// @todo implement getXvt for GLOFNavAlm and test it.
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
