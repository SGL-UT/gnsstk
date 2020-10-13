#include "GPSCNavHealth.hpp"

using namespace std;

namespace gpstk
{
   void GPSCNavHealth ::
   dump(std::ostream& s, Detail dl) const
   {
      NavData::dump(s,dl);
      if (dl == Detail::OneLine)
      {
         return;
      }
      s << "health = " << health << "  "
        << StringUtils::asString(getHealth()) << endl;
   }
}
