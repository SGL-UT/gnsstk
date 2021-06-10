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

#ifndef GPSTK_MODIP_HPP
#define GPSTK_MODIP_HPP

#include "Position.hpp"

namespace gpstk
{
      /** Wrap the MODIP (Modified Dip Latitude) data used by the
       * NeQuick G ionospheric model.  MODIP is a translated latitude
       * that defines regions used by the NeQuick G ionosphere model
       * that are based on a "pseudo-latitude" that is computed using
       * the geodetic latitude and longitude, and a grid that maps
       * from geodetic position to a modeled latitude.  See Galileo
       * Ionospheric Model, Figure 2. */
   class MODIP
   {
   public:
      MODIP();

         /** Get the MODIP value at a lat and lon in degrees.
          * @param[in] pos The observer geodetic position.
          * @return The modeled latitude. */
      double stModip(const Position& pos);

         /** Perform third-order interpolation across a set of data points.
          * @param[in] z An array of 4 points to perform interpolation over.
          * @param[in] x A fractional offset relative to z[1] that is
          *   the target for interpolation.
          * @return The interpolated value. */
      double interpolate(double z[4], double x);
   };
}

#endif // GPSTK_MODIP_HPP
