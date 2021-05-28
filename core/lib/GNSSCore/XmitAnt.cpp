//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
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

#include "XmitAnt.hpp"

namespace gpstk
{
   namespace StringUtils
   {
      std::string asString(XmitAnt e) throw()
      {
         switch (e)
         {
            case XmitAnt::Unknown:   return "Unknown";
            case XmitAnt::Any:       return "Any";
            case XmitAnt::Standard:  return "Standard";
            case XmitAnt::Regional:  return "Regional";
            default:                 return "???";
         } // switch (e)
      } // asString(XmitAnt)


      XmitAnt asXmitAnt(const std::string& s) throw()
      {
         if (s == "Unknown")
            return XmitAnt::Unknown;
         if (s == "Any")
            return XmitAnt::Any;
         if (s == "Standard")
            return XmitAnt::Standard;
         if (s == "Regional")
            return XmitAnt::Regional;
         return XmitAnt::Unknown;
      } // asXmitAnt(string)
   } // namespace StringUtils
} // namespace gpstk
