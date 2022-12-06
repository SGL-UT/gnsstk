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
#include "GPSCNavEph.hpp"
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

class GPSCNavEph_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned validateTest();
   unsigned getXvtTest();
};


unsigned GPSCNavEph_T ::
constructorTest()
{
   TUDEF("GPSCNavEph", "GPSCNavEph");
   gnsstk::GPSCNavEph uut;
   TUASSERTE(uint32_t, 0, uut.pre11);
   TUASSERTE(uint32_t, 0, uut.preClk);
   TUASSERTE(bool, true, uut.healthL1);
   TUASSERTE(bool, true, uut.healthL2);
   TUASSERTE(bool, true, uut.healthL5);
   TUASSERTE(int8_t, -16, uut.uraED);
   TUASSERTE(int8_t, -16, uut.uraNED0);
   TUASSERTE(uint8_t, 0, uut.uraNED1);
   TUASSERTE(uint8_t, 0, uut.uraNED2);
   TUASSERTE(bool, false, uut.alert11);
   TUASSERTE(bool, false, uut.alertClk);
   TUASSERTE(bool, false, uut.integStat);
   TUASSERTE(bool, false, uut.phasingL2C);
   TUASSERTFE(0, uut.deltaA);
   TUASSERTFE(0, uut.dOMEGAdot);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::BEGINNING_OF_TIME, uut.top);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::BEGINNING_OF_TIME,
             uut.xmit11);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::BEGINNING_OF_TIME,
             uut.xmitClk);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Ephemeris,
             uut.signal.messageType);
   TURETURN();
}


unsigned GPSCNavEph_T ::
getUserTimeTest()
{
   TUDEF("GPSCNavEph", "getUserTime");
   gnsstk::GPSCNavEph uut;
      // just using made-up numbers
   uut.timeStamp = gnsstk::GPSWeekSecond(2100,135.0);
   uut.xmitTime = gnsstk::GPSWeekSecond(2100,139.0);
   uut.xmit11 = gnsstk::GPSWeekSecond(2100,200.0);
   uut.xmitClk = gnsstk::GPSWeekSecond(2100,1.0);
      // L2 has a 12s cadence, L5 has a 6s cadence
   gnsstk::CommonTime expL2(gnsstk::GPSWeekSecond(2100,212.0));
   gnsstk::CommonTime expL5(gnsstk::GPSWeekSecond(2100,206.0));
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I,
                            gnsstk::NavType::GPSCNAVL5),
      gnsstk::NavMessageType::Ephemeris);
   TUASSERTE(gnsstk::CommonTime, expL5, uut.getUserTime());
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CM,
                            gnsstk::NavType::GPSCNAVL2),
      gnsstk::NavMessageType::Ephemeris);
   TUASSERTE(gnsstk::CommonTime, expL2, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNavEph_T ::
fixFitTest()
{
   TUDEF("GPSCNavEph", "fixFit");
   gnsstk::GPSCNavEph uut;
   gnsstk::GPSWeekSecond beginExpWS2(2059, 597600), endExpWS2(2060, 3600);
   gnsstk::CommonTime beginExp2(beginExpWS2), endExp2(endExpWS2);
   uut.Toe = gnsstk::GPSWeekSecond(2059, 603000);
   uut.xmitTime = gnsstk::GPSWeekSecond(2059,597600);
   uut.xmit11 = gnsstk::GPSWeekSecond(2059,597612);
   uut.xmitClk = gnsstk::GPSWeekSecond(2059,597624);
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, beginExp2, uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, endExp2, uut.endFit);
      //uut.dump(std::cerr, gnsstk::OrbitDataKepler::Detail::Full);
   TURETURN();
}


unsigned GPSCNavEph_T ::
validateTest()
{
   TUDEF("GPSCNavData", "validate");
   gnsstk::GPSCNavEph uut;
   TUASSERTE(bool, true, uut.validate());
   uut.pre = 0x22c; // this is not valid
   TUASSERTE(bool, false, uut.validate());
   uut.pre = 0x8b; // this is valid
   uut.pre11 = 0x22c; // this is not valid
   TUASSERTE(bool, false, uut.validate());
   uut.pre11 = 0x8b; // this is valid
   uut.preClk = 0x22c; // this is not valid
   TUASSERTE(bool, false, uut.validate());
   uut.preClk = 0x8b; // this is valid (all 3 preambles == 8b)
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}


unsigned GPSCNavEph_T ::
getXvtTest()
{
   TUDEF("GPSCNavEph", "getXvt");
   gnsstk::GPSCNavEph uut;
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
   GPSCNavEph_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.validateTest();
   errorTotal += testClass.getXvtTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
