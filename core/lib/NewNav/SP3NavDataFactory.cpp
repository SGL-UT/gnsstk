#include <iterator>
#include "SP3NavDataFactory.hpp"
#include "SP3Stream.hpp"
#include "SP3Header.hpp"
#include "SP3Data.hpp"
#include "OrbitDataSP3.hpp"
#include "TimeString.hpp"
#include "MiscMath.hpp"

using namespace std;

/// @todo figure out what's a legitimate delta t to allow interpolation
static const double limitDT = 900.0;

namespace gpstk
{
   SP3NavDataFactory ::
   SP3NavDataFactory()
         : storeTimeSystem(TimeSystem::Any),
           halfOrder(5)
   {
      supportedSignals.insert(NavSignalID(SatelliteSystem::GPS,
                                          CarrierBand::L1,
                                          TrackingCode::CA,
                                          NavType::GPSLNAV));
   }


   bool SP3NavDataFactory ::
   find(const NavMessageID& nmid, const CommonTime& when,
        NavDataPtr& navData, SVHealth xmitHealth, NavValidityType valid,
        NavSearchOrder order)
   {
      bool rv;
      if (nmid.messageType != NavMessageType::Ephemeris)
         return false;
      rv = findGeneric(NavMessageType::Ephemeris, nmid, when, navData);
      if (rv == false)
         return false;
      rv = findGeneric(NavMessageType::Clock, nmid, when, navData);
      if (rv == false)
         return false;
      return true;
   }


   bool SP3NavDataFactory ::
   findGeneric(NavMessageType nmt, const NavSatelliteID& nsid,
               const CommonTime& when, NavDataPtr& navData)
   {
      cerr << __PRETTY_FUNCTION__ << " nmt=" << StringUtils::asString(nmt)
           << endl;
      bool giveUp = false;
      auto dataIt = data.find(nmt);
      if (dataIt == data.end())
      {
         cerr << "  no data for nav message type" << endl;
         return false;
      }
         // To support wildcard signals, we need to do a linear search.
      for (auto& sati : dataIt->second)
      {
         if (sati.first != nsid)
            continue; // skip non matches
            // This is not the entry we want, but it is instead the first
            // entry we probably (depending on order) *don't* want.
         auto ti2 = sati.second.upper_bound(when);
         auto ti1 = ti2, ti3 = ti2;
         if (ti2 == sati.second.end())
         {
               // Since we're at the end we can't do interpolation,
               // but we can still check for an exact match.
            cerr << "  probably giving up because we're at the end" << endl;
            giveUp = true;
         }
         else if (((ti1 = std::prev(ti2,halfOrder)) == sati.second.end()) ||
                  ((ti3 = std::next(ti2,halfOrder)) == sati.second.end()))
         {
               // We're on the edge of available and can't interpolate.
            cerr << "  probably giving up because we're near the bound" << endl;
            giveUp = true;
         }
            // always back up one which allows us to check for exact match.
         ti2 = std::prev(ti2);
         if (ti2->second->timeStamp == when)
         {
               // Even though it's an exact match, we still need to
               // make a new object so that we can fill in clock
               // information without affecting the internal store.
            if (!navData)
            {
               OrbitDataSP3 *stored = dynamic_cast<OrbitDataSP3*>(
                  ti2->second.get());
               navData = std::make_shared<OrbitDataSP3>(*stored);
               ti2->second->dump(std::cerr, NavData::Detail::Full);
               navData->dump(std::cerr, NavData::Detail::Full);
               cerr << "  found an exact match" << endl;
               return true;
            }
            else
            {
               OrbitDataSP3 *stored = dynamic_cast<OrbitDataSP3*>(
                  ti2->second.get());
               OrbitDataSP3 *navOut = dynamic_cast<OrbitDataSP3*>(
                  navData.get());
               if (nmt == NavMessageType::Ephemeris)
               {
                  navOut->copyXV(*stored);
               }
               else if (nmt == NavMessageType::Clock)
               {
                  navOut->copyT(*stored);
               }
               stored->dump(std::cerr, NavData::Detail::Full);
               navOut->dump(std::cerr, NavData::Detail::Full);
               cerr << "  found an exact match with existing data" << endl;
               return true;
            }
         }
         else if (giveUp)
         {
               // not an exact match and no data available for interpolation.
            cerr << "  giving up, insufficient data for interpolation" << endl;
            return false;
         }
         else
         {
            cerr << "  faking interpolation" << endl;
            if (!navData)
            {
               cerr << "  creating new empty navData" << endl;
               OrbitDataSP3 *stored = dynamic_cast<OrbitDataSP3*>(
                  ti2->second.get());
               navData = std::make_shared<OrbitDataSP3>(*stored);
               navData->timeStamp = when;
            }
            else
            {
               cerr << "  already have valid navData" << endl;
            }
            if (nmt == gpstk::NavMessageType::Ephemeris)
            {
               interpolateEph(ti1, ti3, when, navData);
               return true;
            }
            else if (nmt == gpstk::NavMessageType::Clock)
            {
               interpolateClk(ti1, ti3, when, navData);
               return true;
            }
         } // else (do interpolation)
      } // for (auto& sati : dataIt->second)
      cerr << "  giving up at the end" << endl;
      return false;
   }


