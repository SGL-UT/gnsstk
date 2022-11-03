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

#ifndef GNSSTK_GPSLBITS_HPP
#define GNSSTK_GPSLBITS_HPP

namespace gnsstk
{
   namespace gpslnav
   {
         /** Start bits, bit counts and scale factor (*n for integer
          * quantities, *2^n for floating point quantities) for fields
          * that apply to all messages. */
      enum FullBitInfo
      {
            // every subframe has a preamble and tlm so no subframe index here.
         fsbPre = 0, ///< Preamble start bit
         fnbPre = 8, ///< Preamble number of bits
         fscPre = 1, ///< Preamble scale factor

         fsbTLM = fsbPre+fnbPre,  ///< Telemetry Message start bit
         fnbTLM = 14, ///< Telemetry Message number of bits
         fscTLM = 1,  ///< Telemetry Message scale factor

         fsbISF = fsbTLM+fnbTLM, ///< Integrity Status Flag bit
         fnbISF = 1,             ///< Integrity Status Flag number of bits
         fscISF = 1,             ///< Integrity Status Flag scale factor

         fsbRsv = fsbISF+fnbISF, ///< Reserved bit
         fnbRsv = 1,

         fsbParity1 = fsbRsv+fnbRsv, ///< Word 1 parity
         fnbParity1 = 6,
         fscParity1 = 1,

         fsbTOW = fsbParity1+fnbParity1, ///< Time of week count
         fnbTOW = 17,
         fscTOW = 6,

         fsbAlert = fsbTOW+fnbTOW, ///< Alert flag start bit
         fnbAlert = 1,             ///< Alert flag number of bits

         fsbAS = fsbAlert+fnbAlert, ///< Anti-spoof flag start bit
         fnbAS = 1,                 ///< Anti-spoof flag number of bits

         fsbSFID = fsbAS+fnbAS, ///< Subframe ID
         fnbSFID = 3,
         fscSFID = 1,

         fsbSolver = fsbSFID+fnbSFID, ///< Bits solved for parity
         fnbSolver = 2,
         fscSolver = 1,

         fsbParity2 = fsbSolver+fnbSolver, ///< Word 2 parity
         fnbParity2 = 6,
         fscParity2 = 1,
      };

         // Miscellaneous constants.  Using enums instead of constants
         // because it doesn't use any memory that way.
      enum MiscConst
      {
         dataIDGPSonQZSS = 0, ///< Neither GPS nor QZSS use this any longer.
         dataIDGPS = 1,       ///< Standard GPS data ID.
         dataIDQZSS = 3,      ///< QZSS almanacs for QZSS satellites.
      };

         // This enum is only used in PNBGPSLNavDataFactory and it's
         // being used to avoid confusion on vector indices.  Using
         // enums instead of constants because it doesn't use any
         // memory that way.  Also not using strongly typed enums
         // because we actually want these values to be implicitly
         // cast to integer values.
      enum SFIndex
      {
         sf1 = 0,
         sf2 = 1,
         sf3 = 2
      };

         /** Subframe index, start bits, bit counts and scale factor (*n for
          * integer quantities, *2^n for floating point quantities) for each of
          * the ephemeris fields.
          * Bit positions/sizes from IS-GPS-200 Figure 20-1, sheet 1-3.
          * Scale factors taken from Tables 20-I and 20-III.
          */
      enum EphBitInfo
      {
         esiWN = sf1,                   ///< WN subframe index
         esbWN = fsbParity2+fnbParity2, ///< WN start bit
         enbWN = 10,                    ///< WN number of bits
         escWN = 1,                     ///< WN scale factor

         esiL2 = sf1,                   ///< L2 codes subframe index
         esbL2 = esbWN+enbWN,           ///< L2 codes start bit
         enbL2 = 2,                     ///< L2 codes number of bits
         escL2 = 1,                     ///< L2 codes scale factor

