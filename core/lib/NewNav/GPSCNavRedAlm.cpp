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
#include "GPSCNavRedAlm.hpp"

using namespace std;

namespace gpstk
{
   GPSCNavRedAlm ::
   GPSCNavRedAlm()
         : deltaA(0),
           phi0(0)
   {
   }


   bool GPSCNavRedAlm ::
   validate() const
   {
      return GPSCNavAlm::validate();
   }


   void GPSCNavRedAlm ::
   fixValues()
   {
      if (signal.sat.system == SatelliteSystem::QZSS)
      {
         M0 = phi0;
         w = refwQZSS;
         ecc = refEccQZSS;
         A = refAQZSS + deltaA;
         Ahalf = ::sqrt(A);
         deltai = refdeltaiQZSS;
         i0 = refioffsetQZSS + deltai;
         OMEGAdot = refOMEGAdotQZSS;
      }
      else
      {
            // GPS
            // IS-GPS-200 gives the PHI0 value as being the sum of
            // M0+w, which is probably fine given the equations in
            // that interface spec, but I'm not convinced this is the
            // right way to handle that in the context of the
            // OrbitDataKepler implementation.  That said, table
            // 5.5.2-9 in IS-QZSS suggests that using a 0 argument of
            // perigee (w) is what you do for GPS.
         M0 = phi0;
         w = refwGPS;
         ecc = refEccGPS;
         A = refAGPS + deltaA;
         Ahalf = ::sqrt(A);
         deltai = refdeltaiGPS;
         i0 = refioffsetGPS + deltai;
         OMEGAdot = refOMEGAdotGPS;
      }
   }
}
