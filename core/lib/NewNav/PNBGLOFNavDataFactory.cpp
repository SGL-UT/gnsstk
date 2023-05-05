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
#include "PNBGLOFNavDataFactory.hpp"
#include "GLOFBits.hpp"
#include "GLOFNavAlm.hpp"
#include "GLOFNavEph.hpp"
#include "GLOFNavHealth.hpp"
#include "GLOFNavISC.hpp"
#include "GLOFNavTimeOffset.hpp"
#include "GLOFNavUT1TimeOffset.hpp"
#include "CivilTime.hpp"
#include "YDSTime.hpp"
#include "GLONASSTime.hpp"
#include "TimeString.hpp"
#include "DebugTrace.hpp"

using namespace std;
using namespace gnsstk::glo;

namespace gnsstk
{
   PNBGLOFNavDataFactory ::
   PNBGLOFNavDataFactory()
         : almDOY(CommonTime::BEGINNING_OF_TIME),
           pendingAlms(false)
   {
   }


   bool PNBGLOFNavDataFactory ::
   addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
           double cadence)
   {
      DEBUGTRACE_FUNCTION();
      if ((navIn->getNavID().navType != NavType::GloCivilF) ||
          (!validSize(navIn)))
      {
         DEBUGTRACE("fail bits=" << navIn->getNumBits() << "  nav="
                    << StringUtils::asString(navIn->getNavID().navType));
            // This class only processes GLONASS FDMA NAV.
         return false;
      }
      bool rv = true;
      try
      {
         unsigned long stringID = navIn->asUnsignedLong(
            fsbStrNum,fnbStrNum,fscStrNum);
         // cerr << __PRETTY_FUNCTION__ << "  num bits: " << navIn->getNumBits() << "  string ID: " << stringID << endl;
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
               /// @todo Implement a parity check, if there is any.
         }
         switch (stringID)
         {
            case 1:
            case 2:
            case 3:
            case 4:
               rv = processEph(stringID, navIn, navOut);
               DEBUGTRACE("Eph rv=" << rv);
               break;
            case 5:
               rv = processTime(navIn, navOut);
               DEBUGTRACE("Time rv=" << rv);
               break;
            default:
                  /// Everything else is almanac
               rv = processAlm(stringID, navIn, navOut);
               DEBUGTRACE("Alm rv=" << rv);
               break;
         }
      }
      catch (Exception& exc)
      {
         DEBUGTRACE("fail exception");
         rv = false;
         cerr << exc << endl;
      }
      catch (std::exception& exc)
      {
         DEBUGTRACE("fail exception");
         rv = false;
         cerr << exc.what() << endl;
      }
      catch (...)
      {
         DEBUGTRACE("fail exception");
         rv = false;
         cerr << "Unknown exception" << endl;
      }
      return rv;
   }


   bool PNBGLOFNavDataFactory ::
   processEph(unsigned long stringID, const PackedNavBitsPtr& navIn,
              NavDataPtrList& navOut)
   {
      NavSatelliteID key(navIn->getsatSys().id, navIn->getsatSys(),
                         navIn->getobsID(), navIn->getNavID());
      if ((stringID < 1) || (stringID > 4))
         return false; // not actually part of the ephemeris.
      if (stringID == 4)
      {
         if (PNBNavDataFactory::processISC)
         {
            NavDataPtr p2 = std::make_shared<GLOFNavISC>();
            GLOFNavISC *isc = dynamic_cast<GLOFNavISC*>(p2.get());
            isc->timeStamp = navIn->getTransmitTime();
            isc->signal = NavMessageID(key, NavMessageType::ISC);
            DEBUGTRACE("ISC signal = " << isc->signal);
            isc->isc = navIn->asSignMagDouble(esbdtaun,enbdtaun,escdtaun);
            navOut.push_back(p2);
         }
         if (PNBNavDataFactory::processTim)
         {
            timeAcc[key].setNT(navIn->asUnsignedLong(esbNT,enbNT,escNT));
         }
      }
      if (!PNBNavDataFactory::processEph && !PNBNavDataFactory::processHea)
      {
            // User doesn't want ephemerides so don't do any processing.
         return true;
      }
      if (ephAcc.find(key) == ephAcc.end())
      {
            // set up a new ephemeris
         ephAcc[key].resize(4); // we only store the 1st 4 strings of 15.
         ephAcc[key][stringID-1] = navIn;
            // nothing in navOut yet and no further processing because
            // we only have one of four strings at this point.
         return true;
      }
      std::vector<PackedNavBitsPtr> &ephS(ephAcc[key]);
      ephS[stringID-1] = navIn;
         // stop processing if we don't have a complete, consecutive
         // set of strings
      if (!ephS[str1] || !ephS[str2] || !ephS[str3] || !ephS[str4] ||
          !validSize(ephS[str1]) ||
          !validSize(ephS[str2]) ||
          !validSize(ephS[str3]) ||
          !validSize(ephS[str4]) ||
          (ephS[str2]->getTransmitTime()-ephS[str1]->getTransmitTime() != 2) ||
          (ephS[str3]->getTransmitTime()-ephS[str2]->getTransmitTime() != 2) ||
          (ephS[str4]->getTransmitTime()-ephS[str3]->getTransmitTime() != 2))
      {
         return true;
      }
         // Health data here only requires string 2-3.
         /// @todo Maybe make it so we can still get the health w/o strings 1&4
      if (PNBNavDataFactory::processHea)
      {
         NavDataPtr p1 = std::make_shared<GLOFNavHealth>();
         p1->timeStamp = navIn->getTransmitTime();
         p1->signal = NavMessageID(key, NavMessageType::Health);
         DEBUGTRACE("Health signal = " << p1->signal);
         GLOFNavHealth *hea = dynamic_cast<GLOFNavHealth*>(p1.get());
         hea->healthBits = ephS[esiBn]->asUnsignedLong(esbBn,enbBn,escBn);
         hea->ln = ephS[esiln]->asBool(esbln);
         navOut.push_back(p1);
      }
      if (!PNBNavDataFactory::processEph)
      {
         return true;
      }
      NavDataPtr p0 = std::make_shared<GLOFNavEph>();
      GLOFNavEph *eph = dynamic_cast<GLOFNavEph*>(p0.get());
      eph->timeStamp = ephS[str1]->getTransmitTime();
      eph->signal = NavMessageID(key, NavMessageType::Ephemeris);
      DEBUGTRACE("Eph signal = " << eph->signal);
      unsigned long tk = ephS[esitk]->asUnsignedLong(esbtk,enbtk,esctk);
         // 30 second offset since the beginning of the day, the
         // document says, but at one bit it's obviously relative to
         // the specified minute.
      unsigned long tk30s = tk & 0x01;
      tk >>= 1;
         // next 6 bits contain the minutes of the hour
      unsigned long tk1m = tk & 0x3f;
      tk >>= 6;
         // 5 MSBs contain the hour of day
      unsigned long tk1h = tk & 0x1f;
         /** @todo I think reference time is referenced to the start
          * of the transmit day, but is that true? */
      CivilTime tmp(eph->timeStamp);
      eph->ref = CivilTime(tmp.year, tmp.month, tmp.day,
                           tk1h, tk1m, 30.0 * tk30s, TimeSystem::GLO);
      eph->xmit2 = ephS[str2]->getTransmitTime();
      eph->xmit3 = ephS[str3]->getTransmitTime();
      eph->xmit4 = ephS[str4]->getTransmitTime();
      eph->pos[0]=ephS[esixn]->asSignMagDouble(esbxn,enbxn,escxn);
      eph->pos[1]=ephS[esiyn]->asSignMagDouble(esbyn,enbyn,escyn);
      eph->pos[2]=ephS[esizn]->asSignMagDouble(esbzn,enbzn,esczn);
      eph->vel[0]=ephS[esixnp]->asSignMagDouble(esbxnp,enbxnp,escxnp);
      eph->vel[1]=ephS[esiynp]->asSignMagDouble(esbynp,enbynp,escynp);
      eph->vel[2]=ephS[esiznp]->asSignMagDouble(esbznp,enbznp,escznp);
      eph->acc[0]=ephS[esixnpp]->asSignMagDouble(esbxnpp,enbxnpp,escxnpp);
      eph->acc[1]=ephS[esiynpp]->asSignMagDouble(esbynpp,enbynpp,escynpp);
      eph->acc[2]=ephS[esiznpp]->asSignMagDouble(esbznpp,enbznpp,escznpp);
      eph->clkBias = ephS[esitaun]->asSignMagDouble(esbtaun,enbtaun,esctaun);
      eph->freqBias = ephS[esiGamman]->asSignMagDouble(
         esbGamman,enbGamman,escGamman);
      eph->healthBits = ephS[esiBn]->asUnsignedLong(esbBn,enbBn,escBn);
      eph->lhealth = ephS[esiln]->asBool(esbln);
      eph->health = ((eph->lhealth != 0) || (eph->healthBits & 0x04)
                     ? SVHealth::Unhealthy
                     : SVHealth::Healthy);
      eph->tb = ephS[esitb]->asUnsignedLong(esbtb,enbtb,esctb);
      eph->P1 = ephS[esiP1]->asUnsignedLong(esbP1,enbP1,escP1);
      eph->P2 = ephS[esiP2]->asUnsignedLong(esbP2,enbP2,escP2);
      eph->P3 = ephS[esiP3]->asUnsignedLong(esbP3,enbP3,escP3);
      eph->P4 = ephS[esiP4]->asUnsignedLong(esbP4,enbP4,escP4);
         // factor to multiply tb by to get seconds of day.
      unsigned tbFactor = 0;
      switch (eph->P1)
      {
         case 3:
            eph->interval = 60;
            break;
         case 2:
            eph->interval = 45;
            break;
         case 1:
            eph->interval = 30;
            break;
         case 0:
         default:
            eph->interval = 0;
            break;
      }
      eph->opStatus = static_cast<GLOFNavPCode>(
         ephS[esiP]->asUnsignedLong(esbP,enbP,escP));
      eph->tauDelta = ephS[esidtaun]->asSignMagDouble(
         esbdtaun,enbdtaun,escdtaun);
      eph->aod = ephS[esiEn]->asUnsignedLong(esbEn,enbEn,escEn);
      eph->accIndex = ephS[esiFT]->asUnsignedLong(esbFT,enbFT,escFT);
      eph->dayCount = ephS[esiNT]->asUnsignedLong(esbNT,enbNT,escNT);
      eph->slot = ephS[esin]->asUnsignedLong(esbn,enbn,escn);
      eph->satType = static_cast<GLOFNavSatType>(
         ephS[esiM]->asUnsignedLong(esbM,enbM,escM));
      YDSTime toe(eph->timeStamp);
         // This is a kludge to have what was deemed to be a
         // reasonable validity time span in older code by using a 30
         // minute interval when P1==0.  I'm not convinced it's
         // suitable, but it's here for now until someone comes up
         // with a more definitive way to handle the fit interval.
         // Without this kludge, the orbit epoch time stamp can go
         // wonky early in the day (see bug below) or the end valid
         // time stamp can be only 30 seconds after the orbit epoch.
      unsigned interval = (eph->interval > 0 ? eph->interval : 30);
      toe.sod = eph->tb * 2 * interval;
         // Move from Moscow Time to UTC(SU) aka TimeSystem::GLO
         /** @bug This doesn't seem right, adding a day to seconds of
          * day then subtracting?  Shouldn't it just be subtracting 3
          * hours (10800 seconds from the CommonTime object once
          * constructed? */
      if (toe.sod >= 10800)
         toe.sod -= 10800;
      else
      {
         toe.sod += 86400;
         toe.sod -= 10800;
      }
      toe.setTimeSystem(TimeSystem::GLO);
      eph->Toe = toe;
      eph->fixFit();
      navOut.push_back(p0);
         // Clear out the broadcast ephemeris that's been processed.
      ephAcc.erase(key);
      // cerr << "navOut.size()=" << navOut.size() << endl
      //      << key.sat << "  Toe="
      //      << printTime(eph->Toe, "%Y/%02m/%02d %02H:%02M:%02S %P") << endl
      //      << key.sat << "  tb=" << eph->tb << "  tb*interval="
      //      << eph->tb*interval << endl
      //      << key.sat << "  toe.sod=" << toe.sod << endl
      //      << key.sat << "  interval=" << eph->interval << " (" << interval
      //      << ")" << endl
      //      << key.sat << "  P1=" << eph->P1 << endl;
      return true;
   }


   bool PNBGLOFNavDataFactory ::
   processAlm(unsigned long stringID, const PackedNavBitsPtr& navIn,
              NavDataPtrList& navOut)
   {
      SatID xmitSat(navIn->getsatSys());
         // Key used for data accumulating, which we don't separate by
         // subject satellite.
      NavSatelliteID key(0, xmitSat, navIn->getobsID(), navIn->getNavID());
         // magic numbers 6 and 15 in this block of code refer to the string ID
      if ((stringID < 6) || (stringID > 15))
      {
         return false; // not actually part of the almanac.
      }
      if ((stringID > 13) && (getFrameNum(navIn->getTransmitTime()) == 5))
      {
            // Not really part of the almanac, but UTC time conversion
            // information is in string 14.
         if (stringID == 14)
         {
            return processTimeUT1(navIn, navOut);
         }
            /// @todo Should we process the transmit satellite health here?
         return true; // string 15
      }
      if (!PNBNavDataFactory::processAlm && !PNBNavDataFactory::processHea)
      {
            // User doesn't want almanacs or health so don't do any processing.
         return true;
      }
      if (almAcc.find(key) == almAcc.end())
      {
            // set up a new almanac
         almAcc[key].resize(10); // Storing strings 6-15
         almAcc[key][stringID-6] = navIn;
            // nothing in navOut yet and no further processing because
            // we only have one of four strings at this point.
         return true;
      }
         // almIdx becomes the array index of the even string number of the pair
      unsigned almIdx = (stringID & ~1) - 6;
      std::vector<PackedNavBitsPtr> &almS(almAcc[key]);
      almS[stringID-6] = navIn;
      return processAlmOrb(key, almIdx, navOut);
   }


   bool PNBGLOFNavDataFactory ::
   processAlmOrb(const NavSatelliteID& key, unsigned almIdx,
                 NavDataPtrList& navOut)
   {
      std::vector<PackedNavBitsPtr> &almS(almAcc[key]);
      // cerr << "stringID=" << stringID << "  almIdx=" << almIdx << endl;
      // if ((stringID & 0x01) == 0)
      // {
      //    cerr << "n=" << almS[almIdx]->asUnsignedLong(asbn,anbn,ascn) << endl;
      // }
      if (!almS[almIdx] || !almS[almIdx+1] ||
          (almS[almIdx]->getNumBits() != 96) ||
          (almS[almIdx+1]->getNumBits() != 96) ||
          (almS[almIdx+1]->getTransmitTime() - almS[almIdx]->getTransmitTime()
           != 2))
      {
         // cerr << "incomplete almanac  ";
         // if (!almS[almIdx])
         // {
         //    cerr << " no-even";
         // }
         // else if (!almS[almIdx+1])
         // {
         //    cerr << " no-odd";
         // }
         // else
         // {
         //    cerr << (almS[almIdx]->getNumBits() != 96 ? " even-wrong-size" : "")
         //         << (almS[almIdx+1]->getNumBits() != 96 ? "  odd-wrong-size" : "")
         //         << ((almS[almIdx+1]->getTransmitTime() - almS[almIdx]->getTransmitTime() != 2) ? " bad time" : "");
         // }
         // cerr << endl;
         return true;
      }
      if (almDOY == CommonTime::BEGINNING_OF_TIME)
      {
            // We have a complete almanac, but no reference time, so
            // let's flag it for when a string 5 shows up.
         pendingAlms = true;
         return true;
      }
      // cerr << "complete almanac" << endl;
      SatID xmitSat(almS[almIdx]->getsatSys());
      unsigned long slot = almS[almIdx+ason]->asUnsignedLong(asbn,anbn,ascn);
      NavSatelliteID sat(slot, xmitSat, almS[almIdx]->getobsID(),
                         almS[almIdx]->getNavID());
      if (PNBNavDataFactory::processHea)
      {
         NavDataPtr p1 = std::make_shared<GLOFNavHealth>();
         p1->timeStamp = almS[almIdx]->getTransmitTime();
         p1->signal = NavMessageID(sat, NavMessageType::Health);
         DEBUGTRACE("Health signal = " << p1->signal);
         GLOFNavHealth *hea = dynamic_cast<GLOFNavHealth*>(p1.get());
         hea->Cn = almS[almIdx+asoC]->asBool(asbC);
         hea->ln = almS[almIdx+asol]->asBool(asbl);
         navOut.push_back(p1);
      }
      if (!PNBNavDataFactory::processAlm)
      {
            // nothing more to do here
         return true;
      }
      NavDataPtr p0 = std::make_shared<GLOFNavAlm>();
      GLOFNavAlm *alm = dynamic_cast<GLOFNavAlm*>(p0.get());
      alm->timeStamp = almS[almIdx]->getTransmitTime();
      alm->xmit2 = almS[almIdx+1]->getTransmitTime();
         // Actual satellite ID (subject and transmit)
      alm->slot = slot;
      alm->signal = NavMessageID(sat, NavMessageType::Almanac);
      DEBUGTRACE("Alm signal = " << alm->signal);
      alm->healthBits = almS[almIdx+asoC]->asBool(asbC);
      alm->health = (alm->healthBits == 1
                     ? SVHealth::Healthy
                     : SVHealth::Unhealthy);
      alm->satType = static_cast<GLOFNavSatType>(
         almS[almIdx+asoM]->asUnsignedLong(asbM,anbM,ascM));
      alm->taunA = almS[almIdx+asotau]->asSignMagDouble(asbtau,anbtau,asctau);
      alm->lambdanA = almS[almIdx+asolambda]->asSignMagDoubleSemiCircles(
         asblambda,anblambda,asclambda);
      alm->deltainA = almS[almIdx+asodeltai]->asSignMagDoubleSemiCircles(
         asbdeltai,anbdeltai,ascdeltai);
      alm->eccnA = almS[almIdx+asoepsilon]->asSignMagDouble(
         asbepsilon,anbepsilon,ascepsilon);
      alm->omeganA = almS[almIdx+asoomega]->asSignMagDoubleSemiCircles(
         asbomega,anbomega,ascomega);
      alm->tLambdanA = almS[almIdx+asot]->asUnsignedDouble(asbt,anbt,asct);
         // Epoch time for the almanac is tied to the first ascending
         // crossing of the plane.  The NATIVE timescale for GLONASS
         // time is UTC(SU) + 3h.  The Na parameter from String 5 should
         // be reference to UTC(SU) + 3h.   Therefore, we can add the
         // SOD epoch time to the almanac day (all in UTC(SU)+3h)
         // then move that time to UTC(SU).
      YDSTime almYDS(almDOY);
      alm->Toa = YDSTime(almYDS.year,almYDS.doy,alm->tLambdanA,TimeSystem::GLO);
      alm->deltaTnA = almS[almIdx+asoDeltaT]->asSignMagDouble(
         asbDeltaT,anbDeltaT,ascDeltaT);
      alm->deltaTdotnA = almS[almIdx+asoDeltaTdot]->asSignMagDouble(
         asbDeltaTdot,anbDeltaTdot,ascDeltaTdot);
      alm->freqnA = almS[almIdx+asoH]->asLong(asbH,anbH,ascH);
      alm->lhealth = almS[almIdx+asol]->asBool(asbl);
      alm->fixFit();
      navOut.push_back(p0);
         // Release the shared pointers
      almAcc[key][almIdx].reset();
      almAcc[key][almIdx+1].reset();
      return true;
   }


   bool PNBGLOFNavDataFactory ::
   processTime(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      bool rv = true;
      NavSatelliteID key(navIn->getsatSys().id, navIn->getsatSys(),
                         navIn->getobsID(), navIn->getNavID());
      if (PNBNavDataFactory::processHea)
      {
         NavDataPtr p1 = std::make_shared<GLOFNavHealth>();
         p1->timeStamp = navIn->getTransmitTime();
         p1->signal = NavMessageID(key, NavMessageType::Health);
         DEBUGTRACE("Health signal = " << p1->signal);
         GLOFNavHealth *hea = dynamic_cast<GLOFNavHealth*>(p1.get());
         hea->ln = navIn->asBool(tsbln);
         navOut.push_back(p1);
      }
         // This code block gets the reference time used for both
         // almanac orbital elements as well as almanac health.
      if (PNBNavDataFactory::processAlm || PNBNavDataFactory::processHea)
      {
            // day within four-year interval
         unsigned NA = navIn->asUnsignedLong(tsbNA,tnbNA,tscNA);
         timeAcc[key].setNA(NA);
            // number of leap years since 1996
         unsigned N4 = navIn->asUnsignedLong(tsbN4,tnbN4,tscN4);
         almDOY = GLONASSTime(N4, NA, 0);
         if (pendingAlms)
         {
               // iterate through the almanac accumulator and finish any
               // possible almanacs.
            for (const auto& i : almAcc)
            {
               for (unsigned j = 0; j < i.second.size(); j += 2)
               {
                  bool result = processAlmOrb(i.first, j, navOut);
                  rv &= result;
               }
            }
         }
            // clear pending alms if we successfully processed everything
         pendingAlms = !rv;
      }
      if (PNBNavDataFactory::processTim)
      {
         NavDataPtr p0 = std::make_shared<GLOFNavTimeOffset>();
         p0->timeStamp = navIn->getTransmitTime();
         p0->signal = NavMessageID(key, NavMessageType::TimeOffset);
         DEBUGTRACE("Time signal = " << p0->signal);
         GLOFNavTimeOffset *to = dynamic_cast<GLOFNavTimeOffset*>(p0.get());
         to->a0 = navIn->asSignMagDouble(tsbtauGPS,tnbtauGPS,tsctauGPS);
            /** @bug Table 4.9 in the ICD suggests there are two sets
             * of scales and number of bits, one for legacy GLONASS
             * and one for GLONASS-M, but it doesn't say if tau_c
             * starts at the same bit position in legacy.  Another
             * implementation always uses the -M bit pattern.  It's
             * not clear what's correct. */
         timeAcc[key].tauc = navIn->asSignMagDouble(tsbtauc,tnbtaucM,tsctaucM);
            /** @bug This is a bit sketchy because it relies on the
             * hard-coded integer leap seconds in
             * gnsstk::getLeapSeconds(), but I can't think of a more
             * reliable way to get this data here at the moment.
             * Luckily changes are few and far between. */
         double offset = 0;
         bool result = btsc.getOffset(TimeSystem::GLO, TimeSystem::GPS,
                                      to->timeStamp, offset);
         rv &= result;
         if (result)
         {
            to->deltatLS = -offset;
               // These values don't really matter much, since A1 and A2 are 0.
               // We set them mostly for display purposes (dump).
            to->refTime = to->effTime = to->timeStamp;
            navOut.push_back(p0);
         }
      }
      return rv;
   }


   bool PNBGLOFNavDataFactory ::
   processTimeUT1(const PackedNavBitsPtr& navIn,
                  NavDataPtrList& navOut)
   {
      if (!PNBNavDataFactory::processTim)
      {
            // User doesn't want time offsets so don't do any processing.
         return true;
      }
         /// @todo determine if this data is only in GLONASS-M
      NavSatelliteID key(navIn->getsatSys().id, navIn->getsatSys(),
                         navIn->getobsID(), navIn->getNavID());
      timeAcc[key].B1 = navIn->asSignMagDouble(usbB1,unbB1,uscB1);
      timeAcc[key].B2 = navIn->asSignMagDouble(usbB2,unbB2,uscB2);
      timeAcc[key].KP = navIn->asUnsignedLong(usbKP,unbKP,uscKP);
      if (timeAcc[key].isValid())
      {
         NavDataPtr p0 = timeAcc[key];
         p0->timeStamp = navIn->getTransmitTime();
         p0->signal = NavMessageID(key, NavMessageType::TimeOffset);
         DEBUGTRACE("Time signal = " << p0->signal);
         navOut.push_back(p0);
      }
      return true;
   }


   unsigned PNBGLOFNavDataFactory ::
   getFrameNum(const CommonTime& xmitTime)
   {
      CommonTime rv(xmitTime);
      GNSSTK_ASSERT(rv.changeTimeSystem(TimeSystem::GLO, &btsc));
      double dSOD = static_cast<YDSTime>(rv).sod;
      unsigned long SOD = (unsigned long) dSOD;
         // 30 second per frame
         // ...and frames are in the range 1-5
      unsigned long frameCount = SOD / 30;
      unsigned long frameNumber = (frameCount % 5) + 1;
      // cerr << "frame=" << frameNumber << endl;
      return frameNumber;
   }


   void PNBGLOFNavDataFactory ::
   resetState()
   {
      timeAcc.clear();
      ephAcc.clear();
      almAcc.clear();
   }


   PNBGLOFNavDataFactory::TimeMeta ::
   operator NavDataPtr() const
   {
      NavDataPtr p0 = std::make_shared<GLOFNavUT1TimeOffset>();
      GLOFNavUT1TimeOffset *to = dynamic_cast<GLOFNavUT1TimeOffset*>(p0.get());
      to->tauc = tauc;
      to->B1 = B1;
      to->B2 = B2;
      to->KP = KP;
      to->NT = NT;
      to->NA = NA;
      return p0;
   }


   void PNBGLOFNavDataFactory::TimeMeta ::
   setNT(unsigned v)
   {
      NT = v;
      B1.set_valid(false);
      B2.set_valid(false);
   }


   void PNBGLOFNavDataFactory::TimeMeta ::
   setNA(unsigned v)
   {
      NA = v;
      B1.set_valid(false);
      B2.set_valid(false);
   }


   bool PNBGLOFNavDataFactory::TimeMeta ::
   isValid()
   {
      return
         tauc.is_valid() &&
         B1.is_valid() &&
         B2.is_valid() &&
         NT.is_valid() &&
         NA.is_valid();
   }

} // namespace gnsstk
