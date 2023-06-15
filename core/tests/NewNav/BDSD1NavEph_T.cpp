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
#include "BDSD1NavEph.hpp"
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

class BDSD1NavEph_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned validateTest();
   unsigned getAODTest();
   unsigned getXvtTest();
};


unsigned BDSD1NavEph_T ::
constructorTest()
{
   TUDEF("BDSD1NavEph", "BDSD1NavEph");
   gnsstk::BDSD1NavEph obj;
   TUASSERTE(uint32_t, 0, obj.pre2);
   TUASSERTE(uint32_t, 0, obj.pre3);
   TUASSERTE(uint32_t, 0, obj.rev2);
   TUASSERTE(uint32_t, 0, obj.rev3);
   TUASSERTE(uint16_t, 0, obj.sow2);
   TUASSERTE(uint16_t, 0, obj.sow3);
   TUASSERTE(bool, true, obj.satH1);
   TUASSERTE(unsigned, 0xff, obj.aodc);
   TUASSERTE(unsigned, 0xff, obj.aode);
   TUASSERTE(unsigned, 15, obj.uraIndex);
   TUASSERTE(gnsstk::CommonTime,gnsstk::CommonTime::BEGINNING_OF_TIME,obj.xmit2);
   TUASSERTE(gnsstk::CommonTime,gnsstk::CommonTime::BEGINNING_OF_TIME,obj.xmit3);
   TUASSERTE(bool, true, std::isnan(obj.tgd1));
   TUASSERTE(bool, true, std::isnan(obj.tgd2));
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Ephemeris,
             obj.signal.messageType);
   TURETURN();
}


unsigned BDSD1NavEph_T ::
getUserTimeTest()
{
   TUDEF("BDSD1NavEph", "getUserTime");
   gnsstk::BDSD1NavEph obj;
   obj.timeStamp = gnsstk::BDSWeekSecond(2100,253.0);
   obj.xmitTime = gnsstk::BDSWeekSecond(2100,135.0);
   obj.xmit2 = gnsstk::BDSWeekSecond(2100,141.0);
   obj.xmit3 = gnsstk::BDSWeekSecond(2100,147.0);
   gnsstk::CommonTime exp(gnsstk::BDSWeekSecond(2100,153.0));
   TUASSERTE(gnsstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}


unsigned BDSD1NavEph_T ::
fixFitTest()
{
   TUDEF("BDSD1NavEph", "fixFit");
   gnsstk::BDSD1NavEph obj;
   gnsstk::BDSWeekSecond beginExpWS2(826,511200), endExpWS2(827,30);
   gnsstk::CommonTime beginExp2(beginExpWS2), endExp2(endExpWS2);
   obj.Toe = obj.Toc = gnsstk::BDSWeekSecond(826,518400);
   obj.xmitTime = gnsstk::BDSWeekSecond(826,518400);
   TUCATCH(obj.fixFit());
   TUASSERTE(gnsstk::CommonTime, beginExp2, obj.beginFit);
   TUASSERTE(gnsstk::CommonTime, endExp2, obj.endFit);
      //obj.dump(std::cerr, gnsstk::OrbitDataKepler::Detail::Full);
   gnsstk::BDSWeekSecond beginExpWS3(826,547620), endExpWS3(826,634050);
   gnsstk::CommonTime beginExp3(beginExpWS3), endExp3(endExpWS3);
   obj.Toe = obj.Toc = gnsstk::BDSWeekSecond(826,547200);
   obj.xmitTime = gnsstk::BDSWeekSecond(826,547620);
   TUCATCH(obj.fixFit());
   TUASSERTE(gnsstk::CommonTime, beginExp3, obj.beginFit);
   TUASSERTE(gnsstk::CommonTime, endExp3, obj.endFit);
   TURETURN();
}


unsigned BDSD1NavEph_T ::
validateTest()
{
   TUDEF("BDSD1NavData", "validate");
   gnsstk::BDSD1NavEph obj;
   obj.fraID = 1;
   TUASSERTE(bool, true, obj.validate());
   obj.pre = 0x22c; // this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.pre = 0x712; // this is valid
   obj.pre2 = 0x22c; // this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.pre2 = 0x712; // this is valid
   obj.pre3 = 0x22c; // this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.pre3 = 0x712; // this is valid (all 3 preambles == 712)
   TUASSERTE(bool, true, obj.validate());
   TURETURN();
}


unsigned BDSD1NavEph_T ::
getAODTest()
{
   TUDEF("BDSD1NavEph", "getAOD");
   for (uint8_t i = 0; i < 32; i++)
   {
      unsigned exp = i < 25 ? i : (i-23)*24;
      TUASSERTE(unsigned, exp, gnsstk::BDSD1NavEph::getAOD(i));
   }
   TUASSERTE(unsigned, -1, gnsstk::BDSD1NavEph::getAOD(33));
   TURETURN();
}


unsigned BDSD1NavEph_T ::
getXvtTest()
{
   TUDEF("BDSD1NavEph", "getXvt");
   gnsstk::BDSD1NavEph uut;
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
   BDSD1NavEph_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.validateTest();
   errorTotal += testClass.getAODTest();
   errorTotal += testClass.getXvtTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
