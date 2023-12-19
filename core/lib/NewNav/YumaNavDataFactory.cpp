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
#include "YumaNavDataFactory.hpp"
#include "YumaStream.hpp"
#include "YumaHeader.hpp"
#include "GPSLNavHealth.hpp"
#include "GPSWeekSecond.hpp"
#include "NavDataFactoryStoreCallback.hpp"

using namespace std;

namespace gnsstk
{
   YumaNavDataFactory ::
   YumaNavDataFactory()
   {
      supportedSignals.insert(NavSignalID(SatelliteSystem::GPS,
                                          CarrierBand::L1,
                                          TrackingCode::CA,
                                          NavType::GPSLNAV));
   }


   bool YumaNavDataFactory ::
   loadIntoMap(const std::string& filename, NavMessageMap& navMap,
               NavNearMessageMap& navNearMap, OffsetCvtMap& ofsMap)
   {
      gnsstk::NavDataFactoryStoreCallback cb(this, navMap, navNearMap, ofsMap);
      return process(filename, cb);
   }


   bool YumaNavDataFactory ::
   process(const std::string& filename,
           NavDataFactoryCallback& cb)
   {
      bool rv = true;
      bool processAlm = (procNavTypes.count(NavMessageType::Almanac) > 0);
      bool processHea = (procNavTypes.count(NavMessageType::Health) > 0);
         /** @bug The use of gotdata is a kludge.  It appears that the
          * YumaStream implementation will read invalid data to the
          * end of the file, and we need it to not do that. */
      bool gotdata = false;
      try
      {
         YumaStream is(filename.c_str(), ios::in);
         YumaData data;
         if (!is)
            return false;
         while (is)
         {
            is >> data;
            if (!is)
            {
               if (is.eof() && gotdata)
                  break;
               else
                  return false; // some other error
            }
            gotdata = true;
            NavDataPtr alm, health;
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


   std::string YumaNavDataFactory ::
   getFactoryFormats() const
   {
      if (procNavTypes.empty() ||
          (procNavTypes.count(NavMessageType::Almanac) > 0) ||
          (procNavTypes.count(NavMessageType::Health) > 0))
      {
         return "Yuma";
      }
      return "";
   }


   bool YumaNavDataFactory ::
   convertToOrbit(const YumaData& navIn, NavDataPtr& navOut)
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
         // searches will work.  YumaData has a xmit_time field
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
      gps->dndot = 0.0; // no dndot in Yuma or in GPS LNav
      gps->ecc = navIn.ecc;
      gps->Ahalf = navIn.Ahalf;
      gps->A = navIn.Ahalf * navIn.Ahalf;
      gps->Adot = 0.0; // no Adot in Yuma or in GPS LNav
      gps->OMEGA0 = navIn.OMEGA0;
      gps->i0 = navIn.i_total;
      gps->w = navIn.w;
      gps->OMEGAdot = navIn.OMEGAdot;
      gps->idot = 0;
      gps->af0 = navIn.AF0;
      gps->af1 = navIn.AF1;
      gps->af2 = 0.0;
      gps->tlm = 0; // tlm not available in Yuma nav
      return rv;
   }


   bool YumaNavDataFactory ::
   convertToHealth(const YumaData& navIn, NavDataPtr& healthOut)
   {
      bool rv = true;
      GPSLNavHealth *gps;
      healthOut = std::make_shared<GPSLNavHealth>();
      gps = dynamic_cast<GPSLNavHealth*>(healthOut.get());
         // NavData
      fillNavData(navIn, healthOut);
         // manipulate a time stamp to mirror convertToOrbit
      gps->timeStamp = GPSWeekSecond(navIn.week, navIn.Toa);
      gps->timeStamp -= (70 * 3600.0);
         // GPSLNavHealth
      gps->svHealth = navIn.SV_health;
      return rv;
   }


   void YumaNavDataFactory ::
   fillNavData(const YumaData& navIn, NavDataPtr& navOut)
   {
         // NavData
         // Yuma isn't really transmitted, so we set the sats the same
      navOut->signal.sat = SatID(navIn.PRN,SatelliteSystem::GPS);
      navOut->signal.xmitSat = SatID(navIn.PRN,SatelliteSystem::GPS);
      navOut->signal.system = SatelliteSystem::GPS;
         // we can't obtain these from Yuma nav, so just assume L1 C/A
      navOut->signal.obs = ObsID(ObservationType::NavMsg, CarrierBand::L1,
                                 TrackingCode::CA);
      navOut->signal.nav = NavType::GPSLNAV;
   }
}
