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
#include "GalFNavAlm.hpp"
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

   std::ostream& operator<<(std::ostream& s, gnsstk::SVHealth e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GalFNavAlm_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned fixHealthTest();
   unsigned getXvtTest();
};


unsigned GalFNavAlm_T ::
constructorTest()
{
   TUDEF("GalFNavAlm", "GalFNavAlm");
   gnsstk::GalFNavAlm uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Almanac,
             uut.signal.messageType);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime(), uut.xmit2);
   TUASSERTFE(0.0, uut.dAhalf);
   TUASSERTFE(0.0, uut.deltai);
   TUASSERTE(unsigned, 0, uut.wna);
   TUASSERTFE(0.0, uut.t0a);
   TUASSERTE(unsigned, 0, uut.ioda5);
   TUASSERTE(unsigned, 0, uut.ioda6);
   TUASSERTE(gnsstk::GalHealthStatus,gnsstk::GalHealthStatus::Unknown,uut.hsE5a);
   TURETURN();
}


unsigned GalFNavAlm_T ::
getUserTimeTest()
{
   TUDEF("GalFNavAlm", "getUserTime");
   gnsstk::GalFNavAlm uut;
   uut.xmitTime = gnsstk::GALWeekSecond(2100,141.0);
   uut.xmit2 = gnsstk::GALWeekSecond(2100,135.0);
   gnsstk::CommonTime exp(uut.xmitTime + 10);
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Galileo,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::E5aI,
                            gnsstk::NavType::GalFNAV),
      gnsstk::NavMessageType::Almanac);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GalFNavAlm_T ::
fixFitTest()
{
   TUDEF("GalFNavAlm", "fixFit");
   gnsstk::CommonTime toa = gnsstk::GALWeekSecond(2100,135.0);
   gnsstk::CommonTime xmit = gnsstk::GALWeekSecond(2099,604000.0);
   gnsstk::CommonTime expBegin = xmit;
   gnsstk::CommonTime expEnd   = toa + (74.0 * 3600.0);
   gnsstk::GalFNavAlm uut;
   uut.Toe = toa;
   uut.xmitTime = xmit;
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, expBegin, uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, expEnd, uut.endFit);
   TURETURN();
}


unsigned GalFNavAlm_T ::
fixHealthTest()
{
   TUDEF("GalFNavAlm", "fixHealth");
   gnsstk::GalFNavAlm uut;
   uut.hsE5a = gnsstk::GalHealthStatus::OK;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut.health);
   uut.hsE5a = gnsstk::GalHealthStatus::OutOfService;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut.health);
   uut.hsE5a = gnsstk::GalHealthStatus::WillBeOOS;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Degraded, uut.health);
   uut.hsE5a = gnsstk::GalHealthStatus::InTest;
   uut.fixHealth();
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut.health);
   TURETURN();
}


unsigned GalFNavAlm_T ::
getXvtTest()
{
   TUDEF("GalFNavAlm", "getXvt");
   gnsstk::GalFNavAlm uut;
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
   GalFNavAlm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.fixHealthTest();
   errorTotal += testClass.getXvtTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
