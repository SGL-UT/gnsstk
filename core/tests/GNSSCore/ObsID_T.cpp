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

#include "ObsID.hpp"

#include "TestUtil.hpp"
#include "Rinex3ObsHeader.hpp"
#include <iostream>
#include <sstream>

namespace gnsstk
{
      // define some stream operators so that test failures involving
      // enums are a bit more readable.

   std::ostream& operator<<(std::ostream& s, gnsstk::ObservationType e)
   {
      s << (long)e << " (" << StringUtils::asString(e) << ")";
      return s;
   }
   std::ostream& operator<<(std::ostream& s, CarrierBand e)
   {
      s << (long)e << " (" << StringUtils::asString(e) << ")";
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gnsstk::TrackingCode e)
   {
      s << (long)e << " (" << StringUtils::asString(e) << ")";
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gnsstk::XmitAnt e)
   {
      s << (long)e << " (" << StringUtils::asString(e) << ")";
      return s;
   }
}

#define CBDESCTEST(EXP, CARRIERBAND) TUASSERTE(std::string, EXP, gnsstk::ObsID::cbDesc[CARRIERBAND])
#define TCDESCTEST(EXP, TRACKINGCODE) TUASSERTE(std::string, EXP, gnsstk::ObsID::tcDesc[TRACKINGCODE])
#define OTDESCTEST(EXP, OBSTYPE) TUASSERTE(std::string, EXP, gnsstk::ObsID::otDesc[OBSTYPE])

class ObsID_T
{
public:

   unsigned initializationTest()
   {
      TUDEF("ObsID", "ObsID()");
      std::string failMesg;

      gnsstk::ObsID empty;
      TUASSERTE(gnsstk::ObservationType,
                gnsstk::ObservationType::Unknown, empty.type);
      TUASSERTE(gnsstk::CarrierBand,
                gnsstk::CarrierBand::Unknown, empty.band);
      TUASSERTE(gnsstk::TrackingCode,
                gnsstk::TrackingCode::Unknown, empty.code);
      TUASSERTE(gnsstk::XmitAnt,
                gnsstk::XmitAnt::Any, empty.xmitAnt);
      TUASSERTE(int, 0, empty.freqOffs);
      TUASSERTE(bool, true, empty.freqOffsWild);
      TUASSERTE(uint32_t, 0, empty.mcode);
      TUASSERTE(uint32_t, 0, empty.mcodeMask);

      TUCSM("ObsID(ObservationType,CarrierBand,TrackingCode)");
      gnsstk::ObsID compare(gnsstk::ObservationType::Range,
                           gnsstk::CarrierBand::L1,
                           gnsstk::TrackingCode::CA);
      TUASSERTE(gnsstk::ObservationType,
                gnsstk::ObservationType::Range, compare.type);
      TUASSERTE(gnsstk::CarrierBand,
                gnsstk::CarrierBand::L1, compare.band);
      TUASSERTE(gnsstk::TrackingCode,
                gnsstk::TrackingCode::CA, compare.code);
      TUASSERTE(gnsstk::XmitAnt,
                gnsstk::XmitAnt::Any, compare.xmitAnt);
      TUASSERTE(int, 0, compare.freqOffs);
      TUASSERTE(bool, true, compare.freqOffsWild);
      TUASSERTE(uint32_t, 0, compare.mcode);
      TUASSERTE(uint32_t, 0, compare.mcodeMask);

      TUCSM("ObsID(ObservationType,CarrierBand,TrackingCode,XmitAnt)");
      gnsstk::ObsID uut3(gnsstk::ObservationType::Range,
                        gnsstk::CarrierBand::L1,
                        gnsstk::TrackingCode::CA,
                        gnsstk::XmitAnt::Standard);
      TUASSERTE(gnsstk::ObservationType,
                gnsstk::ObservationType::Range, uut3.type);
      TUASSERTE(gnsstk::CarrierBand,
                gnsstk::CarrierBand::L1, uut3.band);
      TUASSERTE(gnsstk::TrackingCode,
                gnsstk::TrackingCode::CA, uut3.code);
      TUASSERTE(gnsstk::XmitAnt,
                gnsstk::XmitAnt::Standard, uut3.xmitAnt);
      TUASSERTE(int, 0, uut3.freqOffs);
      TUASSERTE(bool, true, uut3.freqOffsWild);
      TUASSERTE(uint32_t, 0, uut3.mcode);
      TUASSERTE(uint32_t, 0, uut3.mcodeMask);

      TUCSM("ObsID(ObservationType,CarrierBand,TrackingCode,int)");
      gnsstk::ObsID uut4(gnsstk::ObservationType::Phase,
                        gnsstk::CarrierBand::G1,
                        gnsstk::TrackingCode::Standard,
                        -1);
      TUASSERTE(gnsstk::ObservationType,
                gnsstk::ObservationType::Phase, uut4.type);
      TUASSERTE(gnsstk::CarrierBand,
                gnsstk::CarrierBand::G1, uut4.band);
      TUASSERTE(gnsstk::TrackingCode,
                gnsstk::TrackingCode::Standard, uut4.code);
      TUASSERTE(gnsstk::XmitAnt,
                gnsstk::XmitAnt::Any, uut4.xmitAnt);
      TUASSERTE(int, -1, uut4.freqOffs);
      TUASSERTE(bool, false, uut4.freqOffsWild);
      TUASSERTE(uint32_t, 0, uut4.mcode);
      TUASSERTE(uint32_t, 0, uut4.mcodeMask);

      TUCSM("ObsID(ObservationType,CarrierBand,TrackingCode,int,XmitAnt)");
      gnsstk::ObsID uut5(gnsstk::ObservationType::Phase,
                        gnsstk::CarrierBand::G1,
                        gnsstk::TrackingCode::Standard,
                        -1,
                        gnsstk::XmitAnt::Standard);
      TUASSERTE(gnsstk::ObservationType,
                gnsstk::ObservationType::Phase, uut5.type);
      TUASSERTE(gnsstk::CarrierBand,
                gnsstk::CarrierBand::G1, uut5.band);
      TUASSERTE(gnsstk::TrackingCode,
                gnsstk::TrackingCode::Standard, uut5.code);
      TUASSERTE(gnsstk::XmitAnt,
                gnsstk::XmitAnt::Standard, uut5.xmitAnt);
      TUASSERTE(int, -1, uut5.freqOffs);
      TUASSERTE(bool, false, uut5.freqOffsWild);
      TUASSERTE(uint32_t, 0, uut5.mcode);
      TUASSERTE(uint32_t, 0, uut5.mcodeMask);

      TURETURN();
   }


