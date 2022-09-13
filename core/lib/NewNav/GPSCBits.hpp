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

#ifndef GNSSTK_GPSCBITS_HPP
#define GNSSTK_GPSCBITS_HPP

namespace gnsstk
{
   namespace gpscnav
   {
         // This enum is only used in PNBGPSCNavDataFactory and it's
         // being used to avoid confusion on vector indices.  Using
         // enums instead of constants because it doesn't use any
         // memory that way.  Also not using strongly typed enums
         // because we actually want these values to be implicitly
         // cast to integer values.
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

            /// @note these apply to message type 35 (clock & GGTO)

         isbtop = esbAlert + enbAlert,
         inbtop = 11,
         isctop = 300,

         isbURAned0 = isbtop + inbtop,
         inbURAned0 = 5,
         iscURAned0 = 1,

         isbURAned1 = isbURAned0 + inbURAned0,
         inbURAned1 = 3,
         iscURAned1 = 1,

         isbURAned2 = isbURAned1 + inbURAned1,
         inbURAned2 = 3,
         iscURAned2 = 1,

         isbtoc = isbURAned2 + inbURAned2,
         inbtoc = 11,
         isctoc = 300,

         isbaf0 = isbtoc + inbtoc,
         inbaf0 = 26,
         iscaf0 = -35,

         isbaf1 = isbaf0 + inbaf0,
         inbaf1 = 20,
         iscaf1 = -48,

         isbaf2 = isbaf1 + inbaf1,
         inbaf2 = 10,
         iscaf2 = -60,

         isbTgd = isbaf2 + inbaf2,
         inbTgd = 13,
         iscTgd = -35,

         isbISCL1CA = isbTgd + inbTgd,
         inbISCL1CA = 13,
         iscISCL1CA = -35,

         isbISCL2C = isbISCL1CA + inbISCL1CA,
         inbISCL2C = 13,
         iscISCL2C = -35,

         isbISCL5I5 = isbISCL2C + inbISCL2C,
         inbISCL5I5 = 13,
         iscISCL5I5 = -35,

         isbISCL5Q5 = isbISCL5I5 + inbISCL5I5,
         inbISCL5Q5 = 13,
         iscISCL5Q5 = -35,

         isbAlpha0 = isbISCL5Q5 + inbISCL5Q5,
         inbAlpha0 = 8,
         iscAlpha0 = -30,

         isbAlpha1 = isbAlpha0 + inbAlpha0,
         inbAlpha1 = 8,
         iscAlpha1 = -27,

         isbAlpha2 = isbAlpha1 + inbAlpha1,
         inbAlpha2 = 8,
         iscAlpha2 = -24,

         isbAlpha3 = isbAlpha2 + inbAlpha2,
         inbAlpha3 = 8,
         iscAlpha3 = -24,

         isbBeta0 = isbAlpha3 + inbAlpha3,
         inbBeta0 = 8,
         iscBeta0 = 11,

         isbBeta1 = isbBeta0 + inbBeta0,
         inbBeta1 = 8,
         iscBeta1 = 14,

         isbBeta2 = isbBeta1 + inbBeta1,
         inbBeta2 = 8,
         iscBeta2 = 16,

         isbBeta3 = isbBeta2 + inbBeta2,
         inbBeta3 = 8,
         iscBeta3 = 16,

         isbWNOP = isbBeta3 + inbBeta3,
         inbWNOP = 8,
         iscWNOP = 0
      };

   } // namespace gpscnav
} // namespace gnsstk

#endif // GNSSTK_GPSCBITS_HPP
