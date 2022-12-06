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

#include "RefFrame.hpp"
#include "StringUtils.hpp"

namespace gnsstk
{
   RefFrame ::
   RefFrame()
         : system(RefFrameSys::Unknown),
           realization(RefFrameRlz::Unknown)
   {
   }


   RefFrame ::
   RefFrame(RefFrameRlz rlz)
         : realization(rlz),
           system(getRefFrameSys(rlz))
   {
   }


   RefFrame ::
   RefFrame(RefFrameSys sys, const gnsstk::CommonTime& when)
         : system(sys),
           realization(getRefFrameRlz(sys,when))
   {
   }


   RefFrame ::
   RefFrame(const std::string& str, const gnsstk::CommonTime& when)
   {
      realization = StringUtils::asRefFrameRlz(str);
      if (realization != gnsstk::RefFrameRlz::Unknown)
      {
         system = getRefFrameSys(realization);
      }
      else
      {
            // try again but treat str as a system rather than a realization
         system = StringUtils::asRefFrameSys(str);
         realization = getRefFrameRlz(system, when);
      }
   }


   bool RefFrame ::
   operator<(const RefFrame& right) const noexcept
   {
      if (system < right.system) return true;
      if (right.system < system) return false;
      if (realization < right.realization) return true;
      return false;
   }


   bool RefFrame ::
   operator==(ReferenceFrame orf) const noexcept
   {
         // Only compare the reference frame SYSTEMS because that's
         // how it's being used in HelmertTransform.
      return (((orf == ReferenceFrame::Unknown) &&
               (system == RefFrameSys::Unknown)) ||
              ((orf == ReferenceFrame::WGS84) &&
               (system == RefFrameSys::WGS84)) ||
              ((orf == ReferenceFrame::ITRF) &&
               (system == RefFrameSys::ITRF)) ||
              ((orf == ReferenceFrame::PZ90) &&
               (system == RefFrameSys::PZ90)) ||
              ((orf == ReferenceFrame::CGCS2000) &&
               (system == RefFrameSys::CGCS2000)));
   }


   RefFrame ::
   RefFrame(ReferenceFrame orf, const gnsstk::CommonTime& when)
   {
      switch (orf)
      {
         case ReferenceFrame::WGS84:
            *this = RefFrame(RefFrameSys::WGS84, when);
            break;
         case ReferenceFrame::WGS84G730:
            *this = RefFrame(RefFrameRlz::WGS84G730);
            break;
         case ReferenceFrame::WGS84G873:
            *this = RefFrame(RefFrameRlz::WGS84G873);
            break;
         case ReferenceFrame::WGS84G1150:
            *this = RefFrame(RefFrameRlz::WGS84G1150);
            break;
         case ReferenceFrame::WGS84G1674:
            *this = RefFrame(RefFrameRlz::WGS84G1674);
            break;
         case ReferenceFrame::WGS84G1762:
            *this = RefFrame(RefFrameRlz::WGS84G1762);
            break;
         case ReferenceFrame::ITRF:
            *this = RefFrame(RefFrameSys::ITRF, when);
            break;
         case ReferenceFrame::ITRF94:
            *this = RefFrame(RefFrameRlz::ITRF94);
            break;
         case ReferenceFrame::ITRF96:
            *this = RefFrame(RefFrameRlz::ITRF96);
            break;
         case ReferenceFrame::ITRF97:
            *this = RefFrame(RefFrameRlz::ITRF97);
            break;
         case ReferenceFrame::ITRF2000:
            *this = RefFrame(RefFrameRlz::ITRF2000);
            break;
         case ReferenceFrame::ITRF2005:
            *this = RefFrame(RefFrameRlz::ITRF2005);
            break;
         case ReferenceFrame::ITRF2008:
            *this = RefFrame(RefFrameRlz::ITRF2008);
            break;
         case ReferenceFrame::ITRF2014:
            *this = RefFrame(RefFrameRlz::ITRF2014);
            break;
         case ReferenceFrame::PZ90:
            *this = RefFrame(RefFrameSys::PZ90, when);
            break;
         case ReferenceFrame::PZ90KGS:
            *this = RefFrame(RefFrameRlz::PZ90KGS);
            break;
         case ReferenceFrame::CGCS2000:
            *this = RefFrame(RefFrameSys::CGCS2000, when);
            break;
         default:
         {
            InvalidParameter exc("Invalid ReferenceFrame " +
                                 StringUtils::asString((int)orf));
            GNSSTK_THROW(exc);
            break;
         }
      }
   }
} // namespace gnsstk
