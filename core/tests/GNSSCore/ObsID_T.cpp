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

#include "ObsID.hpp"

#include "TestUtil.hpp"
#include "Rinex3ObsHeader.hpp"
#include <iostream>
#include <sstream>

namespace gpstk
{
      // define some stream operators so that test failures involving
      // enums are a bit more readable.

   std::ostream& operator<<(std::ostream& s, gpstk::ObservationType e)
   {
      s << (long)e << " (" << StringUtils::asString(e) << ")";
      return s;
   }
   std::ostream& operator<<(std::ostream& s, CarrierBand e)
   {
      s << (long)e << " (" << StringUtils::asString(e) << ")";
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gpstk::TrackingCode e)
   {
      s << (long)e << " (" << StringUtils::asString(e) << ")";
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gpstk::XmitAnt e)
   {
      s << (long)e << " (" << StringUtils::asString(e) << ")";
      return s;
   }
}

#define CBDESCTEST(EXP, CARRIERBAND) TUASSERTE(std::string, EXP, gpstk::ObsID::cbDesc[CARRIERBAND])
#define TCDESCTEST(EXP, TRACKINGCODE) TUASSERTE(std::string, EXP, gpstk::ObsID::tcDesc[TRACKINGCODE])
#define OTDESCTEST(EXP, OBSTYPE) TUASSERTE(std::string, EXP, gpstk::ObsID::otDesc[OBSTYPE])

class ObsID_T
{
public:

