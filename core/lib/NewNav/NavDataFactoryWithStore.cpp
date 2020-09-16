#include <iterator>
#include "NavDataFactoryWithStore.hpp"
#include "TimeString.hpp"

namespace gpstk
{
   bool NavDataFactoryWithStore ::
   find(const NavMessageID& nmid, const CommonTime& when,
        NavDataPtr& navData, NavValidityType valid,
        NavSearchOrder order)
   {
         // dig through the maps of maps, matching keys with nmid along the way
      if (data.find(nmid.messageType) == data.end())
      {
         // std::cerr << " false = not found 1" << std::endl;
         return false; // not found.
      }
      NavSignalMap &nsigm(data[nmid.messageType]);
      if (nsigm.find(nmid) == nsigm.end())
      {
         // std::cerr << " false = not found 2" << std::endl;
         return false; // not found.
      }
      NavSatMap &nsm(nsigm[nmid]);
      if (nsm.find(nmid) == nsm.end())
      {
         // std::cerr << " false = not found 3" << std::endl;
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
         // std::cerr << " when=" << printTime(when,"%Y/%03j/%02H:%02M:%02S")
         //           << std::endl;
         while (((ti2 != nm.end()) && (ti2->second->getUserTime() > when)) ||
                !validityCheck(ti2, nm, valid))
         {
            // if (ti2 == nm.begin())
            // {
            //    std::cerr << "already at the beginning" << std::endl;
            // }
            // std::cerr << "  ti2->second->getUserTime()="
            //           << printTime(ti2->second->getUserTime(),
            //                        "%Y/%03j/%02H:%02M:%02S") << std::endl;
               // don't use operator--, it won't give the right results
            ti2 = std::prev(ti2);
            // if (ti2 == nm.end())
            // {
            //    std::cerr << "reached the end" << std::endl;
            // }
         }
         if (ti2 == nm.end())
         {
               // no good
            // std::cerr << " false = not good 4" << std::endl;
            return false;
         }
         // std::cerr << "  ti2->second->getUserTime()="
         //           << printTime(ti2->second->getUserTime(),
         //                        "%Y/%03j/%02H:%02M:%02S") << std::endl;
         // ti2->second->dump(std::cerr,NavData::Detail::Full);
            // good.
         navData = ti2->second;
         return true;
      }
      else if (order == NavSearchOrder::Nearest)
      {
      }
      // std::cerr << " false 5" << std::endl;
      return false;
   }


   void NavDataFactoryWithStore ::
   edit(const CommonTime& fromTime, const CommonTime& toTime)
   {
      for (auto mti = data.begin(); mti != data.end();)
      {
         for (auto sigi = mti->second.begin(); sigi != mti->second.end();)
         {
            for (auto sati = sigi->second.begin(); sati != sigi->second.end();)
            {
               auto ti1 = sati->second.lower_bound(fromTime);
               auto ti2 = sati->second.lower_bound(toTime);
               sati->second.erase(ti1,ti2);
                  // clean out empty maps
               if (sati->second.empty())
               {
                  sati = sigi->second.erase(sati);
               }
               else
               {
                  ++sati;
               }
            }
               // clean out empty maps
            if (sigi->second.empty())
            {
               sigi = mti->second.erase(sigi);
            }
            else
            {
               ++sigi;
            }
         } // for (auto& sigi : mti->second)
            // clean out empty maps
         if (mti->second.empty())
         {
            mti = data.erase(mti);
         }
         else
         {
            ++mti;
         }
      } // for (auto& mti : data)
   }


   void NavDataFactoryWithStore ::
   edit(const CommonTime& fromTime, const CommonTime& toTime,
        const NavSatelliteID& satID)
   {
      for (auto mti = data.begin(); mti != data.end();)
      {
         if (mti->second.find(satID) == mti->second.end())
         {
            ++mti;
            continue;
         }
         NavSatMap &nsm(mti->second[satID]);
         if (nsm.find(satID) == nsm.end())
            return; // nothing to do
         NavMap &nm(nsm[satID]);
         auto ti1 = nm.lower_bound(fromTime);
         auto ti2 = nm.lower_bound(toTime);
         nm.erase(ti1,ti2);
            // clean out empty maps
         if (nm.empty())
            nsm.erase(satID);
         if (nsm.empty())
            mti->second.erase(satID);
         if (mti->second.empty())
            mti = data.erase(mti);
         else
            ++mti;
      }
   }


