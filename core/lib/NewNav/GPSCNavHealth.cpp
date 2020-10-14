#include "GPSCNavHealth.hpp"

using namespace std;

namespace gpstk
{
   CommonTime GPSCNavHealth ::
   getUserTime() const
   {
      if (signal.nav == NavType::GPSCNAVL2)
         return timeStamp + 12.0;
      return timeStamp + 6.0;
   }


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