   unsigned initializationTest()
   {
      TUDEF("ObsID", "ObsID()");
      std::string failMesg;

      gpstk::ObsID empty;
      TUASSERTE(gpstk::ObservationType,
                gpstk::ObservationType::Unknown, empty.type);
      TUASSERTE(gpstk::CarrierBand,
                gpstk::CarrierBand::Unknown, empty.band);
      TUASSERTE(gpstk::TrackingCode,
                gpstk::TrackingCode::Unknown, empty.code);
      TUASSERTE(gpstk::XmitAnt,
                gpstk::XmitAnt::Any, empty.xmitAnt);
      TUASSERTE(int, 0, empty.freqOffs);
      TUASSERTE(bool, true, empty.freqOffsWild);
      TUASSERTE(uint32_t, 0, empty.mcode);
      TUASSERTE(uint32_t, 0, empty.mcodeMask);

      TUCSM("ObsID(ObservationType,CarrierBand,TrackingCode)");
      gpstk::ObsID compare(gpstk::ObservationType::Range,
                           gpstk::CarrierBand::L1,
                           gpstk::TrackingCode::CA);
      TUASSERTE(gpstk::ObservationType,
                gpstk::ObservationType::Range, compare.type);
      TUASSERTE(gpstk::CarrierBand,
                gpstk::CarrierBand::L1, compare.band);
      TUASSERTE(gpstk::TrackingCode,
                gpstk::TrackingCode::CA, compare.code);
      TUASSERTE(gpstk::XmitAnt,
                gpstk::XmitAnt::Any, compare.xmitAnt);
      TUASSERTE(int, 0, compare.freqOffs);
      TUASSERTE(bool, true, compare.freqOffsWild);
      TUASSERTE(uint32_t, 0, compare.mcode);
      TUASSERTE(uint32_t, 0, compare.mcodeMask);

      TUCSM("ObsID(ObservationType,CarrierBand,TrackingCode,XmitAnt)");
      gpstk::ObsID uut3(gpstk::ObservationType::Range,
                        gpstk::CarrierBand::L1,
                        gpstk::TrackingCode::CA,
                        gpstk::XmitAnt::Standard);
      TUASSERTE(gpstk::ObservationType,
                gpstk::ObservationType::Range, uut3.type);
      TUASSERTE(gpstk::CarrierBand,
                gpstk::CarrierBand::L1, uut3.band);
      TUASSERTE(gpstk::TrackingCode,
                gpstk::TrackingCode::CA, uut3.code);
      TUASSERTE(gpstk::XmitAnt,
                gpstk::XmitAnt::Standard, uut3.xmitAnt);
      TUASSERTE(int, 0, uut3.freqOffs);
      TUASSERTE(bool, true, uut3.freqOffsWild);
      TUASSERTE(uint32_t, 0, uut3.mcode);
      TUASSERTE(uint32_t, 0, uut3.mcodeMask);

      TUCSM("ObsID(ObservationType,CarrierBand,TrackingCode,int)");
      gpstk::ObsID uut4(gpstk::ObservationType::Phase,
                        gpstk::CarrierBand::G1,
                        gpstk::TrackingCode::Standard,
                        -1);
      TUASSERTE(gpstk::ObservationType,
                gpstk::ObservationType::Phase, uut4.type);
      TUASSERTE(gpstk::CarrierBand,
                gpstk::CarrierBand::G1, uut4.band);
      TUASSERTE(gpstk::TrackingCode,
                gpstk::TrackingCode::Standard, uut4.code);
      TUASSERTE(gpstk::XmitAnt,
                gpstk::XmitAnt::Any, uut4.xmitAnt);
      TUASSERTE(int, -1, uut4.freqOffs);
      TUASSERTE(bool, false, uut4.freqOffsWild);
      TUASSERTE(uint32_t, 0, uut4.mcode);
      TUASSERTE(uint32_t, 0, uut4.mcodeMask);

      TUCSM("ObsID(ObservationType,CarrierBand,TrackingCode,int,XmitAnt)");
      gpstk::ObsID uut5(gpstk::ObservationType::Phase,
                        gpstk::CarrierBand::G1,
                        gpstk::TrackingCode::Standard,
                        -1,
                        gpstk::XmitAnt::Standard);
      TUASSERTE(gpstk::ObservationType,
                gpstk::ObservationType::Phase, uut5.type);
      TUASSERTE(gpstk::CarrierBand,
                gpstk::CarrierBand::G1, uut5.band);
      TUASSERTE(gpstk::TrackingCode,
                gpstk::TrackingCode::Standard, uut5.code);
      TUASSERTE(gpstk::XmitAnt,
                gpstk::XmitAnt::Standard, uut5.xmitAnt);
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
      gpstk::ObsID compare(gpstk::ObservationType::Doppler,
                           gpstk::CarrierBand::L2,
                           gpstk::TrackingCode::Y);

      std::string outputString, referenceString;
      std::stringstream outputStream, os2;

      compare.dump(outputStream);
      outputString = outputStream.str();

      referenceString = "L2 GPSY doppler";

      TUASSERTE(std::string, referenceString, outputString);

      gpstk::ObsID::verbose = true;
      compare.dump(os2);
      TUASSERTE(std::string, "L2 GPSY doppler 0/true 0/0 Any", os2.str());
         // make sure to turn it back off for other tests.
      gpstk::ObsID::verbose = false;
      TURETURN();
   }


   unsigned asStringTest()
   {
      TUDEF("ObsID", "asString");
      std::string failMesg;
      gpstk::ObsID compare(gpstk::ObservationType::Phase,
                           gpstk::CarrierBand::E5b,
                           gpstk::TrackingCode::E5abI);

      std::string outputString, referenceString;

      outputString = gpstk::StringUtils::asString(compare);
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
      for (gpstk::ObservationType ot : gpstk::ObservationTypeIterator())
      {
         std::string s;
         TUCATCH(s = gpstk::StringUtils::asString(ot));
         TUASSERT(!s.empty());
         TUASSERT(s != "???");
         gpstk::ObservationType ot2;
         TUCATCH(ot2 = gpstk::StringUtils::asObservationType(s));
         TUASSERTE(gpstk::ObservationType, ot, ot2);
      }
      for (gpstk::CarrierBand cb : gpstk::CarrierBandIterator())
      {
         std::string s;
         TUCATCH(s = gpstk::StringUtils::asString(cb));
         TUASSERT(!s.empty());
         TUASSERT(s != "???");
         gpstk::CarrierBand cb2;
         TUCATCH(cb2 = gpstk::StringUtils::asCarrierBand(s));
         TUASSERTE(gpstk::CarrierBand, cb, cb2);
      }
      for (gpstk::TrackingCode tc : gpstk::TrackingCodeIterator())
      {
         std::string s;
         TUCATCH(s = gpstk::StringUtils::asString(tc));
         TUASSERT(!s.empty());
         TUASSERT(s != "???");
         gpstk::TrackingCode tc2;
         TUCATCH(tc2 = gpstk::StringUtils::asTrackingCode(s));
         TUASSERTE(gpstk::TrackingCode, tc, tc2);
      }
      TURETURN();
   }


