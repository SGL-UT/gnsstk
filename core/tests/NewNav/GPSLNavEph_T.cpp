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
#include "GPSLNavEph.hpp"
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
   std::ostream& operator<<(std::ostream& s, gnsstk::GPSLNavL2Codes e)
   {
      s << static_cast<long>(e);
      return s;
   }
}

class GPSLNavEph_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned validateTest();
      /** This is actually implemented in NavData but needs to be in
       * any one of the leaf classes to be tested properly. */
   unsigned getClassNameTest();
   unsigned getXvtTest();
   unsigned svRelativityTest();
};


unsigned GPSLNavEph_T ::
getClassNameTest()
{
   TUDEF("NavData", "getClassName");
   gnsstk::GPSLNavEph uut;
   TUASSERTE(std::string, "gnsstk::GPSLNavEph", uut.getClassName());
   TURETURN();
}


unsigned GPSLNavEph_T ::
constructorTest()
{
   TUDEF("GPSLNavEph", "GPSLNavEph");
   gnsstk::GPSLNavEph obj;
   TUASSERTE(uint32_t, 0, obj.pre2);
   TUASSERTE(uint32_t, 0, obj.pre3);
   TUASSERTE(uint32_t, 0, obj.tlm2);
   TUASSERTE(uint32_t, 0, obj.tlm3);
   TUASSERTE(bool, false, obj.isf2);
   TUASSERTE(bool, false, obj.isf3);
   TUASSERTE(uint16_t, 0, obj.iodc);
   TUASSERTE(uint16_t, 0, obj.iode);
   TUASSERTE(uint8_t,  0, obj.fitIntFlag);
   TUASSERTE(uint8_t, 0xff, obj.healthBits);
   TUASSERTE(uint8_t, 0, obj.uraIndex);
   TUASSERTFE(0.0, obj.tgd);
   TUASSERTE(bool, false, obj.asFlag2);
   TUASSERTE(bool, false, obj.asFlag3);
   TUASSERTE(bool, false, obj.alert2);
   TUASSERTE(bool, false, obj.alert3);
   TUASSERTE(gnsstk::GPSLNavL2Codes, gnsstk::GPSLNavL2Codes::Invalid1,
             obj.codesL2);
   TUASSERTE(bool, false, obj.L2Pdata);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Ephemeris,
             obj.signal.messageType);
   TURETURN();
}


