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
#ifndef GNSSTK_GLOFBITS_HPP
#define GNSSTK_GLOFBITS_HPP

namespace gnsstk
{
   namespace glo
   {
         // This enum is being used to avoid confusion on vector
         // indices.  Using enums instead of constants because it
         // doesn't use any memory that way.  Also not using strongly
         // typed enums because we actually want these values to be
         // implicitly cast to integer values.
      enum SFIndex
      {
         str1 = 0,
         str2 = 1,
         str3 = 2,
         str4 = 3,
         str5 = 4,
         str6 = 0,
         alm1 = str6,
         str7 = 1,
         str8 = 2,
         alm2 = str8,
         str9 = 3,
         str10 = 4,
         alm3 = str10,
         str11 = 5,
         str12 = 6,
         alm4 = str12,
         str13 = 7,
         str14 = 8,
         alm5 = str14,
         str15 = 9
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
            // first bit is always zero

         fsbStrNum = 1,                         ///< String number start bit
         fnbStrNum = 4,
         fscStrNum = 1,
      }; // enum FullBitInfo

         /// Array index, start bits, bit count and scale factor for strings 1-4
         /// If the names of the fields below are terse, it's because
         /// I copied what was in the ICD bit diagram...
         /// @see FullBitInfo for more
      enum EphBitInfo
      {
         esiRev1 = str1,                        ///< Reserved bits 1 start
         esbRev1 = fsbStrNum+fnbStrNum,
         enbRev1 = 2,
         escRev1 = 1,

         esiP1 = str1,                          ///< Adjacent t_b interval flag
         esbP1 = esbRev1+enbRev1,
         enbP1 = 2,
         escP1 = 1,

         esitk = str1,                          ///< Time ref'd to frame start
         esbtk = esbP1+enbP1,
         enbtk = 12,
         esctk = 1,

         esixnp = str1,                         ///< X velocity
         esbxnp = esbtk+enbtk,
         enbxnp = 24,
         escxnp = -20,

         esixnpp = str1,                        ///< X acceleration
         esbxnpp = esbxnp+enbxnp,
         enbxnpp = 5,
         escxnpp = -30,

         esixn = str1,                          ///< X position
         esbxn = esbxnpp+enbxnpp,
         enbxn = 27,
         escxn = -11,

            // KX and MB are in all strings, in the same location
         fsbKX = esbxn+enbxn,                   ///< Hamming code
         fnbKX = 8,
         fscKX = 0,

         fsbMB = fsbKX+fnbKX,                   ///< Time mark
         fnbMB = 11, ///< @todo Is this right?
         fscMB = 0,

            // string 2

         esiBn = str2,                          ///< Health flag
         esbBn = fsbStrNum+fnbStrNum,
         enbBn = 3,
         escBn = 1,

         esiP2 = str2,                          ///< t_b odds or evens.
         esbP2 = esbBn+enbBn,
         enbP2 = 1,
         escP2 = 1,

         esitb = str2,                          ///< Time interval index
         esbtb = esbP2+enbP2,
         enbtb = 7,
         esctb = 15,

         esiRev2 = str2,                        ///< Reserved
         esbRev2 = esbtb+enbtb,
         enbRev2 = 5,
         escRev2 = 1,

         esiynp = str2,                         ///< Y velocity
         esbynp = esbRev2+enbRev2,
         enbynp = 24,
         escynp = -20,

         esiynpp = str2,                        ///< Y acceleration
         esbynpp = esbynp+enbynp,
         enbynpp = 5,
         escynpp = -30,

         esiyn = str2,                          ///< Y position
         esbyn = esbynpp+enbynpp,
         enbyn = 27,
         escyn = -11,

            // string 3

         esiP3 = str3,                          ///< 4 or 5 satellites in alm.
         esbP3 = fsbStrNum+fnbStrNum,
         enbP3 = 1,
         escP3 = 1,

         esiGamman = str3,                      
         esbGamman = esbP3+enbP3,
         enbGamman = 11,
         escGamman = -40,

         esiRev3 = str3,                        ///< Reserved
         esbRev3 = esbGamman+enbGamman,
         enbRev3 = 1,
         escRev3 = 1,

         esiP = str3,                           ///< Satellite operation mode.
         esbP = esbRev3+enbRev3,
         enbP = 2,
         escP = 1,

         esiln = str3,                          
         esbln = esbP+enbP,
         enbln = 1,
         escln = 1,

         esiznp = str3,                         ///< Z velocity
         esbznp = esbln+enbln,
         enbznp = 24,
         escznp = -20,

         esiznpp = str3,                        ///< Z acceleration
         esbznpp = esbznp+enbznp,
         enbznpp = 5,
         escznpp = -30,

         esizn = str3,                          ///< Z position
         esbzn = esbznpp+enbznpp,
         enbzn = 27,
         esczn = -11,

            // string 4

         esitaun = str4,                        ///< Clock bias
         esbtaun = fsbStrNum+fnbStrNum,
         enbtaun = 22,
         esctaun = -30,

         esidtaun = str4,
         esbdtaun = esbtaun+enbtaun,
         enbdtaun = 5,
         escdtaun = -30,

         esiEn = str4,
         esbEn = esbdtaun+enbdtaun,
         enbEn = 5,
         escEn = 1,