         esiURA = sf1,                  ///< URA index subframe index
         esbURA = esbL2+enbL2,          ///< URA index start bit
         enbURA = 4,                    ///< URA index number of bits
         escURA = 1,                    ///< URA index scale factor

         esiHea = sf1,                  ///< SV health subframe index
         esbHea = esbURA+enbURA,        ///< SV health start bit
         enbHea = 6,                    ///< SV health number of bits
         escHea = 1,                    ///< SV health scale factor

         esiIODC  = sf1,                ///< IODC subframe index
         esbIODCm = esbHea+enbHea,      ///< IODC MSBs start bit
         enbIODCm = 2,                  ///< IODC MSBs number of bits
         escIODC = 1,                   ///< IODC scale factor

         esbParity3 = esbIODCm+enbIODCm,///< Word 3 Parity start bit
         enbParity3 = 6,                ///< Word 3 Parity number of bits
         escParity3 = 1,                ///< Word 3 Parity scale factor

         esiL2P = sf1,                  ///< L2 P data flag subframe index
         esbL2P = esbParity3+enbParity3,///< L2 P data flag start bit
         enbL2P = 1,                    ///< L2 P data flag number of bits
         escL2P = 1,                    ///< L2 P data flag scale factor

         esbRsv4 = esbL2P+enbL2P,       ///< Word 4 reserved bits (sf1).
         enbRsv4 = 23,
         escRsv4 = 1,

         esbParity4 = esbRsv4+enbRsv4,  ///< Word 4 Parity
         enbParity4 = 6,
         escParity4 = 1,

         esbRsv5 = esbParity4+enbParity4, ///< Word 5 reserved bits (sf1).
         enbRsv5 = 24,
         escRsv5 = 1,

         esbParity5 = esbRsv5+enbRsv5,  ///< Word 5 Parity
         enbParity5 = 6,
         escParity5 = 1,

         esbRsv6 = esbParity5+enbParity5, ///< Word 6 reserved bits (sf1).
         enbRsv6 = 24,
         escRsv6 = 1,

         esbParity6 = esbRsv6+enbRsv6,  ///< Word 6 Parity
         enbParity6 = 6,
         escParity6 = 1,

         esbRsv7 = esbParity6+enbParity6, ///< Word 7 reserved bits (sf1).
         enbRsv7 = 16,
         escRsv7 = 1,

         esiTGD = sf1,             ///< Tgd subframe index
         esbTGD = esbRsv7+enbRsv7, ///< Tgd start bit
         enbTGD = 8,               ///< Tgd number of bits
         escTGD = -31,             ///< Tgd scale factor

         esbParity7 = esbTGD+enbTGD, ///< Word 7 Parity
         enbParity7 = 6,
         escParity7 = 1,

         esbIODCl = esbParity7+enbParity7, ///< IODC LSBs start bit
         enbIODCl = 8,                     ///< IODC LSBs number of bits

         esitoc = sf1,                   ///< toc subframe index
         esbtoc = esbIODCl+enbIODCl,     ///< toc start bit
         enbtoc = 16,                    ///< toc number of bits
         esctoc = 4,                     ///< toc scale factor

         esbParity8 = esbtoc+enbtoc,     ///< Word 8 Parity
         enbParity8 = 6,
         escParity8 = 1,

         esiaf2 = sf1,                   ///< af2 subframe index
         esbaf2 = esbParity8+enbParity8, ///< af2 start bit
         enbaf2 = 8,                     ///< af2 number of bits
         escaf2 = -55,                   ///< af2 scale factor

         esiaf1 = sf1,                   ///< af1 subframe index
         esbaf1 = esbaf2+enbaf2,         ///< af1 start bit
         enbaf1 = 16,                    ///< af1 number of bits
         escaf1 = -43,                   ///< af1 scale factor

         esbParity9 = esbaf1+enbaf1,     ///< Word 9 Parity
         enbParity9 = 6,
         escParity9 = 1,

