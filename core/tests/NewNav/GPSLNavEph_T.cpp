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
   gnsstk::GPSLNavEph obj;
      /** @todo When using navdmp, it indicated the begin valid time
       * was 597600 SOW.  The fixFit() method, which was taken from
       * OrbElemRinex::computeBeginValid, does not make this 2-hour
       * boundary adjustment when the Toe isn't already aligned on
       * 2-hour boundaries.  So for now we use 603360 SOW for the
       * test, since that matches expected behavior for this code. */
   gnsstk::GPSWeekSecond beginExpWS2(2121, 603360), endExpWS2(2122, 7200);
   gnsstk::CommonTime beginExp2(beginExpWS2), endExp2(endExpWS2);
   obj.iodc = 0xd;
   obj.fitIntFlag = 0;
   obj.Toe = obj.Toc = gnsstk::GPSWeekSecond(2122, 0);
   obj.xmitTime = gnsstk::GPSWeekSecond(2121,603360);
   TUCATCH(obj.fixFit());
   TUASSERTE(gnsstk::CommonTime, beginExp2, obj.beginFit);
   TUASSERTE(gnsstk::CommonTime, endExp2, obj.endFit);
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
