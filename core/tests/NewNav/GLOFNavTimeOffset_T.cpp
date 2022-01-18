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
#include "TestUtil.hpp"
#include "GLOFNavTimeOffset.hpp"
#include "CivilTime.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class GLOFNavTimeOffset_T
{
public:
   unsigned constructorTest();
   unsigned validateTest();
   unsigned getUserTimeTest();
   unsigned getOffsetTest();
   unsigned getConversionsTest();
};


unsigned GLOFNavTimeOffset_T ::
constructorTest()
{
   TUDEF("GLOFNavTimeOffset", "GLOFNavTimeOffset");
   gnsstk::GLOFNavTimeOffset uut;
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GLO, uut.src);
   TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GPS, uut.tgt);
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


unsigned GLOFNavTimeOffset_T ::
validateTest()
{
   TUDEF("GLOFNavTimeOffset", "validate");
   gnsstk::GLOFNavTimeOffset uut;
      /// @todo Implement some testing when the function has some meat to it.
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}


unsigned GLOFNavTimeOffset_T ::
getUserTimeTest()
{
   TUDEF("GLOFNavTimeOffset", "getUserTime");
   gnsstk::GLOFNavTimeOffset uut;
   uut.timeStamp = gnsstk::CivilTime(2006, 10, 01, 0, 15, 0,
                                     gnsstk::TimeSystem::GLO);
      // 1x 2s string
   gnsstk::CommonTime exp(uut.timeStamp + 2.0);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GLOFNavTimeOffset_T ::
getOffsetTest()
{
   TUDEF("GLOFNavTimeOffset", "getOffset");
   gnsstk::GLOFNavTimeOffset uut;
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(
         gnsstk::SatID(109,gnsstk::SatelliteSystem::Glonass),
         gnsstk::SatID(109,gnsstk::SatelliteSystem::Glonass),
         gnsstk::ObsID(gnsstk::ObservationType::NavMsg,
                       gnsstk::CarrierBand::G1,
                       gnsstk::TrackingCode::Standard,
                       2),
         gnsstk::NavID(gnsstk::NavType::GloCivilF)),
      gnsstk::NavMessageType::TimeOffset);
   uut.timeStamp = gnsstk::CivilTime(2021,10,31,0,0,8,gnsstk::TimeSystem::GLO);
   gnsstk::CommonTime when1(uut.timeStamp),
      when2(gnsstk::CivilTime(2021,11,6,0,0,8,gnsstk::TimeSystem::GLO));
   uut.deltatLS = 18.0;
   uut.a0 = 1.708985306E-03;
   uut.refTime = uut.effTime = uut.timeStamp;
   double offset;
      /// @todo Truth values here need to be verified.
   TUASSERT(uut.getOffset(gnsstk::TimeSystem::GLO, gnsstk::TimeSystem::GPS,
                          when1, offset));
   TUASSERTFE(18.001708985305999278, offset);
   TUASSERT(uut.getOffset(gnsstk::TimeSystem::GLO, gnsstk::TimeSystem::GPS,
                          when2, offset));
   TUASSERTFE(18.001708985305999278, offset);
   TURETURN();
}


unsigned GLOFNavTimeOffset_T ::
getConversionsTest()
{
   TUDEF("GLOFNavTimeOffset", "getConversions");
   gnsstk::TimeCvtSet convs;
   gnsstk::GLOFNavTimeOffset offs;
   gnsstk::TimeCvtKey key1(gnsstk::TimeSystem::GPS,
                                          gnsstk::TimeSystem::GLO);
   gnsstk::TimeCvtKey key2(gnsstk::TimeSystem::GLO,
                                          gnsstk::TimeSystem::GPS);
   TUCATCH(convs = offs.getConversions());
   TUASSERTE(size_t, 2, convs.size());
   TUASSERTE(size_t, 1, convs.count(key1));
   TUASSERTE(size_t, 1, convs.count(key2));
   TURETURN();
}


int main()
{
   GLOFNavTimeOffset_T testClass;
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
