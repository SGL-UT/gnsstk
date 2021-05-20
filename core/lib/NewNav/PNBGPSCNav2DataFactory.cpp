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
#include <math.h>
#include "PNBGPSCNav2DataFactory.hpp"
#include "GPSCNav2Alm.hpp"
// #include "GPSCNav2RedAlm.hpp"
#include "GPSCNav2Eph.hpp"
#include "GPSCNav2Health.hpp"
#include "GPSCNav2TimeOffset.hpp"
#include "GPSCNav2Iono.hpp"
#include "TimeCorrection.hpp"

using namespace std;

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
};


namespace gpstk
{
   bool PNBGPSCNav2DataFactory ::
   addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
           double cadence)
   {
      if (navIn->getNavID().navType != NavType::GPSCNAV2)
      {
            // This class only processes GPS CNav2.
         return false;
      }
      bool rv = true;
      bool isQZSS = navIn->getsatSys().system == SatelliteSystem::QZSS;
      try
      {
         switch (navIn->getNumBits())
         {
            case nnbSF2:
               rv = processEph(navIn, navOut);
               break;
            case nnbSF3:
               rv = processSF3(navIn, navOut);
               break;
            case nnbComplete:
               rv = processEph(navIn, navOut, nnbSF1) &&
                  processSF3(navIn, navOut, nnbSF1+nnbSF2+nnbCRC);
               break;
            default:
               return true;
         }
      }
      catch (gpstk::Exception& exc)
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


   bool PNBGPSCNav2DataFactory ::
   processEph(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
              unsigned offset)
   {
      unsigned prn = navIn->getsatSys().id;
      if (processHea)
      {
            // Add ephemeris health bit
         NavDataPtr p1 = std::make_shared<GPSCNav2Health>();
         p1->timeStamp = navIn->getTransmitTime();
         p1->signal = NavMessageID(
            NavSatelliteID(prn, prn, navIn->getsatSys().system,
                           CarrierBand::L1, TrackingCode::L1CD,
                           NavType::GPSCNAV2),
            NavMessageType::Health);
         dynamic_cast<GPSCNav2Health*>(p1.get())->health =
            navIn->asBool(offset+esbHeaL1C);
         dynamic_cast<GPSCNav2Health*>(p1.get())->setEph(true);
         // cerr << "add CNAV2 eph health" << endl;
         navOut.push_back(p1);
      }
      if (!PNBNavDataFactory::processEph)
      {
            // User doesn't want ephemerides so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNav2Eph>();
      GPSCNav2Eph *eph = dynamic_cast<GPSCNav2Eph*>(p0.get());
         // NavData
      eph->timeStamp = navIn->getTransmitTime();
      eph->signal = NavMessageID(
         NavSatelliteID(prn, navIn->getsatSys(), navIn->getobsID(),
                        navIn->getNavID()),
         NavMessageType::Ephemeris);
         // OrbitData = empty
         // OrbitDataKepler
      eph->xmitTime = eph->timeStamp;
         /** @todo apply 13-bit week rollover adjustment, not 10-bit.
          * Must be completed by January, 2137 :-) */
      long wn = navIn->asUnsignedLong(offset+esbWN,enbWN,escWN);
      double toe = navIn->asUnsignedLong(offset+esbtoe,enbtoe,esctoe);
      eph->itow = navIn->asUnsignedLong(offset+esbITOW,enbITOW,escITOW);
      eph->Toe = eph->Toc = GPSWeekSecond(wn,toe);
      if (navIn->getsatSys().system == gpstk::SatelliteSystem::QZSS)
      {
         eph->Toe.setTimeSystem(gpstk::TimeSystem::QZS);
         eph->Toc.setTimeSystem(gpstk::TimeSystem::QZS);
      }
         // health is set below
      eph->Cuc = navIn->asSignedDouble(offset+esbCuc,enbCuc,escCuc);
      eph->Cus = navIn->asSignedDouble(offset+esbCus,enbCus,escCus);
      eph->Crc = navIn->asSignedDouble(offset+esbCrc,enbCrc,escCrc);
      eph->Crs = navIn->asSignedDouble(offset+esbCrs,enbCrs,escCrs);
      eph->Cic = navIn->asSignedDouble(offset+esbCic,enbCic,escCic);
      eph->Cis = navIn->asSignedDouble(offset+esbCis,enbCis,escCis);
      eph->M0  = navIn->asDoubleSemiCircles(offset+esbM0,enbM0,escM0);
      eph->dn  = navIn->asDoubleSemiCircles(offset+esbdn0,enbdn0,escdn0);
      eph->dndot = navIn->asDoubleSemiCircles(offset+esbdn0dot,enbdn0dot,
                                              escdn0dot);
      eph->ecc = navIn->asUnsignedDouble(offset+esbEcc,enbEcc,escEcc);
      eph->deltaA = navIn->asSignedDouble(offset+esbdA,enbdA,escdA);
      eph->dOMEGAdot = navIn->asDoubleSemiCircles(
         offset+esbdOMEGAdot,enbdOMEGAdot,escdOMEGAdot);
      if (eph->signal.sat.system == SatelliteSystem::QZSS)
      {
         eph->A = eph->deltaA + GPSCNav2Eph::refAQZSS;
         eph->OMEGAdot = eph->dOMEGAdot + GPSCNav2Eph::refOMEGAdotQZSS;
      }
      else
      {
         eph->A = eph->deltaA + GPSCNav2Eph::refAGPS;
         eph->OMEGAdot = eph->dOMEGAdot + GPSCNav2Eph::refOMEGAdotGPS;
      }
      eph->Ahalf = ::sqrt(eph->A);
      eph->Adot = navIn->asSignedDouble(offset+esbAdot,enbAdot,escAdot);
      eph->OMEGA0 = navIn->asDoubleSemiCircles(offset+esbOMEGA0,enbOMEGA0,
                                               escOMEGA0);
      eph->i0 = navIn->asDoubleSemiCircles(offset+esbi0,enbi0,esci0);
      eph->w = navIn->asDoubleSemiCircles(offset+esbw,enbw,escw);
      eph->idot = navIn->asDoubleSemiCircles(offset+esbidot,enbidot,escidot);
      eph->af0 = navIn->asSignedDouble(offset+esbaf0,enbaf0,escaf0);
      eph->af1 = navIn->asSignedDouble(offset+esbaf1,enbaf1,escaf1);
      eph->af2 = navIn->asSignedDouble(offset+esbaf2,enbaf2,escaf2);
         // GPSCNav2Eph
      eph->healthL1C = navIn->asBool(offset+esbHeaL1C);
      eph->health = eph->healthL1C ? SVHealth::Unhealthy : SVHealth::Healthy;
      eph->uraED = navIn->asLong(offset+esbURA,enbURA,escURA);
      unsigned wnop = navIn->asUnsignedLong(offset+esbWNop,enbWNop,escWNop);
      double top = navIn->asUnsignedLong(offset+esbtop,enbtop,esctop);
      wnop = timeAdjust8BitWeekRollover(wnop, wn);
      eph->top = GPSWeekSecond(wnop,top);
      if (navIn->getsatSys().system == gpstk::SatelliteSystem::QZSS)
      {
         eph->top.setTimeSystem(gpstk::TimeSystem::QZS);
      }
      eph->uraNED0= navIn->asLong(offset+esbURAned0,enbURAned0,escURAned0);
      eph->uraNED1= navIn->asUnsignedLong(offset+esbURAned1,enbURAned1,
                                          escURAned1);
      eph->uraNED2= navIn->asUnsignedLong(offset+esbURAned2,enbURAned2,
                                          escURAned2);
         // We have to decode tgd twice unfortunately because a
         // special bit pattern is used to indicate tgd is invalid.
      unsigned long tgdBits = navIn->asUnsignedLong(offset+esbTGD,enbTGD,1);
      if (tgdBits == 0x1000)
      {
            // set to NaN because tgd is not available
         eph->tgd = std::numeric_limits<float>::quiet_NaN();
      }
      else
      {
         eph->tgd = navIn->asSignedDouble(offset+esbTGD,enbTGD,escTGD);
      }
      eph->iscL1CP = navIn->asSignedDouble(offset+esbISCL1CP,enbISCL1CP,
                                           escISCL1CP);
      eph->iscL1CD = navIn->asSignedDouble(offset+esbISCL1CD,enbISCL1CD,
                                           escISCL1CD);
      eph->integStat = navIn->asBool(offset+esbISF);
      eph->fixFit();
      // cerr << "add CNAV2 eph" << endl;
      navOut.push_back(p0);
      return true;
   }


   bool PNBGPSCNav2DataFactory ::
   processSF3(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
              unsigned offset)
   {
      unsigned long pageNum = navIn->asUnsignedLong(offset+asbPage,anbPage,
                                                    ascPage);
      switch (pageNum)
      {
         case npgUTC:
            return processUTCIono(navIn, navOut, offset);
            break;
         case npgGGTO:
            return processGGTOEOP(navIn, navOut, offset);
            break;
         case npgMAlm:
            return processAlmOrb(navIn, navOut, offset);
            break;
         default:
               // Just ignore everything else.
            return true;
      }
   }


   bool PNBGPSCNav2DataFactory ::
   processAlmOrb(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
                 unsigned offset)
   {
      unsigned long sprn = navIn->asUnsignedLong(offset+asbPRNa,anbPRNa,
                                                 ascPRNa);
      SatID xmitSat(navIn->getsatSys());
      SatelliteSystem subjSys = xmitSat.system;
      SatID subjSat(sprn, subjSys);
         // No checks for correct svid, just assume that the input
         // data has already been checked (it will have been by
         // addData).
      if (processHea)
      {
            // Add almanac health bits from message type 37.
         NavDataPtr p1L1 = std::make_shared<GPSCNav2Health>();
         NavDataPtr p1L2 = std::make_shared<GPSCNav2Health>();
         NavDataPtr p1L5 = std::make_shared<GPSCNav2Health>();
         p1L1->timeStamp = navIn->getTransmitTime();
         p1L2->timeStamp = navIn->getTransmitTime();
         p1L5->timeStamp = navIn->getTransmitTime();
            /** @todo I'm not entirely sure what's appropriate here.
             * The source signal is actually L1C CNAV2 but it's
             * broadcasting signal status for L2 signals that don't
             * have CNAV2. */
         p1L1->signal = NavMessageID(
            NavSatelliteID(subjSat, xmitSat, navIn->getobsID(),
                           navIn->getNavID()),
            NavMessageType::Health);
         p1L2->signal = NavMessageID(
            NavSatelliteID(subjSat, xmitSat,
                           ObsID(ObservationType::NavMsg, CarrierBand::L2,
                                 TrackingCode::L2CM),
                           NavID(NavType::GPSCNAVL2)),
            NavMessageType::Health);
         p1L5->signal = NavMessageID(
            NavSatelliteID(subjSat, xmitSat,
                           ObsID(ObservationType::NavMsg, CarrierBand::L5,
                                 TrackingCode::L5I),
                           NavID(NavType::GPSCNAVL5)),
            NavMessageType::Health);
         dynamic_cast<GPSCNav2Health*>(p1L1.get())->health =
            navIn->asBool(offset+asbHeaL1);
         dynamic_cast<GPSCNav2Health*>(p1L2.get())->health =
            navIn->asBool(offset+asbHeaL2);
         dynamic_cast<GPSCNav2Health*>(p1L5.get())->health =
            navIn->asBool(offset+asbHeaL5);
         dynamic_cast<GPSCNav2Health*>(p1L1.get())->setEph(false);
         dynamic_cast<GPSCNav2Health*>(p1L2.get())->setEph(false);
         dynamic_cast<GPSCNav2Health*>(p1L5.get())->setEph(false);
         // cerr << "add CNAV2 alm health" << endl;
         navOut.push_back(p1L1);
         // cerr << "add CNAV2 alm health" << endl;
         navOut.push_back(p1L2);
         // cerr << "add CNAV2 alm health" << endl;
         navOut.push_back(p1L5);
      }
      if (!PNBNavDataFactory::processAlm)
      {
            // User doesn't want almanacs so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNav2Alm>();
      GPSCNav2Alm *alm = dynamic_cast<GPSCNav2Alm*>(p0.get());
         // NavData
      alm->timeStamp = navIn->getTransmitTime();
      alm->signal = NavMessageID(
         NavSatelliteID(subjSat, xmitSat, navIn->getobsID(),
                        navIn->getNavID()),
         NavMessageType::Almanac);
         // OrbitData = empty
         // OrbitDataKepler
      alm->xmitTime = alm->timeStamp;
         /** @todo apply 13-bit week rollover adjustment, not 10-bit.
          * Must be completed by January, 2137 :-) */
      alm->wna = navIn->asUnsignedLong(offset+asbWNa,anbWNa,ascWNa);
      alm->toa = navIn->asUnsignedDouble(offset+asbtoa,anbtoa,asctoa);
      alm->Toc = alm->Toe = GPSWeekSecond(alm->wna,alm->toa);
      if (navIn->getsatSys().system == gpstk::SatelliteSystem::QZSS)
      {
         alm->Toe.setTimeSystem(gpstk::TimeSystem::QZS);
         alm->Toc.setTimeSystem(gpstk::TimeSystem::QZS);
      }
      alm->M0 = navIn->asDoubleSemiCircles(offset+asbM0,anbM0,ascM0);
      alm->ecc = navIn->asUnsignedDouble(offset+asbEcc,anbEcc,ascEcc);
      alm->Ahalf = navIn->asUnsignedDouble(offset+asbAhalf,anbAhalf,ascAhalf);
      alm->A = alm->Ahalf * alm->Ahalf;
      alm->OMEGA0 = navIn->asDoubleSemiCircles(offset+asbOMEGA0,anbOMEGA0,
                                               ascOMEGA0);
         // i0 is set below
      alm->w = navIn->asDoubleSemiCircles(offset+asbw,anbw,ascw);
      alm->OMEGAdot = navIn->asDoubleSemiCircles(offset+asbOMEGAdot,anbOMEGAdot,
                                                 ascOMEGAdot);
      alm->af0 = navIn->asSignedDouble(offset+asbaf0,anbaf0,ascaf0);
      alm->af1 = navIn->asSignedDouble(offset+asbaf1,anbaf1,ascaf1);
         // GPSCNav2Alm
      alm->healthL1 = navIn->asBool(offset+asbHeaL1);
      alm->healthL2 = navIn->asBool(offset+asbHeaL2);
      alm->healthL5 = navIn->asBool(offset+asbHeaL5);
      alm->health = ((alm->healthL1 == false) ? SVHealth::Healthy :
                     SVHealth::Unhealthy);
      alm->deltai = navIn->asDoubleSemiCircles(offset+asbdi,anbdi,ascdi);
      if (alm->signal.sat.system == SatelliteSystem::QZSS)
      {
            /** @todo GEO QZSS satellites use a different i0
             * reference, but I have yet to figure out how to
             * determine if a QZSS satellite is GEO or QZO */
         alm->i0 = GPSCNav2Alm::refi0QZSS + alm->deltai;
      }
      else
      {
         alm->i0 = GPSCNav2Alm::refi0GPS + alm->deltai;
      }
      alm->fixFit();
      // cerr << "add CNAV2 alm" << endl;
      navOut.push_back(p0);
      return true;
   }


   bool PNBGPSCNav2DataFactory ::
   processUTCIono(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
                  unsigned offset)
   {
      if (PNBNavDataFactory::processIono)
      {
         NavDataPtr p1 = std::make_shared<GPSCNav2Iono>();
         GPSCNav2Iono *iono = dynamic_cast<GPSCNav2Iono*>(p1.get());
            // NavData
         p1->timeStamp = navIn->getTransmitTime();
         p1->signal = NavMessageID(
            NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                           navIn->getobsID(), navIn->getNavID()),
            NavMessageType::Iono);
            // KlobucharIonoData
         iono->alpha[0] = navIn->asSignedDouble(csbAlpha0,cnbAlpha0,cscAlpha0);
         iono->alpha[1] = navIn->asSignedDouble(csbAlpha1,cnbAlpha1,cscAlpha1);
         iono->alpha[2] = navIn->asSignedDouble(csbAlpha2,cnbAlpha2,cscAlpha2);
         iono->alpha[3] = navIn->asSignedDouble(csbAlpha3,cnbAlpha3,cscAlpha3);
         iono->beta[0] = navIn->asSignedDouble(csbBeta0,cnbBeta0,cscBeta0);
         iono->beta[1] = navIn->asSignedDouble(csbBeta1,cnbBeta1,cscBeta1);
         iono->beta[2] = navIn->asSignedDouble(csbBeta2,cnbBeta2,cscBeta2);
         iono->beta[3] = navIn->asSignedDouble(csbBeta3,cnbBeta3,cscBeta3);
            // GPSCNav2Iono
         navOut.push_back(p1);
      }
      if (!PNBNavDataFactory::processTim)
      {
            // User doesn't want time offset data so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNav2TimeOffset>();
      p0->timeStamp = navIn->getTransmitTime();
      p0->signal = NavMessageID(
         NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                               navIn->getobsID(), navIn->getNavID()),
         NavMessageType::TimeOffset);
      GPSCNav2TimeOffset *to =
         dynamic_cast<GPSCNav2TimeOffset*>(p0.get());
      to->tgt = TimeSystem::UTC; // by definition
      to->a0 = navIn->asSignedDouble(offset+csbA0,cnbA0,cscA0);
      to->a1 = navIn->asSignedDouble(offset+csbA1,cnbA1,cscA1);
      to->a2 = navIn->asSignedDouble(offset+csbA2,cnbA2,cscA2);
      to->deltatLS = navIn->asLong(offset+csbdtLS,cnbdtLS,cscdtLS);
      to->tot = navIn->asUnsignedDouble(offset+csbtot,cnbtot,csctot);
      to->wnot = navIn->asUnsignedLong(offset+csbWNot,cnbWNot,cscWNot);
      to->refTime = gpstk::GPSWeekSecond(to->wnot, to->tot);
      if (navIn->getsatSys().system == gpstk::SatelliteSystem::QZSS)
      {
         to->src = gpstk::TimeSystem::QZS;
         to->refTime.setTimeSystem(gpstk::TimeSystem::QZS);
      }
      to->wnLSF = navIn->asUnsignedLong(offset+csbWNlsf,cnbWNlsf,cscWNlsf);
      to->dn = navIn->asUnsignedLong(offset+csbDN,cnbDN,cscDN);
      to->deltatLSF = navIn->asLong(offset+csbdtLSF,cnbdtLSF,cscdtLSF);
      navOut.push_back(p0);
      return true;
   }


   bool PNBGPSCNav2DataFactory ::
   processGGTOEOP(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
                  unsigned offset)
   {
      if (!PNBNavDataFactory::processTim)
      {
            // User doesn't want time offset data so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNav2TimeOffset>();
      p0->timeStamp = navIn->getTransmitTime();
      p0->signal = NavMessageID(
         NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                               navIn->getobsID(), navIn->getNavID()),
         NavMessageType::TimeOffset);
      GPSCNav2TimeOffset *to =
         dynamic_cast<GPSCNav2TimeOffset*>(p0.get());
      uint8_t gnssID = navIn->asUnsignedLong(offset+gsbGNSS,gnbGNSS,gscGNSS);
      switch (gnssID)
      {
         case 0:
               // no data available.
            return true;
         case 1:
            to->tgt = TimeSystem::GAL;
            break;
         case 2:
            to->tgt = TimeSystem::GLO;
            break;
         case 3:
               /** @note this is defined in IS-QZSS.  Not sure if we
                * should really be checking to make sure the signal is
                * from a QZSS satellite. */
            to->tgt = TimeSystem::QZS;
            break;
         default:
               // unknown/unsupported
            return false;
      }
      to->tot = navIn->asUnsignedDouble(offset+gsbt,gnbt,gsct);
      to->wnot = navIn->asUnsignedLong(offset+gsbWN,gnbWN,gscWN);
      to->refTime = gpstk::GPSWeekSecond(to->wnot, to->tot);
      if (navIn->getsatSys().system == gpstk::SatelliteSystem::QZSS)
      {
            // source time system is supposed to be GPS even for QZSS,
            // so don't set to->src
         to->refTime.setTimeSystem(gpstk::TimeSystem::QZS);
      }
      to->a0 = navIn->asSignedDouble(offset+gsbA0,gnbA0,gscA0);
      to->a1 = navIn->asSignedDouble(offset+gsbA1,gnbA1,gscA1);
      to->a2 = navIn->asSignedDouble(offset+gsbA2,gnbA2,gscA2);
      navOut.push_back(p0);
      return true;
   }

} // namespace gpstk
