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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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
#include "GPSCNav2Alm.hpp"

using namespace std;

namespace gpstk
{
   const double GPSCNav2Alm::refioffsetGPS = 0.3 * PI;

   GPSCNav2Alm ::
   GPSCNav2Alm()
         : healthL1(true),
           healthL2(true),
           healthL5(true),
           deltai(0),
           wna(0),
           toa(0)
   {
      signal.messageType = NavMessageType::Almanac;
      msgLenSec = 5.48;
   }


   bool GPSCNav2Alm ::
   validate() const
   {
         /// @todo implement some checks.
      return true;
   }


   void GPSCNav2Alm ::
   fixFit()
   {
         /** @todo Determine a more reasonable set of values.  This
          * was copied from OrbAlmExt. */
      beginFit = xmitTime;
      endFit   = gpstk::CommonTime::END_OF_TIME;
      endFit.setTimeSystem(beginFit.getTimeSystem());
   }
}