         esiaf0 = sf1,                   ///< af0 subframe index
         esbaf0 = esbParity9+enbParity9, ///< af0 start bit
         enbaf0 = 22,                    ///< af0 number of bits
         escaf0 = -31,                   ///< af0 scale factor

         esiIODE2 = sf2,                 ///< IODE2 subframe index
         esbIODE2 = fsbParity2+fnbParity2,///< IODE2 start bit
         enbIODE2 = 8,                   ///< IODE2 number of bits
         escIODE2 = 1,                   ///< IODE2 scale factor

         esiCrs = sf2,                   ///< Crs subframe index
         esbCrs = esbIODE2+enbIODE2,     ///< Crs start bit
         enbCrs = 16,                    ///< Crs number of bits
         escCrs = -5,                    ///< Crs scale factor

         esidn = sf2,                    ///< Delta n subframe index
         esbdn = esbParity3+enbParity4,  ///< Delta n start bit
         enbdn = 16,                     ///< Delta n number of bits
         escdn = -43,                    ///< Delta n scale factor

         esiM0  = sf2,                   ///< M0 subframe index
         esbM0m = esbdn+enbdn,           ///< M0 MSBs start bit
         enbM0m = 8,                     ///< M0 MSBs number of bits
         escM0  = -31,                   ///< M0 scale factor

         esbM0l = esbParity4+enbParity4, ///< M0 LSBs start bit
         enbM0l = 24,                    ///< M0 LSBs number of bits

         esiCuc = sf2,                   ///< Cuc subframe index
         esbCuc = esbParity5+enbParity5, ///< Cuc start bit
         enbCuc = 16,                    ///< Cuc number of bits
         escCuc = -29,                   ///< Cuc scale factor

         esiEcc  = sf2,                  ///< Ecc subframe index
         esbEccm = esbCuc+enbCuc,        ///< Ecc MSBs start bit
         enbEccm = 8,                    ///< Ecc MSBs number of bits
         escEcc  = -33,                  ///< Ecc scale factor

         esbEccl = esbParity6+enbParity6, ///< Ecc LSBs start bit
         enbEccl = 24,                    ///< Ecc LSBs number of bits

         esiCus = sf2,                   ///< Cus subframe index
         esbCus = esbParity7+enbParity7, ///< Cus start bit
         enbCus = 16,                    ///< Cus number of bits
         escCus = -29,                   ///< Cus scale factor

         esiAhalf  = sf2,                ///< Ahalf subframe index
         esbAhalfm = esbCus+enbCus,      ///< Ahalf MSBs start bit
         enbAhalfm = 8,                  ///< Ahalf MSBs number of bits
         escAhalf  = -19,                ///< Ahalf scale factor

         esbAhalfl = esbParity8+enbParity8, ///< Ahalf LSBs start bit
         enbAhalfl = 24,                 ///< Ahalf LSBs number of bits

         esitoe = sf2,                   ///< toe subframe index
         esbtoe = esbParity9+enbParity9, ///< toe start bit
         enbtoe = 16,                    ///< toe number of bits
         esctoe = 4,                     ///< toe scale factor

         esiFitInt = sf2,                ///< Fit interval flag subframe index
         esbFitInt = esbtoe+enbtoe,      ///< Fit interval flag start bit
         enbFitInt = 1,                  ///< Fit interval flag number of bits
         escFitInt = 1,                  ///< Fit interval flag scale factor

         esiAODO = sf2,                  ///< AODO subframe index
         esbAODO = esbFitInt+enbFitInt,  ///< AODO start bit
         enbAODO = 5,                    ///< AODO number of bits
         escAODO = 900,                  ///< AODO scale factor

         esiCic = sf3,                   ///< Cic subframe index
         esbCic = fsbParity2+fnbParity2, ///< Cic start bit
         enbCic = 16,                    ///< Cic number of bits
         escCic = -29,                   ///< Cic scale factor

