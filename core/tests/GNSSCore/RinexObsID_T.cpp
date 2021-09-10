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

#include "TestUtil.hpp"
#include "RinexObsID.hpp"
#include "Rinex3ObsHeader.hpp"
#include <iostream>

using namespace std;

namespace gnsstk
{
   ostream& operator<<(ostream& s, CarrierBand cb)
   {
      s << gnsstk::StringUtils::asString(cb);
      return s;
   }
   ostream& operator<<(ostream& s, TrackingCode tc)
   {
      s << gnsstk::StringUtils::asString(tc);
      return s;
   }
   ostream& operator<<(ostream& s, ObservationType ot)
   {
      s << gnsstk::StringUtils::asString(ot);
      return s;
   }
}


// This makes sure that
// 1) The RINEX obs ID decodes as expected
// 2) The RINEX obs ID encodes as expected
// 3) The wildcard (Any) ObsID enums match
#define CONTEST(RINEXCODE, CARRIERBAND, TRACKINGCODE)                   \
   try                                                                  \
   {                                                                    \
      TUCSM("RinexObsID(\"" RINEXCODE "\")");                           \
      gnsstk::RinexObsID obs(RINEXCODE,                                  \
                            gnsstk::Rinex3ObsBase::currentVersion);      \
      TUASSERTE(gnsstk::ObservationType,                                 \
                gnsstk::ObservationType::Phase,                          \
                obs.type);                                              \
      TUASSERTE(gnsstk::CarrierBand,                                     \
                CARRIERBAND, obs.band);                                 \
      TUASSERTE(gnsstk::TrackingCode,                                    \
                TRACKINGCODE, obs.code);                                \
      gnsstk::RinexObsID obs2(gnsstk::ObservationType::Phase,             \
                             CARRIERBAND, TRACKINGCODE);                \
      TUASSERTE(std::string, std::string(RINEXCODE).substr(1),          \
                obs2.asString());                                       \
      gnsstk::RinexObsID wildcard("****",                                \
                                 gnsstk::Rinex3ObsBase::currentVersion); \
      TUASSERTE(gnsstk::RinexObsID, wildcard, obs);                      \
   }                                                                    \
   catch (gnsstk::Exception& exc)                                        \
   {                                                                    \
      cerr << exc << endl;                                              \
      TUFAIL("Unexpected exception");                                   \
   }                                                                    \
   catch (std::exception& exc)                                          \
   {                                                                    \
      TUFAIL("Unexpected exception " + std::string(exc.what()));        \
   }                                                                    \
   catch (...)                                                          \
   {                                                                    \
      TUFAIL("Unknown exception");                                      \
   }

class RinexObsID_T
{
public:
   RinexObsID_T() {} // Default Constructor, set the precision value
   ~RinexObsID_T() {} // Default Desructor
      /// Make sure RinexObsID can decode all valid observation codes.
   unsigned decodeTest();
      /// This was migrated from ObsID_T when the RINEX-isms were
      /// moved and probably duplicates a fair bit of decodeTest, but
      /// I can't be bothered to unify the testing right now.
   unsigned fromStringConstructorTest();
   unsigned newIDTest();
      /** Iterate through codes for testing
       * @param[in,out] testFramework The TestUtil object for the
       *   currently executing test.
       * @param[in] system The single character RINEX 3 system code
       *   to be tested ("G", "R", etc).
       * @param[in] bandCode The two character RINEX 3 code for band
       *   and tracking code, e.g. 1C.
       * @param[in] prValid If false, a pseudorange measurement is
       *   invalid for this code. */
   void testCodes(gnsstk::TestUtil& testFramework, const std::string& system,
                  const std::string& bandCode, bool prValid = true);
};


