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
#include "PNBGPSCNav2DataFactory.hpp"

#include <cmath>
#include <memory>

#include "GPSNavConfig.hpp"
#include "GPSCNav2Alm.hpp"
// #include "GPSCNav2RedAlm.hpp"
#include "GPSCNav2Eph.hpp"
#include "GPSCNav2Health.hpp"
#include "GPSCNav2TimeOffset.hpp"
#include "GPSCNav2Iono.hpp"
#include "SatID.hpp"
#include "TimeCorrection.hpp"
#include "YDSTime.hpp"
#include "GPSC2Bits.hpp"

using namespace std;
using namespace gnsstk::gpscnav2;

/// Epsilon value for checking timestamps of nav data (in seconds).
constexpr double TIME_EPSILON = 1e-9;
/// CNAV2 frame length in seconds.
constexpr double CNAV2_FRAME_LEN = 18.0;
/// Offset in seconds of SF2 from start of CNAV2 frame.
constexpr double CNAV2_SF2_OFFS = 0.52;
/// Offset in seconds of SF3 from start of CNAV2 frame.
constexpr double CNAV2_SF3_OFFS = 12.52;

namespace gnsstk
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
               rv = processEph(navIn, navOut) && processISC(navIn, navOut);
               break;
            case nnbSF3:
               rv = processSF3(navIn, navOut) && processISC(navIn, navOut);
               break;
            case nnbComplete:
               rv = processEph(navIn, navOut, nnbSF1) &&
                  processSF3(navIn, navOut, nnbSF1+nnbSF2) &&
                  processISC(navIn, navOut);
               break;
            default:
               return true;
         }
      }
      catch (gnsstk::Exception& exc)
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
         p1->timeStamp = getSF2Time(navIn->getTransmitTime());
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
      eph->timeStamp = getSF2Time(navIn->getTransmitTime());
      eph->signal = NavMessageID(
         NavSatelliteID(prn, navIn->getsatSys(), navIn->getobsID(),
                        navIn->getNavID()),
         NavMessageType::Ephemeris);
         // OrbitData = empty
         // OrbitDataKepler
      eph->xmitTime = getSF2Time(eph->timeStamp);
         /** @todo apply 13-bit week rollover adjustment, not 10-bit.
          * Must be completed by January, 2137 :-) */
      long wn = navIn->asUnsignedLong(offset+esbWN,enbWN,escWN);
      double toe = navIn->asUnsignedLong(offset+esbtoe,enbtoe,esctoe);
      eph->itow = navIn->asUnsignedLong(offset+esbITOW,enbITOW,escITOW);
      eph->Toe = eph->Toc = GPSWeekSecond(wn,toe);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)
      {
         eph->Toe.setTimeSystem(gnsstk::TimeSystem::QZS);
         eph->Toc.setTimeSystem(gnsstk::TimeSystem::QZS);
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
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)
      {
         eph->top.setTimeSystem(gnsstk::TimeSystem::QZS);
      }
      eph->uraNED0= navIn->asLong(offset+esbURAned0,enbURAned0,escURAned0);
      eph->uraNED1= navIn->asUnsignedLong(offset+esbURAned1,enbURAned1,
                                          escURAned1);
      eph->uraNED2= navIn->asUnsignedLong(offset+esbURAned2,enbURAned2,
                                          escURAned2);
      eph->tgd = InterSigCorr::getGPSISC(navIn, offset+esbTGD);
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
#if false /// @todo enable this if the SV config page is ever observed
         case npgSVConfig:
            return processSVConfig(navIn, navOut, offset);
