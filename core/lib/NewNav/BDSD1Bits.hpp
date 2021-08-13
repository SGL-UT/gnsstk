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

#ifndef GPSTK_BDSD1BITS_HPP
#define GPSTK_BDSD1BITS_HPP

namespace gpstk
{
   namespace bds
   {
         /** Start bits, bit counts and scale factor (*n for integer
          * quantities, *2^n for floating point quantities) for fields
          * that apply to all messages. */
      enum FullBitInfo
      {
         fsbPre = 0,  ///< Preamble start bit
         fnbPre = 11, ///< Preamble number of bits
         fscPre = 1,  ///< Preamble scale factor

         fsbRev = fsbPre+fnbPre, ///< Reserved data start bit
         fnbRev = 4,             ///< Reserved data number of bits
         fscRev = 1,             ///< Reserved data scale factor

         fsbFraID = fsbRev+fnbRev, ///< Frame ID start bit
         fnbFraID = 3,             ///< Frame ID number of bits
         fscFraID = 1,             ///< Frame ID scale factor

         fsbSOWm = fsbFraID+fnbFraID,   ///< SOW MSBs start bit
         fnbSOWm = 8,                   ///< SOW MSBs number of bits
         fscSOW  = 0,                   ///< SOW scale factor

         fsbParity1 = fsbSOWm+fnbSOWm,  ///< Word 1 parity start bit
         fnbParity1 = 4,                ///< Word 1 parity number of bits
         fscParity1 = 1,                ///< Word 1 parity scale factor

         fsbSOWl = fsbParity1+fnbParity1, ///< SOW LSBs start bit
         fnbSOWl = 12,                    ///< SOW LSBs number of bits

         fsbRev2 = fsbSOWl+fnbSOWl,     ///< Reserved data in SF4/5 word 2, start bit
         fnbRev2 = 1,                   ///< Reserved data in SF4/5 word 2, num bits
         fscRev2 = 1,                   ///< Reserved data in SF4/5 word 2, scale

         fsbPnum = fsbRev2+fnbRev2,     ///< Page number in SF4/5 word 2, start bit
         fnbPnum = 7,                   ///< Page number in SF4/5 word 2, num bits
         fscPnum = 1,                   ///< Page number in SF4/5 word 2, scale
      };

         /** Start bits, bit counts and scale factor (*n for integer
          * quantities, *2^n for floating point quantities) for each of the
          * fields in almanac pages.
          * Bit positions/sizes from BeiDou SIS-ICD, Fig 5-11-1
          * Scale factors taken from Table 4-13
          */
      enum AlmBitInfo
      {
         asbAhalfm = fsbPnum+fnbPnum,         ///< Semi-major axis**0.5 MSBs
         anbAhalfm = 2,
         ascAhalf = -11,

         asbParity2 = asbAhalfm+anbAhalfm,    ///< Word 2 parity
         anbParity2 = 8,
         ascParity2 = 1,

         asbAhalfl = asbParity2+anbParity2,   ///< Semi-major axis**0.5 LSBs
         anbAhalfl = 22,

         asbParity3 = asbAhalfl+anbAhalfl,    ///< Word 3 parity
         anbParity3 = 8,
         ascParity3 = 1,

         asbA1 = asbParity3+anbParity3,       ///< Clock drift correction
         anbA1 = 11,
         ascA1 = -38,

         asbA0 = asbA1+anbA1,                 ///< Clock correction
         anbA0 = 11,
         ascA0 = -20,

         asbParity4 = asbA0+anbA0,            ///< Word 4 parity
         anbParity4 = 8,
         ascParity4 = 1,

         asbOMEGA0m = asbParity4+anbParity4,  ///< OMEGA0 MSBs
         anbOMEGA0m = 22,
         ascOMEGA0 = -23,

         asbParity5 = asbOMEGA0m+anbOMEGA0m,  ///< Word 5 parity
         anbParity5 = 8,
         ascParity5 = 1,

         asbOMEGA0l = asbParity5+anbParity5,  ///< OMEGA0 LSBs
         anbOMEGA0l = 2,

         asbEcc = asbOMEGA0l+anbOMEGA0l,      ///< Eccentricity
         anbEcc = 17,
         ascEcc = -21,

         asbdim = asbEcc+anbEcc,              ///< delta inclination MSBs
         anbdim = 3,
         ascdi = -19,

         asbParity6 = asbdim+anbdim,          ///< Word 6 parity
         anbParity6 = 8,
         ascParity6 = 1,

         asbdil = asbParity6+anbParity6,      ///< delta inclination LSBs
         anbdil = 13,

