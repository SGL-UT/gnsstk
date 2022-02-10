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
#include "GPSCNavData.hpp"

namespace gnsstk
{
   const double GPSCNavData::refAGPS = 26559710;
   const double GPSCNavData::refOMEGAdotEphGPS = -2.6e-9 * PI;
   const double GPSCNavData::refOMEGAdotRedAlmGPS = -2.6e-9 * PI;
   const double GPSCNavData::refi0GPS = 0.3 * PI;
   const double GPSCNavData::refdeltaiGPS = 0.0056 * PI;
   const double GPSCNavData::refwGPS = 0;
   const double GPSCNavData::refEccGPS = 0;

   const double GPSCNavData::refAQZSS = 42164200;
   const double GPSCNavData::refOMEGAdotEphQZSS = -2.6e-9 * PI;
   const double GPSCNavData::refOMEGAdotRedAlmQZSS = -8.7e-10 * PI;
   const double GPSCNavData::refi0QZSS = 0.25 * PI;
   const double GPSCNavData::refdeltaiQZSS = 0.0056 * PI;
   const double GPSCNavData::refwQZSS = 2.0 * PI * 270.0 / 360.0;
   const double GPSCNavData::refEccQZSS = 0.075;

   GPSCNavData ::
   GPSCNavData()
         : pre(0),
           alert(false)
   {}


   bool GPSCNavData ::
   validate() const
   {
      return ((pre == 0) || (pre == 0x8b));
   }
}