unsigned RinexObsID_T ::
decodeTest()
{
   TUDEF("RinexObsID", "RinexObsID(string)");
      // GPS L1
   std::string codes("CSLXPWYM");
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "G", std::string("1") + codes[i]);
   }
   testCodes(testFramework, "G", "1N", false);
      // GPS L2
   codes = "CDSLXPWYM";
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "G", std::string("2") + codes[i]);
   }
   testCodes(testFramework, "G", "2N", false);
      // GPS L5
   codes = "IQX";
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "G", std::string("5") + codes[i]);
   }
      // GLONASS G1
   codes = "CP";
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "R", std::string("1") + codes[i]);
   }
      // GLONASS G1a
   codes = "ABX";
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "R", std::string("4") + codes[i]);
   }
      // GLONASS G2
   codes = "CP";
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "R", std::string("2") + codes[i]);
   }
      // GLONASS G2a
   codes = "ABX";
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "R", std::string("6") + codes[i]);
   }
      // GLONASS G3
   codes = "IQX";
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "R", std::string("3") + codes[i]);
   }
      // Galileo E1
   codes = "ABCXZ";
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "E", std::string("1") + codes[i]);
   }
      // Galileo E5a
   codes = "IQX";
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "E", std::string("5") + codes[i]);
   }
      // Galileo E5b
   codes = "IQX";
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "E", std::string("7") + codes[i]);
   }
      // Galileo E5
   codes = "IQX";
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "E", std::string("8") + codes[i]);
   }
      // Galileo E6
   codes = "ABCXZ";
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "E", std::string("6") + codes[i]);
   }
      // SBAS L1
   testCodes(testFramework, "S", "1C");
      // SBAS L5
   codes = "IQX";
   for (unsigned i = 0; i < codes.length(); i++)
   {
      testCodes(testFramework, "S", std::string("5") + codes[i]);
   }

      // test that channel num pseudo obs is decoded properly
   TUCSM("RinexObsID(\"GX1 \")");
   gnsstk::RinexObsID roid("GX1 ", gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType, gnsstk::ObservationType::Channel, roid.type);
      // RINEX requires that the "band" be "1" at all times, but I'm
      // not sure it strictly makes sense to actually translate it to
      // L1.
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, roid.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::Undefined, roid.code);

      // test that iono delay pseudo obs is decoded properly
   TUCSM("RinexObsID(\"GI1 \")");
   gnsstk::RinexObsID roidI1("GI1 ", gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType, gnsstk::ObservationType::Iono, roidI1.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, roidI1.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::Undefined,roidI1.code);
   TUCSM("RinexObsID(\"GI2 \")");
   gnsstk::RinexObsID roidI2("GI2 ", gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType, gnsstk::ObservationType::Iono, roidI2.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L2, roidI2.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::Undefined,roidI2.code);
   TUCSM("RinexObsID(\"RI3 \")");
   gnsstk::RinexObsID roidI3("RI3 ", gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType, gnsstk::ObservationType::Iono, roidI3.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::G3, roidI3.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::Undefined,roidI3.code);
   TUCSM("RinexObsID(\"RI4 \")");
   gnsstk::RinexObsID roidI4("RI4 ", gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType, gnsstk::ObservationType::Iono, roidI4.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::G1a, roidI4.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::Undefined,roidI4.code);
   TUCSM("RinexObsID(\"GI5 \")");
   gnsstk::RinexObsID roidI5("GI5 ", gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType, gnsstk::ObservationType::Iono, roidI5.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L5, roidI5.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::Undefined,roidI5.code);
   TUCSM("RinexObsID(\"EI6 \")");
   gnsstk::RinexObsID roidI6("EI6 ", gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType, gnsstk::ObservationType::Iono, roidI6.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::G2a, roidI6.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::Undefined,roidI6.code);
   TUCSM("RinexObsID(\"CI7 \")");
   gnsstk::RinexObsID roidI7("CI7 ", gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType, gnsstk::ObservationType::Iono, roidI7.type);
      /** @bug The constructor rather unintelligently returns the same
       * band regardless of the GNSS being decoded.  Fixing this will
       * require rewriting the ObsID constructor which I'm putting off
       * for now. */
      //TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::B2, roidI7.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::Undefined,roidI7.code);
   TUCSM("RinexObsID(\"EI8 \")");
   gnsstk::RinexObsID roidI8("EI8 ", gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType, gnsstk::ObservationType::Iono, roidI8.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::E5ab, roidI8.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::Undefined,roidI8.code);
   TUCSM("RinexObsID(\"II9 \")");
   gnsstk::RinexObsID roidI9("II9 ", gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType, gnsstk::ObservationType::Iono, roidI9.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::I9, roidI9.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::Undefined,roidI9.code);

      // test RINEX 3.02 crapola
   TUCSM("RinexObsID() 3.02");
   gnsstk::RinexObsID roid302("CC1I", 3.02);
   TUASSERTE(gnsstk::ObservationType,gnsstk::ObservationType::Range,roid302.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::B1, roid302.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::B1I, roid302.code);
   TUASSERTE(std::string, "C1I", roid302.asString());
   TUASSERTFE(3.02, roid302.rinexVersion);
   roid302.rinexVersion = 3.04;
   TUASSERTE(std::string, "C2I", roid302.asString());

   gnsstk::RinexObsID roid302q("CC1Q", 3.02);
   TUASSERTE(gnsstk::ObservationType,gnsstk::ObservationType::Range,roid302q.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::B1, roid302q.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::B1Q, roid302q.code);
   TUASSERTE(std::string, "C1Q", roid302q.asString());
   TUASSERTFE(3.02, roid302q.rinexVersion);
   roid302q.rinexVersion = 3.04;
   TUASSERTE(std::string, "C2Q", roid302q.asString());

   gnsstk::RinexObsID roid302iq("CC1X", 3.02);
   TUASSERTE(gnsstk::ObservationType, gnsstk::ObservationType::Range,
             roid302iq.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::B1, roid302iq.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::B1IQ, roid302iq.code);
   TUASSERTE(std::string, "C1X", roid302iq.asString());
   TUASSERTFE(3.02, roid302iq.rinexVersion);
   roid302iq.rinexVersion = 3.04;
   TUASSERTE(std::string, "C2X", roid302iq.asString());

   TUCSM("RinexObsID() 3.03");
   gnsstk::RinexObsID roid303("CC2I", 3.03);
   TUASSERTE(gnsstk::ObservationType,gnsstk::ObservationType::Range,roid303.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::B1, roid303.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::B1I, roid303.code);
   TUASSERTFE(3.03, roid303.rinexVersion);

   TUCSM("RinexObsID() 3.04");
   gnsstk::RinexObsID roid304("CC2I", 3.04);
   TUASSERTE(gnsstk::ObservationType,gnsstk::ObservationType::Range,roid304.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::B1, roid304.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::B1I, roid304.code);
   TUASSERTFE(3.04, roid304.rinexVersion);

   gnsstk::RinexObsID roid304d("CC1D", 3.04);
   TUASSERTE(gnsstk::ObservationType,gnsstk::ObservationType::Range,roid304d.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, roid304d.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::B1CD, roid304d.code);
   TUASSERTFE(3.04, roid304d.rinexVersion);

   gnsstk::RinexObsID roid304p("CC1P", 3.04);
   TUASSERTE(gnsstk::ObservationType,gnsstk::ObservationType::Range,roid304p.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, roid304p.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::B1CP, roid304p.code);
   TUASSERTFE(3.04, roid304p.rinexVersion);

   gnsstk::RinexObsID roid304dp("CC1X", 3.04);
   TUASSERTE(gnsstk::ObservationType, gnsstk::ObservationType::Range,
             roid304dp.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, roid304dp.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::B1CDP, roid304dp.code);
   TUASSERTFE(3.04, roid304dp.rinexVersion);

   gnsstk::RinexObsID roid304a("CC1A", 3.04);
   TUASSERTE(gnsstk::ObservationType,gnsstk::ObservationType::Range,roid304a.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, roid304a.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::B1A, roid304a.code);
   TUASSERTFE(3.04, roid304a.rinexVersion);

   gnsstk::RinexObsID roid304n("CL1N", 3.04);
   TUASSERTE(gnsstk::ObservationType,gnsstk::ObservationType::Phase,roid304n.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, roid304n.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::BCodeless,
             roid304n.code);
   TUASSERTFE(3.04, roid304n.rinexVersion);

   TURETURN();
}


