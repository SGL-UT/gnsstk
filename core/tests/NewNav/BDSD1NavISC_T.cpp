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
#include <math.h>
#include "BDSD1NavISC.hpp"
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


class BDSD1NavISC_T
{
public:
   BDSD1NavISC_T()
         : oid1(gpstk::ObservationType::Unknown,
                gpstk::CarrierBand::B1,
                gpstk::TrackingCode::B1I),
           oid2(gpstk::ObservationType::Unknown,
                gpstk::CarrierBand::B2,
                gpstk::TrackingCode::B2I),
           oid3(gpstk::ObservationType::Unknown,
                gpstk::CarrierBand::B3,
                gpstk::TrackingCode::B3I),
           oid4(gpstk::ObservationType::SNR,
                gpstk::CarrierBand::B2,
                gpstk::TrackingCode::B2I)
   {}
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
      /// Check validate()
   unsigned validateTest();
      /// Check getUserTime()
   unsigned getUserTimeTest();
      /// Single frequency ISC test
   unsigned getISCSFTest();
      /// Dual frequency ISC test
   unsigned getISCDFTest();

   gpstk::ObsID oid1, oid2, oid3, oid4;
};


unsigned BDSD1NavISC_T ::
constructorTest()
{
   TUDEF("BDSD1NavISC", "BDSD1NavISC");
   gpstk::BDSD1NavISC uut;
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::ISC,
             uut.signal.messageType);
   TUASSERTE(uint32_t, 0, uut.pre);
   TUASSERTE(uint32_t, 0, uut.rev);
   TUASSERTE(unsigned, 0, uut.fraID);
   TUASSERTE(uint32_t, 0, uut.sow);
   TUASSERTE(bool, true, std::isnan(uut.tgd1));
   TUASSERTE(bool, true, std::isnan(uut.tgd2));
   TURETURN();
}


unsigned BDSD1NavISC_T ::
validateTest()
{
   TUDEF("BDSD1NavISC", "validate");
   gpstk::BDSD1NavISC uut;
   uut.fraID = 1;
   uut.tgd1 = uut.tgd2 = 0;
   TUASSERTE(bool, true, uut.validate());
   uut.pre = 0;
   TUASSERTE(bool, true, uut.validate());
   uut.pre = 0x712;
   TUASSERTE(bool, true, uut.validate());
   uut.pre = 1;
   TUASSERTE(bool, false, uut.validate());
   uut.pre = 0x713;
   TUASSERTE(bool, false, uut.validate());
   TURETURN();
}


unsigned BDSD1NavISC_T ::
getUserTimeTest()
{
   TUDEF("BDSD1NavISC", "getUserTime");
   gpstk::BDSD1NavISC uut;
   uut.timeStamp = gpstk::BDSWeekSecond(2100,135.0);
   gpstk::CommonTime exp(gpstk::BDSWeekSecond(2100,135.0));
   exp = exp + 6.0;
   TUASSERTE(gpstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned BDSD1NavISC_T ::
getISCSFTest()
{
   TUDEF("BDSD1NavISC", "getISC(single-frequency)");
   gpstk::BDSD1NavISC uut;
      // isc should be nan
   double corr = 0.123456;
   double expCorr1 = 0.123456;
   double expCorr2 = 6.519258E-09;
   double expCorr3 = 3.581365E-09;
   TUASSERTE(bool, false, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr1, corr); // corr should not change.
      // set tgd1
   uut.tgd1 = -6.519258E-09;
   TUASSERTE(bool, true, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr2, corr);
      // set tgd2
   uut.tgd2 = -3.581365E-09;
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid2, corr));
   TUASSERTFE(expCorr3, corr);
      // and again with B3I
   corr = 0.123456;
   TUASSERTE(bool, false, uut.getISC(oid3, corr));
   TUASSERTFE(expCorr1, corr);
      // again but with an ObservationType other than "Unknown"
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid4, corr));
   TUASSERTFE(expCorr3, corr);
   TURETURN();
}


unsigned BDSD1NavISC_T ::
getISCDFTest()
{
   TUDEF("BDSD1NavISC", "getISC(dual-frequency)");
   gpstk::BDSD1NavISC uut;
   double corr = 0.123456;
   uut.tgd1 = -6.519258E-09;
   TUASSERTE(bool, true, uut.getISC(oid1, oid3, corr));
   TUASSERTFE(1.9190620929478330958e-08, corr);
   corr = 0.123456;
   TUASSERTE(bool, false, uut.getISC(oid3, oid1, corr));
   TUASSERTFE(0.123456, corr);
   TURETURN();
}


int main()
{
   BDSD1NavISC_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.validateTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getISCSFTest();
   errorTotal += testClass.getISCDFTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
