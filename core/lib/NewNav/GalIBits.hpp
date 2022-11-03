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

#ifndef GNSSTK_GALIBITS_HPP
#define GNSSTK_GALIBITS_HPP

namespace gnsstk
{
   namespace galinav
   {
         /** Start bits, bit counts and scale factor (*n for integer
          * quantities, *2^n for floating point quantities) for fields that
          * apply to all messages. */
      enum FullBitInfo
      {
         fsbType = 0, ///< Word type start bit
         fnbType = 6, ///< Word type number of bits
         fscType = 1, ///< Word type scale factor
      };

         // This enum is only used here and it's being used to avoid confusion
         // on vector indices.  Using enums instead of constants because it
         // doesn't result in memory allocation.  Also not using strongly typed
         // enums because we actually want these values to be implicitly cast
         // to integer values.
      enum WTIndex
      {
         wt1 = 0,  ///< Array index of word type 1 in ephAcc.
         wt2 = 1,  ///< Array index of word type 2 in ephAcc.
         wt3 = 2,  ///< Array index of word type 3 in ephAcc.
         wt4 = 3,  ///< Array index of word type 4 in ephAcc.
         wt5 = 4,  ///< Array index of word type 5 in ephAcc.
         wt7 = 0,  ///< Array index of word type 7 in almAcc.
         wt8 = 1,  ///< Array index of word type 8 in almAcc.
         wt9 = 2,  ///< Array index of word type 9 in almAcc.
         wt10 = 3, ///< Array index of word type 10 in almAcc.
         numEphWTs = 5, ///< Number of ephemeris word types (array size).
         numAlmWTs = 4  ///< Number of almanac word types (array size).
      };

         /** Word type index, start bits, bit counts and scale factor (*n for
          * integer quantities, *2^n for floating point quantities) for each of
          * the ephemeris fields.
          * Bit positions/sizes from Galileo OS-SIS-ICD, Tables 39-42.
          * Scale factors taken from Table 57.
          */
      enum EphBitInfo
      {
         esbIOD = fsbType+fnbType, ///< IODnav
         enbIOD = 10,
         escIOD = 1,

         esit0e = wt1, ///< t0e subframe index
         esbt0e = esbIOD+enbIOD,  ///< t0e start bit
         enbt0e = 14,  ///< t0e number of bits
         esct0e = 60,  ///< t0e scale factor

         esiM0  = wt1, ///< M0 subframe index
         esbM0 = esbt0e+enbt0e, ///< M0 start bit
         enbM0 = 32,   ///< M0 number of bits
         escM0  = -31, ///< M0 scale factor

         esiEcc  = wt1, ///< Ecc subframe index
         esbEcc = esbM0+enbM0, ///< Ecc start bit
         enbEcc = 32,   ///< Ecc number of bits
         escEcc  = -33, ///< Ecc scale factor

         esiAhalf = wt1, ///< Ahalf subframe index
         esbAhalf = esbEcc+enbEcc, ///< Ahalf start bit
         enbAhalf = 32,   ///< Ahalf number of bits
         escAhalf = -19, ///< Ahalf scale factor

         esiOMEGA0  = wt2, ///< OMEGA0 subframe index
         esbOMEGA0 = esbIOD+enbIOD,  ///< OMEGA0 start bit
         enbOMEGA0 = 32,   ///< OMEGA0 number of bits
         escOMEGA0  = -31, ///< OMEGA0 scale factor

         esii0  = wt2, ///< i0 subframe index
         esbi0 = esbOMEGA0+enbOMEGA0, ///< i0 start bit
         enbi0 = 32,   ///< i0 number of bits
         esci0  = -31, ///< i0 scale factor

         esiw  = wt2, ///< w subframe index
         esbw = esbi0+enbi0, ///< w start bit
         enbw = 32,   ///< w number of bits
         escw  = -31, ///< w scale factor

         esiidot = wt2, ///< idot subframe index
         esbidot = esbw+enbw, ///< idot start bit
         enbidot = 14,  ///< idot number of bits
         escidot = -43, ///< idot scale factor

         esiOMEGAdot = wt3, ///< OMEGAdot subframe index
         esbOMEGAdot = esbIOD+enbIOD, ///< OMEGAdot start bit
         enbOMEGAdot = 24,  ///< OMEGAdot number of bits
         escOMEGAdot = -43, ///< OMEGAdot scale factor

