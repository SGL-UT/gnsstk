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
#include <iterator>
#include "NavDataFactoryWithStore.hpp"
#include "TimeString.hpp"
#include "OrbitDataKepler.hpp"
#include "NavHealthData.hpp"
#include "BasicTimeSystemConverter.hpp"
#include "DebugTrace.hpp"

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
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("class: " << getClassName());
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
      typedef std::list<FindMatches> MatchList;

      DEBUGTRACE("nmid=" << nmid << "  when=" << gnsstk::printTime(when,dts));

         // dig through the maps of maps, matching keys with nmid along the way
      auto dataIt = data.find(nmid.messageType);
      if (dataIt == data.end())
      {
         DEBUGTRACE("false = not found 1");
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
         DEBUGTRACE("wildcard search: " << nmid);
         for (NavSatMap::iterator sati = dataIt->second.begin();
              sati != dataIt->second.end(); sati++)
         {
            if (sati->first != nmid)
            {
               DEBUGTRACE(sati->first << " != " << nmid);
               continue; // skip non matches
            }
            DEBUGTRACE("matches " << sati->first);
            NavMap::iterator nmi = sati->second.lower_bound(when);
            if (nmi == sati->second.end())
            {
               nmi = std::prev(nmi);
            }
            DEBUGTRACE("user time : "
                       << gnsstk::printTime(nmi->second->getUserTime(),dts));
            DEBUGTRACE("(nmi != sati->second.end()) = "
                       << (nmi != sati->second.end()));
            DEBUGTRACE("(nmi->second->getUserTime() > when) = "
                       << (nmi->second->getUserTime() > when));
            while ((nmi != sati->second.end()) &&
                   (nmi->second->getUserTime() > when))
            {
               DEBUGTRACE("backing up (maybe)");
               nmi = (nmi == sati->second.begin() ? sati->second.end()
                      : std::prev(nmi));
               if (nmi != sati->second.end())
               {
                  DEBUGTRACE("user time : "
                             << gnsstk::printTime(nmi->second->getUserTime(),
                                                  dts));
               }
            }
            if (nmi != sati->second.end())
            {
               itList.push_back(FindMatches(&(sati->second), nmi));
            }
            else
            {
               DEBUGTRACE("did not add match");
            }
         }
      }
      else
      {
         DEBUGTRACE("non-wildcard search: " << nmid);
         auto sati = dataIt->second.find(nmid);
         if (sati != dataIt->second.end())
         {
            DEBUGTRACE("found");
            NavMap::iterator nmi = sati->second.lower_bound(when);
            if (nmi == sati->second.end())
            {
               nmi = std::prev(nmi);
            }
            DEBUGTRACE("user time : "
                       << gnsstk::printTime(nmi->second->getUserTime(),dts));
            while ((nmi != sati->second.end()) &&
                   (nmi->second->getUserTime() > when))
            {
               nmi = (nmi == sati->second.begin() ? sati->second.end()
                      : std::prev(nmi));
               if (nmi != sati->second.end())
               {
                  DEBUGTRACE("user time : "
                             << gnsstk::printTime(nmi->second->getUserTime(),
                                                  dts));
               }
            }
            if (nmi != sati->second.end())
            {
               itList.push_back(FindMatches(&(sati->second), nmi));
            }
            else
            {
               DEBUGTRACE("did not add match");
            }
         }
         else
         {
            DEBUGTRACE("not found");
         }
      }
      DEBUGTRACE("itList.size() = " << itList.size());
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
               DEBUGTRACE("not end, not right time");
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
               DEBUGTRACE("Found something good at "
                          << printTime(imi.it->first, dts));
               if (imi.it->second->getUserTime() > mostRecent)
               {
                  mostRecent = imi.it->second->getUserTime();
                  navData = imi.it->second;
                  DEBUGTRACE("result is now " << navData->signal);
               }
               imi.finished = true;
               rv = true;
            }
         }
      }
      DEBUGTRACE("Most recent = " << printTime(mostRecent, dts));
      return rv;
   }


   bool NavDataFactoryWithStore ::
   findNearest(const NavMessageID& nmid, const CommonTime& when,
               NavDataPtr& navData, SVHealth xmitHealth,
               NavValidityType valid)
   {
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("class: " << getClassName());
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
      typedef std::list<FindMatches> MatchList;

         // dig through the maps of maps, matching keys with nmid along the way
      auto dataIt = nearestData.find(nmid.messageType);
      if (dataIt == nearestData.end())
      {
         DEBUGTRACE(" false = not found 1");
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
         DEBUGTRACE("wildcard search: " << nmid);
         for (NavNearSatMap::iterator sati = dataIt->second.begin();
              sati != dataIt->second.end(); sati++)
         {
            if (sati->first != nmid)
               continue; // skip non matches
            DEBUGTRACE("matches " << sati->first);
            DEBUGTRACE("when = " << gnsstk::printTime(when,dts));
            NavNearMap::iterator nmi = sati->second.lower_bound(when);
            itList.push_back(FindMatches(&(sati->second), nmi, when));
         }
      }
      else
      {
         DEBUGTRACE("non-wildcard search: " << nmid);
         auto sati = dataIt->second.find(nmid);
         if (sati != dataIt->second.end())
         {
            DEBUGTRACE("found");
            NavNearMap::iterator nmi = sati->second.lower_bound(when);
            itList.push_back(FindMatches(&(sati->second), nmi, when));
         }
         else
         {
            DEBUGTRACE("not found");
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
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("class: " << getClassName());
      DEBUGTRACE(printTime(when,"looking for "+dts));
      bool rv = false;
         // Only search for forward key and let the TimeOffset classes
         // and factories handle the reverse offset.
      TimeCvtKey fwdKey(fromSys,toSys);
         // First look in the offsetData map for the key matching the
         // offset translation in the forward direction (fromSys->toSys).
      auto odi = offsetData.find(fwdKey);
      DEBUGTRACE("fwdKey=<" << gnsstk::StringUtils::asString(fwdKey.first)
                 << "," << gnsstk::StringUtils::asString(fwdKey.second) << ">");
      if (odi == offsetData.end())
      {
         if (DebugTrace::enabled)
         {
            DEBUGTRACE("did not find key, giving up");
            DEBUGTRACE("offsetData.size() = " << offsetData.size());
            for (const auto& x : offsetData)
            {
               DEBUGTRACE("fwdKey=<"
                          << gnsstk::StringUtils::asString(x.first.first) << ","
                          << gnsstk::StringUtils::asString(x.first.second)
                          << ">");
            }
         }
         return false; // no conversion available
      }
      else
      {
         DEBUGTRACE("found forward key");
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
         DEBUGTRACE("got end right away, backing up one");
         oemi = std::prev(oemi);
      }
      DEBUGTRACE(printTime(oemi->first,"found "+dts));
      bool done = false;
      while (!done)
      {
         if (oemi == odi->second.end())
         {
               // give up, couldn't find any valid matches.
            DEBUGTRACE("giving up, reached the end");
            done = true;
         }
         else if (oemi->first > whenny)
         {
               // time of data is after the requested time of interest
               // so back up if possible
            oemi = (oemi == odi->second.begin()
                    ? odi->second.end() : std::prev(oemi));
            DEBUGTRACE(printTime(oemi->first,"backed up to "+dts));
         }
         else
         {
            DEBUGTRACE("looking for acceptable data "
                       << gnsstk::StringUtils::asString(valid) << " "
                       << gnsstk::StringUtils::asString(xmitHealth));
               // Message time is valid, so iterate through the
               // per-signal data for a usable record (matching
               // validity and transmit satellite health)
            for (const auto& omi : oemi->second)
            {
               TimeOffsetData *todp = dynamic_cast<TimeOffsetData*>(
                  omi.second.get());
               DEBUGTRACE("checking " << todp->signal);
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
            DEBUGTRACE("didn't find any acceptable data");
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
      initialTime = gnsstk::CommonTime::END_OF_TIME;
      finalTime = gnsstk::CommonTime::BEGINNING_OF_TIME;
   }


   bool NavDataFactoryWithStore ::
   addNavData(const NavDataPtr& nd, NavMessageMap& navMap,
              NavNearMessageMap& navNearMap, OffsetCvtMap& ofsMap)
   {
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("class: " << getClassName());
      OrbitDataKepler *odkp = nullptr;
      OrbitData *odp = nullptr;
      TimeOffsetData *todp = nullptr;
      DEBUGTRACE("addNavData user = " << nd->getUserTime()
                 << "  nearest = " << nd->getNearTime());
      SatID satID = nd->signal.sat;
         // TimeOffset data doesn't have an associated satellite, so
         // ignore those to avoid time system conflicts in this block
         // of code.
      if (satID.system != SatelliteSystem::Unknown)
      {
         if (firstLastMap.find(satID) == firstLastMap.end())
         {
            firstLastMap[satID] = std::pair<CommonTime,CommonTime>(
               nd->timeStamp,nd->timeStamp);
         }
         else
         {
               // Ignore time systems when comparing, because I'm
               // lazy.  The few seconds difference in time systems
               // isn't going to have a big effect on this information
               // anyway.
            CommonTime anyFirst(firstLastMap[satID].first),
               anyLast(firstLastMap[satID].second),
               anyTimeStamp(nd->timeStamp);
            anyFirst.setTimeSystem(TimeSystem::Any);
            anyLast.setTimeSystem(TimeSystem::Any);
            anyTimeStamp.setTimeSystem(TimeSystem::Any);
               // set the stored time stamps using the original time system.
            if (anyTimeStamp < anyFirst)
               firstLastMap[satID].first = nd->timeStamp;
            if (anyTimeStamp > anyLast)
               firstLastMap[satID].second = nd->timeStamp;
         }
      }
      if ((odkp = dynamic_cast<OrbitDataKepler*>(nd.get())) != nullptr)
      {
         if (!updateInitialFinal(odkp->beginFit, odkp->endFit))
            return false;
      }
      else if ((odp = dynamic_cast<OrbitData*>(nd.get())) != nullptr)
      {
            // Non-Keplerian orbit data. Tabular, usually.  Use the
            // reference time to update initial/final time.
         if (!updateInitialFinal(odp->timeStamp,odp->timeStamp))
            return false;
      }
         // always add to navMap/navNearMap
      navMap[nd->signal.messageType][nd->signal][nd->getUserTime()] = nd;
      navNearMap[nd->signal.messageType][nd->signal][nd->getNearTime()]
         .push_back(nd);
         // TimeOffsetData has its own special map for look-up.
      if ((todp = dynamic_cast<TimeOffsetData*>(nd.get())) != nullptr)
      {
         TimeCvtSet conversions = todp->getConversions();
         for (const auto& ci : conversions)
         {
            ofsMap[ci][nd->getUserTime()][nd->signal] = nd;
         }
      }
      return true;
   }


   bool NavDataFactoryWithStore ::
   updateInitialFinal(const CommonTime& begin, const CommonTime& end)
   {
      if (((initialTime.getTimeSystem() != begin.getTimeSystem()) &&
           (initialTime.getTimeSystem() != TimeSystem::Any)) ||
          ((finalTime.getTimeSystem() != end.getTimeSystem()) &&
           (finalTime.getTimeSystem() != TimeSystem::Any)))
      {
            // different time systems, convert to UTC first.
         CommonTime t0(initialTime), t1(finalTime), f0(begin),
            f1(end);
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
            initialTime = begin;
         if (f1 > t1)
            finalTime = end;
      }
      else
      {
         initialTime = std::min(initialTime, begin);
         finalTime = std::max(finalTime, end);
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
         // OffsetCvtMap doesn't need to be counted because the data
         // is now also being stored in the data above.
      return rv;
   }


   size_t NavDataFactoryWithStore ::
   count(const NavMessageID& nmid) const
   {
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("class: " << getClassName());
      size_t rv = 0;
         // Make a copy of the key that can be modified so that values
         // that are otherwise not wildcards e.g. SatelliteSystem can
         // be managed like wildcards.
      NavMessageID key(nmid);
         // Iterate over all nav message types.  Skip anything that
         // doesn't match, treating "Unknown" as a wildcard.
      for (NavMessageType i : NavMessageTypeIterator())
      {
         if ((nmid.messageType != NavMessageType::Unknown) &&
             (nmid.messageType != i))
         {
            DEBUGTRACE("skipping message type " << StringUtils::asString(i));
            continue;
         }
         DEBUGTRACE("counting message type " << StringUtils::asString(i));
         auto dataIt = data.find(i);
         if (dataIt == data.end())
         {
               // no data for this message type, move on
            continue;
         }
            // There are no non-wildcard searches because we treat
            // SatelliteSystem::Unknown as a wildcard when it normally
            // is not.
         DEBUGTRACE("wildcard search: " << nmid);
         for (const auto& sati : dataIt->second)
         {
               // treat the SatelliteSystem::Unknown like a wildcard
            if (nmid.system == SatelliteSystem::Unknown)
            {
               key.system = sati.first.system;
            }
            if (sati.first == key)
            {
               DEBUGTRACE("matches " << sati.second.size() << " x "
                          << sati.first);
               rv += sati.second.size();
            }
            else
            {
               DEBUGTRACE(sati.first << " != " << nmid);
            }
         }
      }
      return rv;
   }


   size_t NavDataFactoryWithStore ::
   count(SatelliteSystem sys, NavMessageType nmt) const
   {
      NavMessageID key(
         NavSatelliteID(gnsstk::SatID(gnsstk::SatelliteSystem::Unknown),
                        gnsstk::SatID(gnsstk::SatelliteSystem::Unknown),
                        gnsstk::ObsID(gnsstk::ObservationType::Any,
                                      gnsstk::CarrierBand::Any,
                                      gnsstk::TrackingCode::Any,
                                      gnsstk::XmitAnt::Any),
                        gnsstk::NavID(gnsstk::NavType::Any)),
         nmt);
      key.sat.makeWild();
      key.xmitSat.makeWild();
      key.system = sys;
      return count(key);
   }


   size_t NavDataFactoryWithStore ::
   count(const SatID& satID, NavMessageType nmt) const
   {
      NavMessageID key(
         NavSatelliteID(satID,
                        gnsstk::SatID(gnsstk::SatelliteSystem::Unknown),
                        gnsstk::ObsID(gnsstk::ObservationType::Any,
                                      gnsstk::CarrierBand::Any,
                                      gnsstk::TrackingCode::Any,
                                      gnsstk::XmitAnt::Any),
                        gnsstk::NavID(gnsstk::NavType::Any)),
         nmt);
      key.xmitSat.makeWild();
      return count(key);
   }


   size_t NavDataFactoryWithStore ::
   count(NavMessageType nmt) const
   {
      NavMessageID key(
         NavSatelliteID(gnsstk::SatID(gnsstk::SatelliteSystem::Unknown),
                        gnsstk::SatID(gnsstk::SatelliteSystem::Unknown),
                        gnsstk::ObsID(gnsstk::ObservationType::Any,
                                      gnsstk::CarrierBand::Any,
                                      gnsstk::TrackingCode::Any,
                                      gnsstk::XmitAnt::Any),
                        gnsstk::NavID(gnsstk::NavType::Any)),
         nmt);
      key.sat.makeWild();
      key.xmitSat.makeWild();
      return count(key);
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
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("class: " << getClassName());
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
            DEBUGTRACE("attempting to match "
                       << gnsstk::StringUtils::asString(xmitHealth));
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
               DEBUGTRACE("looking up health data");
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
                  DEBUGTRACE("  couldn't find health");
                  return false;
               }
               hea = dynamic_cast<NavHealthData*>(heaPtr.get());
               rv = (xmitHealth == hea->getHealth());
               DEBUGTRACE("transmit health "
                          << (rv ? "matches" : "does not match"));
            }
            break;
         default:
               // treat all other cases as valid
            break;
      }
      return rv;
   }


   CommonTime NavDataFactoryWithStore :: getFirstTime(const SatID& sat) const
   {
      auto i = firstLastMap.find(sat);
      if (i != firstLastMap.end())
         return i->second.first;
      return CommonTime::END_OF_TIME;
   }


   CommonTime NavDataFactoryWithStore :: getLastTime(const SatID& sat) const
   {
      auto i = firstLastMap.find(sat);
      if (i != firstLastMap.end())
         return i->second.second;
      return CommonTime::BEGINNING_OF_TIME;
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


   std::set<SatID> NavDataFactoryWithStore ::
   getIndexSet(const CommonTime& fromTime,
               const CommonTime& toTime) const
   {
      std::set<SatID> rv;
      for (const auto& nmmi : data)
      {
         for (const auto& nsmi : nmmi.second)
         {
            auto ti1 = nsmi.second.lower_bound(fromTime);
            if ((ti1 != nsmi.second.end()) && (ti1->first < toTime))
            {
               rv.insert(nsmi.first.sat);
            }
         }
      }
      return rv;
   }


   std::set<SatID> NavDataFactoryWithStore ::
   getIndexSet(NavMessageType nmt,
               const CommonTime& fromTime,
               const CommonTime& toTime) const
   {
      std::set<SatID> rv;
      auto nmmi = data.find(nmt);
      if (nmmi != data.end())
      {
         for (const auto& nsmi : nmmi->second)
         {
            auto ti1 = nsmi.second.lower_bound(fromTime);
            if ((ti1 != nsmi.second.end()) && (ti1->first < toTime))
            {
               rv.insert(nsmi.first.sat);
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


   const NavMap* NavDataFactoryWithStore ::
   getNavMap(const NavMessageID& nmid) const
   {
      auto nmmi = data.find(nmid.messageType);
      if (nmmi != data.end())
      {
         auto nsmi = nmmi->second.find(nmid);
         if (nsmi != nmmi->second.end())
         {
            return &nsmi->second;
         }
      }
      return nullptr;
   }


   void NavDataFactoryWithStore ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("class: " << getClassName());
      DEBUGTRACE("data.size() = " << data.size());
      for (const auto& nmmi : data)
      {
         for (const auto& nsami : nmmi.second)
         {
            switch (dl)
            {
               case DumpDetail::OneLine:
                  s << StringUtils::asString(nmmi.first) << " "
                    << StringUtils::asString(nsami.first) << " "
                    << nsami.second.size() << " objects" << std::endl;
                  break;
               case DumpDetail::Terse:
                     /** @todo To support the variances between nav
                      * codes Terse dump formats, it would probably be
                      * best to implement a "getTerseHeader" method
                      * and just call it for the first object in this
                      * map (which should be the same class for each
                      * item in nsami.second). */
                  s << "  Map for " << StringUtils::asString(nmmi.first)
                    << " " << StringUtils::asString(nsami.first) << " has "
                    << nsami.second.size() << " entries." << std::endl
                    << "SVN  PRN     Begin Fit        Toe          End Fit"
                    << "       URA     IODC      Health" << std::endl;
                  for (const auto& cti : nsami.second)
                  {
                     cti.second->dump(s, dl);
                  }                  
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
   }
}
