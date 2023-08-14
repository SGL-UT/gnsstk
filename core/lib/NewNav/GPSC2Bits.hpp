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

#ifndef GNSSTK_GPSC2BITS_HPP
#define GNSSTK_GPSC2BITS_HPP

namespace gnsstk
{
   namespace gpscnav2
   {
         /** Constants collection: start bits, bit counts and scale factor (*n
          * for integer quantities, *2^n for floating point quantities) for
          * each of the nav message fields.
          * @note When the comment for an esc* enum says "scale factor", the
          *   bits are multiplied by that scalar.  When the comment says "power
          *   factor", the bits are multiplied by 2^n where n is the esc*
          *   value.
          */
      enum CNav2BitInfo
      {
         nnbComplete = 883,     ///< Number of bits in a complete subframe 1/2/3 msg.
         nnbSF1 = 9,            ///< Number of bits in subframe 1 message.
         nnbSF2 = 600,          ///< Number of bits in subframe 2 message.
         nnbSF3 = 274,          ///< Number of bits in subframe 3 message.
         nnbCRC = 24,           ///< Number of bits in the CRC at the end of SF2/3.

         npgUTC = 1,            ///< UTC & Iono page number for subframe 3.
         npgGGTO = 2,           ///< GPS-GNSS time offset page number for subframe 3.
         npgMAlm = 4,           ///< Midi Almanac page number for subframe 3.
         npgSVConfig = 7,       ///< SV configuration page number for subframe 3.

            // these apply to subframe 2

         esbWN = 0,             ///< WN start bit
         enbWN = 13,            ///< WN number of bits
         escWN = 1,             ///< WN scale factor

         esbITOW = 13,          ///< ITOW start bit
         enbITOW = 8,           ///< ITOW number of bits
         escITOW = 1,           ///< ITOW scale factor

         esbtop = 21,           ///< top start bit
         enbtop = 11,           ///< top number of bits
         esctop = 300,          ///< top scale factor

         esbHeaL1C = 32,        ///< L1C Signal health bit

         esbURA = 33,           ///< URA index start bit
         enbURA = 5,            ///< URA index number of bits
         escURA = 1,            ///< URA index scale factor

         esbtoe = 38,           ///< toe start bit
         enbtoe = 11,           ///< toe number of bits
         esctoe = 300,          ///< toe scale factor

         esbdA = 49,            ///< Delta A start bit
         enbdA = 26,            ///< Delta A number of bits
         escdA = -9,            ///< Delta A power factor

         esbAdot = 75,          ///< Adot start bit
         enbAdot = 25,          ///< Adot number of bits
         escAdot = -21,         ///< Adot power factor

         esbdn0 = 100,          ///< Delta n0 start bit
         enbdn0 = 17,           ///< Delta n0 number of bits
         escdn0 = -44,          ///< Delta n0 power factor

         esbdn0dot = 117,       ///< Delta n0 dot start bit
         enbdn0dot = 23,        ///< Delta n0 dot number of bits
         escdn0dot = -57,       ///< Delta n0 dot power factor

         esbM0 = 140,           ///< M0-n start bit
         enbM0 = 33,            ///< M0-n number of bits
         escM0 = -32,           ///< M0-n power factor

         esbEcc = 173,          ///< Ecc n start bit
         enbEcc = 33,           ///< Ecc n number of bits
         escEcc = -34,          ///< Ecc n power factor

         esbw = 206,            ///< w start bit
         enbw = 33,             ///< w number of bits
         escw = -32,            ///< w power factor

         esbOMEGA0 = 239,       ///< OMEGA0-n start bit
         enbOMEGA0 = 33,        ///< OMEGA0-n number of bits
         escOMEGA0 = -32,       ///< OMEGA0-n power factor

         esbi0 = 272,           ///< i0-n start bit
         enbi0 = 33,            ///< i0-n number of bits
         esci0 = -32,           ///< i0-n power factor

         esbdOMEGAdot = 305,    ///< Delta OMEGAdot start bit
         enbdOMEGAdot = 17,     ///< Delta OMEGAdot number of bits
         escdOMEGAdot = -44,    ///< Delta OMEGAdot power factor

         esbidot = 322,         ///< i0-n - DOT start bit
         enbidot = 15,          ///< i0-n - DOT number of bits
         escidot = -44,         ///< i0-n - DOT power factor

         esbCis = 337,          ///< Cis-n start bit
         enbCis = 16,           ///< Cis-n number of bits
         escCis = -30,          ///< Cis-n power factor

         esbCic = 353,          ///< Cic-n start bit
         enbCic = 16,           ///< Cic-n number of bits
         escCic = -30,          ///< Cic-n power factor

         esbCrs = 369,          ///< Crs-n start bit
         enbCrs = 24,           ///< Crs-n number of bits
         escCrs = -8,           ///< Crs-n power factor

         esbCrc = 393,          ///< Crc-n start bit
         enbCrc = 24,           ///< Crc-n number of bits
         escCrc = -8,           ///< Crc-n power factor

