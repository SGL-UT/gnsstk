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
#include "TestUtil.hpp"
#include "GLOFNavHealth.hpp"
#include "CivilTime.hpp"

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

class GLOFNavHealth_T
{
public:
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned getHealthTest();
};


unsigned GLOFNavHealth_T ::
constructorTest()
{
   TUDEF("GLOFNavHealth", "GLOFNavHealth");
   gnsstk::GLOFNavHealth uut;
   TUASSERTE(bool, false, uut.healthBits.is_valid());
   TUASSERTE(bool, false, uut.ln.is_valid());
   TUASSERTE(bool, false, uut.Cn.is_valid());
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Health,
             uut.signal.messageType);
   TURETURN();
}


unsigned GLOFNavHealth_T ::
getUserTimeTest()
{
   TUDEF("GLOFNavHealth", "getUserTime");
   gnsstk::GLOFNavHealth uut;
   uut.timeStamp = gnsstk::CivilTime(2006, 10, 01, 0, 15, 0,
                                     gnsstk::TimeSystem::GLO);
      // 1x 2s string
   gnsstk::CommonTime exp(uut.timeStamp + 2.0);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GLOFNavHealth_T ::
getHealthTest()
{
   TUDEF("GLOFNavHealth", "getHealth");
   gnsstk::GLOFNavHealth uut1, uut2, uut3;
      // default should be unknown
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unknown, uut1.getHealth());
      // healthy ephemeris
   uut1.healthBits = 0;
   uut1.ln = false;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut1.getHealth());
      // healthy ephemeris
   uut1.healthBits = 3;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut1.getHealth());
      // unhealthy ephemeris
   uut1.healthBits = 4;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut1.getHealth());
      // unhealthy ephemeris
   uut1.healthBits = 0;
   uut1.ln = true;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut1.getHealth());
      // healthy almanac
   uut2.Cn = true;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut2.getHealth());
   uut2.ln = true;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut2.getHealth());
   uut2.ln = false;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut2.getHealth());
      // unhealthy almanac
   uut2.Cn = false;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut2.getHealth());
   uut2.ln = true;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut2.getHealth());
   uut2.ln = false;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut2.getHealth());
      // healthy string 5
   uut3.ln = true;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut3.getHealth());
      // unhealthy string 5
   uut3.ln = false;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut3.getHealth());
   TURETURN();
}


int main()
{
   GLOFNavHealth_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getHealthTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
