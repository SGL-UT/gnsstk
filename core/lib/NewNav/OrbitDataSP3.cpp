#include "OrbitDataSP3.hpp"

using namespace std;

namespace gpstk
{
   OrbitDataSP3 ::
   OrbitDataSP3()
         : pos(3, 0.0), vel(3, 0.0), clkBias(0.0), clkDrift(0.0)
   {
      signal.messageType = NavMessageType::Ephemeris;
   }


   bool OrbitDataSP3 ::
   getXvt(const CommonTime& when, Xvt& xvt)
   {
      return false;
   }


   void OrbitDataSP3 ::
   dump(std::ostream& s, Detail dl)
   {
      NavData::dump(s,dl);
      if (dl == Detail::OneLine)
         return;
         // brief and full are the same
      const ios::fmtflags oldFlags = s.flags();
      s << fixed << setprecision(6) << "P: {";
      for (unsigned i = 0; i < pos.size(); i++)
         s << " " << pos[i];
      s << "} bias: " << clkBias << endl
        << "V: {";
      for (unsigned i = 0; i < vel.size(); i++)
         s << " " << vel[i];
      s << "} drift: " << clkDrift << endl;
      s.flags(oldFlags);
   }
}
