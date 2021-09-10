//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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
#include <iterator>
#include "NavDataFactoryWithStore.hpp"
#include "TimeString.hpp"
#include "OrbitDataKepler.hpp"
#include "NavHealthData.hpp"
#include "BasicTimeSystemConverter.hpp"

#if !defined(__PRETTY_FUNCTION__)
#ifdef __FUNCSIG__
#define __PRETTY_FUNCTION__ __FUNCSIG__
#else
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif
#endif

/// debug time string
static const std::string dts("%Y/%03j/%02H:%02M:%02S %P");

namespace gnsstk
{
   NavDataFactoryWithStore ::
   NavDataFactoryWithStore()
   {
         // We are NOT using END_OF_TIME or BEGINNING_OF_TIME here
         // because of issues with static initialization order.  As
         // such we're essentially forced to use magic numbers here,
         // because we can't guarantee that any static data (outside
         // this class) will be initialized prior to this constructor.
      initialTime.set(3442448L,0,0.0,TimeSystem::Any);
      finalTime.set(0,0,0.0,TimeSystem::Any);
   }


   bool NavDataFactoryWithStore ::
   find(const NavMessageID& nmid, const CommonTime& when,
        NavDataPtr& navOut, SVHealth xmitHealth, NavValidityType valid,
        NavSearchOrder order)
   {
      bool rv = false;
      switch (order)
      {
         case NavSearchOrder::User:
            rv = findUser(nmid, when, navOut, xmitHealth, valid);
            break;
         case NavSearchOrder::Nearest:
            rv = findNearest(nmid, when, navOut, xmitHealth, valid);
            break;
         default:
               // requested an invalid search order
            return false;
      }
      if (rv)
      {
            // One last check for fit interval validity, but it only
            // applies to OrbitDataKepler.
         OrbitDataKepler *odk = dynamic_cast<OrbitDataKepler*>(navOut.get());
         if (odk != nullptr)
         {
            if ((when < odk->beginFit) || (when > odk->endFit))
            {
                  // not a valid match, so clear the results.
               navOut.reset();
               rv = false;
            }
         }
      }
      return rv;
   }


