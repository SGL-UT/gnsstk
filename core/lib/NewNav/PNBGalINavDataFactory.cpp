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
#include "PNBGalINavDataFactory.hpp"
#include "GalINavEph.hpp"
#include "GalINavTimeOffset.hpp"
#include "GalINavIono.hpp"
#include "GalINavISC.hpp"
#include "GALWeekSecond.hpp"
#include "TimeCorrection.hpp"
#include "EngNav.hpp"
#include "TimeString.hpp"

using namespace std;

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

namespace gnsstk
{
   bool PNBGalINavDataFactory ::
   addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
           double cadence)
   {
      if ((navIn->getNavID().navType != NavType::GalINAV) ||
          (navIn->getNumBits() != 128))
      {
            // This class only processes Galileo I/NAV.
         return false;
      }
      bool rv = true;
      try
      {
         unsigned long wordType = navIn->asUnsignedLong(
            fsbType,fnbType,fscType);
            /// @todo implement validity checks if there are any.
         switch (wordType)
         {
               /** @note While the ICD doesn't label word type 5 as
                * being part of the ephemeris, there is data in word
                * type 5 that is required to construct the
                * ephemeris. */
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
               // cerr << "wordType " << wordType << " = ephemeris" << endl;
               rv = processEph(wordType, navIn, navOut);
               break;
            case 6:
               rv = processOffset(navIn, navOut);
               break;
            case 7:
            case 8:
            case 9:
            case 10:
               // cerr << "wordType " << wordType << " = almanac" << endl;
               rv = processAlm(wordType, navIn, navOut);
               break;
            default:
               // cerr << "wordType " << wordType << " = unhandled" << endl;
               rv = true;
               break;
         }
         // cerr << "  results: " << navOut.size() << endl;
         // for (const auto& i : navOut)
         //    i->dump(cerr,DumpDetail::Full);
      }
      catch (Exception& exc)
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


   bool PNBGalINavDataFactory ::
   processEph(unsigned wordType, const PackedNavBitsPtr& navIn,
              NavDataPtrList& navOut)
   {
      NavSatelliteID key(navIn->getsatSys().id, navIn->getsatSys(),
                         navIn->getobsID(), navIn->getNavID());
      if (!PNBNavDataFactory::processEph && !PNBNavDataFactory::processHea)
      {
            // User doesn't want ephemerides or health so don't do any
            // processing.
         return true;
      }
      if (ephAcc.find(key) == ephAcc.end())
      {
         // cerr << "ephAcc is empty for signal " << key << endl;
            // set up a new ephemeris
         ephAcc[key].resize(numEphWTs);
         ephAcc[key][wordType-1] = navIn;
            // nothing in navOut yet and no further processing because
            // we only have one of three subframes at this point.
         return true;
      }
      std::vector<PackedNavBitsPtr> &ephWord(ephAcc[key]);
      ephWord[wordType-1] = navIn;
      if (wordType == 5)
      {
         if (PNBNavDataFactory::processIono)
         {
               // Add iono data from word type 5
            NavDataPtr p3 = std::make_shared<GalINavIono>();
            GalINavIono *ip3 = dynamic_cast<GalINavIono*>(p3.get());
            ip3->timeStamp = navIn->getTransmitTime();
            ip3->signal = NavMessageID(key, NavMessageType::Iono);
            ip3->ai[0] = navIn->asUnsignedDouble(isbai0,inbai0,iscai0);
            ip3->ai[1] = navIn->asSignedDouble(isbai1,inbai1,iscai1);
            ip3->ai[2] = navIn->asSignedDouble(isbai2,inbai2,iscai2);
            ip3->idf[0] = navIn->asBool(isbIDFR1);
            ip3->idf[1] = navIn->asBool(isbIDFR2);
            ip3->idf[2] = navIn->asBool(isbIDFR3);
            ip3->idf[3] = navIn->asBool(isbIDFR4);
            ip3->idf[4] = navIn->asBool(isbIDFR5);
            navOut.push_back(p3);
         }
         if (PNBNavDataFactory::processISC)
         {
               // Add ISC data from word type 5
            NavDataPtr p4 = std::make_shared<GalINavISC>();
            GalINavISC *ip4 = dynamic_cast<GalINavISC*>(p4.get());
            ip4->timeStamp = navIn->getTransmitTime();
            ip4->signal = NavMessageID(key, NavMessageType::ISC);
            ip4->isc = std::numeric_limits<double>::quiet_NaN();
            ip4->bgdE1E5a = navIn->asSignedDouble(isbBGDa,inbBGDa,iscBGDa);
            ip4->bgdE1E5b = navIn->asSignedDouble(isbBGDb,inbBGDb,iscBGDb);
            navOut.push_back(p4);
         }
            // Health information is in word type 5, but we also need
            // word type 3 for SISA.
         if (PNBNavDataFactory::processHea && ephWord[wt3])
         {
               // Add health bits from word type 5.
            NavDataPtr p1 = std::make_shared<GalINavHealth>();
            GalINavHealth *hp1 = dynamic_cast<GalINavHealth*>(p1.get());
            hp1->timeStamp = navIn->getTransmitTime();
            hp1->signal = NavMessageID(key, NavMessageType::Health);
            hp1->signal.obs.band = CarrierBand::E5b;
            hp1->signal.obs.code = TrackingCode::E5bI;
            hp1->sigHealthStatus = static_cast<GalHealthStatus>(
               ephWord[isiE5bhs]->asUnsignedLong(isbE5bhs,inbE5bhs,iscE5bhs));
            hp1->dataValidityStatus = static_cast<GalDataValid>(
               ephWord[isiE5bdvs]->asUnsignedLong(
                  isbE5bdvs,inbE5bdvs,iscE5bdvs));
            hp1->sisaIndex = ephWord[esiSISA]->asUnsignedLong(esbSISA,enbSISA,
                                                              escSISA);
            NavDataPtr p2 = std::make_shared<GalINavHealth>();
            GalINavHealth *hp2 = dynamic_cast<GalINavHealth*>(p2.get());
            *hp2 = *hp1; // copy data
            hp2->signal.obs.band = CarrierBand::L1;
            hp2->signal.obs.code = TrackingCode::E1B;
            hp2->sigHealthStatus = static_cast<GalHealthStatus>(
               ephWord[isiE1Bhs]->asUnsignedLong(isbE1Bhs,inbE1Bhs,iscE1Bhs));
            hp2->dataValidityStatus = static_cast<GalDataValid>(
               ephWord[isiE1Bdvs]->asUnsignedLong(
                  isbE1Bdvs,inbE1Bdvs,iscE1Bdvs));
               // reset is the same as p1 and already copied
            navOut.push_back(p1);
            navOut.push_back(p2);
         }
      }
      if (!PNBNavDataFactory::processEph)
      {
            // User doesn't want ephemerides so don't do any further
            // processing.
         return true;
      }
         // stop processing if we don't have three full subframes
      if (!ephWord[wt1] || !ephWord[wt2] || !ephWord[wt3] || !ephWord[wt4] ||
           !ephWord[wt5] ||
          (ephWord[wt1]->getNumBits() != 128) ||
          (ephWord[wt2]->getNumBits() != 128) ||
          (ephWord[wt3]->getNumBits() != 128) ||
          (ephWord[wt4]->getNumBits() != 128) ||
          (ephWord[wt5]->getNumBits() != 128))
      {
         // cerr << "Not ready for full I/NAV eph processing" << endl
         //      << "  wt1: " << (bool)ephWord[wt1] << " "
         //      << (ephWord[wt1] ? ephWord[wt1]->getNumBits() : -1) << endl
         //      << "  wt2: " << (bool)ephWord[wt2] << " "
         //      << (ephWord[wt2] ? ephWord[wt2]->getNumBits() : -1) << endl
         //      << "  wt3: " << (bool)ephWord[wt3] << " "
         //      << (ephWord[wt3] ? ephWord[wt3]->getNumBits() : -1) << endl
         //      << "  wt4: " << (bool)ephWord[wt4] << " "
         //      << (ephWord[wt4] ? ephWord[wt4]->getNumBits() : -1) << endl
         //      << "  wt5: " << (bool)ephWord[wt5] << " "
         //      << (ephWord[wt5] ? ephWord[wt5]->getNumBits() : -1) << endl;
         return true;
      }
         // Stop processing if we don't have matching IODnav in each
         // of the four word types.
      unsigned long iod1, iod2, iod3, iod4;
      iod1 = ephWord[wt1]->asUnsignedLong(esbIOD,enbIOD,escIOD);
      iod2 = ephWord[wt2]->asUnsignedLong(esbIOD,enbIOD,escIOD);
      iod3 = ephWord[wt3]->asUnsignedLong(esbIOD,enbIOD,escIOD);
      iod4 = ephWord[wt4]->asUnsignedLong(esbIOD,enbIOD,escIOD);
         /** @todo Word type 5 is not officially part of the ephemeris
          * and doesn't contain an IODnav field, so how do we make
          * sure that our word type 5 is usable with the ephemeris? */
      if ((iod1 != iod2) || (iod1 != iod3) || (iod1 != iod4))
      {
         // cerr << "IODnav mismatch, not processing" << endl;
            // Even though the mismatch might be considered an error,
            // we don't really want to mark it as such and rather
            // consider it as a "valid" but unprocessable data set.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GalINavEph>();
      GalINavEph *eph = dynamic_cast<GalINavEph*>(p0.get());
         // NavData
      eph->timeStamp = ephWord[wt1]->getTransmitTime();
      eph->signal = NavMessageID(key, NavMessageType::Ephemeris);
      // cerr << "Ready for full I/NAV eph processing for " << (NavSignalID)key << endl;
         // OrbitData = empty
         // OrbitDataKepler
      eph->xmitTime = eph->timeStamp;
      double t0e = ephWord[esit0e]->asUnsignedLong(esbt0e,enbt0e,esct0e);
      double t0c = ephWord[esit0c]->asUnsignedLong(esbt0c,enbt0c,esct0c);
      unsigned wn = ephWord[isiWN]->asUnsignedLong(isbWN,inbWN,iscWN);
      unsigned tow = ephWord[isiTOW]->asUnsignedLong(isbTOW,inbTOW,iscTOW);
      // cerr << "  wn=" << wn << "  tow=" << tow << "  t0e=" << t0e << "  t0c=" << t0c << endl;
      GALWeekSecond xmit(wn, tow);
      eph->Toe = GALWeekSecond(wn,t0e).weekRolloverAdj(xmit);
      eph->Toc = GALWeekSecond(wn,t0c).weekRolloverAdj(xmit);
         // health is set below
      eph->Cuc = ephWord[esiCuc]->asSignedDouble(esbCuc,enbCuc,escCuc);
      eph->Cus = ephWord[esiCus]->asSignedDouble(esbCus,enbCus,escCus);
      eph->Crc = ephWord[esiCrc]->asSignedDouble(esbCrc,enbCrc,escCrc);
      eph->Crs = ephWord[esiCrs]->asSignedDouble(esbCrs,enbCrs,escCrs);
      eph->Cic = ephWord[esiCic]->asSignedDouble(esbCic,enbCic,escCic);
      eph->Cis = ephWord[esiCis]->asSignedDouble(esbCis,enbCis,escCis);
      eph->M0  = ephWord[esiM0]->asDoubleSemiCircles(esbM0,enbM0,escM0);
      eph->dn  = ephWord[esidn]->asDoubleSemiCircles(esbdn,enbdn,escdn);
         // no dndot in I/NAV
      eph->ecc = ephWord[esiEcc]->asUnsignedDouble(esbEcc,enbEcc,escEcc);
      eph->Ahalf = ephWord[esiAhalf]->asUnsignedDouble(esbAhalf,enbAhalf,
                                                       escAhalf);
      eph->A = eph->Ahalf * eph->Ahalf;
         // no Adot in I/NAV
      eph->OMEGA0 = ephWord[esiOMEGA0]->asDoubleSemiCircles(esbOMEGA0,enbOMEGA0,
                                                            escOMEGA0);
      eph->i0 = ephWord[esii0]->asDoubleSemiCircles(esbi0,enbi0,esci0);
      eph->w = ephWord[esiw]->asDoubleSemiCircles(esbw,enbw,escw);
      eph->OMEGAdot = ephWord[esiOMEGAdot]->asDoubleSemiCircles(
         esbOMEGAdot,enbOMEGAdot,escOMEGAdot);
      eph->idot = ephWord[esiidot]->asDoubleSemiCircles(esbidot,enbidot,
                                                        escidot);
      eph->af0 = ephWord[esiaf0]->asSignedDouble(esbaf0,enbaf0,escaf0);
      eph->af1 = ephWord[esiaf1]->asSignedDouble(esbaf1,enbaf1,escaf1);
      eph->af2 = ephWord[esiaf2]->asSignedDouble(esbaf2,enbaf2,escaf2);
         // GalINavEph
      eph->bgdE5aE1 = ephWord[isiBGDa]->asSignedDouble(isbBGDa,inbBGDa,iscBGDa);
      eph->bgdE5bE1 = ephWord[isiBGDb]->asSignedDouble(isbBGDb,inbBGDb,iscBGDb);
      eph->sisaIndex = ephWord[esiSISA]->asUnsignedLong(esbSISA,enbSISA,
                                                        escSISA);
      eph->svid = ephWord[esiSVID]->asUnsignedLong(esbSVID,enbSVID,escSVID);
      eph->xmit2 = ephWord[wt2]->getTransmitTime();
      eph->xmit3 = ephWord[wt3]->getTransmitTime();
      eph->xmit4 = ephWord[wt4]->getTransmitTime();
      eph->xmit5 = ephWord[wt5]->getTransmitTime();
      eph->iodnav1 = iod1;
      eph->iodnav2 = iod2;
      eph->iodnav3 = iod3;
      eph->iodnav4 = iod4;
      eph->hsE5b = static_cast<GalHealthStatus>(
         ephWord[isiE5bhs]->asUnsignedLong(isbE5bhs,inbE5bhs,iscE5bhs));
      eph->hsE1B = static_cast<GalHealthStatus>(
         ephWord[isiE1Bhs]->asUnsignedLong(isbE1Bhs,inbE1Bhs,iscE1Bhs));
      eph->dvsE5b = static_cast<GalDataValid>(
         ephWord[isiE5bdvs]->asUnsignedLong(isbE5bdvs,inbE5bdvs,iscE5bdvs));
      eph->dvsE1B = static_cast<GalDataValid>(
         ephWord[isiE1Bdvs]->asUnsignedLong(isbE1Bdvs,inbE1Bdvs,iscE1Bdvs));
         // set health using the Galileo algorithms.
      if (eph->signal.obs.band == gnsstk::CarrierBand::L1)
      {
            // E1B
         eph->health = GalINavHealth::galHealth(eph->hsE1B,eph->dvsE1B,
                                               eph->sisaIndex);
      }
      else
      {
            // E5b
         eph->health = GalINavHealth::galHealth(eph->hsE5b,eph->dvsE5b,
                                               eph->sisaIndex);
      }
      eph->fixFit();
      // cerr << "add I/NAV eph" << endl;
      navOut.push_back(p0);
         // Clear out the broadcast ephemeris that's been processed.
      ephAcc.erase(key);
      return true;
   }


   bool PNBGalINavDataFactory ::
   processAlm(unsigned wordType, const PackedNavBitsPtr& navIn,
              NavDataPtrList& navOut)
   {
      NavSatelliteID key(navIn->getsatSys().id, navIn->getsatSys(),
                         navIn->getobsID(), navIn->getNavID());
      if (!PNBNavDataFactory::processAlm && !PNBNavDataFactory::processHea &&
          !PNBNavDataFactory::processTim)
      {
            // User doesn't want almanacs or health so don't do any
            // processing.
         return true;
      }
         /** @todo Matthew Koenn suggested effectively caching results
          * for almAcc look-ups.  Probably can't take the time to dig
          * into it right now, but wanted to leave a reference to the
          * comment here.
          * https://sgl-git.arlut.utexas.edu/sgl-tks/gnsstk/-/merge_requests/523#note_524670
          */
      if (almAcc.find(key) == almAcc.end())
      {
         // cerr << "almAcc is empty for signal " << key << endl;
            // set up a new almanac
         almAcc[key].resize(numAlmWTs);
         almAcc[key][wordType-7] = navIn;
            // nothing in navOut yet and no further processing because
            // we only have one of three subframes at this point.
         return true;
      }
      std::vector<PackedNavBitsPtr> &almWord(almAcc[key]);
      almWord[wordType-7] = navIn;
      if ((wordType == 10) && PNBNavDataFactory::processTim)
      {
         NavDataPtr p3 = std::make_shared<GalINavTimeOffset>();
         p3->timeStamp = navIn->getTransmitTime();
         p3->signal = NavMessageID(
            NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                           navIn->getobsID(), navIn->getNavID()),
            NavMessageType::TimeOffset);
         GalINavTimeOffset *to = dynamic_cast<GalINavTimeOffset*>(p3.get());
         to->tgt = TimeSystem::GPS;
         to->a0 = almWord[asiA0G]->asSignedDouble(asbA0G,anbA0G,ascA0G);
         to->a1 = almWord[asiA1G]->asSignedDouble(asbA1G,anbA1G,ascA1G);
         to->tot = almWord[asit0G]->asUnsignedLong(asbt0G,anbt0G,asct0G);
         to->wnot = almWord[asiWN0G]->asUnsignedLong(asbWN0G,anbWN0G,ascWN0G);
            // Not sure if this is appropriate but it beats saving the
            // 12-bit WN from word type 5.
         GALWeekSecond gws(to->timeStamp);
            // WN0G is 6 bits...
         gws.week = (gws.week & ~0x3f) | to->wnot;
         gws.sow = to->tot;
         to->refTime = gws;
         // cerr << "gws.week=" << gws.week << "  gws.sow=" << gws.sow << "  refTime=" << to->refTime << endl;
         navOut.push_back(p3);
      }
         // stop processing if we don't have three full subframes
      if (!almWord[wt7] || !almWord[wt8] || !almWord[wt9] || !almWord[wt10] ||
          (almWord[wt7]->getNumBits() != 128) ||
          (almWord[wt8]->getNumBits() != 128) ||
          (almWord[wt9]->getNumBits() != 128) ||
          (almWord[wt10]->getNumBits() != 128))
      {
         // cerr << "Not ready for full I/NAV alm processing" << endl
         //      << "  wt7: " << (bool)almWord[wt7] << " "
         //      << (almWord[wt7] ? almWord[wt7]->getNumBits() : -1) << endl
         //      << "  wt8: " << (bool)almWord[wt8] << " "
         //      << (almWord[wt8] ? almWord[wt8]->getNumBits() : -1) << endl
         //      << "  wt9: " << (bool)almWord[wt9] << " "
         //      << (almWord[wt9] ? almWord[wt9]->getNumBits() : -1) << endl
         //      << "  wt10: " << (bool)almWord[wt10] << " "
         //      << (almWord[wt10] ? almWord[wt10]->getNumBits() : -1) << endl;
         return true;
      }
         // Stop processing if we don't have matching IODa in each of
         // the four word types.
      unsigned long ioda1, ioda2, ioda3, ioda4;
      ioda1 = almWord[wt7]->asUnsignedLong(asbIODa,anbIODa,ascIODa);
      ioda2 = almWord[wt8]->asUnsignedLong(asbIODa,anbIODa,ascIODa);
      ioda3 = almWord[wt9]->asUnsignedLong(asbIODa,anbIODa,ascIODa);
      ioda4 = almWord[wt10]->asUnsignedLong(asbIODa,anbIODa,ascIODa);
      if ((ioda1 != ioda2) || (ioda1 != ioda3) || (ioda1 != ioda4))
      {
         // cerr << "IODa mismatch, not processing: " << ioda1 << " " << ioda2 << " " << ioda3 << " " << ioda4 << endl;
            // Even though the mismatch might be considered an error,
            // we don't really want to mark it as such and rather
            // consider it as a "valid" but unprocessable data set.
         return true;
      }
         // SVID1
      NavDataPtr p0 = std::make_shared<GalINavAlm>();
      NavDataPtr p1 = std::make_shared<GalINavHealth>();
      NavDataPtr p2 = std::make_shared<GalINavHealth>();
      GalINavAlm *alm = dynamic_cast<GalINavAlm*>(p0.get());
      GalINavHealth *hp1 = dynamic_cast<GalINavHealth*>(p1.get());
      GalINavHealth *hp2 = dynamic_cast<GalINavHealth*>(p2.get());
      if (processAlmOrb(almWord, alm, hp1, hp2, wt7, wt8, asiWNa_A, asit0a_A,
                        asiSVID_1, asbSVID_1, asidAhalf_1, asbdAhalf_1,
                        asiEcc_1, asbEcc_1, asiw_1, asbw_1, asidi_1, asbdi_1,
                        asiOMEGA0_1, asbOMEGA0_1, asiOMEGAdot_1, asbOMEGAdot_1,
                        asiM0_1, asbM0_1, asiaf0_1, asbaf0_1, asiaf1_1,
                        asbaf1_1, asiE5bhs_1, asbE5bhs_1, asiE1Bhs_1,
                        asbE1Bhs_1))
      {
         if (PNBNavDataFactory::processAlm)
         {
            // cerr << "add I/NAV alm SVID1" << endl;
            alm->ioda1 = ioda1;
            alm->ioda2 = ioda2;
            navOut.push_back(p0);
         }
         if (PNBNavDataFactory::processHea)
         {
            navOut.push_back(p1);
            navOut.push_back(p2);
         }
      }
         // SVID2
      p0 = std::make_shared<GalINavAlm>();
      p1 = std::make_shared<GalINavHealth>();
      p2 = std::make_shared<GalINavHealth>();
      alm = dynamic_cast<GalINavAlm*>(p0.get());
      hp1 = dynamic_cast<GalINavHealth*>(p1.get());
      hp2 = dynamic_cast<GalINavHealth*>(p2.get());
      if (processAlmOrb(almWord, alm, hp1, hp2, wt8, wt9, asiWNa_B, asit0a_B,
                        asiSVID_2, asbSVID_2, asidAhalf_2, asbdAhalf_2,
                        asiEcc_2, asbEcc_2, asiw_2, asbw_2, asidi_2, asbdi_2,
                        asiOMEGA0_2, asbOMEGA0_2, asiOMEGAdot_2, asbOMEGAdot_2,
                        asiM0_2, asbM0_2, asiaf0_2, asbaf0_2, asiaf1_2,
                        asbaf1_2, asiE5bhs_2, asbE5bhs_2, asiE1Bhs_2,
                        asbE1Bhs_2))
      {
         if (PNBNavDataFactory::processAlm)
         {
            // cerr << "add I/NAV alm SVID2" << endl;
            alm->ioda1 = ioda2;
            alm->ioda2 = ioda3;
            navOut.push_back(p0);
         }
         if (PNBNavDataFactory::processHea)
         {
            navOut.push_back(p1);
            navOut.push_back(p2);
         }
      }
         // SVID3
      p0 = std::make_shared<GalINavAlm>();
      p1 = std::make_shared<GalINavHealth>();
      p2 = std::make_shared<GalINavHealth>();
      alm = dynamic_cast<GalINavAlm*>(p0.get());
      hp1 = dynamic_cast<GalINavHealth*>(p1.get());
      hp2 = dynamic_cast<GalINavHealth*>(p2.get());
      if (processAlmOrb(almWord, alm, hp1, hp2, wt9, wt10, asiWNa_B, asit0a_B,
                        asiSVID_3, asbSVID_3, asidAhalf_3, asbdAhalf_3,
                        asiEcc_3, asbEcc_3, asiw_3, asbw_3, asidi_3, asbdi_3,
                        asiOMEGA0_3, asbOMEGA0_3, asiOMEGAdot_3, asbOMEGAdot_3,
                        asiM0_3, asbM0_3, asiaf0_3, asbaf0_3, asiaf1_3,
                        asbaf1_3, asiE5bhs_3, asbE5bhs_3, asiE1Bhs_3,
                        asbE1Bhs_3))
      {
         if (PNBNavDataFactory::processAlm)
         {
            // cerr << "add I/NAV alm SVID3" << endl;
            alm->ioda1 = ioda3;
            alm->ioda2 = ioda4;
            navOut.push_back(p0);
         }
         if (PNBNavDataFactory::processHea)
         {
            navOut.push_back(p1);
            navOut.push_back(p2);
         }
      }
         // Clear out the broadcast almanac that's been processed.
      almAcc.erase(key);
      return true;
   }


   bool PNBGalINavDataFactory ::
   processAlmOrb(const std::vector<PackedNavBitsPtr>& almWord,
                 GalINavAlm *alm, GalINavHealth *hp1, GalINavHealth *hp2,
                 int wtA, int wtB,
                 int asiWNa, int asit0a,
                 int asiSVID, int asbSVID,
                 int asidAhalf, int asbdAhalf,
                 int asiEcc, int asbEcc,
                 int asiw, int asbw,
                 int asidi, int asbdi,
                 int asiOMEGA0, int asbOMEGA0,
                 int asiOMEGAdot, int asbOMEGAdot,
                 int asiM0, int asbM0,
                 int asiaf0, int asbaf0,
                 int asiaf1, int asbaf1,
                 int asiE5bhs, int asbE5bhs,
                 int asiE1Bhs, int asbE1Bhs)
   {
         // NavData
      alm->timeStamp = almWord[wtA]->getTransmitTime();
      unsigned long svid = almWord[asiSVID]->asUnsignedLong(asbSVID,anbSVID,
                                                            ascSVID);
      if (svid == 0)
         return false; // empty almanac, do nothing.
      SatID xmitSat(almWord[wtA]->getsatSys());
      NavSatelliteID sat(svid, xmitSat, almWord[wtA]->getobsID(),
                         almWord[wtA]->getNavID());
      alm->signal = NavMessageID(sat, NavMessageType::Almanac);
      // cerr << "Ready for full I/NAV alm processing for " << (NavSignalID)sat << endl;
         // OrbitData = empty
         // OrbitDataKepler
      alm->xmitTime = alm->timeStamp;
      alm->xmit2 = almWord[wtB]->getTransmitTime();
      alm->t0a = almWord[asit0a]->asUnsignedLong(asbt0a,anbt0a,asct0a);
      alm->wna = almWord[asiWNa]->asUnsignedLong(asbWNa,anbWNa,ascWNa);
         // Not sure if this is appropriate but it beats saving the
         // 12-bit WN from word type 5.
      GALWeekSecond gws(alm->xmitTime);
         // WNa is 2 bits...
      gws.week = (gws.week & ~0x03) | alm->wna;
      gws.sow = alm->t0a;
      alm->Toe = alm->Toc = gws;
      // cerr << "  wna=" << alm->wna << "  tow=" << gws.sow << "  t0a=" << alm->t0a << "  Toe=" << printTime(alm->Toe, "%L %g") << "  xmit=" << printTime(alm->xmitTime, "%L %g") << endl;
      alm->wna = gws.week;
      alm->M0  = almWord[asiM0]->asDoubleSemiCircles(asbM0,anbM0,ascM0);
      alm->ecc = almWord[asiEcc]->asUnsignedDouble(asbEcc,anbEcc,ascEcc);
      alm->dAhalf = almWord[asidAhalf]->asUnsignedDouble(asbdAhalf,anbdAhalf,
                                                         ascdAhalf);
         // slightly annoying that the reference value is for A but
         // the transmitted data is A**1/2.
      alm->Ahalf = ::sqrt(GalINavAlm::refA) + alm->dAhalf;
      alm->A = alm->Ahalf * alm->Ahalf;
      alm->OMEGA0 = almWord[asiOMEGA0]->asDoubleSemiCircles(asbOMEGA0,anbOMEGA0,
                                                            ascOMEGA0);
      alm->deltai = almWord[asidi]->asDoubleSemiCircles(asbdi,anbdi,ascdi);
      alm->i0 = GalINavAlm::refioffset + alm->deltai;
      alm->w = almWord[asiw]->asDoubleSemiCircles(asbw,anbw,ascw);
      alm->OMEGAdot = almWord[asiOMEGAdot]->asDoubleSemiCircles(
         asbOMEGAdot,anbOMEGAdot,ascOMEGAdot);
      alm->af0 = almWord[asiaf0]->asSignedDouble(asbaf0,anbaf0,ascaf0);
      alm->af1 = almWord[asiaf1]->asSignedDouble(asbaf1,anbaf1,ascaf1);
         // GalINavAlm
      alm->hsE5b = static_cast<GalHealthStatus>(
         almWord[asiE5bhs]->asUnsignedLong(asbE5bhs,anbE5bhs,ascE5bhs));
      alm->hsE1B = static_cast<GalHealthStatus>(
         almWord[asiE1Bhs]->asUnsignedLong(asbE1Bhs,anbE1Bhs,ascE1Bhs));
      alm->fixFit();
      alm->fixHealth();
      if (PNBNavDataFactory::processHea)
      {
         hp1->timeStamp = almWord[asiE5bhs]->getTransmitTime();
         hp1->signal = NavMessageID(sat, NavMessageType::Health);
         hp1->signal.obs.band = CarrierBand::E5b;
         hp1->signal.obs.code = TrackingCode::E5bI;
         hp1->sigHealthStatus = static_cast<GalHealthStatus>(
            almWord[asiE5bhs]->asUnsignedLong(asbE5bhs,anbE5bhs,ascE5bhs));
            /// @todo figure out what DVS and SISA should be here.
         hp2->timeStamp = almWord[asiE1Bhs]->getTransmitTime();
         hp2->signal = NavMessageID(sat, NavMessageType::Health);
         hp2->signal.obs.band = CarrierBand::L1;
         hp2->signal.obs.code = TrackingCode::E1B;
         hp2->sigHealthStatus = static_cast<GalHealthStatus>(
            almWord[asiE1Bhs]->asUnsignedLong(asbE1Bhs,anbE1Bhs,ascE1Bhs));
            /// @todo figure out what DVS and SISA should be here.
      }
      return true;
   }


   bool PNBGalINavDataFactory ::
   processOffset(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
         // No checks for correct svid, just assume that the input
         // data has already been checked (it will have been by
         // addData).
      if (!PNBNavDataFactory::processTim)
      {
            // User doesn't want time offset data so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GalINavTimeOffset>();
      p0->timeStamp = navIn->getTransmitTime();
      p0->signal = NavMessageID(
         NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                        navIn->getobsID(), navIn->getNavID()),
         NavMessageType::TimeOffset);
      GalINavTimeOffset *to = dynamic_cast<GalINavTimeOffset*>(p0.get());
      to->tgt = TimeSystem::UTC;
      to->a0 = navIn->asSignedDouble(csbA0, cnbA0, cscA0);
      to->a1 = navIn->asSignedDouble(csbA1, cnbA1, cscA1);
      to->deltatLS = navIn->asLong(csbdtLS, cnbdtLS, cscdtLS);
      to->tot = navIn->asUnsignedLong(csbtot, cnbtot, csctot);
      to->wnot = navIn->asUnsignedLong(csbWNot, cnbWNot, cscWNot);
      to->wnLSF = navIn->asUnsignedLong(csbWNlsf, cnbWNlsf, cscWNlsf);
      to->dn = navIn->asUnsignedLong(csbDN, cnbDN, cscDN);
      to->deltatLSF = navIn->asLong(csbdtLSF, cnbdtLSF, cscdtLSF);
      to->tow = navIn->asUnsignedLong(csbTOW, cnbTOW, cscTOW);
         // adjust week numbers to full week
      GALWeekSecond ws(p0->timeStamp);
      long refWeek = ws.week;
      to->wnot = timeAdjust8BitWeekRollover(to->wnot, refWeek);
      to->wnLSF = timeAdjust8BitWeekRollover(to->wnLSF, refWeek);
      to->refTime = GALWeekSecond(to->wnot, to->tot);
      to->effTime = GALWeekSecond(to->wnLSF, (to->dn-1)*86400);
         // return results.
      // cerr << "add I/NAV word type 6 time offset" << endl;
      navOut.push_back(p0);
      return true;
   }


#if 0
   void PNBGalINavDataFactory ::
   dumpState(std::ostream& s)
      const
   {
      s << "fullWNaMap.size() = " << fullWNaMap.size() << " (expect 32)" << endl
        << "almAcc.size() = " << almAcc.size() << " (expect 32)" << endl;
      for (const auto& i : almAcc)
      {
         s << "  almAcc[" << i.first << "].size() = " << i.second.size()
           << endl;
      }
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
#endif


   void PNBGalINavDataFactory ::
   resetState()
   {
      almAcc.clear();
      ephAcc.clear();
   }
} // namespace gnsstk
