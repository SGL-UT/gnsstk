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
#include "GalFNavAlm.hpp"
#include "TestUtil.hpp"
#include "GALWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::GalHealthStatus e)
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
};


unsigned GalFNavAlm_T ::
constructorTest()
{
   TUDEF("GalFNavAlm", "GalFNavAlm");
   gpstk::GalFNavAlm uut;
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Almanac,
             uut.signal.messageType);
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime(), uut.xmit2);
   TUASSERTFE(0.0, uut.dAhalf);
   TUASSERTFE(0.0, uut.deltai);
   TUASSERTE(unsigned, 0, uut.wna);
   TUASSERTFE(0.0, uut.t0a);
   TUASSERTE(unsigned, 0, uut.ioda5);
   TUASSERTE(unsigned, 0, uut.ioda6);
   TUASSERTE(gpstk::GalHealthStatus,gpstk::GalHealthStatus::Unknown,uut.hsE5a);
   TURETURN();
}


unsigned GalFNavAlm_T ::
getUserTimeTest()
{
   TUDEF("GalFNavAlm", "getUserTime");
   gpstk::GalFNavAlm uut;
   uut.xmitTime = gpstk::GALWeekSecond(2100,141.0);
   uut.xmit2 = gpstk::GALWeekSecond(2100,135.0);
   gpstk::CommonTime exp(uut.xmitTime + 10);
   uut.signal = gpstk::NavMessageID(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::Galileo,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::E5aI,
                            gpstk::NavType::GalFNAV),
      gpstk::NavMessageType::Almanac);
   TUASSERTE(gpstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GalFNavAlm_T ::
fixFitTest()
{
   TUDEF("GalFNavAlm", "fixFit");
   gpstk::CommonTime toa = gpstk::GALWeekSecond(2100,135.0);
   gpstk::CommonTime xmit = gpstk::GALWeekSecond(2099,604000.0);
   gpstk::CommonTime expBegin = xmit;
   gpstk::CommonTime expEnd   = toa + (74.0 * 3600.0);
   gpstk::GalFNavAlm uut;
   uut.Toe = toa;
   uut.xmitTime = xmit;
   TUCATCH(uut.fixFit());
   TUASSERTE(gpstk::CommonTime, expBegin, uut.beginFit);
   TUASSERTE(gpstk::CommonTime, expEnd, uut.endFit);
   TURETURN();
}


int main()
{
   GalFNavAlm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
