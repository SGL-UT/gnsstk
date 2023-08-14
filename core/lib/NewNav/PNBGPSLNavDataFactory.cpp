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

#include <memory>

#include "EngNav.hpp"
#include "GPSLBits.hpp"
#include "GPSLNavAlm.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavHealth.hpp"
#include "GPSLNavISC.hpp"
#include "GPSLNavIono.hpp"
#include "GPSLNavTimeOffset.hpp"
#include "GPSNavConfig.hpp"
#include "NavMessageID.hpp"
#include "NavMessageType.hpp"
#include "NavSatelliteID.hpp"
#include "PNBNavDataFactory.hpp"
#include "TimeCorrection.hpp"
#include "EngNav.hpp"
#include "GPSLBits.hpp"
#include "DebugTrace.hpp"
#include "TimeString.hpp"

using namespace std;
using namespace gnsstk::gpslnav;

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
         unsigned long sfid = navIn->asUnsignedLong(fsbSFID,fnbSFID,fscSFID);
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
                     // process health and/or SV config
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
      DEBUGTRACE_FUNCTION();
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
      DEBUGTRACE("IODC=" << hex << iodc << dec << " prn=" << eph->signal.sat.id
                 << printTime(eph->timeStamp, " timeStamp=%Y %j %s"));
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
      eph->codesL2 = static_cast<GPSLNavL2Codes>(
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
            navIn->asUnsignedLong(asbHea,anbHea,ascHea);
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
      alm->ecc = navIn->asUnsignedDouble(asbEcc,anbEcc,ascEcc);
      alm->toa = navIn->asUnsignedDouble(asbtoa,anbtoa,asctoa);
      GPSWeekSecond ws(alm->xmitTime);
      // cerr << "page " << prn << " WNa = ??  toa = " << alm->toa
      //      << "  WNx = " << (ws.week & 0x0ff) << "  tox = " << ws.sow << endl;
      alm->deltai = navIn->asDoubleSemiCircles(asbdeltai,anbdeltai,ascdeltai);
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
      alm->OMEGAdot = navIn->asDoubleSemiCircles(asbOMEGAdot,anbOMEGAdot,
                                                 ascOMEGAdot);
      alm->healthBits = navIn->asUnsignedLong(asbHea,anbHea,ascHea);
      alm->health = (alm->healthBits == 0 ? SVHealth::Healthy :
                     SVHealth::Unhealthy);
      alm->Ahalf = navIn->asUnsignedDouble(asbAhalf,anbAhalf,ascAhalf);
      alm->A = alm->Ahalf * alm->Ahalf;
      alm->OMEGA0 = navIn->asDoubleSemiCircles(asbOMEGA0,anbOMEGA0,ascOMEGA0);
      alm->w = navIn->asDoubleSemiCircles(asbw,anbw,ascw);
      alm->M0 = navIn->asDoubleSemiCircles(asbM0,anbM0,ascM0);
      const unsigned af0start[] = {asbaf0m,asbaf0l};
      const unsigned af0num[] = {anbaf0m,anbaf0l};
         // 2 is the size of the af0start/af0num arrays
      alm->af0 = navIn->asSignedDouble(af0start,af0num,2,ascaf0);
      alm->af1 = navIn->asSignedDouble(asbaf1,anbaf1,ascaf1);
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
         double toa = navIn->asUnsignedDouble(asbtoa51,anbtoa51,asctoa51);
         unsigned shortWNa = navIn->asUnsignedLong(asbWNa51,anbWNa51,ascWNa51);
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
         // No checks for correct svid, just assume that the input data has
         // already been checked (it will have been by addData).

         // svid 63 = sf 4 page 25. The format has SVs 25-32 health and SVs 1-32
         // config in it.

      SatID xmitSat(navIn->getsatSys());
      ObsID oid(navIn->getobsID());
      NavID navid(navIn->getNavID());

      if (processSys)
      {
         for (unsigned prn = 1; prn <= 32; ++prn)
         {
            // PRNs 1-4 are at the end of word 2 (zero-indexed), then every word
            // after that has the PRNs sequentially placed from the start of the
            // word. This means that word selection (and location within the
            // word) follow a predictable pattern.
            const unsigned word{((prn + 1) / 6) + 2}; // zero-indexed
            const unsigned bitInWord{((prn + 1) % 6) * 4}; // counting from MSB

            auto configPtr{std::make_shared<GPSNavConfig>()};
            configPtr->timeStamp = navIn->getTransmitTime();
            configPtr->signal = NavMessageID{
               NavSatelliteID{prn, xmitSat, oid, navid},
               NavMessageType::System};
            configPtr->antispoofOn = navIn->asBool(30 * word + bitInWord);
            configPtr->svConfig =
               navIn->asUnsignedLong(30 * word + bitInWord + 1, 3, 1);
            navOut.emplace_back(configPtr);
         }
      }

      if (processHea)
      {
         for (unsigned prn = 25; prn <= 32; ++prn)
         {
            // PRN 25 is at the end of word 7 (zero-indexed), then every word
            // after that has the PRNs sequentially placed from the start of the
            // word. This means that word selection (and location within the
            // word) follow a predictable pattern.
            const unsigned word{((prn + 2) / 4) + 1}; // zero-indexed
            const unsigned bitInWord{((prn + 2) % 4) * 6}; // counting from MSB

            auto healthPtr{std::make_shared<GPSLNavHealth>()};
            healthPtr->timeStamp = navIn->getTransmitTime();
            healthPtr->signal = NavMessageID{
               NavSatelliteID{prn, xmitSat, oid, navid},
               NavMessageType::Health};
            healthPtr->svHealth =
               navIn->asUnsignedLong(30 * word + bitInWord, 6, 1);
            navOut.emplace_back(healthPtr);
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
      const unsigned a0start[] = { asbA0m, asbA0l };
      const unsigned a0num[] = { anbA0m, anbA0l };
      to->deltatLS = navIn->asLong(asbDeltatLS,anbDeltatLS,ascDeltatLS);
         // 2 is the size of the start/num arrays, while -30 is the
         // scale factor i.e. x*2^-30
      to->a0 = navIn->asSignedDouble(a0start,a0num,2,ascA0);
      to->a1 = navIn->asSignedDouble(asbA1,anbA1,ascA1);
      to->tot = navIn->asUnsignedDouble(asbtot,anbtot,asctot);
      to->wnot = navIn->asUnsignedLong(asbWNt,anbWNt,ascWNt);
      to->wnLSF = navIn->asUnsignedLong(asbWNLSF,anbWNLSF,ascWNLSF);
      to->dn = navIn->asUnsignedLong(asbDN,anbDN,ascDN);
      to->deltatLSF = navIn->asLong(asbDeltatLSF,anbDeltatLSF,ascDeltatLSF);
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
