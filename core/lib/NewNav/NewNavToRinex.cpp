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

#include "NewNavToRinex.hpp"
#include "RinexNavDataFactory.hpp"
#include "GPSLNavEph.hpp"
#include "GalFNavEph.hpp"
#include "GalFNavHealth.hpp"
#include "GalINavEph.hpp"
#include "GalINavHealth.hpp"
#include "BDSD1NavEph.hpp"
#include "BDSD2NavEph.hpp"
#include "GLOFNavEph.hpp"
#include "GPSWeekSecond.hpp"
#include "BDSWeekSecond.hpp"
#include "KlobucharIonoNavData.hpp"
#include "NeQuickIonoNavData.hpp"
#include "DebugTrace.hpp"
#include "TimeString.hpp"

namespace gnsstk
{
   bool NewNavToRinex ::
   translate(const NavDataPtrList& data, HealthGetter& healthGet)
   {
      DEBUGTRACE_FUNCTION();
      bool rv = false, tmp;
         // Collection of satellite systems that made it into rndList.
         // Used to set the header system type.
      std::set<SatelliteSystem> systems;
      for (const auto& i : data)
      {
         tmp = fillHeader(i, header);
         if (tmp)
         {
               // No need to try to fill data from this object, as the
               // header and data objects are mutually exclusive.
            rv = true;
            continue;
         }
         Rinex3NavData rnd;
         tmp = fillData(i, rnd, healthGet);
         if (tmp)
         {
            rndList.push_back(rnd);
            systems.insert(rnd.sat.system);
            rv = true;
         }
      }
      if (systems.size() == 1)
      {
         header.fileSysSat.system = *systems.begin();
         RinexSatID rsat(-1, header.fileSysSat.system);
         header.fileSys = rsat.systemChar();
      }
      else if (systems.size() > 1)
      {
         header.fileSys = "M"; // Mixed
         header.fileSysSat.system = SatelliteSystem::Mixed;
      }
      return rv;
   }


   bool NewNavToRinex ::
   write(const std::string& filename)
   {
      DEBUGTRACE_FUNCTION();
      using namespace std;
      Rinex3NavStream rns(filename.c_str(), ios::out);
      if (!rns)
      {
         cerr << "Unable to open \"" << filename << "\" for output." << endl;
         return false;
      }
      rns << header;
      if (!rns)
      {
         cerr << "Failed to write header to \"" << filename << "\"." << endl;
         return false;
      }
      for (const auto& i : rndList)
      {
         rns << i;
         if (!rns)
         {
            cerr << "Failed to write data to \"" << filename << "\"." << endl;
            return false;
         }
      }
      return true;
   }