   bool SP3NavDataFactory ::
   addDataSource(const std::string& source)
   {
      bool rv = true;
      bool processEph = (procNavTypes.count(NavMessageType::Ephemeris) > 0);
      bool processClk = (procNavTypes.count(NavMessageType::Clock) > 0);
         // When either of these two change, we store the existing
         // NavDataPtr and create a new one.
      CommonTime lastTime;
      SatID lastSat;
      NavDataPtr eph, clk;
      try
      {
         SP3Stream is(source.c_str(), ios::in);
         SP3Header head;
         SP3Data data;
         if (!is)
            return false;
         is >> head;
         if (!is)
            return false;

            // know whether to look for the extra info contained in SP3c
         bool isC = (head.version==SP3Header::SP3c);
            // check/save TimeSystem to storeTimeSystem
         if ((head.timeSystem != TimeSystem::Any) &&
             (head.timeSystem != TimeSystem::Unknown))
         {
               // if store time system has not been set, do so
            if (storeTimeSystem == TimeSystem::Any)
            {
                  /// @note store TimeSystem must be consistent.
               storeTimeSystem = head.timeSystem;
            }
            else if (storeTimeSystem != head.timeSystem)
            {
                  // Don't load an SP3 file with a differing time system
               return false;
            }
         }

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
               // cerr << "time or satellite change, storing" << endl;
               lastSat = data.sat;
               lastTime = data.time;
               cerr << "storing eph" << endl;
               if (!store(processEph, eph))
                  return false;
               cerr << "storing clk" << endl;
               if (!store(processClk, clk))
                  return false;
            }
               // Don't process time records otherwise we'll end up
               // storing junk in the store that has a time stamp and
               // a bogus satellite ID.
            if (data.RecType != '*')
            {
               if (processEph)
               {
                  if (!convertToOrbit(head, data, isC, eph))
                     return false;
               }
               if (processClk)
               {
                  if (!convertToClock(head, data, isC, clk))
                     return false;
               }
            }
         }
            // store the final record(s)
         cerr << "storing last eph" << endl;
         if (!store(processEph, eph))
            return false;
         cerr << "storing last clk" << endl;
         if (!store(processClk, clk))
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
   convertToOrbit(const SP3Header& head, const SP3Data& navIn, bool isC,
                  NavDataPtr& navOut)
   {
      // cerr << __PRETTY_FUNCTION__ << endl;
      OrbitDataSP3 *gps;
         // SP3 needs to merge multiple records, position and
         // velocity, so we only create new objects as needed.
      if (!navOut)
      {
         // cerr << "  creating OrbitDataSP3" << endl;
         navOut = std::make_shared<OrbitDataSP3>();
      }
      gps = dynamic_cast<OrbitDataSP3*>(navOut.get());
      // cerr << "  navIn.RecType=" << navIn.RecType << endl
      //      << "  navIn.correlationFlag=" << navIn.correlationFlag << endl;
      switch (navIn.RecType)
      {
         case 'P':
            gps->timeStamp = navIn.time;
               // SP3Data says x is in m for position, but it's really
               // in km, so we scale it to m
            for (unsigned i = 0; i < 3; i++)
            {
               if (navIn.correlationFlag)
               {
                  gps->posSig[i] = navIn.sdev[i];
               }
               else
               {
                  gps->pos[i] = 1000.0 * navIn.x[i];
                  if (isC && (navIn.sig[i] >= 0))
                  {
                     gps->posSig[i] = ::pow(head.basePV, navIn.sig[i]);
                  }
               }
            }
            gps->signal.sat = navIn.sat;
            gps->signal.xmitSat = navIn.sat;
            gps->signal.system = navIn.sat.system;
               // we can't obtain these from SP3, can we? So just assume L1 C/A
            navOut->signal.carrier = CarrierBand::L1;
            navOut->signal.code = TrackingCode::CA;
               /// @todo determine what should really be here for SP3 data.
            navOut->signal.nav = NavType::GPSLNAV;
            break;
         case 'V':
            gps->timeStamp = navIn.time;
               // SP3Data says x is in dm/s for velocity so we scale it to m/s
            for (unsigned i = 0; i < 3; i++)
            {
               if (navIn.correlationFlag)
               {
                  gps->velSig[i] = navIn.sdev[i];
               }
               else
               {
                     // Yes, x.  Because SP3Data stores position and
                     // velocity in separate records and the data from
                     // both goes into x.
                  gps->vel[i] = 10.0 * navIn.x[i];
                  if (isC && (navIn.sig[i] >= 0))
                  {
                     gps->velSig[i] = ::pow(head.basePV, navIn.sig[i]);
                  }
               }
            }
            break;
      }
      return true;
   }