unsigned GPSLNavEph_T ::
getUserTimeTest()
{
   TUDEF("GPSLNavEph", "getUserTime");
   gnsstk::GPSLNavEph obj;
   obj.xmitTime = gnsstk::GPSWeekSecond(2100,135.0);
   obj.xmit2 = gnsstk::GPSWeekSecond(2100,111.0);
   obj.xmit3 = gnsstk::GPSWeekSecond(2100,147.0);
   gnsstk::CommonTime exp(gnsstk::GPSWeekSecond(2100,153.0));
   TUASSERTE(gnsstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}


unsigned GPSLNavEph_T ::
fixFitTest()
{
   TUDEF("GPSLNavEph", "fixFit");
   
   // Normal GPS operations -- Standard 4 hour curve fit interval and Toe on 2 hour boundary.
   // Broadcasted on the hour.
   {
      gnsstk::CommonTime beginExp{gnsstk::GPSWeekSecond(2121, 597600)};
      gnsstk::CommonTime endExp{gnsstk::GPSWeekSecond(2122, 7200)};
      gnsstk::GPSLNavEph obj;
      obj.signal.system = gnsstk::SatelliteSystem::GPS;
      obj.fitIntFlag = 0;
      obj.iodc = 13;
      obj.Toe = gnsstk::GPSWeekSecond(2122, 0);
      obj.xmitTime = gnsstk::GPSWeekSecond(2121,597600);
      TUCATCH(obj.fixFit());
      TUASSERTE(gnsstk::CommonTime, beginExp, obj.beginFit);
      TUASSERTE(gnsstk::CommonTime, endExp, obj.endFit);
   }

   // Normal GPS operations -- Standard 4 hour curve fit interval and Toe on 2 hour boundary.
   // Broadcasted off the hour.
   // The begin fit can be assumed to be before the transmit time.
   {
      gnsstk::CommonTime beginExp{gnsstk::GPSWeekSecond(2121, 597600)};
      gnsstk::CommonTime endExp{gnsstk::GPSWeekSecond(2122, 7200)};
      gnsstk::GPSLNavEph obj;
      obj.signal.system = gnsstk::SatelliteSystem::GPS;
      obj.fitIntFlag = 0;
      obj.iodc = 13;
      obj.Toe = gnsstk::GPSWeekSecond(2122, 0);
      obj.xmitTime = gnsstk::GPSWeekSecond(2121,599040);
      TUCATCH(obj.fixFit());
      TUASSERTE(gnsstk::CommonTime, beginExp, obj.beginFit);
      TUASSERTE(gnsstk::CommonTime, endExp, obj.endFit);
   }

   // Normal GPS operations Upload Cutover -- Toe not on hour boundary.
   // Without additional info, the begin fit can only be assumed as the transmit time.
   // End fit time is based on the midpoint of the curve fit interval but the Toe
   // is no longer the midpoint in this case.
   {
      gnsstk::CommonTime beginExp{gnsstk::GPSWeekSecond(2121, 603360)};
      gnsstk::CommonTime endExp{gnsstk::GPSWeekSecond(2122, 7200)};
      gnsstk::GPSLNavEph obj;
      obj.signal.system = gnsstk::SatelliteSystem::GPS;
      obj.fitIntFlag = 0;
      obj.iodc = 13;
      obj.Toe = gnsstk::GPSWeekSecond(2121, 604784);
      obj.xmitTime = gnsstk::GPSWeekSecond(2121,603360);
      TUCATCH(obj.fixFit());
      TUASSERTE(gnsstk::CommonTime, beginExp, obj.beginFit);
      TUASSERTE(gnsstk::CommonTime, endExp, obj.endFit);
   }

   // GPS short term extended nav -- 6 hour curve fit interval, broadcasted every 4 hours.
   // Fit interval flag is 1 and IODE is less than 240.
   // IODC is then used to determine curve fit interval
   {
      gnsstk::CommonTime beginExp{gnsstk::GPSWeekSecond(2122, 0)};
      gnsstk::CommonTime endExp{gnsstk::GPSWeekSecond(2122, 21600)};
      gnsstk::GPSLNavEph obj;
      obj.signal.system = gnsstk::SatelliteSystem::GPS;
      obj.fitIntFlag = 1;
      obj.iodc = 239;
      obj.Toe = gnsstk::GPSWeekSecond(2122, 10800);
      obj.xmitTime = gnsstk::GPSWeekSecond(2122, 3630);
      TUCATCH(obj.fixFit());
      TUASSERTE(gnsstk::CommonTime, beginExp, obj.beginFit);
      TUASSERTE(gnsstk::CommonTime, endExp, obj.endFit);
   }

   // GPS long term extended nav -- 8 hour curve fit interval, broadcasted every 6 hours.
   // Fit interval flag  is 1 and IODC is between 240-247
   {
      gnsstk::CommonTime beginExp{gnsstk::GPSWeekSecond(2122, 0)};
      gnsstk::CommonTime endExp{gnsstk::GPSWeekSecond(2122, 28800)};
      gnsstk::GPSLNavEph obj;
      obj.signal.system = gnsstk::SatelliteSystem::GPS;
      obj.fitIntFlag = 1;
      obj.iodc = 244;
      obj.Toe = gnsstk::GPSWeekSecond(2122, 14400);
      obj.xmitTime = gnsstk::GPSWeekSecond(2122, 3630);
      TUCATCH(obj.fixFit());
      TUASSERTE(gnsstk::CommonTime, beginExp, obj.beginFit);
      TUASSERTE(gnsstk::CommonTime, endExp, obj.endFit);
   }
   
   // GPS long term extended nav -- 14 hour curve fit interval, broadcasted every 12 hours.
   // Fit interval flag  is 1 and IODC is 248-255, 496
   {
      gnsstk::CommonTime beginExp{gnsstk::GPSWeekSecond(2122, 0)};
      gnsstk::CommonTime endExp{gnsstk::GPSWeekSecond(2122, 50400)};
      gnsstk::GPSLNavEph obj;
      obj.signal.system = gnsstk::SatelliteSystem::GPS;
      obj.fitIntFlag = 1;
      obj.iodc = 496;
      obj.Toe = gnsstk::GPSWeekSecond(2122, 25200);
      obj.xmitTime = gnsstk::GPSWeekSecond(2122, 3630);
      TUCATCH(obj.fixFit());
      TUASSERTE(gnsstk::CommonTime, beginExp, obj.beginFit);
      TUASSERTE(gnsstk::CommonTime, endExp, obj.endFit);
   }

   // GPS long term extended nav -- 26 hour curve fit interval, broadcasted every 24 hours.
   // Fit interval flag  is 1 and IODC is 497-503, 1021-1023
   {
      gnsstk::CommonTime beginExp{gnsstk::GPSWeekSecond(2122, 0)};
      gnsstk::CommonTime endExp{gnsstk::GPSWeekSecond(2122, 93600)};
      gnsstk::GPSLNavEph obj;
      obj.signal.system = gnsstk::SatelliteSystem::GPS;
      obj.fitIntFlag = 1;
      obj.iodc = 1021;
      obj.Toe = gnsstk::GPSWeekSecond(2122, 46800);
      obj.xmitTime = gnsstk::GPSWeekSecond(2122, 3630);
      TUCATCH(obj.fixFit());
      TUASSERTE(gnsstk::CommonTime, beginExp, obj.beginFit);
      TUASSERTE(gnsstk::CommonTime, endExp, obj.endFit);
   }

   // QZSS normal operations -- 2 hour curve fit interval, broadcasted every hour
   {
      gnsstk::CommonTime beginExp{gnsstk::GPSWeekSecond(2122, 0)};
      gnsstk::CommonTime endExp{gnsstk::GPSWeekSecond(2122, 7200)};
      gnsstk::GPSLNavEph obj;
      obj.signal.system = gnsstk::SatelliteSystem::QZSS;
      obj.fitIntFlag = 0;
      obj.iodc = 13;
      obj.Toe = gnsstk::GPSWeekSecond(2122, 3600);
      obj.xmitTime = gnsstk::GPSWeekSecond(2122, 30);
      TUCATCH(obj.fixFit());
      TUASSERTE(gnsstk::CommonTime, beginExp, obj.beginFit);
      TUASSERTE(gnsstk::CommonTime, endExp, obj.endFit);
   }

      //obj.dump(std::cerr, gnsstk::OrbitDataKepler::Detail::Full);
   TURETURN();
}


unsigned GPSLNavEph_T ::
validateTest()
{
   TUDEF("GPSLNavData", "validate");
   gnsstk::GPSLNavEph obj;
   TUASSERTE(bool, true, obj.validate());
   obj.pre = 0x22c; // this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.pre = 0x8b; // this is valid
   obj.pre2 = 0x22c; // this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.pre2 = 0x8b; // this is valid
   obj.pre3 = 0x22c; // this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.pre3 = 0x8b; // this is valid (all 3 preambles == 8b)
   TUASSERTE(bool, true, obj.validate());
   TURETURN();
}


unsigned GPSLNavEph_T ::
getXvtTest()
{
   TUDEF("GPSLNavEph", "getXvt");
   gnsstk::GPSLNavEph uut;
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


unsigned GPSLNavEph_T ::
svRelativityTest()
{
   TUDEF("GPSLNavEph", "svRelativity");
   gnsstk::GPSLNavEph uut;
   gnsstk::CommonTime ct(
      gnsstk::CivilTime(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS));
   uut.xmitTime = gnsstk::GPSWeekSecond(1854, .720000000000e+04);
   uut.Toe = gnsstk::GPSWeekSecond(1854, .143840000000e+05);
   uut.Toc = gnsstk::CivilTime(2015,7,19,3,59,44.0,gnsstk::TimeSystem::GPS);
   uut.health = gnsstk::SVHealth::Healthy;
   uut.Cuc = .200793147087e-05;
   uut.Cus = .823289155960e-05;
   uut.Crc = .214593750000e+03;
   uut.Crs = .369375000000e+02;
   uut.Cic = -.175088644028e-06;
   uut.Cis = .335276126862e-07;
   uut.M0 = .218771233916e+01;
   uut.dn = .511592738462e-08;
      // dndot
   uut.ecc = .422249664553e-02;
   uut.Ahalf =.515360180473e+04;
   uut.A = uut.Ahalf * uut.Ahalf;
      // Adot
   uut.OMEGA0 = -.189462874179e+01;
   uut.i0 = .946122987969e+00;
   uut.w = .374892043461e+00;
   uut.OMEGAdot = -.823034282681e-08;
   uut.idot = .492877673191e-09;
   uut.af0 = -.216379296035e-03;
   uut.af1 = .432009983342e-11;
   uut.af2 = .000000000000e+00;
      //uut.iode = .190000000000e+02;
      // uut.codes = .100000000000e+01
      //uut.week = .185400000000e+04
      //uut.l2p = .000000000000e+00
      //uut.accuracy = .240000000000e+01
      //uut.tgd = -.107102096081e-07
      //uut.iodc = .190000000000e+02
      //uut.fitint = .400000000000e+01;
   TUASSERTFE(-8.7994080166185110758e-09, uut.svRelativity(ct));
   TUASSERTFE(-8.8197758101551758427e-09, uut.svRelativity(ct+35));
   TURETURN();
}


int main()
{
   GPSLNavEph_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.validateTest();
   errorTotal += testClass.getXvtTest();
   errorTotal += testClass.svRelativityTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
