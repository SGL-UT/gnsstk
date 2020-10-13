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
         // Set the fit times to toa-70h through toa+74 hours.  This
         // is an estimate based on IS-GPS-200 Table 20-XIII.
      beginFit = Toe - (70.0 * 3600.0);
      endFit   = Toe + (74.0 * 3600.0);
   }
}
