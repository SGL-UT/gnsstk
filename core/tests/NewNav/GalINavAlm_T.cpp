//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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
#include "GalINavAlm.hpp"
#include "TestUtil.hpp"
#include "GALWeekSecond.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::SVHealth e)
   {
      s << StringUtils::asString(e);
      return s;
   }

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
}

class GalINavAlm_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned fixHealthTest();
};


unsigned GalINavAlm_T ::
constructorTest()
{
   TUDEF("GalINavAlm", "GalINavAlm");
   gnsstk::GalINavAlm uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Almanac,
             uut.signal.messageType);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime(), uut.xmit2);
   TUASSERTFE(0.0, uut.dAhalf);
   TUASSERTFE(0.0, uut.deltai);
   TUASSERTE(unsigned, 0, uut.wna);
   TUASSERTFE(0.0, uut.t0a);
   TUASSERTE(unsigned, 0, uut.ioda1);
   TUASSERTE(unsigned, 0, uut.ioda2);
   TUASSERTE(gnsstk::GalHealthStatus,gnsstk::GalHealthStatus::Unknown,uut.hsE5b);
   TUASSERTE(gnsstk::GalHealthStatus,gnsstk::GalHealthStatus::Unknown,uut.hsE1B);
   TURETURN();
}


unsigned GalINavAlm_T ::
getUserTimeTest()
{
   TUDEF("GalINavAlm", "getUserTime");
   gnsstk::GalINavAlm uut;
   uut.xmitTime = gnsstk::GALWeekSecond(2100,141.0);
   uut.xmit2 = gnsstk::GALWeekSecond(2100,135.0);
   gnsstk::CommonTime exp(uut.xmitTime + 2);
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Galileo,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GalINAV),
      gnsstk::NavMessageType::Almanac);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GalINavAlm_T ::
fixFitTest()
{
   TUDEF("GalINavAlm", "fixFit");
   gnsstk::CommonTime toa = gnsstk::GALWeekSecond(2100,135.0);
   gnsstk::CommonTime xmit = gnsstk::GALWeekSecond(2099,604000.0);
   gnsstk::CommonTime expBegin = xmit;
   gnsstk::CommonTime expEnd   = toa + (74.0 * 3600.0);
   gnsstk::GalINavAlm uut;
   uut.Toe = toa;
   uut.xmitTime = xmit;
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, expBegin, uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, expEnd, uut.endFit);
   TURETURN();
}


unsigned GalINavAlm_T ::
fixHealthTest()
{
   TUDEF("GalINavAlm", "fixHealth");
   gnsstk::GalINavAlm uut;
      // both OK
   uut.hsE5b = uut.hsE1B = gnsstk::GalHealthStatus::OK;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut.health);
      // OK + OOS
   uut.hsE1B = gnsstk::GalHealthStatus::OutOfService;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Degraded, uut.health);
      // OK + will be OOS
   uut.hsE1B = gnsstk::GalHealthStatus::WillBeOOS;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Degraded, uut.health);
      // OK + in test
   uut.hsE1B = gnsstk::GalHealthStatus::InTest;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Degraded, uut.health);
      // both OOS
   uut.hsE5b = uut.hsE1B = gnsstk::GalHealthStatus::OutOfService;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut.health);
      // OOS + will be OOS
   uut.hsE1B = gnsstk::GalHealthStatus::WillBeOOS;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Degraded, uut.health);
      // OOS + in test
   uut.hsE1B = gnsstk::GalHealthStatus::InTest;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Degraded, uut.health);
      // both will be OOS
   uut.hsE5b = uut.hsE1B = gnsstk::GalHealthStatus::WillBeOOS;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Degraded, uut.health);
      // will be OOS + in test
   uut.hsE1B = gnsstk::GalHealthStatus::InTest;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Degraded, uut.health);
      // both in test
   uut.hsE5b = uut.hsE1B = gnsstk::GalHealthStatus::InTest;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Degraded, uut.health);
   TURETURN();
}


int main()
{
   GalINavAlm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.fixHealthTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
