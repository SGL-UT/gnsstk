#include "PNBNavDataFactory.hpp"

using namespace std;

namespace gpstk
{
   PNBNavDataFactory ::
   PNBNavDataFactory()
         : navValidity(NavValidityType::Any)
   {
      setTypeFilter(allNavMessageTypes);
   }


   void PNBNavDataFactory ::
   setTypeFilter(const NavMessageTypeSet& nmts)
   {
         // We use boolean values instead of a set so that we're not
         // checking a set every time a new subframe is added.
      processEph = (nmts.count(gpstk::NavMessageType::Ephemeris) > 0);
      processAlm = (nmts.count(gpstk::NavMessageType::Almanac) > 0);
      processHea = (nmts.count(gpstk::NavMessageType::Health) > 0);
      processTim = (nmts.count(gpstk::NavMessageType::TimeOffset) > 0);
   }

}
