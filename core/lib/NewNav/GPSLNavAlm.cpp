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
   }
}
