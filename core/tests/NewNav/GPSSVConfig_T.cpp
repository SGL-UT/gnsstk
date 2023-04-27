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
#include "GPSSVConfig.hpp"
#include "TestUtil.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::GPSSVConfig e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class GPSSVConfig_T
{
public:
   unsigned convertTest();
};


unsigned GPSSVConfig_T ::
convertTest()
{
   TUDEF("GPSSVConfig", "asString");
      // This effectively tests GPSSVConfigIterator, asString and asGPSSVConfig
      // all at once.
   for (const auto c : gnsstk::GPSSVConfigIterator())
   {
      TUCSM("asString");
      const auto s{gnsstk::StringUtils::asString(c)};
      TUASSERT(!s.empty());
      TUASSERT(s != "???");

      TUCSM("asGPSSVConfig");
      const auto c2{gnsstk::StringUtils::asGPSSVConfig(s)};
      TUASSERTE(gnsstk::GPSSVConfig, c, c2);
   }
   TURETURN();
}


int main()
{
   GPSSVConfig_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.convertTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal << '\n';

   return errorTotal;
}
