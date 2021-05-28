//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//  
//  This software was developed by Applied Research Laboratories at the 
//  University of Texas at Austin.
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
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
#include "StdNavTimeOffset.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gpstk
{
   StdNavTimeOffset ::
   StdNavTimeOffset()
         : a0(0.0), a1(0.0), a2(0.0), deltatLS(0.0), tot(0.0), wnot(0),
           wnLSF(0), dn(0), deltatLSF(0.0),
           src(TimeSystem::Unknown), tgt(TimeSystem::Unknown)
   {
   }


   bool StdNavTimeOffset ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, double& offset)
      const
   {
      if (((fromSys == src) && (toSys == tgt)) ||
          ((fromSys == tgt) && (toSys == src)))
      {
         GPSTK_ASSERT(when.getTimeSystem()  == fromSys);
            // Change the reference time system to the source time
            // system (fromSys) to allow for reverse conversions
            // (e.g. UTC->GPS vs GPS->UTC).
         CommonTime t0(refTime);
         t0.setTimeSystem(fromSys);
         double dt = when - t0;
            // delta tLS should be 0 for anything other than UTC.
         offset = deltatLS + a0 + a1*dt + a2*dt*dt;
         if (fromSys == tgt)
            offset = -offset;
         return true;
      }
      return false;
   }


   void StdNavTimeOffset ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      const ios::fmtflags oldFlags = s.flags();
      NavData::dump(s,dl);
      double offset;
      switch (dl)
      {
         case DumpDetail::OneLine:
            break;
         case DumpDetail::Brief:
               // brief just shows the offset as of the reference time.
            getOffset(src, tgt, refTime, offset);
            s << StringUtils::asString(src) << "-" << StringUtils::asString(tgt)
              << " offset = " << offset << endl;
            break;
         case DumpDetail::Full:
            getOffset(src, tgt, refTime, offset);
            s << setprecision(16)
              << "  src system = " << StringUtils::asString(src) << endl
              << "  tgt system = " << StringUtils::asString(tgt) << endl
              << "  A0         = " << a0 << endl
              << "  A1         = " << a1 << endl
              << "  A2         = " << a2 << endl
              << "  delta tLS  = " << deltatLS << endl
              << "  refTime    = "
              << printTime(refTime,"%Y/%02m/%02d %02H:%02M:%02S") << endl
              << "  tot        = " << tot << endl
              << "  WNot       = " << wnot << endl
              << "  WN(LSF)    = " << wnLSF << endl
              << "  DN         = " << dn << endl
              << "  delta tLSF = " << deltatLSF << endl
              << "  offset(ref)= " << offset << endl;
            break;
      }
      s.flags(oldFlags);
   }


   TimeCvtSet StdNavTimeOffset ::
   getConversions() const
   {
      TimeCvtKey keyF(src,tgt);
      TimeCvtKey keyR(tgt,src);
      return TimeCvtSet({ keyF, keyR });
   }
}

