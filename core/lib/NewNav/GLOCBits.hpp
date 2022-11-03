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
#ifndef GNSSTK_GLOCBITS_HPP
#define GNSSTK_GLOCBITS_HPP

namespace gnsstk
{
   namespace gloc
   {
         // This enum is being used to avoid confusion on vector
         // indices.  Using enums instead of constants because it
         // doesn't use any memory that way.  Also not using strongly
         // typed enums because we actually want these values to be
         // implicitly cast to integer values.
      enum SFIndex
      {
         firstEphString = 10,
         str10 = 0,
         str11 = 1,
         str12 = 2,
      };

         /** Start bits, bit counts and scale factor (*n for integer
          * quantities, *2^n for floating point quantities) for fields that
          * apply to all messages.
          *
          * Naming convention: 
          * XPPNNNNNNNNNN
          *
          * Where X = some relevant 1- or maybe 2-character prefix
          * (e=ephemeris, a=almanac, etc.)
          *
          * PP is a 2-character quantity identification (sb=start bit,
          * nb=number of bits, sc=scale, si=string index)
          *
          * NNNNNNNNNN is a field name of arbitrary length (though
          * typically kept short) */
      enum FullBitInfo
      {
         fsbPreamble = 0,                       ///< Preamble start bit
         fnbPreamble = 20,
         fscPreamble = 1,
         valPreamble = 0x494e,

         fsbType = fsbPreamble+fnbPreamble,     ///< Message type ID start bit
         fnbType = 6,
         fscType = 1,

         fsbTS = fsbType+fnbType,               ///< Time Stamp start bit
         fnbTS = 15,
         fscTS = 1,

         fsbj = fsbTS+fnbTS,                    ///< SV ID start bit
         fnbj = 6,
         fscj = 1,

         fsbHj = fsbj+fnbj,                     ///< SV health start bit
         fnbHj = 1,
         fscHj = 1,

         fsblj = fsbHj+fnbHj,                   ///< data validity start bit
         fnblj = 1,
         fsclj = 1,

         fsbP1 = fsblj+fnblj,
         fnbP1 = 4,
         fscP1 = 1,

         fsbP2 = fsbP1+fnbP1,
         fnbP2 = 1,
         fscP2 = 1,

         fsbKP = fsbP2+fnbP2,                   ///< UTC planned corr start bit
         fnbKP = 2,
         fscKP = 1,

         fsbA = fsbKP+fnbKP,                    ///< Time corr planned start bit
         fnbA = 1,
         fscA = 1,

         fsbCRC = 276,                          ///< CRC start bit
         fnbCRC = 24,
         fscCRC = 1,
      }; // enum FullBitInfo

         /// Array index, start bits, bit count and scale factor for
         /// string types 10-12.
         /// If the names of the fields below are terse, it's because
         /// I copied what was in the ICD bit diagram...
         /// @see FullBitInfo for more
      enum EphBitInfo
      {
            // string 10
         esiN4 = str10,
         esbN4 = fsbA+fnbA,                     ///<
         enbN4 = 5,
         escN4 = 1,

         esiNT = str10,
         esbNT = esbN4+enbN4,
         enbNT = 11,
         escNT = 1,

         esiMj = str10,
         esbMj = esbNT+enbNT,
         enbMj = 3,
         escMj = 1,

         esiPS = str10,
         esbPS = esbMj+enbMj,
         enbPS = 6,
         escPS = 1,

         esitb = str10,
         esbtb = esbPS+enbPS,
         enbtb = 10,
         esctb = 90,

         esiEjE = str10,
         esbEjE = esbtb+enbtb,
         enbEjE = 8,
         escEjE = 1,

         esiEjT = str10,
         esbEjT = esbEjE+enbEjE,
         enbEjT = 8,
         escEjT = 1,

         esiRjE = str10,
         esbRjE = esbEjT+enbEjT,
         enbRjE = 2,
         escRjE = 1,

         esiRjT = str10,
         esbRjT = esbRjE+enbRjE,
         enbRjT = 2,
         escRjT = 1,

         esiFjE = str10,
         esbFjE = esbRjT+enbRjT,
         enbFjE = 5,
         escFjE = 1,

         esiFjT = str10,
         esbFjT = esbFjE+enbFjE,
         enbFjT = 5,
         escFjT = 1,

         esitauj = str10,
         esbtauj = esbFjT+enbFjT,
         enbtauj = 32,
         esctauj = -38,

