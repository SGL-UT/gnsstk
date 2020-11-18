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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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
#include "PNBGPSCNavDataFactory.hpp"
#include "GPSCNavAlm.hpp"
#include "GPSCNavRedAlm.hpp"
#include "GPSCNavEph.hpp"
#include "GPSCNavHealth.hpp"
#include "GPSCNavTimeOffset.hpp"
#include "TimeCorrection.hpp"

using namespace std;

// This enum is only used here and it's being used to avoid confusion
// on vector indices.  Using enums instead of constants because it
// doesn't use any memory that way.  Also not using strongly typed
// enums because we actually want these values to be implicitly cast
// to integer values.
enum SFIndex
{
   ephM10 = 0,  ///< Message type 10
   ephM11 = 1,  ///< Message type 11
   ephMClk = 2  ///< Clock message (any of type 30-37)
};

/// More CNAV-related constants.

/** "Subframe" (message vector) index, start bits, bit counts and
 * scale factor (*n for integer quantities, *2^n for floating point
 * quantities) for each of the ephemeris fields.
 * Bit positions/sizes from IS-GPS-200 Figures 30-1 and 30-2.
 * Scale factors taken from Tables 20-I and 20-III.
 * @note When the comment for an esc* enum says "scale factor", the
 *   bits are multiplied by that scalar.  When the comment says "power
 *   factor", the bits are multiplied by 2^n where n is the esc*
 *   value.
 */
enum CNavBitInfo
{
      // every message has a preamble so no index here.
      /// @todo Make sure all the scale factors are correct.
   esbPre = 0,            ///< Preamble start bit
   enbPre = 8,            ///< Preamble number of bits
   escPre = 1,            ///< Preamble scale factor

   esbPRN = 8,            ///< PRN start bit
   enbPRN = 6,            ///< PRN number of bits
   escPRN = 1,            ///< PRN scale factor

   esbMsgType = 14,       ///< Message type start bit
   enbMsgType = 6,        ///< Message type number of bits
   escMsgType = 1,        ///< Message type scale factor

   esbTOW = 20,           ///< TOW start bit
   enbTOW = 17,           ///< TOW number of bits
   escTOW = 6,            ///< TOW scale factor

   esbAlert = 37,         ///< Alert flag start bit
   enbAlert = 1,          ///< Alert flag number of bits
   escAlert = 1,          ///< Alert flag scale factor

   esiWN = ephM10,        ///< WN message index
   esbWN = 38,            ///< WN start bit
   enbWN = 13,            ///< WN number of bits
   escWN = 1,             ///< WN scale factor

   esiHea   = ephM10,     ///< Signal health message index
   esbHeaL1 = 51,         ///< L1 Signal health bit
   esbHeaL2 = 52,         ///< L2 Signal health bit
   esbHeaL5 = 53,         ///< L5 Signal health bit

   esitop = ephM10,       ///< top message index
   esbtop = 54,           ///< top start bit
   enbtop = 11,           ///< top number of bits
   esctop = 300,          ///< top scale factor

   esiURA = ephM10,       ///< URA index message index
   esbURA = 65,           ///< URA index start bit
   enbURA = 5,            ///< URA index number of bits
   escURA = 1,            ///< URA index scale factor

   esitoe1 = ephM10,      ///< toe (ephemeris 1) message index
   esbtoe1 = 70,          ///< toe (ephemeris 1) start bit
   enbtoe1 = 11,          ///< toe (ephemeris 1) number of bits
   esctoe1 = 300,         ///< toe (ephemeris 1) scale factor

   esidA = ephM10,        ///< Delta A message index
   esbdA = 81,            ///< Delta A start bit
   enbdA = 26,            ///< Delta A number of bits
   escdA = -9,            ///< Delta A power factor

   esiAdot = ephM10,      ///< Adot message index
   esbAdot = 107,         ///< Adot start bit
   enbAdot = 25,          ///< Adot number of bits
   escAdot = -21,         ///< Adot power factor

   esidn0 = ephM10,       ///< Delta n0 message index
   esbdn0 = 132,          ///< Delta n0 start bit
   enbdn0 = 17,           ///< Delta n0 number of bits
   escdn0 = -44,          ///< Delta n0 power factor

   esidn0dot = ephM10,    ///< Delta n0 dot message index
   esbdn0dot = 149,       ///< Delta n0 dot start bit
   enbdn0dot = 23,        ///< Delta n0 dot number of bits
   escdn0dot = -57,       ///< Delta n0 dot power factor

   esiM0 = ephM10,        ///< M0-n message index
   esbM0 = 172,           ///< M0-n start bit
   enbM0 = 33,            ///< M0-n number of bits
   escM0 = -32,           ///< M0-n power factor

   esiEcc = ephM10,       ///< Ecc n message index
   esbEcc = 205,          ///< Ecc n start bit
   enbEcc = 33,           ///< Ecc n number of bits
   escEcc = -34,          ///< Ecc n power factor

   esiw = ephM10,         ///< w message index
   esbw = 238,            ///< w start bit
   enbw = 33,             ///< w number of bits
   escw = -32,            ///< w power factor

      // stored as bool, no scale/bit count needed
   esiInt = ephM10,       ///< Integrity status flag message index
   esbInt = 271,          ///< Integrity status flag start bit

   esiPL2C = ephM10,      ///< L2C Phasing message index
   esbPL2C = 272,         ///< L2C Phasing start bit

   esitoe2 = ephM11,      ///< toe (ephemeris 2) message index
   esbtoe2 = 38,          ///< toe (ephemeris 2) start bit
   enbtoe2 = 11,          ///< toe (ephemeris 2) number of bits
   esctoe2 = 300,         ///< toe (ephemeris 2) scale factor

   esiOMEGA0 = ephM11,    ///< OMEGA0-n message index
   esbOMEGA0 = 49,        ///< OMEGA0-n MSBs start bit
   enbOMEGA0 = 33,        ///< OMEGA0-n MSBs number of bits
   escOMEGA0 = -32,       ///< OMEGA0-n power factor

   esii0 = ephM11,        ///< i0-n message index
   esbi0 = 82,            ///< i0-n MSBs start bit
   enbi0 = 33,            ///< i0-n MSBs number of bits
   esci0 = -32,           ///< i0-n power factor

