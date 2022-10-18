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

#include "SimpleTropModel.hpp"
#include "GPSEllipsoid.hpp"

namespace gnsstk
{
   SimpleTropModel::SimpleTropModel()
   {
      //setWeather(20.0, 980.0, 50.0);
      Cwetdelay = 0.122382715318184;
      Cdrydelay = 2.235486646978727;
      Cwetmap = 1.000282213715744;
      Cdrymap = 1.001012704615527;
      valid = true;
   }


   SimpleTropModel::SimpleTropModel(const WxObservation& wx)
   {
      setWeather(wx);
      valid = true;
   }


   SimpleTropModel::SimpleTropModel(const double& T,
                                    const double& P,
                                    const double& H)
   {
      setWeather(T,P,H);
      valid = true;
   }


   void SimpleTropModel::setWeather(const double& T,
                                    const double& P,
                                    const double& H)
   {
      TropModel::setWeather(T,P,H);
      GPSEllipsoid ell;
      Cdrydelay = 2.343*(press/1013.25)*(temp-3.96)/temp;
      double tks = temp * temp;
      Cwetdelay = 8.952/tks*humid*std::exp(-37.2465+0.213166*temp-(0.256908e-3)*tks);
      Cdrymap =1.0+(0.15)*148.98*(temp-3.96)/ell.a();
      Cwetmap =1.0+(0.15)*12000.0/ell.a();
      valid = true;
   }


   void SimpleTropModel::setWeather(const WxObservation& wx)
   {
      TropModel::setWeather(wx);
   }


   double SimpleTropModel::dry_zenith_delay() const
   {
      THROW_IF_INVALID();
      return Cdrydelay;
   }


   double SimpleTropModel::wet_zenith_delay() const
   {
      THROW_IF_INVALID();
      return Cwetdelay;
   }


   double SimpleTropModel::dry_mapping_function(double elevation) const
   {
      THROW_IF_INVALID();

      if(elevation < 0.0) return 0.0;

      double d = std::cos(elevation*DEG_TO_RAD);
      d /= Cdrymap;
      return (1.0/SQRT(1.0-d*d));
   }


   double SimpleTropModel::wet_mapping_function(double elevation) const
   {
      THROW_IF_INVALID();

      if(elevation < 0.0) return 0.0;

      double d = std::cos(elevation*DEG_TO_RAD);
      d /= Cwetmap;
      return (1.0/SQRT(1.0-d*d));
   }

}
