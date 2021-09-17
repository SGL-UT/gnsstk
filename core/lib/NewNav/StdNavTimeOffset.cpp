//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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

namespace gnsstk
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
         GNSSTK_ASSERT(when.getTimeSystem() == fromSys);
            // Change the reference time system to the source time
            // system (fromSys) to allow for reverse conversions
            // (e.g. UTC->GPS vs GPS->UTC).
         CommonTime t0(refTime);
         t0.setTimeSystem(fromSys);
            // difference between tE and tot (using GPS terms)
         double dt = when - t0;
            // delta tLS should be 0 for anything other than UTC.
         offset = deltatLS + a0 + a1*dt + a2*dt*dt;
            // UTC conversion includes leap seconds and everyone
            // copied what GPS was doing, more or less, including the
            // ICD/IS.
         if ((fromSys == TimeSystem::UTC) || (toSys == TimeSystem::UTC))
         {
            CommonTime effBlank(effTime);
            effBlank.setTimeSystem(fromSys);
            if ((when >= effBlank) && ((when - effBlank) <= effEnd()))
            {
                  // use deltatLSF from effectivity time to
                  // effectivity time + effEnd()
               offset = deltatLSF + a0 + a1*dt + a2*dt*dt;
            }
         }
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
      if (dl == DumpDetail::OneLine)
      {
         NavData::dump(s,dl);
         return;
      }
      if (dl == DumpDetail::Brief)
      {
         double offset;
         getOffset(src, tgt, refTime, offset);
            // brief just shows the offset as of the reference time.
         s << StringUtils::asString(src) << "-" << StringUtils::asString(tgt)
           << " offset = " << offset << endl;
         return;
      }
      s << "****************************************************************"
        << "************" << endl
        << "Time System Offset" << endl << endl
        << "PRN : " << setw(2) << signal.sat << " / "
        << "SVN : " << setw(2);
      std::string svn;
      if (getSVN(signal.sat, timeStamp, svn))
      {
         s << svn;
      }

      s << "           TIMES OF INTEREST" << endl << endl
        << "              Week(10bt)     SOW     DOW   UTD     SOD"
        << "   MM/DD/YYYY   HH:MM:SS\n"
        << "Transmit:     " << printTime(timeStamp, weekFmt+dumpTimeFmt)
        << endl << endl
        << "           " << StringUtils::asString(src) << " "
        << StringUtils::asString(tgt) << " PARAMETERS" << endl
        << "Parameter              Value" << endl
        << "Reference   "
        << printTime(refTime,"%Y/%02m/%02d %02H:%02M:%02S") << endl
        << "Effective   "
        << printTime(effTime,"%Y/%02m/%02d %02H:%02M:%02S") << endl
        << right << uppercase << fixed << setprecision(0) << setfill(' ')
        << "t-sub-ot    " << setw(16) << tot << " sec" << endl
        << scientific << setprecision(9)
        << "A0          " << setw(16) << a0 << " sec" << endl
        << "A1          " << setw(16) << a1 << " sec/sec" << endl
        << "A2          " << setw(16) << a2 << " sec/sec**2" << endl
        << fixed << setprecision(0)
        << "dtLS        " << setw(16) << deltatLS << " sec" << endl
        << "dtLSF       " << setw(16) << deltatLSF << " sec" << endl
        << "WNot        " << setw(11) << wnot << "("
        << setw(3) << (wnot & 0x0ff) << ") Full week (modulo 256 week)" << endl
        << "WN(LSF)     " << setw(11) << wnLSF << "("
        << setw(3) << (wnLSF & 0x0ff) << ") Full week (modulo 256 week)" << endl
            /** @todo maybe need to make this a dynamic label for
             * systems that start at DN=0 */
        << "DN          " << setw(16) << dn << " day (1-7)" << endl;
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