#endif
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
         p1L1->timeStamp =
            p1L2->timeStamp =
            p1L5->timeStamp = getSF3Time(navIn->getTransmitTime());
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
      alm->timeStamp = getSF3Time(navIn->getTransmitTime());
      alm->signal = NavMessageID(
         NavSatelliteID(subjSat, xmitSat, navIn->getobsID(),
                        navIn->getNavID()),
         NavMessageType::Almanac);
         // OrbitData = empty
         // OrbitDataKepler
      alm->xmitTime = getSF3Time(alm->timeStamp);
         /** @todo apply 13-bit week rollover adjustment, not 10-bit.
          * Must be completed by January, 2137 :-) */
      alm->wna = navIn->asUnsignedLong(offset+asbWNa,anbWNa,ascWNa);
      alm->toa = navIn->asUnsignedDouble(offset+asbtoa,anbtoa,asctoa);
      alm->Toc = alm->Toe = GPSWeekSecond(alm->wna,alm->toa);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)
      {
         alm->Toe.setTimeSystem(gnsstk::TimeSystem::QZS);
         alm->Toc.setTimeSystem(gnsstk::TimeSystem::QZS);
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
         p1->timeStamp = getSF3Time(navIn->getTransmitTime());
         p1->signal = NavMessageID(
            NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                           navIn->getobsID(), navIn->getNavID()),
            NavMessageType::Iono);
            // KlobucharIonoNavData
         iono->alpha[0] = navIn->asSignedDouble(offset+csbAlpha0,cnbAlpha0,
                                                cscAlpha0);
         iono->alpha[1] = navIn->asSignedDouble(offset+csbAlpha1,cnbAlpha1,
                                                cscAlpha1);
         iono->alpha[2] = navIn->asSignedDouble(offset+csbAlpha2,cnbAlpha2,
                                                cscAlpha2);
         iono->alpha[3] = navIn->asSignedDouble(offset+csbAlpha3,cnbAlpha3,
                                                cscAlpha3);
         iono->beta[0] = navIn->asSignedDouble(offset+csbBeta0,cnbBeta0,
                                               cscBeta0);
         iono->beta[1] = navIn->asSignedDouble(offset+csbBeta1,cnbBeta1,
                                               cscBeta1);
         iono->beta[2] = navIn->asSignedDouble(offset+csbBeta2,cnbBeta2,
                                               cscBeta2);
         iono->beta[3] = navIn->asSignedDouble(offset+csbBeta3,cnbBeta3,
                                               cscBeta3);
            // GPSCNav2Iono
         navOut.push_back(p1);
      }
      if (!PNBNavDataFactory::processTim)
      {
            // User doesn't want time offset data so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNav2TimeOffset>();
      p0->timeStamp = getSF3Time(navIn->getTransmitTime());
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
      to->refTime = GPSWeekSecond(to->wnot, to->tot);
      to->wnLSF = navIn->asUnsignedLong(offset+csbWNlsf,cnbWNlsf,cscWNlsf);
      to->dn = navIn->asUnsignedLong(offset+csbDN,cnbDN,cscDN);
      to->effTime = GPSWeekSecond(to->wnLSF, (to->dn-1)*86400);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)
      {
         to->src = gnsstk::TimeSystem::QZS;
         to->refTime.setTimeSystem(gnsstk::TimeSystem::QZS);
         to->effTime.setTimeSystem(gnsstk::TimeSystem::QZS);
      }
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
      p0->timeStamp = getSF3Time(navIn->getTransmitTime());
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
      to->refTime = gnsstk::GPSWeekSecond(to->wnot, to->tot);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)
      {
            // source time system is supposed to be GPS even for QZSS,
            // so don't set to->src
         to->refTime.setTimeSystem(gnsstk::TimeSystem::QZS);
      }
      to->a0 = navIn->asSignedDouble(offset+gsbA0,gnbA0,gscA0);
      to->a1 = navIn->asSignedDouble(offset+gsbA1,gnbA1,gscA1);
      to->a2 = navIn->asSignedDouble(offset+gsbA2,gnbA2,gscA2);
      navOut.push_back(p0);
      return true;
   }


#if false // TODO: enable this if the SV config page is ever observed
   bool PNBGPSCNav2DataFactory ::
   processSVConfig(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
                   unsigned offset)
   {
      if (!processSys)
      {
            // User doesn't want time offset data so don't do any processing.
         return true;
      }

      for (unsigned prn = 1; prn <= numSVConfs; ++prn)
      {
         auto configPtr{std::make_shared<GPSNavConfig>()};
         configPtr->timeStamp = getSF3Time(navIn->getTransmitTime());
         configPtr->signal = NavMessageID{
            NavSatelliteID{
               prn, navIn->getsatSys(), navIn->getobsID(), navIn->getNavID()},
            NavMessageType::System};

         configPtr->antispoofOn = 
            navIn->asBool(offset + asbConf1 + anbConfLen * (prn - 1));
         configPtr->svConfig = navIn->asUnsignedLong(
            offset + asbConf1 + anbConfLen * (prn - 1) + 1, 3, 1);

         navOut.emplace_back(configPtr);         
      }
      return true;
   }