   unsigned dumpTest()
   {
      TUDEF("ObsID", "dump");
      std::string failMesg;
      gnsstk::ObsID compare(gnsstk::ObservationType::Doppler,
                           gnsstk::CarrierBand::L2,
                           gnsstk::TrackingCode::Y);

      std::string outputString, referenceString;
      std::stringstream outputStream, os2;

      compare.dump(outputStream);
      outputString = outputStream.str();

      referenceString = "L2 GPSY doppler";

      TUASSERTE(std::string, referenceString, outputString);

      gnsstk::ObsID::verbose = true;
      compare.dump(os2);
      TUASSERTE(std::string, "L2 GPSY doppler 0/true 0/0 Any", os2.str());
         // make sure to turn it back off for other tests.
      gnsstk::ObsID::verbose = false;
      TURETURN();
   }


   unsigned asStringTest()
   {
      TUDEF("ObsID", "asString");
      std::string failMesg;
      gnsstk::ObsID compare(gnsstk::ObservationType::Phase,
                           gnsstk::CarrierBand::E5b,
                           gnsstk::TrackingCode::E5abI);

      std::string outputString, referenceString;

      outputString = gnsstk::StringUtils::asString(compare);
      referenceString = "E5b GALI5 phase";

      TUASSERTE(std::string, referenceString, outputString);

      TURETURN();
   }


   unsigned asStringEnumTest()
   {
      TUDEF("ObsID", "asString");
         // These tests verify that every enum has a string
         // representation and every string has a corresponding enum.
         // It also implicitly verifies that the string
         // representations aren't duplicated, since if two enums
         // translated to string "XXX", the attempt to reverse the
         // translation would fail.
      for (gnsstk::ObservationType ot : gnsstk::ObservationTypeIterator())
      {
         std::string s;
         TUCATCH(s = gnsstk::StringUtils::asString(ot));
         TUASSERT(!s.empty());
         TUASSERT(s != "???");
         gnsstk::ObservationType ot2;
         TUCATCH(ot2 = gnsstk::StringUtils::asObservationType(s));
         TUASSERTE(gnsstk::ObservationType, ot, ot2);
      }
      for (gnsstk::CarrierBand cb : gnsstk::CarrierBandIterator())
      {
         std::string s;
         TUCATCH(s = gnsstk::StringUtils::asString(cb));
         TUASSERT(!s.empty());
         TUASSERT(s != "???");
         gnsstk::CarrierBand cb2;
         TUCATCH(cb2 = gnsstk::StringUtils::asCarrierBand(s));
         TUASSERTE(gnsstk::CarrierBand, cb, cb2);
      }
      for (gnsstk::TrackingCode tc : gnsstk::TrackingCodeIterator())
      {
         std::string s;
         TUCATCH(s = gnsstk::StringUtils::asString(tc));
         TUASSERT(!s.empty());
         TUASSERT(s != "???");
         gnsstk::TrackingCode tc2;
         TUCATCH(tc2 = gnsstk::StringUtils::asTrackingCode(s));
         TUASSERTE(gnsstk::TrackingCode, tc, tc2);
      }
      TURETURN();
   }


