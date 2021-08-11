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
#include "BDSD1NavAlm.hpp"
#include "TestUtil.hpp"
#include "BDSWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
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
   gpstk::BDSD1NavAlm uut;
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Almanac,
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
   gpstk::BDSD1NavAlm uut;
   uut.timeStamp = gpstk::BDSWeekSecond(2100,135.0);
   gpstk::CommonTime exp(gpstk::BDSWeekSecond(2100,135.0));
      // almanac = 1 subframes * 6 seconds
      // We don't take the timestamp of the WNa page into account here.
   exp = exp + 6.0;
   TUASSERTE(gpstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned BDSD1NavAlm_T ::
fixFitTest()
{
   TUDEF("BDSD1NavAlm", "fixFit");
   gpstk::CommonTime expBegin = gpstk::BDSWeekSecond(2100,135.0);
   gpstk::CommonTime expEnd   = gpstk::CommonTime::END_OF_TIME;
   gpstk::BDSD1NavAlm uut;
   uut.xmitTime = gpstk::BDSWeekSecond(2100,135.0);
   TUCATCH(uut.fixFit());
   TUASSERTE(gpstk::CommonTime, expBegin, uut.beginFit);
   TUASSERTE(gpstk::CommonTime, expEnd, uut.endFit);
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