#endif


   bool PNBGPSCNav2DataFactory ::
   processISC(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      if (!PNBNavDataFactory::processISC)
         return true;
      NavSatelliteID nsid(navIn->getsatSys().id, navIn->getsatSys(),
                          navIn->getobsID(), navIn->getNavID());
      GPSCNav2ISCPtr isc = getISCObj(navIn, nsid);
         // offset for subframe 3
      const unsigned offs = nnbSF1+nnbSF2;
      switch (navIn->getNumBits())
      {
         case nnbSF2:
            if (isc->haveSF2 && !isc->haveSF3)
            {
                  // Two sequential subframe 2 with no subframe 3,
                  // publish the previous subframe 2 data.
               navOut.push_back(std::make_shared<GPSCNav2ISC>(*isc));
            }
            isc->timeStamp = isc->xmit2 = getSF2Time(navIn->getTransmitTime());
            isc->haveSF2 = true;
            isc->isc = InterSigCorr::getGPSISC(navIn, esbTGD);
            isc->iscL1CP = InterSigCorr::getGPSISC(navIn, esbISCL1CP);
            isc->iscL1CD = InterSigCorr::getGPSISC(navIn, esbISCL1CD);
            if (isc->haveSF3)
            {
                  // We have a complete set of ISCs, so immediately
                  // add it to the output
               navOut.push_back(std::make_shared<GPSCNav2ISC>(*isc));
            }
            break;
         case nnbSF3:
               // It's theoretically possible to process multiple
               // sequential subframe 3, page 1, but I don't think
               // it's useful without the Tgd in subframe 2.

               // only process page 1 of subframe 3
            if (navIn->asUnsignedLong(asbPage,anbPage,ascPage) != 1)
               break;
            isc->timeStamp = isc->xmit3 = getSF3Time(navIn->getTransmitTime());
            isc->haveSF3 = true;
            isc->iscL1CA = InterSigCorr::getGPSISC(navIn, csbISCL1CA);
            isc->iscL2C = InterSigCorr::getGPSISC(navIn, csbISCL2C);
            isc->iscL5I5 = InterSigCorr::getGPSISC(navIn, csbISCL5I5);
            isc->iscL5Q5 = InterSigCorr::getGPSISC(navIn, csbISCL5Q5);
            if (isc->haveSF2)
            {
                  // We have a complete set of ISCs, so immediately
                  // add it to the output
               navOut.push_back(std::make_shared<GPSCNav2ISC>(*isc));
            }
            break;
         case nnbComplete:
            if (isc->haveSF2 && !isc->haveSF3)
            {
                  // Two sequential subframe 2 with no subframe 3,
                  // publish the previous subframe 2 data.
               navOut.push_back(std::make_shared<GPSCNav2ISC>(*isc));
            }
            isc->timeStamp = getSF3Time(navIn->getTransmitTime());
            isc->xmit2 = getSF2Time(navIn->getTransmitTime());
            isc->xmit3 = isc->timeStamp;
            isc->haveSF2 = true;
            isc->isc = InterSigCorr::getGPSISC(navIn, nnbSF1+esbTGD);
            isc->iscL1CP = InterSigCorr::getGPSISC(navIn, nnbSF1+esbISCL1CP);
            isc->iscL1CD = InterSigCorr::getGPSISC(navIn, nnbSF1+esbISCL1CD);
            if (isc->haveSF3 &&
                (navIn->asUnsignedLong(asbPage,anbPage,ascPage) != 1))
            {
                  // We have a complete set of ISCs and the subframe 3
                  // page for the complete message isn't page 1, so
                  // immediately add it to the output
               navOut.push_back(std::make_shared<GPSCNav2ISC>(*isc));
            }
            else if (navIn->asUnsignedLong(offs+asbPage,anbPage,ascPage) == 1)
            {
                  // Got a subframe 3 page 1, so decode it, store it,
                  // and output it.
               isc->timeStamp = getSF3Time(navIn->getTransmitTime());
               isc->haveSF3 = true;
               isc->iscL1CA = InterSigCorr::getGPSISC(navIn, offs+csbISCL1CA);
               isc->iscL2C = InterSigCorr::getGPSISC(navIn, offs+csbISCL2C);
               isc->iscL5I5 = InterSigCorr::getGPSISC(navIn, offs+csbISCL5I5);
               isc->iscL5Q5 = InterSigCorr::getGPSISC(navIn, offs+csbISCL5Q5);
               navOut.push_back(std::make_shared<GPSCNav2ISC>(*isc));
            }
            break;
         default:
            return true;
      }
      return true;
   }


   PNBGPSCNav2DataFactory::GPSCNav2ISCPtr PNBGPSCNav2DataFactory ::
   getISCObj(const PackedNavBitsPtr& navIn, const NavSatelliteID& nsid)
   {
      if (iscAcc.find(nsid) == iscAcc.end())
      {
         iscAcc[nsid] = std::make_shared<GPSCNav2ISC>();
         GPSCNav2ISCPtr &isc(iscAcc[nsid]);
         isc->signal = NavMessageID(nsid, NavMessageType::ISC);
      }
      return iscAcc[nsid];
   }


   CommonTime PNBGPSCNav2DataFactory ::
   getSF2Time(const CommonTime& timestamp)
   {
         // Convert to YDS which is easier to manipulate since it's
         // evenly divisible by 18 seconds, the period of a CNAV-2
         // frame.
      YDSTime yds(timestamp);
      if (fabs(remainder(yds.sod,CNAV2_FRAME_LEN)) < TIME_EPSILON)
      {
            // given time stamp is for subframe 1, add the offset for sf2
         return timestamp + CNAV2_SF2_OFFS;
      }
      return timestamp;
   }


   CommonTime PNBGPSCNav2DataFactory ::
   getSF3Time(const CommonTime& timestamp)
   {
         // Convert to YDS which is easier to manipulate since it's
         // evenly divisible by 18 seconds, the period of a CNAV-2
         // frame.
      YDSTime yds(timestamp);
      if (fabs(remainder(yds.sod,CNAV2_FRAME_LEN)) < TIME_EPSILON)
      {
            // given time stamp is for subframe 1, add the offset for sf3
         return timestamp + CNAV2_SF3_OFFS;
      }
      return timestamp;
   }

} // namespace gnsstk
