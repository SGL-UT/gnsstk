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
#include "GPSCNavTimeOffset.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class GPSCNavTimeOffset_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned validateTest();
   unsigned getUserTimeTest();
   unsigned getOffsetTest();
   unsigned getConversionsTest();
};


unsigned GPSCNavTimeOffset_T ::
constructorTest()
{
   TUDEF("GPSCNavTimeOffset", "GPSCNavTimeOffset");
   gnsstk::GPSCNavTimeOffset uut;
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::Unknown, uut.tgt);
   TUASSERTFE(0.0, uut.a0);
   TUASSERTFE(0.0, uut.a1);
   TUASSERTFE(0.0, uut.a2);
   TUASSERTFE(0.0, uut.deltatLS);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime(), uut.refTime);
   TUASSERTFE(0.0, uut.tot);
   TUASSERTE(unsigned, 0, uut.wnot);
   TUASSERTE(unsigned, 0, uut.wnLSF);
   TUASSERTE(unsigned, 0, uut.dn);
   TUASSERTFE(0.0, uut.deltatLSF);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::TimeOffset,
             uut.signal.messageType);
   TURETURN();
}


unsigned GPSCNavTimeOffset_T ::
validateTest()
{
   TUDEF("GPSCNavTimeOffset", "validate");
   gnsstk::GPSCNavTimeOffset offs;
   TUASSERTE(bool, true, offs.validate());
   offs.tot = 604784.0;
   TUASSERTE(bool, true, offs.validate());
   offs.dn = 7;
   TUASSERTE(bool, true, offs.validate());
   offs.dn = 8;
   TUASSERTE(bool, false, offs.validate());
   offs.dn = 7;
   offs.tot = -0.001;
   TUASSERTE(bool, false, offs.validate());
   offs.tot = 604784.1;
   TUASSERTE(bool, false, offs.validate());
   TURETURN();
}


unsigned GPSCNavTimeOffset_T ::
getUserTimeTest()
{
   TUDEF("GPSCNavTimeOffset", "getUserTime");
   gnsstk::GPSCNavTimeOffset uut;
      // L2 has a 12s cadence, L5 has a 6s cadence
   gnsstk::CommonTime expL2(gnsstk::GPSWeekSecond(2100,147.0));
   gnsstk::CommonTime expL5(gnsstk::GPSWeekSecond(2100,141.0));
   uut.timeStamp = gnsstk::GPSWeekSecond(2100,135.0);
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I,
                            gnsstk::NavType::GPSCNAVL5),
      gnsstk::NavMessageType::TimeOffset);
   TUASSERTE(gnsstk::CommonTime, expL5, uut.getUserTime());
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CM,
                            gnsstk::NavType::GPSCNAVL2),
      gnsstk::NavMessageType::TimeOffset);
   TUASSERTE(gnsstk::CommonTime, expL2, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNavTimeOffset_T ::
getOffsetTest()
{
   TUDEF("GPSCNavTimeOffset", "getOffset");
   gnsstk::GPSCNavTimeOffset uut;
   gnsstk::GPSWeekSecond ws1(2060, 405504.0);
   gnsstk::GPSWeekSecond ws2(2061, 405504.0);
   uut.tgt = gnsstk::TimeSystem::UTC;
   uut.a0 = 1.9790604711E-09;
   uut.a1 = 7.5495165675E-15;
   uut.a2 = 0;
   uut.deltatLS = 18.0;
   uut.tot = 21600.0;
   uut.wnot = 2060;
   uut.refTime = gnsstk::GPSWeekSecond(uut.wnot,uut.tot);
   double offset;
      /// @todo Truth values here need to be verified.
   TUASSERT(uut.getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC, ws1,
                          offset));
   TUASSERTFE(18.000000004877350079, offset);
   TUASSERT(uut.getOffset(gnsstk::TimeSystem::GPS, gnsstk::TimeSystem::UTC, ws2,
                          offset));
   TUASSERTFE(18.000000009443297699, offset);
   TURETURN();
}


unsigned GPSCNavTimeOffset_T ::
getConversionsTest()
{
   TUDEF("GPSCNavTimeOffset", "getConversions");
   gnsstk::TimeCvtSet convs;
   gnsstk::GPSCNavTimeOffset offs;
      // This looks a bit weird, but basically getConversions is
      // expected to return a set containing one key pair for GPS to
      // the target time system, which by default is Unknown.
   gnsstk::TimeCvtKey key1(gnsstk::TimeSystem::GPS,
                                          gnsstk::TimeSystem::Unknown);
   gnsstk::TimeCvtKey key2(gnsstk::TimeSystem::Unknown,
                                          gnsstk::TimeSystem::GPS);
   TUCATCH(convs = offs.getConversions());
   TUASSERTE(size_t, 2, convs.size());
   TUASSERTE(size_t, 1, convs.count(key1));
   TUASSERTE(size_t, 1, convs.count(key2));
   TURETURN();
}


int main()
{
   GPSCNavTimeOffset_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.validateTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getOffsetTest();
   errorTotal += testClass.getConversionsTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
