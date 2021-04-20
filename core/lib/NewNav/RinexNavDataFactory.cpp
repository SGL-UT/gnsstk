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
#include "RinexNavDataFactory.hpp"
#include "Rinex3NavStream.hpp"
#include "Rinex3NavHeader.hpp"
#include "GPSLNavHealth.hpp"
#include "GalINavEph.hpp"
#include "GalFNavEph.hpp"
#include "GalNavHealth.hpp"
#include "RinexTimeOffset.hpp"
#include "TimeString.hpp"

using namespace std;

static const std::string dts("%Y/%03j/%02H:%02M:%02S %P");

namespace gpstk
{
   RinexNavDataFactory ::
   RinexNavDataFactory()
   {
      supportedSignals.insert(NavSignalID(SatelliteSystem::GPS,
                                          CarrierBand::L1,
                                          TrackingCode::CA,
                                          NavType::GPSLNAV));
      supportedSignals.insert(NavSignalID(SatelliteSystem::Galileo,
                                          CarrierBand::L1,
                                          TrackingCode::E1B,
                                          NavType::GalINAV));
      supportedSignals.insert(NavSignalID(SatelliteSystem::Galileo,
                                          CarrierBand::E5b,
                                          TrackingCode::E5bI,
                                          NavType::GalINAV));
      supportedSignals.insert(NavSignalID(SatelliteSystem::Galileo,
                                          CarrierBand::L5,
                                          TrackingCode::E5aI,
                                          NavType::GalFNAV));
   }


