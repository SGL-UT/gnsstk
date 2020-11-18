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
#include "NavSignalID.hpp"

namespace gpstk
{
   NavSignalID ::
   NavSignalID()
         : system(SatelliteSystem::Unknown),
           carrier(CarrierBand::Unknown),
           code(TrackingCode::Unknown),
           nav(NavType::Unknown)
   {
   }


   NavSignalID ::
   NavSignalID(SatelliteSystem sys, CarrierBand car, TrackingCode track,
               NavType nmt)
         : system(sys),
           carrier(car),
           code(track),
           nav(nmt)
   {
   }


   int NavSignalID ::
   order(const NavSignalID& right) const
   {
      if (system < right.system) return -1;
      if (system > right.system) return 1;
      if ((carrier != CarrierBand::Any) && (right.carrier != CarrierBand::Any))
      {
         if (carrier < right.carrier) return -1;
         if (carrier > right.carrier) return 1;
      }
      if ((code != TrackingCode::Any) && (right.code != TrackingCode::Any))
      {
         if (code < right.code) return -1;
         if (code > right.code) return 1;
      }
      if ((nav != NavType::Any) && (right.nav != NavType::Any))
      {
         if (nav < right.nav) return -1;
         if (nav > right.nav) return 1;
      }
      return 0;
   }


   bool NavSignalID ::
   isWild() const
   {
      return ((carrier == CarrierBand::Any) ||
              (code == TrackingCode::Any) ||
              (nav == NavType::Any));
   }
}
