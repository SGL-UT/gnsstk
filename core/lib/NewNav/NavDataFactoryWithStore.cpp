#include <iterator>
#include "NavDataFactoryWithStore.hpp"
#include "TimeString.hpp"
#include "OrbitDataKepler.hpp"
#include "NavHealthData.hpp"

namespace gpstk
{
   bool NavDataFactoryWithStore ::
   find(const NavMessageID& nmid, const CommonTime& when,
        NavDataPtr& navData, SVHealth xmitHealth, NavValidityType valid,
        NavSearchOrder order)
   {

         /** Class for gathering matches in find().  It's only used in
          * find so it is declared and implemented here alone. */
      class FindMatches
      {
      public:
         NavMap *map;
         bool finished;
         NavMap::iterator it;
      };

      // std::cerr << __PRETTY_FUNCTION__ << std::endl;
         // dig through the maps of maps, matching keys with nmid along the way
      auto dataIt = data.find(nmid.messageType);
      if (dataIt == data.end())
      {
         // std::cerr << " false = not found 1" << std::endl;
         return false; // not found.
      }
         // Make a collection of the NavMap iterators that match the
         // requested NavMessageID.  If wildcards are used in the
         // NavMessageID, we have to do a linear search for matches,
         // otherwise we can do a direct match using map::find.  The
         // bool of the pair in itMap is used to indicate that there
         // is no point in further manipulating the iterator as it is
         // either older than the most recent matching data or there's
         // nothing more to find.
      using ItPair = std::pair<bool,NavMap::iterator>;
      std::map<NavMap*, ItPair> itMap;
      if (nmid.isWild())
      {
         // std::cerr << "wildcard search: " << nmid << std::endl;
         for (NavSatMap::iterator sati = dataIt->second.begin();
              sati != dataIt->second.end(); sati++)
         {
            if (sati->first != nmid)
               continue; // skip non matches
            // std::cerr << "  matches " << sati->first << std::endl;
            NavMap::iterator nmi = sati->second.lower_bound(when);
            if (nmi == sati->second.end())
            {
               nmi = std::prev(nmi);
            }
            while ((nmi != sati->second.end()) &&
                   (nmi->second->getUserTime() > when))
            {
               nmi = (nmi == sati->second.begin() ? sati->second.end()
                      : std::prev(nmi));
            }
            if (nmi != sati->second.end())
               itMap[&(sati->second)] = ItPair(false,nmi);
         }
      }
      else
      {
         // std::cerr << "non-wildcard search: " << nmid << std::endl;
         auto sati = dataIt->second.find(nmid);
         if (sati != dataIt->second.end())
         {
            // std::cerr << "  found" << std::endl;
            NavMap::iterator nmi = sati->second.lower_bound(when);
            if (nmi == sati->second.end())
            {
               nmi = std::prev(nmi);
            }
            while ((nmi != sati->second.end()) &&
                   (nmi->second->getUserTime() > when))
            {
               nmi = (nmi == sati->second.begin() ? sati->second.end()
                      : std::prev(nmi));
            }
            if (nmi != sati->second.end())
               itMap[&(sati->second)] = ItPair(false,nmi);
         }
         // else
         // {
         //    std::cerr << "  not found" << std::endl;
         // }
      }
      // std::cerr << "itMap.size() = " << itMap.size() << std::endl;
      gpstk::CommonTime mostRecent = gpstk::CommonTime::BEGINNING_OF_TIME;
      mostRecent.setTimeSystem(gpstk::TimeSystem::Any);
      bool done = itMap.empty();
      bool rv = false;
      while (!done)
      {
         for (auto& imi : itMap)
         {
            // if (imi.second.second != imi.first->end())
            //    std::cerr << "examining " << imi.second.second->second->signal << std::endl;
            // else
            //    std::cerr << "examining end iterator" << std::endl;
            done = true; // default to being done.  Gets reset to false below.
            if (imi.second.first)
            {
                  // no need to process this iterator any further
               continue;
            }
            else if ((imi.second.second != imi.first->end()) &&
                     (imi.second.second->second->getUserTime() < mostRecent))
            {
                  // Data is less recent than the most recent good data, so stop
                  // processing this iterator.
               imi.second.first = true;
            }
            else if (((imi.second.second != imi.first->end()) &&
                      (imi.second.second->second->getUserTime() > when)) ||
                     !validityCheck(imi.second.second,*imi.first,valid,
                                    xmitHealth))
            {
               // std::cerr << "  not end, not right time" << std::endl;
               imi.second.second = (imi.second.second == imi.first->begin()
                                    ? imi.first->end()
                                    : std::prev(imi.second.second));
               done = false;
            }
            else if (imi.second.second == imi.first->end())
            {
                  // give up.
               imi.second.first = true;
            }
            else
            {
               // std::cerr << "Found something good at "
               //           << printTime(imi.second.second->first,
               //                        "%Y/%03j/%02H:%02M:%02S")
               //           << std::endl;
               if (imi.second.second->second->getUserTime() > mostRecent)
               {
                  mostRecent = imi.second.second->second->getUserTime();
                  navData = imi.second.second->second;
                  // std::cerr << "result is now " << navData->signal << std::endl;
               }
               imi.second.first = true;
               rv = true;
            }
         }
      }
      // std::cerr << "Most recent = "
      //           << printTime(mostRecent, "%Y/%03j/%02H:%02M:%02S")
      //           << std::endl;
      return rv;
   }


