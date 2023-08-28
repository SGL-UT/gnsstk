//==============================================================================
//
//  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
//
//==============================================================================


//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin, under contract to an agency or agencies
//  within the U.S. Department of Defense. The U.S. Government retains all
//  rights to use, duplicate, distribute, disclose, or release this software.
//
//  Pursuant to DoD Directive 523024
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public
//                            release, distribution is unlimited.
//
//==============================================================================
#include "OrbitDataSP3.hpp"

using namespace std;

namespace gnsstk
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


   OrbitDataSP3 ::
   OrbitDataSP3(double val)
         : pos(val, val, val), posSig(val, val, val),
           vel(val, val, val), velSig(val, val, val),
           acc(val, val, val), accSig(val, val, val),
           clkBias(val), biasSig(val), clkDrift(val), driftSig(val),
           clkDrRate(val), drRateSig(val)
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
   getXvt(const CommonTime& when, Xvt& xvt, const ObsID& oid)
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
      xvt.health = Xvt::HealthStatus::Unused;
      xvt.computeRelativityCorrection();
      xvt.frame = frame;
      return true;
   }


   void OrbitDataSP3 ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      NavData::dump(s,dl);
      if (dl == DumpDetail::OneLine)
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
      if (dl == DumpDetail::Brief)
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
