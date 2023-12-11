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
#include "SEMNavDataFactory.hpp"

#include <memory>

#include "GPSLNavHealth.hpp"
#include "GPSNavConfig.hpp"
#include "GPSWeekSecond.hpp"
#include "NavDataFactoryStoreCallback.hpp"
#include "NavMessageType.hpp"
#include "SatelliteSystem.hpp"
#include "SEMHeader.hpp"
#include "SEMStream.hpp"

using namespace std;

namespace gnsstk
{
   SEMNavDataFactory ::
   SEMNavDataFactory()
   {
      supportedSignals.insert(NavSignalID(SatelliteSystem::GPS,
                                          CarrierBand::L1,
                                          TrackingCode::CA,
                                          NavType::GPSLNAV));
   }


   bool SEMNavDataFactory ::
   loadIntoMap(const std::string& filename, NavMessageMap& navMap,
               NavNearMessageMap& navNearMap, OffsetCvtMap& ofsMap)
   {
      gnsstk::NavDataFactoryStoreCallback cb(this, navMap, navNearMap, ofsMap);
      return process(filename, cb);
   }


   bool SEMNavDataFactory ::
   process(const std::string& filename,
           NavDataFactoryCallback& cb)
   {
      bool rv = true;
      bool processAlm = (procNavTypes.count(NavMessageType::Almanac) > 0);
      bool processHea = (procNavTypes.count(NavMessageType::Health) > 0);
      bool processSys{procNavTypes.count(NavMessageType::System) > 0};
      try
      {
         SEMStream is(filename.c_str(), ios::in);
         SEMHeader head;
         SEMData data;
         if (!is)
            return false;
         is >> head;
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
            NavDataPtr alm, health, sys;
            if (processAlm)
            {
               if (!convertToOrbit(data, alm))
                  return false;
            }
            if (processHea)
            {
               if (!convertToHealth(data, health))
                  return false;
            }
            if (processSys)
            {
               if (!convertToSystem(data, sys))
               {
                  return false;
               }
            }
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
            if (check)
            {
               if (processAlm)
               {
                  if (alm->validate() == expect)
                  {
                     if (!cb.process(alm))
                        return false;
                  }
               }
               if (processHea)
               {
                  if (health->validate() == expect)
                  {
                     if (!cb.process(health))
                        return false;
                  }
               }
               if (processSys)
               {
                  if (sys->validate() == expect)
                  {
                     if (!cb.process(sys))
                     {
                        return false;
                     }
                  }
               }
            }
            else
            {
               if (processAlm)
               {
                  if (!cb.process(alm))
                     return false;
               }
               if (processHea)
               {
                  if (!cb.process(health))
                     return false;
               }
               if (processSys)
               {
                  if (!cb.process(sys))
                  {
                    return false;
                  }
               }
            }
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


   std::string SEMNavDataFactory ::
   getFactoryFormats() const
   {
      if (procNavTypes.empty() ||
          (procNavTypes.count(NavMessageType::Almanac) > 0) ||
          (procNavTypes.count(NavMessageType::Health) > 0) ||
          (procNavTypes.count(NavMessageType::System) > 0))
      {
         return "SEM";
      }
      return "";
   }


   bool SEMNavDataFactory ::
   convertToOrbit(const SEMData& navIn, NavDataPtr& navOut)
   {
      bool rv = true;
      GPSLNavAlm *gps;
      navOut = std::make_shared<GPSLNavAlm>();
      gps = dynamic_cast<GPSLNavAlm*>(navOut.get());
         // NavData
      fillNavData(navIn, navOut);
         // OrbitDataKepler
      gps->Toe = GPSWeekSecond(navIn.week, navIn.Toa);
         // Toc, which is used to generate the clock corrections
         // using af0 and af1, is the same as Toa in the almanac
         // context.
      gps->Toc = gps->Toe;
      gps->fixFit();
         // Transmit time is set to beginFit so that User-type
         // searches will work.  SEMData has a xmit_time field
         // but it's always set to 0.  Not sure why it's even
         // there.
      gps->xmitTime = gps->beginFit;
      gps->timeStamp = gps->xmitTime;
      gps->health = ((navIn.SV_health == 0) ? SVHealth::Healthy :
                     SVHealth::Unhealthy);
         // not in almanac data.
      gps->Cuc = 0.0;
      gps->Cus = 0.0;
      gps->Crc = 0.0;
      gps->Crs = 0.0;
      gps->Cic = 0.0;
      gps->Cis = 0.0;
      gps->M0 = navIn.M0;
      gps->dn = 0.0;
      gps->dndot = 0.0; // no dndot in SEM or in GPS LNav
      gps->ecc = navIn.ecc;
      gps->Ahalf = navIn.Ahalf;
      gps->A = navIn.Ahalf * navIn.Ahalf;
      gps->Adot = 0.0; // no Adot in SEM or in GPS LNav
      gps->OMEGA0 = navIn.OMEGA0;
      gps->i0 = navIn.i_total;
      gps->w = navIn.w;
      gps->OMEGAdot = navIn.OMEGAdot;
      gps->idot = 0;
      gps->af0 = navIn.AF0;
      gps->af1 = navIn.AF1;
      gps->af2 = 0.0;
      gps->tlm = 0; // tlm not available in SEM nav
      return rv;
   }


   bool SEMNavDataFactory ::
   convertToHealth(const SEMData& navIn, NavDataPtr& healthOut)
   {
      bool rv = true;
      GPSLNavHealth *gps;
      healthOut = std::make_shared<GPSLNavHealth>();
      gps = dynamic_cast<GPSLNavHealth*>(healthOut.get());
         // NavData
      fillNavData(navIn, healthOut);
         // this is the only timestamp we have from SEM
      gps->timeStamp = GPSWeekSecond(navIn.week, navIn.Toa);
      // Set the fit times to toa-70h through toa+74 hours.  This
      // is an estimate based on IS-GPS-200 Table 20-XIII.
      gps->timeStamp -= (70 * 3600.0);

         // GPSLNavHealth
      gps->svHealth = navIn.SV_health;
      return rv;
   }


   bool SEMNavDataFactory ::
   convertToSystem(const SEMData &navIn, NavDataPtr &systemOut)
   {
     systemOut = std::make_shared<GPSNavConfig>();
     fillNavData(navIn, systemOut);

     // Dynamically cast to a GPSNavConfig pointer.
     auto configOut{std::dynamic_pointer_cast<GPSNavConfig>(systemOut)};
     // The only available timestamp in SEM.
     configOut->timeStamp = GPSWeekSecond(navIn.week, navIn.Toa);
     // Antispoof is the MSB of the four valid bits in the config word.
     configOut->antispoofOn = (navIn.satConfig & 0x8) != 0;
     // SV config is the 3 LSBs of the four valid bits in the config word.
     configOut->svConfig = navIn.satConfig & 0x7;

     return true;
   }


   void SEMNavDataFactory ::
   fillNavData(const SEMData& navIn, NavDataPtr& navOut)
   {
         // NavData
         // SEM isn't really transmitted, so we set the sats the same
      navOut->signal.sat = SatID(navIn.PRN,SatelliteSystem::GPS);
      navOut->signal.system = SatelliteSystem::GPS;
      navOut->signal.xmitSat = SatID(navIn.PRN,SatelliteSystem::GPS);
         // we can't obtain these from SEM nav, so just assume L1 C/A
      navOut->signal.obs = ObsID(ObservationType::NavMsg, CarrierBand::L1,
                                 TrackingCode::CA);
      navOut->signal.nav = NavType::GPSLNAV;
   }
}
