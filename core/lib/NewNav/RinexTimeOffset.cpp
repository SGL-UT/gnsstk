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

#include "RinexTimeOffset.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gnsstk
{
   RinexTimeOffset ::
   RinexTimeOffset()
         : deltatLS(0)
   {
   }


   RinexTimeOffset ::
   RinexTimeOffset(const TimeSystemCorrection& right, double leapSec)
         : TimeSystemCorrection(right), deltatLS(leapSec)
   {
   }


   bool RinexTimeOffset ::
   validate() const
   {
         /// @todo implement some validation
      return true;
   }


   CommonTime RinexTimeOffset ::
   getUserTime() const
   {
         /// @todo figure out a sensible value, if there is one.
      return TimeOffsetData::getUserTime();
   }


   void RinexTimeOffset ::
   dump(ostream& s, DumpDetail dl) const
   {
      const ios::fmtflags oldFlags = s.flags();
      switch (dl)
      {
         case DumpDetail::OneLine:
            NavData::dump(s,dl);
            break;
         case DumpDetail::Brief:
            NavData::dump(s,dl);
               // brief just shows the offset as of the reference time.
            s << StringUtils::asString(frTS) << "-"
              << StringUtils::asString(toTS) << " offset = " << (deltatLS+A0)
              << endl;
            break;
         case DumpDetail::Full:
            s << "*************************************************************"
              << "***************" << endl
              << "Time System Offset" << endl << endl
              << "           TIMES OF INTEREST" << endl << endl
              << "              " << getDumpTimeHdr(dl) << endl
              << "Transmit:     " << getDumpTime(dl, timeStamp)
              << endl << endl
              << "           " << StringUtils::asString(frTS) << " "
              << StringUtils::asString(toTS) << " PARAMETERS" << endl
              << "Parameter                 Value" << endl
              << "Reference   "
              << printTime(refTime,"%Y/%02m/%02d %02H:%02M:%02S") << endl
              << scientific << setprecision(9) << setfill(' ')
              << "A0             " << setw(16) << A0 << " sec" << endl
              << "A1             " << setw(16) << A1 << " sec/sec" << endl
              << fixed << setprecision(0)
              << "dtLS           " << setw(16) << deltatLS << " sec" << endl
              << "Provider       " << setw(16) << geoProvider << endl
              << "UTC            " << setw(16);
            switch (geoUTCid)
            {
               case 0: s << "Unknown" << endl; break;
               case 1: s << "NIST" << endl; break;
               case 2: s << "USNO" << endl; break;
               case 3: s << "SU" << endl; break;
               case 4: s << "BIPM" << endl; break;
               case 5: s << "Europe" << endl; break;
               case 6: s << "CRL" << endl; break;
               default: s << geoUTCid << "=???" << endl; break;
            }
            break;
      }
      s.flags(oldFlags);
   }


   bool RinexTimeOffset ::
   getOffset(TimeSystem fromSys, TimeSystem toSys,
             const CommonTime& when, double& offset)
      const
   {
      try
      {
            // TimeSystemCorrection::Correction assumes the source
            // time system is defined in the refTime value and the
            // target is defined in "when".
         offset = deltatLS + Correction(when);
         return true;
      }
      catch (gnsstk::Exception& exc)
      {
         cerr << exc << endl;
         return false;
      }
   }


   TimeCvtSet RinexTimeOffset ::
   getConversions() const
   {
      TimeCvtKey keyF(frTS,toTS);
      TimeCvtKey keyR(toTS,frTS);
      return TimeCvtSet({ keyF, keyR });
   }

} // namespace gnsstk
