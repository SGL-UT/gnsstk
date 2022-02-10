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
#include <math.h>
#include "GLOFNavISC.hpp"
#include "TimeString.hpp"
#include "YDSTime.hpp"
#include "FreqConv.hpp"

using namespace std;

namespace gnsstk
{
   GLOFNavISC ::
   GLOFNavISC()
   {
      msgLenSec = 2.0;
      iscLabel = "dtau_n";
   }


   bool GLOFNavISC ::
   validate() const
   {
         /// @todo implement some checking.
      return true;
   }


   bool GLOFNavISC ::
   getISC(const ObsID& oid1, const ObsID& oid2, double& corrOut) const
   {
      if (isnan(isc))
         return false;
      if ((oid1.code == TrackingCode::Standard) &&
          (oid2.code == TrackingCode::Standard))
      {
         if ((oid1.band == CarrierBand::G1) && (oid2.band == CarrierBand::G2))
         {
               /** @todo I'm not sure if I have this polarity right.
                * dtau_n=t_f2-t_f1. */
            corrOut = isc;
            return true;
         }
         else if ((oid1.band == CarrierBand::G2) &&
                  (oid2.band == CarrierBand::G1))
         {
            corrOut = -isc;
            return true;
         }
      }
      return false;
   }
}
