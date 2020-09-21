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
      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
         // dig through the maps of maps, matching keys with nmid along the way
      auto dataIt = data.find(nmid.messageType);
      if (dataIt == data.end())
      {
         // std::cerr << " false = not found 1" << std::endl;
         return false; // not found.
      }
         // To support wildcard signals, we need to do a linear search.
      for (auto& sati : dataIt->second)
      {
         if (sati.first != nmid)
            continue; // skip non matches
            // This is not the entry we want, but it is instead the first
            // entry we probably (depending on order) *don't* want.
         auto ti2 = sati.second.lower_bound(when);
         if (order == NavSearchOrder::User)
         {
               // The map goes by transmit time so we do what should be a
               // quick linear search to find the best by user time
               // (i.e. when the entire message will have been received).
               // std::cerr << " when="
               //           << printTime(when,"%Y/%03j/%02H:%02M:%02S")
               //           << std::endl;
               // If ti2 starts at the end of the map, that doesn't mean
               // we've failed.  We should still try and back up at least
               // once.
            if (ti2 == sati.second.end())
            {
               ti2 = std::prev(ti2);
            }
            while (((ti2 != sati.second.end()) &&
                    (ti2->second->getUserTime() > when)) ||
                   !validityCheck(ti2, sati.second, valid))
            {
                  // if (ti2 == sati.second.begin())
                  // {
                  //    std::cerr << "already at the beginning" << std::endl;
                  // }
                  // std::cerr << "  ti2->second->getUserTime()="
                  //           << printTime(ti2->second->getUserTime(),
                  //                        "%Y/%03j/%02H:%02M:%02S")
                  //           << std::endl;
                  // don't use operator--, it won't give the right results
               ti2 = std::prev(ti2);
                  // if (ti2 == sati.second.end())
                  // {
                  //    std::cerr << "reached the end" << std::endl;
                  // }
            }
            if (ti2 == sati.second.end())
            {
                  // no good
                  // std::cerr << " false = not good 4" << std::endl;
               return false;
            }
               // std::cerr << "  ti2->second->getUserTime()="
               //           << printTime(ti2->second->getUserTime(),
               //                        "%Y/%03j/%02H:%02M:%02S")
               //           << std::endl;
               // ti2->second->dump(std::cerr,NavData::Detail::Full);
               // good.
            navData = ti2->second;
            return true;
         }
         else if (order == NavSearchOrder::Nearest)
         {
         }
      }
      // std::cerr << " false 5" << std::endl;
      return false;
   }


   void NavDataFactoryWithStore ::
   edit(const CommonTime& fromTime, const CommonTime& toTime)
   {
      for (auto mti = data.begin(); mti != data.end();)
      {
         for (auto sati = mti->second.begin(); sati != mti->second.end();)
         {
            auto ti1 = sati->second.lower_bound(fromTime);
            auto ti2 = sati->second.lower_bound(toTime);
            sati->second.erase(ti1,ti2);
               // clean out empty maps
            if (sati->second.empty())
            {
               sati = mti->second.erase(sati);
            }
            else
            {
               ++sati;
            }
         }
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
         for (auto sati = mti->second.begin(); sati != mti->second.end();)
         {
            if (sati->first != satID)
            {
                  // not a match
               ++sati;
               continue;
            }
            auto ti1 = sati->second.lower_bound(fromTime);
            auto ti2 = sati->second.lower_bound(toTime);
            sati->second.erase(ti1,ti2);
               // clean out empty maps
            if (sati->second.empty())
            {
               sati = mti->second.erase(sati);
            }
            else
            {
               ++sati;
            }
         }
            // clean out empty maps
         if (mti->second.empty())
         {
            mti = data.erase(mti);
         }
         else
         {
            ++mti;
         }
      }
   }


   void NavDataFactoryWithStore ::
   edit(const CommonTime& fromTime, const CommonTime& toTime,
        const NavSignalID& signal)
   {
      NavSatelliteID satMatch(signal);
      return edit(fromTime, toTime, satMatch);
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
      nmm[nd->signal.messageType][nd->signal][nd->timeStamp] = nd;
      return true;
   }


   size_t NavDataFactoryWithStore ::
   size() const
   {
      size_t rv = 0;
      for (const auto& mti : data)
      {
         for (const auto& satIt : mti.second)
         {
            rv += satIt.second.size();
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
         for (const auto& satIt : mti.second)
         {
            uniques.insert(satIt.first);
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
         for (const auto& satIt : mti.second)
         {
            uniques.insert(satIt.first);
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
         for (const auto& nsami : nmmi.second)
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
