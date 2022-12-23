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
#include <map>
#include "RefFrameRlz.hpp"
#include "GPSWeekSecond.hpp"
#include "YDSTime.hpp"
#include "TestUtil.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::RefFrameRlz e)
   {
      s << StringUtils::asString(e) << "(" << (int)e << ")";
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gnsstk::RefFrameSys e)
   {
      s << StringUtils::asString(e) << "(" << (int)e << ")";
      return s;
   }
}


class RefFrameRlz_T
{
public:
   unsigned convertTest();
   unsigned getRefFrameSysTest();
   unsigned getRefFrameRlzTest();
};


unsigned RefFrameRlz_T ::
convertTest()
{
   TUDEF("RefFrameRlz", "asString");
      // This effectively tests RefFrameRlzIterator, and asRefFrameRlz
      // all at once.
   for (gnsstk::RefFrameRlz e : gnsstk::RefFrameRlzIterator())
   {
      TUCSM("asString");
      std::string s(gnsstk::StringUtils::asString(e));
      TUASSERT(!s.empty());
      TUASSERT(s != "???");
      TUCSM("asRefFrameRlz");
      gnsstk::RefFrameRlz e2 = gnsstk::StringUtils::asRefFrameRlz(s);
      TUASSERTE(gnsstk::RefFrameRlz, e, e2);
   }
   TURETURN();
}


unsigned RefFrameRlz_T ::
getRefFrameSysTest()
{
   TUDEF("RefFrameRlz", "getRefFrameSys");
      // make sure they all have a valid system
   for (gnsstk::RefFrameRlz i : gnsstk::RefFrameRlzIterator())
   {
      if (i == gnsstk::RefFrameRlz::Unknown)
      {
         TUASSERTE(gnsstk::RefFrameSys, gnsstk::RefFrameSys::Unknown,
                   getRefFrameSys(i));
      }
      else
      {
         TUASSERT(getRefFrameSys(i) != gnsstk::RefFrameSys::Unknown);
      }
   }
      // Now test the ones we know about (which could get added to,
      // which is why we test the above separately).
   for (gnsstk::RefFrameRlz i : gnsstk::EnumIterator<gnsstk::RefFrameRlz,
           gnsstk::RefFrameRlz::WGS84G0, gnsstk::RefFrameRlz::ITRF94>())
   {
      TUASSERTE(gnsstk::RefFrameSys, gnsstk::RefFrameSys::WGS84,
                getRefFrameSys(i));
   }
   for (gnsstk::RefFrameRlz i : gnsstk::EnumIterator<gnsstk::RefFrameRlz,
           gnsstk::RefFrameRlz::ITRF94, gnsstk::RefFrameRlz::PZ90Y2007>())
   {
      TUASSERTE(gnsstk::RefFrameSys, gnsstk::RefFrameSys::ITRF,
                getRefFrameSys(i));
   }
   for (gnsstk::RefFrameRlz i : gnsstk::EnumIterator<gnsstk::RefFrameRlz,
           gnsstk::RefFrameRlz::PZ90Y2007, gnsstk::RefFrameRlz::CGCS2000Y2008>())
   {
      TUASSERTE(gnsstk::RefFrameSys, gnsstk::RefFrameSys::PZ90,
                getRefFrameSys(i));
   }
   for (gnsstk::RefFrameRlz i : gnsstk::EnumIterator<gnsstk::RefFrameRlz,
           gnsstk::RefFrameRlz::CGCS2000Y2008, gnsstk::RefFrameRlz::Last>())
   {
      TUASSERTE(gnsstk::RefFrameSys, gnsstk::RefFrameSys::CGCS2000,
                getRefFrameSys(i));
   }
   TURETURN();
}