   bool NewNavToRinex ::
   fillHeader(const NavDataPtr& ndp, Rinex3NavHeader& h)
   {
      DEBUGTRACE_FUNCTION();
      bool rv = false;
      std::shared_ptr<StdNavTimeOffset> todPtr;
      std::shared_ptr<KlobucharIonoNavData> kiPtr;
      std::shared_ptr<NeQuickIonoNavData> neqiPtr;
      switch (ndp->signal.messageType)
      {
         case NavMessageType::TimeOffset:
            todPtr = std::dynamic_pointer_cast<StdNavTimeOffset>(ndp);
            if (todPtr)
            {
               TimeSystemCorrection tsc;
               if (fillTimeSystemCorrection(todPtr, tsc))
               {
                  h.valid |= Rinex3NavHeader::validTimeSysCorr;
                  h.mapTimeCorr[tsc.asString4()] = tsc;
                  if (todPtr->deltatLS > 0)
                  {
                     h.leapSeconds = todPtr->deltatLS;
                     h.leapDelta = todPtr->deltatLSF;
                     h.leapWeek = todPtr->wnLSF;
                     h.leapDay = todPtr->dn;
                     h.valid |= Rinex3NavHeader::validLeapSeconds;
                  }
                  rv = true;
               }
            }
            break;
         case NavMessageType::Iono:
            kiPtr =
               std::dynamic_pointer_cast<KlobucharIonoNavData>(ndp);
            if (kiPtr)
            {
                  // Only GPS Alpha and Beta parameters are
                  // supported in RINEX (e.g. no QZSS Klobuchar).
               if (ndp->signal.system != SatelliteSystem::GPS)
               {
                  break;
               }
               IonoCorr icA, icB;
               icA.type = IonoCorr::GPSA;
               icA.param[0] = kiPtr->alpha[0];
               icA.param[1] = kiPtr->alpha[1];
               icA.param[2] = kiPtr->alpha[2];
               icA.param[3] = kiPtr->alpha[3];
               icB.type = IonoCorr::GPSB;
               icB.param[0] = kiPtr->beta[0];
               icB.param[1] = kiPtr->beta[1];
               icB.param[2] = kiPtr->beta[2];
               icB.param[3] = kiPtr->beta[3];
               h.mapIonoCorr[icA.type] = icA;
               h.mapIonoCorr[icB.type] = icB;
               h.valid |= Rinex3NavHeader::validIonoCorr;
               rv = true;
            }
            else
            {
               neqiPtr =
                  std::dynamic_pointer_cast<NeQuickIonoNavData>(ndp);
               if (neqiPtr)
               {
                     // Only Galileo NeQuick parameters are
                     // supported in RINEX.
                  if (ndp->signal.system != SatelliteSystem::Galileo)
                  {
                     break;
                  }
                  IonoCorr ic;
                  ic.param[0] = neqiPtr->ai[0];
                  ic.param[1] = neqiPtr->ai[1];
                  ic.param[2] = neqiPtr->ai[2];
                  ic.type = IonoCorr::GAL;
                  h.mapIonoCorr[ic.type] = ic;
                  h.valid |= Rinex3NavHeader::validIonoCorr;
                  rv = true;
               }
            }
            break;
         default:
               // ignore.
            break;
      } // switch (ndp->signal.messageType)
      return rv;
   } // NewNavToRinex::fillHeader(const NavDataPtr& ndp, Rinex3NavHeader& h)


   bool NewNavToRinex ::
   fillData(const NavDataPtr& ndp, Rinex3NavData& rnd, HealthGetter& healthGet)
   {
      DEBUGTRACE_FUNCTION();
      if (!ndp)
      {
         DEBUGTRACE("null ndp");
         return false;
      }
      DEBUGTRACE(ndp->signal);
      if (ndp->signal.messageType != NavMessageType::Ephemeris)
      {
         DEBUGTRACE("non-ephemeris");
         return false;
      }
      if (!fillDataGPSLNav(ndp, rnd) &&
          !fillDataGalFNav(ndp, rnd, healthGet) &&
          !fillDataGalINav(ndp, rnd, healthGet) &&
          !fillDataBDSD1Nav(ndp, rnd) &&
          !fillDataBDSD2Nav(ndp, rnd) &&
          !fillDataGLOFNav(ndp, rnd))
      {
         DEBUGTRACE("NO TRANSLATION");
         return false;
      }
      return true;
   }


   bool NewNavToRinex ::
   fillDataGPSLNav(const NavDataPtr& ndp, Rinex3NavData& rnd)
   {
      DEBUGTRACE_FUNCTION();
      if (ndp->signal.nav != gnsstk::NavType::GPSLNAV)
      {
         DEBUGTRACE("not GPSLNAV");
         return false;
      }
      std::shared_ptr<OrbitDataKepler> odk =
         std::dynamic_pointer_cast<OrbitDataKepler>(ndp);
      if (!odk)
      {
         DEBUGTRACE("not an OrbitDataKepler");
         return false;
      }
      std::shared_ptr<GPSLNavEph> eph =
         std::dynamic_pointer_cast<GPSLNavEph>(ndp);
      if (!eph)
      {
         DEBUGTRACE("not a GPSLNavEph");
         return false;
      }
      if (!fillDataKepler(odk, rnd))
      {
         DEBUGTRACE("fillDataKepler failed");
         return false;
      }
      GPSWeekSecond xmit(eph->timeStamp);
      rnd.xmitTime = xmit.sow;
      rnd.weeknum = xmit.week;
      rnd.accuracy = ura2nominalAccuracy(eph->uraIndex);
      rnd.health = eph->healthBits;
      rnd.codeflgs = static_cast<short>(eph->codesL2);
      rnd.L2Pdata = eph->L2Pdata;
      rnd.IODC = eph->iodc;
      rnd.IODE = eph->iode;
      rnd.Tgd = eph->tgd;
      rnd.fitint = getLegacyFitInterval(eph->iodc, eph->fitIntFlag);
      return true;
   }


