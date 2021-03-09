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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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
#include "GPSCNavTimeOffset.hpp"
#include "GPSWeekSecond.hpp"

using namespace std;

static const unsigned maxDN = 7;
static const double maxtot = 604784.0;
static const double mintot = 0;

namespace gpstk
{
   GPSCNavTimeOffset ::
   GPSCNavTimeOffset()
         : a0(0.0), a1(0.0), a2(0.0), deltatLS(0.0), tot(0.0), wnot(0),
           wnLSF(0), dn(0), deltatLSF(0.0),
           tgt(TimeSystem::Unknown)
   {
   }


   bool GPSCNavTimeOffset ::
   validate() const
   {
         // see IS-GPS-200 
         // DN is defined in the ICD as being 1-7 but we also include
         // the value 0 as valid because we won't always have anything
         // else.
      return ((dn <= maxDN) && (tot <= maxtot) && (tot >= mintot));
   }


   CommonTime GPSCNavTimeOffset ::
   getUserTime() const
   {
      if (signal.nav == NavType::GPSCNAVL2)
         return timeStamp + 12.0;
      return timeStamp + 6.0;
   }


   bool GPSCNavTimeOffset ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, double& offset)
      const
   {
      if (((fromSys == TimeSystem::GPS) && (toSys == tgt)) ||
          ((fromSys == tgt) && (toSys == TimeSystem::GPS)))
      {
         GPSWeekSecond ws(when);
            //       tE       tot             WN      WNot
         double t = (ws.sow - tot + 604800.0*(ws.week-wnot));
            // delta tLS should be 0 for anything other than UTC.
         offset = deltatLS + a0 + a1*t + a2*t*t;
         if (fromSys == tgt)
            offset = -offset;
         return true;
      }
      return false;
   }


   void GPSCNavTimeOffset ::
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
            getOffset(TimeSystem::GPS, tgt,
                      gpstk::GPSWeekSecond(wnot,tot), offset);
            s << "GPS-" << StringUtils::asString(tgt) << " offset = " << offset
              << endl;
            break;
         case DumpDetail::Full:
            getOffset(TimeSystem::GPS, tgt,
                      gpstk::GPSWeekSecond(wnot,tot), offset);
            s << setprecision(16)
              << "  time system= " << StringUtils::asString(tgt) << endl
              << "  A0         = " << a0 << endl
              << "  A1         = " << a1 << endl
              << "  A2         = " << a2 << endl
              << "  delta tLS  = " << deltatLS << endl
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


   TimeCvtSet GPSCNavTimeOffset ::
   getConversions() const
   {
      TimeCvtKey keyF(TimeSystem::GPS,tgt);
      TimeCvtKey keyR(tgt,TimeSystem::GPS);
      return TimeCvtSet({ keyF, keyR });
   }
}

