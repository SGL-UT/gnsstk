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
#include "GLOCNavHealth.hpp"
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

class GLOCNavHealth_T
{
public:
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned getHealthTest();
};


unsigned GLOCNavHealth_T ::
constructorTest()
{
   TUDEF("GLOCNavHealth", "GLOCNavHealth");
   gnsstk::GLOCNavHealth uut;
   TUASSERTE(bool, true, uut.Hj);
   TUASSERTE(bool, true, uut.lj);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Health,
             uut.signal.messageType);
   TURETURN();
}


unsigned GLOCNavHealth_T ::
getUserTimeTest()
{
   TUDEF("GLOCNavHealth", "getUserTime");
   gnsstk::GLOCNavHealth uut;
   uut.timeStamp = gnsstk::CivilTime(2006, 10, 01, 0, 15, 0,
                                     gnsstk::TimeSystem::GLO);
      // 1x 2s string
   gnsstk::CommonTime exp(uut.timeStamp + 3.0);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GLOCNavHealth_T ::
getHealthTest()
{
   TUDEF("GLOCNavHealth", "getHealth");
   gnsstk::GLOCNavHealth uut;
      // default should be unhealthy
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut.getHealth());
   uut.Hj = false;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut.getHealth());
   uut.lj = false;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy, uut.getHealth());
   uut.Hj = true;
   TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy, uut.getHealth());
   TURETURN();
}


int main()
{
   GLOCNavHealth_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getHealthTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