   unsigned operatorTest()
   {
      TUDEF("ObsID", "operator==");
      std::string failMesg;

      gnsstk::ObsID compare1(gnsstk::ObservationType::Range,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::CA);
      gnsstk::ObsID compare2(gnsstk::ObservationType::Range,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::CA);
      gnsstk::ObsID compare3(gnsstk::ObservationType::Doppler,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::CA);
      gnsstk::ObsID compare4(gnsstk::ObservationType::Doppler,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::CA, 0);
      gnsstk::ObsID compare5(gnsstk::ObservationType::Doppler,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::CA, 1);
      gnsstk::ObsID compare6(gnsstk::ObservationType::Doppler,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::CA,
                            gnsstk::XmitAnt::Standard);
      gnsstk::ObsID compare7(gnsstk::ObservationType::Doppler,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::CA,
                            gnsstk::XmitAnt::Regional);
      gnsstk::ObsID compare8(gnsstk::ObservationType::Doppler,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::CA);
      gnsstk::ObsID compare9(gnsstk::ObservationType::Doppler,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::CA);
      gnsstk::ObsID compare10(gnsstk::ObservationType::Doppler,
                             gnsstk::CarrierBand::L1,
                             gnsstk::TrackingCode::CA);

      gnsstk::ObsID::verbose = true;
      compare8.setMcodeBits(0x81234765, 0xffffffff);
      compare9.setMcodeBits(0x8123476a, 0xffffffff);
      compare10.setMcodeBits(0x81234765, 0xfffffff0);

      TUASSERTE(gnsstk::ObsID, compare1, compare2);
      TUASSERT(!(compare1 == compare3));
      TUASSERTE(gnsstk::ObsID, compare3, compare4);
      TUASSERT(!(compare4 == compare5));
      TUASSERT(compare3 == compare6);
      TUASSERT(compare3 == compare7);
      TUASSERT(!(compare6 == compare7));
      TUASSERTE(gnsstk::ObsID, compare3, compare8);
      TUASSERTE(gnsstk::ObsID, compare3, compare9);
      TUASSERT(!(compare8 == compare9));
      TUASSERTE(gnsstk::ObsID, compare3, compare10);
      TUASSERTE(gnsstk::ObsID, compare9, compare10);
      TUCSM("operator!=");
      TUASSERT(!(compare1 != compare2));
      TUASSERT(compare1 != compare3);
      TUASSERT(!(compare3 != compare4));
      TUASSERT(compare4 != compare5);
      TUASSERT(!(compare3 != compare6));
      TUASSERT(!(compare3 != compare7));
      TUASSERT(compare6 != compare7);
      TUASSERT(!(compare3 != compare8));
      TUASSERT(!(compare3 != compare9));
      TUASSERT(compare8 != compare9);
      TUASSERT(!(compare3 != compare10));
      TUASSERT(!(compare9 != compare10));
      gnsstk::ObsID::verbose = false;
      TURETURN();
   }


   unsigned cbDescTest()
   {
      TUDEF("ObsID", "cbDesc");
      CBDESCTEST("B1", gnsstk::CarrierBand::B1);
      CBDESCTEST("B2", gnsstk::CarrierBand::B2);
      CBDESCTEST("B3", gnsstk::CarrierBand::B3);
      CBDESCTEST("comboL1L2", gnsstk::CarrierBand::L1L2);
      CBDESCTEST("E5a+b", gnsstk::CarrierBand::E5ab);
      CBDESCTEST("E5b", gnsstk::CarrierBand::E5b);
      CBDESCTEST("E6", gnsstk::CarrierBand::E6);
      CBDESCTEST("G1", gnsstk::CarrierBand::G1);
      CBDESCTEST("G2", gnsstk::CarrierBand::G2);
      CBDESCTEST("G3", gnsstk::CarrierBand::G3);
      CBDESCTEST("L1", gnsstk::CarrierBand::L1);
      CBDESCTEST("L2", gnsstk::CarrierBand::L2);
      CBDESCTEST("L5", gnsstk::CarrierBand::L5);
      TURETURN();
   }


