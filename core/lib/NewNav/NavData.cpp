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
#include "NavData.hpp"
#include "TimeString.hpp"
#include "demangle.hpp"

namespace gnsstk
{
   const std::string NavData :: dumpTimeFmt("%3a-%w   %3j   %5.0s   %02m/%02d/%04Y   %02H:%02M:%02S");
   const std::string NavData :: dumpTimeFmtBrief("%4Y/%02m/%02d %03j %02H:%02M:%02S");
   gnsstk::SatMetaDataStore *NavData::satMetaDataStore = nullptr;

   NavData ::
   NavData()
         : msgLenSec(0),
           weekFmt("%4F(%4G)")
   {
   }


   bool NavData ::
   isSameData(const NavDataPtr& right) const
   {
      return ((timeStamp == right->timeStamp) &&
              (signal == right->signal));
   }


   std::list<std::string> NavData ::
   compare(const NavDataPtr& right) const
   {
      std::list<std::string> rv;
         // old nav implementation clearly didn't check this
         // if (timeStamp != right->timeStamp)
         //    rv.push_back("timeStamp");
      if (signal != right->signal)
         rv.push_back("signal");
      return rv;
   }


   void NavData ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      s << getDumpTime(dl,timeStamp) << " " << signal << std::endl;
   }


   std::string NavData ::
   getDumpTimeHdr(DumpDetail dl) const
   {
      std::string hdr;
      switch (dl)
      {
         case DumpDetail::Full:
            if (!weekFmt.empty())
            {
               hdr = "Week(10bt)     SOW   ";
            }
            hdr += "  DOW   UTD     SOD   MM/DD/YYYY   HH:MM:SS";
            break;
      }
      return hdr;
   }


   std::string NavData ::
   getDumpTime(DumpDetail dl, const CommonTime& t) const
   {
      std::string fmt;
      switch (dl)
      {
         case DumpDetail::OneLine:
         case DumpDetail::Brief:
            fmt = dumpTimeFmtBrief;
            break;
         case DumpDetail::Full:
            if (weekFmt.empty())
            {
               fmt = dumpTimeFmt;
            }
            else
            {
               fmt = weekFmt + "  %6.0g   " + dumpTimeFmt;
            }
            break;
      }
      if (fmt.empty())
         return fmt;
      return printTime(t, fmt);
   }


   std::string NavData ::
   getClassName() const
   {
      return demangle(typeid(*this).name());
   }
}

