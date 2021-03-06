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
#include "GPSCNavISC.hpp"
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

class GPSCNavISC_T
{
public:
   GPSCNavISC_T()
         : oid1(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L1,
                gnsstk::TrackingCode::CA),
           oid2(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L2,
                gnsstk::TrackingCode::L2CM),
           oid3(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L2,
                gnsstk::TrackingCode::L2CL),
           oid4(gnsstk::ObservationType::Unknown,
                gnsstk::CarrierBand::L2,
                gnsstk::TrackingCode::L2CML),
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
   unsigned validateTest();
   unsigned getUserTimeTest();
      /// Single frequency ISC test
   unsigned getISCSFTest();
      /// Dual frequency ISC test
   unsigned getISCDFTest();

   gnsstk::ObsID oid1, oid2, oid3, oid4, oid5, oid6, oid7, oid8, oid9, oid10;
};


unsigned GPSCNavISC_T ::
constructorTest()
{
   TUDEF("GPSCNavISC", "GPSCNavISC");
   gnsstk::GPSCNavISC uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::ISC,
             uut.signal.messageType);
   TUASSERTE(uint32_t, 0, uut.pre);
   TUASSERTE(bool, false, uut.alert);
   TUASSERTE(bool, true, std::isnan(uut.iscL1CA));
   TUASSERTE(bool, true, std::isnan(uut.iscL2C));
   TUASSERTE(bool, true, std::isnan(uut.iscL5I5));
   TUASSERTE(bool, true, std::isnan(uut.iscL5Q5));
   TURETURN();
}


unsigned GPSCNavISC_T ::
validateTest()
{
   TUDEF("GPSCNavISC", "validate");
   gnsstk::GPSCNavISC uut;
   TUASSERTE(bool, true, uut.validate());
   uut.pre = 0x8b;
   TUASSERTE(bool, true, uut.validate());
   uut.pre = 0x8c;
   TUASSERTE(bool, false, uut.validate());
   TURETURN();
}


unsigned GPSCNavISC_T ::
getUserTimeTest()
{
   TUDEF("GPSCNavISC", "getUserTime");
   gnsstk::GPSCNavISC uut;
      // L2 has a 12s cadence, L5 has a 6s cadence
   gnsstk::CommonTime expL2(gnsstk::GPSWeekSecond(2100,147.0));
   gnsstk::CommonTime expL5(gnsstk::GPSWeekSecond(2100,141.0));
   uut.timeStamp = gnsstk::GPSWeekSecond(2100,135.0);
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I,
                            gnsstk::NavType::GPSCNAVL5),
      gnsstk::NavMessageType::ISC);
   TUASSERTE(gnsstk::CommonTime, expL5, uut.getUserTime());
   uut.signal = gnsstk::NavMessageID(
      gnsstk::NavSatelliteID(1, 1, gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CM,
                            gnsstk::NavType::GPSCNAVL2),
      gnsstk::NavMessageType::ISC);
   TUASSERTE(gnsstk::CommonTime, expL2, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNavISC_T ::
getISCSFTest()
{
   TUDEF("GPSCNavISC", "getISC(single-frequency)");
   gnsstk::GPSCNavISC uut;
   double corr = 0.123456;
   uut.isc     =  5.58793545E-09;
   uut.iscL1CA = -3.49245965E-10;
   uut.iscL2C  = -3.14321369E-09;
   uut.iscL5I5 =  6.43194653E-09;
   uut.iscL5Q5 =  6.54836185E-09;
   double expCorr1 = uut.iscL1CA - uut.isc;
   double expCorr2 = uut.iscL2C - uut.isc;
   double expCorr5I = uut.iscL5I5 - uut.isc;
   double expCorr5Q = uut.iscL5Q5 - uut.isc;
   double expCorrX = 0.123456;
   TUASSERTE(bool, true, uut.getISC(oid1, corr));
   TUASSERTFE(expCorr1, corr);
   TUASSERTE(bool, true, uut.getISC(oid2, corr));
   TUASSERTFE(expCorr2, corr);
   TUASSERTE(bool, true, uut.getISC(oid3, corr));
   TUASSERTFE(expCorr2, corr);
   TUASSERTE(bool, true, uut.getISC(oid4, corr));
   TUASSERTFE(expCorr2, corr);
   TUASSERTE(bool, true, uut.getISC(oid5, corr));
   TUASSERTFE(expCorr5I, corr);
   TUASSERTE(bool, true, uut.getISC(oid6, corr));
   TUASSERTFE(expCorr5Q, corr);
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


unsigned GPSCNavISC_T ::
getISCDFTest()
{
   TUDEF("GPSCNavISC", "getISC(dual-frequency)");
   gnsstk::GPSCNavISC uut;
   double corr = 0.123456;
      /// @warning the truth values for corr have not been vetted
   double expCorrL5I = -1.4485582019935180719e-08;
   double expCorrL5Q = -1.4632335676116669653e-08;
   double expCorrL2 = -1.618467885588237585e-09;
   uut.isc     =  5.58793545E-09;
   uut.iscL1CA = -3.49245965E-10;
   uut.iscL2C  = -3.14321369E-09;
   uut.iscL5I5 =  6.43194653E-09;
   uut.iscL5Q5 =  6.54836185E-09;
   TUASSERTE(bool, true, uut.getISC(oid1, oid5, corr));
   TUASSERTFE(expCorrL5I, corr);
   TUASSERTE(bool, true, uut.getISC(oid1, oid6, corr));
   TUASSERTFE(expCorrL5Q, corr);
   TUASSERTE(bool, false, uut.getISC(oid1, oid9, corr));
   TUASSERTE(bool, true, uut.getISC(oid1, oid2, corr));
   TUASSERTFE(expCorrL2, corr);
   TUASSERTE(bool, true, uut.getISC(oid1, oid3, corr));
   TUASSERTFE(expCorrL2, corr);
   TUASSERTE(bool, true, uut.getISC(oid1, oid4, corr));
   TUASSERTFE(expCorrL2, corr);
      // test error conditions
   TUASSERTE(bool, false, uut.getISC(oid7, oid2, corr));
   TUASSERTE(bool, false, uut.getISC(oid10, oid2, corr));
   TUASSERTE(bool, false, uut.getISC(oid1, oid8, corr));
   TUASSERTE(bool, false, uut.getISC(oid1, oid10, corr));
   TURETURN();
}


int main()
{
   GPSCNavISC_T testClass;
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