   unsigned tcDescTest()
   {
      TUDEF("ObsID", "tcDesc");
      TCDESCTEST("BDSIB1", gnsstk::TrackingCode::B1I);
      TCDESCTEST("BDSIB2", gnsstk::TrackingCode::B2I);
      TCDESCTEST("BDSIB3", gnsstk::TrackingCode::B3I);
      TCDESCTEST("BDSI+QB2", gnsstk::TrackingCode::B2IQ);
      TCDESCTEST("BDSI+QB3", gnsstk::TrackingCode::B3IQ);
      TCDESCTEST("BDSQB1", gnsstk::TrackingCode::B1Q);
      TCDESCTEST("BDSQB2", gnsstk::TrackingCode::B2Q);
      TCDESCTEST("BDSQB3", gnsstk::TrackingCode::B3Q);
      TCDESCTEST("GALA+B+C", gnsstk::TrackingCode::E1ABC);
      TCDESCTEST("GALA", gnsstk::TrackingCode::E1A);
      TCDESCTEST("GALB+C", gnsstk::TrackingCode::E1BC);
      TCDESCTEST("GALB", gnsstk::TrackingCode::E1B);
      TCDESCTEST("GALC", gnsstk::TrackingCode::E1C);
      TCDESCTEST("GALI5a", gnsstk::TrackingCode::E5aI);
      TCDESCTEST("GALI5b", gnsstk::TrackingCode::E5bI);
      TCDESCTEST("GALI5", gnsstk::TrackingCode::E5abI);
      TCDESCTEST("GALI+Q5a", gnsstk::TrackingCode::E5aIQ);
      TCDESCTEST("GALI+Q5b", gnsstk::TrackingCode::E5bIQ);
      TCDESCTEST("GALI+Q5", gnsstk::TrackingCode::E5abIQ);
      TCDESCTEST("GALQ5a", gnsstk::TrackingCode::E5aQ);
      TCDESCTEST("GALQ5b", gnsstk::TrackingCode::E5bQ);
      TCDESCTEST("GALQ5", gnsstk::TrackingCode::E5abQ);
      TCDESCTEST("GLOC/A", gnsstk::TrackingCode::Standard);
      TCDESCTEST("GLOI+QR5", gnsstk::TrackingCode::L3OCDP);
      TCDESCTEST("GLOIR5", gnsstk::TrackingCode::L3OCD);
      TCDESCTEST("GLOP", gnsstk::TrackingCode::Precise);
      TCDESCTEST("GLOQR5", gnsstk::TrackingCode::L3OCP);
      TCDESCTEST("GPSC1D", gnsstk::TrackingCode::L1CD);
      TCDESCTEST("GPSC1(D+P)", gnsstk::TrackingCode::L1CDP);
      TCDESCTEST("GPSC1P", gnsstk::TrackingCode::L1CP);
      TCDESCTEST("GPSC2L", gnsstk::TrackingCode::L2CL);
      TCDESCTEST("GPSC2L+M", gnsstk::TrackingCode::L2CML);
      TCDESCTEST("GPSC2M", gnsstk::TrackingCode::L2CM);
      TCDESCTEST("GPSC/A", gnsstk::TrackingCode::CA);
      TCDESCTEST("GPScodeless", gnsstk::TrackingCode::Semicodeless);
      TCDESCTEST("GPScodelessZ", gnsstk::TrackingCode::Ztracking);
      TCDESCTEST("GPSI5", gnsstk::TrackingCode::L5I);
      TCDESCTEST("GPSI+Q5", gnsstk::TrackingCode::L5IQ);
      TCDESCTEST("GPSM", gnsstk::TrackingCode::MDP);
      TCDESCTEST("GPSP", gnsstk::TrackingCode::P);
      TCDESCTEST("GPSQ5", gnsstk::TrackingCode::L5Q);
      TCDESCTEST("GPSsquare", gnsstk::TrackingCode::YCodeless);
      TCDESCTEST("GPSY", gnsstk::TrackingCode::Y);
      TCDESCTEST("IRNSSL5A", gnsstk::TrackingCode::SPSL5);
      TCDESCTEST("IRNSSL5B+C", gnsstk::TrackingCode::RSL5DP);
      TCDESCTEST("IRNSSL5B", gnsstk::TrackingCode::RSL5D);
      TCDESCTEST("IRNSSL5C", gnsstk::TrackingCode::RSL5P);
      TCDESCTEST("IRNSSL9A", gnsstk::TrackingCode::SPSS);
      TCDESCTEST("IRNSSL9B+C", gnsstk::TrackingCode::RSSDP);
      TCDESCTEST("IRNSSL9B", gnsstk::TrackingCode::RSSD);
      TCDESCTEST("IRNSSL9C", gnsstk::TrackingCode::RSSP);
//      TCDESCTEST("QZSSC/A", gnsstk::TrackingCode::CA);
//      TCDESCTEST("QZSSL1C(D)", gnsstk::TrackingCode::L1CD);
//      TCDESCTEST("QZSSL1C(D+P)", gnsstk::TrackingCode::L1CDP);
//      TCDESCTEST("QZSSL1C(P)", gnsstk::TrackingCode::L1CP);
      TCDESCTEST("QZSSL1-SAIF", gnsstk::TrackingCode::L1S);
//      TCDESCTEST("QZSSL2C(L)", gnsstk::TrackingCode::L2CL);
//      TCDESCTEST("QZSSL2C(M)", gnsstk::TrackingCode::L2CM);
//      TCDESCTEST("QZSSL2C(M+L)", gnsstk::TrackingCode::L2CML);
//      TCDESCTEST("QZSSL5I", gnsstk::TrackingCode::L5I);
//      TCDESCTEST("QZSSL5I+Q", gnsstk::TrackingCode::L5IQ);
//      TCDESCTEST("QZSSL5Q", gnsstk::TrackingCode::L5Q);
      TCDESCTEST("QZSSL6I", gnsstk::TrackingCode::LEXS);
      TCDESCTEST("QZSSL6I+Q", gnsstk::TrackingCode::LEXSL);
      TCDESCTEST("QZSSL6Q", gnsstk::TrackingCode::LEXL);
//      TCDESCTEST("SBASC/A", gnsstk::TrackingCode::CA);
//      TCDESCTEST("SBASI5", gnsstk::TrackingCode::L5I);
//      TCDESCTEST("SBASI+Q5", gnsstk::TrackingCode::L5IQ);
//      TCDESCTEST("SBASQ5", gnsstk::TrackingCode::L5Q);
      TURETURN();
   }