         asbtoa = asbdil+anbdil,              ///< toa (reference time)
         anbtoa = 8,
         asctoa = 12,

         asbOMEGAdotm = asbtoa+anbtoa,        ///< OMEGA dot MSBs
         anbOMEGAdotm = 1,
         ascOMEGAdot = -38,

         asbParity7 = asbOMEGAdotm+anbOMEGAdotm,      ///< Word 7 parity
         anbParity7 = 8,
         ascParity7 = 1,

         asbOMEGAdotl = asbParity7+anbParity7,        ///< OMEGA dot LSBs
         anbOMEGAdotl = 16,

         asbwm = asbOMEGAdotl+anbOMEGAdotl,   ///< little omega MSBs
         anbwm = 6,
         ascw = -23,

         asbParity8 = asbwm+anbwm,            ///< Word 8 parity
         anbParity8 = 8,
         ascParity8 = 1,

         asbwl = asbParity8+anbParity8,       ///< little omega LSBs
         anbwl = 18,

         asbM0m = asbwl+anbwl,                ///< M0 MSBs
         anbM0m = 4,
         ascM0 = -23,

         asbParity9 = asbM0m+anbM0m,          ///< Word 9 parity
         anbParity9 = 8,
         ascParity9 = 1,

         asbM0l = asbParity9+anbParity9,      ///< M0 LSBs
         anbM0l = 20,

         asbAmEpID = asbM0l+anbM0l,           ///< AmEpID for sat ID
         anbAmEpID = 2,
         ascAmEpID = 1,
      };

         /** Define array indices for ephemeris subframes when storing
          * the data for later use. */
      enum SFIndex
      {
         sf1 = 0,
         sf2 = 1,
         sf3 = 2
      };

         /** Subframe index, start bits, bit counts and scale factor (*n for
          * integer quantities, *2^n for floating point quantities) for each of
          * the fields in ephemeris messages.
          * Bit positions/sizes from BeiDou SIS-ICD, Fig 5-11-1
          * Scale factors taken from Table 4-13
          */
      enum EphBitInfo
      {
            // subframe 1
         esiSatH1 = sf1,                      ///< Satellite health
         esbSatH1 = fsbSOWl+fnbSOWl,
         enbSatH1 = 1,
         escSatH1 = 1,

         esiAODC = sf1,                       ///< AODC
         esbAODC = esbSatH1+enbSatH1,
         enbAODC = 5,
         escAODC = 1,

         esiURAI = sf1,                       ///< User range accuracy index
         esbURAI = esbAODC+enbAODC,
         enbURAI = 4,
         escURAI = 1,

         esiParity12 = sf1,                   ///< Word 2 parity
         esbParity12 = esbURAI+enbURAI,
         enbParity12 = 8,
         escParity12 = 1,

         esiWN = sf1,                         ///< Reference week number
         esbWN = esbParity12+enbParity12,
         enbWN = 13,
         escWN = 1,

         esitocm = sf1,                       ///< Clock correction reference SOW MSBs
         esbtocm = esbWN+enbWN,
         enbtocm = 9,
         esctoc = 3,

         esiParity13 = sf1,                    ///< Word 3 parity
         esbParity13 = esbtocm+enbtocm,
         enbParity13 = 8,
         escParity13 = 1,

         esitocl = sf1,                       ///< Clock correction reference SOW LSBs
         esbtocl = esbParity13+enbParity13,
         enbtocl = 8,

         esiTGD1 = sf1,                       ///< Group delay differential #1
         esbTGD1 = esbtocl+enbtocl,
         enbTGD1 = 10,
         escTGD1 = 0,                         ///< Actual scale is 0.1 per 5.2.4.8

         esiTGD2m = sf1,                      ///< Group delay differential #2 MSBs
         esbTGD2m = esbTGD1+enbTGD1,
         enbTGD2m = 4,
         escTGD2 = 0,                         ///< Actual scale is 0.1 per 5.2.4.8

         esiParity14 = sf1,                    ///< Word 4 parity
         esbParity14 = esbTGD2m+enbTGD2m,
         enbParity14 = 8,
         escParity14 = 1,

         esiTGD2l = sf1,                      ///< Group delay differential #2 LSBs
         esbTGD2l = esbParity14+enbParity14,
         enbTGD2l = 6,

         esiAlpha0 = sf1,                     ///< Klobuchar iono alpha_0
         esbAlpha0 = esbTGD2l+enbTGD2l,
         enbAlpha0 = 8,
         escAlpha0 = -30,