         esidn = wt3, ///< Delta n subframe index
         esbdn = esbOMEGAdot+enbOMEGAdot,  ///< Delta n start bit
         enbdn = 16,  ///< Delta n number of bits
         escdn = -43, ///< Delta n scale factor

         esiCuc = wt3, ///< Cuc subframe index
         esbCuc = esbdn+enbdn, ///< Cuc start bit
         enbCuc = 16,  ///< Cuc number of bits
         escCuc = -29, ///< Cuc scale factor

         esiCus = wt3, ///< Cus subframe index
         esbCus = esbCuc+enbCuc, ///< Cus start bit
         enbCus = 16,  ///< Cus number of bits
         escCus = -29, ///< Cus scale factor

         esiCrc = wt3, ///< Crc subframe index
         esbCrc = esbCus+enbCus, ///< Crc start bit
         enbCrc = 16,  ///< Crc number of bits
         escCrc = -5,  ///< Crc scale factor

         esiCrs = wt3, ///< Crs subframe index
         esbCrs = esbCrc+enbCrc,  ///< Crs start bit
         enbCrs = 16,  ///< Crs number of bits
         escCrs = -5,  ///< Crs scale factor

         esiSISA = wt3,
         esbSISA = esbCrs+enbCrs,
         enbSISA = 8,
         escSISA = 1,

         esiSVID = wt4,
         esbSVID = esbIOD+enbIOD,
         enbSVID = 6,
         escSVID = 1,

         esiCic = wt4, ///< Cic subframe index
         esbCic = esbSVID+enbSVID,  ///< Cic start bit
         enbCic = 16,  ///< Cic number of bits
         escCic = -29, ///< Cic scale factor

         esiCis = wt4, ///< Cis subframe index
         esbCis = esbCic+enbCic, ///< Cis start bit
         enbCis = 16,  ///< Cis number of bits
         escCis = -29, ///< Cis scale factor

         esit0c = wt4, ///< t0c subframe index
         esbt0c = esbCis+enbCis, ///< t0c start bit
         enbt0c = 14,  ///< t0c number of bits
         esct0c = 60,   ///< t0c scale factor

         esiaf0 = wt4, ///< af0 subframe index
         esbaf0 = esbt0c+enbt0c, ///< af0 start bit
         enbaf0 = 31,  ///< af0 number of bits
         escaf0 = -34, ///< af0 scale factor

         esiaf1 = wt4, ///< af1 subframe index
         esbaf1 = esbaf0+enbaf0, ///< af1 start bit
         enbaf1 = 21,  ///< af1 number of bits
         escaf1 = -46, ///< af1 scale factor

         esiaf2 = wt4, ///< af2 subframe index
         esbaf2 = esbaf1+enbaf1, ///< af2 start bit
         enbaf2 = 6,   ///< af2 number of bits
         escaf2 = -59, ///< af2 scale factor
      };

         /** Word type index, start bits, bit counts and scale factor (*n for
          * integer quantities, *2^n for floating point quantities) for each of
          * the fields in word type 5.
          * Bit positions/sizes from Galileo OS-SIS-ICD, Tables 43-44.
          * Scale factors taken from Tables 59, 60 and 62.
          */
      enum IonoBitInfo
      {
         isiai0 = wt5,
         isbai0 = fsbType+fnbType,
         inbai0 = 11,
         iscai0 = -2,

         isiai1 = wt5,
         isbai1 = isbai0+inbai0,
         inbai1 = 11,
         iscai1 = -8,

         isiai2 = wt5,
         isbai2 = isbai1+inbai1,
         inbai2 = 14,
         iscai2 = -15,

         isiIDFR1 = wt5,
         isbIDFR1 = isbai2+inbai2,
         inbIDFR1 = 1,
         iscIDFR1 = 1,

         isiIDFR2 = wt5,
         isbIDFR2 = isbIDFR1+inbIDFR1,
         inbIDFR2 = 1,
         iscIDFR2 = 1,

         isiIDFR3 = wt5,
         isbIDFR3 = isbIDFR2+inbIDFR2,
         inbIDFR3 = 1,
         iscIDFR3 = 1,

         isiIDFR4 = wt5,
         isbIDFR4 = isbIDFR3+inbIDFR3,
         inbIDFR4 = 1,
         iscIDFR4 = 1,

         isiIDFR5 = wt5,
         isbIDFR5 = isbIDFR4+inbIDFR4,
         inbIDFR5 = 1,
         iscIDFR5 = 1,