void RinexObsID_T ::
testCodes(gnsstk::TestUtil& testFramework,
          const std::string& system,
          const std::string& bandCode,
          bool prValid)
{
   std::string invalidCodes("ABEFGHIJKMNOPQRTUVWXYZabcdefghijklmnopqrstuvwxyz"),
      validCodes("LDS");
   if (prValid)
      validCodes += "C";
   else
      invalidCodes += "C";
   for (unsigned i = 0; i < invalidCodes.length(); i++)
   {
      std::string rin3code = system + invalidCodes[i] + bandCode;
         // RinexObsID doesn't throw an exception for invalid codes?
         // TUCSM("RinexObsID("+rin3code+")");
         // TUTHROW(gnsstk::RinexObsID(rin3code));
      TUCSM("::isValidRinexObsID("+rin3code+")");
      TUASSERT(!gnsstk::isValidRinexObsID(rin3code));
   }
   for (unsigned i = 0; i < validCodes.length(); i++)
   {
      std::string rin3code = system + validCodes[i] + bandCode;
      TUCSM("RinexObsID("+rin3code+")");
      TUCATCH(gnsstk::RinexObsID(rin3code));
         // this is declared a global at the moment :-/
      TUCSM("::isValidRinexObsID("+rin3code+")");
      TUASSERT(gnsstk::isValidRinexObsID(rin3code));
   }
}


   //reads in 3-4 length string
