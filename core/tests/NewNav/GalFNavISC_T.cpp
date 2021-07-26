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
#include "GalFNavISC.hpp"
#include "TestUtil.hpp"
#include "GALWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GalFNavISC_T
{
public:
   GalFNavISC_T();
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

   gpstk::ObsID oid1, oid2, oid3, oid4, oid5, oid6, oid7;
};


GalFNavISC_T ::
GalFNavISC_T()
      : oid1(gpstk::ObservationType::Unknown,
             gpstk::CarrierBand::L1,
             gpstk::TrackingCode::E1B),
        oid2(gpstk::ObservationType::Unknown,
             gpstk::CarrierBand::L1,
             gpstk::TrackingCode::E1C),
        oid3(gpstk::ObservationType::Unknown,
             gpstk::CarrierBand::L5,
             gpstk::TrackingCode::E5aI),
        oid4(gpstk::ObservationType::SNR,
             gpstk::CarrierBand::L5,
             gpstk::TrackingCode::E5aQ),
        oid5(gpstk::ObservationType::Unknown,
             gpstk::CarrierBand::E5b,
             gpstk::TrackingCode::E5bI),
        oid6(gpstk::ObservationType::SNR,
             gpstk::CarrierBand::E5b,
             gpstk::TrackingCode::E5bQ),
        oid7(gpstk::ObservationType::SNR,
             gpstk::CarrierBand::L1,
             gpstk::TrackingCode::CA)
{
}


unsigned GalFNavISC_T ::
constructorTest()
{
   TUDEF("GalFNavISC", "GalFNavISC");
   gpstk::GalFNavISC uut;
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::ISC,
             uut.signal.messageType);
   TUASSERTE(bool, true, std::isnan(uut.isc));
   TURETURN();
}


unsigned GalFNavISC_T ::
validateTest()
{
   TUDEF("GalFNavISC", "validate");
   gpstk::GalFNavISC uut;
   TUASSERTE(bool, false, uut.validate());
   uut.isc = 0;
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}


unsigned GalFNavISC_T ::
getUserTimeTest()
{
   TUDEF("GalFNavISC", "getUserTime");
   gpstk::GalFNavISC uut;
   uut.timeStamp = gpstk::GALWeekSecond(2100,141.0);
   gpstk::CommonTime exp(uut.timeStamp + 10);
   uut.signal = gpstk::NavMessageID(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::Galileo,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::E5aI,
                            gpstk::NavType::GalFNAV),
      gpstk::NavMessageType::ISC);
   TUASSERTE(gpstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GalFNavISC_T ::
getISCSFTest()
{
   TUDEF("GalFNavISC", "getISC(single-frequency)");
   gpstk::GalFNavISC uut;
   double corr = 0.123456;
   const double gammaE1E5a = (1575.42/1176.45)*(1575.42/1176.45);
   const double gammaE1E5b = (1575.42/1207.14)*(1575.42/1207.14);
   const double expCorr1 = 0.123456;
   const double expCorr2 = gammaE1E5a * 4.656613E-09;
   TUASSERTE(bool, false, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr1, corr); // corr should not change.
      // set isc
   uut.bgdE1E5a = -4.656613E-09;
   TUASSERTE(bool, false, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr1, corr);
      // try again with same band, different code.
   corr = 0.123456;
   TUASSERTE(bool, false, uut.getISC(oid2, corr));
   TUASSERTFE(expCorr1, corr);
      // different band/code
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid3, corr));
   TUASSERTFE(expCorr2, corr);
      // try again with same band, different code.
   corr = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid4, corr));
   TUASSERTFE(expCorr2, corr);
      // different band/code
   corr = 0.123456;
   TUASSERTE(bool, false, uut.getISC(oid5, corr));
   TUASSERTFE(expCorr1, corr);
      // try again with same band, different code.
   corr = 0.123456;
   TUASSERTE(bool, false, uut.getISC(oid6, corr));
   TUASSERTFE(expCorr1, corr);
      // try with unsupported signal
   corr = 0.123456;
   TUASSERTE(bool, false, uut.getISC(oid7, corr));
   TUASSERTFE(expCorr1, corr);
   TURETURN();
}


unsigned GalFNavISC_T ::
getISCDFTest()
{
   TUDEF("GalFNavISC", "getISC(dual-frequency)");
   gpstk::GalFNavISC uut;
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
   GalFNavISC_T testClass;
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
