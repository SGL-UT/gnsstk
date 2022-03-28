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
#include "GPSCNav2ISC.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"
#include "UnixTime.hpp"
#include "TimeString.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GPSCNav2ISC_T
{
public:
   GPSCNav2ISC_T()
         : oid1(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L1,
                gnsstk::TrackingCode::CA),
           oid2(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L1,
                gnsstk::TrackingCode::L1CP),
           oid3(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L1,
                gnsstk::TrackingCode::L1CD),
           oid4(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L2,
                gnsstk::TrackingCode::L2CM),
           oid5(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L5,
                gnsstk::TrackingCode::L5I),
           oid6(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L5,
                gnsstk::TrackingCode::L5Q),
           oid7(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L1,
                gnsstk::TrackingCode::Standard),
           oid8(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L2,
                gnsstk::TrackingCode::Standard),
           oid9(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L5,
                gnsstk::TrackingCode::Standard),
           oid10(gnsstk::ObservationType::Unknown,
                 gnsstk::CarrierBand::G1,
                 gnsstk::TrackingCode::CA)
   {}
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
      /// Single frequency ISC test
   unsigned getISCSFTest();
      /// Dual frequency ISC test
   unsigned getISCDFTest();

   gnsstk::ObsID oid1, oid2, oid3, oid4, oid5, oid6, oid7, oid8, oid9, oid10;
};


unsigned GPSCNav2ISC_T ::
constructorTest()
{
   TUDEF("GPSCNav2ISC", "GPSCNav2ISC");
   gnsstk::GPSCNav2ISC uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::ISC,
             uut.signal.messageType);
   TUASSERTE(bool, false, uut.haveSF2);
   TUASSERTE(bool, false, uut.haveSF3);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime(), uut.xmit2);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime(), uut.xmit3);
   TUASSERTE(bool, true, std::isnan(uut.iscL1CP));
   TUASSERTE(bool, true, std::isnan(uut.iscL1CD));
   TUASSERTE(bool, true, std::isnan(uut.iscL1CA));
   TUASSERTE(bool, true, std::isnan(uut.iscL2C));
   TUASSERTE(bool, true, std::isnan(uut.iscL5I5));
   TUASSERTE(bool, true, std::isnan(uut.iscL5Q5));
   TURETURN();
}


unsigned GPSCNav2ISC_T ::
getUserTimeTest()
{
   TUDEF("GPSCNav2ISC", "getUserTime");
   gnsstk::GPSCNav2ISC uut;
      // using UnixTime because of rounding issues when trying to set
      // 480ms via the GPSWeekSecond constructor.
   gnsstk::CommonTime exp(gnsstk::UnixTime(1586044952, 480000,
                                           gnsstk::TimeSystem::GPS));
   std::cerr << gnsstk::printTime(exp, "exp=%Y/%03j/%08.2s") << std::endl;
   uut.xmit2 = gnsstk::GPSWeekSecond(2100,135.0);
   uut.xmit3 = gnsstk::GPSWeekSecond(2100,147.0);
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD,
                            gnsstk::NavType::GPSCNAV2),
      gnsstk::NavMessageType::ISC);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNav2ISC_T ::
getISCSFTest()
{
   TUDEF("GPSCNav2ISC", "getISC(single-frequency)");
   gnsstk::GPSCNav2ISC uut;
   double corr = 0.123456;
   uut.isc = -8.78935680E-09;
   uut.iscL1CP = -7.85803422E-10;
   uut.iscL1CD = -3.49245965E-10;
   uut.iscL1CA = -5.82076609E-10;
   uut.iscL2C = -6.60656951E-09;
   uut.iscL5I5 = -1.17870513E-08;
   uut.iscL5Q5 = -1.17870513E-08;

   double expCorr1 = uut.iscL1CA - uut.isc;
   double expCorr2 = uut.iscL1CP - uut.isc;
   double expCorr3 = uut.iscL1CD - uut.isc;
   double expCorr4 = uut.iscL2C - uut.isc;
   double expCorr5 = uut.iscL5I5 - uut.isc;
   double expCorr6 = uut.iscL5Q5 - uut.isc;
   double expCorrX = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr1, corr);
   TUASSERTE(bool, true, uut.getISC(oid2, corr));
   TUASSERTFE(expCorr2, corr);
   TUASSERTE(bool, true, uut.getISC(oid3, corr));
   TUASSERTFE(expCorr3, corr);
   TUASSERTE(bool, true, uut.getISC(oid4, corr));
   TUASSERTFE(expCorr4, corr);
   TUASSERTE(bool, true, uut.getISC(oid5, corr));
   TUASSERTFE(expCorr5, corr);
   TUASSERTE(bool, true, uut.getISC(oid6, corr));
   TUASSERTFE(expCorr6, corr);
      // and error tests
   corr = 0.123456;
   TUASSERTE(bool, false, uut.getISC(oid7, corr));
   TUASSERTFE(expCorrX, corr);
   TUASSERTE(bool, false, uut.getISC(oid8, corr));
   TUASSERTFE(expCorrX, corr);
   TUASSERTE(bool, false, uut.getISC(oid9, corr));
   TUASSERTFE(expCorrX, corr);
   TUASSERTE(bool, false, uut.getISC(oid10, corr));
   TUASSERTFE(expCorrX, corr);
   TURETURN();
}


unsigned GPSCNav2ISC_T ::
getISCDFTest()
{
   TUDEF("GPSCNav2ISC", "getISC(dual-frequency)");
   gnsstk::GPSCNav2ISC uut;
   double corr = 0.123456;
      /// @warning the truth values for corr have not been vetted
   double expCorrL5I = 2.2332319776213512011e-08;
   double expCorrL5Q = 2.2332319776213512011e-08;
   double expCorrL1CAL2 = 1.7519506229471444802e-08;
   double expCorrL1CPL2 = 1.7000873222053237761e-08;
   double expCorrL1CDL2 = 1.8112229667975519701e-08;
   uut.isc = -8.78935680E-09;
   uut.iscL1CP = -7.85803422E-10;
   uut.iscL1CD = -3.49245965E-10;
   uut.iscL1CA = -5.82076609E-10;
   uut.iscL2C = -6.60656951E-09;
   uut.iscL5I5 = -1.17870513E-08;
   uut.iscL5Q5 = -1.17870513E-08;
   TUASSERTE(bool, true, uut.getISC(oid1, oid5, corr));
   TUASSERTFE(expCorrL5I, corr);
   TUASSERTE(bool, true, uut.getISC(oid1, oid6, corr));
   TUASSERTFE(expCorrL5Q, corr);
   TUASSERTE(bool, false, uut.getISC(oid1, oid9, corr));
   TUASSERTE(bool, true, uut.getISC(oid1, oid4, corr));
   TUASSERTFE(expCorrL1CAL2, corr);
   TUASSERTE(bool, true, uut.getISC(oid2, oid4, corr));
   TUASSERTFE(expCorrL1CPL2, corr);
   TUASSERTE(bool, true, uut.getISC(oid3, oid4, corr));
   TUASSERTFE(expCorrL1CDL2, corr);
      // test error conditions
   TUASSERTE(bool, false, uut.getISC(oid7, oid2, corr));
   TUASSERTE(bool, false, uut.getISC(oid10, oid2, corr));
   TUASSERTE(bool, false, uut.getISC(oid1, oid8, corr));
   TUASSERTE(bool, false, uut.getISC(oid1, oid10, corr));
   TURETURN();
}


int main()
{
   GPSCNav2ISC_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getISCSFTest();
   errorTotal += testClass.getISCDFTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
