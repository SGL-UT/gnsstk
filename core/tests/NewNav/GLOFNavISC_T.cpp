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
#include <math.h>
#include "GLOFNavISC.hpp"
#include "TestUtil.hpp"
#include "CivilTime.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class GLOFNavISC_T
{
public:
   GLOFNavISC_T()
         : oid1(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::G1,
                gnsstk::TrackingCode::Standard),
           oid2(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::G2,
                gnsstk::TrackingCode::Standard),
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


unsigned GLOFNavISC_T ::
constructorTest()
{
   TUDEF("GLOFNavISC", "GLOFNavISC");
   gnsstk::GLOFNavISC uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::ISC,
             uut.signal.messageType);
   TURETURN();
}


unsigned GLOFNavISC_T ::
validateTest()
{
   TUDEF("GLOFNavISC", "validate");
   gnsstk::GLOFNavISC uut;
      /// @todo Implement some testing when the function has some meat to it.
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}


unsigned GLOFNavISC_T ::
getUserTimeTest()
{
   TUDEF("GLOFNavISC", "getUserTime");
   gnsstk::GLOFNavISC uut;
   uut.timeStamp = gnsstk::CivilTime(2006, 10, 01, 0, 15, 0,
                                     gnsstk::TimeSystem::GLO);
      // 1x 2s string
   gnsstk::CommonTime exp(uut.timeStamp + 2.0);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GLOFNavISC_T ::
getISCSFTest()
{
   TUDEF("GLOFNavISC", "getISC(single-frequency)");
   gnsstk::GLOFNavISC uut;
      // isc is initially nan
   double corr = 0.123456;
   double expCorr1 = 0.123456;
   TUASSERTE(bool, false, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr1, corr); // corr should not change.
      // set isc
   uut.isc = -6.519258E-09;
   TUASSERTE(bool, false, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr1, corr);
      // try again with same band, different code.
   TUASSERTE(bool, false, uut.getISC(oid2, corr));
   TUASSERTFE(expCorr1, corr);
      // and again with L2-Y
   TUASSERTE(bool, false, uut.getISC(oid3, corr));
   TUASSERTFE(expCorr1, corr);
      // again but with an ObservationType other than "Unknown"
   TUASSERTE(bool, false, uut.getISC(oid4, corr));
   TUASSERTFE(expCorr1, corr);
   TURETURN();
}


unsigned GLOFNavISC_T ::
getISCDFTest()
{
   TUDEF("GLOFNavISC", "getISC(dual-frequency)");
   gnsstk::GLOFNavISC uut;
   uut.isc = 987654;
   double corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid1, oid2, corr));
   TUASSERTFE(987654, corr);
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid2, oid1, corr));
   TUASSERTFE(-987654, corr);
   TUASSERTE(bool, false, uut.getISC(oid1, oid3, corr));
   TUASSERTE(bool, false, uut.getISC(oid1, oid4, corr));
   TUASSERTE(bool, false, uut.getISC(oid2, oid3, corr));
   TUASSERTE(bool, false, uut.getISC(oid2, oid4, corr));
   TUASSERTE(bool, false, uut.getISC(oid3, oid1, corr));
   TUASSERTE(bool, false, uut.getISC(oid3, oid2, corr));
   TUASSERTE(bool, false, uut.getISC(oid4, oid1, corr));
   TUASSERTE(bool, false, uut.getISC(oid4, oid2, corr));
   TURETURN();
}


int main()
{
   GLOFNavISC_T testClass;
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