         esbCus = 417,          ///< Cus-n start bit
         enbCus = 21,           ///< Cus-n number of bits
         escCus = -30,          ///< Cus-n power factor

         esbCuc = 438,          ///< Cuc-n start bit
         enbCuc = 21,           ///< Cuc-n number of bits
         escCuc = -30,          ///< Cuc-n power factor

         esbURAned0 = 459,      ///< URA_NED0 start bit
         enbURAned0 = 5,        ///< URA_NED0 number of bits
         escURAned0 = 1,        ///< URA_NED0 scale factor

         esbURAned1 = 464,      ///< URA_NED1 start bit
         enbURAned1 = 3,        ///< URA_NED1 number of bits
         escURAned1 = 1,        ///< URA_NED1 scale factor

         esbURAned2 = 467,      ///< URA_NED2 start bit
         enbURAned2 = 3,        ///< URA_NED2 number of bits
         escURAned2 = 1,        ///< URA_NED2 scale factor

         esbaf0 = 470,          ///< af0 start bit
         enbaf0 = 26,           ///< af0 number of bits
         escaf0 = -35,          ///< af0 power factor

         esbaf1 = 496,          ///< af1 start bit
         enbaf1 = 20,           ///< af1 number of bits
         escaf1 = -48,          ///< af1 power factor

         esbaf2 = 516,          ///< af2 start bit
         enbaf2 = 10,           ///< af2 number of bits
         escaf2 = -60,          ///< af2 power factor

         esbTGD = 526,          ///< Tgd start bit
         enbTGD = 13,           ///< Tgd number of bits
         escTGD = -35,          ///< Tgd power factor

         esbISCL1CP = 539,      ///< ISC_L1CP start bit
         enbISCL1CP = 13,       ///< ISC_L1CP number of bits
         escISCL1CP = -35,      ///< ISC_L1CP power factor

         esbISCL1CD = 552,      ///< ISC_L1CD start bit
         enbISCL1CD = 13,       ///< ISC_L1CD number of bits
         escISCL1CD = -35,      ///< ISC_L1CD power factor

         esbISF = 565,          ///< ISF start bit

         esbWNop = 566,         ///< WNop start bit
         enbWNop = 8,           ///< WNop number of bits
         escWNop = 1,           ///< WNop scale factor

            // these apply to subframe 3

         asbPRN = 0,            ///< PRN start bit
         anbPRN = 8,            ///< PRN number of bits
         ascPRN = 1,            ///< PRN scale factor

         asbPage = 8,           ///< Page # start bit
         anbPage = 6,           ///< Page # number of bits
         ascPage = 1,           ///< Page # scale factor

            // these apply to subframe 3 page 1 (UTC & IONO)

         csbA0 = 14,            ///< A0-n start bit
         cnbA0 = 16,            ///< A0-n number of bits
         cscA0 = -35,           ///< A0-n power factor

         csbA1 = 30,            ///< A1-n start bit
         cnbA1 = 13,            ///< A1-n number of bits
         cscA1 = -51,           ///< A1-n power factor

         csbA2 = 43,            ///< A2-n start bit
         cnbA2 = 7,             ///< A2-n number of bits
         cscA2 = -68,           ///< A2-n power factor

         csbdtLS = 50,          ///< dtLS start bit
         cnbdtLS = 8,           ///< dtLS number of bits
         cscdtLS = 1,           ///< dtLS scale factor

         csbtot = 58,           ///< tot start bit
         cnbtot = 16,           ///< tot number of bits
         csctot = 4,            ///< tot power factor

         csbWNot = 74,          ///< WNot start bit
         cnbWNot = 13,          ///< WNot number of bits
         cscWNot = 1,           ///< WNot scale factor

         csbWNlsf = 87,         ///< WNlsf start bit
         cnbWNlsf = 13,         ///< WNlsf number of bits
         cscWNlsf = 1,          ///< WNlsf scale factor

         csbDN = 100,           ///< DN start bit
         cnbDN = 4,             ///< DN number of bits
         cscDN = 1,             ///< DN scale factor

         csbdtLSF = 104,        ///< dtLSF start bit
         cnbdtLSF = 8,          ///< dtLSF number of bits
         cscdtLSF = 1,          ///< dtLSF scale factor

         csbAlpha0 = csbdtLSF + cnbdtLSF,
         cnbAlpha0 = 8,
         cscAlpha0 = -30,

         csbAlpha1 = csbAlpha0 + cnbAlpha0,
         cnbAlpha1 = 8,
         cscAlpha1 = -27,

         csbAlpha2 = csbAlpha1 + cnbAlpha1,
         cnbAlpha2 = 8,
         cscAlpha2 = -24,

         csbAlpha3 = csbAlpha2 + cnbAlpha2,
         cnbAlpha3 = 8,
         cscAlpha3 = -24,

         csbBeta0 = csbAlpha3 + cnbAlpha3,
         cnbBeta0 = 8,
         cscBeta0 = 11,

