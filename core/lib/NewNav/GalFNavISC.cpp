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

namespace gnsstk
{
   GalFNavISC ::
   GalFNavISC()
   {
      weekFmt = "%4L(%4l)";
         // Galileo F/NAV nominal page transmit time is 10 seconds per
         // OS-SIS-ICD figure 14.
      msgLenSec = 10.0;
   }


   void GalFNavISC ::
   dumpCorrections(std::ostream& s) const
   {
      const ios::fmtflags oldFlags = s.flags();
      s << "           CORRECTION"
        << endl << endl
        << scientific << setprecision(8) << setfill(' ')
        << setw(20) << left << "BDS(E1,E5a):" << setw(15) << isc << endl;
      s.flags(oldFlags);
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
         /** @note Single-frequency E1 corrections cannot be obtained
          * from F/NAV, only Single-frequency E5a corrections.  This
          * is because F/NAV does not transmit on nor provide clock
          * corrections for E1.  I/NAV must be used to get E1
          * corrections.  See GalINavISC for more. */
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