   bool NavDataFactoryWithStore ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, double& offset,
             SVHealth xmitHealth, NavValidityType valid, NavSearchOrder order)
   {
         /// @todo implement "Nearest" search
         /// @todo Implement xmitHealth matching
      // std::cerr << printTime(when,"looking for %Y/%03j/%02H:%02M:%02S") << std::endl;
      bool rv = false;
      TimeOffsetData::TimeCvtKey fwdKey(fromSys,toSys), bwdKey(toSys,fromSys);
         // These get set according to the direction of the search.
      TimeSystem getFromSys = fromSys, getToSys = toSys;
      bool reverse = false; // if true, we need to negate the offset
      auto odi = offsetData.find(fwdKey);
      if (odi == offsetData.end())
      {
         // std::cerr << "did not find forward key" << std::endl;
         odi = offsetData.find(bwdKey);
         if (odi == offsetData.end())
         {
            // std::cerr << "did not find reverse key, giving up" << std::endl;
            return false; // no conversion available in either direction
         }
         // std::cerr << "found reverse key" << std::endl;
            // The offset we get is actually going to fromSys from
            // toSys so negate the offset at the end.
         getFromSys = toSys;
         getToSys = fromSys;
         reverse = true;
      }
      // else
      // {
      //    std::cerr << "found forward key" << std::endl;
      // }
      auto oemi = odi->second.lower_bound(when);
      if (oemi == odi->second.end())
      {
         // std::cerr << "got end right away, backing up one" << std::endl;
         oemi = std::prev(oemi);
      }
      // std::cerr << printTime(oemi->first,"found %Y/%03j/%02H:%02M:%02S") << std::endl;
      for (const auto& omi : oemi->second)
      {
         TimeOffsetData *todp = dynamic_cast<TimeOffsetData*>(omi.second.get());
         if (todp == nullptr)
            continue; // shouldn't happen.
         switch (valid)
         {
            case NavValidityType::ValidOnly:
               if (omi.second->validate())
               {
                  rv = todp->getOffset(getFromSys, getToSys, when, offset);
                  if (reverse)
                     offset = -offset;
                  return rv;
               }
               break;
            case NavValidityType::InvalidOnly:
               if (!omi.second->validate())
               {
                  rv = todp->getOffset(getFromSys, getToSys, when, offset);
                  if (reverse)
                     offset = -offset;
                  return rv;
               }
               break;
            default:
               rv = todp->getOffset(getFromSys, getToSys, when, offset);
               if (reverse)
                  offset = -offset;
               return rv;
               break;
         }
      }
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
      TimeOffsetData *todp = nullptr;
      if ((todp = dynamic_cast<TimeOffsetData*>(nd.get())) == nullptr)
      {
            // everything BUT TimeOffsetData
         data[nd->signal.messageType][nd->signal][nd->getUserTime()] = nd;
         return true;
      }
         // TimeOffsetData
      TimeOffsetData::TimeCvtSet conversions = todp->getConversions();
      for (const auto& ci : conversions)
      {
         offsetData[ci][nd->getUserTime()][nd->signal] = nd;
      }
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
         // OffsetCvtMap can contain duplicates when a TimeOffsetData
         // object applies to multiple time systems, so count unique
         // pointers.
      std::set<NavData*> ndpUnique;
      for (const auto& ocmi : offsetData)
      {
         for (const auto& oemi : ocmi.second)
         {
            for (const auto& omi : oemi.second)
            {
               ndpUnique.insert(omi.second.get());
            }
         }
      }
      return rv + ndpUnique.size();
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
      for (const auto& ocmi : offsetData)
      {
         for (const auto& oemi : ocmi.second)
         {
            for (const auto& omi : oemi.second)
            {
               uniques.insert(omi.first);
            }
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
      for (const auto& ocmi : offsetData)
      {
         for (const auto& oemi : ocmi.second)
         {
            for (const auto& omi : oemi.second)
            {
               uniques.insert(omi.first);
            }
         }
      }
      return uniques.size();
   }


   bool NavDataFactoryWithStore ::
   validityCheck(const NavMap::iterator& ti,
                 NavMap& nm,
                 NavValidityType valid,
                 SVHealth xmitHealth)
   {
      bool rv = true;
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
            rv = ti->second->validate();
            break;
         case NavValidityType::InvalidOnly:
            rv = !ti->second->validate();
            break;
         default:
            rv = true;
            break;
      }
      if (!rv)
      {
            // already determined to be invalid, don't bother doing
            // further checking
         return false;
      }
         // We're already trying to get health information, seems like
         // we could/should avoid getting into a loop by just
         // returning what we have.
      if (ti->second->signal.messageType == gpstk::NavMessageType::Health)
      {
         return rv;
      }
         // Set to true if the health status matched.  If it remains
         // false, we have to do a look up of the health status of the
         // transmitting satellite because the presumed matched data
         // doesn't contain the health status of the transmitting
         // satellite.
      bool rvSet = false;
      NavData *ndp = ti->second.get();
      OrbitDataKepler *orb;
      NavHealthData *hea;
      switch (xmitHealth)
      {
         case SVHealth::Healthy:
         case SVHealth::Unhealthy:
         case SVHealth::Degraded:
               // make sure the health status is the desired state
            if (ti->second->signal.sat == ti->second->signal.xmitSat)
            {
                  // If the subject and transmitting satellite are the
                  // same, assume the health state is up-to-date for
                  // the satellite.
               if ((orb = dynamic_cast<OrbitDataKepler*>(ndp)) != nullptr)
               {
                  rv = (xmitHealth == orb->health);
                  rvSet = true;
               }
               else if ((hea = dynamic_cast<NavHealthData*>(ndp)) != nullptr)
               {
                  rv = (xmitHealth == hea->getHealth());
                  rvSet = true;
               }
            }
            if (!rvSet)
            {
                  // We were not able to obtain health status of the
                  // transmitting satellite so look it up.  We
                  // specifically use SVHealth::Any because we're
                  // looking up the health.  Not sure if
                  // NavValidityType::Any is the proper choice, but
                  // NavSearchOrder::User definitely is, as that will
                  // result in getting the most recent health
                  // information prior to the time stamp of the data
                  // we're interested in.
               NavDataPtr heaPtr;
               NavMessageID nmid = ti->second->signal;
               nmid.messageType = NavMessageType::Health;
                  /** @todo This next statement kind of sort of
                   * enforces ephemeris health however it's possible,
                   * for example in GPS LNAV to have almanac pages
                   * with the transmit and subject satellites the
                   * same, making it effectively indistinguishible
                   * from ephemeris health.  Are there situations
                   * where that could yield incorrect results? */
               nmid.sat = nmid.xmitSat;
               if (!find(nmid, ti->second->timeStamp, heaPtr,
                         SVHealth::Any, NavValidityType::Any,
                         NavSearchOrder::User))
               {
                  return false;
               }
               hea = dynamic_cast<NavHealthData*>(heaPtr.get());
               rv = (xmitHealth == hea->getHealth());
            }
            break;
         default:
               // treat all other cases as valid
            break;
      }
      return rv;
   }


   void NavDataFactoryWithStore ::
   dump(std::ostream& s, NavData::Detail dl) const
   {
      for (const auto& nmmi : data)
      {
         for (const auto& nsami : nmmi.second)
         {
            switch (dl)
            {
               case NavData::Detail::OneLine:
                  s << StringUtils::asString(nmmi.first) << " "
                    << StringUtils::asString(nsami.first) << std::endl;
                  break;
               case NavData::Detail::Brief:
                  for (const auto& cti : nsami.second)
                  {
                     s << StringUtils::asString(nmmi.first) << " "
                       << StringUtils::asString(nsami.first) << "   "
                       << printTime(cti.first, "%Y %2m %2d %02H:%02M:%04.1f")
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
         // time offset data is a separate map, but still needs to be dumped.
      std::string label = StringUtils::asString(NavMessageType::TimeOffset);
      for (const auto& ocmi : offsetData)
      {
         switch (dl)
         {
            case NavData::Detail::OneLine:
               s << label << " " << ocmi.first.first << " -> "
                 << ocmi.first.second << std::endl;
               break;
            case NavData::Detail::Brief:
               for (const auto& oemi : ocmi.second)
               {
                  for (const auto& omi : oemi.second)
                  {
                     s << label << " " << ocmi.first.first << " -> "
                       << ocmi.first.second << " " << omi.first << "   "
                       << printTime(oemi.first, "%Y %2m %2d %02H:%02M:%04.1f")
                       << std::endl;
                  }
               }
               break;
            case NavData::Detail::Full:
               for (const auto& oemi : ocmi.second)
               {
                  for (const auto& omi : oemi.second)
                  {
                     omi.second->dump(s,dl);
                  }
               }
               break;
         }
      }
   }
}
