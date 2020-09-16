#include "GPSLNavData.hpp"

namespace gpstk
{
   GPSLNavData ::
   GPSLNavData()
         : pre(0),
           tlm(0),
           asFlag(false),
           alert(false)
   {}


   bool GPSLNavData ::
   validate() const
   {
      return ((pre == 0) || (pre == 0x8b));
   }
}