         esiAlpha1 = sf1,                     ///< Klobuchar iono alpha_1
         esbAlpha1 = esbAlpha0+enbAlpha0,
         enbAlpha1 = 8,
         escAlpha1 = -27,

         esiParity15 = sf1,                    ///< Word 5 parity
         esbParity15 = esbAlpha1+enbAlpha1,
         enbParity15 = 8,
         escParity15 = 1,

         esiAlpha2 = sf1,                     ///< Klobuchar iono alpha_2
         esbAlpha2 = esbParity15+enbParity15,
         enbAlpha2 = 8,
         escAlpha2 = -24,

         esiAlpha3 = sf1,                     ///< Klobuchar iono alpha_3
         esbAlpha3 = esbAlpha2+enbAlpha2,
         enbAlpha3 = 8,
         escAlpha3 = -24,

         esiBeta0m = sf1,                     ///< Klobuchar iono beta_0 MSBs
         esbBeta0m = esbAlpha3+enbAlpha3,
         enbBeta0m = 6,
         escBeta0 = 11,

         esiParity16 = sf1,                    ///< Word 6 parity
         esbParity16 = esbBeta0m+enbBeta0m,
         enbParity16 = 8,
         escParity16 = 1,

         esiBeta0l = sf1,                     ///< Klobuchar iono beta_0 LSBs
         esbBeta0l = esbParity16+enbParity16,
         enbBeta0l = 2,

         esiBeta1 = sf1,                      ///< Klobuchar iono beta_1
         esbBeta1 = esbBeta0l+enbBeta0l,
         enbBeta1 = 8,
         escBeta1 = 14,

         esiBeta2 = sf1,                      ///< Klobuchar iono beta_2
         esbBeta2 = esbBeta1+enbBeta1,
         enbBeta2 = 8,
         escBeta2 = 16,

         esiBeta3m = sf1,                     ///< Klobuchar iono beta_3 MSBs
         esbBeta3m = esbBeta2+enbBeta2,
         enbBeta3m = 4,
         escBeta3 = 16,

         esiParity17 = sf1,                    ///< Word 7 parity
         esbParity17 = esbBeta3m+enbBeta3m,
         enbParity17 = 8,
         escParity17 = 1,

         esiBeta3l = sf1,                     ///< Klobuchar iono beta_3 LSBs
         esbBeta3l = esbParity17+enbParity17,
         enbBeta3l = 4,

         esia2 = sf1,                         ///< Clock correction 2nd order term
         esba2 = esbBeta3l+enbBeta3l,
         enba2 = 11,
         esca2 = -66,

         esia0m = sf1,                        ///< Clock correction 0th order term MSB
         esba0m = esba2+enba2,
         enba0m = 7,
         esca0 = -33,

         esiParity18 = sf1,                    ///< Word 8 parity
         esbParity18 = esba0m+enba0m,
         enbParity18 = 8,
         escParity18 = 1,

         esia0l = sf1,                        ///< Clock correction 0th order term LSB
         esba0l = esbParity18+enbParity18,
         enba0l = 17,

         esia1m = sf1,                        ///< Clock correction 1st order term MSB
         esba1m = esba0l+enba0l,
         enba1m = 5,
         esca1 = -50,

         esiParity19 = sf1,                    ///< Word 9 parity
         esbParity19 = esba1m+enba1m,
         enbParity19 = 8,
         escParity19 = 1,

         esia1l = sf1,                        ///< Clock correction 1st order term LSB
         esba1l = esbParity19+enbParity19,
         enba1l = 17,

         esiAODE = sf1,                       ///< Age of data - ephemeris
         esbAODE = esba1l+enba1l,
         enbAODE = 5,
         escAODE = 1,

            // subframe 2
         esidnm = sf2,
         esbdnm = fsbSOWl+fnbSOWl,
         enbdnm = 10,
         escdn = -43,

         esiParity22 = sf2,
         esbParity22 = esbdnm+enbdnm,
         enbParity22 = 8,
         escParity22 = 1,

         esidnl = sf2,
         esbdnl = esbParity22+enbParity22,
         enbdnl = 6,

         esiCucm = sf2,
         esbCucm = esbdnl+enbdnl,
         enbCucm = 16,
         escCuc = -31,

         esiParity23 = sf2,
         esbParity23 = esbCucm+enbCucm,
         enbParity23 = 8,
         escParity23 = 1,

         esiCucl = sf2,
         esbCucl = esbParity23+enbParity23,
         enbCucl = 2,

         esiM0m = sf2,
         esbM0m = esbCucl+enbCucl,
         enbM0m = 20,
         escM0 = -31,

         esiParity24 = sf2,
         esbParity24 = esbM0m+enbM0m,
         enbParity24 = 8,
         escParity24 = 1,

