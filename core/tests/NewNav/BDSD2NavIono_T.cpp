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
#include "BDSD2NavIono.hpp"
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

class BDSD2NavIono_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned getIonoCorrTest();
};


unsigned BDSD2NavIono_T ::
constructorTest()
{
   TUDEF("BDSD2NavIono", "BDSD2NavIono");
   gnsstk::BDSD2NavIono uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Iono,
             uut.signal.messageType);
   TUASSERTE(uint32_t, 0, uut.pre);
   TUASSERTE(uint32_t, 0, uut.rev);
   TUASSERTE(unsigned, 0, uut.fraID);
   TUASSERTE(uint32_t, 0, uut.sow);
   TURETURN();
}


unsigned BDSD2NavIono_T ::
getUserTimeTest()
{
   TUDEF("BDSD2NavIono", "getUserTime");
   gnsstk::BDSD2NavIono uut;
   uut.timeStamp = gnsstk::BDSWeekSecond(2100,135.0);
   gnsstk::CommonTime exp(gnsstk::BDSWeekSecond(2100,135.0));
      // iono = 1 subframes * 6 seconds
   exp = exp + 6.0;
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned BDSD2NavIono_T ::
getIonoCorrTest()
{
   TUDEF("BDSD2NavIono", "getIonoCorr");
   gnsstk::BDSD2NavIono uut;
   gnsstk::CommonTime when = gnsstk::BDSWeekSecond(2100,135.0);
   gnsstk::Position rx, sv;
   rx.setECEF(-1575232.0141,-4707872.2332, 3993198.4383);
   sv.setECEF(18217581.007, -14220522.580,  12707796.859);
   uut.alpha[0] =  1.11758709E-08;
   uut.alpha[1] = -2.37159346E-09;
   uut.alpha[2] = -6.03921316E-09;
   uut.alpha[3] =  3.84468251E-09;
   uut.beta[0]  =  1.16736000E+05;
   uut.beta[1]  = -7.30126485E+04;
   uut.beta[2]  = -1.32803702E+04;
   uut.beta[3]  =  3.38181850E+04;
      // probably wouldn't use L1 or L2 for BDS D2 but this is a sanity check
   TUASSERTFE(13.174577965354167475,
              uut.getIonoCorr(when, rx, sv, gnsstk::CarrierBand::L2));
   TUASSERTFE(7.9994064218713107906,
              uut.getIonoCorr(when, rx, sv, gnsstk::CarrierBand::L1));
   TUASSERTFE(8.1468578153895165883,
              uut.getIonoCorr(when, rx, sv, gnsstk::CarrierBand::B1));
   TUASSERTFE(13.624958539291872839,
              uut.getIonoCorr(when, rx, sv, gnsstk::CarrierBand::B2));
   TUASSERTFE(12.338314431653227388,
              uut.getIonoCorr(when, rx, sv, gnsstk::CarrierBand::B3));
   TURETURN();
}


int main()
{
   BDSD2NavIono_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getIonoCorrTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
