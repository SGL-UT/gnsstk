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
#include <math.h>
#include "TestUtil.hpp"
#include "GLOFNavUT1TimeOffset.hpp"
#include "CivilTime.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class GLOFNavUT1TimeOffset_T
{
public:
   unsigned constructorTest();
   unsigned validateTest();
   unsigned getUserTimeTest();
   unsigned getOffsetTest();
   unsigned getConversionsTest();
};


unsigned GLOFNavUT1TimeOffset_T ::
constructorTest()
{
   TUDEF("GLOFNavUT1TimeOffset", "GLOFNavUT1TimeOffset");
   gnsstk::GLOFNavUT1TimeOffset uut;
   TUASSERTE(int, 1, isnan(uut.tauc));
   TUASSERTE(int, 1, isnan(uut.B1));
   TUASSERTE(int, 1, isnan(uut.B2));
   TUASSERTE(unsigned, -1, uut.KP);
   TUASSERTE(unsigned, -1, uut.NT);
   TUASSERTE(unsigned, -1, uut.NA);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::TimeOffset,
             uut.signal.messageType);
   TURETURN();
}


unsigned GLOFNavUT1TimeOffset_T ::
validateTest()
{
   TUDEF("GLOFNavUT1TimeOffset", "validate");
   gnsstk::GLOFNavUT1TimeOffset uut;
      /// @todo Implement some testing when the function has some meat to it.
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}


unsigned GLOFNavUT1TimeOffset_T ::
getUserTimeTest()
{
   TUDEF("GLOFNavUT1TimeOffset", "getUserTime");
   gnsstk::GLOFNavUT1TimeOffset uut;
   uut.timeStamp = gnsstk::CivilTime(2006, 10, 01, 0, 15, 0,
                                     gnsstk::TimeSystem::GLO);
      // 1x 2s string
   gnsstk::CommonTime exp(uut.timeStamp + 2.0);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GLOFNavUT1TimeOffset_T ::
getOffsetTest()
{
   TUDEF("GLOFNavUT1TimeOffset", "getOffset");
   gnsstk::GLOFNavUT1TimeOffset uut;
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(
         gnsstk::SatID(414,gnsstk::SatelliteSystem::Glonass),
         gnsstk::SatID(414,gnsstk::SatelliteSystem::Glonass),
         gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                       gnsstk::CarrierBand::G1,
                       gnsstk::TrackingCode::Standard,
                       7),
         gnsstk::NavID(gnsstk::NavType::GloCivilF)),
      gnsstk::NavMessageType::TimeOffset);
   uut.timeStamp = gnsstk::CivilTime(2021,10,31,20,32,26,
                                     gnsstk::TimeSystem::GLO);
   uut.tauc = 4.656613E-10;
   uut.B1 = -1.035156E-01;
   uut.B2 = -3.051758E-04;
   uut.KP = 0;
   uut.NT = 670;
   uut.NA = 669;
   gnsstk::CommonTime when1(uut.timeStamp),
      when2(gnsstk::CivilTime(2021,11,6,0,0,8,gnsstk::TimeSystem::GLO));
   double offset;
      /// @todo Truth values here need to be verified.
   TUASSERT(uut.getOffset(gnsstk::TimeSystem::GLO, gnsstk::TimeSystem::UTC,
                          when1, offset));
   TUASSERTFE(-10800.103820775333588, offset);
   TUASSERT(uut.getOffset(gnsstk::TimeSystem::GLO, gnsstk::TimeSystem::UTC,
                          when2, offset));
   TUASSERTFE(-10800.103820775333588, offset);
   TURETURN();
}


unsigned GLOFNavUT1TimeOffset_T ::
getConversionsTest()
{
   TUDEF("GLOFNavUT1TimeOffset", "getConversions");
   gnsstk::TimeCvtSet convs;
   gnsstk::GLOFNavUT1TimeOffset uut;
   gnsstk::TimeCvtKey key1(gnsstk::TimeSystem::UTC, gnsstk::TimeSystem::GLO);
   gnsstk::TimeCvtKey key2(gnsstk::TimeSystem::GLO, gnsstk::TimeSystem::UTC);
   TUCATCH(convs = uut.getConversions());
   TUASSERTE(size_t, 2, convs.size());
   TUASSERTE(size_t, 1, convs.count(key1));
   TUASSERTE(size_t, 1, convs.count(key2));
   TURETURN();
}


int main()
{
   GLOFNavUT1TimeOffset_T testClass;
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
