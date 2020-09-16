#include "NavData.hpp"
#include "TimeString.hpp"

namespace gpstk
{
   static const std::string timeFmt("%4Y/%02m/%02d %03j %02H:%02M:%02S");

   void NavData ::
   dump(std::ostream& s, Detail dl)
   {
      s << printTime(timeStamp, timeFmt) << " " << signal << std::endl;
   }
}

