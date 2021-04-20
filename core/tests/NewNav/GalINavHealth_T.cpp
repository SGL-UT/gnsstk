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
#include "GalINavHealth.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gpstk::SVHealth e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gpstk::GalHealthStatus e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gpstk::GalDataValid e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GalINavHealth_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
      /// explicitly test getHealth, implicitly test galHealth
   unsigned getHealthTest();
};


unsigned GalINavHealth_T ::
constructorTest()
{
   TUDEF("GalINavHealth", "GalINavHealth");
   gpstk::GalINavHealth uut;
   TUASSERTE(gpstk::GalHealthStatus, gpstk::GalHealthStatus::Unknown,
             uut.sigHealthStatus);
   TUASSERTE(gpstk::GalDataValid, gpstk::GalDataValid::Unknown,
             uut.dataValidityStatus);
   TUASSERTE(unsigned, 255, uut.sisaIndex);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Health,
             uut.signal.messageType);
   TURETURN();
}


unsigned GalINavHealth_T ::
getUserTimeTest()
{
   TUDEF("GalINavHealth", "getUserTime");
   gpstk::GalINavHealth uut;
   uut.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime exp(gpstk::GPSWeekSecond(2100,135.0));
   exp = exp + 2.0;
   TUASSERTE(gpstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GalINavHealth_T ::
getHealthTest()
{
   TUDEF("GalINavHealth", "getHealth");
   gpstk::GalINavHealth uut;
      // default should be unknown
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unknown, uut.getHealth());
   uut.sigHealthStatus = gpstk::GalHealthStatus::OutOfService;
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unhealthy, uut.getHealth());
   uut.sigHealthStatus = gpstk::GalHealthStatus::Unknown;
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unknown, uut.getHealth());
   uut.sigHealthStatus = gpstk::GalHealthStatus::InTest;
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unhealthy, uut.getHealth());
   uut.sigHealthStatus = gpstk::GalHealthStatus::WillBeOOS;
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Degraded, uut.getHealth());
   uut.sigHealthStatus = gpstk::GalHealthStatus::OK;
   uut.dataValidityStatus = gpstk::GalDataValid::NoGuarantee;
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Degraded, uut.getHealth());
   uut.dataValidityStatus = gpstk::GalDataValid::Valid;
   uut.sisaIndex = 255;
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Degraded, uut.getHealth());
   uut.sisaIndex = 254;
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, uut.getHealth());
   uut.sisaIndex = 0;
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, uut.getHealth());
   uut.dataValidityStatus = gpstk::GalDataValid::Unknown;
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unknown, uut.getHealth());
   
   TURETURN();
}


int main()
{
   GalINavHealth_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getHealthTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