   void NavDataFactoryWithStore ::
   edit(const CommonTime& fromTime, const CommonTime& toTime,
        const NavSignalID& signal)
   {
      for (auto mti = data.begin(); mti != data.end();)
      {
         if (mti->second.find(signal) == mti->second.end())
         {
            ++mti;
            continue;
         }
         NavSatMap &nsm(mti->second[signal]);

         for (auto sati = nsm.begin(); sati != nsm.end();)
         {
            auto ti1 = sati->second.lower_bound(fromTime);
            auto ti2 = sati->second.lower_bound(toTime);
            sati->second.erase(ti1,ti2);
               // clean out empty maps
            if (sati->second.empty())
               sati = nsm.erase(sati);
            else
               ++sati;
         }
            // clean out empty maps
         if (nsm.empty())
            mti->second.erase(signal);
         if (mti->second.empty())
            mti = data.erase(mti);
         else
            ++mti;
      }
   }


   void NavDataFactoryWithStore ::
   clear()
   {
      data.clear();
   }


   bool NavDataFactoryWithStore ::
   addNavData(const NavDataPtr& nd)
   {
      return addNavData(data, nd);
   }


   bool NavDataFactoryWithStore ::
   addNavData(NavMessageMap& nmm, const NavDataPtr& nd)
   {
         /* This is for testing only
      if (data.find(nd->signal) != data.end())
      {
         NavSatMap& nsm(data[nd->signal]);
         if (nsm.find(nd->signal) != nsm.end())
         {
            NavMap& nm(nsm[nd->signal]);
            if (nm.find(nd->timeStamp) != nm.end())
            {
               std::cerr << "duplicate message" << std::endl;
            }
         }
      }
         */
         // It looks funny because we're relying on inheritance to
         // implicitly become the proper key class type.
      nmm[nd->signal.messageType][nd->signal][nd->signal][nd->timeStamp] = nd;
      return true;
   }


   size_t NavDataFactoryWithStore ::
   size() const
   {
      size_t rv = 0;
      for (const auto& mti : data)
      {
         for (const auto& sigIt : mti.second)
         {
            for (const auto& satIt : sigIt.second)
            {
               rv += satIt.second.size();
            }
         }
      }
      return rv;
   }


   size_t NavDataFactoryWithStore ::
   numSignals() const
   {
      std::set<NavSignalID> uniques;
      for (const auto& mti : data)
      {
         for (const auto& sigIt : mti.second)
         {
            uniques.insert(sigIt.first);
         }
      }
      return uniques.size();
   }


   size_t NavDataFactoryWithStore ::
   numSatellites() const
   {
      std::set<NavSatelliteID> uniques;
      for (const auto& mti : data)
      {
         for (const auto& sigIt : mti.second)
         {
            for (const auto& satIt : sigIt.second)
            {
               uniques.insert(satIt.first);
            }
         }
      }
      return uniques.size();
   }


   bool NavDataFactoryWithStore ::
   validityCheck(const NavMap::iterator& ti,
                 NavMap& nm,
                 NavValidityType valid)
   {
         // We can't check the validity of an invalid iterator, BUT we
         // have to say it's valid because otherwise the while loop in
         // find() breaks.
      if (ti == nm.end())
      {
         return true;
      }
      switch (valid)
      {
         case NavValidityType::ValidOnly:
            return ti->second->validate();
         case NavValidityType::InvalidOnly:
            return !ti->second->validate();
         default:
            return true;
      }
   }


   void NavDataFactoryWithStore ::
   dump(std::ostream& s, NavData::Detail dl) const
   {
      for (const auto& nmmi : data)
      {
         for (const auto& nsimi : nmmi.second)
         {
            for (const auto& nsami : nsimi.second)
            {
               s << StringUtils::asString(nmmi.first) << " "
                 << StringUtils::asString(nsami.first) << std::endl;
               switch (dl)
               {
                  case NavData::Detail::OneLine:
                     break;
                  case NavData::Detail::Brief:
                     for (const auto& cti : nsami.second)
                     {
                        s << "   " << printTime(cti.first,
                                                "%Y %2m %2d %02H:%02M:%04.1f")
                          << std::endl;
                     }
                     break;
                  case NavData::Detail::Full:
                     for (const auto& cti : nsami.second)
                     {
                        cti.second->dump(s,dl);
                     }
                     break;
               }
            }
         }
      }
   }
}
