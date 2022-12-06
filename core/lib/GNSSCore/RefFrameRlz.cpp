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

#include "RefFrameRlz.hpp"
#include "YDSTime.hpp"
#include "GPSWeekSecond.hpp"
#include "BasicTimeSystemConverter.hpp"

using namespace std;

namespace gnsstk
{
   RefFrameSys getRefFrameSys(RefFrameRlz rlz)
      noexcept
   {
      switch (rlz)
      {
         case RefFrameRlz::Unknown:       return RefFrameSys::Unknown;
         case RefFrameRlz::WGS84G0:       return RefFrameSys::WGS84;
         case RefFrameRlz::WGS84G730:     return RefFrameSys::WGS84;
         case RefFrameRlz::WGS84G873:     return RefFrameSys::WGS84;
         case RefFrameRlz::WGS84G1150:    return RefFrameSys::WGS84;
         case RefFrameRlz::WGS84G1674:    return RefFrameSys::WGS84;
         case RefFrameRlz::WGS84G1762:    return RefFrameSys::WGS84;
         case RefFrameRlz::WGS84G2139:    return RefFrameSys::WGS84;
         case RefFrameRlz::ITRF94:        return RefFrameSys::ITRF;
         case RefFrameRlz::ITRF96:        return RefFrameSys::ITRF;
         case RefFrameRlz::ITRF97:        return RefFrameSys::ITRF;
         case RefFrameRlz::ITRF2000:      return RefFrameSys::ITRF;
         case RefFrameRlz::ITRF2005:      return RefFrameSys::ITRF;
         case RefFrameRlz::ITRF2008:      return RefFrameSys::ITRF;
         case RefFrameRlz::ITRF2014:      return RefFrameSys::ITRF;
         case RefFrameRlz::ITRF2020:      return RefFrameSys::ITRF;
         case RefFrameRlz::PZ90Y2007:     return RefFrameSys::PZ90;
         case RefFrameRlz::PZ90KGS:       return RefFrameSys::PZ90;
         case RefFrameRlz::CGCS2000Y2008: return RefFrameSys::CGCS2000;
         default:                         return RefFrameSys::Unknown;
      }
   }


   RefFrameRlz getRefFrameRlz(RefFrameSys sys, const CommonTime& when)
   {
      CommonTime whenUTC(when);
      if (whenUTC.getTimeSystem() != TimeSystem::UTC)
      {
         BasicTimeSystemConverter btsc;
            // time isn't in UTC, do a rough (1s precision) conversion
         if (!whenUTC.changeTimeSystem(TimeSystem::UTC, &btsc))
         {
               // Can't convert for whatever reason, change it to Any
               // and hope for the best.  This will usually be okay as
               // the supported time systems are generally only a few
               // seconds apart, except for GLO.
            whenUTC.setTimeSystem(TimeSystem::Any);
         }
      }
      switch (sys)
      {
         case RefFrameSys::WGS84:
               /** @todo fix these dates up, I wasn't able to quickly
                * find actual publication dates. */
            if (whenUTC >= GPSWeekSecond(2139, 0, TimeSystem::UTC))
            {
               return RefFrameRlz::WGS84G2139;
            }
            else if (whenUTC >= GPSWeekSecond(1762, 0, TimeSystem::UTC))
            {
               return RefFrameRlz::WGS84G1762;
            }
            else if (whenUTC >= GPSWeekSecond(1674, 0, TimeSystem::UTC))
            {
               return RefFrameRlz::WGS84G1674;
            }
            else if (whenUTC >= GPSWeekSecond(1150, 0, TimeSystem::UTC))
            {
               return RefFrameRlz::WGS84G1150;
            }
            else if (whenUTC >= GPSWeekSecond(873, 0, TimeSystem::UTC))
            {
               return RefFrameRlz::WGS84G873;
            }
            else if (whenUTC >= GPSWeekSecond(730, 0, TimeSystem::UTC))
            {
               return RefFrameRlz::WGS84G730;
            }
            else
            {
               return RefFrameRlz::WGS84G0;
            }
         case RefFrameSys::ITRF:
               /** @todo fix these dates up, I wasn't able to quickly
                * find actual publication dates. */
            if (whenUTC >= YDSTime(2020, 1, 0, TimeSystem::UTC))
            {
               return RefFrameRlz::ITRF2020;
            }
            else if (whenUTC >= YDSTime(2014, 1, 0, TimeSystem::UTC))
            {
               return RefFrameRlz::ITRF2014;
            }
            else if (whenUTC >= YDSTime(2008, 1, 0, TimeSystem::UTC))
            {
               return RefFrameRlz::ITRF2008;
            }
            else if (whenUTC >= YDSTime(2005, 1, 0, TimeSystem::UTC))
            {
               return RefFrameRlz::ITRF2005;
            }
            else if (whenUTC >= YDSTime(2000, 1, 0, TimeSystem::UTC))
            {
               return RefFrameRlz::ITRF2000;
            }
            else if (whenUTC >= YDSTime(1997, 1, 0, TimeSystem::UTC))
            {
               return RefFrameRlz::ITRF97;
            }
            else if (whenUTC >= YDSTime(1996, 1, 0, TimeSystem::UTC))
            {
               return RefFrameRlz::ITRF96;
            }
            else
            {
               return RefFrameRlz::ITRF94;
            }
         case RefFrameSys::PZ90:
            if (whenUTC >= YDSTime(2007,263,61200.0,TimeSystem::UTC))
            {
               return RefFrameRlz::PZ90Y2007;
            }
            else
            {
               return RefFrameRlz::PZ90KGS;
            }
            break;
         case RefFrameSys::CGCS2000:
               // only one realization so far
            return RefFrameRlz::CGCS2000Y2008;
         default:
            return RefFrameRlz::Unknown;
      }
   }

