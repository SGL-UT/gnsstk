#include "SP3NavDataFactory.hpp"
#include "SP3Stream.hpp"
#include "SP3Header.hpp"
#include "SP3Data.hpp"
#include "OrbitDataSP3.hpp"
#include "TimeString.hpp"

using namespace std;

/// @todo figure out what's a legitimate delta t to allow interpolation
static const double limitDT = 900.0;

namespace gpstk
{
   SP3NavDataFactory ::
   SP3NavDataFactory()
   {
      supportedSignals.insert(NavSignalID(SatelliteSystem::GPS,
                                          CarrierBand::L1,
                                          TrackingCode::CA,
                                          NavType::GPSLNAV));
   }


   bool SP3NavDataFactory ::
   find(const NavMessageID& nmid, const CommonTime& when,
        NavDataPtr& navData, NavValidityType valid,
        NavSearchOrder order)
   {
         // dig through the maps of maps, matching keys with nmid along the way
      if (data.find(nmid.messageType) == data.end())
      {
         std::cerr << " false = not found 1" << std::endl;
         return false; // not found.
      }
      NavSignalMap &nsigm(data[nmid.messageType]);
      if (nsigm.find(nmid) == nsigm.end())
      {
         std::cerr << " false = not found 2" << std::endl;
         return false; // not found.
      }
      NavSatMap &nsm(nsigm[nmid]);
      if (nsm.find(nmid) == nsm.end())
      {
         std::cerr << " false = not found 3" << std::endl;
         return false; // not found.
      }
      NavMap &nm(nsm[nmid]);
         // This is not the entry we want, but it is instead the first
         // entry we probably (depending on order) *don't* want.
      auto ti2 = nm.lower_bound(when);
      if (order == NavSearchOrder::User)
      {
            // The map goes by transmit time so we do what should be a
            // quick linear search to find the best by user time
            // (i.e. when the entire message will have been received).
            // If ti2 starts at the end of the map, that doesn't mean
            // we've failed.  We should still try and back up at least
            // once.
         if (ti2 == nm.end())
         {
            ti2 = std::prev(ti2);
         }
         std::cerr << " when=" << printTime(when,"%Y/%03j/%02H:%02M:%02S")
                   << std::endl;
         while (((ti2 != nm.end()) && (ti2->second->getUserTime() > when)) ||
                !validityCheck(ti2, nm, valid))
         {
            if (ti2 == nm.begin())
            {
               std::cerr << "already at the beginning" << std::endl;
            }
            std::cerr << "  ti2->second->getUserTime()="
                      << printTime(ti2->second->getUserTime(),
                                   "%Y/%03j/%02H:%02M:%02S") << std::endl;
               // don't use operator--, it won't give the right results
            ti2 = std::prev(ti2);
            if (ti2 == nm.end())
            {
               std::cerr << "reached the end" << std::endl;
            }
         }
         if (ti2 == nm.end())
         {
               // no good
            std::cerr << " false = not good 4" << std::endl;
            return false;
         }
         std::cerr << "  ti2->second->getUserTime()="
                   << printTime(ti2->second->getUserTime(),
                                "%Y/%03j/%02H:%02M:%02S") << std::endl;
         if (fabs(when - ti2->second->getUserTime()) > limitDT)
         {
            std::cerr << " false = not good time 5" << std::endl;
            return false;
         }
         // ti2->second->dump(std::cerr,NavData::Detail::Full);
            // good.
         navData = ti2->second;
         return true;
      }
      else if (order == NavSearchOrder::Nearest)
      {
      }
      std::cerr << " false final" << std::endl;
      return false;
   }


   bool SP3NavDataFactory ::
   loadIntoMap(const std::string& filename, NavMessageMap& navMap)
   {
      bool rv = true;
      bool processEph = (procNavTypes.count(NavMessageType::Ephemeris) > 0);
         // When either of these two change, we store the existing
         // NavDataPtr and create a new one.
      CommonTime lastTime;
      SatID lastSat;
      NavDataPtr eph;
      try
      {
         SP3Stream is(filename.c_str(), ios::in);
         SP3Header head;
         SP3Data data;
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
            if ((lastSat != data.sat) || (lastTime != data.time))
            {
               lastSat = data.sat;
               lastTime = data.time;
               if (!store(processEph, eph))
                  return false;
            }
               // Don't process time records otherwise we'll end up
               // storing junk in the store that has a time stamp and
               // a bogus satellite ID.
            if (processEph && (data.RecType != '*'))
            {
               if (!convertToOrbit(data, eph))
                  return false;
            }
         }
            // store the final record(s)
         if (!store(processEph, eph))
            return false;
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


   bool SP3NavDataFactory ::
   convertToOrbit(const SP3Data& navIn, NavDataPtr& navOut)
   {
      bool rv = true;
      OrbitDataSP3 *gps;
         // SP3 needs to merge multiple records, position and
         // velocity, so we only create new objects as needed.
      if (!navOut)
      {
         navOut = std::make_shared<OrbitDataSP3>();
      }
      gps = dynamic_cast<OrbitDataSP3*>(navOut.get());
      switch (navIn.RecType)
      {
         case 'P':
            gps->timeStamp = navIn.time;
               // SP3Data says x is in m for position, but it's really
               // in km, so we scale it to m
            gps->pos[0] = 1000.0 * navIn.x[0];
            gps->pos[1] = 1000.0 * navIn.x[1];
            gps->pos[2] = 1000.0 * navIn.x[2];
            gps->signal.sat = navIn.sat;
            gps->signal.xmitSat = navIn.sat;
            gps->signal.system = navIn.sat.system;
               // we can't obtain these from RINEX NAV, so just assume L1 C/A
            navOut->signal.carrier = CarrierBand::L1;
            navOut->signal.code = TrackingCode::CA;
               /// @todo determine what should really be here for SP3 data.
            navOut->signal.nav = NavType::GPSLNAV;
            gps->clkBias = navIn.clk;
            break;
         case 'V':
            gps->timeStamp = navIn.time;
               // SP3Data says x is in dm/s for velocity so we scale it to m/s
            gps->vel[0] = 10.0 * navIn.x[0];
            gps->vel[1] = 10.0 * navIn.x[1];
            gps->vel[2] = 10.0 * navIn.x[2];
            gps->clkDrift = navIn.clk;
            break;
      }
      return rv;
   }


   bool SP3NavDataFactory ::
   store(bool processEph, NavDataPtr& eph)
   {
         // only process if we have something to process.
      if (eph)
      {
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
            if (processEph)
            {
               if (eph->validate() == expect)
               {
                  if (!addNavData(eph))
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
         }
            // Clear the shared_ptr so the next time
            // convertToOrbit is called, it creates a new one.
         eph.reset();
      }
      return true;
   }
} // namespace gpstk