   unsigned operatorTest()
   {
      TUDEF("ObsID", "operator==");
      std::string failMesg;

      gpstk::ObsID compare1(gpstk::ObservationType::Range,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::CA);
      gpstk::ObsID compare2(gpstk::ObservationType::Range,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::CA);
      gpstk::ObsID compare3(gpstk::ObservationType::Doppler,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::CA);
      gpstk::ObsID compare4(gpstk::ObservationType::Doppler,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::CA, 0);
      gpstk::ObsID compare5(gpstk::ObservationType::Doppler,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::CA, 1);
      gpstk::ObsID compare6(gpstk::ObservationType::Doppler,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::CA,
                            gpstk::XmitAnt::Standard);
      gpstk::ObsID compare7(gpstk::ObservationType::Doppler,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::CA,
                            gpstk::XmitAnt::Regional);
      gpstk::ObsID compare8(gpstk::ObservationType::Doppler,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::CA);
      gpstk::ObsID compare9(gpstk::ObservationType::Doppler,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::CA);
      gpstk::ObsID compare10(gpstk::ObservationType::Doppler,
                             gpstk::CarrierBand::L1,
                             gpstk::TrackingCode::CA);

      gpstk::ObsID::verbose = true;
      compare8.setMcodeBits(0x81234765, 0xffffffff);
      compare9.setMcodeBits(0x8123476a, 0xffffffff);
      compare10.setMcodeBits(0x81234765, 0xfffffff0);

      TUASSERTE(gpstk::ObsID, compare1, compare2);
      TUASSERT(!(compare1 == compare3));
      TUASSERTE(gpstk::ObsID, compare3, compare4);
      TUASSERT(!(compare4 == compare5));
      TUASSERT(compare3 == compare6);
      TUASSERT(compare3 == compare7);
      TUASSERT(!(compare6 == compare7));
      TUASSERTE(gpstk::ObsID, compare3, compare8);
      TUASSERTE(gpstk::ObsID, compare3, compare9);
      TUASSERT(!(compare8 == compare9));
      TUASSERTE(gpstk::ObsID, compare3, compare10);
      TUASSERTE(gpstk::ObsID, compare9, compare10);
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
      gpstk::ObsID::verbose = false;
      TURETURN();
   }


   unsigned cbDescTest()
   {
      TUDEF("ObsID", "cbDesc");
      CBDESCTEST("B1", gpstk::CarrierBand::B1);
      CBDESCTEST("B2", gpstk::CarrierBand::B2);
      CBDESCTEST("B3", gpstk::CarrierBand::B3);
      CBDESCTEST("comboL1L2", gpstk::CarrierBand::L1L2);
      CBDESCTEST("E5a+b", gpstk::CarrierBand::E5ab);
      CBDESCTEST("E5b", gpstk::CarrierBand::E5b);
      CBDESCTEST("E6", gpstk::CarrierBand::E6);
      CBDESCTEST("G1", gpstk::CarrierBand::G1);
      CBDESCTEST("G2", gpstk::CarrierBand::G2);
      CBDESCTEST("G3", gpstk::CarrierBand::G3);
      CBDESCTEST("L1", gpstk::CarrierBand::L1);
      CBDESCTEST("L2", gpstk::CarrierBand::L2);
      CBDESCTEST("L5", gpstk::CarrierBand::L5);
      TURETURN();
   }