   bool NewNavToRinex ::
   fillDataGalFNav(const NavDataPtr& ndp, Rinex3NavData& rnd,
                   HealthGetter& healthGet)
   {
      DEBUGTRACE_FUNCTION();
      if (ndp->signal.nav != gnsstk::NavType::GalFNAV)
      {
         DEBUGTRACE("not GalFNAV");
         return false;
      }
      std::shared_ptr<OrbitDataKepler> odk =
         std::dynamic_pointer_cast<OrbitDataKepler>(ndp);
      if (!odk)
      {
         DEBUGTRACE("not an OrbitDataKepler");
         return false;
      }
      std::shared_ptr<GalFNavEph> eph =
         std::dynamic_pointer_cast<GalFNavEph>(ndp);
      if (!eph)
      {
         DEBUGTRACE("not a GalFNavEph");
         return false;
      }
      if (!fillDataKepler(odk, rnd))
      {
         DEBUGTRACE("fillDataKepler failed");
         return false;
      }
         // RINEX 3.04 Table A8 note 4 says the GAL week number
         // is identical to the GPS week number.  ergo, it's not
         // really a Galileo week number.
      GPSWeekSecond xmit(eph->timeStamp);
      rnd.xmitTime = xmit.sow;
      DEBUGTRACE("GAL F/Nav week number=" << xmit.week
                 << printTime(eph->timeStamp,"  date=%Y/%02m/%02d"));
      rnd.weeknum = xmit.week;
      rnd.IODnav = eph->iodnav1;
      rnd.datasources = 0;
      if ((eph->signal.obs.band == CarrierBand::L5) &&
          (eph->signal.obs.code == TrackingCode::E5aI))
      {
         rnd.datasources |= 0x02;
         rnd.datasources |= 0x100;
      }
      rnd.health = ((int)eph->dvsE5a & 0x01) << 3;
      rnd.health |= ((int)eph->hsE5a & 0x03) << 4;
      NavDataPtr healthPtr;
      NavSatelliteID sidE1B(eph->signal);
      sidE1B.obs.band = CarrierBand::L1;
      sidE1B.obs.code = TrackingCode::E1B;
      sidE1B.nav = NavType::GalINAV;
      if (healthGet.getHealth(sidE1B, eph->timeStamp, healthPtr))
      {
         DEBUGTRACE("got some health");
         auto fnh(std::dynamic_pointer_cast<GalINavHealth>(healthPtr));
         if (fnh)
         {
            DEBUGTRACE("and it's I/NAV");
               //healthPtr->dump(std::cerr, DumpDetail::Full);
            rnd.health |= ((int)fnh->dataValidityStatus & 0x01);
            rnd.health |= ((int)fnh->sigHealthStatus & 0x03) << 1;
         }
      }
      NavSatelliteID sidE5b(eph->signal);
      sidE5b.obs.band = CarrierBand::L1;
      sidE5b.obs.code = TrackingCode::E5bI;
      sidE5b.nav = NavType::GalINAV;
      if (healthGet.getHealth(sidE5b, eph->timeStamp, healthPtr))
      {
         DEBUGTRACE("got some health");
         auto fnh(std::dynamic_pointer_cast<GalINavHealth>(healthPtr));
         if (fnh)
         {
            DEBUGTRACE("and it's I/NAV");
               //healthPtr->dump(std::cerr, DumpDetail::Full);
            rnd.health |= ((int)fnh->dataValidityStatus & 0x01) << 6;
            rnd.health |= ((int)fnh->sigHealthStatus & 0x03) << 7;
         }
      }
      rnd.Tgd = eph->bgdE5aE1;
         /** @todo figure out how best to get BGD E5b/E1 which is not
          * in the F/NAV ephemeris. */
      rnd.accuracy = RinexNavDataFactory::encodeSISA(eph->sisaIndex);
      return true;
   }


