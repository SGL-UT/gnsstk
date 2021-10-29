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
#include "SVHealth.hpp"
#include "TestUtil.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::SVHealth e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class SVHealth_T
{
public:
   unsigned convertTest();
};


unsigned SVHealth_T ::
convertTest()
{
   TUDEF("SVHealth", "asString");
      // This effectively tests SVHealthIterator, asString and
      // asSVHealth all at once.
   for (gnsstk::SVHealth e : gnsstk::SVHealthIterator())
   {
      TUCSM("asString");
      std::string s(gnsstk::StringUtils::asString(e));
      TUASSERT(!s.empty());
      TUASSERT(s != "???");
      TUCSM("asSVHealth");
      gnsstk::SVHealth e2 = gnsstk::StringUtils::asSVHealth(s);
      TUASSERTE(gnsstk::SVHealth, e, e2);
   }
   TURETURN();
}


int main()
{
   SVHealth_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.convertTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