   bool SP3NavDataFactory ::
   convertToClock(const SP3Header& head, const SP3Data& navIn, bool isC,
                  NavDataPtr& clkOut)
   {
      bool rv = true;
      OrbitDataSP3 *gps;
         // SP3 needs to merge multiple records, position and
         // velocity, so we only create new objects as needed.
      if (!clkOut)
      {
         clkOut = std::make_shared<OrbitDataSP3>();
            // Force the message type to clock because OrbitDataSP3
            // defaults to Ephemeris.
         clkOut->signal.messageType = NavMessageType::Clock;
      }
      gps = dynamic_cast<OrbitDataSP3*>(clkOut.get());
      switch (navIn.RecType)
      {
         case 'P':
            gps->timeStamp = navIn.time;
            if (navIn.correlationFlag)
            {
               gps->biasSig = navIn.sdev[3] * 1e-6;
            }
            else
            {
               gps->clkBias = navIn.clk;
               if (isC && (navIn.sig[3] >= 0))
               {
                  gps->biasSig = ::pow(head.baseClk, navIn.sig[3]) * 1e-6;
               }
            }
            gps->signal.sat = navIn.sat;
            gps->signal.xmitSat = navIn.sat;
            gps->signal.system = navIn.sat.system;
               // we can't obtain these from SP3, can we? So just assume L1 C/A
            clkOut->signal.carrier = CarrierBand::L1;
            clkOut->signal.code = TrackingCode::CA;
               /// @todo determine what should really be here for SP3 data.
            clkOut->signal.nav = NavType::GPSLNAV;
            break;
         case 'V':
            gps->timeStamp = navIn.time;
               // SP3Data says x is in dm/s for velocity so we scale it to m/s
            if (navIn.correlationFlag)
            {
               gps->driftSig = navIn.sdev[3] * 1e-10;
            }
            else
            {
               gps->clkDrift = navIn.clk;
               if (isC && (navIn.sig[3] >= 0))
               {
                  gps->driftSig = ::pow(head.baseClk, navIn.sig[3]) * 1e-10;
               }
            }
            break;
      }
      return rv;
   }