         esigammaj = str10,
         esbgammaj = esbtauj+enbtauj,
         enbgammaj = 19,
         escgammaj = -48,

         esiBetaj = str10,
         esbBetaj = esbgammaj+enbgammaj,
         enbBetaj = 15,
         escBetaj = -57,

         esitauc = str10,
         esbtauc = esbBetaj+enbBetaj,
         enbtauc = 40,
         esctauc = -31,

         esitaucdot = str10,
         esbtaucdot = esbtauc+enbtauc,
         enbtaucdot = 13,
         esctaucdot = -49,

         esiRev1 = str10,
         esbRev1 = esbtaucdot+enbtaucdot,
         enbRev1 = 35,
         escRev1 = 1,

            // string 11
         esixj = str11,
         esbxj = fsbA+fnbA,
         enbxj = 40,
         escxj = -20,

         esiyj = str11,
         esbyj = esbxj+enbxj,
         enbyj = 40,
         escyj = -20,

         esizj = str11,
         esbzj = esbyj+enbyj,
         enbzj = 40,
         esczj = -20,

         esixjp = str11,
         esbxjp = esbzj+enbzj,
         enbxjp = 35,
         escxjp = -30,

         esiyjp = str11,
         esbyjp = esbxjp+enbxjp,
         enbyjp = 35,
         escyjp = -30,

         esiRev2 = str11,
         esbRev2 = esbyjp+enbyjp,
         enbRev2 = 29,
         escRev2 = 1,

            // string 12
         esizjp = str12,
         esbzjp = fsbA+fnbA,
         enbzjp = 35,
         esczjp = -30,

         esixjpp = str12,
         esbxjpp = esbzjp+enbzjp,
         enbxjpp = 15,
         escxjpp = -39,

         esiyjpp = str12,
         esbyjpp = esbxjpp+enbxjpp,
         enbyjpp = 15,
         escyjpp = -39,

         esizjpp = str12,
         esbzjpp = esbyjpp+enbyjpp,
         enbzjpp = 15,
         esczjpp = -39,

         esiDeltaxjpc = str12,
         esbDeltaxjpc = esbzjpp+enbzjpp,
         enbDeltaxjpc = 13,
         escDeltaxjpc = -10,

         esiDeltayjpc = str12,
         esbDeltayjpc = esbDeltaxjpc+enbDeltaxjpc,
         enbDeltayjpc = 13,
         escDeltayjpc = -10,

         esiDeltazjpc = str12,
         esbDeltazjpc = esbDeltayjpc+enbDeltayjpc,
         enbDeltazjpc = 13,
         escDeltazjpc = -10,

         esiDeltataujL3 = str12,
         esbDeltataujL3 = esbDeltazjpc+enbDeltazjpc,
         enbDeltataujL3 = 18,
         escDeltataujL3 = -38,

         esiTauGPS = str12,
         esbTauGPS = esbDeltataujL3+enbDeltataujL3,
         enbTauGPS = 30,
         escTauGPS = -38,

         esiRev3 = str12,
         esbRev3 = esbTauGPS+enbTauGPS,
         enbRev3 = 52,
         escRev3 = 1,
      }; // EphBitInfo

         /// Start bits, bit count and scale factor for
         /// string type 20.
         /// If the names of the fields below are terse, it's because
         /// I copied what was in the ICD bit diagram...
         /// @see FullBitInfo for more
      enum AlmBitInfo
      {
         asbTO = fsbA+fnbA,                     ///< Orbit type
         anbTO = 2,
         ascTO = 1,

         asbNS = asbTO+anbTO,                   ///< Number of SVs in almanac
         anbNS = 6,
         ascNS = 1,

         asbE = asbNS+anbNS,                    ///< Age of almanac
         anbE = 6,
         ascE = 1,

         asbNA = asbE+anbE,                     ///< Days from leap year
         anbNA = 11,
         ascNA = 1,

         asbSR = asbNA+anbNA,                   ///< Status register
         anbSR = 5,
         ascSR = 1,

         asbM = asbSR+anbSR,                    ///< Sat type and signal
         anbM = 3,
         ascM = 1,

         asbtau = asbM+anbM,                    ///< rough time correction
         anbtau = 14,
         asctau = -20,

         asblambda = asbtau+anbtau,             ///< 
         anblambda = 21,
         asclambda = -20,

         asbtlambda = asblambda+anblambda,
         anbtlambda = 21,
         asctlambda = -5,

         asbDeltai = asbtlambda+anbtlambda,
         anbDeltai = 15,
         ascDeltai = -20,

