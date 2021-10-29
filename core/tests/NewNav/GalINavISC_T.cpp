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
#include "GalINavISC.hpp"
#include "TestUtil.hpp"
#include "GALWeekSecond.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GalINavISC_T
{
public:
   GalINavISC_T();
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

   gnsstk::ObsID oid1, oid2, oid3, oid4, oid5, oid6, oid7;
};


GalINavISC_T ::
GalINavISC_T()
      : oid1(gnsstk::ObservationType::Unknown,
             gnsstk::CarrierBand::L1,
             gnsstk::TrackingCode::E1B),
        oid2(gnsstk::ObservationType::Unknown,
             gnsstk::CarrierBand::L1,
             gnsstk::TrackingCode::E1C),
        oid3(gnsstk::ObservationType::Unknown,
             gnsstk::CarrierBand::L5,
             gnsstk::TrackingCode::E5aI),
        oid4(gnsstk::ObservationType::SNR,
             gnsstk::CarrierBand::L5,
             gnsstk::TrackingCode::E5aQ),
        oid5(gnsstk::ObservationType::Unknown,
             gnsstk::CarrierBand::E5b,
             gnsstk::TrackingCode::E5bI),
        oid6(gnsstk::ObservationType::SNR,
             gnsstk::CarrierBand::E5b,
             gnsstk::TrackingCode::E5bQ),
        oid7(gnsstk::ObservationType::SNR,
             gnsstk::CarrierBand::L1,
             gnsstk::TrackingCode::CA)
{
}


unsigned GalINavISC_T ::
constructorTest()
{
   TUDEF("GalINavISC", "GalINavISC");
   gnsstk::GalINavISC uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::ISC,
             uut.signal.messageType);
   TUASSERTE(bool, true, std::isnan(uut.bgdE1E5a));
   TUASSERTE(bool, true, std::isnan(uut.bgdE1E5b));
   TURETURN();
}


unsigned GalINavISC_T ::
validateTest()
{
   TUDEF("GalINavISC", "validate");
   gnsstk::GalINavISC uut;
   TUASSERTE(bool, false, uut.validate());
   uut.bgdE1E5a = 0;
   TUASSERTE(bool, false, uut.validate());
   uut.bgdE1E5b = 0;
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}


unsigned GalINavISC_T ::
getUserTimeTest()
{
   TUDEF("GalINavISC", "getUserTime");
   gnsstk::GalINavISC uut;
   uut.timeStamp = gnsstk::GALWeekSecond(2100,141.0);
   gnsstk::CommonTime exp(uut.timeStamp + 2);
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::Galileo,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GalINAV),
      gnsstk::NavMessageType::ISC);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GalINavISC_T ::
getISCSFTest()
{
   TUDEF("GalINavISC", "getISC(single-frequency)");
   gnsstk::GalINavISC uut;
   double corr = 0.123456;
   const double gammaE1E5a = (1575.42/1176.45)*(1575.42/1176.45);
   const double gammaE1E5b = (1575.42/1207.14)*(1575.42/1207.14);
   const double expCorr1 = 0.123456;
   const double expCorr2 = 5.122274E-09;
   const double expCorr3 = gammaE1E5a * 4.656613E-09;
   const double expCorr4 = gammaE1E5b * expCorr2;
   TUASSERTE(bool, false, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr1, corr); // corr should not change.
      // set isc
   uut.bgdE1E5b = -5.122274E-09;
   TUASSERTE(bool, true, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr2, corr);
      // try again with same band, different code.
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid2, corr));
   TUASSERTFE(expCorr2, corr);
      // different band/code, uninitialized isc
   corr = 0.123456;
   TUASSERTE(bool, false, uut.getISC(oid3, corr));
      // set isc
   corr = 0.123456;
   uut.bgdE1E5a = -4.656613E-09;
   TUASSERTE(bool, true, uut.getISC(oid3, corr));
   TUASSERTFE(expCorr3, corr);
      // try again with same band, different code.
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid4, corr));
   TUASSERTFE(expCorr3, corr);
      // different band/code
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid5, corr));
   TUASSERTFE(expCorr4, corr);
      // try again with same band, different code.
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid6, corr));
   TUASSERTFE(expCorr4, corr);
      // try with unsupported signal
   corr = 0.123456;
   TUASSERTE(bool, false, uut.getISC(oid7, corr));
   TUASSERTFE(expCorr1, corr);
   TURETURN();
}


unsigned GalINavISC_T ::
getISCDFTest()
{
   TUDEF("GalINavISC", "getISC(dual-frequency)");
   gnsstk::GalINavISC uut;
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
   GalINavISC_T testClass;
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
