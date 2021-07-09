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
#include "GalFNavISC.hpp"
#include "FreqConv.hpp"

using namespace std;

namespace gpstk
{
   GalFNavISC ::
   GalFNavISC()
   {
         // Galileo F/NAV nominal page transmit time is 10 seconds per
         // OS-SIS-ICD figure 14.
      msgLenSec = 10.0;
   }


   bool GalFNavISC ::
   validate() const
   {
      return !std::isnan(isc);
   }


   bool GalFNavISC ::
   getISC(const ObsID& oid, double& corrOut)
      const
   {
         // if isc is NaN, we definitely can't return anything.
      if (std::isnan(isc))
      {
         return false;
      }
         /** @note Single-frequency E1 corrections can potentially be
          * confusing based on Eq.15 of the SIS-ICD, but it's my
          * understanding (correct or not) that F/NAV is on E5/E5a and
          * I/NAV is on E1/E1B and E5b/E5bI.  So the E1
          * single-frequency correction should come from BGD(E1,E5b)
          * because the clock corrections will come from the same
          * source.  IOW, the clock corrections must come from the
          * same source as the single-frequency correction, and thus
          * I/NAV and BGD(E1,E5b). */
      if ((oid.band == CarrierBand::L5) && // same as E5a
          ((oid.code == TrackingCode::E5aI) ||
           (oid.code == TrackingCode::E5aQ)))
      {
         corrOut = -(getGamma(CarrierBand::L1,oid.band) * isc);
         return true;
      }
      return false;
   }
}
