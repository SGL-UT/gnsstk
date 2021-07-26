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
#include <cmath>
#include "GalINavISC.hpp"
#include "FreqConv.hpp"

using namespace std;

namespace gpstk
{
   GalINavISC ::
   GalINavISC()
         : bgdE1E5a(std::numeric_limits<double>::quiet_NaN()),
           bgdE1E5b(std::numeric_limits<double>::quiet_NaN())
   {
      msgLenSec = 2.0;
   }


   bool GalINavISC ::
   validate() const
   {
      return !(std::isnan(bgdE1E5a) || std::isnan(bgdE1E5b));
   }


   bool GalINavISC ::
   getISC(const ObsID& oid, double& corrOut)
      const
   {
      if ((oid.band == CarrierBand::L1) && // same as E1
          ((oid.code == TrackingCode::E1B) ||
           (oid.code == TrackingCode::E1C)))
      {
            /** @note Single-frequency E1 corrections can potentially
             * be confusing based on Eq.15 of the SIS-ICD, but it's my
             * understanding (correct or not) that F/NAV is on E5/E5a
             * and I/NAV is on E1/E1B and E5b/E5bI.  So the E1
             * single-frequency correction should come from
             * BGD(E1,E5b) because the clock corrections will come
             * from the same source.  IOW, the clock corrections must
             * come from the same source as the single-frequency
             * correction, and thus I/NAV and BGD(E1,E5b). */
         if (std::isnan(bgdE1E5b))
            return false;
         corrOut = -bgdE1E5b;
         return true;
      }
      else if ((oid.band == CarrierBand::L5) && // same as E5a
               ((oid.code == TrackingCode::E5aI) ||
                (oid.code == TrackingCode::E5aQ)))
      {
         if (std::isnan(bgdE1E5a))
            return false;
         corrOut = -(getGamma(CarrierBand::L1,oid.band) * bgdE1E5a);
         return true;
      }
      else if ((oid.band == CarrierBand::E5b) &&
               ((oid.code == TrackingCode::E5bI) ||
                (oid.code == TrackingCode::E5bQ)))
      {
         if (std::isnan(bgdE1E5b))
            return false;
         corrOut = -(getGamma(CarrierBand::L1,oid.band) * bgdE1E5b);
         return true;
      }
      return false;
   }
}
