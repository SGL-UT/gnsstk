#include "OrbitDataSP3.hpp"

using namespace std;

namespace gpstk
{
   OrbitDataSP3 ::
   OrbitDataSP3()
         : pos(0.0, 0.0, 0.0), posSig(0.0, 0.0, 0.0),
           vel(0.0, 0.0, 0.0), velSig(0.0, 0.0, 0.0),
           acc(0.0, 0.0, 0.0), accSig(0.0, 0.0, 0.0),
           clkBias(0.0), biasSig(0.0), clkDrift(0.0), driftSig(0.0),
           clkDrRate(0.0), drRateSig(0.0)
   {
      signal.messageType = NavMessageType::Ephemeris;
   }


   void OrbitDataSP3 ::
   copyXV(const OrbitDataSP3& right)
   {
      pos = right.pos;
      posSig = right.posSig;
      vel = right.vel;
      velSig = right.velSig;
      acc = right.acc;
      accSig = right.accSig;
   }


   void OrbitDataSP3 ::
   copyT(const OrbitDataSP3& right)
   {
      clkBias = right.clkBias;
      biasSig = right.biasSig;
      clkDrift = right.clkDrift;
      driftSig = right.driftSig;
      clkDrRate = right.clkDrRate;
      drRateSig = right.drRateSig;
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
      for (unsigned i = 0; i < 3; i++)
      {
         xvt.x[i] = pos[i] * 1000.0;
         xvt.v[i] = vel[i] * 0.1;
      }
      xvt.clkbias = clkBias * 1e-6; // microseconds to seconds
      xvt.clkdrift = clkDrift * 1e-6;
      xvt.health = Xvt::HealthStatus::Unavailable;
      xvt.computeRelativityCorrection();
         /// @todo set xvt.frame
      return true;
   }


   void OrbitDataSP3 ::
   dump(std::ostream& s, Detail dl) const
   {
      NavData::dump(s,dl);
      if (dl == Detail::OneLine)
         return;
      const ios::fmtflags oldFlags = s.flags();
      s << fixed << setprecision(6) << "P: {";
      for (unsigned i = 0; i < pos.size(); i++)
         s << " " << pos[i];
      s << "} bias: " << clkBias << endl
        << "V: {";
      for (unsigned i = 0; i < vel.size(); i++)
         s << " " << vel[i];
      s << "} drift: " << clkDrift << endl;
      if (dl == Detail::Brief)
         return;
      s << "A: {";
      for (unsigned i = 0; i < acc.size(); i++)
         s << " " << acc[i];
      s << "} drift rate: " << clkDrRate << endl
        << "P sigma: {";
      for (unsigned i = 0; i < posSig.size(); i++)
         s << " " << posSig[i];
      s << "} bias sigma: " << biasSig << endl
        << "V sigma: {";
      for (unsigned i = 0; i < velSig.size(); i++)
         s << " " << velSig[i];
      s << "} drift sigma: " << driftSig << endl
        << "A sigma: {";
      for (unsigned i = 0; i < accSig.size(); i++)
         s << " " << accSig[i];
      s << "} drift rate sigma: " << drRateSig << endl;
      s.flags(oldFlags);
   }
}