   unsigned otDescTest()
   {
      TUDEF("ObsID", "otDesc");
      OTDESCTEST("UnknownType", gnsstk::ObservationType::Unknown);
      OTDESCTEST("AnyType", gnsstk::ObservationType::Any);
      OTDESCTEST("pseudorange", gnsstk::ObservationType::Range);
      OTDESCTEST("phase", gnsstk::ObservationType::Phase);
      OTDESCTEST("doppler", gnsstk::ObservationType::Doppler);
      OTDESCTEST("snr", gnsstk::ObservationType::SNR);
      OTDESCTEST("channel", gnsstk::ObservationType::Channel);
      OTDESCTEST("demodStatus", gnsstk::ObservationType::DemodStat);
      OTDESCTEST("iono", gnsstk::ObservationType::Iono);
      OTDESCTEST("ssi", gnsstk::ObservationType::SSI);
      OTDESCTEST("lli", gnsstk::ObservationType::LLI);
      OTDESCTEST("tlen", gnsstk::ObservationType::TrackLen);
      OTDESCTEST("navmsg", gnsstk::ObservationType::NavMsg);
      OTDESCTEST("rngSigma", gnsstk::ObservationType::RngStdDev);
      OTDESCTEST("phsSigma", gnsstk::ObservationType::PhsStdDev);
      OTDESCTEST("freqIndx", gnsstk::ObservationType::FreqIndx);
      OTDESCTEST("undefined", gnsstk::ObservationType::Undefined);
      TURETURN();
   }
};


int main()
{
   unsigned errorTotal = 0;
   ObsID_T testClass;

   errorTotal += testClass.initializationTest();
   errorTotal += testClass.dumpTest();
   errorTotal += testClass.asStringTest();
   errorTotal += testClass.asStringEnumTest();
   errorTotal += testClass.operatorTest();
   errorTotal += testClass.cbDescTest();
   errorTotal += testClass.tcDescTest();
   errorTotal += testClass.otDescTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