         esiM0l = sf2,
         esbM0l = esbParity24+enbParity24,
         enbM0l = 12,

         esiEccm = sf2,
         esbEccm = esbM0l+enbM0l,
         enbEccm = 10,
         escEcc = -33,

         esiParity25 = sf2,
         esbParity25 = esbEccm+enbEccm,
         enbParity25 = 8,
         escParity25 = 1,

         esiEccl = sf2,
         esbEccl = esbParity25+enbParity25,
         enbEccl = 22,

         esiParity26 = sf2,
         esbParity26 = esbEccl+enbEccl,
         enbParity26 = 8,
         escParity26 = 1,

         esiCus = sf2,
         esbCus = esbParity26+enbParity26,
         enbCus = 18,
         escCus = -31,

         esiCrcm = sf2,
         esbCrcm = esbCus+enbCus,
         enbCrcm = 4,
         escCrc = -6,

         esiParity27 = sf2,
         esbParity27 = esbCrcm+enbCrcm,
         enbParity27 = 8,
         escParity27 = 1,

         esiCrcl = sf2,
         esbCrcl = esbParity27+enbParity27,
         enbCrcl = 14,

         esiCrsm = sf2,
         esbCrsm = esbCrcl+enbCrcl,
         enbCrsm = 8,
         escCrs = -6,

         esiParity28 = sf2,
         esbParity28 = esbCrsm+enbCrsm,
         enbParity28 = 8,
         escParity28 = 1,

         esiCrsl = sf2,
         esbCrsl = esbParity28+enbParity28,
         enbCrsl = 10,

         esiAhalfm = sf2,
         esbAhalfm = esbCrsl+enbCrsl,
         enbAhalfm = 12,
         escAhalf = -19,

         esiParity29 = sf2,
         esbParity29 = esbAhalfm+enbAhalfm,
         enbParity29 = 8,
         escParity29 = 1,

         esiAhalfl = sf2,
         esbAhalfl = esbParity29+enbParity29,
         enbAhalfl = 20,

         esitoeh = sf2,
         esbtoeh = esbAhalfl+enbAhalfl,
         enbtoeh = 2,
         esctoe = 3,
   
            // subframe 3
         esitoem = sf3,
         esbtoem = fsbSOWl+fnbSOWl,
         enbtoem = 10,

         esiParity32 = sf3,
         esbParity32 = esbtoem+enbtoem,
         enbParity32 = 8,
         escParity32 = 1,

         esitoel = sf3,
         esbtoel = esbParity32+enbParity32,
         enbtoel = 5,

         esii0m = sf3,
         esbi0m = esbtoel+enbtoel,
         enbi0m = 17,
         esci0 = -31,

         esiParity33 = sf3,
         esbParity33 = esbi0m+enbi0m,
         enbParity33 = 8,
         escParity33 = 1,

         esii0l = sf3,
         esbi0l = esbParity33+enbParity33,
         enbi0l = 15,

         esiCicm = sf3,
         esbCicm = esbi0l+enbi0l,
         enbCicm = 7,
         escCic = -31,

         esiParity34 = sf3,
         esbParity34 = esbCicm+enbCicm,
         enbParity34 = 8,
         escParity34 = 1,

         esiCicl = sf3,
         esbCicl = esbParity34+enbParity34,
         enbCicl = 11,

         esiOMEGAdotm = sf3,
         esbOMEGAdotm = esbCicl+enbCicl,
         enbOMEGAdotm = 11,
         escOMEGAdot = -43,

         esiParity35 = sf3,
         esbParity35 = esbOMEGAdotm+enbOMEGAdotm,
         enbParity35 = 8,
         escParity35 = 1,

         esiOMEGAdotl = sf3,
         esbOMEGAdotl = esbParity35+enbParity35,
         enbOMEGAdotl = 13,

         esiCism = sf3,
         esbCism = esbOMEGAdotl+enbOMEGAdotl,
         enbCism = 9,
         escCis = -31,

         esiParity36 = sf3,
         esbParity36 = esbCism+enbCism,
         enbParity36 = 8,
         escParity36 = 1,

         esiCisl = sf3,
         esbCisl = esbParity36+enbParity36,
         enbCisl = 9,

         esiidotm = sf3,
         esbidotm = esbCisl+enbCisl,
         enbidotm = 13,
         escidot = -43,

         esiParity37 = sf3,
         esbParity37 = esbidotm+enbidotm,
         enbParity37 = 8,
         escParity37 = 1,

         esiidotl = sf3,
         esbidotl = esbParity37+enbParity37,
         enbidotl = 1,

