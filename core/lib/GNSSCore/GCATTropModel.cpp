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


#include "GCATTropModel.hpp"

namespace gnsstk
{
   GCATTropModel::GCATTropModel(const double& ht)
   {
      setReceiverHeight(ht);
      valid = true;
   }


   double GCATTropModel::correction(double elevation) const
   {
      THROW_IF_INVALID();

      if(elevation < 5.0) return 0.0;

      return ( (dry_zenith_delay() + wet_zenith_delay()) *
               mapping_function(elevation));
   }


   double GCATTropModel::correction( const Position& RX,
                                     const Position& SV )
   {

      try
      {
         setReceiverHeight( RX.getAltitude() );
      }
      catch(GeometryException& e)
      {
         valid = false;
      }

      if(!valid) throw InvalidTropModel("Invalid model");

      double c;
      try
      {
         c = correction(RX.elevationGeodetic(SV));
      }
      catch(InvalidTropModel& e)
      {
         GNSSTK_RETHROW(e);
      }

      return c;

   }


   double GCATTropModel::correction( const Xvt& RX,
                                     const Xvt& SV,
                                     const CommonTime& tt )
   {

      Position R(RX),S(SV);

      return GCATTropModel::correction(R,S);
   }


   double GCATTropModel::dry_zenith_delay() const
   {
      THROW_IF_INVALID();

      double ddry(2.29951*std::exp((-0.000116 * gcatHeight) ));

      return ddry;
   }


   double GCATTropModel::mapping_function(double elevation) const
   {
      THROW_IF_INVALID();

      if(elevation < 5.0) return 0.0;

      double d = std::sin(elevation*DEG_TO_RAD);
      d = SQRT(0.002001+(d*d));

      return (1.001/d);
   }


   void GCATTropModel::setReceiverHeight(const double& ht)
   {
      gcatHeight = ht;
      valid = true;
   }

}
