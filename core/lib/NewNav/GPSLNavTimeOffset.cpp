#include "GPSLNavTimeOffset.hpp"
#include "GPSWeekSecond.hpp"

namespace gpstk
{
   GPSLNavTimeOffset ::
   GPSLNavTimeOffset()
         : deltatLS(0.0), a0(0.0), a1(0.0), tot(0.0), wnt(0)
   {
   }


   bool GPSLNavTimeOffset ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, double& offset)
   {
      if (((fromSys == TimeSystem::GPS) && (toSys == TimeSystem::UTC)) ||
          ((fromSys == TimeSystem::UTC) && (toSys == TimeSystem::GPS)))
      {
         GPSWeekSecond ws(when);
         offset = deltatLS + a0 + a1*(ws.sow - tot + 604800.0*(ws.week-wnt));
         return true;
      }
      return false;
   }
}

