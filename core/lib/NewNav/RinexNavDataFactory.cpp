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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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
#include "RinexTimeOffset.hpp"

using namespace std;

namespace gpstk
{
   RinexNavDataFactory ::
   RinexNavDataFactory()
   {
      supportedSignals.insert(NavSignalID(SatelliteSystem::GPS,
                                          CarrierBand::L1,
                                          TrackingCode::CA,
                                          NavType::GPSLNAV));
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
            NavDataPtr eph, health;
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
                  if (health->validate() == expect)
                  {
                     if (!addNavData(health))
                        return false;
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
                  if (!addNavData(health))
                     return false;
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
      switch (navIn.sat.system)
      {
         case SatelliteSystem::GPS:
            navOut = std::make_shared<GPSLNavEph>();
            gps = dynamic_cast<GPSLNavEph*>(navOut.get());
               // NavData
            fillNavData(navIn, navOut);
               // OrbitDataKepler
            fixTimeGPS(navIn, *gps);

            gps->Toc = navIn.time;
            gps->health = ((navIn.health == 0) ? SVHealth::Healthy :
                           SVHealth::Unhealthy);
            gps->Cuc = navIn.Cuc;
            gps->Cus = navIn.Cus;
            gps->Crc = navIn.Crc;
            gps->Crs = navIn.Crs;
            gps->Cic = navIn.Cic;
            gps->Cis = navIn.Cis;
            gps->M0 = navIn.M0;
            gps->dn = navIn.dn;
               // no dndot in RINEX 3 or in GPS LNav
            gps->ecc = navIn.ecc;
            gps->Ahalf = navIn.Ahalf;
            gps->A = navIn.Ahalf * navIn.Ahalf;
               // no Adot in RINEX 3 or in GPS LNav
            gps->OMEGA0 = navIn.OMEGA0;
            gps->i0 = navIn.i0;
            gps->w = navIn.w;
            gps->OMEGAdot = navIn.OMEGAdot;
            gps->idot = navIn.idot;
            gps->af0 = navIn.af0;
            gps->af1 = navIn.af1;
            gps->af2 = navIn.af2;
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
         default:
               /// @todo add other GNSSes
            rv = false;
            break;
      }
      return rv;
   }


   bool RinexNavDataFactory ::
   convertToHealth(const Rinex3NavData& navIn, NavDataPtr& healthOut)
   {
      bool rv = true;
      GPSLNavHealth *gps;
      switch (navIn.sat.system)
      {
         case SatelliteSystem::GPS:
            healthOut = std::make_shared<GPSLNavHealth>();
            gps = dynamic_cast<GPSLNavHealth*>(healthOut.get());
               // NavData
            fillNavData(navIn, healthOut);
               // GPSLNavHealth
            gps->svHealth = navIn.health;
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
            navOut->timeStamp =
               gpstk::GPSWeekSecond(navIn.weeknum,navIn.xmitTime);
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
}
