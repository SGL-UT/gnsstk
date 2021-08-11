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
#include "BDSD1NavEph.hpp"
#include "TestUtil.hpp"
#include "BDSWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
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
};


unsigned BDSD1NavEph_T ::
constructorTest()
{
   TUDEF("BDSD1NavEph", "BDSD1NavEph");
   gpstk::BDSD1NavEph obj;
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
   TUASSERTE(gpstk::CommonTime,gpstk::CommonTime::BEGINNING_OF_TIME,obj.xmit2);
   TUASSERTE(gpstk::CommonTime,gpstk::CommonTime::BEGINNING_OF_TIME,obj.xmit3);
   TUASSERTE(bool, true, std::isnan(obj.tgd1));
   TUASSERTE(bool, true, std::isnan(obj.tgd2));
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             obj.signal.messageType);
   TURETURN();
}


unsigned BDSD1NavEph_T ::
getUserTimeTest()
{
   TUDEF("BDSD1NavEph", "getUserTime");
   gpstk::BDSD1NavEph obj;
   obj.timeStamp = gpstk::BDSWeekSecond(2100,253.0);
   obj.xmitTime = gpstk::BDSWeekSecond(2100,135.0);
   obj.xmit2 = gpstk::BDSWeekSecond(2100,141.0);
   obj.xmit3 = gpstk::BDSWeekSecond(2100,147.0);
   gpstk::CommonTime exp(gpstk::BDSWeekSecond(2100,153.0));
   TUASSERTE(gpstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}


unsigned BDSD1NavEph_T ::
fixFitTest()
{
   TUDEF("BDSD1NavEph", "fixFit");
   gpstk::BDSD1NavEph obj;
   gpstk::BDSWeekSecond beginExpWS2(826,511200), endExpWS2(826,525600);
   gpstk::CommonTime beginExp2(beginExpWS2), endExp2(endExpWS2);
   obj.Toe = obj.Toc = gpstk::BDSWeekSecond(826,518400);
   obj.xmitTime = gpstk::BDSWeekSecond(826,518400);
   TUCATCH(obj.fixFit());
   TUASSERTE(gpstk::CommonTime, beginExp2, obj.beginFit);
   TUASSERTE(gpstk::CommonTime, endExp2, obj.endFit);
      //obj.dump(std::cerr, gpstk::OrbitDataKepler::Detail::Full);
   gpstk::BDSWeekSecond beginExpWS3(826,547620), endExpWS3(826,554400);
   gpstk::CommonTime beginExp3(beginExpWS3), endExp3(endExpWS3);
   obj.Toe = obj.Toc = gpstk::BDSWeekSecond(826,547200);
   obj.xmitTime = gpstk::BDSWeekSecond(826,547620);
   TUCATCH(obj.fixFit());
   TUASSERTE(gpstk::CommonTime, beginExp3, obj.beginFit);
   TUASSERTE(gpstk::CommonTime, endExp3, obj.endFit);
   TURETURN();
}


unsigned BDSD1NavEph_T ::
validateTest()
{
   TUDEF("BDSD1NavData", "validate");
   gpstk::BDSD1NavEph obj;
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
      TUASSERTE(unsigned, exp, gpstk::BDSD1NavEph::getAOD(i));
   }
   TUASSERTE(unsigned, -1, gpstk::BDSD1NavEph::getAOD(33));
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

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
