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
#include "PNBGPSLNavDataFactory.hpp"
#include "GPSLNavAlm.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavHealth.hpp"
#include "GPSLNavTimeOffset.hpp"
#include "GPSLNavIono.hpp"
#include "GPSLNavISC.hpp"
#include "TimeCorrection.hpp"
#include "EngNav.hpp"

using namespace std;

/** Start bits, bit counts and scale factor (*n for integer
 * quantities, *2^n for floating point quantities) for fields that
 * apply to all messages. */
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

   fsbAlert = 47, ///< Alert flag start bit
   fnbAlert = 1,  ///< Alert flag number of bits
   fscAlert = 1,  ///< Alert flag scale factor

   fsbAS = 48, ///< Anti-spoof flag start bit
   fnbAS = 1,  ///< Anti-spoof flag number of bits
   fscAS = 1,  ///< Anti-spoof flag scale factor
};

// Miscellaneous constants.  Using enums instead of constants because it
// doesn't use any memory that way.
enum MiscConst
{
   dataIDGPSonQZSS = 0, ///< Neither GPS nor QZSS use this any longer.
   dataIDGPS = 1,       ///< Standard GPS data ID.
   dataIDQZSS = 3,      ///< QZSS almanacs for QZSS satellites.
};

// This enum is only used here and it's being used to avoid confusion
// on vector indices.  Using enums instead of constants because it
// doesn't use any memory that way.  Also not using strongly typed
// enums because we actually want these values to be implicitly cast
// to integer values.
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
   esiWN = sf1, ///< WN subframe index
   esbWN = 60,  ///< WN start bit
   enbWN = 10,  ///< WN number of bits
   escWN = 1,   ///< WN scale factor

   esiL2 = sf1, ///< L2 codes subframe index
   esbL2 = 70,  ///< L2 codes start bit
   enbL2 = 2,   ///< L2 codes number of bits
   escL2 = 1,   ///< L2 codes scale factor

   esiURA = sf1, ///< URA index subframe index
   esbURA = 72,  ///< URA index start bit
   enbURA = 4,   ///< URA index number of bits
   escURA = 1,   ///< URA index scale factor

   esiHea = sf1, ///< SV health subframe index
   esbHea = 76,  ///< SV health start bit
   enbHea = 6,   ///< SV health number of bits
   escHea = 1,   ///< SV health scale factor

   esiIODC  = sf1, ///< IODC subframe index
   esbIODCm = 82,  ///< IODC MSBs start bit
   enbIODCm = 2,   ///< IODC MSBs number of bits
   escIODC = 1,    ///< IODC scale factor

   esiL2P = sf1, ///< L2 P data flag subframe index
   esbL2P = 90,  ///< L2 P data flag start bit
   enbL2P = 1,   ///< L2 P data flag number of bits
   escL2P = 1,   ///< L2 P data flag scale factor

   esiTGD = sf1, ///< Tgd subframe index
   esbTGD = 196, ///< Tgd start bit
   enbTGD = 8,   ///< Tgd number of bits
   escTGD = -31, ///< Tgd scale factor

   esbIODCl = 210, ///< IODC LSBs start bit
   enbIODCl = 8,   ///< IODC LSBs number of bits

   esitoc = sf1, ///< toc subframe index
   esbtoc = 218, ///< toc start bit
   enbtoc = 16,  ///< toc number of bits
   esctoc = 4,   ///< toc scale factor

   esiaf2 = sf1, ///< af2 subframe index
   esbaf2 = 240, ///< af2 start bit
   enbaf2 = 8,   ///< af2 number of bits
   escaf2 = -55, ///< af2 scale factor

   esiaf1 = sf1, ///< af1 subframe index
   esbaf1 = 248, ///< af1 start bit
   enbaf1 = 16,  ///< af1 number of bits
   escaf1 = -43, ///< af1 scale factor

   esiaf0 = sf1, ///< af0 subframe index
   esbaf0 = 270, ///< af0 start bit
   enbaf0 = 22,  ///< af0 number of bits
   escaf0 = -31, ///< af0 scale factor

   esiIODE2 = sf2, ///< IODE2 subframe index
   esbIODE2 = 60,  ///< IODE2 start bit
   enbIODE2 = 8,   ///< IODE2 number of bits
   escIODE2 = 1,   ///< IODE2 scale factor

   esiCrs = sf2, ///< Crs subframe index
   esbCrs = 68,  ///< Crs start bit
   enbCrs = 16,  ///< Crs number of bits
   escCrs = -5,  ///< Crs scale factor

   esidn = sf2, ///< Delta n subframe index
   esbdn = 90,  ///< Delta n start bit
   enbdn = 16,  ///< Delta n number of bits
   escdn = -43, ///< Delta n scale factor

   esiM0  = sf2, ///< M0 subframe index
   esbM0m = 106, ///< M0 MSBs start bit
   enbM0m = 8,   ///< M0 MSBs number of bits
   escM0  = -31, ///< M0 scale factor

   esbM0l = 120, ///< M0 LSBs start bit
   enbM0l = 24,  ///< M0 LSBs number of bits

   esiCuc = sf2, ///< Cuc subframe index
   esbCuc = 150, ///< Cuc start bit
   enbCuc = 16,  ///< Cuc number of bits
   escCuc = -29, ///< Cuc scale factor

   esiEcc  = sf2, ///< Ecc subframe index
   esbEccm = 166, ///< Ecc MSBs start bit
   enbEccm = 8,   ///< Ecc MSBs number of bits
   escEcc  = -33, ///< Ecc scale factor

   esbEccl = 180, ///< Ecc LSBs start bit
   enbEccl = 24,  ///< Ecc LSBs number of bits

   esiCus = sf2, ///< Cus subframe index
   esbCus = 210, ///< Cus start bit
   enbCus = 16,  ///< Cus number of bits
   escCus = -29, ///< Cus scale factor

   esiAhalf  = sf2, ///< Ahalf subframe index
   esbAhalfm = 226, ///< Ahalf MSBs start bit
   enbAhalfm = 8,   ///< Ahalf MSBs number of bits
   escAhalf  = -19, ///< Ahalf scale factor

   esbAhalfl = 240, ///< Ahalf LSBs start bit
   enbAhalfl = 24,  ///< Ahalf LSBs number of bits

   esitoe = sf2, ///< toe subframe index
   esbtoe = 270, ///< toe start bit
   enbtoe = 16,  ///< toe number of bits
   esctoe = 4,   ///< toe scale factor

   esiFitInt = sf2, ///< Fit interval flag subframe index
   esbFitInt = 286, ///< Fit interval flag start bit
   enbFitInt = 1,   ///< Fit interval flag number of bits
   escFitInt = 1,   ///< Fit interval flag scale factor

   esiAODO = sf2, ///< AODO subframe index
   esbAODO = 287, ///< AODO start bit
   enbAODO = 5,   ///< AODO number of bits
   escAODO = 900, ///< AODO scale factor

   esiCic = sf3, ///< Cic subframe index
   esbCic = 60,  ///< Cic start bit
   enbCic = 16,  ///< Cic number of bits
   escCic = -29, ///< Cic scale factor

   esiOMEGA0  = sf3, ///< OMEGA0 subframe index
   esbOMEGA0m = 76,  ///< OMEGA0 MSBs start bit
   enbOMEGA0m = 8,   ///< OMEGA0 MSBs number of bits
   escOMEGA0  = -31, ///< OMEGA0 scale factor

   esbOMEGA0l = 90,  ///< OMEGA0 LSBs start bit
   enbOMEGA0l = 24,  ///< OMEGA0 LSBs number of bits

   esiCis = sf3, ///< Cis subframe index
   esbCis = 120, ///< Cis start bit
   enbCis = 16,  ///< Cis number of bits
   escCis = -29, ///< Cis scale factor

   esii0  = sf3, ///< i0 subframe index
   esbi0m = 136, ///< i0 MSBs start bit
   enbi0m = 8,   ///< i0 MSBs number of bits
   esci0  = -31, ///< i0 scale factor

   esbi0l = 150, ///< i0 LSBs start bit
   enbi0l = 24,  ///< i0 LSBs number of bits

   esiCrc = sf3, ///< Crc subframe index
   esbCrc = 180, ///< Crc start bit
   enbCrc = 16,  ///< Crc number of bits
   escCrc = -5,  ///< Crc scale factor

   esiw  = sf3, ///< w subframe index
   esbwm = 196, ///< w MSBs start bit
   enbwm = 8,   ///< w MSBs number of bits
   escw  = -31, ///< w scale factor

   esbwl = 210, ///< w LSBs start bit
   enbwl = 24,  ///< w LSBs number of bits

   esiOMEGAdot = sf3, ///< OMEGAdot subframe index
   esbOMEGAdot = 240, ///< OMEGAdot start bit
   enbOMEGAdot = 24,  ///< OMEGAdot number of bits
   escOMEGAdot = -43, ///< OMEGAdot scale factor

   esiIODE3 = sf3, ///< IODE3 subframe index
   esbIODE3 = 270, ///< IODE3 start bit
   enbIODE3 = 8,   ///< IODE3 number of bits
   escIODE3 = 1,   ///< IODE3 scale factor

   esiidot = sf3, ///< idot subframe index
   esbidot = 278, ///< idot start bit
   enbidot = 14,  ///< idot number of bits
   escidot = -43, ///< idot scale factor
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

   asbDataID = 60,
   anbDataID = 2,
   ascDataID = 1,

   asbPageID = asbDataID + anbDataID,
   anbPageID = 6,
   ascPageID = 1,

      // ionospheric parameters (sf4 p18)

   asbAlpha0 = asbPageID + anbPageID,
   anbAlpha0 = 8,
   ascAlpha0 = -30,

   asbAlpha1 = asbAlpha0 + anbAlpha0,
   anbAlpha1 = 8,
   ascAlpha1 = -27,

   asbParity3 = asbAlpha1 + anbAlpha1,
   anbParity3 = 6,
   ascParity3 = 1,

   asbAlpha2 = asbParity3 + anbParity3,
   anbAlpha2 = 8,
   ascAlpha2 = -24,

   asbAlpha3 = asbAlpha2 + anbAlpha2,
   anbAlpha3 = 8,
   ascAlpha3 = -24,

   asbBeta0 = asbAlpha3 + anbAlpha3,
   anbBeta0 = 8,
   ascBeta0 = 11,

   asbParity4 = asbBeta0 + anbBeta0,
   anbParity4 = 6,
   ascParity4 = 1,

   asbBeta1 = asbParity4 + anbParity4,
   anbBeta1 = 8,
   ascBeta1 = 14,

   asbBeta2 = asbBeta1 + anbBeta1,
   anbBeta2 = 8,
   ascBeta2 = 16,

   asbBeta3 = asbBeta2 + anbBeta2,
   anbBeta3 = 8,
   ascBeta3 = 16,

   asbParity5 = asbBeta3 + anbBeta3,
   anbParity5 = 6,
   ascParity5 = 1,
};