         esiOMEGA0m = sf3,
         esbOMEGA0m = esbidotl+enbidotl,
         enbOMEGA0m = 21,
         escOMEGA0 = -31,

         esiParity38 = sf3,
         esbParity38 = esbOMEGA0m+enbOMEGA0m,
         enbParity38 = 8,
         escParity38 = 1,

         esiOMEGA0l = sf3,
         esbOMEGA0l = esbParity38+enbParity38,
         enbOMEGA0l = 11,

         esiwm = sf3,
         esbwm = esbOMEGA0l+enbOMEGA0l,
         enbwm = 11,
         escw = -31,

         esiParity39 = sf3,
         esbParity39 = esbwm+enbwm,
         enbParity39 = 8,
         escParity39 = 1,

         esiwl = sf3,
         esbwl = esbParity39+enbParity39,
         enbwl = 21,
      };

         /** Start bits, bit counts and scale factor (*n for integer
          * quantities, *2^n for floating point quantities) for each of the
          * fields in subframe 5 page 7 (health).
          * Bit positions/sizes from BeiDou SIS-ICD, Fig 5-11-2
          */
      enum SF5Pg7BitInfo
      {
         h1sbHea1m = fsbPnum+fnbPnum,
         h1nbHea1m = 2,
         h1scHea1 = 1,

         h1sbParity2 = h1sbHea1m+h1nbHea1m,
         h1nbParity2 = 8,
         h1scParity2 = 1,

         h1sbHea1l = h1sbParity2+h1nbParity2,
         h1nbHea1l = 7,

         h1sbHea2 = h1sbHea1l+h1nbHea1l,
         h1nbHea2 = 9,
         h1scHea2 = 1,

         h1sbHea3m = h1sbHea2+h1nbHea2,
         h1nbHea3m = 6,
         h1scHea3 = 1,

         h1sbParity3 = h1sbHea3m+h1nbHea3m,
         h1nbParity3 = 8,
         h1scParity3 = 1,

         h1sbHea3l = h1sbParity3+h1nbParity3,
         h1nbHea3l = 3,

         h1sbHea4 = h1sbHea3l+h1nbHea3l,
         h1nbHea4 = 9,
         h1scHea4 = 1,

         h1sbHea5 = h1sbHea4+h1nbHea4,
         h1nbHea5 = 9,
         h1scHea5 = 1,

         h1sbHea6m = h1sbHea5+h1nbHea5,
         h1nbHea6m = 1,
         h1scHea6 = 1,

         h1sbParity4 = h1sbHea6m+h1nbHea6m,
         h1nbParity4 = 8,
         h1scParity4 = 1,

         h1sbHea6l = h1sbParity4+h1nbParity4,
         h1nbHea6l = 8,

         h1sbHea7 = h1sbHea6l+h1nbHea6l,
         h1nbHea7 = 9,
         h1scHea7 = 1,

         h1sbHea8m = h1sbHea7+h1nbHea7,
         h1nbHea8m = 5,
         h1scHea8 = 1,

         h1sbParity5 = h1sbHea8m+h1nbHea8m,
         h1nbParity5 = 8,
         h1scParity5 = 1,

         h1sbHea8l = h1sbParity5+h1nbParity5,
         h1nbHea8l = 4,

         h1sbHea9 = h1sbHea8l+h1nbHea8l,
         h1nbHea9 = 9,
         h1scHea9 = 1,

         h1sbHea10 = h1sbHea9+h1nbHea9,
         h1nbHea10 = 9,
         h1scHea10 = 1,

         h1sbParity6 = h1sbHea10+h1nbHea10,
         h1nbParity6 = 8,
         h1scParity6 = 1,

         h1sbHea11 = h1sbParity6+h1nbParity6,
         h1nbHea11 = 9,
         h1scHea11 = 1,

         h1sbHea12 = h1sbHea11+h1nbHea11,
         h1nbHea12 = 9,
         h1scHea12 = 1,

         h1sbHea13m = h1sbHea12+h1nbHea12,
         h1nbHea13m = 4,
         h1scHea13 = 1,

         h1sbParity7 = h1sbHea13m+h1nbHea13m,
         h1nbParity7 = 8,
         h1scParity7 = 1,

         h1sbHea13l = h1sbParity7+h1nbParity7,
         h1nbHea13l = 5,

         h1sbHea14 = h1sbHea13l+h1nbHea13l,
         h1nbHea14 = 9,
         h1scHea14 = 1,

         h1sbHea15m = h1sbHea14+h1nbHea14,
         h1nbHea15m = 8,
         h1scHea15 = 1,