         esiRev4 = str4,                        ///< Reserved
         esbRev4 = esbEn+enbEn,
         enbRev4 = 14,
         escRev4 = 1,

         esiP4 = str4,                          ///< Ephemeris available or no
         esbP4 = esbRev4+enbRev4,
         enbP4 = 1,
         escP4 = 1,

         esiFT = str4,
         esbFT = esbP4+enbP4,
         enbFT = 4,
         escFT = 1,

         esiRev5 = str4,                        ///< Reserved
         esbRev5 = esbFT+enbFT,
         enbRev5 = 3,
         escRev5 = 1,

         esiNT = str4,                          ///< Current date w/in 4 years
         esbNT = esbRev5+enbRev5,
         enbNT = 11,
         escNT = 1,

         esin = str4,                           ///< Slot number.
         esbn = esbNT+enbNT,
         enbn = 5,
         escn = 1,

         esiM = str4,                           ///< GLONASS or GLONASS-M
         esbM = esbn+enbn,
         enbM = 2,
         escM = 1,
      }; // enum EphBitInfo


         /// Array index offset, start bits, bit count and scale
         /// factor for strings 6-15.
         /// If the names of the fields below are terse, it's because
         /// I copied what was in the ICD bit diagram...
         /// @note The even numbered strings are identical, and the
         ///   odd numbered strings are identical.  As such, the
         ///   string index enums are offset, e.g alm1+asotau,
         ///   alm1+asoomega, alm2+asotau, etc.
         /// @see FullBitInfo for more
      enum AlmBitInfo
      {
            // even strings

         asoC = 0,
         asbC = fsbStrNum+fnbStrNum,
         anbC = 1,
         ascC = 1,

         asoM = 0,
         asbM = asbC+anbC,
         anbM = 2,
         ascM = 1,

         ason = 0,
         asbn = asbM+anbM,
         anbn = 5,
         ascn = 1,

         asotau = 0,
         asbtau = asbn+anbn,
         anbtau = 10,
         asctau = -18,

         asolambda = 0,
         asblambda = asbtau+anbtau,
         anblambda = 21,
         asclambda = -20,

         asodeltai = 0,
         asbdeltai = asblambda+anblambda,
         anbdeltai = 18,
         ascdeltai = -20,

         asoepsilon = 0,
         asbepsilon = asbdeltai+anbdeltai,
         anbepsilon = 15,
         ascepsilon = -20,

            // odd strings

         asoomega = 1,
         asbomega = fsbStrNum+fnbStrNum,
         anbomega = 16,
         ascomega = -15,

         asot = 1,
         asbt = asbomega+anbomega,
         anbt = 21,
         asct = -5,

         asoDeltaT = 1,
         asbDeltaT = asbt+anbt,
         anbDeltaT = 22,
         ascDeltaT = -9,

         asoDeltaTdot = 1,
         asbDeltaTdot = asbDeltaT+anbDeltaT,
         anbDeltaTdot = 7,
         ascDeltaTdot = -14,

         asoH = 1,
         asbH = asbDeltaTdot+anbDeltaTdot,
         anbH = 5,
         ascH = 1,

         asol = 1,
         asbl = asbH+anbH,
         anbl = 1,
         ascl = 1,
         
      }; // enum AlmBitInfo


         /// Start bits, bit count and scale factor for string 5.
         /// If the names of the fields below are terse, it's because
         /// I copied what was in the ICD bit diagram...
         /// @see FullBitInfo for more
      enum String5BitInfo
      {
         tsbNA = fsbStrNum+fnbStrNum,
         tnbNA = 11,
         tscNA = 1,

         tsbtauc = tsbNA+tnbNA,
         tnbtauc = 28,                          ///< tau_c bits for GLONASS
         tsctauc = -27,                         ///< tau_c scale for GLONASS
         tnbtaucM = 32,                         ///< tau_c bits for GLONASS-M
         tsctaucM = -31,                        ///< tau_c scale for GLONASS-M

         tsbRev6 = tsbtauc+tnbtaucM,
         tnbRev6 = 1,
         tscRev6 = 1,

         tsbN4 = tsbRev6+tnbRev6,
         tnbN4 = 5,
         tscN4 = 1,

         tsbtauGPS = tsbN4+tnbN4,
         tnbtauGPS = 22,
         tsctauGPS = -30,

         tsbln = tsbtauGPS+tnbtauGPS,
         tnbln = 1,
         tscln = 1,
      }; // enum String5BitInfo

         /// Start bits, bit count and scale factor for string 15 (frame 5).
         /// If the names of the fields below are terse, it's because
         /// I copied what was in the ICD bit diagram...
         /// @see FullBitInfo for more
      enum String15BitInfo
      {
         usbB1 = fsbStrNum+fnbStrNum,
         unbB1 = 11,
         uscB1 = -10,

         usbB2 = usbB1+unbB1,
         unbB2 = 10,
         uscB2 = -16,

         usbKP = usbB2+unbB2,
         unbKP = 2,
         uscKP = 1,
      }; // enum String15BitInfo

   } // namespace glo
} //  namespace gnsstk

#endif // GNSSTK_GLOFBITS_HPP