         csbBeta1 = csbBeta0 + cnbBeta0,
         cnbBeta1 = 8,
         cscBeta1 = 14,

         csbBeta2 = csbBeta1 + cnbBeta1,
         cnbBeta2 = 8,
         cscBeta2 = 16,

         csbBeta3 = csbBeta2 + cnbBeta2,
         cnbBeta3 = 8,
         cscBeta3 = 16,

         csbISCL1CA = csbBeta3 + cnbBeta3,
         cnbISCL1CA = 13,
         cscISCL1CA = -35,

         csbISCL2C = csbISCL1CA + cnbISCL1CA,
         cnbISCL2C = 13,
         cscISCL2C = -35,

         csbISCL5I5 = csbISCL2C + cnbISCL2C,
         cnbISCL5I5 = 13,
         cscISCL5I5 = -35,

         csbISCL5Q5 = csbISCL5I5 + cnbISCL5I5,
         cnbISCL5Q5 = 13,
         cscISCL5Q5 = -35,

            // these apply to subframe 3 page 2 (GGTO & EOP)

         gsbGNSS = 14,          ///< GNSS ID start bit
         gnbGNSS = 3,           ///< GNSS ID number of bits
         gscGNSS = 1,           ///< GNSS ID scale factor

         gsbt = 17,             ///< tggto start bit
         gnbt = 16,             ///< tggto number of bits
         gsct = 4,              ///< tggto power factor

         gsbWN = 33,            ///< WNggto start bit
         gnbWN = 13,            ///< WNggto number of bits
         gscWN = 1,             ///< WNggto scale factor

         gsbA0 = 46,            ///< A0-ggto start bit
         gnbA0 = 16,            ///< A0-ggto number of bits
         gscA0 = -35,           ///< A0-ggto power factor

         gsbA1 = 62,            ///< A1-ggto start bit
         gnbA1 = 13,            ///< A1-ggto number of bits
         gscA1 = -51,           ///< A1-ggto power factor

         gsbA2 = 75,            ///< A2-ggto start bit
         gnbA2 = 7,             ///< A2-ggto number of bits
         gscA2 = -68,           ///< A2-ggto power factor

            // these apply to subframe 3 page 4 (midi almanac)

         asbWNa = 14,           ///< WNa-n start bit
         anbWNa = 13,           ///< WNa-n number of bits
         ascWNa = 1,            ///< WNa-n scale factor

         asbtoa = 27,           ///< toa start bit
         anbtoa = 8,            ///< toa number of bits
         asctoa = 12,           ///< toa power factor

         asbPRNa = 35,          ///< PRNa start bit
         anbPRNa = 8,           ///< PRNa number of bits
         ascPRNa = 1,           ///< PRNa scale factor

         asbHeaL1 = 43,         ///< L1 Signal health bit
         asbHeaL2 = 44,         ///< L2 Signal health bit
         asbHeaL5 = 45,         ///< L5 Signal health bit

         asbEcc = 46,           ///< Ecc n start bit
         anbEcc = 11,           ///< Ecc n number of bits
         ascEcc = -16,          ///< Ecc n power factor

         asbdi = 57,            ///< delta i start bit
         anbdi = 11,            ///< delta i n number of bits
         ascdi = -14,           ///< delta i n power factor

         asbOMEGAdot = 68,      ///< OMEGAdot start bit
         anbOMEGAdot = 11,      ///< OMEGAdot number of bits
         ascOMEGAdot = -33,     ///< OMEGAdot power factor

         asbAhalf = 79,         ///< Ahalf MSBs start bit
         anbAhalf = 17,         ///< Ahalf MSBs number of bits
         ascAhalf = -4,         ///< Ahalf scale factor

         asbOMEGA0 = 96,        ///< OMEGA0 MSBs start bit
         anbOMEGA0 = 16,        ///< OMEGA0 MSBs number of bits
         ascOMEGA0 = -15,       ///< OMEGA0 power factor

         asbw = 112,            ///< w start bit
         anbw = 16,             ///< w number of bits
         ascw = -15,            ///< w power factor

         asbM0 = 128,           ///< M0 start bit
         anbM0 = 16,            ///< M0 number of bits
         ascM0 = -15,           ///< M0 power factor

         asbaf0 = 144,          ///< af0 start bit
         anbaf0 = 11,           ///< af0 number of bits
         ascaf0 = -20,          ///< af0 power factor

         asbaf1 = 155,          ///< af1 start bit
         anbaf1 = 10,           ///< af1 number of bits
         ascaf1 = -37,          ///< af1 power factor

            // These apply to subframe 3 page 7 (SV configuration)

         asbConf1 = 14,         ///< 1st SV config start bit
         anbConfLen = 4,        ///< Length of each SV config entry, in bits
         numSVConfs = 63,       ///< Number of SV config entries
      };
   } // namespace gpscnav2
} // namespace gnsstk

#endif // GPSTK_GPSC2BITS_HPP
