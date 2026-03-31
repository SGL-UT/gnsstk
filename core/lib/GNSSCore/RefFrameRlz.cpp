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
#include "BasicTimeSystemConverter.hpp"
#include "CivilTime.hpp"

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
         case RefFrameRlz::WGS84G2296:    return RefFrameSys::WGS84;
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
            // These are the dates at which the realizations became effective in NGA PE.
            // https://navi.ion.org/content/72/2/navi.693
            if (whenUTC >= CivilTime(2024, 1, 7, 0, 0, 0, TimeSystem::UTC))
            {
               // https://geodesy.noaa.gov/FGCS/BlueBook/pdf/Annex_N.pdf
               //   -- WGS 84 (G2296). Used by NGA from 1/7/2024 through present. Aligned to
               //   ITRF2020 at all epochs using IGS20 orbits, clocks, and antenna calibrations.
               return RefFrameRlz::WGS84G2296;
            }
            else if (whenUTC >= CivilTime(2021, 1, 3, 0, 0, 0, TimeSystem::UTC))
            {
               // https://earth-info.nga.mil/php/download.php?file=(U)WGS%2084(G2139).pdf
               //   A new realization of WGS 84 RF was introduced on January 3, 2021 that removed
               //   small systematic biases with respect to ITRF. Accordingly, the current frame
               //   is designated WGS 84(G2139) and replaces WGS 84(G1762’).
               return RefFrameRlz::WGS84G2139;
            }
            else if (whenUTC >= CivilTime(2013, 10, 16, 0, 0, 0, TimeSystem::UTC))
            {
               // https://nsgreg.nga.mil/doc/view?i=4085 (NGA.STND.0036_1.0.0_WGS84.pdf)
               //   Table 2.1 WGS 84 Station Coordinate Updates
               //    WGS 84 (G1762) | 16 Oct 2013
               return RefFrameRlz::WGS84G1762;
            }
            else if (whenUTC >= CivilTime(2012, 5, 7, 0, 0, 0, TimeSystem::UTC))
            {
               // https://nsgreg.nga.mil/doc/view?i=4085 (NGA.STND.0036_1.0.0_WGS84.pdf)
               //   Table 2.1 WGS 84 Station Coordinate Updates
               //    WGS 84 (G1674) | 7 May 2012
               // https://geodesy.noaa.gov/FGCS/BlueBook/pdf/Annex_N.pdf
               //   -- WGS 84 (G1674). Used by NGA from 2/8/2012 (broadcast) and 5/7/2012 (precise)
               //   through 10/15/2013. Aligned with ITRF2008 at epoch 2005.0.
               return RefFrameRlz::WGS84G1674;
            }
            else if (whenUTC >= CivilTime(2002, 1, 20, 0, 0, 0, TimeSystem::UTC))
            {
               // https://nsgreg.nga.mil/doc/view?i=4085 (NGA.STND.0036_1.0.0_WGS84.pdf)
               //   Table 2.1 WGS 84 Station Coordinate Updates
               //    WGS 84 (G1150)  | 20 Jan 2002
               return RefFrameRlz::WGS84G1150;
            }
            else if (whenUTC >= CivilTime(1996, 9, 29, 0, 0, 0, TimeSystem::UTC))
            {
               // https://nsgreg.nga.mil/doc/view?i=4085 (NGA.STND.0036_1.0.0_WGS84.pdf)
               //   Table 2.1 WGS 84 Station Coordinate Updates
               //    WGS 84 (G873)  | 29 Sep 1996
               return RefFrameRlz::WGS84G873;
            }
            else if (whenUTC >= CivilTime(1994, 1, 2, 0, 0, 0, TimeSystem::UTC))
            {
               // https://nsgreg.nga.mil/doc/view?i=4085 (NGA.STND.0036_1.0.0_WGS84.pdf)
               //   Table 2.1 WGS 84 Station Coordinate Updates
               //    WGS 84 (G730)  | 2 Jan 1994
               return RefFrameRlz::WGS84G730;
            }
            else
            {
               // https://nsgreg.nga.mil/doc/view?i=4085 (NGA.STND.0036_1.0.0_WGS84.pdf)
               //   Table 2.1 WGS 84 Station Coordinate Updates
               //    WGS 84          | 1 Jan 1987
               return RefFrameRlz::WGS84G0;
            }
         case RefFrameSys::ITRF:
            // These are the dates at which the realizations became effective in IGS products.
            if (whenUTC >= CivilTime(2022, 11, 27, 0, 0, 0, TimeSystem::UTC))
            {
               // https://lists.igs.org/pipermail/igsmail/2022/008270.html
               //   This is a kindly reminder about the upcoming switch of the IGS products to
               //   IGS20/igs20.atx and repro3 standards as described in IGSMAIL-8238 and IGSMAIL-8256
               //   starting from GPS week 2238, 27 November 2022.
               return RefFrameRlz::ITRF2020;
            }
            else if (whenUTC >= CivilTime(2017, 1, 29, 0, 0, 0, TimeSystem::UTC))
            {
               // https://lists.igs.org/pipermail/igsmail/2016/001233.html
               //   The IGS intends to switch from IGS08/igs08.atx to IGS14/igs14.atx
               //   starting with products of GPS week 1934 (29 January 2017).
               return RefFrameRlz::ITRF2014;
            }
            else if (whenUTC >= CivilTime(2011, 4, 17, 0, 0, 0, TimeSystem::UTC))
            {
               // https://lists.igs.org/pipermail/igsmail/2011/000188.html
               //   The IGS intends to switch from (IGS05 + igs05.atx) to (IGS08 + igs08.atx)
               //   starting with products of GPS week 1632 (17 April 2011).
               return RefFrameRlz::ITRF2008;
            }
            else if (whenUTC >= CivilTime(2006, 11, 5, 0, 0, 0, TimeSystem::UTC))
            {
               // https://lists.igs.org/pipermail/igsmail/2006/006809.html
               //   The first IGS products with IGS05 and igs05_1390.atx will be distributed...
               //   starting week 1400.
               // https://lists.igs.org/pipermail/igsmail/2011/000188.html
               //   The (IGS05 + igs05.atx) framework has been in effect since GPS week 1400 (5 Nov 2006)
               return RefFrameRlz::ITRF2005;
            }
            else if (whenUTC >= CivilTime(2001, 12, 2, 0, 0, 0, TimeSystem::UTC))
            {
               // https://lists.igs.org/pipermail/igsmail/2001/004978.html
               //   IGS Analysis Centers will begin to use the IGS realization of ITRF2000 (IGS00)
               //   with GPS week 1143, which starts on December 2, 2001.
               return RefFrameRlz::ITRF2000;
            }
            else if (whenUTC >= CivilTime(1999, 8, 1, 0, 0, 0, TimeSystem::UTC))
            {
               // https://lists.igs.org/pipermail/igsmail/1999/003805.html
               //   On 1 August 1999 the IGS changed its realization of the International
               //   Terrestrial Reference Frame by switching from the ITRF96 to the ITRF97.
               return RefFrameRlz::ITRF97;
            }
            else if (whenUTC >= CivilTime(1998, 3, 1, 0, 0, 0, TimeSystem::UTC))
            {
               // https://lists.igs.org/pipermail/igsmail/1998/003210.html
               //   Starting on March 1, 1998 (GPS Wk 0947; MJD 50873), ...all the IGS products,
               //   including the IGS Rapid and Final combined orbits/EOP, are based on ITRF96.
               return RefFrameRlz::ITRF96;
            }
            else
            {
               // The GNSSTk will not return the IGS realizations of ITRF reference frames before IGS94.
               // https://geodesy.noaa.gov/FGCS/BlueBook/pdf/Annex_N.pdf
               //   -- ITRF 89 Epoch 1988.0 (International Earth Rotation Service) NOT USED AS
               //   A GPS REFERENCE FRAME
               //   -- ITRF 91 Epoch 1988.0 [NGS] from 8/16/92 through 12/19/92
               //   -- ITRF 91 Epoch 1992.6 [NGS] from 12/1/93 through 1/8/94
               //   -- ITRF 92 Epoch 1994.0 [NGS] from 1/9/94 through 12/31/94
               //   -- ITRF 93 Epoch 1995.0 [NGS] from 1/1/95 through 6/29/96
               // https://lists.igs.org/pipermail/igsmail/1996/002763.html
               //   Starting with Wk 0860 (June 30, 1996): All IGS combined products (orbits, EOP
               //   and station solutions) are based on ITRF94.
               return RefFrameRlz::ITRF94;
            }
         case RefFrameSys::PZ90:
            if (whenUTC >= YDSTime(2007, 263, 61200.0, TimeSystem::UTC))
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
            case RefFrameRlz::WGS84G2296:    return "WGS84(G2296)";
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
         if (s == "WGS84(G2296)")   return RefFrameRlz::WGS84G2296;
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