unsigned RinexObsID_T :: fromStringConstructorTest()
{
   TUDEF("RinexObsID", "RinexObsID(string)");
   std::string failMesg;

      //set with invalid length
   failMesg = "[testing] RinexObsID constructor from invalid string, [expected]"
      " exception gnsstk::Exception, [actual] threw no exception";
   try
   {
      gnsstk::RinexObsID invalidID("G 10 ",gnsstk::Rinex3ObsBase::currentVersion);
      TUFAIL(failMesg);
   }
   catch (gnsstk::Exception e)
   {
      TUPASS(failMesg);
   }

   try {
      gnsstk::RinexObsID invalidID("G1",gnsstk::Rinex3ObsBase::currentVersion);
      TUFAIL(failMesg);
   }
   catch (gnsstk::Exception e)
   {
      TUPASS(failMesg);
   }

      //testing base assign w/out using any of the reused codes
      // GPS L1 C/A PseudoRange
   gnsstk::RinexObsID obs1("GC1C",gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType,gnsstk::ObservationType::Range,obs1.type);
   TUASSERTE(gnsstk::CarrierBand,gnsstk::CarrierBand::L1,obs1.band);
   TUASSERTE(gnsstk::TrackingCode,gnsstk::TrackingCode::CA,obs1.code);

      //testing only case of reassinged codes for GPS
      // GPS L5 IQ Doppler
   gnsstk::RinexObsID obs2("GD5X",gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType,
             gnsstk::ObservationType::Doppler, obs2.type);
   TUASSERTE(gnsstk::CarrierBand,gnsstk::CarrierBand::L5,obs2.band);
   TUASSERTE(gnsstk::TrackingCode,gnsstk::TrackingCode::L5IQ,obs2.code);

      //testing completely random case
      // QZSS E6 L Carrier Phase
   gnsstk::RinexObsID obs3("JL6L",gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType,gnsstk::ObservationType::Phase,obs3.type);
   TUASSERTE(gnsstk::CarrierBand,gnsstk::CarrierBand::E6,obs3.band);
   TUASSERTE(gnsstk::TrackingCode,gnsstk::TrackingCode::LEXL,obs3.code);

   CONTEST("CL2I", gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I);
   CONTEST("CL2Q", gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1Q);
   CONTEST("CL2X", gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1IQ);
   CONTEST("CL1D", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::B1CD);
   CONTEST("CL1P", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::B1CP);
   CONTEST("CL1X", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::B1CDP);
   CONTEST("CL1A", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::B1A);
   CONTEST("CL1N", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::BCodeless);
      // these are only valid in rinex 3.02 and the macro defaults
      // to the current version, which is not 3.02.
      // CONTEST("CL1I", gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1I);
      // CONTEST("CL1Q", gnsstk::CarrierBand::B1, gnsstk::TrackingCode::B1Q);
   CONTEST("CL5D", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::B2aI);
   CONTEST("CL5P", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::B2aQ);
   CONTEST("CL5X", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::B2aIQ);
   CONTEST("CL7I", gnsstk::CarrierBand::B2, gnsstk::TrackingCode::B2I);
   CONTEST("CL7Q", gnsstk::CarrierBand::B2, gnsstk::TrackingCode::B2Q);
   CONTEST("CL7X", gnsstk::CarrierBand::B2, gnsstk::TrackingCode::B2IQ);
   CONTEST("CL7D", gnsstk::CarrierBand::B2, gnsstk::TrackingCode::B2bI);
   CONTEST("CL7P", gnsstk::CarrierBand::B2, gnsstk::TrackingCode::B2bQ);
   CONTEST("CL7Z", gnsstk::CarrierBand::B2, gnsstk::TrackingCode::B2bIQ);
   CONTEST("CL8D", gnsstk::CarrierBand::E5ab, gnsstk::TrackingCode::B2abI);
   CONTEST("CL8P", gnsstk::CarrierBand::E5ab, gnsstk::TrackingCode::B2abQ);
   CONTEST("CL8X", gnsstk::CarrierBand::E5ab, gnsstk::TrackingCode::B2abIQ);
   CONTEST("CL6I", gnsstk::CarrierBand::B3, gnsstk::TrackingCode::B3I);
   CONTEST("CL6Q", gnsstk::CarrierBand::B3, gnsstk::TrackingCode::B3Q);
   CONTEST("CL6X", gnsstk::CarrierBand::B3, gnsstk::TrackingCode::B3IQ);
   CONTEST("CL6A", gnsstk::CarrierBand::B3, gnsstk::TrackingCode::B3AIQ);
   CONTEST("EL1A", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::E1A);
   CONTEST("EL1B", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::E1B);
   CONTEST("EL1C", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::E1C);
   CONTEST("EL1X", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::E1BC);
   CONTEST("EL1Z", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::E1ABC);
   CONTEST("EL5I", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::E5aI);
   CONTEST("EL5Q", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::E5aQ);
   CONTEST("EL5X", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::E5aIQ);
   CONTEST("EL7I", gnsstk::CarrierBand::E5b, gnsstk::TrackingCode::E5bI);
   CONTEST("EL7Q", gnsstk::CarrierBand::E5b, gnsstk::TrackingCode::E5bQ);
   CONTEST("EL7X", gnsstk::CarrierBand::E5b, gnsstk::TrackingCode::E5bIQ);
   CONTEST("EL8I", gnsstk::CarrierBand::E5ab, gnsstk::TrackingCode::E5abI);
   CONTEST("EL8Q", gnsstk::CarrierBand::E5ab, gnsstk::TrackingCode::E5abQ);
   CONTEST("EL8X", gnsstk::CarrierBand::E5ab, gnsstk::TrackingCode::E5abIQ);
   CONTEST("EL6A", gnsstk::CarrierBand::E6, gnsstk::TrackingCode::E6A);
   CONTEST("EL6B", gnsstk::CarrierBand::E6, gnsstk::TrackingCode::E6B);
   CONTEST("EL6C", gnsstk::CarrierBand::E6, gnsstk::TrackingCode::E6C);
   CONTEST("EL6X", gnsstk::CarrierBand::E6, gnsstk::TrackingCode::E6BC);
   CONTEST("EL6Z", gnsstk::CarrierBand::E6, gnsstk::TrackingCode::E6ABC);
   CONTEST("RL1C", gnsstk::CarrierBand::G1, gnsstk::TrackingCode::Standard);
   CONTEST("RL1P", gnsstk::CarrierBand::G1, gnsstk::TrackingCode::Precise);
   CONTEST("RL4A", gnsstk::CarrierBand::G1a, gnsstk::TrackingCode::L1OCD);
   CONTEST("RL4B", gnsstk::CarrierBand::G1a, gnsstk::TrackingCode::L1OCP);
   CONTEST("RL4X", gnsstk::CarrierBand::G1a, gnsstk::TrackingCode::L1OCDP);
   CONTEST("RL2C", gnsstk::CarrierBand::G2, gnsstk::TrackingCode::Standard);
   CONTEST("RL2P", gnsstk::CarrierBand::G2, gnsstk::TrackingCode::Precise);
   CONTEST("RL6A", gnsstk::CarrierBand::G2a, gnsstk::TrackingCode::L2CSI);
   CONTEST("RL6B", gnsstk::CarrierBand::G2a, gnsstk::TrackingCode::L2OCP);
   CONTEST("RL6X", gnsstk::CarrierBand::G2a, gnsstk::TrackingCode::L2CSIL2OCp);
   CONTEST("RL3I", gnsstk::CarrierBand::G3, gnsstk::TrackingCode::L3OCD);
   CONTEST("RL3Q", gnsstk::CarrierBand::G3, gnsstk::TrackingCode::L3OCP);
   CONTEST("RL3X", gnsstk::CarrierBand::G3, gnsstk::TrackingCode::L3OCDP);
   CONTEST("GL1C", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA);
   CONTEST("GL1S", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD);
   CONTEST("GL1L", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CP);
   CONTEST("GL1X", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CDP);
   CONTEST("GL1P", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::P);
   CONTEST("GL1W", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::Ztracking);
   CONTEST("GL1Y", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::Y);
   CONTEST("GL1M", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::MDP);
   CONTEST("GL1N", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::YCodeless);
   CONTEST("GL2C", gnsstk::CarrierBand::L2, gnsstk::TrackingCode::CA);
   CONTEST("GL2D", gnsstk::CarrierBand::L2, gnsstk::TrackingCode::Semicodeless);
   CONTEST("GL2S", gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CM);
   CONTEST("GL2L", gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CL);
   CONTEST("GL2X", gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CML);
   CONTEST("GL2P", gnsstk::CarrierBand::L2, gnsstk::TrackingCode::P);
   CONTEST("GL2W", gnsstk::CarrierBand::L2, gnsstk::TrackingCode::Ztracking);
   CONTEST("GL2Y", gnsstk::CarrierBand::L2, gnsstk::TrackingCode::Y);
   CONTEST("GL2M", gnsstk::CarrierBand::L2, gnsstk::TrackingCode::MDP);
   CONTEST("GL2N", gnsstk::CarrierBand::L2, gnsstk::TrackingCode::YCodeless);
   CONTEST("GL5I", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I);
   CONTEST("GL5Q", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5Q);
   CONTEST("GL5X", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5IQ);
   CONTEST("IL5A", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::SPSL5);
   CONTEST("IL5B", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::RSL5D);
   CONTEST("IL5C", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::RSL5P);
   CONTEST("IL5X", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::RSL5DP);
   CONTEST("IL9A", gnsstk::CarrierBand::I9, gnsstk::TrackingCode::SPSS);
   CONTEST("IL9B", gnsstk::CarrierBand::I9, gnsstk::TrackingCode::RSSD);
   CONTEST("IL9C", gnsstk::CarrierBand::I9, gnsstk::TrackingCode::RSSP);
   CONTEST("IL9X", gnsstk::CarrierBand::I9, gnsstk::TrackingCode::RSSDP);
   CONTEST("JL1C", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA);
   CONTEST("JL1L", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CP);
   CONTEST("JL1S", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CD);
   CONTEST("JL1X", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1CDP);
   CONTEST("JL1Z", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::L1S);
   CONTEST("JL2S", gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CM);
   CONTEST("JL2L", gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CL);
   CONTEST("JL2X", gnsstk::CarrierBand::L2, gnsstk::TrackingCode::L2CML);
   CONTEST("JL5I", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I);
   CONTEST("JL5Q", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5Q);
   CONTEST("JL5X", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5IQ);
   CONTEST("JL5D", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5SI);
   CONTEST("JL5P", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5SQ);
   CONTEST("JL5Z", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5SIQ);
   CONTEST("JL6S", gnsstk::CarrierBand::E6, gnsstk::TrackingCode::LEXS);
      // This is a duplicate of the previous one only with
      // different expectations so we have to ignore one or the
      // other.  I chose to ignore this one since the previous one
      // is how we've been decoding things in the past.
      //CONTEST("JL6S", gnsstk::CarrierBand::E6, gnsstk::TrackingCode::L6D);
   CONTEST("JL6L", gnsstk::CarrierBand::E6, gnsstk::TrackingCode::LEXL);
   CONTEST("JL6X", gnsstk::CarrierBand::E6, gnsstk::TrackingCode::LEXSL);
   CONTEST("JL6E", gnsstk::CarrierBand::E6, gnsstk::TrackingCode::L6E);
   CONTEST("JL6Z", gnsstk::CarrierBand::E6, gnsstk::TrackingCode::L6DE);
   CONTEST("SL1C", gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA);
   CONTEST("SL5I", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5I);
   CONTEST("SL5Q", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5Q);
   CONTEST("SL5X", gnsstk::CarrierBand::L5, gnsstk::TrackingCode::L5IQ);

   gnsstk::RinexObsID wild("****", gnsstk::Rinex3ObsBase::currentVersion);
   TUASSERTE(gnsstk::ObservationType, gnsstk::ObservationType::Any, wild.type);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::Any, wild.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::Any, wild.code);

   TURETURN();
}


