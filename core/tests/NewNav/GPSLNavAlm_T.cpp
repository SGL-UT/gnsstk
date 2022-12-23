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
#include "GPSLNavAlm.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"
#include "CivilTime.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GPSLNavAlm_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned getXvtTest();
};


unsigned GPSLNavAlm_T ::
constructorTest()
{
   TUDEF("GPSLNavAlm", "GPSLNavAlm");
   gnsstk::GPSLNavAlm uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Almanac,
             uut.signal.messageType);
   TUASSERTE(uint8_t, 0xff, uut.healthBits);
   TUASSERTFE(0.0, uut.deltai);
   TUASSERTFE(0.0, uut.toa);
   TURETURN();
}


unsigned GPSLNavAlm_T ::
getUserTimeTest()
{
   TUDEF("GPSLNavAlm", "getUserTime");
   gnsstk::GPSLNavAlm uut;
   uut.timeStamp = gnsstk::GPSWeekSecond(2100,135.0);
   gnsstk::CommonTime exp(gnsstk::GPSWeekSecond(2100,135.0));
      // almanac = 1 subframes * 6 seconds
   exp = exp + 6.0;
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GPSLNavAlm_T ::
fixFitTest()
{
   TUDEF("GPSLNavAlm", "fixFit");
   gnsstk::CommonTime toa = gnsstk::GPSWeekSecond(2100,135.0);
   gnsstk::CommonTime expBegin = toa - (70.0 * 3600.0);
   gnsstk::CommonTime expEnd   = toa + (74.0 * 3600.0);
   gnsstk::GPSLNavAlm uut;
   uut.Toe = toa;
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, expBegin, uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, expEnd, uut.endFit);
   TURETURN();
}


unsigned GPSLNavAlm_T ::
getXvtTest()
{
   TUDEF("GPSLNavAlm", "getXvt");
   gnsstk::GPSLNavAlm uut;
   gnsstk::Xvt xvt;
   gnsstk::RefFrame expRF(gnsstk::RefFrameRlz::WGS84G1762);
   uut.xmitTime = gnsstk::GPSWeekSecond(1854, .720000000000e+04);
   uut.Toe = gnsstk::GPSWeekSecond(1854, .143840000000e+05);
   uut.Toc = gnsstk::CivilTime(2015,7,19,3,59,44.0,gnsstk::TimeSystem::GPS);
   uut.health = gnsstk::SVHealth::Healthy;
   gnsstk::CivilTime civ(2015,7,19,2,0,35.0,gnsstk::TimeSystem::GPS);
   TUASSERT(uut.getXvt(civ, xvt));
   TUASSERTE(gnsstk::Xvt::HealthStatus, gnsstk::Xvt::Healthy, xvt.health);
   TUASSERTE(gnsstk::RefFrame,expRF,xvt.frame);
   TURETURN();
}


int main()
{
   GPSLNavAlm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.getXvtTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
