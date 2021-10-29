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
#include "GalFNavHealth.hpp"
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

class GalFNavHealth_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
      /// explicitly test getHealth, implicitly test galHealth
   unsigned getHealthTest();
};


unsigned GalFNavHealth_T ::
constructorTest()
{
   TUDEF("GalFNavHealth", "GalFNavHealth");
   gnsstk::GalFNavHealth uut;
   TUASSERTE(gnsstk::GalHealthStatus, gnsstk::GalHealthStatus::Unknown,
             uut.sigHealthStatus);
   TUASSERTE(gnsstk::GalDataValid, gnsstk::GalDataValid::Unknown,
             uut.dataValidityStatus);
   TUASSERTE(unsigned, 255, uut.sisaIndex);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Health,
             uut.signal.messageType);
   TURETURN();
}


unsigned GalFNavHealth_T ::
getUserTimeTest()
{
   TUDEF("GalFNavHealth", "getUserTime");
   gnsstk::GalFNavHealth uut;
   uut.timeStamp = gnsstk::GPSWeekSecond(2100,135.0);
   gnsstk::CommonTime exp(gnsstk::GPSWeekSecond(2100,135.0));
   exp = exp + 10.0;
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GalFNavHealth_T ::
getHealthTest()
{
   TUDEF("GalFNavHealth", "getHealth");
   gnsstk::GalFNavHealth uut;
      // default should be unknown
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unknown, uut.getHealth());
   uut.sigHealthStatus = gnsstk::GalHealthStatus::OutOfService;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut.getHealth());
   uut.sigHealthStatus = gnsstk::GalHealthStatus::Unknown;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unknown, uut.getHealth());
   uut.sigHealthStatus = gnsstk::GalHealthStatus::InTest;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut.getHealth());
   uut.sigHealthStatus = gnsstk::GalHealthStatus::WillBeOOS;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Degraded, uut.getHealth());
   uut.sigHealthStatus = gnsstk::GalHealthStatus::OK;
   uut.dataValidityStatus = gnsstk::GalDataValid::NoGuarantee;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Degraded, uut.getHealth());
   uut.dataValidityStatus = gnsstk::GalDataValid::Valid;
   uut.sisaIndex = 255;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Degraded, uut.getHealth());
   uut.sisaIndex = 254;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut.getHealth());
   uut.sisaIndex = 0;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut.getHealth());
   uut.dataValidityStatus = gnsstk::GalDataValid::Unknown;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unknown, uut.getHealth());

   TURETURN();
}


int main()
{
   GalFNavHealth_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getHealthTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