   esidOMEGAdot = ephM11, ///< Delta OMEGAdot message index
   esbdOMEGAdot = 115,    ///< Delta OMEGAdot start bit
   enbdOMEGAdot = 17,     ///< Delta OMEGAdot number of bits
   escdOMEGAdot = -44,    ///< Delta OMEGAdot power factor

   esiidot = ephM11,      ///< i0-n - DOT message index
   esbidot = 132,         ///< i0-n - DOT start bit
   enbidot = 15,          ///< i0-n - DOT number of bits
   escidot = -44,         ///< i0-n - DOT power factor

   esiCis = ephM11,       ///< Cis-n message index
   esbCis = 147,          ///< Cis-n start bit
   enbCis = 16,           ///< Cis-n number of bits
   escCis = -30,          ///< Cis-n power factor

   esiCic = ephM11,       ///< Cic-n message index
   esbCic = 163,          ///< Cic-n start bit
   enbCic = 16,           ///< Cic-n number of bits
   escCic = -30,          ///< Cic-n power factor

   esiCrs = ephM11,       ///< Crs-n message index
   esbCrs = 179,          ///< Crs-n start bit
   enbCrs = 24,           ///< Crs-n number of bits
   escCrs = -8,           ///< Crs-n power factor

   esiCrc = ephM11,       ///< Crc-n message index
   esbCrc = 203,          ///< Crc-n start bit
   enbCrc = 24,           ///< Crc-n number of bits
   escCrc = -8,           ///< Crc-n power factor

   esiCus = ephM11,       ///< Cus-n message index
   esbCus = 227,          ///< Cus-n start bit
   enbCus = 21,           ///< Cus-n number of bits
   escCus = -30,          ///< Cus-n power factor

   esiCuc = ephM11,       ///< Cuc-n message index
   esbCuc = 248,          ///< Cuc-n start bit
   enbCuc = 21,           ///< Cuc-n number of bits
   escCuc = -30,          ///< Cuc-n power factor

      /// @note these apply to all clock messages, type 30-37
   csitop = ephMClk,      ///< top message index
   csbtop = 38,           ///< top start bit
   cnbtop = 11,           ///< top number of bits
   csctop = 300,          ///< top scale factor

   csiURAned0 = ephMClk,  ///< URA_NED0 message index
   csbURAned0 = 49,       ///< URA_NED0 start bit
   cnbURAned0 = 5,        ///< URA_NED0 number of bits
   cscURAned0 = 1,        ///< URA_NED0 scale factor

   csiURAned1 = ephMClk,  ///< URA_NED1 message index
   csbURAned1 = 54,       ///< URA_NED1 start bit
   cnbURAned1 = 3,        ///< URA_NED1 number of bits
   cscURAned1 = 1,        ///< URA_NED1 scale factor

   csiURAned2 = ephMClk,  ///< URA_NED2 message index
   csbURAned2 = 57,       ///< URA_NED2 start bit
   cnbURAned2 = 3,        ///< URA_NED2 number of bits
   cscURAned2 = 1,        ///< URA_NED2 scale factor

   csitoc = ephMClk,      ///< toc message index
   csbtoc = 60,           ///< toc start bit
   cnbtoc = 11,           ///< toc number of bits
   csctoc = 300,          ///< toc scale factor

   csiaf0 = ephMClk,      ///< af0 message index
   csbaf0 = 71,           ///< af0 start bit
   cnbaf0 = 26,           ///< af0 number of bits
   cscaf0 = -35,          ///< af0 power factor

   csiaf1 = ephMClk,      ///< af1 message index
   csbaf1 = 97,           ///< af1 start bit
   cnbaf1 = 20,           ///< af1 number of bits
   cscaf1 = -48,          ///< af1 power factor

   csiaf2 = ephMClk,      ///< af2 message index
   csbaf2 = 117,          ///< af2 start bit
   cnbaf2 = 10,           ///< af2 number of bits
   cscaf2 = -60,          ///< af2 power factor

      /// @note these apply to message type 37 (midi almanac)

   asbWNa = 127,          ///< WNa-n start bit
   anbWNa = 13,           ///< WNa-n number of bits
   ascWNa = 1,            ///< WNa-n scale factor

   asbtoa = 140,          ///< toa start bit
   anbtoa = 8,            ///< toa number of bits
   asctoa = 12,           ///< toa power factor

   asbPRNa = 148,         ///< PRNa start bit
   anbPRNa = 6,           ///< PRNa number of bits
   ascPRNa = 1,           ///< PRNa scale factor

   asbHeaL1 = 154,        ///< L1 Signal health bit
   asbHeaL2 = 155,        ///< L2 Signal health bit
   asbHeaL5 = 156,        ///< L5 Signal health bit

   asbEcc = 157,          ///< Ecc n start bit
   anbEcc = 11,           ///< Ecc n number of bits
   ascEcc = -16,          ///< Ecc n power factor

   asbdi = 168,           ///< delta i start bit
   anbdi = 11,            ///< delta i n number of bits
   ascdi = -14,           ///< delta i n power factor

   asbOMEGAdot = 179,     ///< OMEGAdot start bit
   anbOMEGAdot = 11,      ///< OMEGAdot number of bits
   ascOMEGAdot = -33,     ///< OMEGAdot power factor

   asbAhalf = 190,        ///< Ahalf MSBs start bit
   anbAhalf = 17,         ///< Ahalf MSBs number of bits
   ascAhalf = -4,         ///< Ahalf scale factor

   asbOMEGA0 = 207,       ///< OMEGA0 MSBs start bit
   anbOMEGA0 = 16,        ///< OMEGA0 MSBs number of bits
   ascOMEGA0 = -15,       ///< OMEGA0 power factor

   asbw = 223,            ///< w start bit
   anbw = 16,             ///< w number of bits
   ascw = -15,            ///< w power factor

   asbM0 = 239,           ///< M0 start bit
   anbM0 = 16,            ///< M0 number of bits
   ascM0 = -15,           ///< M0 power factor

   asbaf0 = 255,          ///< af0 start bit
   anbaf0 = 11,           ///< af0 number of bits
   ascaf0 = -20,          ///< af0 power factor

   asbaf1 = 266,          ///< af1 start bit
   anbaf1 = 10,           ///< af1 number of bits
   ascaf1 = -37,          ///< af1 power factor

      /// @note these apply to message type 33 (clock & UTC)

   csbA0 = 127,           ///< A0-n start bit
   cnbA0 = 16,            ///< A0-n number of bits
   cscA0 = -35,           ///< A0-n power factor