   namespace StringUtils
   {
      std::string asString(RefFrameRlz e)
         noexcept
      {
         switch (e)
         {
            case RefFrameRlz::Unknown:       return "Unknown";
            case RefFrameRlz::WGS84G0:       return "WGS84(G0)";
            case RefFrameRlz::WGS84G730:     return "WGS84(G730)";
            case RefFrameRlz::WGS84G873:     return "WGS84(G873)";
            case RefFrameRlz::WGS84G1150:    return "WGS84(G1150)";
	    case RefFrameRlz::WGS84G1674:    return "WGS84(G1674)";
	    case RefFrameRlz::WGS84G1762:    return "WGS84(G1762)";
            case RefFrameRlz::WGS84G2139:    return "WGS84(G2139)";
            case RefFrameRlz::ITRF94:        return "ITRF(1994)";
            case RefFrameRlz::ITRF96:        return "ITRF(1996)";
            case RefFrameRlz::ITRF97:        return "ITRF(1997)";
            case RefFrameRlz::ITRF2000:      return "ITRF(2000)";
            case RefFrameRlz::ITRF2005:      return "ITRF(2005)";
            case RefFrameRlz::ITRF2008:      return "ITRF(2008)";
            case RefFrameRlz::ITRF2014:      return "ITRF(2014)";
            case RefFrameRlz::ITRF2020:      return "ITRF(2020)";
            case RefFrameRlz::PZ90Y2007:     return "PZ90(2007)";
            case RefFrameRlz::PZ90KGS:       return "PZ90KGS";
            case RefFrameRlz::CGCS2000Y2008: return "CGCS2000(2008)";
            default:                         return "???";
         }
      }


      RefFrameRlz asRefFrameRlz(const std::string& s)
         noexcept
      {
         if (s == "Unknown")        return RefFrameRlz::Unknown;
         if (s == "WGS84(G0)")      return RefFrameRlz::WGS84G0;
         if (s == "WGS84(G730)")    return RefFrameRlz::WGS84G730;
         if (s == "WGS84(G873)")    return RefFrameRlz::WGS84G873;
         if (s == "WGS84(G1150)")   return RefFrameRlz::WGS84G1150;
         if (s == "WGS84(G1674)")   return RefFrameRlz::WGS84G1674;
         if (s == "WGS84(G1762)")   return RefFrameRlz::WGS84G1762;
         if (s == "WGS84(G2139)")   return RefFrameRlz::WGS84G2139;
         if (s == "ITRF(1994)")     return RefFrameRlz::ITRF94;
         if (s == "ITRF(1996)")     return RefFrameRlz::ITRF96;
         if (s == "ITRF(1997)")     return RefFrameRlz::ITRF97;
         if (s == "ITRF(2000)")     return RefFrameRlz::ITRF2000;
         if (s == "ITRF(2005)")     return RefFrameRlz::ITRF2005;
         if (s == "ITRF(2008)")     return RefFrameRlz::ITRF2008;
         if (s == "ITRF(2014)")     return RefFrameRlz::ITRF2014;
         if (s == "ITRF(2020)")     return RefFrameRlz::ITRF2020;
         if (s == "PZ90(2007)")     return RefFrameRlz::PZ90Y2007;
         if (s == "PZ90KGS")        return RefFrameRlz::PZ90KGS;
         if (s == "CGCS2000(2008)") return RefFrameRlz::CGCS2000Y2008;
            // strings as they (probably) appear in SP3
         if (s == "ITR94")          return RefFrameRlz::ITRF94;
         if (s == "ITR96")          return RefFrameRlz::ITRF96;
         if (s == "ITR97")          return RefFrameRlz::ITRF97;
         return RefFrameRlz::Unknown;
      }
   } // namespace StringUtils
}   // end namespace
