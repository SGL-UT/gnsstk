#include "SEMNavDataFactory.hpp"
#include "SEMStream.hpp"
#include "SEMHeader.hpp"
#include "GPSLNavHealth.hpp"

using namespace std;

namespace gpstk
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
   loadIntoMap(const std::string& filename, NavMessageMap& navMap)
   {
      bool rv = true;
      bool processAlm = (procNavTypes.count(NavMessageType::Almanac) > 0);
      bool processHea = (procNavTypes.count(NavMessageType::Health) > 0);
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
                     if (!addNavData(alm))
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
               if (processAlm)
               {
                  if (!addNavData(alm))
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
      gps->healthy = (navIn.SV_health == 0);
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
      gps->idot = navIn.i_offset;
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
         // GPSLNavHealth
      gps->svHealth = navIn.SV_health;
      return rv;
   }


   void SEMNavDataFactory ::
   fillNavData(const SEMData& navIn, NavDataPtr& navOut)
   {
         // NavData
         // SEM isn't really transmitted, so we set the sats the same
      navOut->signal.sat = WildSatID(navIn.PRN,SatelliteSystem::GPS);
      navOut->signal.xmitSat = WildSatID(navIn.PRN,SatelliteSystem::GPS);
      navOut->signal.system = SatelliteSystem::GPS;
         // we can't obtain these from SEM nav, so just assume L1 C/A
      navOut->signal.carrier = CarrierBand::L1;
      navOut->signal.code = TrackingCode::CA;
      navOut->signal.nav = NavType::GPSLNAV;
   }
}
