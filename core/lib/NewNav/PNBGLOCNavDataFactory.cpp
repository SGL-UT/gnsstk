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
#include "DebugTrace.hpp"
#include "PNBGLOCNavDataFactory.hpp"
#include "GLOCBits.hpp"
#include "GLOCNavAlm.hpp"
#include "GLOCNavEph.hpp"
#include "GLOCNavHealth.hpp"
#include "GLOCNavIono.hpp"
#if 0
#include "GLOCNavISC.hpp"
#include "GLOCNavTimeOffset.hpp"
#endif
#include "GLOCNavUT1TimeOffset.hpp"
#include "GLONASSTime.hpp"
#include "CivilTime.hpp"
#include "YDSTime.hpp"
#include "TimeString.hpp"
#include "DebugTrace.hpp"

using namespace std;
using namespace gnsstk::gloc;

namespace gnsstk
{
   PNBGLOCNavDataFactory ::
   PNBGLOCNavDataFactory()
         : almDOY(CommonTime::BEGINNING_OF_TIME),
           pendingAlms(false)
   {
   }


   bool PNBGLOCNavDataFactory ::
   addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
           double cadence)
   {
      DEBUGTRACE_FUNCTION();
      static bool printed = false;
      if ((navIn->getNavID().navType != NavType::GloCivilC) ||
          (navIn->getNumBits() != 300))
      {
         DEBUGTRACE("nope, failed.  " << navIn->getNavID().navType << "  "
                    << navIn->getNumBits());
            // This class processes only GLONASS L3OCD NAV.
         return false;
      }
      bool rv = true;
      try
      {
         unsigned long msgType = navIn->asUnsignedLong(
            fsbType, fnbType, fscType);
         DEBUGTRACE("msgType = " << msgType);
         bool checkMsg = false, expMsg = false;
         switch (navValidity)
         {
            case NavValidityType::ValidOnly:
               checkMsg = true;
               expMsg = true;
               break;
            case NavValidityType::InvalidOnly:
               checkMsg = true;
               expMsg = false;
               break;
         }
         if (checkMsg)
         {
               // first check the Preamble
            unsigned long preamble = navIn->asUnsignedLong(
               fsbPreamble, fnbPreamble, fscPreamble);
            bool valid = (preamble == valPreamble);
            DEBUGTRACE("Checking message validity = " << valid
                       << "  preamble = " << hex << preamble);
            if (valid != expMsg)
            {
               return true;
            }
               /// @todo implement CRC check
               // Now check the CRC which is a bit more
               // computationally involved.  It should be the last
               // check done here unless another even more
               // computation-heavy validation is added (unlikely).
         }
         DEBUGTRACE("doing switch");
         switch (msgType)
         {
            case 10:
            case 11:
            case 12:
               DEBUGTRACE("eph");
                  // immediate (ephemeris) data
               rv = processEph(msgType, navIn, navOut);
               break;
            case 20:
               DEBUGTRACE("alm");
               rv = processAlm(navIn, navOut);
               break;
            case 25:
               DEBUGTRACE("earth");
                  // Earth rotation, iono model, time offset
               processEarth(navIn, navOut);
               break;
            case 31:
            case 32:
               DEBUGTRACE("ltdmp");
               processLTDMP(msgType, navIn);
               break;
            default:
               DEBUGTRACE("ignore");
                  // Ignore everything else.
               rv = true;
               break;
         }
         if (rv && processHea)
         {
            rv = processHealth(navIn, navOut);
         }
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


   bool PNBGLOCNavDataFactory ::
   processHeader(const PackedNavBitsPtr& navIn, GLOCNavHeader& navOut)
   {
      DEBUGTRACE_FUNCTION();
         // In the interest of not wasting time in the 99% case,
         // double-checking the nav message size and type.  This could
         // result in a crash if a user calls this method directly
         // with garbage, but that's a trade-off I'm willing to make
         // rather than encumber the other 99% with multiple identical
         // if tests on every message.
      navOut.xmit = navIn->getTransmitTime();
      navOut.preamble = navIn->asUnsignedLong(fsbPreamble, fnbPreamble,
                                              fscPreamble);
      navOut.TS = navIn->asUnsignedLong(fsbTS, fnbTS, fscTS);
      navOut.svid = navIn->asUnsignedLong(fsbj, fnbj, fscj);
      navOut.svUnhealthy = navIn->asBool(fsbHj);
      navOut.dataInvalid = navIn->asBool(fsblj);
      DEBUGTRACE("svUnhealthy=" << navOut.svUnhealthy);
      DEBUGTRACE("dataInvalid=" << navOut.dataInvalid);
      navOut.P1 = navIn->asUnsignedLong(fsbP1, fnbP1, fscP1);
      navOut.P2 = navIn->asBool(fsbP2);
      navOut.KP = navIn->asUnsignedLong(fsbKP, fnbKP, fscKP);
      navOut.A = navIn->asBool(fsbA);
      return true;
   }


   bool PNBGLOCNavDataFactory ::
   processEarth(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      DEBUGTRACE_FUNCTION();
      NavSatelliteID key(navIn->getsatSys().id, navIn->getsatSys(),
                         navIn->getobsID(), navIn->getNavID());
      DEBUGTRACE("key = " << key);
      if (processIono)
      {
         DEBUGTRACE("iono");
         NavDataPtr p0 = std::make_shared<GLOCNavIono>();
         p0->timeStamp = navIn->getTransmitTime();
         p0->signal = NavMessageID(key, NavMessageType::Iono);
         GLOCNavIono *iono = dynamic_cast<GLOCNavIono*>(p0.get());
         iono->peakTECF2  = navIn->asUnsignedDouble(isbcA,inbcA,isccA);
         iono->solarIndex = navIn->asUnsignedDouble(isbcF107,inbcF107,isccF107);
         iono->geoIndex   = navIn->asUnsignedDouble(isbcAp,inbcAp,isccAp);
         navOut.push_back(p0);
      }
      if (processTim && timeAcc[key].isValid())
      {
         DEBUGTRACE("time");
         NavDataPtr p0 = std::make_shared<GLOCNavUT1TimeOffset>();
         GLOCNavUT1TimeOffset *to = dynamic_cast<GLOCNavUT1TimeOffset*>(
            p0.get());
         to->NB = navIn->asUnsignedLong(isbNB, inbNB, iscNB);
         to->B0 = navIn->asSignMagDouble(isbB0,inbB0,iscB0);
         to->B1 = navIn->asSignMagDouble(isbB1,inbB1,iscB1);
            // see the warning in GLOCNavUT1TimeOffset
         to->B2 = navIn->asSignMagDouble(isbB2,inbB2,iscB2) / 2.0;
            // Table 5.5 indicates this is signed, strangely enough.
         to->UTCTAI = navIn->asSignMagLong(isbUTCTAI, inbUTCTAI, iscUTCTAI);
         to->timeStamp = navIn->getTransmitTime();
            // reference time is always the start of the day
         to->refTime = GLONASSTime(timeAcc[key].N4, to->NB);
         to->signal = NavMessageID(key, NavMessageType::TimeOffset);
         navOut.push_back(p0);
      }
      return true;
   }
   

   bool PNBGLOCNavDataFactory ::
   processHealth(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      if (processHea)
      {
         NavSatelliteID key(navIn->getsatSys().id, navIn->getsatSys(),
                            navIn->getobsID(), navIn->getNavID());
         NavDataPtr p0 = std::make_shared<GLOCNavHealth>();
         key.sat.id = navIn->asUnsignedLong(fsbj, fnbj, fscj);
         p0->timeStamp = navIn->getTransmitTime();
         p0->signal = NavMessageID(key, NavMessageType::Health);
         dynamic_cast<GLOCNavHealth*>(p0.get())->Hj = navIn->asBool(fsbHj);
         dynamic_cast<GLOCNavHealth*>(p0.get())->lj = navIn->asBool(fsblj);
         navOut.push_back(p0);
      }
         // No reason to return false in this code so far, maybe later.
      return true;
   }


   bool PNBGLOCNavDataFactory ::
   processEph(unsigned long stringID, const PackedNavBitsPtr& navIn,
              NavDataPtrList& navOut)
   {
      DEBUGTRACE_FUNCTION();
      NavSatelliteID key(navIn->getsatSys().id, navIn->getsatSys(),
                         navIn->getobsID(), navIn->getNavID());
      if ((stringID < 10) || (stringID > 12))
      {
         DEBUGTRACE("not an eph stringID");
         return false; // not actually part of the ephemeris.
      }
      if (stringID == 10)
      {
      //    if (PNBNavDataFactory::processISC)
      //    {
      //       NavDataPtr p2 = std::make_shared<GLOCNavISC>();
      //       GLOCNavISC *isc = dynamic_cast<GLOCNavISC*>(p2.get());
      //       isc->timeStamp = navIn->getTransmitTime();
      //       isc->signal = NavMessageID(key, NavMessageType::ISC);
      //       isc->isc = navIn->asSignMagDouble(esbdtaun,enbdtaun,escdtaun);
      //       navOut.push_back(p2);
      //    }
         if (PNBNavDataFactory::processTim || PNBNavDataFactory::processAlm)
         {
            timeAcc[key].setN4(navIn->asUnsignedLong(esbN4,enbN4,escN4));
         }
      }
      if (!PNBNavDataFactory::processEph)
      {
         DEBUGTRACE("ephs not requested");
         DEBUGTRACE("processEph = " << PNBNavDataFactory::processEph);
         DEBUGTRACE("processAlm = " << PNBNavDataFactory::processAlm);
         DEBUGTRACE("processHea = " << PNBNavDataFactory::processHea);
         DEBUGTRACE("processTim = " << PNBNavDataFactory::processTim);
         DEBUGTRACE("processIono = " << PNBNavDataFactory::processIono);
         DEBUGTRACE("processISC = " << PNBNavDataFactory::processISC);
            // User doesn't want ephemerides so don't do any processing.
         return true;
      }
      if (ephAcc.find(key) == ephAcc.end())
      {
         DEBUGTRACE("new ephemeris");
            // set up a new ephemeris
         ephAcc[key].resize(3); // we only store strings 10-12
         ephAcc[key][stringID-firstEphString] = navIn;
            // nothing in navOut yet and no further processing because
            // we only have one of four strings at this point.
         return true;
      }
      std::vector<PackedNavBitsPtr> &ephS(ephAcc[key]);
      ephS[stringID-firstEphString] = navIn;
      DEBUGTRACE("stored ephS[" << (stringID-firstEphString) << "] for "<< key);
         // stop processing if we don't have a complete, consecutive
         // set of strings
      if (!ephS[str10] || !ephS[str11] || !ephS[str12] ||
          (ephS[str10]->getNumBits() != 300) ||
          (ephS[str11]->getNumBits() != 300) ||
          (ephS[str12]->getNumBits() != 300) ||
          (ephS[str11]->getTransmitTime()-ephS[str10]->getTransmitTime() != 3)||
          (ephS[str12]->getTransmitTime()-ephS[str11]->getTransmitTime() != 3))
      {
         DEBUGTRACE("!ephS[str10]=" << (!ephS[str10]));
         DEBUGTRACE("!ephS[str11]=" << (!ephS[str11]));
         DEBUGTRACE("!ephS[str12]=" << (!ephS[str12]));
         if (ephS[str10] && ephS[str11] && ephS[str12])
         {
            DEBUGTRACE("ephS[str10]->getNumBits()=" << ephS[str10]->getNumBits());
            DEBUGTRACE("ephS[str11]->getNumBits()=" << ephS[str11]->getNumBits());
            DEBUGTRACE("ephS[str12]->getNumBits()=" << ephS[str12]->getNumBits());
            DEBUGTRACE("dT(11,10)=" << (ephS[str11]->getTransmitTime()-ephS[str10]->getTransmitTime()));
            DEBUGTRACE("dT(12,11)=" << (ephS[str12]->getTransmitTime()-ephS[str11]->getTransmitTime()));
         }
         return true;
      }
      DEBUGTRACE("Sufficient data for ephemeris. Proceeding");
      NavDataPtr p0 = std::make_shared<GLOCNavEph>();
      GLOCNavEph *eph = dynamic_cast<GLOCNavEph*>(p0.get());
      eph->signal = NavMessageID(key, NavMessageType::Ephemeris);
      if (!processHeader(ephS[str10], eph->header))
      {
         return false;
      }
      if (!processHeader(ephS[str11], eph->header11))
      {
         return false;
      }
      if (!processHeader(ephS[str12], eph->header12))
      {
         return false;
      }
      eph->timeStamp = eph->header.xmit;
      DEBUGTRACE("N4 bits: " << esiN4 << " " << esbN4 << " " << enbN4 << " " << escN4);
      DEBUGTRACE("NT bits: " << esiNT << " " << esbNT << " " << enbNT << " " << escNT);
      DEBUGTRACE("tb bits: " << esitb << " " << esbtb << " " << enbtb << " " << esctb);
      DEBUGTRACE("tb: " << hex << ephS[esitb]->asUnsignedLong(82,10,1));
      eph->N4 = ephS[esiN4]->asUnsignedLong(esbN4, enbN4, escN4);
      eph->NT = ephS[esiNT]->asUnsignedLong(esbNT, enbNT, escNT);
      eph->Mj = static_cast<GLOCSatType>(
         ephS[esiMj]->asUnsignedLong(esbMj, enbMj, escMj));
      eph->PS = ephS[esiPS]->asUnsignedLong(esbPS, enbPS, escPS);
      eph->tb = ephS[esitb]->asUnsignedLong(esbtb, enbtb, esctb);
      DEBUGTRACE("N4 = " << (unsigned)eph->N4);
      DEBUGTRACE("NT = " << eph->NT);
      DEBUGTRACE("tb (string " << (esitb+firstEphString) << ") = " << eph->tb);
      eph->Toe = GLONASSTime(eph->N4, eph->NT, eph->tb, TimeSystem::GLO);
         // change toe from Moscow Time to UTC(SU) aka TimeSystem::GLO
      eph->Toe -= 10800;
      eph->Toe.setTimeSystem(TimeSystem::GLO);
      eph->EjE = ephS[esiEjE]->asUnsignedLong(esbEjE, enbEjE, escEjE);
      eph->EjT = ephS[esiEjT]->asUnsignedLong(esbEjT, enbEjT, escEjT);
      eph->RjE = static_cast<GLOCRegime>(
         ephS[esiRjE]->asUnsignedLong(esbRjE, enbRjE, escRjE));
      eph->RjT = static_cast<GLOCRegime>(
         ephS[esiRjT]->asUnsignedLong(esbRjT, enbRjT, escRjT));
      eph->FjE = ephS[esiFjE]->asSignMagLong(esbFjE, enbFjE, escFjE);
      eph->FjT = ephS[esiFjT]->asSignMagLong(esbFjT, enbFjT, escFjT);
      eph->clkBias = ephS[esitauj]->asSignMagDouble(esbtauj, enbtauj, esctauj);
      eph->freqBias = ephS[esigammaj]->asSignMagDouble(
         esbgammaj, enbgammaj, escgammaj);
         /// @todo Not sure if this is signed or not but it seems likely.
      eph->driftRate = ephS[esiBetaj]->asSignMagDouble(
         esbBetaj, enbBetaj, escBetaj);
      eph->tauc = ephS[esitauc]->asSignMagDouble(esbtauc, enbtauc, esctauc);
      eph->taucdot = ephS[esitaucdot]->asSignMagDouble(
         esbtaucdot, enbtaucdot, esctaucdot);
      eph->pos[0]=ephS[esixj]->asSignMagDouble(esbxj,enbxj,escxj);
      eph->pos[1]=ephS[esiyj]->asSignMagDouble(esbyj,enbyj,escyj);
      eph->pos[2]=ephS[esizj]->asSignMagDouble(esbzj,enbzj,esczj);
      eph->vel[0]=ephS[esixjp]->asSignMagDouble(esbxjp,enbxjp,escxjp);
      eph->vel[1]=ephS[esiyjp]->asSignMagDouble(esbyjp,enbyjp,escyjp);
      eph->vel[2]=ephS[esizjp]->asSignMagDouble(esbzjp,enbzjp,esczjp);
      eph->acc[0]=ephS[esixjpp]->asSignMagDouble(esbxjpp,enbxjpp,escxjpp);
      eph->acc[1]=ephS[esiyjpp]->asSignMagDouble(esbyjpp,enbyjpp,escyjpp);
      eph->acc[2]=ephS[esizjpp]->asSignMagDouble(esbzjpp,enbzjpp,esczjpp);
      eph->apcOffset[0] = ephS[esiDeltaxjpc]->asSignMagDouble(
         esbDeltaxjpc, enbDeltaxjpc, escDeltaxjpc);
      eph->apcOffset[1] = ephS[esiDeltayjpc]->asSignMagDouble(
         esbDeltayjpc, enbDeltayjpc, escDeltayjpc);
      eph->apcOffset[2] = ephS[esiDeltazjpc]->asSignMagDouble(
         esbDeltazjpc, enbDeltazjpc, escDeltazjpc);
      eph->tauDelta = ephS[esiDeltataujL3]->asSignMagDouble(
         esbDeltataujL3, enbDeltataujL3, escDeltataujL3);
      eph->tauGPS = ephS[esiTauGPS]->asSignMagDouble(
         esbTauGPS, enbTauGPS, escTauGPS);
      if (ltdmpAcc[key].tbMatch(eph->tb))
      {
         eph->ltdmp = ltdmpAcc[key];
      }
         // Always set the "main" health according to all health info,
         // since it's going to be the one being referenced 99% of the
         // time.  Copy the value into the other headers for
         // consistency.
      eph->header.health =
         eph->header11.health =
         eph->header12.health =
         ((eph->header.svUnhealthy || eph->header11.svUnhealthy ||
           eph->header12.svUnhealthy)
          ? SVHealth::Unhealthy
          : SVHealth::Healthy);


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


   bool PNBGLOCNavDataFactory ::
   processAlm(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      DEBUGTRACE_FUNCTION();
      SatID xmitSat(navIn->getsatSys());
         // Key used for data accumulating, which we don't separate by
         // subject satellite.
      NavSatelliteID key(0, xmitSat, navIn->getobsID(), navIn->getNavID());
      if (!PNBNavDataFactory::processAlm)
      {
            // User doesn't want almanacs so don't do any processing.
         return true;
      }
      NavDataPtr p0 = std::make_shared<GLOCNavAlm>();
      GLOCNavAlm *alm = dynamic_cast<GLOCNavAlm*>(p0.get());
      alm->signal = NavMessageID(key, NavMessageType::Almanac);
      if (!processHeader(navIn, alm->header))
      {
         return false;
      }
      alm->timeStamp = alm->header.xmit;
      alm->signal.sat.id = alm->header.svid;
      alm->orbitType = static_cast<GLOCOrbitType>(
         navIn->asUnsignedLong(asbTO, anbTO, ascTO));
      alm->numSVs = navIn->asUnsignedLong(asbNS, anbNS, ascNS);
      alm->aoa = navIn->asUnsignedLong(asbE, anbE, ascE);
      alm->NA = navIn->asUnsignedLong(asbNA, anbNA, ascNA);
      alm->statusReg = navIn->asUnsignedLong(asbSR, anbSR, ascSR);
      alm->satType = static_cast<GLOCSatType>(
         navIn->asUnsignedLong(asbM, anbM, ascM));
      alm->tau = navIn->asSignMagDouble(asbtau, anbtau, asctau);
      alm->lambda = navIn->asSignMagDouble(asblambda, anblambda, asclambda);
      alm->tLambda = navIn->asUnsignedDouble(
         asbtlambda, anbtlambda, asctlambda);
      alm->deltai = navIn->asSignMagDouble(asbDeltai, anbDeltai, ascDeltai);
      alm->ecc = navIn->asSignMagDouble(asbepsilon, anbepsilon, ascepsilon);
      alm->omega = navIn->asSignMagDouble(asbomega, anbomega, ascomega);
      alm->deltaT = navIn->asSignMagDouble(asbDeltaT, anbDeltaT, ascDeltaT);
      alm->deltaTdot = navIn->asSignMagDouble(
         asbDeltaTdot, anbDeltaTdot, ascDeltaTdot);
      alm->header.health =
         (alm->header.svUnhealthy ? SVHealth::Unhealthy : SVHealth::Healthy);
      int N4 = 0;
      if (timeAcc[key].isValid())
      {
         N4 = timeAcc[key].N4;
      }
      else
      {
            // No ephemeris yet to supply the N4 term, make a best
            // guess based on the transmit time.
         GLONASSTime getN4(alm->timeStamp);
         N4 = getN4.epoch;
      }
      alm->Toa = gnsstk::GLONASSTime(N4, alm->NA, alm->tLambda,
                                     gnsstk::TimeSystem::GLO);
      alm->fixFit();
      navOut.push_back(p0);
      return true;
   }


   bool PNBGLOCNavDataFactory ::
   processLTDMP(unsigned long stringID,
                const PackedNavBitsPtr& navIn)
   {
      DEBUGTRACE_FUNCTION();
      uint16_t tb;      ///< Instant in Moscow time this data relates to.
      tb = navIn->asUnsignedLong(lsbtb, lnbtb, lsctb);
      DEBUGTRACE("tb (string " << stringID << ") = " << tb);
      NavSatelliteID key(navIn->getsatSys().id, navIn->getsatSys(),
                         navIn->getobsID(), navIn->getNavID());
      GLOCNavLTDMP *ltdmp = &ltdmpAcc[key];
      switch (stringID)
      {
         case 31:
            ltdmp->tb31 = tb;
            if (!processHeader(navIn, ltdmp->header31))
            {
               return false;
            }
            ltdmp->dax0 = navIn->asSignMagDouble(lsbdax0, lnbdax0, lscdax0);
            ltdmp->day0 = navIn->asSignMagDouble(lsbday0, lnbday0, lscday0);
            ltdmp->daz0 = navIn->asSignMagDouble(lsbdaz0, lnbdaz0, lscdaz0);
            ltdmp->ax1 = navIn->asSignMagDouble(lsbax1, lnbax1, lscax1);
            ltdmp->ay1 = navIn->asSignMagDouble(lsbay1, lnbay1, lscay1);
            ltdmp->az1 = navIn->asSignMagDouble(lsbaz1, lnbaz1, lscaz1);
            ltdmp->ax2 = navIn->asSignMagDouble(lsbax2, lnbax2, lscax2);
            ltdmp->ay2 = navIn->asSignMagDouble(lsbay2, lnbay2, lscay2);
            ltdmp->az2 = navIn->asSignMagDouble(lsbaz2, lnbaz2, lscaz2);
            break;
         case 32:
            ltdmp->tb32 = tb;
            if (!processHeader(navIn, ltdmp->header32))
            {
               return false;
            }
            ltdmp->ax3 = navIn->asSignMagDouble(lsbax3, lnbax3, lscax3);
            ltdmp->ay3 = navIn->asSignMagDouble(lsbay3, lnbay3, lscay3);
            ltdmp->az3 = navIn->asSignMagDouble(lsbaz3, lnbaz3, lscaz3);
            ltdmp->ax4 = navIn->asSignMagDouble(lsbax4, lnbax4, lscax4);
            ltdmp->ay4 = navIn->asSignMagDouble(lsbay4, lnbay4, lscay4);
            ltdmp->az4 = navIn->asSignMagDouble(lsbaz4, lnbaz4, lscaz4);
            break;
         default:
            return false;
      }
      return true;
   }


   void PNBGLOCNavDataFactory ::
   resetState()
   {
      DEBUGTRACE_FUNCTION();
      timeAcc.clear();
      ephAcc.clear();
      almAcc.clear();
   }


   void PNBGLOCNavDataFactory::TimeMeta ::
   setN4(unsigned v)
   {
      N4 = v;
   }


   bool PNBGLOCNavDataFactory::TimeMeta ::
   isValid()
   {
      return N4.is_valid();
   }

} // namespace gnsstk
