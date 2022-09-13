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

#ifndef GNSSTK_GALFBITS_HPP
#define GNSSTK_GALFBITS_HPP

namespace gnsstk
{
   namespace galfnav
   {
         /** Start bits, bit counts and scale factor (*n for integer
          * quantities, *2^n for floating point quantities) for fields that
          * apply to all messages. */
      enum FullBitInfo
      {
         fsbType = 0, ///< page type start bit
         fnbType = 6, ///< page type number of bits
         fscType = 1, ///< page type scale factor
      };

         // This enum is only used in PNBGalFNavDataFactory and it's
         // being used to avoid confusion on vector indices.  Using
         // enums instead of constants because it doesn't result in
         // memory allocation.  Also not using strongly typed enums
         // because we actually want these values to be implicitly
         // cast to integer values.
      enum PTIndex
      {
         pt1 = 0,  ///< Array index of page type 1 in ephAcc.
         pt2 = 1,  ///< Array index of page type 2 in ephAcc.
         pt3 = 2,  ///< Array index of page type 3 in ephAcc.
         pt4 = 3,  ///< Array index of page type 4 in ephAcc.
         pt5 = 0,  ///< Array index of page type 5 in almAcc.
         pt6 = 1,  ///< Array index of page type 6 in almAcc.
         numEphPTs = 4, ///< Number of ephemeris page types (array size).
         numAlmPTs = 2  ///< Number of almanac page types (array size).
      };

         /** page type index, start bits, bit counts and scale factor (*n for
          * integer quantities, *2^n for floating point quantities) for each of
          * the ephemeris fields.
          * Bit positions/sizes from Galileo OS-SIS-ICD, Tables 27-30.
          * Scale factors taken from Tables 57, 69, 60, 65 and 66.
          */
      enum EphBitInfo
      {
         esiSVID = pt1,
         esbSVID = fsbType+fnbType,
         enbSVID = 6,
         escSVID = 1,

            // Appears in each of page types 1-4 but for some reason IODnav
            // appears in a different location in page type 1 ONLY.
         esiIOD_1 = pt1,
         esbIOD_1 = esbSVID+enbSVID, ///< IODnav
         enbIOD_1 = 10,
         escIOD_1 = 1,

         esit0c = pt1, ///< t0c subframe index
         esbt0c = esbIOD_1+enbIOD_1, ///< t0c start bit
         enbt0c = 14,  ///< t0c number of bits
         esct0c = 60,  ///< t0c scale factor

         esiaf0 = pt1, ///< af0 subframe index
         esbaf0 = esbt0c+enbt0c, ///< af0 start bit
         enbaf0 = 31,  ///< af0 number of bits
         escaf0 = -34, ///< af0 scale factor

         esiaf1 = pt1, ///< af1 subframe index
         esbaf1 = esbaf0+enbaf0, ///< af1 start bit
         enbaf1 = 21,  ///< af1 number of bits
         escaf1 = -46, ///< af1 scale factor

         esiaf2 = pt1, ///< af2 subframe index
         esbaf2 = esbaf1+enbaf1, ///< af2 start bit
         enbaf2 = 6,   ///< af2 number of bits
         escaf2 = -59, ///< af2 scale factor

         esiSISA = pt1,
         esbSISA = esbaf2+enbaf2,
         enbSISA = 8,
         escSISA = 1,

         esiai0 = pt1,
         esbai0 = esbSISA+enbSISA,
         enbai0 = 11,
         escai0 = -2,

         esiai1 = pt1,
         esbai1 = esbai0+enbai0,
         enbai1 = 11,
         escai1 = -8,

         esiai2 = pt1,
         esbai2 = esbai1+enbai1,
         enbai2 = 14,
         escai2 = -15,

         esiIDFR1 = pt1,
         esbIDFR1 = esbai2+enbai2,
         enbIDFR1 = 1,
         escIDFR1 = 1,

         esiIDFR2 = pt1,
         esbIDFR2 = esbIDFR1+enbIDFR1,
         enbIDFR2 = 1,
         escIDFR2 = 1,

         esiIDFR3 = pt1,
         esbIDFR3 = esbIDFR2+enbIDFR2,
         enbIDFR3 = 1,
         escIDFR3 = 1,

         esiIDFR4 = pt1,
         esbIDFR4 = esbIDFR3+enbIDFR3,
         enbIDFR4 = 1,
         escIDFR4 = 1,