   unsigned tcDescTest()
   {
      TUDEF("ObsID", "tcDesc");
      TCDESCTEST("BDSIB1", gpstk::TrackingCode::B1I);
      TCDESCTEST("BDSIB2", gpstk::TrackingCode::B2I);
      TCDESCTEST("BDSIB3", gpstk::TrackingCode::B3I);
      TCDESCTEST("BDSI+QB2", gpstk::TrackingCode::B2IQ);
      TCDESCTEST("BDSI+QB3", gpstk::TrackingCode::B3IQ);
      TCDESCTEST("BDSQB1", gpstk::TrackingCode::B1Q);
      TCDESCTEST("BDSQB2", gpstk::TrackingCode::B2Q);
      TCDESCTEST("BDSQB3", gpstk::TrackingCode::B3Q);
      TCDESCTEST("GALA+B+C", gpstk::TrackingCode::E1ABC);
      TCDESCTEST("GALA", gpstk::TrackingCode::E1A);
      TCDESCTEST("GALB+C", gpstk::TrackingCode::E1BC);
      TCDESCTEST("GALB", gpstk::TrackingCode::E1B);
      TCDESCTEST("GALC", gpstk::TrackingCode::E1C);
      TCDESCTEST("GALI5a", gpstk::TrackingCode::E5aI);
      TCDESCTEST("GALI5b", gpstk::TrackingCode::E5bI);
      TCDESCTEST("GALI5", gpstk::TrackingCode::E5abI);
      TCDESCTEST("GALI+Q5a", gpstk::TrackingCode::E5aIQ);
      TCDESCTEST("GALI+Q5b", gpstk::TrackingCode::E5bIQ);
      TCDESCTEST("GALI+Q5", gpstk::TrackingCode::E5abIQ);
      TCDESCTEST("GALQ5a", gpstk::TrackingCode::E5aQ);
      TCDESCTEST("GALQ5b", gpstk::TrackingCode::E5bQ);
      TCDESCTEST("GALQ5", gpstk::TrackingCode::E5abQ);
      TCDESCTEST("GLOC/A", gpstk::TrackingCode::Standard);
      TCDESCTEST("GLOI+QR5", gpstk::TrackingCode::L3OCDP);
      TCDESCTEST("GLOIR5", gpstk::TrackingCode::L3OCD);
      TCDESCTEST("GLOP", gpstk::TrackingCode::Precise);
      TCDESCTEST("GLOQR5", gpstk::TrackingCode::L3OCP);
      TCDESCTEST("GPSC1D", gpstk::TrackingCode::L1CD);
      TCDESCTEST("GPSC1(D+P)", gpstk::TrackingCode::L1CDP);
      TCDESCTEST("GPSC1P", gpstk::TrackingCode::L1CP);
      TCDESCTEST("GPSC2L", gpstk::TrackingCode::L2CL);
      TCDESCTEST("GPSC2L+M", gpstk::TrackingCode::L2CML);
      TCDESCTEST("GPSC2M", gpstk::TrackingCode::L2CM);
      TCDESCTEST("GPSC/A", gpstk::TrackingCode::CA);
      TCDESCTEST("GPScodeless", gpstk::TrackingCode::Semicodeless);
      TCDESCTEST("GPScodelessZ", gpstk::TrackingCode::Ztracking);
      TCDESCTEST("GPSI5", gpstk::TrackingCode::L5I);
      TCDESCTEST("GPSI+Q5", gpstk::TrackingCode::L5IQ);
      TCDESCTEST("GPSM", gpstk::TrackingCode::MDP);
      TCDESCTEST("GPSP", gpstk::TrackingCode::P);
      TCDESCTEST("GPSQ5", gpstk::TrackingCode::L5Q);
      TCDESCTEST("GPSsquare", gpstk::TrackingCode::YCodeless);
      TCDESCTEST("GPSY", gpstk::TrackingCode::Y);
      TCDESCTEST("IRNSSL5A", gpstk::TrackingCode::SPSL5);
      TCDESCTEST("IRNSSL5B+C", gpstk::TrackingCode::RSL5DP);
      TCDESCTEST("IRNSSL5B", gpstk::TrackingCode::RSL5D);
      TCDESCTEST("IRNSSL5C", gpstk::TrackingCode::RSL5P);
      TCDESCTEST("IRNSSL9A", gpstk::TrackingCode::SPSS);
      TCDESCTEST("IRNSSL9B+C", gpstk::TrackingCode::RSSDP);
      TCDESCTEST("IRNSSL9B", gpstk::TrackingCode::RSSD);
      TCDESCTEST("IRNSSL9C", gpstk::TrackingCode::RSSP);
//      TCDESCTEST("QZSSC/A", gpstk::TrackingCode::CA);
//      TCDESCTEST("QZSSL1C(D)", gpstk::TrackingCode::L1CD);
//      TCDESCTEST("QZSSL1C(D+P)", gpstk::TrackingCode::L1CDP);
//      TCDESCTEST("QZSSL1C(P)", gpstk::TrackingCode::L1CP);
      TCDESCTEST("QZSSL1-SAIF", gpstk::TrackingCode::L1S);
//      TCDESCTEST("QZSSL2C(L)", gpstk::TrackingCode::L2CL);
//      TCDESCTEST("QZSSL2C(M)", gpstk::TrackingCode::L2CM);
//      TCDESCTEST("QZSSL2C(M+L)", gpstk::TrackingCode::L2CML);
//      TCDESCTEST("QZSSL5I", gpstk::TrackingCode::L5I);
//      TCDESCTEST("QZSSL5I+Q", gpstk::TrackingCode::L5IQ);
//      TCDESCTEST("QZSSL5Q", gpstk::TrackingCode::L5Q);
      TCDESCTEST("QZSSL6I", gpstk::TrackingCode::LEXS);
      TCDESCTEST("QZSSL6I+Q", gpstk::TrackingCode::LEXSL);
      TCDESCTEST("QZSSL6Q", gpstk::TrackingCode::LEXL);
//      TCDESCTEST("SBASC/A", gpstk::TrackingCode::CA);
//      TCDESCTEST("SBASI5", gpstk::TrackingCode::L5I);
//      TCDESCTEST("SBASI+Q5", gpstk::TrackingCode::L5IQ);
//      TCDESCTEST("SBASQ5", gpstk::TrackingCode::L5Q);
      TURETURN();
   }