         h1sbParity8 = h1sbHea15m+h1nbHea15m,
         h1nbParity8 = 8,
         h1scParity8 = 1,

         h1sbHea15l = h1sbParity8+h1nbParity8,
         h1nbHea15l = 1,

         h1sbHea16 = h1sbHea15l+h1nbHea15l,
         h1nbHea16 = 9,
         h1scHea16 = 1,

         h1sbHea17 = h1sbHea16+h1nbHea16,
         h1nbHea17 = 9,
         h1scHea17 = 1,

         h1sbHea18m = h1sbHea17+h1nbHea17,
         h1nbHea18m = 3,
         h1scHea18 = 1,

         h1sbParity9 = h1sbHea18m+h1nbHea18m,
         h1nbParity9 = 8,
         h1scParity9 = 1,

         h1sbHea18l = h1sbParity9+h1nbParity9,
         h1nbHea18l = 6,

         h1sbHea19 = h1sbHea18l+h1nbHea18l,
         h1nbHea19 = 9,
         h1scHea19 = 1,
      };


         /** Start bits, bit counts and scale factor (*n for integer
          * quantities, *2^n for floating point quantities) for each of the
          * fields in subframe 5 page 8 (health, WNa, toa).
          * Bit positions/sizes from BeiDou SIS-ICD, Fig 5-11-3
          */
      enum SF5Pg8BitInfo
      {
         h2sbHea20m = fsbPnum+fnbPnum,                ///< PRN 20 health MSBs
         h2nbHea20m = 2,
         h2scHea20 = 1,

         h2sbParity2 = h2sbHea20m+h2nbHea20m,         ///< Word 2 parity
         h2nbParity2 = 8,
         h2scParity2 = 1,

         h2sbHea20l = h2sbParity2+h2nbParity2,        ///< PRN 20 health LSBs
         h2nbHea20l = 7,

         h2sbHea21 = h2sbHea20l+h2nbHea20l,           ///< PRN 21 health
         h2nbHea21 = 9,
         h2scHea21 = 1,

         h2sbHea22m = h2sbHea21+h2nbHea21,            ///< PRN 22 health MSBs
         h2nbHea22m = 6,
         h2scHea22 = 1,

         h2sbParity3 = h2sbHea22m+h2nbHea22m,         ///< Word 3 parity
         h2nbParity3 = 8,
         h2scParity3 = 1,

         h2sbHea22l = h2sbParity3+h2nbParity3,        ///< PRN 22 health LSBs
         h2nbHea22l = 3,

         h2sbHea23 = h2sbHea22l+h2nbHea22l,           ///< PRN 23 health
         h2nbHea23 = 9,
         h2scHea23 = 1,

         h2sbHea24 = h2sbHea23+h2nbHea23,             ///< PRN 24 health
         h2nbHea24 = 9,
         h2scHea24 = 1,

         h2sbHea25m = h2sbHea24+h2nbHea24,            ///< PRN 25 health MSB
         h2nbHea25m = 1,
         h2scHea25 = 1,

         h2sbParity4 = h2sbHea25m+h2nbHea25m,         ///< Word 4 parity
         h2nbParity4 = 8,
         h2scParity4 = 1,

         h2sbHea25l = h2sbParity4+h2nbParity4,        ///< PRN 25 health LSBs
         h2nbHea25l = 8,

         h2sbHea26 = h2sbHea25l+h2nbHea25l,           ///< PRN 26 health
         h2nbHea26 = 9,
         h2scHea26 = 1,

         h2sbHea27m = h2sbHea26+h2nbHea26,            ///< PRN 27 health MSBs
         h2nbHea27m = 5,
         h2scHea27 = 1,

         h2sbParity5 = h2sbHea27m+h2nbHea27m,         ///< Word 5 parity
         h2nbParity5 = 8,
         h2scParity5 = 1,

         h2sbHea27l = h2sbParity5+h2nbParity5,        ///< PRN 27 health LSBs
         h2nbHea27l = 4,

         h2sbHea28 = h2sbHea27l+h2nbHea27l,           ///< PRN 28 health
         h2nbHea28 = 9,
         h2scHea28 = 1,

         h2sbHea29 = h2sbHea28+h2nbHea28,             ///< PRN 29 health
         h2nbHea29 = 9,
         h2scHea29 = 1,

         h2sbParity6 = h2sbHea29+h2nbHea29,           ///< Word 6 parity
         h2nbParity6 = 8,
         h2scParity6 = 1,

         h2sbHea30 = h2sbParity6+h2nbParity6,         ///< PRN 30 health
         h2nbHea30 = 9,
         h2scHea30 = 1,

