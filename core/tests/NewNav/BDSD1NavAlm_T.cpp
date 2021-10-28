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
#include "BDSD1NavAlm.hpp"
#include "TestUtil.hpp"
#include "BDSWeekSecond.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class BDSD1NavAlm_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
};


unsigned BDSD1NavAlm_T ::
constructorTest()
{
   TUDEF("BDSD1NavAlm", "BDSD1NavAlm");
   gnsstk::BDSD1NavAlm uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Almanac,
             uut.signal.messageType);
   TUASSERTE(unsigned, 0xff, uut.pnum);
   TUASSERTFE(0.0, uut.deltai);
   TUASSERTFE(0.0, uut.toa);
   TUASSERTE(uint16_t, 0xffff, uut.healthBits);
   TUASSERTE(bool, false, uut.isDefault);
   TURETURN();
}


unsigned BDSD1NavAlm_T ::
getUserTimeTest()
{
   TUDEF("BDSD1NavAlm", "getUserTime");
   gnsstk::BDSD1NavAlm uut;
   uut.timeStamp = gnsstk::BDSWeekSecond(2100,135.0);
   gnsstk::CommonTime exp(gnsstk::BDSWeekSecond(2100,135.0));
      // almanac = 1 subframes * 6 seconds
      // We don't take the timestamp of the WNa page into account here.
   exp = exp + 6.0;
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned BDSD1NavAlm_T ::
fixFitTest()
{
   TUDEF("BDSD1NavAlm", "fixFit");
   gnsstk::CommonTime expBegin = gnsstk::BDSWeekSecond(2100,135.0);
   gnsstk::CommonTime expEnd   = gnsstk::CommonTime::END_OF_TIME;
   gnsstk::BDSD1NavAlm uut;
   uut.xmitTime = gnsstk::BDSWeekSecond(2100,135.0);
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, expBegin, uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, expEnd, uut.endFit);
   TURETURN();
}


int main()
{
   BDSD1NavAlm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