         isiBGDa = wt5,
         isbBGDa = isbIDFR5+inbIDFR5,
         inbBGDa = 10,
         iscBGDa = -32,

         isiBGDb = wt5,
         isbBGDb = isbBGDa+inbBGDa,
         inbBGDb = 10,
         iscBGDb = -32,

         isiE5bhs = wt5,
         isbE5bhs = isbBGDb+inbBGDb,
         inbE5bhs = 2,
         iscE5bhs = 1,

         isiE1Bhs = wt5,
         isbE1Bhs = isbE5bhs+inbE5bhs,
         inbE1Bhs = 2,
         iscE1Bhs = 1,

         isiE5bdvs = wt5,
         isbE5bdvs = isbE1Bhs+inbE1Bhs,
         inbE5bdvs = 1,
         iscE5bdvs = 1,

         isiE1Bdvs = wt5,
         isbE1Bdvs = isbE5bdvs+inbE5bdvs,
         inbE1Bdvs = 1,
         iscE1Bdvs = 1,

         isiWN = wt5,
         isbWN = isbE1Bdvs+inbE1Bdvs,
         inbWN = 12,
         iscWN = 1,

         isiTOW = wt5,
         isbTOW = isbWN+inbWN,
         inbTOW = 20,
         iscTOW = 1,
      };

         /** Word type index, start bits, bit counts and scale factor (*n for
          * integer quantities, *2^n for floating point quantities) for each of
          * the fields in word type 6.
          * Bit positions/sizes from Galileo OS-SIS-ICD, Tables 43-44.
          * Scale factors taken from Table 65.
          */
      enum ClockBitInfo
      {
         csbA0 = fsbType+fnbType,
         cnbA0 = 32,
         cscA0 = -30,

         csbA1 = csbA0+cnbA0,
         cnbA1 = 24,
         cscA1 = -50,

         csbdtLS = csbA1+cnbA1,
         cnbdtLS = 8,
         cscdtLS = 1,

         csbtot = csbdtLS+cnbdtLS,
         cnbtot = 8,
         csctot = 3600,

         csbWNot = csbtot+cnbtot,
         cnbWNot = 8,
         cscWNot = 1,

         csbWNlsf = csbWNot+cnbWNot,
         cnbWNlsf = 8,
         cscWNlsf = 1,

         csbDN = csbWNlsf+cnbWNlsf,
         cnbDN = 3,
         cscDN = 1,

         csbdtLSF = csbDN+cnbDN,
         cnbdtLSF = 8,
         cscdtLSF = 1,

         csbTOW = csbdtLSF+cnbdtLSF,
         cnbTOW = 20,
         cscTOW = 1,
      };

         /** Word type index, start bits, bit counts and scale factor (*n for
          * integer quantities, *2^n for floating point quantities) for each of
          * the fields in word types 7-10.
          * Bit positions/sizes from Galileo OS-SIS-ICD, Tables 43-44.
          * Scale factors taken from Tables 66 and 75.
          */
      enum AlmBitInfo
      {
         asbIODa = fsbType+fnbType, ///< IODa
         anbIODa = 4,
         ascIODa = 1,

         asiWNa_A = wt7,
         asiWNa_B = wt9,
         asbWNa = asbIODa+anbIODa,
         anbWNa = 2,
         ascWNa = 1,

         asit0a_A = wt7,
         asit0a_B = wt9,
         asbt0a = asbWNa+anbWNa,
         anbt0a = 10,
         asct0a = 600,

            // SV(SVID1)
            // Number of bits and scale factor are the same for all the SVs
            // so only define those once.

         asiSVID_1 = wt7,
         asbSVID_1 = asbt0a+anbt0a,
         anbSVID = 6,
         ascSVID = 1,

         asidAhalf_1 = wt7,
         asbdAhalf_1 = asbSVID_1+anbSVID,
         anbdAhalf = 13,
         ascdAhalf = -9,

         asiEcc_1 = wt7,
         asbEcc_1 = asbdAhalf_1+anbdAhalf,
         anbEcc = 11,
         ascEcc = -16,

         asiw_1 = wt7,
         asbw_1 = asbEcc_1+anbEcc,
         anbw = 16,
         ascw = -15,

         asidi_1 = wt7,
         asbdi_1 = asbw_1+anbw,
         anbdi = 11,
         ascdi = -14,

         asiOMEGA0_1 = wt7,
         asbOMEGA0_1 = asbdi_1+anbdi,
         anbOMEGA0 = 16,
         ascOMEGA0 = -15,

