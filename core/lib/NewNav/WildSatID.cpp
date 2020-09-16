#include "WildSatID.hpp"

namespace gpstk
{
   WildSatID ::
   WildSatID()
         : wildSat(true), wildSys(true)
   {
   }


   WildSatID ::
   WildSatID(int p)
         : SatID(p, SatelliteSystem::Unknown), wildSat(false), wildSys(true)
   {
   }


   WildSatID ::
   WildSatID(SatelliteSystem s)
         : SatID(0, s), wildSat(true), wildSys(false)
   {
   }


   WildSatID ::
   WildSatID(int p, SatelliteSystem s)
         : SatID(p, s), wildSat(false), wildSys(false)
   {
   }


   WildSatID ::
   WildSatID(SatID sid)
         : SatID(sid), wildSat(false), wildSys(false)
   {
   }


   bool WildSatID ::
   operator==(const WildSatID& right) const
   {
      return (wildSat || right.wildSat || (id == right.id)) &&
         (wildSys || right.wildSys || (system == right.system));
   }


   bool WildSatID ::
   operator<(const WildSatID& right) const
   {
         // if either system is a wildcard, the systems are effectively equal
      if (!wildSys && !right.wildSys)
      {
         if (system < right.system)
            return true;
         if (system > right.system)
            return false;
      }
         // if either id is a wildcard, the ids are effectively equal
      if (!wildSat && !right.wildSat)
      {
         if (id < right.id)
            return true;
      }
      return false;
   }
}
