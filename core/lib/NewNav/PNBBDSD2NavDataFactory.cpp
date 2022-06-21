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
#include "PNBBDSD2NavDataFactory.hpp"
#include "BDSD2NavEph.hpp"
#include "BDSD2NavTimeOffset.hpp"
#include "BDSD2NavHealth.hpp"
#include "BDSD2NavIono.hpp"
#include "BDSD2NavISC.hpp"
#include "TimeCorrection.hpp"
#include "TimeString.hpp"
#include "BDSD2Bits.hpp"

using namespace std;
using namespace gnsstk::bds;

namespace gnsstk
{
      // subframe 5:
      //   from    to      what
      // - 1       13      iono
      // - 14      34      reserved
      // I 35              health
      // I 36              health + wna
      // I 37      60      alm
      // - 61      73      iono
      // - 74      94      reserved
      // I 95      100     alm
      // I 101             gnss time offset
      // I 102             utc time offset
      // I 103     115     alm (alt reserved)
      // I 116             health (alt reserved)
      // - 117     120     reserved

   bool PNBBDSD2NavDataFactory ::
   addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
           double cadence)
   {
      if ((navIn->getNavID().navType != NavType::BeiDou_D2) ||
          (navIn->getNumBits() != 300))
      {
            // This class only processes BeiDou D2 NAV.
         return false;
      }
      bool rv = true;
      try
      {
         unsigned long pgid;
         unsigned long sfid = navIn->asUnsignedLong(
            fsbFraID,fnbFraID,fscFraID);
         // cerr << "sfid=" << sfid;
         unsigned long pnum;
            // double TOA;
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
               /// @todo Implement a parity check
         }
         switch (sfid)
         {
            case 1:
               pgid = navIn->asUnsignedLong(esbPnum,enbPnum,escPnum);
               rv = processEph(pgid, navIn, navOut);
               break;
            case 5:
               pgid = navIn->asUnsignedLong(asbPnum,anbPnum,ascPnum);
               if (pgid == 35)
               {
                  rv = processSF5Pg35(navIn, navOut);
               }
               else if (pgid == 36)
               {
                  rv = processSF5Pg36(navIn, navOut);
               }
               else if (((pgid >= 37) && (pgid <= 60)) ||
                        ((pgid >= 95) && (pgid <= 100)) ||
                        ((pgid >= 103) && (pgid <= 115)))
               {
                  rv = processAlm(pgid, navIn, navOut);
               }
               else if (pgid == 101)
               {
                  rv = processSF5Pg101(navIn, navOut);
               }
               else if (pgid == 102)
               {
                  rv = processSF5Pg102(navIn, navOut);
               }
               else if (pgid == 116)
               {
                  rv = processSF5Pg116(navIn, navOut);
               }
               break;
            default:
                  // quietly ignore other subframe IDs, which we don't process.
               rv = true;
               break;
         }
         // cerr << endl;
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


   bool PNBBDSD2NavDataFactory ::
   processAlm(unsigned pnum, const PackedNavBitsPtr& navIn,
              NavDataPtrList& navOut)
   {
      SatID xmitSat(navIn->getsatSys());
         // Key used for data accumulating, which we don't separate by
         // subject satellite.
      NavSatelliteID key(0, xmitSat, navIn->getobsID(), navIn->getNavID());
      if (PNBNavDataFactory::processAlm || PNBNavDataFactory::processHea)
      {
            // We need to store AmEpID for almanacs and/or health.
            // The ICD doesn't describe this very clearly, and the
            // expanded almanacs don't seem to be broadcast on D2 at
            // the moment, but the best I can ascertain is that the
            // most recent AmEpID should be used.  The problem is it
            // could potentially change between the most recent usable
            // subframe with an AmEpID and the subframe for which you
            // intend to use it.  The best we can do is make sure the
            // AmEpID is within the same superframe.
         if (((pnum >= 37) && (pnum <= 60)) ||
             ((pnum >= 95) && (pnum <= 100)))
         {
            amEpIDMap[key].setValues(navIn->getTransmitTime(),
                                     navIn->asUnsignedLong(asbAmEpID,anbAmEpID,
                                                           ascAmEpID));
         }
      }
      if (!PNBNavDataFactory::processAlm)
      {
            // User doesn't want almanacs so don't do any processing.
         return true;
      }
         // Only sf 5 pg 103-115 have almanacs in the expanded almanac.
      if ((pnum >= 103) && (pnum <= 115))
      {
            // Make sure that the stored AmEpID is in the same master
            // frame as the AmID value, and that the AmEpID indicates
            // an expanded subframe.
         if (!amEpIDMap[key].isExpanded(navIn->getTransmitTime()))
         {
               // This is not an expanded almanac, it is either a
               // reserved page or we simply don't know, so skip it.
            return true;
         }
      }
         // Actual satellite ID (subject and transmit)
      unsigned long svid = pnum; // svid is the same as page num for sf4
         // The ICD doesn't explicitly state what the SV ID is for the
         // D2 almanacs, so we have to guess.
      if ((pnum >= 37) && (pnum <= 60))
      {
         svid = pnum-36; // SV ID 1-24 (guess)
      }
      else if ((pnum >= 95) && (pnum <= 100))
      {
         svid = pnum-70; // SV ID 25-30 (guess)
      }
      else if ((pnum >= 103) && (pnum <= 115))
      {
            // AmID is in the same position as AmEpID, it's just a
            // different subframe/page.
         uint8_t amID = navIn->asUnsignedLong(asbAmEpID,anbAmEpID,ascAmEpID);
            // cerr << "  AmID = " << (unsigned)amID << endl;
            // see Table 5-22 in ICD-B1I
         switch (amID)
         {
            case 0x00:
               return true; // reserved values, ignore
            case 0x01:
               svid = pnum-72; // SV ID 31-43
               break;
            case 0x02:
               svid = pnum-59; // SV ID 44-56
               break;
            case 0x03:
               if (pnum < 110)
                  svid = pnum-46; // SV ID 57-63
               else
                  return true; // reserved values, ignore
               break;
            default:
               GNSSTK_THROW(Exception("Invalid AmID value " +
                                     StringUtils::asString((unsigned)amID)));
               break;
         }
      }
      NavSatelliteID sat(svid, xmitSat, navIn->getobsID(), navIn->getNavID());
      auto alm = std::make_shared<BDSD2NavAlm>();
      alm->isDefault = isAlmDefault(navIn);
      // cerr << "  svid=" << svid << "  default=" << alm->isDefault;
         // NavData
      alm->timeStamp = navIn->getTransmitTime();
      alm->signal = NavMessageID(sat, NavMessageType::Almanac);
         // OrbitData
         // OrbitDataKepler
      alm->xmitTime = alm->timeStamp;
         //alm->health is set below
      alm->M0 = navIn->asSignedDouble(asbM0m,anbM0m,asbM0l,anbM0l,ascM0);
      alm->ecc = navIn->asUnsignedDouble(asbEcc,anbEcc,ascEcc);
      alm->Ahalf = navIn->asUnsignedDouble(asbAhalfm,anbAhalfm,
                                           asbAhalfl,anbAhalfl,ascAhalf);
      alm->A = alm->Ahalf * alm->Ahalf;
      alm->OMEGA0 = navIn->asSignedDouble(asbOMEGA0m,anbOMEGA0m,
                                          asbOMEGA0l,anbOMEGA0l,ascOMEGA0);
         // alm->i0 is set below
      alm->w = navIn->asSignedDouble(asbwm,anbwm,asbwl,anbwl,ascw);
      alm->OMEGAdot = navIn->asSignedDouble(asbOMEGAdotm,anbOMEGAdotm,
                                            asbOMEGAdotl,anbOMEGAdotl,
                                            ascOMEGAdot);
      alm->af0 = navIn->asSignedDouble(asbA0,anbA0,ascA0);
      alm->af1 = navIn->asSignedDouble(asbA1,anbA1,ascA1);
         // BDSD2NavData
      alm->pre = navIn->asUnsignedLong(fsbPre,fnbPre,fscPre);
      alm->rev = navIn->asUnsignedLong(fsbRev,fnbRev,fscRev);
      alm->fraID = 5;
      alm->sow = navIn->asUnsignedLong(fsbSOWm,fnbSOWm,fsbSOWl,fnbSOWl,fscSOW);
         // BDSD2NavAlm
      alm->pnum = pnum;
      alm->toa = navIn->asUnsignedDouble(asbtoa,anbtoa,asctoa);
      unsigned long toaab = navIn->asUnsignedLong(asbtoa,anbtoa,1);
      // cerr << "sfid=" << sfid << "  pnum=" << pnum << "  xmit=" << xmitSat.id
      //      << "  subj=" << sat.sat.id << "  toa=" << alm->toa << "=" << toaab
      //      << endl;
      alm->deltai = navIn->asSignedDouble(asbdim,anbdim,asbdil,anbdil,ascdi);
      if ((sat.sat.id > 5) && (sat.sat.id < 59))
      {
            // MEO/IGSO satellite.  GEO satellites have 0 inclination offset.
         alm->i0 = BDSD2NavData::refioffset + alm->deltai;
      }
      if (!finishAlm(alm, false, key, navOut))
         almAcc[key].push_back(alm);
      return true;
   }


   bool PNBBDSD2NavDataFactory ::
   processEph(unsigned long pgid,
              const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
         // Tgd are scaled by 0.1 to get nanoseconds, then x10^-9 to get seconds
      constexpr double sf = 0.1e-9;
      if ((pgid < 1) || (pgid > 10))
         return false;
      NavSatelliteID key(navIn->getsatSys().id, navIn->getsatSys(),
                         navIn->getobsID(), navIn->getNavID());
      if (pgid == 1)
      {
         if (PNBNavDataFactory::processISC)
         {
            std::shared_ptr<BDSD2NavISC> isc = std::make_shared<BDSD2NavISC>();
            isc->timeStamp = navIn->getTransmitTime();
            isc->signal = NavMessageID(
               NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                              navIn->getobsID(), navIn->getNavID()),
               NavMessageType::ISC);
            isc->pre = navIn->asUnsignedLong(fsbPre,fnbPre,fscPre);
            isc->rev = navIn->asUnsignedLong(fsbRev,fnbRev,fscRev);
            isc->fraID = 1;
            isc->sow = navIn->asUnsignedLong(fsbSOWm,fnbSOWm,fsbSOWl,fnbSOWl,
                                             fscSOW);
            isc->tgd1 = sf*navIn->asSignedDouble(esbTGD1,enbTGD1,escTGD1);
            isc->tgd2 = sf*navIn->asSignedDouble(esbTGD2,enbTGD2,escTGD2);
            navOut.push_back(isc);
         }
         if (processHea)
         {
            std::shared_ptr<BDSD2NavHealth> hea  =
               std::make_shared<BDSD2NavHealth>();
            hea->timeStamp = navIn->getTransmitTime();
            hea->signal = NavMessageID(key, NavMessageType::Health);
            hea->isAlmHealth = false;
            hea->satH1 = navIn->asBool(xesbSatH1);
               // cerr << "add D2NAV eph health" << endl;
            navOut.push_back(hea);
         }
      }
      else if ((pgid == 2) && PNBNavDataFactory::processIono)
      {
         std::shared_ptr<BDSD2NavIono> iono =
            std::make_shared<BDSD2NavIono>();
         iono->timeStamp = navIn->getTransmitTime();
         iono->signal = NavMessageID(
            NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                           navIn->getobsID(), navIn->getNavID()),
            NavMessageType::Iono);
         iono->pre = navIn->asUnsignedLong(fsbPre,fnbPre,fscPre);
         iono->rev = navIn->asUnsignedLong(fsbRev,fnbRev,fscRev);
         iono->fraID = 1;
         iono->sow = navIn->asUnsignedLong(fsbSOWm,fnbSOWm,fsbSOWl,fnbSOWl,
                                           fscSOW);
         iono->alpha[0] = navIn->asSignedDouble(esbAlpha0m,enbAlpha0m,
                                                esbAlpha0l,enbAlpha0l,
                                                escAlpha0);
         iono->alpha[1] = navIn->asSignedDouble(esbAlpha1,enbAlpha1,
                                                escAlpha1);
         iono->alpha[2] = navIn->asSignedDouble(esbAlpha2,enbAlpha2,
                                                escAlpha2);
         iono->alpha[3] = navIn->asSignedDouble(esbAlpha3m,enbAlpha3m,
                                                esbAlpha3l,enbAlpha3l,
                                                escAlpha3);
         iono->beta[0] = navIn->asSignedDouble(esbBeta0,enbBeta0,escBeta0);
         iono->beta[1] = navIn->asSignedDouble(esbBeta1,enbBeta1,escBeta1);
         iono->beta[2] = navIn->asSignedDouble(esbBeta2m,enbBeta2m,
                                               esbBeta2l,enbBeta2l,
                                               escBeta2);
         iono->beta[3] = navIn->asSignedDouble(esbBeta3,enbBeta3,escBeta3);
         navOut.push_back(iono);
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
         ephAcc[key].resize(10);
         ephAcc[key][pgid-1] = navIn;
            // nothing in navOut yet and no further processing because
            // we only have one of three subframes at this point.
         return true;
      }
      std::vector<PackedNavBitsPtr> &ephSF(ephAcc[key]);
      ephSF[pgid-1] = navIn;
         // stop processing if we don't have all the necessary subframes
      for (unsigned i = 0; i < 10; i++)
      {
         if (i == pg2)
         {
               // Page 2 only contains Klobuchar model parameters
               // which we don't bother storing in ephemerides.
            continue;
         }
         if (!ephSF[i] || (ephSF[i]->getNumBits() != 300))
         {
               // cerr << "Not ready for full D2NAV eph processing" << endl
               //      << "  sf1: " << (bool)ephSF[sf1] << " "
               //      << (ephSF[sf1] ? ephSF[sf1]->getNumBits() : -1) << endl
               //      << "  sf2: " << (bool)ephSF[sf2] << " "
               //      << (ephSF[sf2] ? ephSF[sf2]->getNumBits() : -1) << endl
               //      << "  sf3: " << (bool)ephSF[sf3] << " "
               //      << (ephSF[sf3] ? ephSF[sf3]->getNumBits() : -1) << endl;
            return true;
         }
         if (i > 0)
         {
               // Stop processing if we don't have consecutive subframes.
               // BeiDou doesn't have anything like IODC/IODE to match
               // subframes.
               /** @bug I can't find in the ICD where it says what the
                * scale factor is for the SOW field.  It's stored in
                * 20 bits even though 604800 only requires 19 bits. */
            unsigned long sowA, sowB;
            if (i == pg3)
            {
                  // page 3, which follows page 2 (of course) may be
                  // absent, but since we don't care about page 2, we
                  // check page 3 against page 1 + (3 seconds).
               sowA = ephSF[i-2]->asUnsignedLong(fsbSOWm,fnbSOWm,fsbSOWl,
                                                 fnbSOWl,fscSOW) + 3;
            }
            else
            {
               sowA = ephSF[i-1]->asUnsignedLong(fsbSOWm,fnbSOWm,fsbSOWl,
                                                 fnbSOWl,fscSOW);
            }
            sowB = ephSF[i]->asUnsignedLong(fsbSOWm,fnbSOWm,fsbSOWl,fnbSOWl,
                                            fscSOW);
               // subframe 1 is broadcast every 3 seconds
            if ((sowB - sowA) != 3)
            {
                  // Even though the mismatch might be considered an error,
                  // we don't really want to mark it as such and rather
                  // consider it as a "valid" but unprocessable data set.
               return true;
            }
         }
      }
      std::shared_ptr<BDSD2NavEph> eph = std::make_shared<BDSD2NavEph>();
         // NavData
      eph->timeStamp = ephSF[pg1]->getTransmitTime();
      eph->signal = NavMessageID(key, NavMessageType::Ephemeris);
      // cerr << "Ready for full D2NAV eph processing for " << (NavSignalID)key << endl;
         // OrbitData = empty
         // OrbitDataKepler
      eph->xmitTime = eph->timeStamp;
      double toe = ephSF[esitoem]->asUnsignedDouble(esbtoem,enbtoem,
                                                    esbtoel,enbtoel,esctoe);
      double toc = ephSF[esitocm]->asUnsignedDouble(esbtocm,enbtocm,
                                                    esbtocl,enbtocl,esctoc);
      unsigned wn = ephSF[esiWN]->asUnsignedLong(esbWN,enbWN,escWN);
      eph->Toe = BDSWeekSecond(wn,toe);
      eph->Toc = BDSWeekSecond(wn,toc);
         // health is set below
      eph->Cuc = PackedNavBits::asSignedDouble
         ({esbCucm,esbCucl},
          {enbCucm,enbCucl},
          {esiCucm,esiCucl},
          ephSF, escCuc);
      // cout << "Cuc bits: " << hex
      //      << ephSF[esiCucm]->asUnsignedLong(esbCucm,enbCucm,esbCucl,enbCucl,
      //                                        escCuc)
      //      << dec << " " << eph->Cuc << endl;
      eph->Cus = ephSF[esiCusm]->asSignedDouble(esbCusm,enbCusm,esbCusl,enbCusl,
                                                escCus);
      // cout << "Cus bits: " << hex
      //      << ephSF[esiCus]->asUnsignedLong(esbCus,enbCus,escCus)
      //      << dec << " " << eph->Cus << endl;
      eph->Crc = ephSF[esiCrcm]->asSignedDouble(esbCrcm,enbCrcm,
                                                esbCrcl,enbCrcl,escCrc);
      eph->Crs = ephSF[esiCrs]->asSignedDouble(esbCrs,enbCrs,escCrs);
      eph->Cic = PackedNavBits::asSignedDouble({esbCicm,esbCici,esbCicl},
                                               {enbCicm,enbCici,enbCicl},
                                               {esiCicm,esiCici,esiCicl},
                                               ephSF, escCic);
      eph->Cis = ephSF[esiCis]->asSignedDouble(esbCis,enbCis,escCis);
      eph->M0 = PI * PackedNavBits::asSignedDouble
         ({esbM0m,esbM0i,esbM0l},
          {enbM0m,enbM0i,enbM0l},
          {esiM0m,esiM0i,esiM0l},
          ephSF, escM0);
      eph->dn  = ephSF[esidn]->asDoubleSemiCircles(esbdn,enbdn,escdn);
         // no dndot in BDS D2NAV
      eph->ecc = PackedNavBits::asUnsignedDouble({esbEccm,esbEcci,esbEccl},
                                                 {enbEccm,enbEcci,enbEccl},
                                                 {esiEccm,esiEcci,esiEccl},
                                                 ephSF, escEcc);
      eph->Ahalf = PackedNavBits::asUnsignedDouble
         ({esbAhalfm,esbAhalfi,esbAhalfl},
          {enbAhalfm,enbAhalfi,enbAhalfl},
          {esiAhalfm,esiAhalfi,esiAhalfl},
          ephSF, escAhalf);
      eph->A = eph->Ahalf * eph->Ahalf;
         // no Adot in BDS D2NAV
      eph->OMEGA0 = PI * PackedNavBits::asSignedDouble
         ({esbOMEGA0m,esbOMEGA0i,esbOMEGA0l},
          {enbOMEGA0m,enbOMEGA0i,enbOMEGA0l},
          {esiOMEGA0m,esiOMEGA0i,esiOMEGA0l},
          ephSF, escOMEGA0);
      eph->i0 = PI * PackedNavBits::asSignedDouble
         ({esbi0m,esbi0i1,esbi0i2,esbi0l},
          {enbi0m,enbi0i1,enbi0i2,enbi0l},
          {esii0m,esii0i1,esii0i2,esii0l},
          ephSF, esci0);
      eph->w = PI * PackedNavBits::asSignedDouble
         ({esbwm,esbwi,esbwl},
          {enbwm,enbwi,enbwl},
          {esiwm,esiwi,esiwl},
          ephSF, escw);
      eph->OMEGAdot = PI * PackedNavBits::asSignedDouble
         ({esbOMEGAdotm,esbOMEGAdoti,esbOMEGAdotl},
          {enbOMEGAdotm,enbOMEGAdoti,enbOMEGAdotl},
          {esiOMEGAdotm,esiOMEGAdoti,esiOMEGAdotl},
          ephSF, escOMEGAdot);
      eph->idot = ephSF[esiidotm]->asDoubleSemiCircles(esbidotm,enbidotm,
                                                       esbidotl,enbidotl,
                                                       escidot);
      eph->af0 = ephSF[esia0m]->asSignedDouble(esba0m,enba0m,esba0l,enba0l,
                                               esca0);
      eph->af1 = PackedNavBits::asSignedDouble
         ({esba1m,esba1i,esba1l},
          {enba1m,enba1i,enba1l},
          {esia1m,esia1i,esia1l},
          ephSF, esca1);
      eph->af2 = ephSF[esia2m]->asSignedDouble(esba2m,enba2m,esba2l,enba2l,
                                               esca2);
         // BDSD2NavData
      eph->pre = ephSF[pg1]->asUnsignedLong(fsbPre,fnbPre,fscPre);
      eph->rev = ephSF[pg1]->asUnsignedLong(fsbRev,fnbRev,fscRev);
      eph->fraID = ephSF[pg1]->asUnsignedLong(fsbFraID,fnbFraID,fscFraID);
      eph->sow = ephSF[pg1]->asUnsignedLong(fsbSOWm,fnbSOWm,fsbSOWl,fnbSOWl,
                                            fscSOW);
         // BDSD2NavEph
      eph->satH1 = ephSF[xesiSatH1]->asBool(xesbSatH1);
      eph->health = ((eph->satH1 == false) ? SVHealth::Healthy :
                     SVHealth::Unhealthy); // actually in OrbitDataKepler
      eph->uraIndex = ephSF[esiURAI]->asUnsignedLong(esbURAI,enbURAI,escURAI);
      eph->tgd1 = sf*ephSF[esiTGD1]->asSignedDouble(esbTGD1,enbTGD1,escTGD1);
      eph->tgd2 = sf*ephSF[esiTGD2]->asSignedDouble(esbTGD2,enbTGD2,escTGD2);
      eph->aodc = ephSF[esiAODC]->asUnsignedLong(esbAODC,enbAODC,escAODC);
      eph->aode = ephSF[esiAODE]->asUnsignedLong(esbAODE,enbAODE,escAODE);
      eph->fixFit();
      // cerr << "add D2NAV eph" << endl;
      navOut.push_back(eph);
         // Clear out the broadcast ephemeris that's been processed.
      ephAcc.erase(key);
      return true;
   }


   bool PNBBDSD2NavDataFactory ::
   processSF5Pg35(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      if (PNBNavDataFactory::processHea || PNBNavDataFactory::processAlm)
      {
         makeHealth(navIn,navOut,1,h1sbHea1m,h1nbHea1m,h1sbHea1l,
                    h1nbHea1l);
         makeHealth(navIn,navOut,2,h1sbHea2,h1nbHea2);
         makeHealth(navIn,navOut,3,h1sbHea3m,h1nbHea3m,h1sbHea3l,
                    h1nbHea3l);
         makeHealth(navIn,navOut,4,h1sbHea4,h1nbHea4);
         makeHealth(navIn,navOut,5,h1sbHea5,h1nbHea5);
         makeHealth(navIn,navOut,6,h1sbHea6m,h1nbHea6m,h1sbHea6l,
                    h1nbHea6l);
         makeHealth(navIn,navOut,7,h1sbHea7,h1nbHea7);
         makeHealth(navIn,navOut,8,h1sbHea8m,h1nbHea8m,h1sbHea8l,
                    h1nbHea8l);
         makeHealth(navIn,navOut,9,h1sbHea9,h1nbHea9);
         makeHealth(navIn,navOut,10,h1sbHea10,h1nbHea10);
         makeHealth(navIn,navOut,11,h1sbHea11,h1nbHea11);
         makeHealth(navIn,navOut,12,h1sbHea12,h1nbHea12);
         makeHealth(navIn,navOut,13,h1sbHea13m,h1nbHea13m,h1sbHea13l,
                    h1nbHea13l);
         makeHealth(navIn,navOut,14,h1sbHea14,h1nbHea14);
         makeHealth(navIn,navOut,15,h1sbHea15m,h1nbHea15m,h1sbHea15l,
                    h1nbHea15l);
         makeHealth(navIn,navOut,16,h1sbHea16,h1nbHea16);
         makeHealth(navIn,navOut,17,h1sbHea17,h1nbHea17);
         makeHealth(navIn,navOut,18,h1sbHea18m,h1nbHea18m,h1sbHea18l,
                    h1nbHea18l);
         makeHealth(navIn,navOut,19,h1sbHea19,h1nbHea19);
         SatID xmitSat(navIn->getsatSys());
         NavSatelliteID key(0, xmitSat, navIn->getobsID(), navIn->getNavID());
         finishAlm(false, key, navOut);
      }
      return true;
   }


   bool PNBBDSD2NavDataFactory ::
   processSF5Pg36(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      if (PNBNavDataFactory::processHea || PNBNavDataFactory::processAlm)
      {
         makeHealth(navIn,navOut,20,h2sbHea20m,h2nbHea20m,h2sbHea20l,
                    h2nbHea20l);
         makeHealth(navIn,navOut,21,h2sbHea21,h2nbHea21);
         makeHealth(navIn,navOut,22,h2sbHea22m,h2nbHea22m,h2sbHea22l,
                    h2nbHea22l);
         makeHealth(navIn,navOut,23,h2sbHea23,h2nbHea23);
         makeHealth(navIn,navOut,24,h2sbHea24,h2nbHea24);
         makeHealth(navIn,navOut,25,h2sbHea25m,h2nbHea25m,h2sbHea25l,
                    h2nbHea25l);
         makeHealth(navIn,navOut,26,h2sbHea26,h2nbHea26);
         makeHealth(navIn,navOut,27,h2sbHea27m,h2nbHea27m,h2sbHea27l,
                    h2nbHea27l);
         makeHealth(navIn,navOut,28,h2sbHea28,h2nbHea28);
         makeHealth(navIn,navOut,29,h2sbHea29,h2nbHea29);
         makeHealth(navIn,navOut,30,h2sbHea30,h2nbHea30);
      }
      if (PNBNavDataFactory::processAlm)
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
            // Set the fullWNa now that we have something to go on,
            // but only if we're processing almanac data, which is the
            // only situation where it's used.
         double toa = navIn->asUnsignedDouble(h2sbtoam,h2nbtoam,
                                              h2sbtoal,h2nbtoal,h2sctoa);
         unsigned long toamsb = navIn->asUnsignedLong(h2sbtoam,h2nbtoam,1);
         unsigned long toalsb = navIn->asUnsignedLong(h2sbtoal,h2nbtoal,1);
         unsigned long toaab = navIn->asUnsignedLong(h2sbtoam,h2nbtoam,
                                                     h2sbtoal,h2nbtoal,1);
         unsigned shortWNa = navIn->asUnsignedLong(h2sbWNa,h2nbWNa,h2scWNa);
         BDSWeekSecond ws(navIn->getTransmitTime());
         long refWeek = ws.week;
         unsigned fullWNa = timeAdjust8BitWeekRollover(shortWNa, refWeek);
         fullWNaMap[xmitSat.id] = BDSWeekSecond(fullWNa, toa);
         // cerr << "sfid=5  pnum=8  xmit=" << xmitSat.id << "  toa=" << toa
         //      << "=" << toaab << "  toa-msb=" << toamsb << " ("
         //      << std::bitset<h2nbtoam>(toamsb) << ")  toa-lsb=" << toalsb
         //      << " (" << std::bitset<h2nbtoal>(toalsb) << ")  fullWNa="
         //      << fullWNa << endl;
         // cerr << "  wna = " << fullWNa << " (" << shortWNa << ")  refWeek = "
         //      << refWeek << "  toa = " << toa << "  key = " << key;
         finishAlm(true, key, navOut);
      }
      return true;
   }


   bool PNBBDSD2NavDataFactory ::
   processSF5Pg101(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      if (PNBNavDataFactory::processTim)
      {
         constexpr double sf = 0.1; // A0/A1 are scaled by 0.1
            // This is my best guess at what is an appropriate
            // reference time.  The documentation is unclear.
            /** @todo figure out what the reference time is supposed
             * to be, which is not a high priority as the ICD itself
             * says "Not broadcast temporarily". */
         BDSWeekSecond ref(navIn->getTransmitTime());
         ref.sow = 0;
            // BDT-GPS time offset
         std::shared_ptr<BDSD2NavTimeOffset> gps =
            std::make_shared<BDSD2NavTimeOffset>();
         gps->timeStamp = navIn->getTransmitTime();
         gps->signal = NavMessageID(
            NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                           navIn->getobsID(), navIn->getNavID()),
            NavMessageType::TimeOffset);
         gps->tgt = TimeSystem::GPS; // by definition
         gps->a0 = sf*navIn->asSignedDouble(csbA0GPS,cnbA0GPS,cscA0GPS);
         gps->a1 = sf*navIn->asSignedDouble(csbA1GPSm,cnbA1GPSm,csbA1GPSl,
                                            cnbA1GPSl,cscA1GPS);
         gps->refTime = ref;
            // cerr << "add D2NAV time offset" << endl;
         if (!filterTimeOffset(gps->a0, gps->a1))
         {
            navOut.push_back(gps);
         }

            // BDT-Galileo time offset
         std::shared_ptr<BDSD2NavTimeOffset> gal =
            std::make_shared<BDSD2NavTimeOffset>();
         gal->timeStamp = navIn->getTransmitTime();
         gal->signal = NavMessageID(
            NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                           navIn->getobsID(), navIn->getNavID()),
            NavMessageType::TimeOffset);
         gal->tgt = TimeSystem::GAL; // by definition
         gal->a0 = sf*navIn->asSignedDouble(csbA0GALm,cnbA0GALm,csbA0GALl,
                                            cnbA0GALl,cscA0GAL);
         gal->a1 = sf*navIn->asSignedDouble(csbA1GAL,cnbA1GAL,cscA1GAL);
         gal->refTime = ref;
            // cerr << "add D2NAV time offset" << endl;
         if (!filterTimeOffset(gal->a0, gal->a1))
         {
            navOut.push_back(gal);
         }

            // BDT-GLONASS time offset
         std::shared_ptr<BDSD2NavTimeOffset> glo =
            std::make_shared<BDSD2NavTimeOffset>();
         glo->timeStamp = navIn->getTransmitTime();
         glo->signal = NavMessageID(
            NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                           navIn->getobsID(), navIn->getNavID()),
            NavMessageType::TimeOffset);
         glo->tgt = TimeSystem::GLO; // by definition
         glo->a0 = sf*navIn->asSignedDouble(csbA0GLO,cnbA0GLO,cscA0GLO);
         glo->a1 = sf*navIn->asSignedDouble(csbA1GLOm,cnbA1GLOm,csbA1GLOl,
                                            cnbA1GLOl,cscA1GLO);
         glo->refTime = ref;
            // cerr << "add D2NAV time offset" << endl;
         if (!filterTimeOffset(glo->a0, glo->a1))
         {
            navOut.push_back(glo);
         }
      }
      return true;
   }


   bool PNBBDSD2NavDataFactory ::
   processSF5Pg102(const PackedNavBitsPtr& navIn,NavDataPtrList& navOut)
   {
      if (PNBNavDataFactory::processTim)
      {
         std::shared_ptr<BDSD2NavTimeOffset> to =
            std::make_shared<BDSD2NavTimeOffset>();
         to->timeStamp = navIn->getTransmitTime();
         to->signal = NavMessageID(
            NavSatelliteID(navIn->getsatSys().id, navIn->getsatSys(),
                           navIn->getobsID(), navIn->getNavID()),
            NavMessageType::TimeOffset);
         to->tgt = TimeSystem::UTC; // by definition
         to->a0 = navIn->asSignedDouble(csbA0UTCm,cnbA0UTCm,csbA0UTCl,cnbA0UTCl,
                                        cscA0UTC);
         to->a1 = navIn->asSignedDouble(csbA1UTCm,cnbA1UTCm,csbA1UTCl,cnbA1UTCl,
                                        cscA1UTC);
         to->deltatLS = navIn->asLong(csbdtLSm,cnbdtLSm,csbdtLSl,cnbdtLSl,
                                      cscdtLS);
            // BDS D2 doesn't use tot or wnot
         to->wnLSF = navIn->asUnsignedLong(csbWNlsf,cnbWNlsf,cscWNlsf);
         to->dn = navIn->asUnsignedLong(csbDN,cnbDN,cscDN);
            // adjust week numbers to full week
         BDSWeekSecond ref(to->timeStamp);
         long refWeek = ref.week;
         to->wnLSF = timeAdjust8BitWeekRollover(to->wnLSF, refWeek);
            // This is my best guess at what is an appropriate
            // reference time.  The documentation is unclear.
         ref.sow = 0;
         to->refTime = ref;
            // DN appears to be intended to be in the range of 0-6,
            // but that's just a guess as dumping the data over a
            // couple of weeks in 2020 and 2021, it was always 6.
         to->effTime = BDSWeekSecond(to->wnLSF,to->dn * 86400);
         // cerr << "wnLSF="  << to->wnLSF << "  dn=" << to->dn << "  refTime=" << to->refTime << endl;
         to->deltatLSF = navIn->asLong(csbdtLSF,cnbdtLSF,cscdtLSF);
            // cerr << "add D2NAV time offset" << endl;
         if (!factControl.bdsTimeZZfilt || (to->a0 != 0.0) || (to->a1 != 0.0))
         {
            navOut.push_back(to);
         }
      }
      return true;
   }


   bool PNBBDSD2NavDataFactory ::
   processSF5Pg116(const PackedNavBitsPtr& navIn,NavDataPtrList& navOut)
   {
      if (PNBNavDataFactory::processHea || PNBNavDataFactory::processAlm)
      {
         unsigned o = 0;
         uint8_t amID = navIn->asUnsignedLong(h3sbAmID,h3nbAmID,h3scAmID);
         // cerr << "  AmID = " << (unsigned)amID << endl;
         switch (amID)
         {
            case 0x00:
               return true; // reserved values, ignore
            case 0x01:
                  // already set to 0
               break;
            case 0x02:
               o = 13;
               break;
            case 0x03:
               o = 26;
               break;
            default:
               GNSSTK_THROW(Exception("Invalid AmID value " +
                                     StringUtils::asString((unsigned)amID)));
               break;
         }
         makeHealth(navIn,navOut,o+31,h3sbHea31m,h3nbHea31m,h3sbHea31l,
                    h3nbHea31l);
         makeHealth(navIn,navOut,o+32,h3sbHea32,h3nbHea32);
         makeHealth(navIn,navOut,o+33,h3sbHea33m,h3nbHea33m,h3sbHea33l,
                    h3nbHea33l);
         makeHealth(navIn,navOut,o+34,h3sbHea34,h3nbHea34);
         makeHealth(navIn,navOut,o+35,h3sbHea35,h3nbHea35);
         makeHealth(navIn,navOut,o+36,h3sbHea36m,h3nbHea36m,h3sbHea36l,
                    h3nbHea36l);
         makeHealth(navIn,navOut,o+37,h3sbHea37,h3nbHea37);
            // AmID b11 only uses 31-37
         if (amID != 0x03)
         {
            makeHealth(navIn,navOut,o+38,h3sbHea38m,h3nbHea38m,h3sbHea38l,
                       h3nbHea38l);
            makeHealth(navIn,navOut,o+39,h3sbHea39,h3nbHea39);
            makeHealth(navIn,navOut,o+40,h3sbHea40,h3nbHea40);
            makeHealth(navIn,navOut,o+41,h3sbHea41,h3nbHea41);
            makeHealth(navIn,navOut,o+42,h3sbHea42,h3nbHea42);
            makeHealth(navIn,navOut,o+43,h3sbHea43m,h3nbHea43m,h3sbHea43l,
                       h3nbHea43l);
         }
         SatID xmitSat(navIn->getsatSys());
         NavSatelliteID key(0, xmitSat, navIn->getobsID(), navIn->getNavID());
         finishAlm(false, key, navOut);
      }
      return true;
   }


   void PNBBDSD2NavDataFactory ::
   makeHealth(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
              unsigned long subjID,
              unsigned startBit1, unsigned numBits1,
              unsigned startBit2, unsigned numBits2)
   {
      std::shared_ptr<BDSD2NavHealth> hea = std::make_shared<BDSD2NavHealth>();
      hea->timeStamp = navIn->getTransmitTime();
      hea->signal = NavMessageID(
         NavSatelliteID(subjID, navIn->getsatSys(), navIn->getobsID(),
                        navIn->getNavID()),
         NavMessageType::Health);
         // health is all "scaled" by 1, so just use the same constant.
      if (numBits2 > 0)
      {
         hea->svHealth = navIn->asUnsignedLong(startBit1, numBits1, startBit2,
                                               numBits2, h2scHea20);
      }
      else
      {
         hea->svHealth = navIn->asUnsignedLong(startBit1, numBits1, h2scHea20);
      }
      heaAcc[hea->signal] = hea->svHealth;
      // cerr << "heaAcc[" << hea->signal << "]=" << hex << hea->svHealth << dec << endl;
      if (PNBNavDataFactory::processHea)
      {
         navOut.push_back(hea);
      }
   }


   bool PNBBDSD2NavDataFactory ::
   isAlmDefault(const PackedNavBitsPtr& navIn)
   {
         // Create a PNB with the same metadata, but all 0s, which is
         // how the BeiDou default pages appear (except words 1-2
         // which we ignore).
      PackedNavBits defPage(navIn->getsatSys(), navIn->getobsID(),
                            navIn->getNavID(), navIn->getRxID(),
                            navIn->getTransmitTime(), navIn->getNumBits(),
                            false);
         // Start match at bit 90 which is the first bit of word 3,
         // and end at bit 289 which excludes the AmEpID bits and
         // parity in word 10.
      return defPage.matchBits(*navIn, 90, 289);
   }


   void PNBBDSD2NavDataFactory ::
   resetState()
   {
      fullWNaMap.clear();
      almAcc.clear();
      ephAcc.clear();
   }


   void PNBBDSD2NavDataFactory ::
   finishAlm(bool fromWNa, const NavSatelliteID& key, NavDataPtrList& navOut)
   {
      AlmPtrList& ndpl(almAcc[key]);
      // cerr << "almAcc[key] size before = " << ndpl.size() << endl;
      for (auto i = ndpl.begin(); i != ndpl.end();)
      {
         if (finishAlm(*i, fromWNa, key, navOut))
         {
            i = ndpl.erase(i);
         }
         else
         {
            ++i;
         }
      }
         // cerr << "almAcc[key] size after = " << ndpl.size() << endl;
   }


   bool PNBBDSD2NavDataFactory ::
   finishAlm(PNBBDSD2NavDataFactory::AlmPtr& alm, bool fromWNa,
             const NavSatelliteID& key, NavDataPtrList& navOut)
   {
      if (alm->isDefault && (heaAcc.find(alm->signal) != heaAcc.end()))
      {
            // Don't produce this almanac if the user only wants valid data.
         if (navValidity != NavValidityType::ValidOnly)
         {
            alm->Toe = alm->Toc = gnsstk::BDSWeekSecond(0,0);
            alm->healthBits = heaAcc[alm->signal];
               /// @todo see comment in BDSD2NavHealth::getHealth()
            alm->health = ((alm->healthBits == 0) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            alm->fixFit();
            // cerr << "add D2NAV alm (2a) " << alm->signal << " toa=" << alm->toa << endl;
            navOut.push_back(alm);
         }
            // Always return true so that the default data will be
            // handled as if it were produced, even if it wasn't.
            // IOW, don't accumulate default pages needlessly.
         return true;
      }
      else if ((fullWNaMap.find(key.xmitSat.id) != fullWNaMap.end()) &&
               (fullWNaMap[key.xmitSat.id].sow == alm->toa) &&
               (heaAcc.find(alm->signal) != heaAcc.end()))
      {
            // Produce a complete almanac when WNa and health are available.
         alm->Toe = alm->Toc = fullWNaMap[key.xmitSat.id];
         alm->healthBits = heaAcc[alm->signal];
            /// @todo see comment in BDSD2NavHealth::getHealth()
         alm->health = ((alm->healthBits == 0) ? SVHealth::Healthy :
                        SVHealth::Unhealthy);
         alm->fixFit();
         // cerr << "add D2NAV alm (2b) " << alm->signal << " toa=" << alm->toa << endl;
         navOut.push_back(alm);
         return true;
      }
      else if (fromWNa &&
               (fullWNaMap.find(key.xmitSat.id) != fullWNaMap.end()) &&
               (fullWNaMap[key.xmitSat.id].sow != alm->toa))
      {
            // We do NOT produce an almanac in this case because the
            // lack of a proper time stamp breaks the NavStore by
            // causing an exception to be thrown in CommonTime.
            // Always return true so that the default data will be
            // handled as if it were produced, even though it wasn't.
         // cerr << "discard D2NAV alm (2c) " << alm->signal << " toa=" << alm->toa << endl;
         return true;
      }
      else
      {
         // if (alm->isDefault)
         //    cerr << "default alm" << endl;
         // if (fromWNa)
         //    cerr << "updating from SF5p8" << endl;
         // if (fullWNaMap.find(key.xmitSat.id) == fullWNaMap.end())
         //    cerr << "alm missing WNa for " << key.xmitSat.id << endl;
         // else if (fullWNaMap[key.xmitSat.id].sow != alm->toa)
         //    cerr << "alm toa mismatch " << fullWNaMap[key.xmitSat.id].sow << " != " << alm->toa << endl;
         // if (heaAcc.find(alm->signal) == heaAcc.end())
         //    cerr << "alm missing health" << endl;
         return false;
      }
   }


   void PNBBDSD2NavDataFactory ::
   dumpState(std::ostream& s)
      const
   {
      unsigned long total = 0;
      s << "fullWNaMap.size() = " << fullWNaMap.size() << endl
        << "almAcc.size() = " << almAcc.size() << endl;
      for (const auto& i : almAcc)
      {
         total += i.second.size();
         for (const auto& p : i.second)
         {
            s << p->signal << "  toa=" << p->toa << endl;
         }
         // s << "  almAcc[" << i.first << "].size() = " << i.second.size()
         //   << endl;
      }
      s << "almAcc total size = " << total << endl
        << "ephAcc.size() = " << ephAcc.size() << endl;
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


   PNBBDSD2NavDataFactory::AmEpID ::
   AmEpID()
   {
         // note that this will get changed when setValues is called,
         // but this is really to prevent isExpanded from throwing an
         // exception if it gets called before setValues
      mfStart.setTimeSystem(TimeSystem::Any);
      mfEnd.setTimeSystem(TimeSystem::Any);
   }


   bool PNBBDSD2NavDataFactory::AmEpID ::
   isExpanded(const CommonTime& t) const
   {
      return ((amEpID == 0x03) &&
              (t >= mfStart) &&
              (t < mfEnd));
   }


   void PNBBDSD2NavDataFactory::AmEpID ::
   setValues(const BDSWeekSecond& t, uint8_t id)
   {
         // superframe is 6 minutes long
      BDSWeekSecond tmod(t);
      tmod.sow -= fmod(tmod.sow,360.0);
      mfStart = mfEnd = tmod;
      mfEnd += 360.0;
      amEpID = id;
   }


} // namespace gnsstk