         esiIDFR5 = pt1,
         esbIDFR5 = esbIDFR4+enbIDFR4,
         enbIDFR5 = 1,
         escIDFR5 = 1,

         esiBGDa = pt1,
         esbBGDa = esbIDFR5+enbIDFR5,
         enbBGDa = 10,
         escBGDa = -32,

         esiE5ahs = pt1,
         esbE5ahs = esbBGDa+enbBGDa,
         enbE5ahs = 2,
         escE5ahs = 1,

         esiWN_1 = pt1,
         esbWN_1 = esbE5ahs+enbE5ahs,
         enbWN_1 = 12,
         escWN_1 = 1,

         esiTOW_1 = pt1,
         esbTOW_1 = esbWN_1+enbWN_1,
         enbTOW_1 = 20,
         escTOW_1 = 1,

         esiE5advs = pt1,
         esbE5advs = esbTOW_1+enbTOW_1,
         enbE5advs = 1,
         escE5advs = 1,

            // page type 2

         esiIOD_2 = pt2,
         esbIOD_2 = fsbType+fnbType,
         enbIOD_2 = 10,
         escIOD_2 = 1,

         esiM0  = pt2, ///< M0 subframe index
         esbM0 = esbIOD_2+enbIOD_2, ///< M0 start bit
         enbM0 = 32,   ///< M0 number of bits
         escM0  = -31, ///< M0 scale factor

         esiOMEGAdot = pt2, ///< OMEGAdot subframe index
         esbOMEGAdot = esbM0+enbM0, ///< OMEGAdot start bit
         enbOMEGAdot = 24,  ///< OMEGAdot number of bits
         escOMEGAdot = -43, ///< OMEGAdot scale factor

         esiEcc  = pt2, ///< Ecc subframe index
         esbEcc = esbOMEGAdot+enbOMEGAdot, ///< Ecc start bit
         enbEcc = 32,   ///< Ecc number of bits
         escEcc  = -33, ///< Ecc scale factor

         esiAhalf = pt2, ///< Ahalf subframe index
         esbAhalf = esbEcc+enbEcc, ///< Ahalf start bit
         enbAhalf = 32,   ///< Ahalf number of bits
         escAhalf = -19, ///< Ahalf scale factor

         esiOMEGA0  = pt2, ///< OMEGA0 subframe index
         esbOMEGA0 = esbAhalf+enbAhalf,  ///< OMEGA0 start bit
         enbOMEGA0 = 32,   ///< OMEGA0 number of bits
         escOMEGA0  = -31, ///< OMEGA0 scale factor

         esiidot = pt2, ///< idot subframe index
         esbidot = esbOMEGA0+enbOMEGA0, ///< idot start bit
         enbidot = 14,  ///< idot number of bits
         escidot = -43, ///< idot scale factor

         esiWN_2 = pt2,
         esbWN_2 = esbidot+enbidot,
         enbWN_2 = 12,
         escWN_2 = 1,

         esiTOW_2 = pt2,
         esbTOW_2 = esbWN_2+enbWN_2,
         enbTOW_2 = 20,
         escTOW_2 = 1,

            // page type 3

         esiIOD_3 = pt3,
         esbIOD_3 = fsbType+fnbType,
         enbIOD_3 = 10,
         escIOD_3 = 1,

         esii0  = pt3, ///< i0 subframe index
         esbi0 = esbIOD_3+enbIOD_3, ///< i0 start bit
         enbi0 = 32,   ///< i0 number of bits
         esci0  = -31, ///< i0 scale factor

         esiw  = pt3, ///< w subframe index
         esbw = esbi0+enbi0, ///< w start bit
         enbw = 32,   ///< w number of bits
         escw  = -31, ///< w scale factor

         esidn = pt3, ///< Delta n subframe index
         esbdn = esbw+enbw,  ///< Delta n start bit
         enbdn = 16,  ///< Delta n number of bits
         escdn = -43, ///< Delta n scale factor

         esiCuc = pt3, ///< Cuc subframe index
         esbCuc = esbdn+enbdn, ///< Cuc start bit
         enbCuc = 16,  ///< Cuc number of bits
         escCuc = -29, ///< Cuc scale factor

         esiCus = pt3, ///< Cus subframe index
         esbCus = esbCuc+enbCuc, ///< Cus start bit
         enbCus = 16,  ///< Cus number of bits
         escCus = -29, ///< Cus scale factor