         h2sbWNa = h2sbHea30+h2nbHea30,               ///< Almanac week
         h2nbWNa = 8,
         h2scWNa = 1,

         h2sbtoam = h2sbWNa+h2nbWNa,                  ///< Almanac ref SOW MSBs
         h2nbtoam = 5,
         h2sctoa = 12,

         h2sbParity7 = h2sbtoam+h2nbtoam,             ///< Word 7 parity
         h2nbParity7 = 8,
         h2scParity7 = 1,

         h2sbtoal = h2sbParity7+h2nbParity7,          ///< Almanac ref SOW LSBs
         h2nbtoal = 3,
      };

         /** Start bits, bit counts and scale factor (*n for integer
          * quantities, *2^n for floating point quantities) for each of the
          * fields in subframe 5 page 7 (health).
          * Bit positions/sizes from BeiDou SIS-ICD, Fig 5-11-7
          */
      enum SF5Pg24BitInfo
      {
         h3sbHea31m = fsbPnum+fnbPnum,
         h3nbHea31m = 2,
         h3scHea31 = 1,

         h3sbParity2 = h3sbHea31m+h3nbHea31m,
         h3nbParity2 = 8,
         h3scParity2 = 1,

         h3sbHea31l = h3sbParity2+h3nbParity2,
         h3nbHea31l = 7,

         h3sbHea32 = h3sbHea31l+h3nbHea31l,
         h3nbHea32 = 9,
         h3scHea32 = 1,

         h3sbHea33m = h3sbHea32+h3nbHea32,
         h3nbHea33m = 6,
         h3scHea33 = 1,

         h3sbParity3 = h3sbHea33m+h3nbHea33m,
         h3nbParity3 = 8,
         h3scParity3 = 1,

         h3sbHea33l = h3sbParity3+h3nbParity3,
         h3nbHea33l = 3,

         h3sbHea34 = h3sbHea33l+h3nbHea33l,
         h3nbHea34 = 9,
         h3scHea34 = 1,

         h3sbHea35 = h3sbHea34+h3nbHea34,
         h3nbHea35 = 9,
         h3scHea35 = 1,

         h3sbHea36m = h3sbHea35+h3nbHea35,
         h3nbHea36m = 1,
         h3scHea36 = 1,

         h3sbParity4 = h3sbHea36m+h3nbHea36m,
         h3nbParity4 = 8,
         h3scParity4 = 1,

         h3sbHea36l = h3sbParity4+h3nbParity4,
         h3nbHea36l = 8,

         h3sbHea37 = h3sbHea36l+h3nbHea36l,
         h3nbHea37 = 9,
         h3scHea37 = 1,

         h3sbHea38m = h3sbHea37+h3nbHea37,
         h3nbHea38m = 5,
         h3scHea38 = 1,

         h3sbParity5 = h3sbHea38m+h3nbHea38m,
         h3nbParity5 = 8,
         h3scParity5 = 1,

         h3sbHea38l = h3sbParity5+h3nbParity5,
         h3nbHea38l = 4,

         h3sbHea39 = h3sbHea38l+h3nbHea38l,
         h3nbHea39 = 9,
         h3scHea39 = 1,

         h3sbHea40 = h3sbHea39+h3nbHea39,
         h3nbHea40 = 9,
         h3scHea40 = 1,

         h3sbParity6 = h3sbHea40+h3nbHea40,
         h3nbParity6 = 8,
         h3scParity6 = 1,

         h3sbHea41 = h3sbParity6+h3nbParity6,
         h3nbHea41 = 9,
         h3scHea41 = 1,

         h3sbHea42 = h3sbHea41+h3nbHea41,
         h3nbHea42 = 9,
         h3scHea42 = 1,

         h3sbHea43m = h3sbHea42+h3nbHea42,
         h3nbHea43m = 4,
         h3scHea43 = 1,

         h3sbParity7 = h3sbHea43m+h3nbHea43m,
         h3nbParity7 = 8,
         h3scParity7 = 1,

         h3sbHea43l = h3sbParity7+h3nbParity7,
         h3nbHea43l = 5,

         h3sbAmID = h3sbHea43l+h3nbHea43l,
         h3nbAmID = 2,
         h3scAmID = 1,
      };


         /** Start bits, bit counts and scale factor (*n for integer
          * quantities, *2^n for floating point quantities) for each of the
          * fields in subframe 5 pages 9 and 10 (time offset).
          * Bit positions/sizes from BeiDou SIS-ICD, Fig 5-11-4 and Fig 5-11-5.
          */
      enum TimeOffsetBitInfo
      {
         csbRev3 = fsbPnum+fnbPnum,                  ///< Reserved bits
         cnbRev3 = 2,
         cscRev3 = 1,

