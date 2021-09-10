//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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
#include <math.h>
#include "GPSLNavISC.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class GPSLNavISC_T
{
public:
   GPSLNavISC_T()
         : oid1(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L1,
                gnsstk::TrackingCode::CA),
           oid2(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L1,
                gnsstk::TrackingCode::Y),
           oid3(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L2,
                gnsstk::TrackingCode::Y),
           oid4(gnsstk::ObservationType::SNR,
                gnsstk::CarrierBand::L2,
                gnsstk::TrackingCode::Y)
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

   gnsstk::ObsID oid1, oid2, oid3, oid4;
};


unsigned GPSLNavISC_T ::
constructorTest()
{
   TUDEF("GPSLNavISC", "GPSLNavISC");
   gnsstk::GPSLNavISC uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::ISC,
             uut.signal.messageType);
   TUASSERTE(uint32_t, 0, uut.pre);
   TUASSERTE(uint32_t, 0, uut.tlm);
   TUASSERTE(bool, false, uut.alert);
   TUASSERTE(bool, false, uut.asFlag);
   TURETURN();
}


unsigned GPSLNavISC_T ::
validateTest()
{
   TUDEF("GPSLNavISC", "validate");
   gnsstk::GPSLNavISC uut;
   TUASSERTE(bool, true, uut.validate());
   uut.pre = 0;
   TUASSERTE(bool, true, uut.validate());
   uut.pre = 0x8b;
   TUASSERTE(bool, true, uut.validate());
   uut.pre = 1;
   TUASSERTE(bool, false, uut.validate());
   uut.pre = 0x8c;
   TUASSERTE(bool, false, uut.validate());
   TURETURN();
}


unsigned GPSLNavISC_T ::
getUserTimeTest()
{
   TUDEF("GPSLNavISC", "getUserTime");
   gnsstk::GPSLNavISC uut;
   uut.timeStamp = gnsstk::GPSWeekSecond(2100,135.0);
   gnsstk::CommonTime exp(gnsstk::GPSWeekSecond(2100,135.0));
   exp = exp + 6.0;
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GPSLNavISC_T ::
getISCSFTest()
{
   TUDEF("GPSLNavISC", "getISC(single-frequency)");
   gnsstk::GPSLNavISC uut;
      // isc should be nan
   double corr = 0.123456;
   double expCorr1 = 0.123456;
   double expCorr2 = 6.519258E-09;
   double expCorr3 = 1.0736855745e-08;
   TUASSERTE(bool, false, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr1, corr); // corr should not change.
      // set isc
   uut.isc = -6.519258E-09;
   TUASSERTE(bool, true, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr2, corr);
      // try again with same band, different code.
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid2, corr));
   TUASSERTFE(expCorr2, corr);
      // and again with L2-Y
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid3, corr));
   TUASSERTFE(expCorr3, corr);
      // again but with an ObservationType other than "Unknown"
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid4, corr));
   TUASSERTFE(expCorr3, corr);
   TURETURN();
}


unsigned GPSLNavISC_T ::
getISCDFTest()
{
   TUDEF("GPSLNavISC", "getISC(dual-frequency)");
   gnsstk::GPSLNavISC uut;
   double corr = 0.123456;
      // dual frequency getISC should always return 0 for this class
   TUASSERTE(bool, true, uut.getISC(oid1, oid3, corr));
   TUASSERTFE(0, corr);
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid3, oid1, corr));
   TUASSERTFE(0, corr);
   TURETURN();
}


int main()
{
   GPSLNavISC_T testClass;
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