   bool NewNavToRinex ::
   fillDataGalINav(const NavDataPtr& ndp, Rinex3NavData& rnd,
                   HealthGetter& healthGet)
   {
      DEBUGTRACE_FUNCTION();
      if (ndp->signal.nav != gnsstk::NavType::GalINAV)
      {
         DEBUGTRACE("not GalINAV");
         return false;
      }
      std::shared_ptr<OrbitDataKepler> odk =
         std::dynamic_pointer_cast<OrbitDataKepler>(ndp);
      if (!odk)
      {
         DEBUGTRACE("not an OrbitDataKepler");
         return false;
      }
      std::shared_ptr<GalINavEph> eph =
         std::dynamic_pointer_cast<GalINavEph>(ndp);
      if (!eph)
      {
         DEBUGTRACE("not a GalINavEph");
         return false;
      }
      if (!fillDataKepler(odk, rnd))
      {
         DEBUGTRACE("fillDataKepler failed");
         return false;
      }
         // RINEX 3.04 Table A8 note 4 says the GAL week number
         // is identical to the GPS week number.  ergo, it's not
         // really a Galileo week number.
      GPSWeekSecond xmit(eph->timeStamp);
      rnd.xmitTime = xmit.sow;
      DEBUGTRACE("GAL I/Nav week number=" << xmit.week
                 << printTime(eph->timeStamp,"  date=%Y/%02m/%02d"));
      rnd.weeknum = xmit.week;
      rnd.IODnav = eph->iodnav1;
      rnd.datasources = 0;
      if ((eph->signal.obs.band == CarrierBand::L1) &&
          (eph->signal.obs.code == TrackingCode::E1B))
      {
         rnd.datasources |= 0x01;
      }
      else if ((eph->signal.obs.band == CarrierBand::E5b) &&
          (eph->signal.obs.code == TrackingCode::E5bI))
      {
         rnd.datasources |= 0x04;
      }
      rnd.datasources |= 0x200;
      rnd.health = (int)eph->dvsE1B & 0x01;
      rnd.health |= ((int)eph->hsE1B & 0x03) << 1;
      NavDataPtr healthPtr;
      NavSatelliteID sidE5a(eph->signal);
      sidE5a.obs.band = CarrierBand::L5;
      sidE5a.obs.code = TrackingCode::E5aI;
      sidE5a.nav = NavType::GalFNAV;
      if (healthGet.getHealth(sidE5a, eph->timeStamp, healthPtr))
      {
         DEBUGTRACE("got some health");
         auto fnh(std::dynamic_pointer_cast<GalFNavHealth>(healthPtr));
         if (fnh)
         {
            DEBUGTRACE("and it's F/NAV");
               //healthPtr->dump(std::cerr, DumpDetail::Full);
            rnd.health |= ((int)fnh->dataValidityStatus & 0x01) << 3;
            rnd.health |= ((int)fnh->sigHealthStatus & 0x03) << 4;
         }
      }
      rnd.health |= ((int)eph->dvsE5b & 0x01) << 6;
      rnd.health |= ((int)eph->hsE5b & 0x03) << 7;
      rnd.Tgd = eph->bgdE5aE1;
      rnd.Tgd2 = eph->bgdE5bE1;
      rnd.accuracy = RinexNavDataFactory::encodeSISA(eph->sisaIndex);
      return true;
   }


