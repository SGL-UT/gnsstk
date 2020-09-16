#include "NavLibrary.hpp"
#include "OrbitData.hpp"
#include "NavHealthData.hpp"
#include "TimeOffsetData.hpp"

namespace gpstk
{
   bool NavLibrary ::
   getXvt(const NavSatelliteID& sat, const CommonTime& when, Xvt& xvt,
          bool useAlm, NavValidityType valid, NavSearchOrder order)
   {
      NavMessageID nmid(sat, useAlm ? NavMessageType::Almanac :
                        NavMessageType::Ephemeris);
      NavDataPtr ndp;
      if (!find(nmid, when, ndp, valid, order))
         return false;
      OrbitData *orb = dynamic_cast<OrbitData*>(ndp.get());
      return orb->getXvt(when, xvt);
   }


   bool NavLibrary ::
   getXvt(const NavSatelliteID& sat, const CommonTime& when, Xvt& xvt,
          NavValidityType valid, NavSearchOrder order)
   {
      NavMessageID nmid(sat, NavMessageType::Ephemeris);
      NavDataPtr ndp;
      if (!find(nmid, when, ndp, valid, order))
      {
         NavMessageID nmida(sat, NavMessageType::Almanac);
         if (!find(nmida, when, ndp, valid, order))
         {
            return false;
         }
      }
      OrbitData *orb = dynamic_cast<OrbitData*>(ndp.get());
      return orb->getXvt(when, xvt);
   }


   bool NavLibrary ::
   getHealth(const NavSatelliteID& sat, const CommonTime& when,
             SVHealth& health, NavValidityType valid, NavSearchOrder order)
   {
      NavMessageID nmid(sat, NavMessageType::Health);
      NavDataPtr ndp;
      if (!find(nmid, when, ndp, valid, order))
         return false;
      NavHealthData *healthData = dynamic_cast<NavHealthData*>(ndp.get());
      health = healthData->getHealth();
      return true;
   }


   bool NavLibrary ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, double& offset,
             NavValidityType valid, NavSearchOrder order)
   {
         // gotta figure out how to search for data without a sat.
      // NavMessageID nmid(sat, NavMessageType::TimeOffset);
      // NavDataPtr ndp;
      // if (!find(nmid, when, ndp, valid, order))
      //    return false;
      // TimeOffsetData *timeOffsetData = dynamic_cast<TimeOffsetData*>(ndp.get());
      // return timeOffsetData->getOffset(fromSys, toSys, when, offset);
      return false;
   }


   bool NavLibrary ::
   find(const NavMessageID& nmid, const CommonTime& when,
        NavDataPtr& navData, NavValidityType valid,
        NavSearchOrder order)
   {
         // search factories until we find what we want.
      auto range = factories.equal_range(nmid);
      for (auto fi = range.first; fi != range.second; ++fi)
      {
         if (fi->second->find(nmid, when, navData, valid, order))
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
}
