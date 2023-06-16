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
//  Copyright 2004-2023, The Board of Regents of The University of Texas System
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
#include "SatTimeSystem.hpp"
#include "TestUtil.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::SatelliteSystem ss)
   {
      s << gnsstk::StringUtils::asString(ss) << " (" << (int)ss << ")";
      return s;
   }
}


class SatelliteSystem_T
{
public:
   unsigned convertTest();
   unsigned satTimeSystemTest();
};


unsigned SatelliteSystem_T ::
convertTest()
{
   TUDEF("SatelliteSystem", "asString");
   for (gnsstk::SatelliteSystem e : gnsstk::SatelliteSystemIterator())
   {
      TUCSM("asString");
      std::string s(gnsstk::StringUtils::asString(e));
      TUASSERT(!s.empty());
      TUASSERT(s != "???");
      TUCSM("asSatelliteSystem");
      gnsstk::SatelliteSystem e2 = gnsstk::StringUtils::asSatelliteSystem(s);
      TUASSERTE(gnsstk::SatelliteSystem, e, e2);
   }
   TURETURN();
}


unsigned SatelliteSystem_T ::
satTimeSystemTest()
{
   TUDEF("SatelliteSystem", "satTimeSystem");
   static const std::map<gnsstk::SatelliteSystem, gnsstk::TimeSystem>
      expData = {{gnsstk::SatelliteSystem::GPS,     gnsstk::TimeSystem::GPS},
                 {gnsstk::SatelliteSystem::Galileo, gnsstk::TimeSystem::GAL},
                 {gnsstk::SatelliteSystem::Glonass, gnsstk::TimeSystem::GLO},
                 {gnsstk::SatelliteSystem::BeiDou,  gnsstk::TimeSystem::BDT},
                 {gnsstk::SatelliteSystem::QZSS,    gnsstk::TimeSystem::QZS},
                 {gnsstk::SatelliteSystem::IRNSS,   gnsstk::TimeSystem::IRN},
                 {gnsstk::SatelliteSystem::Geosync, gnsstk::TimeSystem::GPS},
                 {gnsstk::SatelliteSystem::LEO,     gnsstk::TimeSystem::GPS},
                 {gnsstk::SatelliteSystem::Transit, gnsstk::TimeSystem::GPS},
                 {gnsstk::SatelliteSystem::Unknown,
                  gnsstk::TimeSystem::Unknown},
                 {gnsstk::SatelliteSystem::Mixed,
                  gnsstk::TimeSystem::Unknown},
                 {gnsstk::SatelliteSystem::UserDefined,
                  gnsstk::TimeSystem::Unknown},
   };
   for (gnsstk::SatelliteSystem i : gnsstk::SatelliteSystemIterator())
   {
      auto edi = expData.find(i);
      if (edi == expData.end())
      {
         TUFAIL("Unable to find SatelliteSystem " +
                gnsstk::StringUtils::asString((int)i) + " in truth data");
      }
      else if ((i != gnsstk::SatelliteSystem::Unknown) ||
               (i != gnsstk::SatelliteSystem::Mixed) ||
               (i != gnsstk::SatelliteSystem::UserDefined))
      {
         TUASSERTE(gnsstk::TimeSystem, edi->second,
                   gnsstk::satTimeSystem(i));
      }
   }
   TURETURN();
}


int main()
{
   SatelliteSystem_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.convertTest();
   errorTotal += testClass.satTimeSystemTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
