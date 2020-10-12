#include "GPSLNavHealth.hpp"

using namespace std;

namespace gpstk
{
   void GPSLNavHealth ::
   dump(std::ostream& s, Detail dl) const
   {
      NavData::dump(s,dl);
      if (dl == Detail::OneLine)
      {
         return;
      }
      s << "svHealth = " << hex << (unsigned)svHealth << dec << "  "
        << StringUtils::asString(getHealth()) << endl;
   }
}
