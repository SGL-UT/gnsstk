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
#include "BDSD2NavAlm.hpp"
#include "TestUtil.hpp"
#include "BDSWeekSecond.hpp"
#include "CivilTime.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class BDSD2NavAlm_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned getXvtTest();
};


unsigned BDSD2NavAlm_T ::
constructorTest()
{
   TUDEF("BDSD2NavAlm", "BDSD2NavAlm");
   gnsstk::BDSD2NavAlm uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Almanac,
             uut.signal.messageType);
   TUASSERTE(unsigned, 0xff, uut.pnum);
   TUASSERTFE(0.0, uut.deltai);
   TUASSERTFE(0.0, uut.toa);
   TUASSERTE(uint16_t, 0xffff, uut.healthBits);
   TUASSERTE(bool, false, uut.isDefault);
   TURETURN();
}


unsigned BDSD2NavAlm_T ::
getUserTimeTest()
{
   TUDEF("BDSD2NavAlm", "getUserTime");
   gnsstk::BDSD2NavAlm uut;
   uut.timeStamp = gnsstk::BDSWeekSecond(2100,135.0);
   gnsstk::CommonTime exp(gnsstk::BDSWeekSecond(2100,135.0));
      // almanac = 1 subframes * 6 seconds
      // We don't take the timestamp of the WNa page into account here.
   exp = exp + 6.0;
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned BDSD2NavAlm_T ::
fixFitTest()
{
   TUDEF("BDSD2NavAlm", "fixFit");
   gnsstk::CommonTime expBegin = gnsstk::BDSWeekSecond(2100,135.0);
   gnsstk::CommonTime expEnd   = gnsstk::CommonTime::END_OF_TIME;
   gnsstk::BDSD2NavAlm uut;
   uut.xmitTime = gnsstk::BDSWeekSecond(2100,135.0);
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, expBegin, uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, expEnd, uut.endFit);
   TURETURN();
}


unsigned BDSD2NavAlm_T ::
getXvtTest()
{
   TUDEF("BDSD2NavAlm", "getXvt");
   gnsstk::BDSD2NavAlm uut;
   gnsstk::Xvt xvt;
   gnsstk::RefFrame expRF(gnsstk::RefFrameRlz::CGCS2000Y2008);
   uut.xmitTime = gnsstk::BDSWeekSecond(1854, .720000000000e+04);
   uut.Toe = gnsstk::BDSWeekSecond(1854, .143840000000e+05);
   uut.Toc = gnsstk::CivilTime(2015,7,19,3,59,44.0,gnsstk::TimeSystem::BDT);
   uut.health = gnsstk::SVHealth::Healthy;
   gnsstk::CivilTime civ(2015,7,19,2,0,35.0,gnsstk::TimeSystem::BDT);
   TUASSERT(uut.getXvt(civ, xvt));
   TUASSERTE(gnsstk::Xvt::HealthStatus, gnsstk::Xvt::Healthy, xvt.health);
   TUASSERTE(gnsstk::RefFrame,expRF,xvt.frame);
   TURETURN();
}


int main()
{
   BDSD2NavAlm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.getXvtTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