   bool NewNavToRinex ::
   fillDataBDSD1Nav(const NavDataPtr& ndp, Rinex3NavData& rnd)
   {
      DEBUGTRACE_FUNCTION();
      if (ndp->signal.nav != gnsstk::NavType::BeiDou_D1)
      {
         DEBUGTRACE("not BeiDou_D1");
         return false;
      }
      std::shared_ptr<OrbitDataKepler> odk =
         std::dynamic_pointer_cast<OrbitDataKepler>(ndp);
      if (!odk)
      {
         DEBUGTRACE("not an OrbitDataKepler");
         return false;
      }
      std::shared_ptr<BDSD1NavEph> eph =
         std::dynamic_pointer_cast<BDSD1NavEph>(ndp);
      if (!eph)
      {
         DEBUGTRACE("not a BDSD1NavEph");
         return false;
      }
      if (!fillDataKepler(odk, rnd))
      {
         DEBUGTRACE("fillDataKepler failed");
         return false;
      }
      BDSWeekSecond xmit(eph->timeStamp);
      rnd.xmitTime = xmit.sow;
      rnd.weeknum = xmit.week;
      rnd.IODE = eph->aode;
      rnd.IODC = eph->aodc;
      rnd.health = eph->satH1;
      if (eph->uraIndex >= 15)
      {
         rnd.accuracy = 8192;
      }
      else
      {
         rnd.accuracy = ura2nominalAccuracy(eph->uraIndex);
      }
      rnd.Tgd = eph->tgd1;
      rnd.Tgd2 = eph->tgd2;
      return true;
   }


   bool NewNavToRinex ::
   fillDataBDSD2Nav(const NavDataPtr& ndp, Rinex3NavData& rnd)
   {
      DEBUGTRACE_FUNCTION();
      if (ndp->signal.nav != gnsstk::NavType::BeiDou_D2)
      {
         DEBUGTRACE("not BeiDou_D2");
         return false;
      }
      std::shared_ptr<OrbitDataKepler> odk =
         std::dynamic_pointer_cast<OrbitDataKepler>(ndp);
      if (!odk)
      {
         DEBUGTRACE("not an OrbitDataKepler");
         return false;
      }
      std::shared_ptr<BDSD2NavEph> eph =
         std::dynamic_pointer_cast<BDSD2NavEph>(ndp);
      if (!eph)
      {
         DEBUGTRACE("not a BDSD2NavEph");
         return false;
      }
      if (!fillDataKepler(odk, rnd))
      {
         DEBUGTRACE("fillDataKepler failed");
         return false;
      }
      BDSWeekSecond xmit(eph->timeStamp);
      rnd.xmitTime = xmit.sow;
      rnd.weeknum = xmit.week;
      rnd.IODE = eph->aode;
      rnd.IODC = eph->aodc;
      rnd.health = eph->satH1;
      if (eph->uraIndex >= 15)
      {
         rnd.accuracy = 8192;
      }
      else
      {
         rnd.accuracy = ura2nominalAccuracy(eph->uraIndex);
      }
      rnd.Tgd = eph->tgd1;
      rnd.Tgd2 = eph->tgd2;
      return true;
   }


   bool NewNavToRinex ::
   fillDataGLOFNav(const NavDataPtr& ndp, Rinex3NavData& rnd)
   {
      DEBUGTRACE_FUNCTION();
      if (ndp->signal.nav != gnsstk::NavType::GloCivilF)
      {
         DEBUGTRACE("not GLONASS FDMA");
         return false;
      }
      std::shared_ptr<GLOFNavEph> eph =
         std::dynamic_pointer_cast<GLOFNavEph>(ndp);
      if (!eph)
      {
         DEBUGTRACE("not a GLOFNavEph");
         return false;
      }
      rnd.time = eph->Toe;
      rnd.satSys = "R";
      rnd.PRNID = eph->slot;
      rnd.sat = RinexSatID(eph->slot, SatelliteSystem::Glonass);
      rnd.TauN = -(eph->clkBias);
      rnd.GammaN = eph->freqBias;
         // We're only getting seconds of week so it's odd but okay to
         // use GPSWeekSecond here.
      GPSWeekSecond mft(eph->ref);
      rnd.MFtime = mft.sow;
         // convert meters to km for RINEX
      rnd.px = eph->pos[0] / 1000.0;
      rnd.py = eph->pos[1] / 1000.0;
      rnd.pz = eph->pos[2] / 1000.0;
      rnd.vx = eph->vel[0] / 1000.0;
      rnd.vy = eph->vel[1] / 1000.0;
      rnd.vz = eph->vel[2] / 1000.0;
      rnd.ax = eph->acc[0] / 1000.0;
      rnd.ay = eph->acc[1] / 1000.0;
      rnd.az = eph->acc[2] / 1000.0;
      rnd.health = (eph->healthBits >> 2) & 1;
      rnd.freqNum = eph->signal.obs.freqOffs;
      rnd.ageOfInfo = eph->aod;
      return true;
   }