   bool NavDataFactoryWithStore ::
   findUser(const NavMessageID& nmid, const CommonTime& when,
            NavDataPtr& navData, SVHealth xmitHealth,
            NavValidityType valid)
   {
         /** Class for gathering matches in findUser().  It's only
          * used in findUser so it is declared and implemented here
          * alone. */
      class FindMatches
      {
      public:
         FindMatches()
               : map(nullptr), finished(true)
         {}
         FindMatches(NavMap *theMap, const NavMap::iterator& theIt)
               : map(theMap), finished(false), it(theIt)
         {}
         NavMap *map;
         bool finished;
         NavMap::iterator it;
      };
      using MatchList = std::list<FindMatches>;

      if (debugLevel)
      {
         std::cerr << __PRETTY_FUNCTION__ << std::endl
                   << "  nmid=" << nmid << std::endl
                   << "  when=" << gnsstk::printTime(when,dts) << std::endl;
      }
         // dig through the maps of maps, matching keys with nmid along the way
      auto dataIt = data.find(nmid.messageType);
      if (dataIt == data.end())
      {
         if (debugLevel)
         {
            std::cerr << " false = not found 1" << std::endl;
         }
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
      MatchList itList;
      if (nmid.isWild())
      {
         if (debugLevel)
         {
            std::cerr << "wildcard search: " << nmid << std::endl;
         }
         for (NavSatMap::iterator sati = dataIt->second.begin();
              sati != dataIt->second.end(); sati++)
         {
            if (sati->first != nmid)
            {
               if (debugLevel)
               {
                  std::cerr << "  " << sati->first << " != " << nmid
                            << std::endl;
               }
               continue; // skip non matches
            }
            if (debugLevel)
            {
               std::cerr << "  matches " << sati->first << std::endl;
            }
            NavMap::iterator nmi = sati->second.lower_bound(when);
            if (nmi == sati->second.end())
            {
               nmi = std::prev(nmi);
            }
            if (debugLevel)
            {
               std::cerr << "  user time : "
                         << gnsstk::printTime(nmi->second->getUserTime(),dts)
                         << std::endl
                         << "  (nmi != sati->second.end()) = "
                         << (nmi != sati->second.end()) << std::endl
                         << "  (nmi->second->getUserTime() > when) = "
                         << (nmi->second->getUserTime() > when) << std::endl;
            }
            while ((nmi != sati->second.end()) &&
                   (nmi->second->getUserTime() > when))
            {
               if (debugLevel)
               {
                  std::cerr << "  backing up (maybe)" << std::endl;
               }
               nmi = (nmi == sati->second.begin() ? sati->second.end()
                      : std::prev(nmi));
               if ((debugLevel > 0) && (nmi != sati->second.end()))
               {
                  std::cerr << "  user time : "
                            << gnsstk::printTime(nmi->second->getUserTime(),dts)
                            << std::endl;
               }
            }
            if (nmi != sati->second.end())
            {
               itList.push_back(FindMatches(&(sati->second), nmi));
            }
            else if (debugLevel)
            {
               std::cerr << "  did not add match" << std::endl;
            }
         }
      }
      else
      {
         if (debugLevel)
         {
            std::cerr << "non-wildcard search: " << nmid << std::endl;
         }
         auto sati = dataIt->second.find(nmid);
         if (sati != dataIt->second.end())
         {
            if (debugLevel)
            {
               std::cerr << "  found" << std::endl;
            }
            NavMap::iterator nmi = sati->second.lower_bound(when);
            if (nmi == sati->second.end())
            {
               nmi = std::prev(nmi);
            }
            if (debugLevel)
            {
               std::cerr << "  user time : "
                         << gnsstk::printTime(nmi->second->getUserTime(),dts)
                         << std::endl;
            }
            while ((nmi != sati->second.end()) &&
                   (nmi->second->getUserTime() > when))
            {
               nmi = (nmi == sati->second.begin() ? sati->second.end()
                      : std::prev(nmi));
               if ((debugLevel > 0) && (nmi != sati->second.end()))
               {
                  std::cerr << "  user time : "
                            << gnsstk::printTime(nmi->second->getUserTime(),dts)
                            << std::endl;
               }
            }
            if (nmi != sati->second.end())
            {
               itList.push_back(FindMatches(&(sati->second), nmi));
            }
            else if (debugLevel)
            {
               std::cerr << "  did not add match" << std::endl;
            }
         }
         else if (debugLevel)
         {
            std::cerr << "  not found" << std::endl;
         }
      }
      if (debugLevel)
      {
         std::cerr << "itList.size() = " << itList.size() << std::endl;
      }
      gnsstk::CommonTime mostRecent = gnsstk::CommonTime::BEGINNING_OF_TIME;
      mostRecent.setTimeSystem(gnsstk::TimeSystem::Any);
      bool done = itList.empty();
      bool rv = false;
      while (!done)
      {
         for (auto& imi : itList)
         {
            done = true; // default to being done.  Gets reset to false below.
            if (imi.finished)
            {
                  // no need to process this iterator any further
               continue;
            }
            else if ((imi.it != imi.map->end()) &&
                     (imi.it->second->getUserTime() < mostRecent))
            {
                  // Data is less recent than the most recent good data, so stop
                  // processing this iterator.
               imi.finished = true;
            }
            else if (((imi.it != imi.map->end()) &&
                      (imi.it->second->getUserTime() > when)) ||
                     !validityCheck(imi.it,*imi.map,valid,xmitHealth,when))
            {
               if (debugLevel)
               {
                  std::cerr << "  not end, not right time" << std::endl;
               }
               imi.it = (imi.it == imi.map->begin()
                         ? imi.map->end()
                         : std::prev(imi.it));
               done = false;
            }
            else if (imi.it == imi.map->end())
            {
                  // give up.
               imi.finished = true;
            }
            else
            {
               if (debugLevel)
               {
                  std::cerr << "Found something good at "
                            << printTime(imi.it->first, dts)
                            << std::endl;
               }
               if (imi.it->second->getUserTime() > mostRecent)
               {
                  mostRecent = imi.it->second->getUserTime();
                  navData = imi.it->second;
                  if (debugLevel)
                  {
                     std::cerr << "result is now " << navData->signal
                               << std::endl;
                  }
               }
               imi.finished = true;
               rv = true;
            }
         }
      }
      if (debugLevel)
      {
         std::cerr << "Most recent = "
                   << printTime(mostRecent, dts)
                   << std::endl;
      }
      return rv;
   }


   bool NavDataFactoryWithStore ::
   findNearest(const NavMessageID& nmid, const CommonTime& when,
               NavDataPtr& navData, SVHealth xmitHealth,
               NavValidityType valid)
   {
         /** Class for gathering matches in findNearest().  It's only
          * used in findNearest so it is declared and implemented here
          * alone. */
      class FindMatches
      {
      public:
         // FindMatches()
         //       : map(nullptr)
         // {}
            /** Constructor for the iterator yielded by lower_bound.
             * @param[in] theMap A pointer to the map containing the
             *   matching data.
             * @param[in] theIt The iterator referring to the result
             *   from lower_bound.
             */
         FindMatches(NavNearMap *theMap, const NavNearMap::iterator& theIt,
                     const CommonTime& t)
               : map(theMap), itGT(theIt), when(t)
         {
               // set the "less than" iterator to an appropriate value
            itLT = (itGT == theMap->begin() ? theMap->end() : std::prev(itGT));
           
         }
         double getDistGT() const
         { return itGT == map->end() ? 999e99 : fabs(itGT->first - when); }
         double getDistLT() const
         { return itLT == map->end() ? 999e99 : fabs(itLT->first - when); }
         NavNearMap *map;
         NavNearMap::iterator itGT, itLT;
         const CommonTime& when;
      };
      using MatchList = std::list<FindMatches>;

      if (debugLevel)
      {
         std::cerr << __PRETTY_FUNCTION__ << std::endl;
      }
         // dig through the maps of maps, matching keys with nmid along the way
      auto dataIt = nearestData.find(nmid.messageType);
      if (dataIt == nearestData.end())
      {
         if (debugLevel)
         {
            std::cerr << " false = not found 1" << std::endl;
         }
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
      MatchList itList;
      if (nmid.isWild())
      {
         if (debugLevel)
         {
            std::cerr << "wildcard search: " << nmid << std::endl;
         }
         for (NavNearSatMap::iterator sati = dataIt->second.begin();
              sati != dataIt->second.end(); sati++)
         {
            if (sati->first != nmid)
               continue; // skip non matches
            if (debugLevel)
            {
               std::cerr << "  matches " << sati->first << std::endl
                         << "  when = " << gnsstk::printTime(when,dts)
                         << std::endl;
            }
            NavNearMap::iterator nmi = sati->second.lower_bound(when);
            itList.push_back(FindMatches(&(sati->second), nmi, when));
         }
      }
      else
      {
         if (debugLevel)
         {
            std::cerr << "non-wildcard search: " << nmid << std::endl;
         }
         auto sati = dataIt->second.find(nmid);
         if (sati != dataIt->second.end())
         {
            if (debugLevel)
            {
               std::cerr << "  found" << std::endl;
            }
            NavNearMap::iterator nmi = sati->second.lower_bound(when);
            itList.push_back(FindMatches(&(sati->second), nmi, when));
         }
         else if (debugLevel)
         {
            std::cerr << "  not found" << std::endl;
         }
      }
      bool done = itList.empty();
      bool rv = false;
      while (!done)
      {
         for (auto& imi : itList)
         {
            done = true; // default to being done.  Gets reset to false below.
            if ((imi.itGT == imi.map->end()) && (imi.itLT == imi.map->end()))
            {
                  // nothing more to do, we've reached the end of both
                  // directions
               break;
            }
               // At this point we have up to two iterators that refer
               // to data in a NavNearMap, which is a list of possible
               // matches.  So we iterate through each item in the
               // list until we find one that matches validity/health
               // requirements and return that.  If we do *not* find a
               // match in the list, then we increment or decrement
               // the iterators accordingly and try again until we DO
               // find a match or run out of options.
            if ((imi.itGT != imi.map->end()) &&
                ((imi.itLT == imi.map->end()) ||
                 (fabs(imi.itGT->first - when) < fabs(imi.itLT->first - when))))
            {
                  // time for itGT is nearer to time of interest, try it first.
               for (auto& ndpli : imi.itGT->second)
               {
                  if (validityCheck(ndpli, valid, xmitHealth,when))
                  {
                        // got a match
                     navData = ndpli;
                     return true;
                  }
               }
                  // no match, so advance the iterator and try again
               done = false;
               ++(imi.itGT);
            }
            else
            {
                  // time for itLT is nearer to time of interest, try it first.
               for (auto& ndpli : imi.itLT->second)
               {
                  if (validityCheck(ndpli, valid, xmitHealth,when))
                  {
                        // got a match
                     navData = ndpli;
                     return true;
                  }
               }
                  // no match, so decrement the iterator and try again
               done = false;
               imi.itLT = (imi.itLT == imi.map->begin()
                           ? imi.map->end()
                           : std::prev(imi.itLT));
            }
         }
      }
      return false;
   }


   bool NavDataFactoryWithStore ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, NavDataPtr& offset,
             SVHealth xmitHealth, NavValidityType valid)
   {
      if (debugLevel)
      {
         std::cerr << printTime(when,"looking for "+dts) << std::endl;
      }
      bool rv = false;
         // Only search for forward key and let the TimeOffset classes
         // and factories handle the reverse offset.
      TimeCvtKey fwdKey(fromSys,toSys);
         // First look in the offsetData map for the key matching the
         // offset translation in the forward direction (fromSys->toSys).
      auto odi = offsetData.find(fwdKey);
      if (debugLevel)
      {
         std::cerr << "  fwdKey=<" << gnsstk::StringUtils::asString(fwdKey.first)
                   << "," << gnsstk::StringUtils::asString(fwdKey.second) << ">"
                   << std::endl;
      }
      if (odi == offsetData.end())
      {
         if (debugLevel)
         {
            std::cerr << "did not find key, giving up" << std::endl;
            std::cerr << "offsetData.size() = " << offsetData.size()
                      << std::endl;
            for (const auto& x : offsetData)
            {
               std::cerr << "  fwdKey=<"
                         << gnsstk::StringUtils::asString(x.first.first)
                         << "," << gnsstk::StringUtils::asString(x.first.second)
                         << ">" << std::endl;
            }
         }
         return false; // no conversion available
      }
      else if (debugLevel)
      {
         std::cerr << "found forward key" << std::endl;
      }
         // Make a copy of "when" with a time system of Any so that we
         // can search for offset data whether we're doing a "forward"
         // conversion (e.g. GPS->UTC) or "backward" conversion
         // (e.g. UTC->GPS).
      CommonTime whenny(when);
      whenny.setTimeSystem(TimeSystem::Any);
      auto oemi = odi->second.lower_bound(whenny);
      if (oemi == odi->second.end())
      {
         if (debugLevel)
         {
            std::cerr << "got end right away, backing up one" << std::endl;
         }
         oemi = std::prev(oemi);
      }
      if (debugLevel)
      {
         std::cerr << printTime(oemi->first,"found "+dts) << std::endl;
      }
      bool done = false;
      while (!done)
      {
         if (oemi == odi->second.end())
         {
               // give up, couldn't find any valid matches.
            if (debugLevel)
            {
               std::cerr << "giving up, reached the end" << std::endl;
            }
            done = true;
         }
         else if (oemi->first > whenny)
         {
               // time of data is after the requested time of interest
               // so back up if possible
            oemi = (oemi == odi->second.begin()
                    ? odi->second.end() : std::prev(oemi));
            if (debugLevel)
            {
               std::cerr << printTime(oemi->first,"backed up to "+dts)
                         << std::endl;
            }
         }
         else
         {
            if (debugLevel)
            {
               std::cerr << "looking for acceptable data "
                         << gnsstk::StringUtils::asString(valid) << " "
                         << gnsstk::StringUtils::asString(xmitHealth)
                         << std::endl;
            }
               // Message time is valid, so iterate through the
               // per-signal data for a usable record (matching
               // validity and transmit satellite health)
            for (const auto& omi : oemi->second)
            {
               TimeOffsetData *todp = dynamic_cast<TimeOffsetData*>(
                  omi.second.get());
               if (debugLevel)
               {
                  std::cerr << "  checking " << todp->signal << std::endl;
               }
               if (todp == nullptr)
                  continue; // shouldn't happen.
               switch (valid)
               {
                  case NavValidityType::ValidOnly:
                     if (omi.second->validate() && matchHealth(todp,xmitHealth))
                     {
                        offset = omi.second;
                        return true;
                     }
                     break;
                  case NavValidityType::InvalidOnly:
                     if (!omi.second->validate() &&
                         matchHealth(todp,xmitHealth))
                     {
                        offset = omi.second;
                        return true;
                     }
                     break;
                  default:
                     if (matchHealth(todp,xmitHealth))
                     {
                        offset = omi.second;
                        return true;
                     }
                     break;
               }
            }
            if (debugLevel)
            {
               std::cerr << "  didn't find any acceptable data" << std::endl;
            }
            oemi = (oemi == odi->second.begin()
                    ? odi->second.end() : std::prev(oemi));
         }
      }
      return rv;
   }


   void NavDataFactoryWithStore ::
   edit(const CommonTime& fromTime, const CommonTime& toTime)
   {
         // edit transmit time storage
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
         // edit nearest storage
         // iterate over message types
      for (auto mti = nearestData.begin(); mti != nearestData.end();)
      {
            // iterate over NavSatelliteIDs
         for (auto sati = mti->second.begin(); sati != mti->second.end();)
         {
               // iterate over CommonTimes
            for (auto cti = sati->second.begin(); cti != sati->second.end();)
            {
                  // and iterate over the list.
               for (auto ndpli = cti->second.begin();
                    ndpli != cti->second.end();)
               {
                  if (((*ndpli)->timeStamp < toTime) &&
                      ((*ndpli)->timeStamp >= fromTime))
                  {
                     ndpli = cti->second.erase(ndpli);
                  }
                  else
                  {
                     ++ndpli;
                  }
               }
                  // clean out empty maps
               if (cti->second.empty())
               {
                  cti = sati->second.erase(cti);
               }
               else
               {
                  ++cti;
               }
            }
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
            mti = nearestData.erase(mti);
         }
         else
         {
            ++mti;
         }
      }
         // edit time offset storage
         // iterate over TimeCvtKey
      for (auto ocmi = offsetData.begin(); ocmi != offsetData.end();)
      {
            // iterate over CommonTimes
         for (auto cti = ocmi->second.begin(); cti != ocmi->second.end();)
         {
               // iterate over NavSatelliteIDs
            for (auto sati = cti->second.begin(); sati != cti->second.end();)
            {
               if ((sati->second->timeStamp < toTime) &&
                   (sati->second->timeStamp >= fromTime))
               {
                  sati = cti->second.erase(sati);
               }
               else
               {
                  ++sati;
               }
            }
               // clean out empty maps
            if (cti->second.empty())
            {
               cti = ocmi->second.erase(cti);
            }
            else
            {
               ++cti;
            }
         }
            // clean out empty maps
         if (ocmi->second.empty())
         {
            ocmi = offsetData.erase(ocmi);
         }
         else
         {
            ++ocmi;
         }
      }
   }


   void NavDataFactoryWithStore ::
   edit(const CommonTime& fromTime, const CommonTime& toTime,
        const NavSatelliteID& satID)
   {
         // edit transmit time storage
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
         // edit nearest storage
         // iterate over message types
      for (auto mti = nearestData.begin(); mti != nearestData.end();)
      {
            // iterate over NavSatelliteIDs
         for (auto sati = mti->second.begin(); sati != mti->second.end();)
         {
            if (sati->first != satID)
            {
                  // not a match
               ++sati;
               continue;
            }
               // iterate over CommonTimes
            for (auto cti = sati->second.begin(); cti != sati->second.end();)
            {
                  // and iterate over the list.
               for (auto ndpli = cti->second.begin();
                    ndpli != cti->second.end();)
               {
                  if (((*ndpli)->timeStamp < toTime) &&
                      ((*ndpli)->timeStamp >= fromTime))
                  {
                     ndpli = cti->second.erase(ndpli);
                  }
                  else
                  {
                     ++ndpli;
                  }
               }
                  // clean out empty maps
               if (cti->second.empty())
               {
                  cti = sati->second.erase(cti);
               }
               else
               {
                  ++cti;
               }
            }
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
            mti = nearestData.erase(mti);
         }
         else
         {
            ++mti;
         }
      }
         // edit time offset storage
         // iterate over TimeCvtKey
      for (auto ocmi = offsetData.begin(); ocmi != offsetData.end();)
      {
            // iterate over CommonTimes
         for (auto cti = ocmi->second.begin(); cti != ocmi->second.end();)
         {
               // iterate over NavSatelliteIDs
            for (auto sati = cti->second.begin(); sati != cti->second.end();)
            {
               if (sati->first != satID)
               {
                     // not a match
                  ++sati;
                  continue;
               }
               if ((sati->second->timeStamp < toTime) &&
                   (sati->second->timeStamp >= fromTime))
               {
                  sati = cti->second.erase(sati);
               }
               else
               {
                  ++sati;
               }
            }
               // clean out empty maps
            if (cti->second.empty())
            {
               cti = ocmi->second.erase(cti);
            }
            else
            {
               ++cti;
            }
         }
            // clean out empty maps
         if (ocmi->second.empty())
         {
            ocmi = offsetData.erase(ocmi);
         }
         else
         {
            ++ocmi;
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
      nearestData.clear();
      offsetData.clear();
   }


   bool NavDataFactoryWithStore ::
   addNavData(const NavDataPtr& nd)
   {
      OrbitDataKepler *odkp = nullptr;
      TimeOffsetData *todp = nullptr;
      if (debugLevel)
      {
         std::cerr << "addNavData user = " << nd->getUserTime()
                   << "  nearest = " << nd->getNearTime() << std::endl;
      }
      if ((odkp = dynamic_cast<OrbitDataKepler*>(nd.get())) != nullptr)
      {
         if (((initialTime.getTimeSystem() != odkp->beginFit.getTimeSystem()) &&
              (initialTime.getTimeSystem() != TimeSystem::Any)) ||
             ((finalTime.getTimeSystem() != odkp->endFit.getTimeSystem()) &&
              (finalTime.getTimeSystem() != TimeSystem::Any)))
         {
               // different time systems, convert to UTC first.
            CommonTime t0(initialTime), t1(finalTime), f0(odkp->beginFit),
               f1(odkp->endFit);
            BasicTimeSystemConverter btsc;
            if ((t0.getTimeSystem() != TimeSystem::Any) &&
                !t0.changeTimeSystem(TimeSystem::UTC, &btsc))
            {
               return false;
            }
            if ((t1.getTimeSystem() != TimeSystem::Any) &&
                !t1.changeTimeSystem(TimeSystem::UTC, &btsc))
            {
               return false;
            }
            if ((f0.getTimeSystem() != TimeSystem::Any) &&
                !f0.changeTimeSystem(TimeSystem::UTC, &btsc))
            {
               return false;
            }
            if ((f1.getTimeSystem() != TimeSystem::Any) &&
                !f1.changeTimeSystem(TimeSystem::UTC, &btsc))
            {
               return false;
            }
               // Compare UTC times, but set initialTime/finalTime to
               // original time system
            if (f0 < t0)
               initialTime = odkp->beginFit;
            if (f1 > t1)
               finalTime = odkp->endFit;
         }
         else
         {
            initialTime = std::min(initialTime, odkp->beginFit);
            finalTime = std::max(finalTime, odkp->endFit);
         }
      }
      if ((todp = dynamic_cast<TimeOffsetData*>(nd.get())) == nullptr)
      {
            // everything BUT TimeOffsetData
         data[nd->signal.messageType][nd->signal][nd->getUserTime()] = nd;
         nearestData[nd->signal.messageType][nd->signal][nd->getNearTime()]
            .push_back(nd);
         return true;
      }
         // TimeOffsetData
      TimeCvtSet conversions = todp->getConversions();
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
                 SVHealth xmitHealth,
                 const CommonTime& when)
   {
         // We can't check the validity of an invalid iterator, BUT we
         // have to say it's valid because otherwise the while loop in
         // find() breaks.
      if (ti == nm.end())
      {
         return true;
      }
      return validityCheck(ti->second, valid, xmitHealth, when);
   }


   bool NavDataFactoryWithStore ::
   validityCheck(const NavDataPtr& ndp,
                 NavValidityType valid,
                 SVHealth xmitHealth,
                 const CommonTime& when)
   {
      bool rv = true;
         // One last check for fit interval validity, but it only
         // applies to OrbitDataKepler.
      OrbitDataKepler *odk = dynamic_cast<OrbitDataKepler*>(ndp.get());
      if (odk != nullptr)
      {
         if ((when < odk->beginFit) || (when > odk->endFit))
         {
            return false;
         }
      }
      switch (valid)
      {
         case NavValidityType::ValidOnly:
            rv = ndp->validate();
            break;
         case NavValidityType::InvalidOnly:
            rv = !ndp->validate();
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
      if (ndp->signal.messageType == gnsstk::NavMessageType::Health)
      {
         return rv;
      }
      return matchHealth(ndp.get(), xmitHealth);
   }


   bool NavDataFactoryWithStore ::
   matchHealth(NavData *ndp, SVHealth xmitHealth)
   {
      bool rv = true;
         // Set to true if the health status matched.  If it remains
         // false, we have to do a look up of the health status of the
         // transmitting satellite because the presumed matched data
         // doesn't contain the health status of the transmitting
         // satellite.
      bool rvSet = false;
      OrbitDataKepler *orb;
      NavHealthData *hea;
      switch (xmitHealth)
      {
         case SVHealth::Healthy:
         case SVHealth::Unhealthy:
         case SVHealth::Degraded:
            if (debugLevel)
            {
               std::cerr << "  attempting to match "
                         << gnsstk::StringUtils::asString(xmitHealth)
                         << std::endl;
            }
               // make sure the health status is the desired state
            if (ndp->signal.sat == ndp->signal.xmitSat)
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
               if (debugLevel)
               {
                  std::cerr << "  looking up health data" << std::endl;
               }
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
               NavMessageID nmid = ndp->signal;
               nmid.messageType = NavMessageType::Health;
                  /** @todo This next statement kind of sort of
                   * enforces ephemeris health however it's possible,
                   * for example in GPS LNAV to have almanac pages
                   * with the transmit and subject satellites the
                   * same, making it effectively indistinguishible
                   * from ephemeris health.  Are there situations
                   * where that could yield incorrect results? */
               nmid.sat = nmid.xmitSat;
               if (!find(nmid, ndp->timeStamp, heaPtr,
                         SVHealth::Any, NavValidityType::Any,
                         NavSearchOrder::User))
               {
                  if (debugLevel)
                  {
                     std::cerr << "  couldn't find health" << std::endl;
                  }
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


   NavSatelliteIDSet NavDataFactoryWithStore :: getAvailableSats(
      const CommonTime& fromTime, const CommonTime& toTime)
      const
   {
      NavSatelliteIDSet rv;
      NavMessageIDSet tmp = getAvailableMsgs(fromTime, toTime);
         // copy the NavMessageID objects into the return value which
         // will cast the NavMessageID to NavSatelliteID.
      for (const auto& i : tmp)
         rv.insert(i);
      return rv;
   }


   NavSatelliteIDSet NavDataFactoryWithStore :: getAvailableSats(
      NavMessageType nmt, const CommonTime& fromTime, const CommonTime& toTime)
      const
   {
      NavSatelliteIDSet rv;
      auto nmmi = data.find(nmt);
      if (nmmi != data.end())
      {
         for (const auto& nsmi : nmmi->second)
         {
            auto ti1 = nsmi.second.lower_bound(fromTime);
            if ((ti1 != nsmi.second.end()) && (ti1->first < toTime))
            {
               rv.insert(nsmi.first);
            }
         }
      }
      return rv;
   }


   NavMessageIDSet NavDataFactoryWithStore :: getAvailableMsgs(
      const CommonTime& fromTime, const CommonTime& toTime)
      const
   {
      NavMessageIDSet rv;
      for (const auto& nmmi : data)
      {
         for (const auto& nsmi : nmmi.second)
         {
            auto ti1 = nsmi.second.lower_bound(fromTime);
            if ((ti1 != nsmi.second.end()) && (ti1->first < toTime))
            {
               rv.insert(NavMessageID(nsmi.first, nmmi.first));
            }
         }
      }
      return rv;
   }


   void NavDataFactoryWithStore ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      if (debugLevel)
      {
         std::cerr << "data.size() = " << data.size() << std::endl;
      }
      for (const auto& nmmi : data)
      {
         for (const auto& nsami : nmmi.second)
         {
            switch (dl)
            {
               case DumpDetail::OneLine:
                  s << StringUtils::asString(nmmi.first) << " "
                    << StringUtils::asString(nsami.first) << std::endl;
                  break;
               case DumpDetail::Brief:
                  for (const auto& cti : nsami.second)
                  {
                     s << StringUtils::asString(nmmi.first) << " "
                       << StringUtils::asString(nsami.first) << "   "
                       << printTime(cti.first, "%Y %2m %2d %02H:%02M:%04.1f")
                       << std::endl;
                  }
                  break;
               case DumpDetail::Full:
                  for (const auto& cti : nsami.second)
                  {
                     cti.second->dump(s,dl);
                  }
                  break;
            }
         }
      }
      if (debugLevel)
      {
         std::cerr << "offsetData.size() = " << offsetData.size() << std::endl;
      }
         // time offset data is a separate map, but still needs to be dumped.
      std::string label = StringUtils::asString(NavMessageType::TimeOffset);
      for (const auto& ocmi : offsetData)
      {
         switch (dl)
         {
            case DumpDetail::OneLine:
               s << label << " " << ocmi.first.first << " -> "
                 << ocmi.first.second << std::endl;
               break;
            case DumpDetail::Brief:
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
            case DumpDetail::Full:
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
