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
             NavValidityType valid, NavSearchOrder order)
   {
      NavDataPtr timeOffset;
      if (getOffset(fromSys, toSys, when, timeOffset, xmitHealth, valid, order))
      {
         TimeOffsetData *top = dynamic_cast<TimeOffsetData*>(timeOffset.get());
         return top->getOffset(fromSys, toSys, when, offset);
      }
      return false;
   }


   bool NavLibrary ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, NavDataPtr& offset, SVHealth xmitHealth,
             NavValidityType valid, NavSearchOrder order)
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
                                   valid, order))
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
}