         esiOMEGA0  = sf3,               ///< OMEGA0 subframe index
         esbOMEGA0m = esbCic+enbCic,     ///< OMEGA0 MSBs start bit
         enbOMEGA0m = 8,                 ///< OMEGA0 MSBs number of bits
         escOMEGA0  = -31,               ///< OMEGA0 scale factor

         esbOMEGA0l = esbParity3+enbParity3, ///< OMEGA0 LSBs start bit
         enbOMEGA0l = 24,                ///< OMEGA0 LSBs number of bits

         esiCis = sf3,                   ///< Cis subframe index
         esbCis = esbParity4+enbParity4, ///< Cis start bit
         enbCis = 16,                    ///< Cis number of bits
         escCis = -29,                   ///< Cis scale factor

         esii0  = sf3,                   ///< i0 subframe index
         esbi0m = esbCis+enbCis,         ///< i0 MSBs start bit
         enbi0m = 8,                     ///< i0 MSBs number of bits
         esci0  = -31,                   ///< i0 scale factor

         esbi0l = esbParity5+enbParity5, ///< i0 LSBs start bit
         enbi0l = 24,                    ///< i0 LSBs number of bits

         esiCrc = sf3,                   ///< Crc subframe index
         esbCrc = esbParity6+enbParity6, ///< Crc start bit
         enbCrc = 16,                    ///< Crc number of bits
         escCrc = -5,                    ///< Crc scale factor

         esiw  = sf3,                    ///< w subframe index
         esbwm = esbCrc+enbCrc,          ///< w MSBs start bit
         enbwm = 8,                      ///< w MSBs number of bits
         escw  = -31,                    ///< w scale factor

         esbwl = esbParity7+enbParity7,  ///< w LSBs start bit
         enbwl = 24,                     ///< w LSBs number of bits

         esiOMEGAdot = sf3,              ///< OMEGAdot subframe index
         esbOMEGAdot = esbParity8+enbParity8, ///< OMEGAdot start bit
         enbOMEGAdot = 24,               ///< OMEGAdot number of bits
         escOMEGAdot = -43,              ///< OMEGAdot scale factor

         esiIODE3 = sf3,                 ///< IODE3 subframe index
         esbIODE3 = esbParity9+enbParity9, ///< IODE3 start bit
         enbIODE3 = 8,                   ///< IODE3 number of bits
         escIODE3 = 1,                   ///< IODE3 scale factor

         esiidot = sf3,                  ///< idot subframe index
         esbidot = esbIODE3+enbIODE3,    ///< idot start bit
         enbidot = 14,                   ///< idot number of bits
         escidot = -43,                  ///< idot scale factor
      };

         /** Subframe index, start bits, bit counts and scale factor (*n for
          * integer quantities, *2^n for floating point quantities) for each of
          * the almanac fields.
          * Bit positions/sizes from IS-GPS-200 Figure 20-1, sheet 1-3.
          * Scale factors taken from Tables 20-I and 20-III.
          * @todo Add enumerations for almanac data bits.
          */
      enum AlmBitInfo
      {
            // orbital elements, SVID 1-32 (sf5 p1-24, sf4, p 2,3,4,5,7,8,9,10)

         asbDataID = fsbParity2+fnbParity2,
         anbDataID = 2,
         ascDataID = 1,

         asbPageID = asbDataID + anbDataID,
         anbPageID = 6,
         ascPageID = 1,

            // orbital elements (sf5 p1-24; sf4 p2-5,7-10)

         asbEcc = asbPageID+anbPageID,  ///< Ecc start bit
         anbEcc = 16,                   ///< Ecc number of bits
         ascEcc = -21,                  ///< Ecc scale factor

         asbtoa = esbParity3+enbParity3, ///< Almanac reference time
         anbtoa = 8,
         asctoa = 12,