         asbepsilon = asbDeltai+anbDeltai,
         anbepsilon = 15,
         ascepsilon = -20,

         asbomega = asbepsilon+anbepsilon,
         anbomega = 16,
         ascomega = -15,

         asbDeltaT = asbomega+anbomega,
         anbDeltaT = 19,
         ascDeltaT = -9,

         asbDeltaTdot = asbDeltaT+anbDeltaT,
         anbDeltaTdot = 7,
         ascDeltaTdot = -14,
      }; // enum AlmBitInfo

         /// Start bits, bit count and scale factor for
         /// string type 25.
         /// If the names of the fields below are terse, it's because
         /// I copied what was in the ICD bit diagram...
         /// @see FullBitInfo for more
      enum EarthBitInfo
      {
         isbNB = fsbA+fnbA,
         inbNB = 11,
         iscNB = 1,

         isbxp = isbNB+inbNB,
         inbxp = 16,
         iscxp = -14,

         isbyp = isbxp+inbxp,
         inbyp = 16,
         iscyp = -14,

         isbxpp = isbyp+inbyp,
         inbxpp = 9,
         iscxpp = -14,

         isbypp = isbxpp+inbxpp,
         inbypp = 9,
         iscypp = -14,

         isbxppp = isbypp+inbypp,
         inbxppp = 7,
         iscxppp = -14,

         isbyppp = isbxppp+inbxppp,
         inbyppp = 7,
         iscyppp = -14,

         isbB0 = isbyppp+inbyppp,
         inbB0 = 25,
         iscB0 = -16,

         isbB1 = isbB0+inbB0,
         inbB1 = 10,
         iscB1 = -16,

         isbB2 = isbB1+inbB1,
         inbB2 = 8,
         iscB2 = -16,

         isbcA = isbB2+inbB2,
         inbcA = 9,
         isccA = -7,

         isbcF107 = isbcA+inbcA,
         inbcF107 = 13,
         isccF107 = -4,

         isbcAp = isbcF107+inbcF107,
         inbcAp = 9,
         isccAp = 1,

         isbUTCTAI = isbcAp+inbcAp,
         inbUTCTAI = 9,
         iscUTCTAI = 1,
      };

         /// Start bits, bit count and scale factor for
         /// string types 31 and 32.
         /// If the names of the fields below are terse, it's because
         /// I copied what was in the ICD bit diagram...
         /// @see FullBitInfo for more
      enum LTDMPBitInfo
      {
            // this is present and in the same location in 31 and 32.
         lsbtb = fsbA+fnbA,
         lnbtb = 10,
         lsctb = 90,

         lsbdax0 = lsbtb+lnbtb,
         lnbdax0 = 5,
         lscdax0 = -42,

         lsbday0 = lsbdax0+lnbdax0,
         lnbday0 = 5,
         lscday0 = -42,

         lsbdaz0 = lsbday0+lnbday0,
         lnbdaz0 = 5,
         lscdaz0 = -42,

         lsbax1 = lsbdaz0+lnbdaz0,
         lnbax1 = 18,
         lscax1 = -54,

         lsbay1 = lsbax1+lnbax1,
         lnbay1 = 18,
         lscay1 = -54,

         lsbaz1 = lsbay1+lnbay1,
         lnbaz1 = 18,
         lscaz1 = -54,

         lsbax2 = lsbaz1+lnbaz1,
         lnbax2 = 18,
         lscax2 = -67,

         lsbay2 = lsbax2+lnbax2,
         lnbay2 = 18,
         lscay2 = -67,

         lsbaz2 = lsbay2+lnbay2,
         lnbaz2 = 18,
         lscaz2 = -67,

         lsbax3 = lsbtb+lnbtb,
         lnbax3 = 18,
         lscax3 = -80,

         lsbay3 = lsbax3+lnbax3,
         lnbay3 = 18,
         lscay3 = -80,

         lsbaz3 = lsbay3+lnbay3,
         lnbaz3 = 18,
         lscaz3 = -80,

         lsbax4 = lsbaz3+lnbaz3,
         lnbax4 = 18,
         lscax4 = -95,

         lsbay4 = lsbax4+lnbax4,
         lnbay4 = 18,
         lscay4 = -95,

         lsbaz4 = lsbay4+lnbay4,
         lnbaz4 = 18,
         lscaz4 = -95,
      };
   } // namespace glo
} // namespace gnsstk

#endif // GNSSTK_GLOCBITS_HPP
