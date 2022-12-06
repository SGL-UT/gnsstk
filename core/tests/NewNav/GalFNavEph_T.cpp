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
#include "GalFNavEph.hpp"
#include "TestUtil.hpp"
#include "GALWeekSecond.hpp"
#include "CivilTime.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gnsstk::GalHealthStatus e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gnsstk::GalDataValid e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GalFNavEph_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned getXvtTest();
};


unsigned GalFNavEph_T ::
constructorTest()
{
   TUDEF("GalFNavEph", "GalFNavEph");
   gnsstk::GalFNavEph uut;
   TUASSERTFE(0.0, uut.bgdE5aE1);
   TUASSERTE(unsigned, 255, uut.sisaIndex);
   TUASSERTE(unsigned, 0, uut.svid);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime(), uut.xmit2);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime(), uut.xmit3);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime(), uut.xmit4);
   TUASSERTE(uint16_t, 0, uut.iodnav1);
   TUASSERTE(uint16_t, 0, uut.iodnav2);
   TUASSERTE(uint16_t, 0, uut.iodnav3);
   TUASSERTE(uint16_t, 0, uut.iodnav4);
   TUASSERTE(gnsstk::GalHealthStatus,gnsstk::GalHealthStatus::Unknown,uut.hsE5a);
   TUASSERTE(gnsstk::GalDataValid,gnsstk::GalDataValid::Unknown,uut.dvsE5a);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Ephemeris,
             uut.signal.messageType);
   TUASSERTE(uint16_t, 0, uut.wn1);
   TUASSERTE(uint32_t, 0, uut.tow1);
   TUASSERTE(uint16_t, 0, uut.wn2);
   TUASSERTE(uint32_t, 0, uut.tow2);
   TUASSERTE(uint16_t, 0, uut.wn3);
   TUASSERTE(uint32_t, 0, uut.tow3);
   TUASSERTE(uint32_t, 0, uut.tow4);
   TURETURN();
}


unsigned GalFNavEph_T ::
getUserTimeTest()
{
   TUDEF("GalFNavEph", "getUserTime");
   gnsstk::GalFNavEph uut;
      // just using made-up numbers
   uut.timeStamp = gnsstk::GALWeekSecond(2100,135.0);
   uut.xmitTime = gnsstk::GALWeekSecond(2100,139.0);
   uut.xmit2 = gnsstk::GALWeekSecond(2100,200.0);
   uut.xmit3 = gnsstk::GALWeekSecond(2100,1.0);
   uut.xmit4 = gnsstk::GALWeekSecond(2100,7.0);
   gnsstk::CommonTime exp(gnsstk::GALWeekSecond(2100,210.0));
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GalFNavEph_T ::
fixFitTest()
{
   TUDEF("GalFNavEph", "fixFit");
   gnsstk::GalFNavEph uut;
   gnsstk::GALWeekSecond beginExpWS2(2059, 597600), endExpWS2(2060, 12600);
   gnsstk::CommonTime beginExp2(beginExpWS2), endExp2(endExpWS2);
   uut.Toe = gnsstk::GALWeekSecond(2059, 603000);
   uut.xmitTime = gnsstk::GALWeekSecond(2059,597600);
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, beginExp2, uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, endExp2, uut.endFit);
      //uut.dump(std::cerr, gnsstk::OrbitDataKepler::Detail::Full);
   TURETURN();
}


unsigned GalFNavEph_T ::
getXvtTest()
{
   TUDEF("GalFNavEph", "getXvt");
   gnsstk::GalFNavEph uut;
   gnsstk::Xvt xvt;
   gnsstk::RefFrame expRF(gnsstk::RefFrameRlz::ITRF2014);
   uut.xmitTime = gnsstk::GALWeekSecond(1854, .720000000000e+04);
   uut.Toe = gnsstk::GALWeekSecond(1854, .143840000000e+05);
   uut.Toc = gnsstk::CivilTime(2015,7,19,3,59,44.0,gnsstk::TimeSystem::GAL);
   uut.health = gnsstk::SVHealth::Healthy;
   gnsstk::CivilTime civ(2015,7,19,2,0,35.0,gnsstk::TimeSystem::GAL);
   TUASSERT(uut.getXvt(civ, xvt));
   TUASSERTE(gnsstk::Xvt::HealthStatus, gnsstk::Xvt::Healthy, xvt.health);
   TUASSERTE(gnsstk::RefFrame,expRF,xvt.frame);
   TURETURN();
}


int main()
{
   GalFNavEph_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.getXvtTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
