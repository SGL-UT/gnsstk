#include "GPSCNavData.hpp"

namespace gpstk
{
   GPSCNavData ::
   GPSCNavData()
         : pre(0),
           alert(false)
   {}


   bool GPSCNavData ::
   validate() const
   {
      return ((pre == 0) || (pre == 0x8b));
   }
}