   bool SP3NavDataFactory ::
   store(bool process, NavDataPtr& obj)
   {
      cerr << __PRETTY_FUNCTION__ << endl;
         // only process if we have something to process.
      if (obj)
      {
         cerr << "  store storing " << obj.get() << endl;
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
            if (process)
            {
               if (obj->validate() == expect)
               {
                  if (!addNavData(obj))
                  {
                     cerr << "  store failed to add nav data" << endl;
                     return false;
                  }
               }
            }
         }
         else
         {
            if (process)
            {
               if (!addNavData(obj))
               {
                  cerr << "  store failed to add nav data" << endl;
                  return false;
               }
            }
         }
            // Clear the shared_ptr so the next time
            // convertToOrbit is called, it creates a new one.
         cerr << "  store resetting obj ptr, use_count=" << obj.use_count() << endl;
         NavData *ptr = obj.get();
         cerr << "DUMP BEFORE:" << endl;
         ptr->dump(cerr, NavData::Detail::Full);
         obj.reset();
         cerr << "DUMP AFTER:" << endl;
         ptr->dump(cerr, NavData::Detail::Full);
      }
      return true;
   }


   void SP3NavDataFactory ::
   interpolateEph(const NavMap::iterator& ti1, const NavMap::iterator& ti3,
                  const CommonTime& when, NavDataPtr& navData)
   {
      cerr << "  start interpolating ephemeris, distance = " << std::distance(ti1,ti3) << endl;
      std::vector<double> tdata(2*halfOrder);
      std::vector<std::vector<double>> posData(3), velData(3),
         accData(3);
      CommonTime firstTime(ti1->second->timeStamp);
      unsigned idx = 0;
         // posData etc are 2D arrays, where the first dimension
         // is positional, x=0,y=1,z=2 and the 2nd dimension is
         // the data index for the fit.
      for (unsigned i = 0; i < 3; i++)
      {
         posData[i].resize(2*halfOrder);
         velData[i].resize(2*halfOrder);
         accData[i].resize(2*halfOrder);
      }
      cerr << "  resized" << endl;
      bool haveVel = false, haveAcc = false;
      NavMap::iterator ti2;
      for (ti2 = ti1, idx=0; ti2 != ti3; ++ti2, ++idx)
      {
         cerr << "  idx=" << idx << endl;
         tdata[idx] = ti2->second->timeStamp - firstTime;
         OrbitDataSP3 *nav = dynamic_cast<OrbitDataSP3*>(
            ti2->second.get());
         cerr << "  nav=" << nav << endl;
         ti2->second->dump(cerr, NavData::Detail::Full);
         for (unsigned i = 0; i < 3; i++)
         {
            cerr << "  i=" << i << endl
                 << posData.size() << " " << velData.size() << " "
                 << accData.size() << endl
                 << nav->pos.size() << " " << nav->vel.size() << " "
                 << nav->acc.size() << endl;
            posData[i][idx] = nav->pos[i];
            velData[i][idx] = nav->vel[i];
            accData[i][idx] = nav->acc[i];
            haveVel |= (nav->vel[i] != 0.0);
            haveAcc |= (nav->acc[i] != 0.0);
         }
      }
      double dt = when - firstTime, err;
      OrbitDataSP3 *osp3 = dynamic_cast<OrbitDataSP3*>(navData.get());
      cerr << setprecision(20) << "  dt=" << dt << endl;
      for (unsigned i = 0; i < tdata.size(); i++)
      {
         cerr << "  i=" << i << " times=" << tdata[i] << endl
              << "  P=" << posData[0][i] << " " << posData[1][i] << " "
              << posData[2][i] << endl
              << "  V=" << velData[0][i] << " " << velData[1][i] << " "
              << velData[2][i] << endl
              << "  A=" << accData[0][i] << " " << accData[1][i] << " "
              << accData[2][i] << endl;
      }
      cerr << "  haveVelocity=" << haveVel << "  haveAcceleration="
           << haveAcc << endl;
         // Interpolate XYZ position/velocity/acceleration.
      for (unsigned i = 0; i < 3; i++)
      {
         if (haveVel && haveAcc)
         {
            osp3->pos[i] = LagrangeInterpolation(tdata,posData[i],dt,
                                                 err);
            osp3->vel[i] = LagrangeInterpolation(tdata,velData[i],dt,
                                                 err);
            osp3->acc[i] = LagrangeInterpolation(tdata,accData[i],dt,
                                                 err);
         }
         else if (haveVel && !haveAcc)
         {
            osp3->pos[i] = LagrangeInterpolation(tdata,posData[i],dt,
                                                 err);
            LagrangeInterpolation(tdata, velData[i], dt, osp3->vel[i],
                                  osp3->acc[i]);
         }
         else
         {
               // It would be a strange case to have
               // acceleration data but no velocity data, so
               // derive both.
            LagrangeInterpolation(tdata, posData[i], dt, osp3->pos[i],
                                  osp3->vel[i]);
               // PositionSatStore doesn't derive
               // acceleration in this case, near as I can
               // tell.
         }
      } // for (unsigned i = 0; i < 3; i++)
      for (unsigned i = 0; i < 3; i++)
      {
         cerr << "  pos[" << i << "]=" << osp3->pos[i] << endl
              << "  vel[" << i << "]=" << osp3->vel[i] << endl
              << "  acc[" << i << "]=" << osp3->acc[i] << endl;
      }
   }


   void SP3NavDataFactory ::
   interpolateClk(const NavMap::iterator& ti1, const NavMap::iterator& ti3,
                  const CommonTime& when, NavDataPtr& navData)
   {
      cerr << "  start interpolating clock, distance = " << std::distance(ti1,ti3) << endl;
      std::vector<double> tdata(2*halfOrder), biasData(2*halfOrder),
         driftData(2*halfOrder), drRateData(2*halfOrder);
      CommonTime firstTime(ti1->second->timeStamp);
      unsigned idx = 0;
      bool haveDrift = false, haveDriftRate = false;
      NavMap::iterator ti2;
      for (ti2 = ti1, idx=0; ti2 != ti3; ++ti2, ++idx)
      {
         cerr << "  idx=" << idx << endl;
         tdata[idx] = ti2->second->timeStamp - firstTime;
         OrbitDataSP3 *nav = dynamic_cast<OrbitDataSP3*>(
            ti2->second.get());
         cerr << "  nav=" << nav << endl;
         ti2->second->dump(cerr, NavData::Detail::Full);
         biasData[idx] = nav->clkBias;
         driftData[idx] = nav->clkDrift;
         drRateData[idx] = nav->clkDrRate;
         haveDrift |= (nav->clkDrift != 0.0);
         haveDriftRate |= (nav->clkDrRate != 0.0);
      }
      double dt = when - firstTime, err;
      OrbitDataSP3 *osp3 = dynamic_cast<OrbitDataSP3*>(navData.get());
      cerr << setprecision(20) << "  dt=" << dt << endl;
      if (haveDrift && haveDriftRate)
      {
         osp3->clkBias = LagrangeInterpolation(tdata,biasData,dt,err);
         osp3->clkDrift = LagrangeInterpolation(tdata,driftData,dt,
                                                err);
         osp3->clkDrRate = LagrangeInterpolation(tdata,drRateData,dt,
                                                 err);
      }
      else if (haveDrift && !haveDriftRate)
      {
         osp3->clkBias = LagrangeInterpolation(tdata,biasData,dt,err);
         LagrangeInterpolation(tdata, driftData, dt, osp3->clkDrift,
                               osp3->clkDrRate);
      }
      else
      {
         LagrangeInterpolation(tdata, biasData, dt, osp3->clkBias,
                               osp3->clkDrift);
      }
   }
} // namespace gpstk