         esiCrc = pt3, ///< Crc subframe index
         esbCrc = esbCus+enbCus, ///< Crc start bit
         enbCrc = 16,  ///< Crc number of bits
         escCrc = -5,  ///< Crc scale factor

         esiCrs = pt3, ///< Crs subframe index
         esbCrs = esbCrc+enbCrc,  ///< Crs start bit
         enbCrs = 16,  ///< Crs number of bits
         escCrs = -5,  ///< Crs scale factor

         esit0e = pt3, ///< t0e subframe index
         esbt0e = esbCrs+enbCrs,  ///< t0e start bit
         enbt0e = 14,  ///< t0e number of bits
         esct0e = 60,  ///< t0e scale factor

         esiWN_3 = pt3,
         esbWN_3 = esbt0e+enbt0e,
         enbWN_3 = 12,
         escWN_3 = 1,

         esiTOW_3 = pt3,
         esbTOW_3 = esbWN_3+enbWN_3,
         enbTOW_3 = 20,
         escTOW_3 = 1,

            // page type 4

         esiIOD_4 = pt4,
         esbIOD_4 = fsbType+fnbType,
         enbIOD_4 = 10,
         escIOD_4 = 1,

         esiCic = pt4, ///< Cic subframe index
         esbCic = esbIOD_4+enbIOD_4,  ///< Cic start bit
         enbCic = 16,  ///< Cic number of bits
         escCic = -29, ///< Cic scale factor

         esiCis = pt4, ///< Cis subframe index
         esbCis = esbCic+enbCic, ///< Cis start bit
         enbCis = 16,  ///< Cis number of bits
         escCis = -29, ///< Cis scale factor

         esiA0 = pt4,
         esbA0 = esbCis+enbCis,
         enbA0 = 32,
         escA0 = -30,

         esiA1 = pt4,
         esbA1 = esbA0+enbA0,
         enbA1 = 24,
         escA1 = -50,

         esidtLS = pt4,
         esbdtLS = esbA1+enbA1,
         enbdtLS = 8,
         escdtLS = 1,

         esitot = pt4,
         esbtot = esbdtLS+enbdtLS,
         enbtot = 8,
         esctot = 3600,

         esiWNot = pt4,
         esbWNot = esbtot+enbtot,
         enbWNot = 8,
         escWNot = 1,

         esiWNlsf = pt4,
         esbWNlsf = esbWNot+enbWNot,
         enbWNlsf = 8,
         escWNlsf = 1,

         esiDN = pt4,
         esbDN = esbWNlsf+enbWNlsf,
         enbDN = 3,
         escDN = 1,

         esidtLSF = pt4,
         esbdtLSF = esbDN+enbDN,
         enbdtLSF = 8,
         escdtLSF = 1,

         esit0G = pt4,
         esbt0G = esbdtLSF+enbdtLSF,
         enbt0G = 8,
         esct0G = 3600,

         esiA0G = pt4,
         esbA0G = esbt0G+enbt0G,
         enbA0G = 16,
         escA0G = -35,

         esiA1G = pt4,
         esbA1G = esbA0G+enbA0G,
         enbA1G = 12,
         escA1G = -51,

         esiWN0G = pt4,
         esbWN0G = esbA1G+enbA1G,
         enbWN0G = 6,
         escWN0G = 1,

         esiTOW_4 = pt4,
         esbTOW_4 = esbWN0G+enbWN0G,
         enbTOW_4 = 20,
         escTOW_4 = 1,
      };

         /** page type index, start bits, bit counts and scale factor (*n for
          * integer quantities, *2^n for floating point quantities) for each of
          * the fields in page types 5-6.
          * Bit positions/sizes from Galileo OS-SIS-ICD, Tables 31-32.
          * Scale factors taken from Table 75.
          */
      enum AlmBitInfo
      {
         asbIODa = fsbType+fnbType, ///< IODa
         anbIODa = 4,
         ascIODa = 1,

         asiWNa = pt5,
         asbWNa = asbIODa+anbIODa,
         anbWNa = 2,
         ascWNa = 1,

         asit0a = pt5,
         asbt0a = asbWNa+anbWNa,
         anbt0a = 10,
         asct0a = 600,

            // SV(SVID1)
            // Number of bits and scale factor are the same for all the SVs
            // so only define those once.

         asiSVID_1 = pt5,
         asbSVID_1 = asbt0a+anbt0a,
         anbSVID = 6,
         ascSVID = 1,