         asiOMEGAdot_1 = wt7,
         asbOMEGAdot_1 = asbOMEGA0_1+anbOMEGA0,
         anbOMEGAdot = 11,
         ascOMEGAdot = -33,

         asiM0_1 = wt7,
         asbM0_1 = asbOMEGAdot_1+anbOMEGAdot,
         anbM0 = 16,
         ascM0 = -15,

         asiaf0_1 = wt8,
         asbaf0_1 = asbIODa+anbIODa,
         anbaf0 = 16,
         ascaf0 = -19,

         asiaf1_1 = wt8,
         asbaf1_1 = asbaf0_1+anbaf0,
         anbaf1 = 13,
         ascaf1 = -38,

         asiE5bhs_1 = wt8,
         asbE5bhs_1 = asbaf1_1+anbaf1,
         anbE5bhs = 2,
         ascE5bhs = 1,

         asiE1Bhs_1 = wt8,
         asbE1Bhs_1 = asbE5bhs_1+anbE5bhs,
         anbE1Bhs = 2,
         ascE1Bhs = 1,

            // SV(SVID2)

         asiSVID_2 = wt8,
         asbSVID_2 = asbE1Bhs_1+anbE1Bhs,

         asidAhalf_2 = wt8,
         asbdAhalf_2 = asbSVID_2+anbSVID,

         asiEcc_2 = wt8,
         asbEcc_2 = asbdAhalf_2+anbdAhalf,

         asiw_2 = wt8,
         asbw_2 = asbEcc_2+anbEcc,

         asidi_2 = wt8,
         asbdi_2 = asbw_2+anbw,

         asiOMEGA0_2 = wt8,
         asbOMEGA0_2 = asbdi_2+anbdi,

         asiOMEGAdot_2 = wt8,
         asbOMEGAdot_2 = asbOMEGA0_2+anbOMEGA0,

         asiM0_2 = wt9,
         asbM0_2 = asbt0a+anbt0a,

         asiaf0_2 = wt9,
         asbaf0_2 = asbM0_2+anbM0,

         asiaf1_2 = wt9,
         asbaf1_2 = asbaf0_2+anbaf0,

         asiE5bhs_2 = wt9,
         asbE5bhs_2 = asbaf1_2+anbaf1,

         asiE1Bhs_2 = wt9,
         asbE1Bhs_2 = asbE5bhs_2+anbE5bhs,

            // SV(SVID3)

         asiSVID_3 = wt9,
         asbSVID_3 = asbE1Bhs_2+anbE1Bhs,

         asidAhalf_3 = wt9,
         asbdAhalf_3 = asbSVID_3+anbSVID,

         asiEcc_3 = wt9,
         asbEcc_3 = asbdAhalf_3+anbdAhalf,

         asiw_3 = wt9,
         asbw_3 = asbEcc_3+anbEcc,

         asidi_3 = wt9,
         asbdi_3 = asbw_3+anbw,

         asiOMEGA0_3 = wt10,
         asbOMEGA0_3 = asbIODa+anbIODa,

         asiOMEGAdot_3 = wt10,
         asbOMEGAdot_3 = asbOMEGA0_3+anbOMEGA0,

         asiM0_3 = wt10,
         asbM0_3 = asbOMEGAdot_3+anbOMEGAdot,

         asiaf0_3 = wt10,
         asbaf0_3 = asbM0_3+anbM0,

         asiaf1_3 = wt10,
         asbaf1_3 = asbaf0_3+anbaf0,

         asiE5bhs_3 = wt10,
         asbE5bhs_3 = asbaf1_3+anbaf1,

         asiE1Bhs_3 = wt10,
         asbE1Bhs_3 = asbE5bhs_3+anbE5bhs,

         asiA0G = wt10,
         asbA0G = asbE1Bhs_3+anbE1Bhs,
         anbA0G = 16,
         ascA0G = -35,

         asiA1G = wt10,
         asbA1G = asbA0G+anbA0G,
         anbA1G = 12,
         ascA1G = -51,

         asit0G = wt10,
         asbt0G = asbA1G+anbA1G,
         anbt0G = 8,
         asct0G = 3600,

         asiWN0G = wt10,
         asbWN0G = asbt0G+anbt0G,
         anbWN0G = 6,
         ascWN0G = 1,
      };
   } // namespace galinav
} // namespace gnsstk

#endif // GNSSTK_GALIBITS_HPP
