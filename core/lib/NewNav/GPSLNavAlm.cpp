#include "GPSLNavAlm.hpp"

using namespace std;

namespace gpstk
{
   GPSLNavAlm ::
   GPSLNavAlm()
         : healthBits(0xff)
   {
      signal.messageType = NavMessageType::Almanac;
   }


   bool GPSLNavAlm ::
   validate() const
   {
      return GPSLNavData::validate();
   }


   CommonTime GPSLNavAlm ::
   getUserTime() const
   {
      return timeStamp + 6.0;
   }


   void GPSLNavAlm ::
   fixFit()
   {
         // Set the fit times to toa-70h through toa+74 hours.  This
         // is an estimate based on IS-GPS-200 Table 20-XIII.
      beginFit = Toe - (70.0 * 3600.0);
      endFit   = Toe + (74.0 * 3600.0);
   }
}