         asidAhalf_1 = pt5,
         asbdAhalf_1 = asbSVID_1+anbSVID,
         anbdAhalf = 13,
         ascdAhalf = -9,

         asiEcc_1 = pt5,
         asbEcc_1 = asbdAhalf_1+anbdAhalf,
         anbEcc = 11,
         ascEcc = -16,

         asiw_1 = pt5,
         asbw_1 = asbEcc_1+anbEcc,
         anbw = 16,
         ascw = -15,

         asidi_1 = pt5,
         asbdi_1 = asbw_1+anbw,
         anbdi = 11,
         ascdi = -14,

         asiOMEGA0_1 = pt5,
         asbOMEGA0_1 = asbdi_1+anbdi,
         anbOMEGA0 = 16,
         ascOMEGA0 = -15,

         asiOMEGAdot_1 = pt5,
         asbOMEGAdot_1 = asbOMEGA0_1+anbOMEGA0,
         anbOMEGAdot = 11,
         ascOMEGAdot = -33,

         asiM0_1 = pt5,
         asbM0_1 = asbOMEGAdot_1+anbOMEGAdot,
         anbM0 = 16,
         ascM0 = -15,

         asiaf0_1 = pt5,
         asbaf0_1 = asbM0_1+anbM0,
         anbaf0 = 16,
         ascaf0 = -19,

         asiaf1_1 = pt5,
         asbaf1_1 = asbaf0_1+anbaf0,
         anbaf1 = 13,
         ascaf1 = -38,

         asiE5ahs_1 = pt5,
         asbE5ahs_1 = asbaf1_1+anbaf1,
         anbE5ahs = 2,
         ascE5ahs = 1,

            // SV(SVID2)

         asiSVID_2 = pt5,
         asbSVID_2 = asbE5ahs_1+anbE5ahs,

         asidAhalf_2 = pt5,
         asbdAhalf_2 = asbSVID_2+anbSVID,

         asiEcc_2 = pt5,
         asbEcc_2 = asbdAhalf_2+anbdAhalf,

         asiw_2 = pt5,
         asbw_2 = asbEcc_2+anbEcc,

         asidi_2 = pt5,
         asbdi_2 = asbw_2+anbw,

         asiOMEGA0m_2 = pt5, /// OMEGA0 MSBs start bit
         asbOMEGA0m_2 = asbdi_2+anbdi,
         anbOMEGA0m_2 = 4,

            // page type 6

         asiOMEGA0l_2 = pt6, /// OMEGA0 LSBs start bit
         asbOMEGA0l_2 = asbIODa+anbIODa,
         anbOMEGA0l_2 = 12,

         asiOMEGAdot_2 = pt6,
         asbOMEGAdot_2 = asbOMEGA0l_2+anbOMEGA0l_2,

         asiM0_2 = pt6,
         asbM0_2 = asbOMEGAdot_2+anbOMEGAdot,

         asiaf0_2 = pt6,
         asbaf0_2 = asbM0_2+anbM0,

         asiaf1_2 = pt6,
         asbaf1_2 = asbaf0_2+anbaf0,

         asiE5ahs_2 = pt6,
         asbE5ahs_2 = asbaf1_2+anbaf1,

            // SV(SVID3)

         asiSVID_3 = pt6,
         asbSVID_3 = asbE5ahs_2+anbE5ahs,

         asidAhalf_3 = pt6,
         asbdAhalf_3 = asbSVID_3+anbSVID,

         asiEcc_3 = pt6,
         asbEcc_3 = asbdAhalf_3+anbdAhalf,

         asiw_3 = pt6,
         asbw_3 = asbEcc_3+anbEcc,

         asidi_3 = pt6,
         asbdi_3 = asbw_3+anbw,

         asiOMEGA0_3 = pt6,
         asbOMEGA0_3 = asbdi_3+anbdi,

         asiOMEGAdot_3 = pt6,
         asbOMEGAdot_3 = asbOMEGA0_3+anbOMEGA0,

         asiM0_3 = pt6,
         asbM0_3 = asbOMEGAdot_3+anbOMEGAdot,

         asiaf0_3 = pt6,
         asbaf0_3 = asbM0_3+anbM0,

         asiaf1_3 = pt6,
         asbaf1_3 = asbaf0_3+anbaf0,

         asiE5ahs_3 = pt6,
         asbE5ahs_3 = asbaf1_3+anbaf1,
      };
   } // namespace galfnav
} // namespace gnsstk

#endif // GNSSTK_GALFBITS_HPP
