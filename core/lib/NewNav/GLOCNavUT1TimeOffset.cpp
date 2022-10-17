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
#include "GLOCNavUT1TimeOffset.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   GLOCNavUT1TimeOffset ::
   GLOCNavUT1TimeOffset()
         : NB(-1),
           UTCTAI(std::numeric_limits<double>::quiet_NaN())
   {
      msgLenSec = 3.0;
      weekFmt = "";
   }


   void GLOCNavUT1TimeOffset ::
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
         getOffset(TimeSystem::GLO, TimeSystem::UTC, refTime, offset);
            // brief just shows the offset as of the reference time.
         s << StringUtils::asString(TimeSystem::GLO) << "-"
           << StringUtils::asString(TimeSystem::UTC)
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

      s << endl << endl
        << "           TIMES OF INTEREST" << endl << endl
        << "              " << getDumpTimeHdr(dl) << endl
        << "Transmit:     " << getDumpTime(dl, timeStamp)
        << endl << endl
        << "           " << StringUtils::asString(TimeSystem::GLO) << " "
        << StringUtils::asString(TimeSystem::UTC) << " PARAMETERS" << endl
        << "Parameter                 Value" << endl
        << "Reference   "
        << printTime(refTime,"%Y/%02m/%02d %02H:%02M:%02S") << endl
        << right << uppercase << fixed << setprecision(0) << setfill(' ')
        << scientific << setprecision(9)
        << "B0             " << setw(16) << B0 << " sec" << endl
        << "B1             " << setw(16) << B1 << " sec/sec" << endl
        << "B2             " << setw(16) << B2 << " sec/sec**2" << endl
        << fixed << setprecision(0)
        << "UTC-TAI        " << setw(16) << UTCTAI << " sec" << endl;
      s.flags(oldFlags);
   }


   void GLOCNavUT1TimeOffset ::
   dumpTerse(ostream& s) const
   {
      const ios::fmtflags oldFlags = s.flags();
      s << std::setw(4) << signal.sat.id
        << printTime(timeStamp,"%02m/%02d/%04Y %03j %02H:%02M:%02S") << "! ";
      s.setf(ios::scientific, ios::floatfield);
      s.setf(ios::right, ios::adjustfield);
      s.setf(ios::uppercase);
      s.precision(9);
      s.fill(' ');
      s << "B0:" << B0 << " s, B1:" << B1 << " s/s, B2:" << B2 << "s/s**2 ";
      s.flags(oldFlags);
   }


   bool GLOCNavUT1TimeOffset ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, double& offset)
      const
   {
      if (((fromSys == TimeSystem::GLO) && (toSys == TimeSystem::UTC)) ||
          ((fromSys == TimeSystem::UTC) && (toSys == TimeSystem::GLO)))
      {
         GNSSTK_ASSERT(when.getTimeSystem() == fromSys);
         double dt = (when - refTime) / 86400.0;
         double tauUT1 = B0 + B1*dt + (B2/2.0)*dt*dt;
         offset = tauUT1;
         if (fromSys == TimeSystem::UTC)
         {
            offset = -offset;
         }
         return true;
      }
      return false;
   }


   TimeCvtSet GLOCNavUT1TimeOffset ::
   getConversions() const
   {
      TimeCvtKey keyF(TimeSystem::GLO,TimeSystem::UTC);
      TimeCvtKey keyR(TimeSystem::UTC,TimeSystem::GLO);
      return TimeCvtSet({ keyF, keyR });
   }


   bool GLOCNavUT1TimeOffset ::
   validate() const
   {
         /// @todo add some checks.
      return true;
   }
}

