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
         // The OrbitDataSP3 object is generated on request and is
         // specific to a given time since the data that must be used
         // for interpolation is only available in the factory store.
         // As such, we refuse to generate an XVT for anything other
         // than the time that this OrbitDataSP3 object represents.
      if (when != timeStamp)
         return false;
      xvt.x[0] = pos[0];
      xvt.x[1] = pos[1];
      xvt.x[2] = pos[2];
      xvt.v[0] = vel[0];
      xvt.v[1] = vel[1];
      xvt.v[2] = vel[2];
      xvt.clkbias = clkBias * 1e-6; // microseconds to seconds
      xvt.clkdrift = clkDrift;
      xvt.health = Xvt::HealthStatus::Unavailable;
      xvt.computeRelativityCorrection();
         /// @todo set xvt.frame
      return true;
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
