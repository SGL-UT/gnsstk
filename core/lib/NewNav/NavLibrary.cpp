//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//  
//  This software was developed by Applied Research Laboratories at the 
//  University of Texas at Austin.
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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
#include "NavLibrary.hpp"
#include "OrbitData.hpp"
#include "NavHealthData.hpp"
#include "TimeOffsetData.hpp"

namespace gpstk
{
   bool NavLibrary ::
   getXvt(const NavSatelliteID& sat, const CommonTime& when, Xvt& xvt,
          bool useAlm, SVHealth xmitHealth, NavValidityType valid,
          NavSearchOrder order)
   {
      NavMessageID nmid(sat, useAlm ? NavMessageType::Almanac :
                        NavMessageType::Ephemeris);
      NavDataPtr ndp;
      if (!find(nmid, when, ndp, xmitHealth, valid, order))
         return false;
      OrbitData *orb = dynamic_cast<OrbitData*>(ndp.get());
      return orb->getXvt(when, xvt);
   }


   bool NavLibrary ::
   getXvt(const NavSatelliteID& sat, const CommonTime& when, Xvt& xvt,
          SVHealth xmitHealth, NavValidityType valid, NavSearchOrder order)
   {
      NavMessageID nmid(sat, NavMessageType::Ephemeris);
      NavDataPtr ndp;
      if (!find(nmid, when, ndp, xmitHealth, valid, order))
      {
         NavMessageID nmida(sat, NavMessageType::Almanac);
         if (!find(nmida, when, ndp, xmitHealth, valid, order))
         {
            return false;
         }
      }
      OrbitData *orb = dynamic_cast<OrbitData*>(ndp.get());
      return orb->getXvt(when, xvt);
   }


   bool NavLibrary ::
   getHealth(const NavSatelliteID& sat, const CommonTime& when,
             SVHealth& health, SVHealth xmitHealth, NavValidityType valid,
             NavSearchOrder order)
   {
      NavMessageID nmid(sat, NavMessageType::Health);
      NavDataPtr ndp;
      if (!find(nmid, when, ndp, xmitHealth, valid, order))
         return false;
      NavHealthData *healthData = dynamic_cast<NavHealthData*>(ndp.get());
      health = healthData->getHealth();
      return true;
   }


   bool NavLibrary ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, double& offset, SVHealth xmitHealth,
             NavValidityType valid)
   {
      NavDataPtr timeOffset;
      if (getOffset(fromSys, toSys, when, timeOffset, xmitHealth, valid))
      {
         TimeOffsetData *top = dynamic_cast<TimeOffsetData*>(timeOffset.get());
         return top->getOffset(fromSys, toSys, when, offset);
      }
      return false;
   }


   bool NavLibrary ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, NavDataPtr& offset, SVHealth xmitHealth,
             NavValidityType valid)
   {
         // Search through factories until we get a match or run out
         // of factories.  Use unique pointers to avoid double-searching.
      std::set<NavDataFactory*> uniques;
      for (auto fi = factories.begin(); fi != factories.end(); ++fi)
      {
         NavDataFactory *ndfp = dynamic_cast<NavDataFactory*>(fi->second.get());
         if (uniques.count(ndfp))
            continue; // already processed
         uniques.insert(ndfp);
         if (fi->second->getOffset(fromSys, toSys, when, offset, xmitHealth,
                                   valid))
         {
            return true;
         }
      }
      return false;
   }


   bool NavLibrary ::
   find(const NavMessageID& nmid, const CommonTime& when, NavDataPtr& navData,
        SVHealth xmitHealth, NavValidityType valid, NavSearchOrder order)
   {
         // search factories until we find what we want.
         /** @bug I'm worried this might not work as expected in the
          * context of factories that produce multiple signals and
          * wildcards are used. */
      auto range = factories.equal_range(nmid);
      for (auto fi = range.first; fi != range.second; ++fi)
      {
         if (fi->second->find(nmid, when, navData, xmitHealth, valid, order))
            return true;
      }
      return false;
   }


   void NavLibrary ::
   setValidityFilter(NavValidityType nvt)
   {
      for (auto& i : factories)
      {
         i.second->setValidityFilter(nvt);
      }
   }


   void NavLibrary ::
   setTypeFilter(const NavMessageTypeSet& nmts)
   {
      for (auto& i : factories)
      {
         i.second->setTypeFilter(nmts);
      }
   }


   void NavLibrary ::
   addFactory(NavDataFactoryPtr& fact)
   {
         // Yes, we do add multiple copies of the NavDataFactoryPtr to
         // the map, it's a convenience.
      for (const auto& si : fact->supportedSignals)
      {
         factories.insert(NavDataFactoryMap::value_type(si,fact));
      }
   }


   void NavLibrary ::
   dump(std::ostream& s, NavData::Detail dl) const
   {
         // factories can have multiple copies of a given factory, so
         // keep track of which ones we've checked already.
      std::set<NavDataFactory*> ptrs;
      for (auto& fi : factories)
      {
         NavDataFactory *ptr = fi.second.get();
         if (ptrs.count(ptr) == 0)
         {
            ptrs.insert(ptr);
            ptr->dump(s,dl);
         }
      }
   }


   void NavLibrary ::
   edit(const CommonTime& fromTime, const CommonTime& toTime)
   {
      for (auto& fi : factories)
      {
         fi.second->edit(fromTime, toTime);
      }
   }


   void NavLibrary ::
   edit(const CommonTime& fromTime, const CommonTime& toTime,
        const NavSatelliteID& satID)
   {
      for (auto& fi : factories)
      {
         fi.second->edit(fromTime, toTime, satID);
      }
   }


   void NavLibrary ::
   edit(const CommonTime& fromTime, const CommonTime& toTime,
        const NavSignalID& signal)
   {
      for (auto& fi : factories)
      {
         fi.second->edit(fromTime, toTime, signal);
      }
   }


   void NavLibrary ::
   clear()
   {
      for (auto& fi : factories)
      {
         fi.second->clear();
      }
   }


   CommonTime NavLibrary ::
   getInitialTime() const
   {
      CommonTime rv = CommonTime::END_OF_TIME;
      for (const auto& fi : factories)
      {
         rv = std::min(rv, fi.second->getInitialTime());
      }
      return rv;
   }


   CommonTime NavLibrary ::
   getFinalTime() const
   {
      CommonTime rv = CommonTime::BEGINNING_OF_TIME;
      for (const auto& fi : factories)
      {
         rv = std::max(rv, fi.second->getInitialTime());
      }
      return rv;
   }


   std::string NavLibrary ::
   getFactoryFormats() const
   {
         // factories can have multiple copies of a given factory, so
         // keep track of which ones we've checked already.
      std::set<NavDataFactory*> ptrs;
      std::string rv;
      for (const auto& fi : factories)
      {
         NavDataFactory *ptr = fi.second.get();
         if (ptrs.count(ptr) == 0)
         {
            ptrs.insert(ptr);
            if (!rv.empty())
               rv += ", ";
            rv += ptr->getFactoryFormats();
         }
      }
      return rv;
   }
}
