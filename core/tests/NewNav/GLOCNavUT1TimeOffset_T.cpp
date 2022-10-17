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
#include "GLOCNavUT1TimeOffset.hpp"
#include "CivilTime.hpp"
#include "GLONASSTime.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class GLOCNavUT1TimeOffset_T
{
public:
   unsigned constructorTest();
   unsigned validateTest();
   unsigned getUserTimeTest();
   unsigned getOffsetTest();
   unsigned getConversionsTest();
};


unsigned GLOCNavUT1TimeOffset_T ::
constructorTest()
{
   TUDEF("GLOCNavUT1TimeOffset", "GLOCNavUT1TimeOffset");
   gnsstk::GLOCNavUT1TimeOffset uut;
   TUASSERTFE(0.0, uut.B0);
   TUASSERTFE(0.0, uut.B1);
   TUASSERTFE(0.0, uut.B2);
   TUASSERTE(unsigned, -1, uut.NB);
   TUASSERTE(int, 1, isnan(uut.UTCTAI));
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime(), uut.refTime);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::TimeOffset,
             uut.signal.messageType);
   TURETURN();
}


unsigned GLOCNavUT1TimeOffset_T ::
validateTest()
{
   TUDEF("GLOCNavUT1TimeOffset", "validate");
   gnsstk::GLOCNavUT1TimeOffset uut;
      /// @todo Implement some testing when the function has some meat to it.
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}


unsigned GLOCNavUT1TimeOffset_T ::
getUserTimeTest()
{
   TUDEF("GLOCNavUT1TimeOffset", "getUserTime");
   gnsstk::GLOCNavUT1TimeOffset uut;
   uut.timeStamp = gnsstk::CivilTime(2006, 10, 01, 0, 15, 0,
                                     gnsstk::TimeSystem::GLO);
      // 1x 2s string
   gnsstk::CommonTime exp(uut.timeStamp + 3.0);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GLOCNavUT1TimeOffset_T ::
getOffsetTest()
{
   TUDEF("GLOCNavUT1TimeOffset", "getOffset");
   gnsstk::GLOCNavUT1TimeOffset uut;
   uut.B0 = -9.692382812E-02;
   uut.B1 = 5.950927734E-04;
   uut.B2 = 0.0;
   uut.NB = 853;
   uut.refTime = gnsstk::GLONASSTime(7, uut.NB);
   uut.UTCTAI = -37;
   double offset;
   gnsstk::CommonTime when1(uut.refTime),
      when2(gnsstk::GLONASSTime(7, uut.NB, 43208));
      /// @todo Truth values here need to be verified.
   TUASSERT(uut.getOffset(gnsstk::TimeSystem::GLO, gnsstk::TimeSystem::UTC,
                          when1, offset));
   TUASSERTFE(-0.096923828119999999586, offset);
   TUASSERT(uut.getOffset(gnsstk::TimeSystem::GLO, gnsstk::TimeSystem::UTC,
                          when2, offset));
   TUASSERTFE(-0.096626226632117270809, offset);
   TURETURN();
}


unsigned GLOCNavUT1TimeOffset_T ::
getConversionsTest()
{
   TUDEF("GLOCNavUT1TimeOffset", "getConversions");
   gnsstk::TimeCvtSet convs;
   gnsstk::GLOCNavUT1TimeOffset uut;
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
   GLOCNavUT1TimeOffset_T testClass;
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
