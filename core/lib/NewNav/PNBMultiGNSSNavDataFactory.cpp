#include "PNBMultiGNSSNavDataFactory.hpp"

namespace gpstk
{
   void PNBMultiGNSSNavDataFactory ::
   setValidityFilter(NavValidityType nvt)
   {
      for (auto& fi : factories)
      {
         fi.second->setValidityFilter(nvt);
      }
   }


   void PNBMultiGNSSNavDataFactory ::
   setTypeFilter(const NavMessageTypeSet& nmts)
   {
      for (auto& fi : factories)
      {
         fi.second->setTypeFilter(nmts);
      }
   }


   bool PNBMultiGNSSNavDataFactory ::
   addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
   {
      NavType navType = navIn->getNavID().navType;
      auto fi = factories.find(navType);
      if (fi == factories.end())
      {
            // We don't have a factory for this navigation message type
         return false;
      }
      return fi->second->addData(navIn, navOut);
   }


   bool PNBMultiGNSSNavDataFactory ::
   addFactory(NavType navType, PNBNavDataFactoryPtr& fact)
   {
         // make sure it's a valid class first
      PNBNavDataFactory *ndfp = fact.get();
      if (ndfp == nullptr)
      {
         return false;
      }
      if (dynamic_cast<PNBMultiGNSSNavDataFactory*>(ndfp) != nullptr)
      {
         return false;
      }
      factories[navType] = fact;
      return true;
   }
}