   csbA1 = 143,           ///< A1-n start bit
   cnbA1 = 13,            ///< A1-n number of bits
   cscA1 = -51,           ///< A1-n power factor

   csbA2 = 156,           ///< A2-n start bit
   cnbA2 = 7,             ///< A2-n number of bits
   cscA2 = -68,           ///< A2-n power factor

   csbdtLS = 163,         ///< dtLS start bit
   cnbdtLS = 8,           ///< dtLS number of bits
   cscdtLS = 1,           ///< dtLS scale factor

   csbtot = 171,          ///< tot start bit
   cnbtot = 16,           ///< tot number of bits
   csctot = 4,            ///< tot power factor

   csbWNot = 187,         ///< WNot start bit
   cnbWNot = 13,          ///< WNot number of bits
   cscWNot = 1,           ///< WNot scale factor

   csbWNlsf = 200,        ///< WNlsf start bit
   cnbWNlsf = 13,         ///< WNlsf number of bits
   cscWNlsf = 1,          ///< WNlsf scale factor

   csbDN = 213,           ///< DN start bit
   cnbDN = 4,             ///< DN number of bits
   cscDN = 1,             ///< DN scale factor

   csbdtLSF = 217,        ///< dtLSF start bit
   cnbdtLSF = 8,          ///< dtLSF number of bits
   cscdtLSF = 1,          ///< dtLSF scale factor

      /// @note these apply to message type 35 (clock & GGTO)

   gsbt = 127,            ///< tggto start bit
   gnbt = 16,             ///< tggto number of bits
   gsct = 4,              ///< tggto power factor

   gsbWN = 143,           ///< WNggto start bit
   gnbWN = 13,            ///< WNggto number of bits
   gscWN = 1,             ///< WNggto scale factor

   gsbGNSS = 156,         ///< GNSS ID start bit
   gnbGNSS = 3,           ///< GNSS ID number of bits
   gscGNSS = 1,           ///< GNSS ID scale factor

   gsbA0 = 159,           ///< A0-n start bit
   gnbA0 = 16,            ///< A0-n number of bits
   gscA0 = -35,           ///< A0-n power factor

   gsbA1 = 175,           ///< A1-n start bit
   gnbA1 = 13,            ///< A1-n number of bits
   gscA1 = -51,           ///< A1-n power factor

   gsbA2 = 188,           ///< A2-n start bit
   gnbA2 = 7,             ///< A2-n number of bits
   gscA2 = -68,           ///< A2-n power factor

      /** @note these apply to message type 12 and 31 but are the
       * "message level" data, rather than "packet level" data. */

   rsb12WNa = 38,         ///< message type 12, WNa start bit
   rnb12WNa = 13,         ///< message type 12, WNa number of bits
   rsc12WNa = 1,          ///< message type 12, WNa scale factor

   rsb12toa = 51,         ///< message type 12, toa start bit
   rnb12toa = 8,          ///< message type 12, toa number of bits
   rsc12toa = 12,         ///< message type 12, toa power factor

   rsb12p1 = 59,          ///< message type 12, packet 1 start bit
   rsb12p2 = 90,          ///< message type 12, packet 2 start bit
   rsb12p3 = 121,         ///< message type 12, packet 3 start bit
   rsb12p4 = 152,         ///< message type 12, packet 4 start bit
   rsb12p5 = 183,         ///< message type 12, packet 5 start bit
   rsb12p6 = 214,         ///< message type 12, packet 6 start bit
   rsb12p7 = 245,         ///< message type 12, packet 7 start bit

   rsb31WNa = 127,        ///< message type 31, WNa start bit
   rnb31WNa = 13,         ///< message type 31, WNa number of bits
   rsc31WNa = 1,          ///< message type 31, WNa scale factor

   rsb31toa = 140,        ///< message type 31, toa start bit
   rnb31toa = 8,          ///< message type 31, toa number of bits
   rsc31toa = 31,         ///< message type 31, toa scale factor

   rsb31p1 = 148,         ///< message type 31, packet 1 start bit
   rsb31p2 = 179,         ///< message type 31, packet 2 start bit
   rsb31p3 = 210,         ///< message type 31, packet 3 start bit
   rsb31p4 = 241,         ///< message type 31, packet 4 start bit

      /** @note these apply to message types 12 and 31 (reduced
       * almanac), but are specific to the 31-bit packets, in that the
       * start bits are relative to the start of the packet rather
       * than the start of the message. */

   rsbPRNa = 0,           ///< PRNa start bit
   rnbPRNa = 6,           ///< PRNa number of bits
   rscPRNa = 1,           ///< PRNa scale factor

   rsbdA = 6,             ///< dA start bit
   rnbdA = 8,             ///< dA number of bits
   rscdA = 9,             ///< dA power factor

   rsbOMEGA0 = 14,        ///< OMEGA0 start bit
   rnbOMEGA0 = 7,         ///< OMEGA0 number of bits
   rscOMEGA0 = -6,        ///< OMEGA0 power factor

   rsbPHI0 = 21,          ///< PHI0 start bit
   rnbPHI0 = 7,           ///< PHI0 number of bits
   rscPHI0 = -6,          ///< PHI0 power factor

   rsbHeaL1 = 28,         ///< L1 Signal health bit
   rsbHeaL2 = 29,         ///< L2 Signal health bit
   rsbHeaL5 = 30,         ///< L5 Signal health bit
   
};