   bool RinexNavDataFactory ::
   loadIntoMap(const std::string& filename, NavMessageMap& navMap)
   {
      bool rv = true;
      bool processEph = (procNavTypes.count(NavMessageType::Ephemeris) > 0);
      bool processHea = (procNavTypes.count(NavMessageType::Health) > 0);
      bool processTim = (procNavTypes.count(NavMessageType::TimeOffset) > 0);
         // check the validity
      bool check = false;
      bool expect = false;
      switch (navValidity)
      {
         case NavValidityType::ValidOnly:
            check = true;
            expect = true;
            break;
         case NavValidityType::InvalidOnly:
            check = true;
            expect = false;
            break;
               // Just treat everything else like All, which is to
               // say, no checks.
         default:
            break;
      }
      try
      {
         Rinex3NavStream is(filename.c_str(), ios::in);
         Rinex3NavHeader head;
         Rinex3NavData data;
         if (!is)
            return false;
         is >> head;
         if (processTim)
         {
               // time offset information only exists in RINEX headers.
               /// @todo what about embedded RINEX headers?
            NavDataPtrList navOut;
            if (!convertToOffset(head, navOut))
            {
               return false;
            }
            for (auto& i : navOut)
            {
               if (check)
               {
                  if (i->validate() == expect)
                  {
                     if (!addNavData(i))
                        return false;
                  }
               }
               else
               {
                  if (!addNavData(i))
                     return false;
               }
            }
         }
         if (!is)
            return false;
         while (is)
         {
            is >> data;
            if (!is)
            {
               if (is.eof())
                  break;
               else
                  return false; // some other error
            }
            NavDataPtr eph;
            std::list<gpstk::NavDataPtr> health;
            if (processEph)
            {
               if (!convertToOrbit(data, eph))
                  return false;
            }
            if (processHea)
            {
               if (!convertToHealth(data, health))
                  return false;
            }
            if (check)
            {
               if (processEph)
               {
                  if (eph->validate() == expect)
                  {
                     if (!addNavData(eph))
                        return false;
                  }
               }
               if (processHea)
               {
                  for (const auto& hp : health)
                  {
                     if (hp->validate() == expect)
                     {
                        if (!addNavData(hp))
                           return false;
                     }
                  }
               }
            }
            else
            {
               if (processEph)
               {
                  if (!addNavData(eph))
                     return false;
               }
               if (processHea)
               {
                  for (const auto& hp : health)
                  {
                     if (!addNavData(hp))
                        return false;
                  }
               }
            }
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


   std::string RinexNavDataFactory ::
   getFactoryFormats() const
   {
      if (procNavTypes.empty() ||
          (procNavTypes.count(NavMessageType::Ephemeris) > 0) ||
          (procNavTypes.count(NavMessageType::Health) > 0) ||
          (procNavTypes.count(NavMessageType::TimeOffset) > 0))
      {
         return "RINEX2, RINEX3";
      }
      return "";
   }


   bool RinexNavDataFactory ::
   convertToOrbit(const Rinex3NavData& navIn, NavDataPtr& navOut)
   {
      bool rv = true;
      GPSLNavEph *gps;
      GalINavEph *galINav;
      GalFNavEph *galFNav;
      switch (navIn.sat.system)
      {
         case SatelliteSystem::GPS:
            navOut = std::make_shared<GPSLNavEph>();
            gps = dynamic_cast<GPSLNavEph*>(navOut.get());
               // NavData
            fillNavData(navIn, navOut);
               // OrbitDataKepler
            fixTimeGPS(navIn, *gps);
            gps->health = ((navIn.health == 0) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            convertToOrbitDataKepler(navIn, gps);
               // tlm not available in RINEX NAV
               // GPSLNavEph
            gps->iodc = navIn.IODC;
            gps->iode = navIn.IODE;
            gps->fitIntFlag = (navIn.fitint <= 4 ? 0 : 1);
            gps->healthBits = navIn.health;
            gps->uraIndex = accuracy2ura(navIn.accuracy);
            gps->tgd = navIn.Tgd;
               // We don't have the A-S flag in rinex nav, so just
               // assume it's on, as it more than likely is.  Also
               // assume alert is off.  Maybe at some future point the
               // data and dump method will be changed to know that
               // the data is an unknown value.
            gps->asFlag = gps->asFlag2 = gps->asFlag3 = true;
            gps->alert = gps->alert2 = gps->alert3 = false;
            gps->codesL2 = (GPSLNavEph::L2Codes)navIn.codeflgs;
            gps->L2Pdata = (navIn.L2Pdata > 0);
            gps->fixFit();
            break;
         case SatelliteSystem::Galileo:
            if (((navIn.datasources & 0x01) == 0x01) ||
                ((navIn.datasources & 0x04) == 0x04))
            {
               navOut = std::make_shared<GalINavEph>();
               galINav = dynamic_cast<GalINavEph*>(navOut.get());
                  // NavData
               fillNavData(navIn, navOut);
                  // OrbitDataKepler
               fixTimeGalileo(navIn, *galINav);
                  // RINEX 3.04 Table A8 note 4 says the GAL week number
                  // is identical to the GPS week number.  ergo, it's not
                  // really a Galileo week number.
               galINav->Toe = GPSWeekSecond(navIn.weeknum, navIn.Toe);
                  //galINav->Toe.setTimeSystem(TimeSystem::GAL);
                  // @todo set sv health correctly
                  // galINav->health = ((navIn.health == 0) ? SVHealth::Healthy :
                  //                SVHealth::Unhealthy);
               convertToOrbitDataKepler(navIn, galINav);
                  /// @todo add IOD Nav, SISA, BGD, data source
               galINav->fixFit();
            }
            else if (navIn.datasources & 0x02)
            {
               navOut = std::make_shared<GalFNavEph>();
               galFNav = dynamic_cast<GalFNavEph*>(navOut.get());
                  // NavData
               fillNavData(navIn, navOut);
                  // OrbitDataKepler
               fixTimeGalileo(navIn, *galFNav);
                  // RINEX 3.04 Table A8 note 4 says the GAL week number
                  // is identical to the GPS week number.  ergo, it's not
                  // really a Galileo week number.
               galFNav->Toe = GPSWeekSecond(navIn.weeknum, navIn.Toe);
                  //galFNav->Toe.setTimeSystem(TimeSystem::GAL);
                  // @todo set sv health correctly
                  // galFNav->health = ((navIn.health == 0) ? SVHealth::Healthy :
                  //                SVHealth::Unhealthy);
               convertToOrbitDataKepler(navIn, galFNav);
                  /// @todo add IOD Nav, SISA, BGD, data source
               galFNav->fixFit();
            }
            else
            {
                  // don't know what to do with this.
               rv = false;
            }
            break;
         default:
               /// @todo add other GNSSes
            rv = false;
            break;
      }
      return rv;
   }


   void RinexNavDataFactory ::
   convertToOrbitDataKepler(const Rinex3NavData& navIn, OrbitDataKepler* navOut)
   {
         // OrbitDataKepler
      navOut->Toc = navIn.time;
         // navOut->health is signal-specific
      navOut->Cuc = navIn.Cuc;
      navOut->Cus = navIn.Cus;
      navOut->Crc = navIn.Crc;
      navOut->Crs = navIn.Crs;
      navOut->Cic = navIn.Cic;
      navOut->Cis = navIn.Cis;
      navOut->M0 = navIn.M0;
      navOut->dn = navIn.dn;
         // no dndot in RINEX 3
      navOut->ecc = navIn.ecc;
      navOut->Ahalf = navIn.Ahalf;
      navOut->A = navIn.Ahalf * navIn.Ahalf;
         // no Adot in RINEX 3
      navOut->OMEGA0 = navIn.OMEGA0;
      navOut->i0 = navIn.i0;
      navOut->w = navIn.w;
      navOut->OMEGAdot = navIn.OMEGAdot;
      navOut->idot = navIn.idot;
      navOut->af0 = navIn.af0;
      navOut->af1 = navIn.af1;
      navOut->af2 = navIn.af2;
   }


   bool RinexNavDataFactory ::
   convertToHealth(const Rinex3NavData& navIn,
                   std::list<gpstk::NavDataPtr>& healthOut)
   {
      bool rv = true;
      gpstk::NavDataPtr health;
      GPSLNavHealth *gps;
      GalNavHealth *galNav;
      unsigned healthBits = 0;
      switch (navIn.sat.system)
      {
         case SatelliteSystem::GPS:
            health = std::make_shared<GPSLNavHealth>();
            gps = dynamic_cast<GPSLNavHealth*>(health.get());
               // NavData
            fillNavData(navIn, health);
               // GPSLNavHealth
            gps->svHealth = navIn.health;
            healthOut.push_back(health);
            break;
         case SatelliteSystem::Galileo:
               // construct three health objects, one for each signal
               // in the RINEX record.
               // E1-B first
            health = std::make_shared<GalNavHealth>();
            galNav = dynamic_cast<GalNavHealth*>(health.get());
               // NavData
            fillNavData(navIn, health);
               // GalNavHealth
               // start with original health bits from RINEX, decode and shift.
            healthBits = navIn.health;
            galNav->dataValidityStatus = static_cast<GalDataValid>(
               healthBits & 0x01);
            healthBits >>= 1;
            galNav->sigHealthStatus = static_cast<GalHealthStatus>(
               navIn.health & 0x3);
            healthBits >>= 2;
            galNav->signal.obs.band = CarrierBand::L1;
            galNav->signal.obs.code = TrackingCode::E1B;
            healthOut.push_back(health);
               // Now decode E5a
            health = std::make_shared<GalNavHealth>();
            galNav = dynamic_cast<GalNavHealth*>(health.get());
               // NavData
            fillNavData(navIn, health);
               // GalNavHealth
            galNav->dataValidityStatus = static_cast<GalDataValid>(
               healthBits & 0x01);
            healthBits >>= 1;
            galNav->sigHealthStatus = static_cast<GalHealthStatus>(
               navIn.health & 0x3);
            healthBits >>= 2;
            galNav->signal.obs.band = CarrierBand::L5;
            galNav->signal.obs.code = TrackingCode::E5aI;
            if (navIn.datasources & 0x0100)
            {
                  // The current implementation of Rinex3NavData
                  // doesn't do detailed conversion from accuracy to
                  // SISA index.
               galNav->sisaIndex = decodeSISA(navIn.accuracy);
            }
            healthOut.push_back(health);
               // Finally, decode E5b
            health = std::make_shared<GalNavHealth>();
            galNav = dynamic_cast<GalNavHealth*>(health.get());
               // NavData
            fillNavData(navIn, health);
               // GalNavHealth
            galNav->dataValidityStatus = static_cast<GalDataValid>(
               healthBits & 0x01);
            healthBits >>= 1;
            galNav->sigHealthStatus = static_cast<GalHealthStatus>(
               navIn.health & 0x3);
            healthBits >>= 2;
            galNav->signal.obs.band = CarrierBand::L5;
            galNav->signal.obs.code = TrackingCode::E5bI;
            if (navIn.datasources & 0x0200)
            {
                  // The current implementation of Rinex3NavData
                  // doesn't do detailed conversion from accuracy to
                  // SISA index.
               galNav->sisaIndex = decodeSISA(navIn.accuracy);
            }
            healthOut.push_back(health);
            break;
         default:
               /// @todo add other GNSSes
            rv = false;
            break;
      }
      return rv;
   }


   bool RinexNavDataFactory ::
   convertToOffset(const Rinex3NavHeader& navIn, NavDataPtrList& navOut)
   {
      for (const auto& mti : navIn.mapTimeCorr)
      {
         std::shared_ptr<RinexTimeOffset> rto =
            std::make_shared<RinexTimeOffset>(mti.second, navIn.leapSeconds);
            // We have no idea what the signal was, but that doesn't
            // matter for TimeOffset.
            // We use the reference time as our timeStamp because we
            // have nothing else available.
            /** @bug the time stamp is likely to cause problems for
             * GLONASS which doesn't have a reference time. */
            /** @todo add support for delta t LSF in RINEX 3
             * (leapDelta, leapWeek, leapDay in Rinex3NavHeader). */
         rto->timeStamp = mti.second.refTime;
         navOut.push_back(rto);
      }
      return true;
   }


   void RinexNavDataFactory ::
   fillNavData(const Rinex3NavData& navIn, NavDataPtr& navOut)
   {
      switch (navIn.sat.system)
      {
         case SatelliteSystem::GPS:
               // NavData
            navOut->timeStamp = GPSWeekSecond(navIn.weeknum,navIn.xmitTime);
               // sat and xmitSat are always the same for ephemeris
            navOut->signal.sat = navIn.sat;
            navOut->signal.xmitSat = navIn.sat;
            navOut->signal.system = navIn.sat.system;
               // we can't obtain these from RINEX NAV, so just assume L1 C/A
            navOut->signal.obs = ObsID(ObservationType::NavMsg, CarrierBand::L1,
                                       TrackingCode::CA);
               /// @todo Does RINEX support CNAV and if so how do we know?
            navOut->signal.nav = NavType::GPSLNAV;
            break;
         case SatelliteSystem::Galileo:
               // NavData
               // RINEX 3.04 Table A8 note 4 says the GAL week number
               // is identical to the GPS week number.  ergo, it's not
               // really a Galileo week number.
            navOut->timeStamp = GPSWeekSecond(navIn.weeknum, navIn.xmitTime);
               //navOut->timeStamp.setTimeSystem(TimeSystem::GAL);
               // sat and xmitSat are always the same for ephemeris
            navOut->signal.sat = navIn.sat;
            navOut->signal.xmitSat = navIn.sat;
            navOut->signal.system = navIn.sat.system;
               // The magic numbers here are defined in the RINEX 3
               // standard, table A8.
               /** @todo figure out how to handle the case where Bit 0
                * and 2 are both set (see table A8). */
            if (navIn.datasources & 0x01)
            {
               navOut->signal.obs = ObsID(ObservationType::NavMsg,
                                          CarrierBand::L1,
                                          TrackingCode::E1B);
               navOut->signal.nav = NavType::GalINAV;
            }
            else if (navIn.datasources & 0x02)
            {
               navOut->signal.obs = ObsID(ObservationType::NavMsg,
                                          CarrierBand::L5,
                                          TrackingCode::E5aI);
               navOut->signal.nav = NavType::GalFNAV;
            }
            else if (navIn.datasources & 0x04)
            {
               navOut->signal.obs = ObsID(ObservationType::NavMsg,
                                          CarrierBand::E5b,
                                          TrackingCode::E5bI);
               navOut->signal.nav = NavType::GalINAV;
            }
            break;
         default:
               /// @todo add other GNSSes
            break;
      }
   }


   void RinexNavDataFactory ::
   fixTimeGPS(const Rinex3NavData& navIn, GPSLNavEph& navOut)
   {
      long longToc = (long) navIn.Toc;
      long fullXmitWeekNum = navIn.weeknum;

         // Case 3 check
      long adjHOWtime = navIn.xmitTime;
      if (((longToc % SEC_PER_DAY) == 0) &&
          ((navIn.xmitTime % SEC_PER_DAY) == 0) &&
          (longToc == navIn.xmitTime))
      {
         adjHOWtime = navIn.xmitTime - 30;
         if (adjHOWtime<0)
         {
            adjHOWtime += FULLWEEK;
            fullXmitWeekNum--;
         }
      }

         // Determine Transmit Time
         // Transmit time is the actual time this
         // SF 1/2/3 sample was collected
      long xmit = adjHOWtime - (adjHOWtime % 30);
      double xmitSOW = (double) xmit;
      navOut.xmitTime = GPSWeekSecond(fullXmitWeekNum, (double)xmit,
                                      TimeSystem::GPS);

         // Fully qualified Toe and Toc
         // As broadcast, Toe and Toc are in GPS SOW and do not include
         // the GPS week number.  OrbElem (rightly) insists on having a
         // Toe and Toc in CommonTime objects which implies determining
         // the week number.
      double timeDiff = navIn.Toe - xmitSOW;
      short epochWeek = fullXmitWeekNum;
      if (timeDiff < -HALFWEEK)
      {
         epochWeek++;
      }
      else if (timeDiff > HALFWEEK)
      {
         epochWeek--;
      }

      navOut.Toc = GPSWeekSecond(epochWeek, navIn.Toc, TimeSystem::GPS);
      navOut.Toe = GPSWeekSecond(epochWeek, navIn.Toe, TimeSystem::GPS);
   }


   void RinexNavDataFactory ::
   fixTimeGalileo(const Rinex3NavData& navIn, OrbitDataKepler& navOut)
   {
         /// @todo Probably need to do half week tests on the transmit time
         // RINEX 3.04 Table A8 note 4 says the GAL week number
         // is identical to the GPS week number.  ergo, it's not
         // really a Galileo week number.
      navOut.xmitTime = GPSWeekSecond(navIn.weeknum, navIn.xmitTime);
         //navOut.xmitTime.setTimeSystem(TimeSystem::GAL);
   }


   uint8_t RinexNavDataFactory ::
   decodeSISA(double accuracy)
   {
         // accuracy = -1 (or less than zero anyway)
      if (accuracy < 0)
         return 255;
         // 0-0.49m => 0-49
      if (accuracy < 0.5)
         return (uint8_t)(accuracy*100);
         // .5m-.98m => 50-74
      if (accuracy < 0.75)
         return (uint8_t)((accuracy+0.5)*50);
         // 1m-1.96m => 75-99
      if (accuracy < 2)
         return (uint8_t)((accuracy+2)*25);
         // 2m-5.84m => 100-125
      if (accuracy < 6)
         return (uint8_t)(100+6.51046666666666*(accuracy-2));
      return 255;
   }
}
