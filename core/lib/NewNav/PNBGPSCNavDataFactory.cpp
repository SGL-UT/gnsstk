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
#include <math.h>
#include "PNBGPSCNavDataFactory.hpp"
#include "GPSCNavAlm.hpp"
#include "GPSCNavRedAlm.hpp"
#include "GPSCNavEph.hpp"
#include "GPSCNavHealth.hpp"
#include "GPSCNavTimeOffset.hpp"
#include "GPSCNavIono.hpp"
#include "GPSCNavISC.hpp"
#include "TimeCorrection.hpp"
#include "GPSCBits.hpp"

using namespace std;
using namespace gnsstk::gpscnav;

namespace gnsstk
{
   bool PNBGPSCNavDataFactory ::
   addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
           double cadence)
   {
      if ((navIn->getNavID().navType != NavType::GPSCNAVL2) &&
          (navIn->getNavID().navType != NavType::GPSCNAVL5))
      {
            // This class only processes GPS CNav.
         return false;
      }
      bool rv = true;
      bool isQZSS = navIn->getsatSys().system == SatelliteSystem::QZSS;
      try
      {
         // cerr << "preamble:  " << hex << navIn->asUnsignedLong(0,8,1) << dec
         //      << endl;
         // unsigned long prn = navIn->asUnsignedLong(8, 6, 1);
         // cerr << "prn = " << prn << endl;
         unsigned long msgType = navIn->asUnsignedLong(esbMsgType,enbMsgType,
                                                       escMsgType);
         // cerr << "msgType = " << msgType << endl;
         unsigned long svid = 0;
            // Clock messages (30-37) may get processed twice, once
            // for ephemeris and once for whatever else they might
            // contain.
         if ((msgType == 10) || (msgType == 11) ||
             ((msgType >= 30) && (msgType <= 37)) ||
             (isQZSS && ((msgType == 46) || (msgType == 47) ||
                         (msgType == 49) || (msgType == 51) ||
                         (msgType == 53) || (msgType == 61))))
         {
            rv = rv && processEph(msgType, navIn, navOut);
         }
         if ((msgType == 31) || (isQZSS && (msgType == 47)))
         {
            rv = rv && process31(msgType, navIn, navOut);
         }
         else if ((msgType == 30) || (isQZSS && (msgType == 61)))
         {
            rv = rv && process30(navIn, navOut);
         }
         else if ((msgType == 12) || (isQZSS && (msgType == 28)))
         {
            rv = rv && process12(msgType, navIn, navOut);
         }
         else if ((msgType == 33) || (isQZSS && (msgType == 49)))
         {
            rv = rv && process33(navIn, navOut);
         }
         else if ((msgType == 35) || (isQZSS && (msgType == 51)))
         {
            rv = rv && process35(navIn, navOut);
         }
         else if ((msgType == 37) || (isQZSS && (msgType == 53)))
         {
            rv = rv && processAlmOrb(msgType, navIn, navOut);
         }
         // cerr << "  results: " << navOut.size() << endl;
         // for (const auto& i : navOut)
         //    i->dump(cerr,gnsstk::DumpDetail::Full);
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


   bool PNBGPSCNavDataFactory ::
   processEph(unsigned msgType, const PackedNavBitsPtr& navIn,
              NavDataPtrList& navOut)
   {
         /** @todo Should we ignore the PNB satellite ID and just use
          * the PRN in the nav message, or maybe compare the two as an
          * validity check? */
      unsigned prn = navIn->getsatSys().id;
      unsigned vecIdx;
      switch (msgType)
      {
         case 10:
            vecIdx = ephM10;
            break;
         case 11:
            vecIdx = ephM11;
            break;
         case 30:
         case 31:
         case 32:
         case 33:
         case 34:
         case 35:
         case 36:
         case 37:
               /** @todo Is this strictly relevant for ephemeris
                * assembly?  Seems like if these are rebroadcast
                * messages they're not going to correspond to the
                * ephemeris data being broadcast by QZSS. */
         case 46: // QZSS rebroadcast of GPS MT 30
         case 47: // QZSS rebroadcast of GPS MT 31
         case 49: // QZSS rebroadcast of GPS MT 33
         case 51: // QZSS rebroadcast of GPS MT 35
         case 53: // QZSS rebroadcast of GPS MT 37
         case 61: // QZSS "Japan area" ionospheric parameters
            vecIdx = ephMClk;
            break;
         default:
            return false;
      }
      if ((msgType == 10) && processHea)
      {
            // Add ephemeris health bits from message type 10.
         NavDataPtr p1L1 = std::make_shared<GPSCNavHealth>();
         NavDataPtr p1L2 = std::make_shared<GPSCNavHealth>();
         NavDataPtr p1L5 = std::make_shared<GPSCNavHealth>();
         p1L1->timeStamp = navIn->getTransmitTime();
         p1L2->timeStamp = navIn->getTransmitTime();
         p1L5->timeStamp = navIn->getTransmitTime();
            /** @todo I'm not entirely sure what's appropriate here.
             * The source signal is actually L2C CNAV but it's
             * broadcasting signal status for L1 signals that don't
             * have CNAV. */
         p1L1->signal = NavMessageID(
            NavSatelliteID(prn, prn, navIn->getsatSys().system,
                           CarrierBand::L1, TrackingCode::CA,
                           NavType::GPSLNAV),
            NavMessageType::Health);
         if (navIn->getobsID().band == CarrierBand::L2)
         {
            p1L2->signal = NavMessageID(
               NavSatelliteID(prn, navIn->getsatSys(), navIn->getobsID(),
                              navIn->getNavID()),
               NavMessageType::Health);
            p1L5->signal = NavMessageID(
               NavSatelliteID(prn, prn, navIn->getsatSys().system,
                              CarrierBand::L5, TrackingCode::L5I,
                              NavType::GPSCNAVL5),
               NavMessageType::Health);
         }
         else
         {
               // assume L5
            p1L2->signal = NavMessageID(
               NavSatelliteID(prn, prn, navIn->getsatSys().system,
                              CarrierBand::L2, TrackingCode::L2CM,
                              NavType::GPSCNAVL2),
               NavMessageType::Health);
            p1L5->signal = NavMessageID(
               NavSatelliteID(prn, navIn->getsatSys(), navIn->getobsID(),
                              navIn->getNavID()),
               NavMessageType::Health);
         }
         dynamic_cast<GPSCNavHealth*>(p1L1.get())->health =
            navIn->asBool(esbHeaL1);
         dynamic_cast<GPSCNavHealth*>(p1L2.get())->health =
            navIn->asBool(esbHeaL2);
         dynamic_cast<GPSCNavHealth*>(p1L5.get())->health =
            navIn->asBool(esbHeaL5);
         // cerr << "add CNAV eph health" << endl;
         navOut.push_back(p1L1);
         // cerr << "add CNAV eph health" << endl;
         navOut.push_back(p1L2);
         // cerr << "add CNAV eph health" << endl;
         navOut.push_back(p1L5);
      }
      if (!PNBNavDataFactory::processEph)
      {
            // User doesn't want ephemerides so don't do any processing.
         return true;
      }
      if (ephAcc.find(prn) == ephAcc.end())
      {
            // set up a new ephemeris
         ephAcc[prn].resize(3);
         ephAcc[prn][vecIdx] = navIn;
            // nothing in navOut yet and no further processing because
            // we only have one of three message types at this point.
         return true;
      }
      std::vector<PackedNavBitsPtr> &ephSF(ephAcc[prn]);
      ephSF[vecIdx] = navIn;
         // stop processing if we don't have three full message types
      if (!ephSF[ephM10] || !ephSF[ephM11] || !ephSF[ephMClk] ||
          (ephSF[ephM10]->getNumBits() != 300) ||
          (ephSF[ephM11]->getNumBits() != 300) ||
          (ephSF[ephMClk]->getNumBits() != 300))
      {
         // cerr << "Not ready for full CNAV eph processing" << endl;
         return true;
      }
         // Stop processing if we don't have matching toe/toc in
         // each of the three message types.
         /** @note Some data elements, e.g. toe, are stored internally
          * as floating point, but are treated as integers in the
          * encoded message.  This is not a mistake.  It's mostly due
          * to how the scaling is handled for the quantity,
          * i.e. linear scaling vs fractional (ldexp). */
      double toe10 = ephSF[esitoe1]->asUnsignedLong(esbtoe1,enbtoe1,esctoe1);
      double toe11 = ephSF[esitoe2]->asUnsignedLong(esbtoe2,enbtoe2,esctoe2);
      double toc = ephSF[csitoc]->asUnsignedLong(csbtoc,cnbtoc,csctoc);
      if ((toe10 != toe11) || (toe10 != toc))
      {
         // cerr << "toe/toc mismatch, not processing" << endl;
            // Even though the mismatch might be considered an error,
            // we don't really want to mark it as such and rather
            // consider it as a "valid" but unprocessable data set.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNavEph>();
      GPSCNavEph *eph = dynamic_cast<GPSCNavEph*>(p0.get());
         // NavData
      eph->timeStamp = ephSF[ephM10]->getTransmitTime();
      eph->signal = NavMessageID(
         NavSatelliteID(prn, navIn->getsatSys(), navIn->getobsID(),
                        navIn->getNavID()),
         NavMessageType::Ephemeris);
      // cerr << "Ready for full CNAV eph processing for " << (NavSignalID)eph->signal << endl;
         // OrbitData = empty
         // OrbitDataKepler
      eph->xmitTime = eph->timeStamp;
         /** @todo apply 13-bit week rollover adjustment, not 10-bit.
          * Must be completed by January, 2137 :-) */
      unsigned wn = ephSF[esiWN]->asUnsignedLong(esbWN,enbWN,escWN);
         // Use the transmit time to get a full week for toe/toc
      GPSWeekSecond refTime(eph->xmitTime);
      long refWeek = refTime.week;
      wn = timeAdjustWeekRollover(wn, refWeek);
         // Now we can set the Toe/Toc properly.  Note that IS-GPS-200
         // defines the toc and toe to be the same for a consistent
         // set of data, and we've already enforced they're the same
         // above.
      eph->Toe = eph->Toc = GPSWeekSecond(wn,toe10).weekRolloverAdj(refTime);
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
      eph->M0  = ephSF[esiM0]->asDoubleSemiCircles(esbM0,enbM0,escM0);
      eph->dn  = ephSF[esidn0]->asDoubleSemiCircles(esbdn0,enbdn0,escdn0);
      eph->dndot = ephSF[esidn0dot]->asDoubleSemiCircles(esbdn0dot,enbdn0dot,
                                                         escdn0dot);
      eph->ecc = ephSF[esiEcc]->asUnsignedDouble(esbEcc,enbEcc,escEcc);
      eph->deltaA = ephSF[esidA]->asSignedDouble(esbdA,enbdA,escdA);
      eph->dOMEGAdot = ephSF[esidOMEGAdot]->asDoubleSemiCircles(
         esbdOMEGAdot,enbdOMEGAdot,escdOMEGAdot);
      if (eph->signal.sat.system == SatelliteSystem::QZSS)
      {
         eph->A = eph->deltaA + GPSCNavData::refAQZSS;
            /** @todo GEO QZSS satellites use a different OMEGAdot
             * reference, but I have yet to figure out how to
             * determine if a QZSS satellite is GEO or QZO */
         eph->OMEGAdot = eph->dOMEGAdot + GPSCNavData::refOMEGAdotEphQZSS;
      }
      else
      {
         eph->A = eph->deltaA + GPSCNavData::refAGPS;
         eph->OMEGAdot = eph->dOMEGAdot + GPSCNavData::refOMEGAdotEphGPS;
      }
      eph->Ahalf = ::sqrt(eph->A);
      eph->Adot = ephSF[esiAdot]->asSignedDouble(esbAdot,enbAdot,escAdot);
      eph->OMEGA0 = ephSF[esiOMEGA0]->asDoubleSemiCircles(esbOMEGA0,enbOMEGA0,
                                                          escOMEGA0);
      eph->i0 = ephSF[esii0]->asDoubleSemiCircles(esbi0,enbi0,esci0);
      eph->w = ephSF[esiw]->asDoubleSemiCircles(esbw,enbw,escw);
      eph->idot = ephSF[esiidot]->asDoubleSemiCircles(esbidot,enbidot,escidot);
      eph->af0 = ephSF[csiaf0]->asSignedDouble(csbaf0,cnbaf0,cscaf0);
      eph->af1 = ephSF[csiaf1]->asSignedDouble(csbaf1,cnbaf1,cscaf1);
      eph->af2 = ephSF[csiaf2]->asSignedDouble(csbaf2,cnbaf2,cscaf2);
         // GPSCNavData
      eph->pre = ephSF[ephM10]->asUnsignedLong(esbPre,enbPre,escPre);
      eph->alert = ephSF[ephM10]->asBool(esbAlert);
         // GPSCNavEph
      eph->pre11 = ephSF[ephM11]->asUnsignedLong(esbPre,enbPre,escPre);
      eph->preClk = ephSF[ephMClk]->asUnsignedLong(esbPre,enbPre,escPre);
      eph->healthL1 = ephSF[esiHea]->asBool(esbHeaL1);
      eph->healthL2 = ephSF[esiHea]->asBool(esbHeaL2);
      eph->healthL5 = ephSF[esiHea]->asBool(esbHeaL5);
      switch (navIn->getobsID().band)
      {
         case CarrierBand::L2:
            eph->health = ((eph->healthL2 == false) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            break;
         case CarrierBand::L5:
            eph->health = ((eph->healthL5 == false) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            break;
         default:
               // unexpected/unsupported signal
            return false;
      }
      eph->uraED = ephSF[esiURA]->asLong(esbURA,enbURA,escURA);
      eph->alert11 = ephSF[ephM11]->asBool(esbAlert);
      eph->alertClk = ephSF[ephMClk]->asBool(esbAlert);
      double top = ephSF[esitop]->asUnsignedLong(esbtop,enbtop,esctop);
      eph->top = GPSWeekSecond(wn,top).weekRolloverAdj(eph->Toe);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)
      {
         eph->top.setTimeSystem(gnsstk::TimeSystem::QZS);
      }
      eph->xmit11 = ephSF[ephM11]->getTransmitTime();
      eph->xmitClk = ephSF[ephMClk]->getTransmitTime();
      eph->uraNED0= ephSF[csiURAned0]->asLong(csbURAned0,cnbURAned0,cscURAned0);
      eph->uraNED1= ephSF[csiURAned1]->asUnsignedLong(csbURAned1,cnbURAned1,
                                                      cscURAned1);
      eph->uraNED2= ephSF[csiURAned2]->asUnsignedLong(csbURAned2,cnbURAned2,
                                                      cscURAned2);
      eph->fixFit();
      // cerr << "add CNAV eph" << endl;
      navOut.push_back(p0);
         // Clear out the broadcast ephemeris that's been processed.
      ephAcc.erase(prn);
      return true;
   }


   bool PNBGPSCNavDataFactory ::
   processAlmOrb(unsigned msgType, const PackedNavBitsPtr& navIn,
                 NavDataPtrList& navOut)
   {
      unsigned long sprn = navIn->asUnsignedLong(asbPRNa,anbPRNa,ascPRNa);
      if (sprn == 0)
      {
            // clock data is probably valid but we don't use it.
            // PRN = 0 so don't attempt to process the almanac.
         return true;
      }
      SatID xmitSat(navIn->getsatSys());
      SatelliteSystem subjSys = xmitSat.system;
         // special handling for QZSS per IS-QZSS 1.8E Table 5.5.2-8
      if (subjSys == SatelliteSystem::QZSS)
      {
         if (msgType == 37)
         {
               // When the message type number is 37, it indicates
               // that this value is for a QZS satellite and
               // represents the last 6 bits of the QZS PRN number.
               // .. so we do a bitwise OR to get the QZS PRN.
            sprn |= 0xc0;
         }
         else if (msgType == 53)
         {
               // When the message type number is 53, it indicates
               // that this value is the PRN value for a GPS satellite
            subjSys = SatelliteSystem::GPS;
         }
      }
      SatID subjSat(sprn, subjSys);
         // No checks for correct svid, just assume that the input
         // data has already been checked (it will have been by
         // addData).
      if (processHea)
      {
            // Add almanac health bits from message type 37.
         NavDataPtr p1L1 = std::make_shared<GPSCNavHealth>();
         NavDataPtr p1L2 = std::make_shared<GPSCNavHealth>();
         NavDataPtr p1L5 = std::make_shared<GPSCNavHealth>();
         p1L1->timeStamp = navIn->getTransmitTime();
         p1L2->timeStamp = navIn->getTransmitTime();
         p1L5->timeStamp = navIn->getTransmitTime();
            /** @todo I'm not entirely sure what's appropriate here.
             * The source signal is actually L2C CNAV but it's
             * broadcasting signal status for L1 signals that don't
             * have CNAV. */
         p1L1->signal = NavMessageID(
            NavSatelliteID(subjSat, xmitSat,
                           ObsID(ObservationType::NavMsg, CarrierBand::L1,
                                 TrackingCode::CA),
                           NavID(NavType::GPSLNAV)),
            NavMessageType::Health);
         if (navIn->getobsID().band == CarrierBand::L2)
         {
            p1L2->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat, navIn->getobsID(),
                              navIn->getNavID()),
               NavMessageType::Health);
            p1L5->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat,
                              ObsID(ObservationType::NavMsg, CarrierBand::L5,
                                    TrackingCode::L5I),
                              NavID(NavType::GPSCNAVL5)),
               NavMessageType::Health);
         }
         else
         {
               // assume L5
            p1L2->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat,
                              ObsID(ObservationType::NavMsg, CarrierBand::L2,
                                    TrackingCode::L2CM),
                              NavID(NavType::GPSCNAVL2)),
               NavMessageType::Health);
            p1L5->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat, navIn->getobsID(),
                              navIn->getNavID()),
               NavMessageType::Health);
         }
         dynamic_cast<GPSCNavHealth*>(p1L1.get())->health =
            navIn->asBool(asbHeaL1);
         dynamic_cast<GPSCNavHealth*>(p1L2.get())->health =
            navIn->asBool(asbHeaL2);
         dynamic_cast<GPSCNavHealth*>(p1L5.get())->health =
            navIn->asBool(asbHeaL5);
         // cerr << "add CNAV alm health" << endl;
         navOut.push_back(p1L1);
         // cerr << "add CNAV alm health" << endl;
         navOut.push_back(p1L2);
         // cerr << "add CNAV alm health" << endl;
         navOut.push_back(p1L5);
      }
      if (!PNBNavDataFactory::processAlm)
      {
            // User doesn't want almanacs so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNavAlm>();
      GPSCNavAlm *alm = dynamic_cast<GPSCNavAlm*>(p0.get());
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
      alm->wna = navIn->asUnsignedLong(asbWNa,anbWNa,ascWNa);
      alm->toa = navIn->asUnsignedDouble(asbtoa,anbtoa,asctoa);
      alm->Toc = alm->Toe = GPSWeekSecond(alm->wna,alm->toa);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)
      {
         alm->Toe.setTimeSystem(gnsstk::TimeSystem::QZS);
         alm->Toc.setTimeSystem(gnsstk::TimeSystem::QZS);
      }
      alm->M0 = navIn->asDoubleSemiCircles(asbM0,anbM0,ascM0);
      alm->ecc = navIn->asUnsignedDouble(asbEcc,anbEcc,ascEcc);
      alm->Ahalf = navIn->asUnsignedDouble(asbAhalf,anbAhalf,ascAhalf);
      alm->A = alm->Ahalf * alm->Ahalf;
      alm->OMEGA0 = navIn->asDoubleSemiCircles(asbOMEGA0,anbOMEGA0,ascOMEGA0);
         // i0 is set below
      alm->w = navIn->asDoubleSemiCircles(asbw,anbw,ascw);
      alm->OMEGAdot = navIn->asDoubleSemiCircles(asbOMEGAdot,anbOMEGAdot,
                                                 ascOMEGAdot);
      alm->af0 = navIn->asSignedDouble(asbaf0,anbaf0,ascaf0);
      alm->af1 = navIn->asSignedDouble(asbaf1,anbaf1,ascaf1);
         // GPSCNavData
      alm->pre = navIn->asUnsignedLong(esbPre,enbPre,escPre);
      alm->alert = navIn->asBool(esbAlert);
         // GPSCNavAlm
      alm->healthL1 = navIn->asBool(asbHeaL1);
      alm->healthL2 = navIn->asBool(asbHeaL2);
      alm->healthL5 = navIn->asBool(asbHeaL5);
      switch (navIn->getobsID().band)
      {
         case CarrierBand::L2:
            alm->health = ((alm->healthL2 == false) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            break;
         case CarrierBand::L5:
            alm->health = ((alm->healthL5 == false) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            break;
         default:
               // unexpected/unsupported signal
            return false;
      }
      alm->deltai = navIn->asDoubleSemiCircles(asbdi,anbdi,ascdi);
      if (alm->signal.sat.system == SatelliteSystem::QZSS)
      {
         alm->i0 = GPSCNavData::refi0QZSS + alm->deltai;
      }
      else
      {
         alm->i0 = GPSCNavData::refi0GPS + alm->deltai;
      }
      alm->fixFit();
      // cerr << "add CNAV alm" << endl;
      navOut.push_back(p0);
      return true;
   }


   bool PNBGPSCNavDataFactory ::
   process12(unsigned msgType, const PackedNavBitsPtr& navIn,
             NavDataPtrList& navOut)
   {
      unsigned pre = navIn->asUnsignedLong(esbPre,enbPre,escPre);
      bool alert = navIn->asBool(esbAlert);
      unsigned wna = navIn->asUnsignedLong(rsb12WNa,rnb12WNa,rsc12WNa);
      unsigned long toa = navIn->asUnsignedLong(rsb12toa,rnb12toa,rsc12toa);
      return
         processRedAlmOrb(msgType,rsb12p1,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb12p2,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb12p3,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb12p4,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb12p5,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb12p6,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb12p7,pre,alert,wna,toa,navIn,navOut);
   }


   bool PNBGPSCNavDataFactory ::
   process30(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      if (PNBNavDataFactory::processIono)
      {
         NavDataPtr p0 = std::make_shared<GPSCNavIono>();
         GPSCNavIono *iono = dynamic_cast<GPSCNavIono*>(p0.get());
            // NavData
         p0->timeStamp = navIn->getTransmitTime();
         p0->signal = NavMessageID(
            NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                           navIn->getobsID(), navIn->getNavID()),
            NavMessageType::Iono);
            // KlobucharIonoNavData
         iono->alpha[0] = navIn->asSignedDouble(isbAlpha0,inbAlpha0,iscAlpha0);
         iono->alpha[1] = navIn->asSignedDouble(isbAlpha1,inbAlpha1,iscAlpha1);
         iono->alpha[2] = navIn->asSignedDouble(isbAlpha2,inbAlpha2,iscAlpha2);
         iono->alpha[3] = navIn->asSignedDouble(isbAlpha3,inbAlpha3,iscAlpha3);
         iono->beta[0] = navIn->asSignedDouble(isbBeta0,inbBeta0,iscBeta0);
         iono->beta[1] = navIn->asSignedDouble(isbBeta1,inbBeta1,iscBeta1);
         iono->beta[2] = navIn->asSignedDouble(isbBeta2,inbBeta2,iscBeta2);
         iono->beta[3] = navIn->asSignedDouble(isbBeta3,inbBeta3,iscBeta3);
            // GPSCNavIono
         iono->pre = navIn->asUnsignedLong(esbPre,enbPre,escPre);
         iono->alert = navIn->asBool(esbAlert);
         navOut.push_back(p0);
      }
      if (PNBNavDataFactory::processISC)
      {
         NavDataPtr p1 = std::make_shared<GPSCNavISC>();
         GPSCNavISC *isc = dynamic_cast<GPSCNavISC*>(p1.get());
            // NavData
         p1->timeStamp = navIn->getTransmitTime();
         p1->signal = NavMessageID(
            NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                           navIn->getobsID(), navIn->getNavID()),
            NavMessageType::ISC);
            // InterSigCorr
         isc->isc = navIn->asSignedDouble(isbTgd,inbTgd,iscTgd);
            // GPSCNavISC
         isc->pre = navIn->asUnsignedLong(esbPre,enbPre,escPre);
         isc->alert = navIn->asBool(esbAlert);
         isc->iscL1CA = navIn->asSignedDouble(isbISCL1CA,inbISCL1CA,iscISCL1CA);
         isc->iscL2C = navIn->asSignedDouble(isbISCL2C,inbISCL2C,iscISCL2C);
         isc->iscL5I5 = navIn->asSignedDouble(isbISCL5I5,inbISCL5I5,iscISCL5I5);
         isc->iscL5Q5 = navIn->asSignedDouble(isbISCL5Q5,inbISCL5Q5,iscISCL5Q5);
         navOut.push_back(p1);
      }
      return true;
   }


   bool PNBGPSCNavDataFactory ::
   process31(unsigned msgType, const PackedNavBitsPtr& navIn,
             NavDataPtrList& navOut)
   {
      unsigned pre = navIn->asUnsignedLong(esbPre,enbPre,escPre);
      bool alert = navIn->asBool(esbAlert);
      unsigned wna = navIn->asUnsignedLong(rsb31WNa,rnb31WNa,rsc31WNa);
      unsigned long toa = navIn->asUnsignedLong(rsb31toa,rnb31toa,rsc31toa);
      return
         processRedAlmOrb(msgType,rsb31p1,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb31p2,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb31p3,pre,alert,wna,toa,navIn,navOut) &&
         processRedAlmOrb(msgType,rsb31p4,pre,alert,wna,toa,navIn,navOut);
   }


   bool PNBGPSCNavDataFactory ::
   processRedAlmOrb(unsigned msgType, unsigned offset, unsigned pre, bool alert,
                    unsigned wna, double toa,
                    const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      SatID xmitSat(navIn->getsatSys());
      unsigned long sprn = navIn->asUnsignedLong(offset+rsbPRNa,rnbPRNa,
                                                 rscPRNa);
      SatelliteSystem subjSys = xmitSat.system;
      if (sprn == 0)
      {
            // empty almanac
         return true;
      }
         // special handling for QZSS per IS-QZSS 1.8E Table 5.5.2-9
      if (subjSys == SatelliteSystem::QZSS)
      {
         if ((msgType == 31) || (msgType == 12))
         {
               // When the message type number is 31 or 12, it
               // indicates that this value is for a QZS satellite and
               // represents the last 6 bits of the QZS PRN number.
               // .. so we do a bitwise OR to get the QZS PRN.
            sprn |= 0xc0;
         }
         else if ((msgType == 47) || (msgType == 28))
         {
               // When the message type number is 47 or 28, it
               // indicates that this value is the PRN value for a GPS
               // satellite (PRN No.= 1 ~ 32)
            subjSys = SatelliteSystem::GPS;
         }
      }
      SatID subjSat(sprn, subjSys);
      if (PNBNavDataFactory::processHea)
      {
            // Add reduced almanac health bits
         NavDataPtr p1L1 = std::make_shared<GPSCNavHealth>();
         NavDataPtr p1L2 = std::make_shared<GPSCNavHealth>();
         NavDataPtr p1L5 = std::make_shared<GPSCNavHealth>();
         p1L1->timeStamp = navIn->getTransmitTime();
         p1L2->timeStamp = navIn->getTransmitTime();
         p1L5->timeStamp = navIn->getTransmitTime();
            /** @todo I'm not entirely sure what's appropriate here.
             * The source signal is actually L2C CNAV but it's
             * broadcasting signal status for L1 signals that don't
             * have CNAV. */
         p1L1->signal = NavMessageID(
            NavSatelliteID(subjSat, xmitSat,
                           ObsID(ObservationType::NavMsg, CarrierBand::L1,
                                 TrackingCode::CA),
                           NavID(NavType::GPSLNAV)),
            NavMessageType::Health);
         if (navIn->getobsID().band == CarrierBand::L2)
         {
            p1L2->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat, navIn->getobsID(),
                              navIn->getNavID()),
               NavMessageType::Health);
            p1L5->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat,
                              ObsID(ObservationType::NavMsg, CarrierBand::L5,
                                    TrackingCode::L5I),
                              NavID(NavType::GPSCNAVL5)),
               NavMessageType::Health);
         }
         else
         {
               // assume L5
            p1L2->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat,
                              ObsID(ObservationType::NavMsg, CarrierBand::L2,
                                    TrackingCode::L2CM),
                              NavID(NavType::GPSCNAVL2)),
               NavMessageType::Health);
            p1L5->signal = NavMessageID(
               NavSatelliteID(subjSat, xmitSat, navIn->getobsID(),
                              navIn->getNavID()),
               NavMessageType::Health);
         }
         dynamic_cast<GPSCNavHealth*>(p1L1.get())->health =
            navIn->asBool(offset+rsbHeaL1);
         dynamic_cast<GPSCNavHealth*>(p1L2.get())->health =
            navIn->asBool(offset+rsbHeaL2);
         dynamic_cast<GPSCNavHealth*>(p1L5.get())->health =
            navIn->asBool(offset+rsbHeaL5);
         // cerr << "add CNAV reduced alm health" << endl;
         navOut.push_back(p1L1);
         // cerr << "add CNAV reduced alm health" << endl;
         navOut.push_back(p1L2);
         // cerr << "add CNAV reduced alm health" << endl;
         navOut.push_back(p1L5);
      }
      if (!PNBNavDataFactory::processAlm)
      {
            // User doesn't want almanac data so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNavRedAlm>();
      GPSCNavRedAlm *alm = dynamic_cast<GPSCNavRedAlm*>(p0.get());
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
      alm->Toc = alm->Toe = GPSWeekSecond(wna,toa);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)
      {
         alm->Toe.setTimeSystem(gnsstk::TimeSystem::QZS);
         alm->Toc.setTimeSystem(gnsstk::TimeSystem::QZS);
      }
      alm->OMEGA0 = navIn->asDoubleSemiCircles(offset+rsbOMEGA0,rnbOMEGA0,
                                               rscOMEGA0);
         // GPSCNavData
      alm->pre = pre;
      alm->alert = alert;
         // GPSCNavAlm
      alm->healthL1 = navIn->asBool(offset+rsbHeaL1);
      alm->healthL2 = navIn->asBool(offset+rsbHeaL2);
      alm->healthL5 = navIn->asBool(offset+rsbHeaL5);
      alm->wna = wna;
      alm->toa = toa;
      switch (navIn->getobsID().band)
      {
         case CarrierBand::L2:
            alm->health = ((alm->healthL2 == false) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            break;
         case CarrierBand::L5:
            alm->health = ((alm->healthL5 == false) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            break;
         default:
               // unexpected/unsupported signal
            return false;
      }
         // GPSCNavRedAlm
      alm->phi0 = navIn->asDoubleSemiCircles(offset+rsbPHI0,rnbPHI0,rscPHI0);
      alm->deltaA = navIn->asSignedDouble(offset+rsbdA,rnbdA,rscdA);
      alm->fixValues();
      alm->fixFit();
      // cerr << "add CNAV alm" << endl;
      navOut.push_back(p0);
      return true;
   }


   bool PNBGPSCNavDataFactory ::
   process33(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      if (!PNBNavDataFactory::processTim)
      {
            // User doesn't want time offset data so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNavTimeOffset>();
      p0->timeStamp = navIn->getTransmitTime();
      p0->signal = NavMessageID(
         NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                               navIn->getobsID(), navIn->getNavID()),
         NavMessageType::TimeOffset);
      GPSCNavTimeOffset *to =
         dynamic_cast<GPSCNavTimeOffset*>(p0.get());
      to->tgt = TimeSystem::UTC; // by definition
      to->a0 = navIn->asSignedDouble(csbA0,cnbA0,cscA0);
      to->a1 = navIn->asSignedDouble(csbA1,cnbA1,cscA1);
      to->a2 = navIn->asSignedDouble(csbA2,cnbA2,cscA2);
      to->deltatLS = navIn->asLong(csbdtLS,cnbdtLS,cscdtLS);
      to->tot = navIn->asUnsignedDouble(csbtot,cnbtot,csctot);
      to->wnot = navIn->asUnsignedLong(csbWNot,cnbWNot,cscWNot);
      to->wnLSF = navIn->asUnsignedLong(csbWNlsf,cnbWNlsf,cscWNlsf);
      to->dn = navIn->asUnsignedLong(csbDN,cnbDN,cscDN);
      to->refTime = GPSWeekSecond(to->wnot, to->tot);
      to->effTime = GPSWeekSecond(to->wnLSF, (to->dn-1)*86400);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)
      {
         to->src = gnsstk::TimeSystem::QZS;
         to->refTime.setTimeSystem(gnsstk::TimeSystem::QZS);
         to->effTime.setTimeSystem(gnsstk::TimeSystem::QZS);
      }
      to->deltatLSF = navIn->asLong(csbdtLSF,cnbdtLSF,cscdtLSF);
      // cerr << "add CNAV time offset" << endl;
      navOut.push_back(p0);
      return true;
   }


   bool PNBGPSCNavDataFactory ::
   process35(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      if (!PNBNavDataFactory::processTim)
      {
            // User doesn't want time offset data so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GPSCNavTimeOffset>();
      p0->timeStamp = navIn->getTransmitTime();
      p0->signal = NavMessageID(
         NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                               navIn->getobsID(), navIn->getNavID()),
         NavMessageType::TimeOffset);
      GPSCNavTimeOffset *to =
         dynamic_cast<GPSCNavTimeOffset*>(p0.get());
      uint8_t gnssID = navIn->asUnsignedLong(gsbGNSS,gnbGNSS,gscGNSS);
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
            // cerr << "Unknown GNSS ID " << gnssID << endl;
               // unknown/unsupported
            return false;
      }
      to->tot = navIn->asUnsignedDouble(gsbt,gnbt,gsct);
      to->wnot = navIn->asUnsignedLong(gsbWN,gnbWN,gscWN);
      to->refTime = gnsstk::GPSWeekSecond(to->wnot, to->tot);
      if (navIn->getsatSys().system == gnsstk::SatelliteSystem::QZSS)
      {
         to->src = gnsstk::TimeSystem::QZS;
         to->refTime.setTimeSystem(gnsstk::TimeSystem::QZS);
      }
      to->a0 = navIn->asSignedDouble(gsbA0,gnbA0,gscA0);
      to->a1 = navIn->asSignedDouble(gsbA1,gnbA1,gscA1);
      to->a2 = navIn->asSignedDouble(gsbA2,gnbA2,gscA2);
      // cerr << "add CNAV time offset" << endl;
      navOut.push_back(p0);
      return true;
   }


   void PNBGPSCNavDataFactory ::
   dumpState(std::ostream& s)
      const
   {
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

} // namespace gnsstk