   bool NewNavToRinex ::
   fillDataKepler(const std::shared_ptr<OrbitDataKepler>& odk,
                  Rinex3NavData& rnd)
   {
      DEBUGTRACE_FUNCTION();
      if (!odk)
      {
         return false;
      }
      if (odk->signal.messageType != NavMessageType::Ephemeris)
      {
         DEBUGTRACE("non-ephemeris");
         return false;
      }
      rnd.time = odk->Toc;
      rnd.sat = odk->signal.sat;
      rnd.satSys = rnd.sat.systemChar();
      rnd.PRNID = rnd.sat.id;
         // Seconds of week are the same for everyone (the start of
         // the week is the same, conceptually, I'm not talking about
         // time offsets between systems).
      GPSWeekSecond tocWS(odk->Toc);
      GPSWeekSecond toeWS(odk->Toe);
      rnd.Toc = tocWS.sow;
      rnd.af0 = odk->af0;
      rnd.af1 = odk->af1;
      rnd.af2 = odk->af2;
      rnd.Cuc = odk->Cuc;
      rnd.Cus = odk->Cus;
      rnd.Crc = odk->Crc;
      rnd.Crs = odk->Crs;
      rnd.Cic = odk->Cic;
      rnd.Cis = odk->Cis;
      rnd.Toe = toeWS.sow;
      rnd.M0 = odk->M0;
      rnd.dn = odk->dn;
      rnd.ecc = odk->ecc;
      rnd.Ahalf = odk->Ahalf;
      rnd.OMEGA0 = odk->OMEGA0;
      rnd.i0 = odk->i0;
      rnd.w = odk->w;
      rnd.OMEGAdot = odk->OMEGAdot;
      rnd.idot = odk->idot;
      return true;
   }


   bool NewNavToRinex ::
   fillTimeSystemCorrection(const std::shared_ptr<StdNavTimeOffset>& toffs,
                            TimeSystemCorrection& tsc)
   {
      DEBUGTRACE_FUNCTION();
      if (!TimeSystemCorrection::convertTimeSystemToCorrType(
             toffs->src, toffs->tgt, tsc.type))
      {
         return false;
      }
      tsc.frTS = toffs->src;
      tsc.toTS = toffs->tgt;
      tsc.A0 = toffs->a0;
      tsc.A1 = toffs->a1;
      tsc.refTime = toffs->refTime;
         // This isn't really the geoProvider which is an SBAS thing,
         // but the RINEX 3.04 documentation says to put the "System
         // and number Snn of GNSS satellite broadcasting the time
         // system difference or..."
      RinexSatID sid(toffs->signal.sat);
      tsc.geoProvider = sid.toString();
      return true;
   }


   bool HealthGetter ::
   getHealth(const NavSatelliteID& nsid, const CommonTime& when,
             NavDataPtr& healthOut)
   {
      const auto& hmi = healthMap.find(nsid);
      if (hmi == healthMap.end())
      {
         return false;
      }
      const auto& ndmi = hmi->second.lower_bound(when);
      if (ndmi == hmi->second.end())
      {
         return false;
      }
      healthOut = ndmi->second;
      return true;
   }


   NavLibraryHealthGetter ::
   NavLibraryHealthGetter(NavLibrary& nl)
         : navLib(nl), xmitHealth(SVHealth::Any),
           valid(NavValidityType::ValidOnly), order(NavSearchOrder::User)
   {
   }


   bool NavLibraryHealthGetter ::
   getHealth(const NavSatelliteID& nsid, const CommonTime& when,
             NavDataPtr& healthOut)
   {
      NavMessageID nmid(nsid, NavMessageType::Health);
      return navLib.find(nmid, when, healthOut, xmitHealth, valid, order);
   }
}
