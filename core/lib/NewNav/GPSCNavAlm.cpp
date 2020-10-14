#include "GPSCNavAlm.hpp"

using namespace std;

namespace gpstk
{
   GPSCNavAlm ::
   GPSCNavAlm()
         : healthL1(true),
           healthL2(true),
           healthL5(true),
           deltai(0),
           wna(0),
           toa(0)
   {
      signal.messageType = NavMessageType::Almanac;
   }


   bool GPSCNavAlm ::
   validate() const
   {
      return GPSCNavData::validate();
   }


   CommonTime GPSCNavAlm ::
   getUserTime() const
   {
      return timeStamp + 12.0;
   }


   void GPSCNavAlm ::
   fixFit()
   {
         /** @todo Determine a more reasonable set of values.  This
          * was copied from OrbAlmExt. */
      beginFit = xmitTime;
      endFit   = gpstk::CommonTime::END_OF_TIME;
      endFit.setTimeSystem(beginFit.getTimeSystem());
   }
}