namespace gnsstk
{
   bool PNBGPSLNavDataFactory ::
   addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
           double cadence)
   {
      if (navIn->getNavID().navType != NavType::GPSLNAV)
      {
            // This class only processes GPS LNav.
         return false;
      }
      bool rv = true;
      bool useQZSS = false;
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
         unsigned long sfid = navIn->asUnsignedLong(49,3,1);
         unsigned long svid = 0;
         unsigned dataID = -1;
         bool checkParity = false, expParity = false;
         switch (navValidity)
         {
            case NavValidityType::ValidOnly:
               checkParity = true;
               expParity = true;
               break;
            case NavValidityType::InvalidOnly:
               checkParity = true;
               expParity = false;
               break;
         }
         if (checkParity)
         {
               /// @todo maybe someday add a parity check to PackedNavBits
               // Convert the PackedNavBits into 10 30-bit words so we
               // can use EngNav::checkParity.
            uint32_t sf[10];
            for (unsigned i = 0, j = 0; i < 300; i += 30, j++)
            {
               sf[j] = navIn->asUnsignedLong(i, 30, 1);
            }
               /// @todo Are the nav subframes really known to be upright?
            bool parity = EngNav::checkParity(sf);
            if (parity != expParity)
               return true;
         }
         switch (sfid)
         {
            case 1:
            case 2:
            case 3:
                  //cerr << "sfid " << sfid << " = ephemeris" << endl;
               rv = processEph(sfid, navIn, navOut);
               break;
            case 4:
            case 5:
               svid = navIn->asUnsignedLong(asbPageID,anbPageID,ascPageID);
               dataID = navIn->asUnsignedLong(asbDataID,anbDataID,ascDataID);
               useQZSS =
                  ((navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)&&
                   (dataID == dataIDQZSS));
               if ((svid <= MAX_PRN_GPS) && (svid >= 1))
               {
                  if (useQZSS)
                  {
                        // IS-QZSS 1.8 section 5.2.2.2.5.2
                        // When the Data-ID is "11" (B) , the content
                        // is QZS Almanac data and Almanac health, and
                        // the eight bits of "data ID (2 bits) + SV ID
                        // (6 bits)" indicates the PRN number of the
                        // QZS.
                     svid = svid | (dataID << 6);
                  }
                     /// @todo support GPS PRNs 33+ (IS-GPS-200 40.1, appendix IV)
                     // process orbit and health
                  rv = processAlmOrb(svid, navIn, navOut);
               }
               else if (svid == 51)
               {
                     // process health
                  rv = processSVID51(navIn, navOut);
               }
               else if (svid == 63)
               {
                     // process health
                  rv = processSVID63(navIn, navOut);
               }
               else if ((svid == 56) || (useQZSS && (svid == 61)))
               {
                     // process time offset
                  rv = processSVID56(navIn, navOut);
               }
               break;
            default:
               // cerr << "invalid sfid " << sfid << endl;
                  // This used to be false until I tried processing
                  // MDP data, and immediately started getting invalid
                  // subframe IDs.
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


   bool PNBGPSLNavDataFactory ::
   processEph(unsigned sfid, const PackedNavBitsPtr& navIn,
              NavDataPtrList& navOut)
   {
      NavSatelliteID key(navIn->getsatSys().id, navIn->getsatSys(),
                         navIn->getobsID(), navIn->getNavID());
      if (sfid == 1)
      {
         if (processHea)
         {
               // Add ephemeris health bits from subframe 1.
            NavDataPtr p1 = std::make_shared<GPSLNavHealth>();
            p1->timeStamp = navIn->getTransmitTime();
            p1->signal = NavMessageID(key, NavMessageType::Health);
            dynamic_cast<GPSLNavHealth*>(p1.get())->svHealth =
               navIn->asUnsignedLong(esbHea,enbHea,escHea);
               // cerr << "add LNAV eph health" << endl;
            navOut.push_back(p1);
         }
         if (processISC)
         {
               // Add ephemeris Tgd bits from subframe 1.
            NavDataPtr p2 = std::make_shared<GPSLNavISC>();
            GPSLNavISC *isc = dynamic_cast<GPSLNavISC*>(p2.get());
            isc->timeStamp = navIn->getTransmitTime();
            isc->signal = NavMessageID(key, NavMessageType::ISC);
            isc->pre = navIn->asUnsignedLong(fsbPre,fnbPre,fscPre);
            isc->tlm = navIn->asUnsignedLong(fsbTLM,fnbTLM,fscTLM);
            if (navIn->getsatSys().system == gnsstk::SatelliteSystem::GPS)
            {
               isc->isf = navIn->asBool(fsbISF);
            }
            isc->alert = navIn->asBool(fsbAlert);
            isc->asFlag = navIn->asBool(fsbAS);
            isc->isc = navIn->asSignedDouble(esbTGD,enbTGD,escTGD);
               // cerr << "add LNAV eph Tgd" << endl;
            navOut.push_back(p2);
         }
      }
      if (!PNBNavDataFactory::processEph)
      {
            // User doesn't want ephemerides so don't do any processing.
         return true;
      }
      if (ephAcc.find(key) == ephAcc.end())
      {
         // cerr << "ephAcc is empty for signal " << key << endl;
            // set up a new ephemeris
         ephAcc[key].resize(3);
         ephAcc[key][sfid-1] = navIn;
            // nothing in navOut yet and no further processing because
            // we only have one of three subframes at this point.
         return true;
      }
      std::vector<PackedNavBitsPtr> &ephSF(ephAcc[key]);
      ephSF[sfid-1] = navIn;
         // stop processing if we don't have three full subframes
      if (!ephSF[sf1] || !ephSF[sf2] || !ephSF[sf3] ||
          (ephSF[sf1]->getNumBits() != 300) ||
          (ephSF[sf2]->getNumBits() != 300) ||
          (ephSF[sf3]->getNumBits() != 300))
      {
         // cerr << "Not ready for full LNAV eph processing" << endl
         //      << "  sf1: " << (bool)ephSF[sf1] << " "
         //      << (ephSF[sf1] ? ephSF[sf1]->getNumBits() : -1) << endl
         //      << "  sf2: " << (bool)ephSF[sf2] << " "
         //      << (ephSF[sf2] ? ephSF[sf2]->getNumBits() : -1) << endl
         //      << "  sf3: " << (bool)ephSF[sf3] << " "
         //      << (ephSF[sf3] ? ephSF[sf3]->getNumBits() : -1) << endl;
         return true;
      }
         // Stop processing if we don't have matching IODC/IODE in
         // each of the three subframes.  Only get the 8 LSBs of IODC
         // for this test to match the IODEs.
      unsigned long iodc, iode2, iode3;
      iodc = ephSF[esiIODC]->asUnsignedLong(esbIODCl,enbIODCl,escIODC);
      iode2 = ephSF[esiIODE2]->asUnsignedLong(esbIODE2,enbIODE2,escIODE2);
      iode3 = ephSF[esiIODE3]->asUnsignedLong(esbIODE3,enbIODE3,escIODE3);
      if ((iodc != iode2) || (iodc != iode3))
      {
            //cerr << "IODC/IODE mismatch, not processing" << endl;
            // Even though the mismatch might be considered an error,
            // we don't really want to mark it as such and rather
            // consider it as a "valid" but unprocessable data set.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSLNavEph>();
      GPSLNavEph *eph = dynamic_cast<GPSLNavEph*>(p0.get());
         // NavData
      eph->timeStamp = ephSF[sf1]->getTransmitTime();
      eph->signal = NavMessageID(key, NavMessageType::Ephemeris);
      // cerr << "Ready for full LNAV eph processing for " << (NavSignalID)key << endl;
         // OrbitData = empty
         // OrbitDataKepler
      eph->xmitTime = eph->timeStamp;
      eph->xmit2 = ephSF[sf2]->getTransmitTime();
      eph->xmit3 = ephSF[sf3]->getTransmitTime();
      double toe = ephSF[esitoe]->asUnsignedDouble(esbtoe,enbtoe,esctoe);
      double toc = ephSF[esitoc]->asUnsignedDouble(esbtoc,enbtoc,esctoc);
      unsigned wn = ephSF[esiWN]->asUnsignedLong(esbWN,enbWN,escWN);
         // Use the transmit time to get a full week for toe/toc
      GPSWeekSecond refTime(eph->xmitTime);
      long refWeek = refTime.week;
         // cerr << "toe=" << toe << "  refTime=" << refTime << "  wn(1)=" << wn
         //      << "  refWeek=" << refWeek;
         // this is the 10-bit week rollover fix
      wn = timeAdjustWeekRollover(wn, refWeek);
         // cerr << "  wn(2)=" << wn << endl;
         // Now we can set the Toe/Toc properly
         // and use the half-week test for week rollover fixation.
      eph->Toe = GPSWeekSecond(wn,toe).weekRolloverAdj(refTime);
      eph->Toc = GPSWeekSecond(wn,toc).weekRolloverAdj(refTime);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)
      {
         eph->Toe.setTimeSystem(gnsstk::TimeSystem::QZS);
         eph->Toc.setTimeSystem(gnsstk::TimeSystem::QZS);
      }
         // health is set below
      eph->Cuc = ephSF[esiCuc]->asSignedDouble(esbCuc,enbCuc,escCuc);
      eph->Cus = ephSF[esiCus]->asSignedDouble(esbCus,enbCus,escCus);
      eph->Crc = ephSF[esiCrc]->asSignedDouble(esbCrc,enbCrc,escCrc);
      eph->Crs = ephSF[esiCrs]->asSignedDouble(esbCrs,enbCrs,escCrs);
      eph->Cic = ephSF[esiCic]->asSignedDouble(esbCic,enbCic,escCic);
      eph->Cis = ephSF[esiCis]->asSignedDouble(esbCis,enbCis,escCis);
      eph->M0  = ephSF[esiM0]->asDoubleSemiCircles(esbM0m,enbM0m,esbM0l,enbM0l,
                                                   escM0);
      eph->dn  = ephSF[esidn]->asDoubleSemiCircles(esbdn,enbdn,escdn);
         // no dndot in GPS LNAV
      eph->ecc = ephSF[esiEcc]->asUnsignedDouble(esbEccm,enbEccm,
                                                 esbEccl,enbEccl,escEcc);
      eph->Ahalf = ephSF[esiAhalf]->asUnsignedDouble(esbAhalfm,enbAhalfm,
                                                     esbAhalfl,enbAhalfl,
                                                     escAhalf);
      eph->A = eph->Ahalf * eph->Ahalf;
         // no Adot in GPS LNAV
      eph->OMEGA0 = ephSF[esiOMEGA0]->asDoubleSemiCircles(esbOMEGA0m,enbOMEGA0m,
                                                          esbOMEGA0l,enbOMEGA0l,
                                                          escOMEGA0);
      eph->i0 = ephSF[esii0]->asDoubleSemiCircles(esbi0m,enbi0m,esbi0l,enbi0l,
                                                  esci0);
      eph->w = ephSF[esiw]->asDoubleSemiCircles(esbwm,enbwm,esbwl,enbwl,escw);
      eph->OMEGAdot = ephSF[esiOMEGAdot]->asDoubleSemiCircles(
         esbOMEGAdot,enbOMEGAdot,escOMEGAdot);
      eph->idot = ephSF[esiidot]->asDoubleSemiCircles(esbidot,enbidot,escidot);
      eph->af0 = ephSF[esiaf0]->asSignedDouble(esbaf0,enbaf0,escaf0);
      eph->af1 = ephSF[esiaf1]->asSignedDouble(esbaf1,enbaf1,escaf1);
      eph->af2 = ephSF[esiaf2]->asSignedDouble(esbaf2,enbaf2,escaf2);
         // GPSLNavData
      eph->pre = ephSF[sf1]->asUnsignedLong(fsbPre,fnbPre,fscPre);
      eph->tlm = ephSF[sf1]->asUnsignedLong(fsbTLM,fnbTLM,fscTLM);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::GPS)
      {
         eph->isf = ephSF[sf1]->asBool(fsbISF);
      }
      eph->alert = ephSF[sf1]->asBool(fsbAlert);
      eph->asFlag = ephSF[sf1]->asBool(fsbAS);
         // GPSLNavEph
      eph->pre2 = ephSF[sf2]->asUnsignedLong(fsbPre,fnbPre,fscPre);
      eph->pre3 = ephSF[sf3]->asUnsignedLong(fsbPre,fnbPre,fscPre);
      eph->tlm2 = ephSF[sf2]->asUnsignedLong(fsbTLM,fnbTLM,fscTLM);
      eph->tlm3 = ephSF[sf3]->asUnsignedLong(fsbTLM,fnbTLM,fscTLM);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::GPS)
      {
         eph->isf2 = ephSF[sf2]->asBool(fsbISF);
         eph->isf3 = ephSF[sf3]->asBool(fsbISF);
      }
         // 2 = size of iodcStart/iodcNum arrays
      eph->iodc = ephSF[esiIODC]->asUnsignedLong(esbIODCm,enbIODCm,esbIODCl,
                                                 enbIODCl,escIODC);
      eph->iode = iode2; // we've already extracted it
      eph->fitIntFlag = ephSF[esiFitInt]->asUnsignedLong(esbFitInt,enbFitInt,
                                                         escFitInt);
      eph->healthBits = ephSF[esiHea]->asUnsignedLong(esbHea,enbHea,escHea);
      eph->health = ((eph->healthBits == 0) ? SVHealth::Healthy :
                     SVHealth::Unhealthy); // actually in OrbitDataKepler
      eph->uraIndex = ephSF[esiURA]->asUnsignedLong(esbURA,enbURA,escURA);
      eph->tgd = ephSF[esiTGD]->asSignedDouble(esbTGD,enbTGD,escTGD);
      eph->alert2 = ephSF[sf2]->asBool(fsbAlert);
      eph->alert3 = ephSF[sf3]->asBool(fsbAlert);
      eph->asFlag2 = ephSF[sf2]->asBool(fsbAS);
      eph->asFlag3 = ephSF[sf3]->asBool(fsbAS);
      eph->codesL2 = static_cast<GPSLNavEph::L2Codes>(
         ephSF[esiL2]->asUnsignedLong(esbL2,enbL2,escL2));
      eph->L2Pdata = ephSF[esiL2P]->asUnsignedLong(esbL2P,enbL2P,escL2P);
      eph->aodo = ephSF[esiAODO]->asUnsignedLong(esbAODO,enbAODO,escAODO);
      eph->fixFit();
      // cerr << "add LNAV eph" << endl;
      navOut.push_back(p0);
         // Clear out the broadcast ephemeris that's been processed.
      ephAcc.erase(key);
      return true;
   }


   bool PNBGPSLNavDataFactory ::
   processAlmOrb(unsigned long prn, const PackedNavBitsPtr& navIn,
                 NavDataPtrList& navOut)
   {
      SatID xmitSat(navIn->getsatSys());
         // Key used for data accumulating, which we don't separate by
         // subject satellite.
      NavSatelliteID key(0, xmitSat, navIn->getobsID(), navIn->getNavID());
         // Actual satellite ID (subject and transmit) used in a
         // couple of places.
      NavSatelliteID sat(prn, xmitSat, navIn->getobsID(), navIn->getNavID());
         // No checks for correct svid, just assume that the input
         // data has already been checked (it will have been by
         // addData).
      if (processHea)
      {
            // Add almanac orbit page health bits.
         NavDataPtr p1 = std::make_shared<GPSLNavHealth>();
         p1->timeStamp = navIn->getTransmitTime();
         p1->signal = NavMessageID(sat, NavMessageType::Health);
         dynamic_cast<GPSLNavHealth*>(p1.get())->svHealth =
            navIn->asUnsignedLong(136,8,1);
         // cerr << "add LNAV alm health" << endl;
         navOut.push_back(p1);
      }
      if (!PNBNavDataFactory::processAlm)
      {
            // User doesn't want almanacs so don't do any processing.
         return true;
      }
         // SVID 1-32 contain the almanac orbital elements as well as
         // health information (Figure 20-1 sheet 4), so we'll end up
         // returning two items in navOut.
      NavDataPtr p0 = std::make_shared<GPSLNavAlm>();
      p0->timeStamp = navIn->getTransmitTime();
      p0->signal = NavMessageID(sat, NavMessageType::Almanac);
      GPSLNavAlm *alm = dynamic_cast<GPSLNavAlm*>(p0.get());
         // Bit positions taken from IS-GPS-200 figure 20-1 sheet 4,
         // and figure 20-2, but subtracting 1 since PNB is 0-based.
         // Scales taken from Table 20-VI
         // Sinusoidal corrections and other unset parameters are not
         // used by almanac data and are initialized to 0 by
         // constructor
      alm->pre = navIn->asUnsignedLong(fsbPre,fnbPre,fscPre);
      alm->tlm = navIn->asUnsignedLong(fsbTLM,fnbTLM,fscTLM);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::GPS)
      {
         alm->isf = navIn->asBool(fsbISF);
      }
      alm->alert = navIn->asBool(fsbAlert);
      alm->asFlag = navIn->asBool(fsbAS);
      alm->xmitTime = navIn->getTransmitTime();
      alm->ecc = navIn->asUnsignedDouble(68,16,-21);
      alm->toa = navIn->asUnsignedDouble(90,8,12);
      GPSWeekSecond ws(alm->xmitTime);
      // cerr << "page " << prn << " WNa = ??  toa = " << alm->toa
      //      << "  WNx = " << (ws.week & 0x0ff) << "  tox = " << ws.sow << endl;
      alm->deltai = navIn->asDoubleSemiCircles(98,16,-19);
         /** @todo determine if this offset applies only when the
          * subject satellite is QZSS or if it is used whenever the
          * transmitting satellite is QZSS. */
      if (alm->signal.sat.system == SatelliteSystem::QZSS)
      {
         alm->i0 = GPSLNavData::refioffsetQZSS + alm->deltai;
         alm->ecc += GPSLNavData::refEccQZSS;
         ws.setTimeSystem(gnsstk::TimeSystem::QZS);
      }
      else
      {
         alm->i0 = GPSLNavData::refioffsetGPS + alm->deltai;
      }
      alm->OMEGAdot = navIn->asDoubleSemiCircles(120,16,-38);
      alm->healthBits = navIn->asUnsignedLong(136,8,1);
      alm->health = (alm->healthBits == 0 ? SVHealth::Healthy :
                     SVHealth::Unhealthy);
      alm->Ahalf = navIn->asUnsignedDouble(150,24,-11);
      alm->A = alm->Ahalf * alm->Ahalf;
      alm->OMEGA0 = navIn->asDoubleSemiCircles(180,24,-23);
      alm->w = navIn->asDoubleSemiCircles(210,24,-23);
      alm->M0 = navIn->asDoubleSemiCircles(240,24,-23);
      const unsigned af0start[] = {270,289};
      const unsigned af0num[] = {8,3};
         // 2 is the size of the af0start/af0num arrays
      alm->af0 = navIn->asSignedDouble(af0start,af0num,2,-20);
      alm->af1 = navIn->asSignedDouble(278,11,-38);
         // If we have a wna for this transmitting PRN, use it to set
         // the toa (identified as Toe/Toc in OrbitDataKepler).
         // Otherwise, stash the data until we do have a wna.
      if ((fullWNaMap.find(xmitSat.id) != fullWNaMap.end()) &&
          (fullWNaMap[xmitSat.id].sow == alm->toa))
      {
         alm->Toe = alm->Toc = fullWNaMap[xmitSat.id];
         alm->fixFit();
         // cerr << "add LNAV alm" << endl;
         navOut.push_back(p0);
      }
      else
      {
            // no WNa available yet, accumulate the data until we get a page 51
         almAcc[key].push_back(p0);
      }
      return true;
   }


   bool PNBGPSLNavDataFactory ::
   processSVID51(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
         // No checks for correct svid, just assume that the input
         // data has already been checked (it will have been by
         // addData).
      SatID xmitSat(navIn->getsatSys());
      ObsID oid(navIn->getobsID());
      NavID navid(navIn->getNavID());
         // Key used for data accumulating, which we don't separate by
         // subject satellite.
      NavSatelliteID key(0, xmitSat, oid, navid);
      if (PNBNavDataFactory::processAlm)
      {
            // Set the fullWNa now that we have something to go on,
            // but only if we're processing almanac data, which is the
            // only situation where it's used.
         double toa = navIn->asUnsignedDouble(68,8,12);
         unsigned shortWNa = navIn->asUnsignedLong(76, 8, 1);
         GPSWeekSecond ws(navIn->getTransmitTime());
         long refWeek = ws.week;
         unsigned fullWNa = timeAdjust8BitWeekRollover(shortWNa, refWeek);
         fullWNaMap[xmitSat.id] = GPSWeekSecond(fullWNa,toa);
         fullWNaMap[xmitSat.id].weekRolloverAdj(ws);
         // cerr << "page 51 WNa = " << shortWNa << "  toa = " << toa
         //      << "  WNx = " << (ws.week & 0x0ff) << "  tox = " << ws.sow
         //      << "  fullWNa = " << fullWNa << endl;
         NavDataPtrList& ndpl(almAcc[key]);
         for (auto i = ndpl.begin(); i != ndpl.end();)
         {
            GPSLNavAlm *alm = dynamic_cast<GPSLNavAlm*>(i->get());
            if (alm->toa == toa)
            {
               alm->Toe = alm->Toc = fullWNaMap[xmitSat.id];
               alm->fixFit();
               // cerr << "add LNAV alm" << endl;
               navOut.push_back(*i);
               i = ndpl.erase(i);
            }
            else
            {
               ++i;
            }
         }
      }
         // svid 51 = sf 5 page 25. The format has 24 SVs in it,
         // grouped 4 per word for GPS.  For QZSS, it varies - see
         // IS-QZSS-PNT-004 Table 4.1.2-12.
         /** @todo Should we be generating health objects for both PRN
          * 196 and 203, for example?  It's not entirely clear from
          * the table if the PRN should be 196 for L1 C/A and 203 for
          * L1 C/B, or if the health status applies to both at the
          * same time. */
      if (!PNBNavDataFactory::processHea)
      {
            // User doesn't want health so don't do any processing.
         return true;
      }
         // use GPS defaults.
      unsigned startPRN = 1, endPRN = 24;
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)
      {
         unsigned dataID = navIn->asUnsignedLong(asbDataID,anbDataID,ascDataID);
         if (dataID == dataIDQZSS)
         {
            startPRN = MIN_PRN_QZS;
            endPRN = MAX_PRN_QZS_LNAV;
         }
      }
      for (unsigned prn = startPRN, bit = 90; prn <= endPRN;
           prn += 4, bit += 30)
      {
         NavDataPtr p1 = std::make_shared<GPSLNavHealth>();
         NavDataPtr p2 = std::make_shared<GPSLNavHealth>();
         NavDataPtr p3 = std::make_shared<GPSLNavHealth>();
         NavDataPtr p4 = std::make_shared<GPSLNavHealth>();
         p1->timeStamp = navIn->getTransmitTime();
         p1->signal = NavMessageID(
            NavSatelliteID(prn+0, xmitSat, oid, navid),
            NavMessageType::Health);
         dynamic_cast<GPSLNavHealth*>(p1.get())->svHealth =
            navIn->asUnsignedLong(bit+0, 6, 1);
         // cerr << "add LNAV page 51 health" << endl;
         navOut.push_back(p1);
         if ((prn+1) > endPRN)
            break;
         p2->timeStamp = navIn->getTransmitTime();
         p2->signal = NavMessageID(
            NavSatelliteID(prn+1, xmitSat, oid, navid),
            NavMessageType::Health);
         dynamic_cast<GPSLNavHealth*>(p2.get())->svHealth =
            navIn->asUnsignedLong(bit+6, 6, 1);
         // cerr << "add LNAV page 51 health" << endl;
         navOut.push_back(p2);
         if ((prn+2) > endPRN)
            break;
         p3->timeStamp = navIn->getTransmitTime();
         p3->signal = NavMessageID(
            NavSatelliteID(prn+2, xmitSat, oid, navid),
            NavMessageType::Health);
         dynamic_cast<GPSLNavHealth*>(p3.get())->svHealth =
            navIn->asUnsignedLong(bit+12, 6, 1);
         // cerr << "add LNAV page 51 health" << endl;
         navOut.push_back(p3);
         if ((prn+3) > endPRN)
            break;
         p4->timeStamp = navIn->getTransmitTime();
         p4->signal = NavMessageID(
            NavSatelliteID(prn+3, xmitSat, oid, navid),
            NavMessageType::Health);
         dynamic_cast<GPSLNavHealth*>(p4.get())->svHealth =
            navIn->asUnsignedLong(bit+18, 6, 1);
         // cerr << "add LNAV page 51 health" << endl;
         navOut.push_back(p4);
      }
      return true;
   }


   bool PNBGPSLNavDataFactory ::
   processSVID63(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
         // No checks for correct svid, just assume that the input
         // data has already been checked (it will have been by
         // addData).
         // svid 63 = sf 4 page 25. The format has SVs 25-32 in it.
         // SV 25 is kind of off on its own so do it first, outside the loop
      if (!PNBNavDataFactory::processHea)
      {
            // User doesn't want health so don't do any processing.
         return true;
      }
      SatID xmitSat(navIn->getsatSys());
      ObsID oid(navIn->getobsID());
      NavID navid(navIn->getNavID());
      NavDataPtr p0 = std::make_shared<GPSLNavHealth>();
      p0->timeStamp = navIn->getTransmitTime();
      p0->signal = NavMessageID(
         NavSatelliteID(25, xmitSat, oid, navid),
         NavMessageType::Health);
         // prn 25 health starts at bit 229 (1-based), so we use 228 (0-based)
      dynamic_cast<GPSLNavHealth*>(p0.get())->svHealth =
         navIn->asUnsignedLong(228, 6, 1);
      // cerr << "add LNAV page 63 health" << endl;
      navOut.push_back(p0);
      for (unsigned prn = 26, bit = 240; prn <= 32; prn += 4, bit += 30)
      {
         NavDataPtr p1 = std::make_shared<GPSLNavHealth>();
         NavDataPtr p2 = std::make_shared<GPSLNavHealth>();
         NavDataPtr p3 = std::make_shared<GPSLNavHealth>();
         NavDataPtr p4;
         p1->timeStamp = navIn->getTransmitTime();
         p1->signal = NavMessageID(
            NavSatelliteID(prn+0, xmitSat, oid, navid),
            NavMessageType::Health);
         dynamic_cast<GPSLNavHealth*>(p1.get())->svHealth =
            navIn->asUnsignedLong(bit+0, 6, 1);
         // cerr << "add LNAV page 63 health" << endl;
         navOut.push_back(p1);
         p2->timeStamp = navIn->getTransmitTime();
         p2->signal = NavMessageID(
            NavSatelliteID(prn+1, xmitSat, oid, navid),
            NavMessageType::Health);
         dynamic_cast<GPSLNavHealth*>(p2.get())->svHealth =
            navIn->asUnsignedLong(bit+6, 6, 1);
         // cerr << "add LNAV page 63 health" << endl;
         navOut.push_back(p2);
         p3->timeStamp = navIn->getTransmitTime();
         p3->signal = NavMessageID(
            NavSatelliteID(prn+2, xmitSat, oid, navid),
            NavMessageType::Health);
         dynamic_cast<GPSLNavHealth*>(p3.get())->svHealth =
            navIn->asUnsignedLong(bit+12, 6, 1);
         // cerr << "add LNAV page 63 health" << endl;
         navOut.push_back(p3);
            // Word 9 has 4 PRNs, word 10 only has 3, so we have to do
            // this check.
         if (prn < 30)
         {
            p4 = std::make_shared<GPSLNavHealth>();
            p4->timeStamp = navIn->getTransmitTime();
            p4->signal = NavMessageID(
               NavSatelliteID(prn+3, xmitSat, oid, navid),
               NavMessageType::Health);
            dynamic_cast<GPSLNavHealth*>(p4.get())->svHealth =
               navIn->asUnsignedLong(bit+18, 6, 1);
            // cerr << "add LNAV page 63 health" << endl;
            navOut.push_back(p4);
         }
      }
      return true;
   }


   bool PNBGPSLNavDataFactory ::
   processSVID56(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
         // No checks for correct svid, just assume that the input
         // data has already been checked (it will have been by
         // addData).
         // svid 56 = sf 4 page 18.
      if (PNBNavDataFactory::processIono)
      {
         NavDataPtr p1 = std::make_shared<GPSLNavIono>();
         p1->timeStamp = navIn->getTransmitTime();
         p1->signal = NavMessageID(
            NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                           navIn->getobsID(), navIn->getNavID()),
            NavMessageType::Iono);
         GPSLNavIono *iono = dynamic_cast<GPSLNavIono*>(p1.get());
            // GPSLNavIono
         iono->pre = navIn->asUnsignedLong(fsbPre,fnbPre,fscPre);
         iono->tlm = navIn->asUnsignedLong(fsbTLM,fnbTLM,fscTLM);
         if (navIn->getsatSys().system == gnsstk::SatelliteSystem::GPS)
         {
            iono->isf = navIn->asBool(fsbISF);
         }
         iono->alert = navIn->asBool(fsbAlert);
         iono->asFlag = navIn->asBool(fsbAS);
         iono->alpha[0] = navIn->asSignedDouble(asbAlpha0,anbAlpha0,ascAlpha0);
         iono->alpha[1] = navIn->asSignedDouble(asbAlpha1,anbAlpha1,ascAlpha1);
         iono->alpha[2] = navIn->asSignedDouble(asbAlpha2,anbAlpha2,ascAlpha2);
         iono->alpha[3] = navIn->asSignedDouble(asbAlpha3,anbAlpha3,ascAlpha3);
         iono->beta[0] = navIn->asSignedDouble(asbBeta0,anbBeta0,ascBeta0);
         iono->beta[1] = navIn->asSignedDouble(asbBeta1,anbBeta1,ascBeta1);
         iono->beta[2] = navIn->asSignedDouble(asbBeta2,anbBeta2,ascBeta2);
         iono->beta[3] = navIn->asSignedDouble(asbBeta3,anbBeta3,ascBeta3);
         navOut.push_back(p1);
      }
      if (!PNBNavDataFactory::processTim)
      {
            // User doesn't want time offset data so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSLNavTimeOffset>();
      p0->timeStamp = navIn->getTransmitTime();
      p0->signal = NavMessageID(
         NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                        navIn->getobsID(), navIn->getNavID()),
         NavMessageType::TimeOffset);
      GPSLNavTimeOffset *to = dynamic_cast<GPSLNavTimeOffset*>(p0.get());
         // Bit positions taken from IS-GPS-200 figure 20-1 sheet 8,
         // but subtracting 1 since PNB is 0-based.
         // Scales taken from Table 20-IX
         /** @note while some of the values are stored as doubles,
          * they may be extracted as long (e.g. deltatLS) because
          * there is no scaling for the encoded value. */
      const unsigned a0start[] = { 180,210 };
      const unsigned a0num[] = { 24,8 };
      to->deltatLS = navIn->asLong(240,8,1);
         // 2 is the size of the start/num arrays, while -30 is the
         // scale factor i.e. x*2^-30
      to->a0 = navIn->asSignedDouble(a0start,a0num,2,-30);
      to->a1 = navIn->asSignedDouble(150,24,-50);
      to->tot = navIn->asUnsignedDouble(218,8,12);
      to->wnot = navIn->asUnsignedLong(226,8,1);
      to->wnLSF = navIn->asUnsignedLong(248,8,1);
      to->dn = navIn->asUnsignedLong(256,8,1);
      to->deltatLSF = navIn->asLong(270,8,1);
         // adjust week numbers to full week
      GPSWeekSecond ws(p0->timeStamp);
      long refWeek = ws.week;
      to->wnot = timeAdjust8BitWeekRollover(to->wnot, refWeek);
      to->wnLSF = timeAdjust8BitWeekRollover(to->wnLSF, refWeek);
      to->refTime = GPSWeekSecond(to->wnot, to->tot);
      to->effTime = GPSWeekSecond(to->wnLSF, (to->dn-1)*86400);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)
      {
         to->src = gnsstk::TimeSystem::QZS;
         to->refTime.setTimeSystem(gnsstk::TimeSystem::QZS);
         to->effTime.setTimeSystem(gnsstk::TimeSystem::QZS);
      }
         // return results.
      // cerr << "add LNAV page 56 time offset" << endl;
      navOut.push_back(p0);
      return true;
   }


   void PNBGPSLNavDataFactory ::
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


   void PNBGPSLNavDataFactory ::
   resetState()
   {
      fullWNaMap.clear();
      almAcc.clear();
      ephAcc.clear();
   }

} // namespace gnsstk