unsigned RinexObsID_T :: newIDTest()
{
   TUDEF("RinexObsID", "newID");
   std::string failMesg;

   failMesg = "[testing] RinexObsID::newID to redefine existing ID, [expected]"
      " exception gnsstk::Exception, [actual] threw no exception";
   try
   {
      gnsstk::RinexObsID::newID("C6Z", "L6 Z range");
      TUFAIL(failMesg);
   }
   catch(gnsstk::Exception e)
   {
      TUPASS(failMesg);
   }

      //create a fictional ID completely
   gnsstk::RinexObsID fic(gnsstk::RinexObsID::newID("T9W", "L9 W test"));
   TUASSERT(gnsstk::RinexObsID::char2ot.count('T') > 0);
   TUASSERT(gnsstk::RinexObsID::char2cb.count('9') > 0);
   TUASSERT(gnsstk::RinexObsID::char2tc.count('W') > 0);
   TUASSERTE(gnsstk::ObservationType,
             fic.type, gnsstk::RinexObsID::char2ot['T']);
   TUASSERTE(gnsstk::CarrierBand,
             fic.band, gnsstk::RinexObsID::char2cb['9']);
   TUASSERTE(gnsstk::TrackingCode,
             fic.code, gnsstk::RinexObsID::char2tc['W']);

   failMesg = "[testing] RinexObsID::newID to redefine existing ID, [expected]"
      " exception gnsstk::Exception, [actual] threw no exception";
   try
   {
      gnsstk::RinexObsID::newID("T9W", "L9 W test");
      TUFAIL(failMesg);
   }
   catch(gnsstk::Exception e)
   {
      TUPASS(failMesg);
   }

   TURETURN();
}


int main() //Main function to initialize and run all tests above
{
   unsigned errorTotal = 0;
   RinexObsID_T testClass;

   errorTotal += testClass.decodeTest();
   errorTotal += testClass.fromStringConstructorTest();
   errorTotal += testClass.newIDTest();
   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;
   return errorTotal;
}