namespace gpstk
{
   bool PNBGPSCNavDataFactory ::
   addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      if ((navIn->getNavID().navType != NavType::GPSCNAVL2) &&
          (navIn->getNavID().navType != NavType::GPSCNAVL5))
      {
            // This class only processes GPS CNav.
         return false;
      }
      bool rv = true;
      bool isQZSS = navIn->getsatSys().system == SatelliteSystem::QZSS;
      try
      {
            /*
         cerr << "preamble:  " << hex << navIn->asUnsignedLong(0,8,1) << dec
              << endl
              << "  tlm msg: " << hex << navIn->asUnsignedLong(8,14,1) << dec
              << endl
              << "  integ:   " << navIn->asUnsignedLong(22,1,1) << endl
              << "  reserved:" << navIn->asUnsignedLong(23,1,1) << endl
              << "  parity:  " << hex << navIn->asUnsignedLong(24,6,1) << endl
              << "  tow:     " << navIn->asUnsignedLong(30,17,1) << endl
              << "  alert:   " << navIn->asUnsignedLong(47,1,1) << endl
              << "  A/S:     " << navIn->asUnsignedLong(48,1,1) << endl
              << "  sfid:    " << navIn->asUnsignedLong(49,3,1) << endl;
            */
         unsigned long msgType = navIn->asUnsignedLong(esbMsgType,enbMsgType,
                                                       escMsgType);
         // cerr << "msgType = " << msgType << endl;
         unsigned long svid = 0;
            // Clock messages (30-37) may get processed twice, once
            // for ephemeris and once for whatever else they might
            // contain.
         if ((msgType == 10) || (msgType == 11) ||
             ((msgType >= 30) && (msgType <= 37)) ||
             (isQZSS && ((msgType == 46) || (msgType == 47) ||
                         (msgType == 49) || (msgType == 51) ||
                         (msgType == 53))))
         {
            rv = rv && processEph(msgType, navIn, navOut);
         }
         if ((msgType == 31) || (isQZSS && (msgType == 47)))
         {
            rv = rv && process31(msgType, navIn, navOut);
         }
         else if ((msgType == 12) || (isQZSS && (msgType == 28)))
         {
            rv = rv && process12(msgType, navIn, navOut);
         }
         else if ((msgType == 33) || (isQZSS && (msgType == 49)))
         {
            rv = rv && process33(navIn, navOut);
         }
         else if ((msgType == 35) || (isQZSS && (msgType == 51)))
         {
            rv = rv && process35(navIn, navOut);
         }
         else if ((msgType == 37) || (isQZSS && (msgType == 53)))
         {
            rv = rv && processAlmOrb(msgType, navIn, navOut);
         }
         // cerr << "  results: " << navOut.size() << endl;
         // for (const auto& i : navOut)
         //    i->dump(cerr,gpstk::NavData::Detail::Full);
      }
      catch (gpstk::Exception& exc)
      {
         rv = false;
         cerr << exc << endl;
      }
      catch (std::exception& exc)
      {
         rv = false;
         cerr << exc.what() << endl;
      }
      catch (...)
      {
         rv = false;
         cerr << "Unknown exception" << endl;
      }
      return rv;
   }


   bool PNBGPSCNavDataFactory ::
   processEph(unsigned msgType, const PackedNavBitsPtr& navIn,
              NavDataPtrList& navOut)
   {
         /** @todo Should we ignore the PNB satellite ID and just use
          * the PRN in the nav message, or maybe compare the two as an
          * validity check? */
      unsigned prn = navIn->getsatSys().id;
      unsigned vecIdx;
      switch (msgType)
      {
         case 10:
            vecIdx = ephM10;
            break;
         case 11:
            vecIdx = ephM11;
            break;
         case 30:
         case 31:
         case 32:
         case 33:
         case 34:
         case 35:
         case 36:
         case 37:
               /** @todo Is this strictly relevant for ephemeris
                * assembly?  Seems like if these are rebroadcast
                * messages they're not going to correspond to the
                * ephemeris data being broadcast by QZSS. */
         case 46: // QZSS rebroadcast of GPS MT 30
         case 47: // QZSS rebroadcast of GPS MT 31
         case 49: // QZSS rebroadcast of GPS MT 33
         case 51: // QZSS rebroadcast of GPS MT 35
         case 53: // QZSS rebroadcast of GPS MT 37
            vecIdx = ephMClk;
            break;
         default:
            return false;
      }
      if ((msgType == 10) && processHea)
      {
            // Add ephemeris health bits from message type 10.
         NavDataPtr p1L1 = std::make_shared<GPSCNavHealth>();
         NavDataPtr p1L2 = std::make_shared<GPSCNavHealth>();
         NavDataPtr p1L5 = std::make_shared<GPSCNavHealth>();
         p1L1->timeStamp = navIn->getTransmitTime();
         p1L2->timeStamp = navIn->getTransmitTime();
         p1L5->timeStamp = navIn->getTransmitTime();
            /** @todo I'm not entirely sure what's appropriate here.
             * The source signal is actually L2C CNAV but it's
             * broadcasting signal status for L1 signals that don't
             * have CNAV. */
         p1L1->signal = NavMessageID(
            NavSatelliteID(prn, prn, navIn->getsatSys().system,
                           CarrierBand::L1, TrackingCode::CA,
                           NavType::GPSLNAV),
            NavMessageType::Health);
         if (navIn->getobsID().band == CarrierBand::L2)
         {
            p1L2->signal = NavMessageID(
               NavSatelliteID(prn, navIn->getsatSys(), navIn->getobsID(),
                              navIn->getNavID()),
               NavMessageType::Health);
            p1L5->signal = NavMessageID(
               NavSatelliteID(prn, prn, navIn->getsatSys().system,
                              CarrierBand::L5, TrackingCode::L5I,
                              NavType::GPSCNAVL5),
               NavMessageType::Health);
         }
         else
         {
               // assume L5
            p1L2->signal = NavMessageID(
               NavSatelliteID(prn, prn, navIn->getsatSys().system,
                              CarrierBand::L2, TrackingCode::L2CM,
                              NavType::GPSCNAVL2),
               NavMessageType::Health);
            p1L5->signal = NavMessageID(
               NavSatelliteID(prn, navIn->getsatSys(), navIn->getobsID(),
                              navIn->getNavID()),
               NavMessageType::Health);
         }
         dynamic_cast<GPSCNavHealth*>(p1L1.get())->health =
            navIn->asBool(esbHeaL1);
         dynamic_cast<GPSCNavHealth*>(p1L2.get())->health =
            navIn->asBool(esbHeaL2);
         dynamic_cast<GPSCNavHealth*>(p1L5.get())->health =
            navIn->asBool(esbHeaL5);
         // cerr << "add CNAV eph health" << endl;
         navOut.push_back(p1L1);
         // cerr << "add CNAV eph health" << endl;
         navOut.push_back(p1L2);
         // cerr << "add CNAV eph health" << endl;
         navOut.push_back(p1L5);
      }
      if (!PNBNavDataFactory::processEph)
      {
            // User doesn't want ephemerides so don't do any processing.
         return true;
      }
      if (ephAcc.find(prn) == ephAcc.end())
      {
            // set up a new ephemeris
         ephAcc[prn].resize(3);
         ephAcc[prn][vecIdx] = navIn;
            // nothing in navOut yet and no further processing because
            // we only have one of three message types at this point.
         return true;
      }
      std::vector<PackedNavBitsPtr> &ephSF(ephAcc[prn]);
      ephSF[vecIdx] = navIn;
         // stop processing if we don't have three full message types
      if (!ephSF[ephM10] || !ephSF[ephM11] || !ephSF[ephMClk] ||
          (ephSF[ephM10]->getNumBits() != 300) ||
          (ephSF[ephM11]->getNumBits() != 300) ||
          (ephSF[ephMClk]->getNumBits() != 300))
      {
         // cerr << "Not ready for full CNAV eph processing" << endl;
         return true;
      }
         // Stop processing if we don't have matching toe/toc in
         // each of the three message types.
         /** @note Some data elements, e.g. toe, are stored internally
          * as floating point, but are treated as integers in the
          * encoded message.  This is not a mistake.  It's mostly due
          * to how the scaling is handled for the quantity,
          * i.e. linear scaling vs fractional (ldexp). */
      double toe10 = ephSF[esitoe1]->asUnsignedLong(esbtoe1,enbtoe1,esctoe1);
      double toe11 = ephSF[esitoe2]->asUnsignedLong(esbtoe2,enbtoe2,esctoe2);
      double toc = ephSF[csitoc]->asUnsignedLong(csbtoc,cnbtoc,csctoc);
      if ((toe10 != toe11) || (toe10 != toc))
      {
         // cerr << "toe/toc mismatch, not processing" << endl;
            // Even though the mismatch might be considered an error,
            // we don't really want to mark it as such and rather
            // consider it as a "valid" but unprocessable data set.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNavEph>();
      GPSCNavEph *eph = dynamic_cast<GPSCNavEph*>(p0.get());
         // NavData
      eph->timeStamp = ephSF[ephM10]->getTransmitTime();
      eph->signal = NavMessageID(
         NavSatelliteID(prn, navIn->getsatSys(), navIn->getobsID(),
                        navIn->getNavID()),
         NavMessageType::Ephemeris);
      // cerr << "Ready for full CNAV eph processing for " << (NavSignalID)eph->signal << endl;
         // OrbitData = empty
         // OrbitDataKepler
      eph->xmitTime = eph->timeStamp;
         /** @todo apply 13-bit week rollover adjustment, not 10-bit.
          * Must be completed by January, 2137 :-) */
      unsigned wn = ephSF[esiWN]->asUnsignedLong(esbWN,enbWN,escWN);
         // Use the transmit time to get a full week for toe/toc
         //GPSWeekSecond refTime(eph->xmitTime);
         //long refWeek = refTime.week;
         //wn = timeAdjustWeekRollover(wn, refWeek);
         // Now we can set the Toe/Toc properly.  Note that IS-GPS-200
         // defines the toc and toe to be the same for a consistent
         // set of data, and we've already enforced they're the same
         // above.
      eph->Toe = eph->Toc = GPSWeekSecond(wn,toe10);
         // healthy is set below
      eph->Cuc = ephSF[esiCuc]->asSignedDouble(esbCuc,enbCuc,escCuc);
      eph->Cus = ephSF[esiCus]->asSignedDouble(esbCus,enbCus,escCus);
      eph->Crc = ephSF[esiCrc]->asSignedDouble(esbCrc,enbCrc,escCrc);
      eph->Crs = ephSF[esiCrs]->asSignedDouble(esbCrs,enbCrs,escCrs);
      eph->Cic = ephSF[esiCic]->asSignedDouble(esbCic,enbCic,escCic);
      eph->Cis = ephSF[esiCis]->asSignedDouble(esbCis,enbCis,escCis);
      eph->M0  = ephSF[esiM0]->asDoubleSemiCircles(esbM0,enbM0,escM0);
      eph->dn  = ephSF[esidn0]->asDoubleSemiCircles(esbdn0,enbdn0,escdn0);
      eph->dndot = ephSF[esidn0dot]->asDoubleSemiCircles(esbdn0dot,enbdn0dot,
                                                         escdn0dot);
      eph->ecc = ephSF[esiEcc]->asUnsignedDouble(esbEcc,enbEcc,escEcc);
      eph->deltaA = ephSF[esidA]->asSignedDouble(esbdA,enbdA,escdA);
         /// @todo does this need changing for QZSS?
      eph->A = eph->deltaA + GPSCNavData::refAGPS;
      eph->Ahalf = ::sqrt(eph->A);
      eph->Adot = ephSF[esiAdot]->asSignedDouble(esbAdot,enbAdot,escAdot);
      eph->OMEGA0 = ephSF[esiOMEGA0]->asDoubleSemiCircles(esbOMEGA0,enbOMEGA0,
                                                          escOMEGA0);
      eph->i0 = ephSF[esii0]->asDoubleSemiCircles(esbi0,enbi0,esci0);
      eph->w = ephSF[esiw]->asDoubleSemiCircles(esbw,enbw,escw);
      eph->dOMEGAdot = ephSF[esidOMEGAdot]->asDoubleSemiCircles(
         esbdOMEGAdot,enbdOMEGAdot,escdOMEGAdot);
         /// @todo does this need changing for QZSS?
      eph->OMEGAdot = eph->dOMEGAdot + GPSCNavData::refOMEGAdotGPS;
      eph->idot = ephSF[esiidot]->asDoubleSemiCircles(esbidot,enbidot,escidot);
      eph->af0 = ephSF[csiaf0]->asSignedDouble(csbaf0,cnbaf0,cscaf0);
      eph->af1 = ephSF[csiaf1]->asSignedDouble(csbaf1,cnbaf1,cscaf1);
      eph->af2 = ephSF[csiaf2]->asSignedDouble(csbaf2,cnbaf2,cscaf2);
         // GPSCNavData
      eph->pre = ephSF[ephM10]->asUnsignedLong(esbPre,enbPre,escPre);
      eph->alert = ephSF[ephM10]->asBool(esbAlert);
         // GPSCNavEph
      eph->pre11 = ephSF[ephM11]->asUnsignedLong(esbPre,enbPre,escPre);
      eph->preClk = ephSF[ephMClk]->asUnsignedLong(esbPre,enbPre,escPre);
      eph->healthL1 = ephSF[esiHea]->asBool(esbHeaL1);
      eph->healthL2 = ephSF[esiHea]->asBool(esbHeaL2);
      eph->healthL5 = ephSF[esiHea]->asBool(esbHeaL5);
      switch (navIn->getobsID().band)
      {
         case CarrierBand::L2:
            eph->health = ((eph->healthL2 == false) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            break;
         case CarrierBand::L5:
            eph->health = ((eph->healthL5 == false) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            break;
         default:
               // unexpected/unsupported signal
            return false;
      }
      eph->uraED = ephSF[esiURA]->asLong(esbURA,enbURA,escURA);
      eph->alert11 = ephSF[ephM11]->asBool(esbAlert);
      eph->alertClk = ephSF[ephMClk]->asBool(esbAlert);
      double top = ephSF[esitop]->asUnsignedLong(esbtop,enbtop,esctop);
      eph->top = GPSWeekSecond(wn,top);
      eph->xmit11 = ephSF[ephM11]->getTransmitTime();
      eph->xmitClk = ephSF[ephMClk]->getTransmitTime();
      eph->uraNED0= ephSF[csiURAned0]->asLong(csbURAned0,cnbURAned0,cscURAned0);
      eph->uraNED1= ephSF[csiURAned1]->asUnsignedLong(csbURAned1,cnbURAned1,
                                                      cscURAned1);
      eph->uraNED2= ephSF[csiURAned2]->asUnsignedLong(csbURAned2,cnbURAned2,
                                                      cscURAned2);
      eph->fixFit();
      // cerr << "add CNAV eph" << endl;
      navOut.push_back(p0);
         // Clear out the broadcast ephemeris that's been processed.
      ephAcc.erase(prn);
      return true;
   }


   bool PNBGPSCNavDataFactory ::
   processAlmOrb(unsigned msgType, const PackedNavBitsPtr& navIn,
                 NavDataPtrList& navOut)
   {
      unsigned long sprn = navIn->asUnsignedLong(asbPRNa,anbPRNa,ascPRNa);
      SatID xmitSat(navIn->getsatSys());
      SatelliteSystem subjSys = xmitSat.system;
         // special handling for QZSS per IS-QZSS 1.8E Table 5.5.2-8
      if (subjSys == SatelliteSystem::QZSS)
      {
         if (msgType == 37)
         {
               // When the message type number is 37, it indicates
               // that this value is for a QZS satellite and
               // represents the last 6 bits of the QZS PRN number.
               // .. so we do a bitwise OR to get the QZS PRN.
            sprn |= 0xc0;
         }
         else if (msgType == 52)
         {
               // When the message type number is 53, it indicates
               // that this value is the PRN value for a GPS satellite
            subjSys = SatelliteSystem::GPS;
         }
      }
      SatID subjSat(sprn, subjSys);
         // No checks for correct svid, just assume that the input
         // data has already been checked (it will have been by
         // addData).
      if (processHea)
      {
            // Add almanac health bits from message type 37.
         NavDataPtr p1L1 = std::make_shared<GPSCNavHealth>();
         NavDataPtr p1L2 = std::make_shared<GPSCNavHealth>();
         NavDataPtr p1L5 = std::make_shared<GPSCNavHealth>();
         p1L1->timeStamp = navIn->getTransmitTime();
         p1L2->timeStamp = navIn->getTransmitTime();
         p1L5->timeStamp = navIn->getTransmitTime();
            /** @todo I'm not entirely sure what's appropriate here.
             * The source signal is actually L2C CNAV but it's
             * broadcasting signal status for L1 signals that don't
             * have CNAV. */
         p1L1->signal = NavMessageID(
            NavSatelliteID(subjSat, xmitSat,
                           ObsID(ObservationType::NavMsg, CarrierBand::L1,
                                 TrackingCode::CA),
                           NavID(NavType::GPSLNAV)),
            NavMessageType::Health);
         if (navIn->getobsID().band == CarrierBand::L2)
         {
            p1L2->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat, navIn->getobsID(),
                              navIn->getNavID()),
               NavMessageType::Health);
            p1L5->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat,
                              ObsID(ObservationType::NavMsg, CarrierBand::L5,
                                    TrackingCode::L5I),
                              NavID(NavType::GPSCNAVL5)),
               NavMessageType::Health);
         }
         else
         {
               // assume L5
            p1L2->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat,
                              ObsID(ObservationType::NavMsg, CarrierBand::L2,
                                    TrackingCode::L2CM),
                              NavID(NavType::GPSCNAVL2)),
               NavMessageType::Health);
            p1L5->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat, navIn->getobsID(),
                              navIn->getNavID()),
               NavMessageType::Health);
         }
         dynamic_cast<GPSCNavHealth*>(p1L1.get())->health =
            navIn->asBool(asbHeaL1);
         dynamic_cast<GPSCNavHealth*>(p1L2.get())->health =
            navIn->asBool(asbHeaL2);
         dynamic_cast<GPSCNavHealth*>(p1L5.get())->health =
            navIn->asBool(asbHeaL5);
         // cerr << "add CNAV alm health" << endl;
         navOut.push_back(p1L1);
         // cerr << "add CNAV alm health" << endl;
         navOut.push_back(p1L2);
         // cerr << "add CNAV alm health" << endl;
         navOut.push_back(p1L5);
      }
      if (!PNBNavDataFactory::processAlm)
      {
            // User doesn't want almanacs so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNavAlm>();
      GPSCNavAlm *alm = dynamic_cast<GPSCNavAlm*>(p0.get());
         // NavData
      alm->timeStamp = navIn->getTransmitTime();
      alm->signal = NavMessageID(
         NavSatelliteID(subjSat, xmitSat, navIn->getobsID(),
                        navIn->getNavID()),
         NavMessageType::Almanac);
         // OrbitData = empty
         // OrbitDataKepler
      alm->xmitTime = alm->timeStamp;
         /** @todo apply 13-bit week rollover adjustment, not 10-bit.
          * Must be completed by January, 2137 :-) */
      alm->wna = navIn->asUnsignedLong(asbWNa,anbWNa,ascWNa);
      alm->toa = navIn->asUnsignedDouble(asbtoa,anbtoa,asctoa);
      alm->Toc = alm->Toe = GPSWeekSecond(alm->wna,alm->toa);
      alm->M0 = navIn->asDoubleSemiCircles(asbM0,anbM0,ascM0);
      alm->ecc = navIn->asUnsignedDouble(asbEcc,anbEcc,ascEcc);
      alm->Ahalf = navIn->asUnsignedDouble(asbAhalf,anbAhalf,ascAhalf);
      alm->A = alm->Ahalf * alm->Ahalf;
      alm->OMEGA0 = navIn->asDoubleSemiCircles(asbOMEGA0,anbOMEGA0,ascOMEGA0);
         // i0 is set below
      alm->w = navIn->asDoubleSemiCircles(asbw,anbw,ascw);
      alm->OMEGAdot = navIn->asDoubleSemiCircles(asbOMEGAdot,anbOMEGAdot,
                                                 ascOMEGAdot);
      alm->af0 = navIn->asSignedDouble(asbaf0,anbaf0,ascaf0);
      alm->af1 = navIn->asSignedDouble(asbaf1,anbaf1,ascaf1);
         // GPSCNavData
      alm->pre = navIn->asUnsignedLong(esbPre,enbPre,escPre);
      alm->alert = navIn->asBool(esbAlert);
         // GPSCNavAlm
      alm->healthL1 = navIn->asBool(asbHeaL1);
      alm->healthL2 = navIn->asBool(asbHeaL2);
      alm->healthL5 = navIn->asBool(asbHeaL5);
      switch (navIn->getobsID().band)
      {
         case CarrierBand::L2:
            alm->health = ((alm->healthL2 == false) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            break;
         case CarrierBand::L5:
            alm->health = ((alm->healthL5 == false) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            break;
         default:
               // unexpected/unsupported signal
            return false;
      }
      alm->deltai = navIn->asDoubleSemiCircles(asbdi,anbdi,ascdi);
         /// @todo should this be different for QZSS?
      alm->i0 = GPSCNavData::refioffsetGPS + alm->deltai;
      alm->fixFit();
      // cerr << "add CNAV alm" << endl;
      navOut.push_back(p0);
      return true;
   }


   bool PNBGPSCNavDataFactory ::
   process12(unsigned msgType, const PackedNavBitsPtr& navIn,
             NavDataPtrList& navOut)
   {
      unsigned pre = navIn->asUnsignedLong(esbPre,enbPre,escPre);
      bool alert = navIn->asBool(esbAlert);
      unsigned wna = navIn->asUnsignedLong(rsb12WNa,rnb12WNa,rsc12WNa);
      unsigned long toa = navIn->asUnsignedLong(rsb12toa,rnb12toa,rsc12toa);
      return
         processRedAlmOrb(msgType,rsb12p1,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb12p2,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb12p3,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb12p4,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb12p5,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb12p6,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb12p7,pre,alert,wna,toa,navIn,navOut);
   }


   bool PNBGPSCNavDataFactory ::
   process31(unsigned msgType, const PackedNavBitsPtr& navIn,
             NavDataPtrList& navOut)
   {
      unsigned pre = navIn->asUnsignedLong(esbPre,enbPre,escPre);
      bool alert = navIn->asBool(esbAlert);
      unsigned wna = navIn->asUnsignedLong(rsb31WNa,rnb31WNa,rsc31WNa);
      unsigned long toa = navIn->asUnsignedLong(rsb31toa,rnb31toa,rsc31toa);
      return
         processRedAlmOrb(msgType,rsb31p1,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb31p2,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb31p3,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb31p4,pre,alert,wna,toa,navIn,navOut);
   }


   bool PNBGPSCNavDataFactory ::
   processRedAlmOrb(unsigned msgType, unsigned offset, unsigned pre, bool alert,
                    unsigned wna, double toa,
                    const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      SatID xmitSat(navIn->getsatSys());
      unsigned long sprn = navIn->asUnsignedLong(offset+rsbPRNa,rnbPRNa,
                                                 rscPRNa);
      SatelliteSystem subjSys = xmitSat.system;
      if (sprn == 0)
      {
            // empty almanac
         return true;
      }
         // special handling for QZSS per IS-QZSS 1.8E Table 5.5.2-9
      if (subjSys == SatelliteSystem::QZSS)
      {
         if ((msgType == 31) || (msgType == 12))
         {
               // When the message type number is 31 or 12, it
               // indicates that this value is for a QZS satellite and
               // represents the last 6 bits of the QZS PRN number.
               // .. so we do a bitwise OR to get the QZS PRN.
            sprn |= 0xc0;
         }
         else if ((msgType == 47) || (msgType == 28))
         {
               // When the message type number is 47 or 28, it
               // indicates that this value is the PRN value for a GPS
               // satellite (PRN No.= 1 ~ 32)
            subjSys = SatelliteSystem::GPS;
         }
      }
      SatID subjSat(sprn, subjSys);
      if (PNBNavDataFactory::processHea)
      {
            // Add reduced almanac health bits
         NavDataPtr p1L1 = std::make_shared<GPSCNavHealth>();
         NavDataPtr p1L2 = std::make_shared<GPSCNavHealth>();
         NavDataPtr p1L5 = std::make_shared<GPSCNavHealth>();
         p1L1->timeStamp = navIn->getTransmitTime();
         p1L2->timeStamp = navIn->getTransmitTime();
         p1L5->timeStamp = navIn->getTransmitTime();
            /** @todo I'm not entirely sure what's appropriate here.
             * The source signal is actually L2C CNAV but it's
             * broadcasting signal status for L1 signals that don't
             * have CNAV. */
         p1L1->signal = NavMessageID(
            NavSatelliteID(subjSat, xmitSat,
                           ObsID(ObservationType::NavMsg, CarrierBand::L1,
                                 TrackingCode::CA),
                           NavID(NavType::GPSLNAV)),
            NavMessageType::Health);
         if (navIn->getobsID().band == CarrierBand::L2)
         {
            p1L2->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat, navIn->getobsID(),
                              navIn->getNavID()),
               NavMessageType::Health);
            p1L5->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat,
                              ObsID(ObservationType::NavMsg, CarrierBand::L5,
                                    TrackingCode::L5I),
                              NavID(NavType::GPSCNAVL5)),
               NavMessageType::Health);
         }
         else
         {
               // assume L5
            p1L2->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat,
                              ObsID(ObservationType::NavMsg, CarrierBand::L2,
                                    TrackingCode::L2CM),
                              NavID(NavType::GPSCNAVL2)),
               NavMessageType::Health);
            p1L5->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat, navIn->getobsID(),
                              navIn->getNavID()),
               NavMessageType::Health);
         }
         dynamic_cast<GPSCNavHealth*>(p1L1.get())->health =
            navIn->asBool(offset+rsbHeaL1);
         dynamic_cast<GPSCNavHealth*>(p1L2.get())->health =
            navIn->asBool(offset+rsbHeaL2);
         dynamic_cast<GPSCNavHealth*>(p1L5.get())->health =
            navIn->asBool(offset+rsbHeaL5);
         // cerr << "add CNAV reduced alm health" << endl;
         navOut.push_back(p1L1);
         // cerr << "add CNAV reduced alm health" << endl;
         navOut.push_back(p1L2);
         // cerr << "add CNAV reduced alm health" << endl;
         navOut.push_back(p1L5);
      }
      if (!PNBNavDataFactory::processAlm)
      {
            // User doesn't want almanac data so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNavRedAlm>();
      GPSCNavRedAlm *alm = dynamic_cast<GPSCNavRedAlm*>(p0.get());
         // NavData
      alm->timeStamp = navIn->getTransmitTime();
      alm->signal = NavMessageID(
         NavSatelliteID(subjSat, xmitSat, navIn->getobsID(),
                        navIn->getNavID()),
         NavMessageType::Almanac);
         // OrbitData = empty
         // OrbitDataKepler
      alm->xmitTime = alm->timeStamp;
         /** @todo apply 13-bit week rollover adjustment, not 10-bit.
          * Must be completed by January, 2137 :-) */
      alm->Toc = alm->Toe = GPSWeekSecond(wna,toa);
      alm->OMEGA0 = navIn->asDoubleSemiCircles(offset+rsbOMEGA0,rnbOMEGA0,
                                               rscOMEGA0);
         // GPSCNavData
      alm->pre = pre;
      alm->alert = alert;
         // GPSCNavAlm
      alm->healthL1 = navIn->asBool(offset+rsbHeaL1);
      alm->healthL2 = navIn->asBool(offset+rsbHeaL2);
      alm->healthL5 = navIn->asBool(offset+rsbHeaL5);
      alm->wna = wna;
      alm->toa = toa;
      switch (navIn->getobsID().band)
      {
         case CarrierBand::L2:
            alm->health = ((alm->healthL2 == false) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            break;
         case CarrierBand::L5:
            alm->health = ((alm->healthL5 == false) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            break;
         default:
               // unexpected/unsupported signal
            return false;
      }
         // GPSCNavRedAlm
      alm->phi0 = navIn->asDoubleSemiCircles(offset+rsbPHI0,rnbPHI0,rscPHI0);
      alm->deltaA = navIn->asSignedDouble(offset+rsbdA,rnbdA,rscdA);
      alm->fixValues();
      alm->fixFit();
      // cerr << "add CNAV alm" << endl;
      navOut.push_back(p0);
      return true;
   }


   bool PNBGPSCNavDataFactory ::
   process33(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      if (!PNBNavDataFactory::processTim)
      {
            // User doesn't want time offset data so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNavTimeOffset>();
      p0->timeStamp = navIn->getTransmitTime();
      p0->signal = NavMessageID(
         NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                               navIn->getobsID(), navIn->getNavID()),
         NavMessageType::TimeOffset);
      GPSCNavTimeOffset *to =
         dynamic_cast<GPSCNavTimeOffset*>(p0.get());
      to->tgt = TimeSystem::UTC; // by definition
      to->a0 = navIn->asSignedDouble(csbA0,cnbA0,cscA0);
      to->a1 = navIn->asSignedDouble(csbA1,cnbA1,cscA1);
      to->a2 = navIn->asSignedDouble(csbA2,cnbA2,cscA2);
      to->deltatLS = navIn->asLong(csbdtLS,cnbdtLS,cscdtLS);
      to->tot = navIn->asUnsignedDouble(csbtot,cnbtot,csctot);
      to->wnot = navIn->asUnsignedLong(csbWNot,cnbWNot,cscWNot);
      to->wnLSF = navIn->asUnsignedLong(csbWNlsf,cnbWNlsf,cscWNlsf);
      to->dn = navIn->asUnsignedLong(csbDN,cnbDN,cscDN);
      to->deltatLSF = navIn->asLong(csbdtLSF,cnbdtLSF,cscdtLSF);
      // cerr << "add CNAV time offset" << endl;
      navOut.push_back(p0);
      return true;
   }


   bool PNBGPSCNavDataFactory ::
   process35(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      if (!PNBNavDataFactory::processTim)
      {
            // User doesn't want time offset data so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNavTimeOffset>();
      p0->timeStamp = navIn->getTransmitTime();
      p0->signal = NavMessageID(
         NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                               navIn->getobsID(), navIn->getNavID()),
         NavMessageType::TimeOffset);
      GPSCNavTimeOffset *to =
         dynamic_cast<GPSCNavTimeOffset*>(p0.get());
      uint8_t gnssID = navIn->asUnsignedLong(gsbGNSS,gnbGNSS,gscGNSS);
      switch (gnssID)
      {
         case 0:
               // no data available.
            return true;
         case 1:
            to->tgt = TimeSystem::GAL;
            break;
         case 2:
            to->tgt = TimeSystem::GLO;
            break;
         case 3:
               /** @note this is defined in IS-QZSS.  Not sure if we
                * should really be checking to make sure the signal is
                * from a QZSS satellite. */
            to->tgt = TimeSystem::QZS;
            break;
         default:
            // cerr << "Unknown GNSS ID " << gnssID << endl;
               // unknown/unsupported
            return false;
      }
      to->tot = navIn->asUnsignedDouble(gsbt,gnbt,gsct);
      to->wnot = navIn->asUnsignedLong(gsbWN,gnbWN,gscWN);
      to->a0 = navIn->asSignedDouble(gsbA0,gnbA0,gscA0);
      to->a1 = navIn->asSignedDouble(gsbA1,gnbA1,gscA1);
      to->a2 = navIn->asSignedDouble(gsbA2,gnbA2,gscA2);
      // cerr << "add CNAV time offset" << endl;
      navOut.push_back(p0);
      return true;
   }


   void PNBGPSCNavDataFactory ::
   dumpState(std::ostream& s)
      const
   {
      s << "ephAcc.size() = " << ephAcc.size() << " (expect 32)" << endl;
      for (const auto& i : ephAcc)
      {
         s << "  ephAcc[" << i.first << "].size() = " << i.second.size()
           << endl;
         for (unsigned j = 0; j < i.second.size(); j++)
         {
            s << "    ephAcc[" << i.first << "][" << j << "] = ";
            if (!i.second[j])
               s << "unset";
            else
               s << i.second[j]->getNumBits() << " bits";
            s << endl;
         }
      }
   }

} // namespace gpstk
