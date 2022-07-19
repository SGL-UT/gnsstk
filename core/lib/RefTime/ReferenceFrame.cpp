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

#include "ReferenceFrame.hpp"

using namespace std;

namespace gnsstk
{
   namespace StringUtils
   {
      std::string asString(ReferenceFrame e)
      {
         switch (e)
         {
            case ReferenceFrame::Unknown:    return "Unknown";
            case ReferenceFrame::WGS84:      return "WGS84";
            case ReferenceFrame::WGS84G730:  return "WGS84(G730)";
            case ReferenceFrame::WGS84G873:  return "WGS84(G873)";
            case ReferenceFrame::WGS84G1150: return "WGS84(G1150)";
	    case ReferenceFrame::WGS84G1674: return "WGS84(G1674)";
	    case ReferenceFrame::WGS84G1762: return "WGS84(G1762)";
            case ReferenceFrame::ITRF:       return "ITRF";
            case ReferenceFrame::ITRF94:     return "ITRF(1994)";
            case ReferenceFrame::ITRF96:     return "ITRF(1996)";
            case ReferenceFrame::ITRF97:     return "ITRF(1997)";
            case ReferenceFrame::ITRF2000:   return "ITRF(2000)";
            case ReferenceFrame::ITRF2005:   return "ITRF(2005)";
            case ReferenceFrame::ITRF2008:   return "ITRF(2008)";
            case ReferenceFrame::ITRF2014:   return "ITRF(2014)";
            case ReferenceFrame::PZ90:       return "PZ90";
            case ReferenceFrame::PZ90KGS:    return "PZ90KGS";
            case ReferenceFrame::CGCS2000:   return "CGCS2000";
            default:                         return "???";
         }
      }


      ReferenceFrame asReferenceFrame(const std::string& s)
      {
         if (s == "Unknown")      return ReferenceFrame::Unknown;
         if (s == "WGS84")        return ReferenceFrame::WGS84;
         if (s == "WGS84(G730)")  return ReferenceFrame::WGS84G730;
         if (s == "WGS84(G873)")  return ReferenceFrame::WGS84G873;
         if (s == "WGS84(G1150)") return ReferenceFrame::WGS84G1150;
         if (s == "WGS84(G1674)") return ReferenceFrame::WGS84G1674;
         if (s == "WGS84(G1762)") return ReferenceFrame::WGS84G1762;
         if (s == "ITRF")         return ReferenceFrame::ITRF;
         if (s == "ITRF(1994)")   return ReferenceFrame::ITRF94;
         if (s == "ITRF(1996)")   return ReferenceFrame::ITRF96;
         if (s == "ITRF(1997)")   return ReferenceFrame::ITRF97;
         if (s == "ITRF(2000)")   return ReferenceFrame::ITRF2000;
         if (s == "ITRF(2005)")   return ReferenceFrame::ITRF2005;
         if (s == "ITRF(2008)")   return ReferenceFrame::ITRF2008;
         if (s == "ITRF(2014)")   return ReferenceFrame::ITRF2014;
         if (s == "PZ90")         return ReferenceFrame::PZ90;
         if (s == "PZ90KGS")      return ReferenceFrame::PZ90KGS;
         if (s == "CGCS2000")     return ReferenceFrame::CGCS2000;
            // strings as they (probably) appear in SP3
         if (s == "ITR94")        return ReferenceFrame::ITRF94;
         if (s == "ITR96")        return ReferenceFrame::ITRF96;
         if (s == "ITR97")        return ReferenceFrame::ITRF97;
         return ReferenceFrame::Unknown;
      }
   }
}   // end namespace