   unsigned otDescTest()
   {
      TUDEF("ObsID", "otDesc");
      OTDESCTEST("UnknownType", gpstk::ObservationType::Unknown);
      OTDESCTEST("AnyType", gpstk::ObservationType::Any);
      OTDESCTEST("pseudorange", gpstk::ObservationType::Range);
      OTDESCTEST("phase", gpstk::ObservationType::Phase);
      OTDESCTEST("doppler", gpstk::ObservationType::Doppler);
      OTDESCTEST("snr", gpstk::ObservationType::SNR);
      OTDESCTEST("channel", gpstk::ObservationType::Channel);
      OTDESCTEST("demodStatus", gpstk::ObservationType::DemodStat);
      OTDESCTEST("iono", gpstk::ObservationType::Iono);
      OTDESCTEST("ssi", gpstk::ObservationType::SSI);
      OTDESCTEST("lli", gpstk::ObservationType::LLI);
      OTDESCTEST("tlen", gpstk::ObservationType::TrackLen);
      OTDESCTEST("navmsg", gpstk::ObservationType::NavMsg);
      OTDESCTEST("rngSigma", gpstk::ObservationType::RngStdDev);
      OTDESCTEST("phsSigma", gpstk::ObservationType::PhsStdDev);
      OTDESCTEST("freqIndx", gpstk::ObservationType::FreqIndx);
      OTDESCTEST("undefined", gpstk::ObservationType::Undefined);
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
