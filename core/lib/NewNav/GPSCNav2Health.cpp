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
#include "GPSCNav2Health.hpp"

using namespace std;

namespace gpstk
{
   GPSCNav2Health ::
   GPSCNav2Health()
         : health(true)
   {
      msgLenSec = 5.48;
   }


   void GPSCNav2Health ::
   dump(std::ostream& s, Detail dl) const
   {
      NavData::dump(s,dl);
      if (dl == Detail::OneLine)
      {
         return;
      }
      s << "health = " << health << "  "
        << StringUtils::asString(getHealth()) << endl;
   }


   void GPSCNav2Health ::
   setEph(bool e)
   {
      msgLenSec = e ? 12.0 : 5.48;
   }
}