         asbdeltai = asbtoa+anbtoa,     ///< inclination offset
         anbdeltai = 16,
         ascdeltai = -19,

         asbOMEGAdot = esbParity4+enbParity4, ///< OMEGAdot start bit
         anbOMEGAdot = 16,
         ascOMEGAdot = -38,

         asbHea = asbOMEGAdot+anbOMEGAdot, ///< Health bits
         anbHea = 8,
         ascHea = 1,

         asbAhalf = esbParity5+enbParity5, ///< Ahalf start bit
         anbAhalf = 24,
         ascAhalf = -11,

         asbOMEGA0 = esbParity6+enbParity6, ///< OMEGA0 start bit
         anbOMEGA0 = 24,
         ascOMEGA0 = -23,

         asbw = esbParity7+enbParity7,      ///< w start bit
         anbw = 24,
         ascw = -23,

         asbM0 = esbParity8+enbParity8,     ///< M0 start bit
         anbM0 = 24,
         ascM0 = -23,

         asbaf0m = esbParity9+enbParity9,   ///< af0 MSBs start bit
         anbaf0m = 8,
         ascaf0 = -20,

         asbaf1 = asbaf0m+anbaf0m,          ///< af1 start bit
         anbaf1 = 11,
         ascaf1 = -38,

         asbaf0l = asbaf1+anbaf1,           ///< af0 LSBs start bit
         anbaf0l = 3,

            // ionospheric parameters (sf4 p18)

         asbAlpha0 = asbPageID + anbPageID,
         anbAlpha0 = 8,
         ascAlpha0 = -30,

         asbAlpha1 = asbAlpha0 + anbAlpha0,
         anbAlpha1 = 8,
         ascAlpha1 = -27,

         asbAlpha2 = esbParity3 + enbParity3,
         anbAlpha2 = 8,
         ascAlpha2 = -24,

         asbAlpha3 = asbAlpha2 + anbAlpha2,
         anbAlpha3 = 8,
         ascAlpha3 = -24,

         asbBeta0 = asbAlpha3 + anbAlpha3,
         anbBeta0 = 8,
         ascBeta0 = 11,

         asbBeta1 = esbParity4 + enbParity4,
         anbBeta1 = 8,
         ascBeta1 = 14,

         asbBeta2 = asbBeta1 + anbBeta1,
         anbBeta2 = 8,
         ascBeta2 = 16,

         asbBeta3 = asbBeta2 + anbBeta2,
         anbBeta3 = 8,
         ascBeta3 = 16,

         asbA1 = esbParity5+enbParity5,
         anbA1 = 24,
         ascA1 = -50,

         asbA0m = esbParity6+enbParity6,
         anbA0m = 24,
         ascA0 = -30,

         asbA0l = esbParity7+enbParity7,
         anbA0l = 8,

         asbtot = asbA0l+anbA0l,
         anbtot = 8,
         asctot = 12,

         asbWNt = asbtot+anbtot,
         anbWNt = 8,
         ascWNt = 1,

         asbDeltatLS = esbParity8+enbParity8,
         anbDeltatLS = 8,
         ascDeltatLS = 1,

         asbWNLSF = asbDeltatLS+anbDeltatLS,
         anbWNLSF = 8,
         ascWNLSF = 1,

         asbDN = asbWNLSF+anbWNLSF,
         anbDN = 8,
         ascDN = 1,

         asbDeltatLSF = esbParity9+enbParity9,
         anbDeltatLSF = 8,
         ascDeltatLSF = 1,

            // constellation health page

         asbtoa51 = asbPageID + anbPageID,
         anbtoa51 = 8,
         asctoa51 = 12,

         asbWNa51 = asbtoa51+anbtoa51,
         anbWNa51 = 8,
         ascWNa51 = 1,
      };
   } // namespace gpslnav
} // namespace gnsstk

#endif // GNSSTK_GPSLBITS_HPP
