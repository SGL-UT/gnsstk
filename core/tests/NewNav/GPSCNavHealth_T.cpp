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
#include "GPSCNavHealth.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
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

class GPSCNavHealth_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned getHealthTest();
};


unsigned GPSCNavHealth_T ::
constructorTest()
{
   TUDEF("GPSCNavHealth", "GPSCNavHealth");
   gnsstk::GPSCNavHealth uut;
   TUASSERTE(bool, true, uut.health);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Health,
             uut.signal.messageType);
   TURETURN();
}


unsigned GPSCNavHealth_T ::
getUserTimeTest()
{
   TUDEF("GPSCNavHealth", "getUserTime");
   gnsstk::GPSCNavHealth uut;
      // L2 has a 12s cadence, L5 has a 6s cadence
   gnsstk::CommonTime expL2(gnsstk::GPSWeekSecond(2100,147.0));
   gnsstk::CommonTime expL5(gnsstk::GPSWeekSecond(2100,141.0));
   uut.timeStamp = gnsstk::GPSWeekSecond(2100,135.0);
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I,
                            gnsstk::NavType::GPSCNAVL5),
      gnsstk::NavMessageType::Health);
   TUASSERTE(gnsstk::CommonTime, expL5, uut.getUserTime());
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CM,
                            gnsstk::NavType::GPSCNAVL2),
      gnsstk::NavMessageType::Health);
   TUASSERTE(gnsstk::CommonTime, expL2, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNavHealth_T ::
getHealthTest()
{
   TUDEF("GPSCNavHealth", "getHealth");
   gnsstk::GPSCNavHealth uut;
      // default should be unhealthy
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut.getHealth());
      // the one condition that should result in healthy
   uut.health = false;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut.getHealth());
   TURETURN();
}


int main()
{
   GPSCNavHealth_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getHealthTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