         csbParity2 = csbRev3+cnbRev3,             ///< Word 2 parity
         cnbParity2 = 8,
         cscParity2 = 1,

         csbRev4 = csbParity2+cnbParity2,          ///< Reserved bits
         cnbRev4 = 22,
         cscRev4 = 1,

         csbParity3 = csbRev4+cnbRev4,             ///< Word 3 parity
         cnbParity3 = 8,
         cscParity3 = 1,

         csbRev5 = csbParity3+cnbParity3,          ///< Reserved bits
         cnbRev5 = 6,
         cscRev5 = 1,

         csbA0GPS = csbRev5+cnbRev5,               ///< GPS A0 time offset term
         cnbA0GPS = 14,
         cscA0GPS = 0,                               ///< Scale factor is actually .1

         csbA1GPSm = csbA0GPS+cnbA0GPS,            ///< GPS A1 time offset term MSB
         cnbA1GPSm = 2,
         cscA1GPS = 0,                               ///< Scale factor is actually .1

         csbParity4 = csbA1GPSm+cnbA1GPSm,         ///< Word 4 parity
         cnbParity4 = 8,
         cscParity4 = 1,

         csbA1GPSl = csbParity4+cnbParity4,        ///< GPS A1 time offset term LSB
         cnbA1GPSl = 14,

         csbA0GALm = csbA1GPSl+cnbA1GPSl,          ///< Gal A0 time offset term MSB
         cnbA0GALm = 8,
         cscA0GAL = 0,                               ///< Scale factor is actually .1

         csbParity5 = csbA0GALm+cnbA0GALm,         ///< Word 5 parity
         cnbParity5 = 8,
         cscParity5 = 1,

         csbA0GALl = csbParity5+cnbParity5,        ///< Gal A0 time offset term LSB
         cnbA0GALl = 6,

         csbA1GAL = csbA0GALl+cnbA0GALl,           ///< Gal A1 time offset term
         cnbA1GAL = 16,
         cscA1GAL = 0,                               ///< Scale factor is actually .1

         csbParity6 = csbA1GAL+cnbA1GAL,           ///< Word 6 parity
         cnbParity6 = 8,
         cscParity6 = 1,

         csbA0GLO = csbParity6+cnbParity6,         ///< GLO A0 time offset term
         cnbA0GLO = 14,
         cscA0GLO = 0,                               ///< Scale factor is actually .1

         csbA1GLOm = csbA0GLO+cnbA0GLO,            ///< GLO A0 time offset term MSB
         cnbA1GLOm = 8,
         cscA1GLO = 0,                               ///< Scale factor is actually .1

         csbParity7 = csbA1GLOm+cnbA1GLOm,         ///< Word 7 parity
         cnbParity7 = 8,
         cscParity7 = 1,

         csbA1GLOl = csbParity7+cnbParity7,        ///< GLO A0 time offset term LSB
         cnbA1GLOl = 8,

            // page 10

         csbdtLSm = fsbPnum+fnbPnum,                 ///< dtLS MSBs
         cnbdtLSm = 2,
         cscdtLS = 1,

         csbdtLSl = csbParity2+cnbParity2,         ///< dtLS LSBs
         cnbdtLSl = 6,

         csbdtLSF = csbdtLSl+cnbdtLSl,             ///< dtLSF
         cnbdtLSF = 8,
         cscdtLSF = 1,

         csbWNlsf = csbdtLSF+cnbdtLSF,             ///< WNlsf
         cnbWNlsf = 8,
         cscWNlsf = 1,

         csbA0UTCm = csbParity3+cnbParity3,        ///< UTC A0 time offset term MSB
         cnbA0UTCm = 22,
         cscA0UTC = -30,

         csbA0UTCl = csbParity4+cnbParity4,        ///< UTC A0 time offset term LSB
         cnbA0UTCl = 10,

         csbA1UTCm = csbA0UTCl+cnbA0UTCl,          ///< UTC A1 time offset term MSB
         cnbA1UTCm = 12,
         cscA1UTC = -50,

         csbA1UTCl = csbParity5+cnbParity5,        ///< UTC A1 time offset term LSB
         cnbA1UTCl = 12,

         csbDN = csbA1UTCl+cnbA1UTCl,              ///< DN
         cnbDN = 8,
         cscDN = 1,
      };
   } // namespace BDSD1Bits
} // namespace gpstk

#endif // GPSTK_BDSD1BITS_HPP