unsigned RefFrameRlz_T ::
getRefFrameRlzTest()
{
   TUDEF("RefFrameRlz", "getRefFrameRlzTest");
   typedef std::pair<gnsstk::RefFrameSys, gnsstk::CommonTime> InpPair;
   typedef std::map<InpPair, gnsstk::RefFrameRlz> ExpMap;
   gnsstk::TimeSystem ts(gnsstk::TimeSystem::UTC); // short alias.
   ExpMap inOutMap
      {{InpPair(gnsstk::RefFrameSys::WGS84,gnsstk::GPSWeekSecond(0,0,ts)),
        gnsstk::RefFrameRlz::WGS84G0},
       {InpPair(gnsstk::RefFrameSys::WGS84,gnsstk::GPSWeekSecond(730,0,ts)),
        gnsstk::RefFrameRlz::WGS84G730},
       {InpPair(gnsstk::RefFrameSys::WGS84,gnsstk::GPSWeekSecond(873,0,ts)),
        gnsstk::RefFrameRlz::WGS84G873},
       {InpPair(gnsstk::RefFrameSys::WGS84,gnsstk::GPSWeekSecond(1150,0,ts)),
        gnsstk::RefFrameRlz::WGS84G1150},
       {InpPair(gnsstk::RefFrameSys::WGS84,gnsstk::GPSWeekSecond(1674,0,ts)),
        gnsstk::RefFrameRlz::WGS84G1674},
       {InpPair(gnsstk::RefFrameSys::WGS84,gnsstk::GPSWeekSecond(1762,0,ts)),
        gnsstk::RefFrameRlz::WGS84G1762},
       {InpPair(gnsstk::RefFrameSys::WGS84,gnsstk::GPSWeekSecond(2139,0,ts)),
        gnsstk::RefFrameRlz::WGS84G2139},
       {InpPair(gnsstk::RefFrameSys::ITRF,gnsstk::YDSTime(1994,1,0,ts)),
        gnsstk::RefFrameRlz::ITRF94},
       {InpPair(gnsstk::RefFrameSys::ITRF,gnsstk::YDSTime(1996,1,0,ts)),
        gnsstk::RefFrameRlz::ITRF96},
       {InpPair(gnsstk::RefFrameSys::ITRF,gnsstk::YDSTime(1997,1,0,ts)),
        gnsstk::RefFrameRlz::ITRF97},
       {InpPair(gnsstk::RefFrameSys::ITRF,gnsstk::YDSTime(2000,1,0,ts)),
        gnsstk::RefFrameRlz::ITRF2000},
       {InpPair(gnsstk::RefFrameSys::ITRF,gnsstk::YDSTime(2005,1,0,ts)),
        gnsstk::RefFrameRlz::ITRF2005},
       {InpPair(gnsstk::RefFrameSys::ITRF,gnsstk::YDSTime(2008,1,0,ts)),
        gnsstk::RefFrameRlz::ITRF2008},
       {InpPair(gnsstk::RefFrameSys::ITRF,gnsstk::YDSTime(2014,1,0,ts)),
        gnsstk::RefFrameRlz::ITRF2014},
       {InpPair(gnsstk::RefFrameSys::ITRF,gnsstk::YDSTime(2020,1,0,ts)),
        gnsstk::RefFrameRlz::ITRF2020},
       {InpPair(gnsstk::RefFrameSys::PZ90,gnsstk::YDSTime(2006,1,0,ts)),
        gnsstk::RefFrameRlz::PZ90KGS},
       {InpPair(gnsstk::RefFrameSys::PZ90,gnsstk::YDSTime(2007,263,61200,ts)),
        gnsstk::RefFrameRlz::PZ90Y2007},
       {InpPair(gnsstk::RefFrameSys::CGCS2000,gnsstk::YDSTime(2022,364,0,ts)),
        gnsstk::RefFrameRlz::CGCS2000Y2008},
      };

   for (const auto& emi : inOutMap)
   {
      TUASSERTE(gnsstk::RefFrameRlz, emi.second,
                gnsstk::getRefFrameRlz(emi.first.first, emi.first.second));
   }
   TURETURN();
}


int main()
{
   RefFrameRlz_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.convertTest();
   errorTotal += testClass.getRefFrameSysTest();
   errorTotal += testClass.getRefFrameRlzTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
