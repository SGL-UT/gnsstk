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
#include "GalFNavHealth.hpp"
#include "GalINavHealth.hpp"

using namespace std;

namespace gpstk
{
   GalFNavHealth ::
   GalFNavHealth()
         : sigHealthStatus(GalHealthStatus::Unknown),
           dataValidityStatus(GalDataValid::Unknown),
           sisaIndex(255)
   {
         /** @todo Figure out a way to initialize sisaIndex such that
          * not having the value doesn't result in the health status
          * being tagged as Degraded. */
      msgLenSec = 10.0;
   }


   void GalFNavHealth ::
   dump(std::ostream& s, DumpDetail dl) const
   {
      NavData::dump(s,dl);
      if (dl == DumpDetail::OneLine)
      {
         return;
      }
      s << "  sigHealthStatus = " << static_cast<unsigned>(sigHealthStatus)
        << "  " << gpstk::StringUtils::asString(sigHealthStatus) << endl
        << "  dataValidityStatus = "
        << static_cast<unsigned>(dataValidityStatus)
        << "  " << gpstk::StringUtils::asString(dataValidityStatus) << endl
        << "  sigInSpaceAcc = " << (unsigned)sisaIndex << endl
        << "  health = " << gpstk::StringUtils::asString(getHealth()) << endl;
   }


   SVHealth GalFNavHealth ::
   getHealth() const
   {
      return GalINavHealth::galHealth(sigHealthStatus, dataValidityStatus,
                                      sisaIndex);
   }
}
