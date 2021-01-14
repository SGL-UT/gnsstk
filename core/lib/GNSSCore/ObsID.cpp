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

/// @file ObsID.cpp
/// gpstk::ObsID - Identifies types of observations

#include <math.h>
#include "ObsID.hpp"

namespace gpstk
{
   // Convenience output method
   std::ostream& ObsID::dump(std::ostream& s) const
   {
      s << ObsID::cbDesc[band] << " "
        << ObsID::tcDesc[code] << " "
        << ObsID::otDesc[type];
      return s;
   } // ObsID::dump()



   // Equality requires all fields to be the same unless the field is unknown
   bool ObsID::operator==(const ObsID& right) const
   {
         // combined mask, which basically means that a 0 in either
         // mask is a wildcard.
      uint32_t cmask = mcodeMask & right.mcodeMask;
      return ((type == ObservationType::Any ||
               right.type == ObservationType::Any || type == right.type) &&
              (band == CarrierBand::Any || right.band == CarrierBand::Any ||
               band == right.band) &&
              (code == TrackingCode::Any || right.code == TrackingCode::Any ||
               code == right.code) &&
              (freqOffsWild == true || right.freqOffsWild == true ||
               freqOffs == right.freqOffs) &&
              ((mcode & mcodeMask) == (right.mcode & cmask)));
   }


   // This ordering is somewhat arbitrary but is required to be able
   // to use an ObsID as an index to a std::map. If an application needs
   // some other ordering, inherit and override this function.
   bool ObsID::operator<(const ObsID& right) const
   {
      if (band < right.band) return true;
      if (right.band < band) return false;
      if (code < right.code) return true;
      if (right.code < code) return false;
      if (type < right.type) return true;
      if (right.type < type) return false;
      if (!freqOffsWild && !right.freqOffsWild)
      {
         if (freqOffs < right.freqOffs) return true;
         if (right.freqOffs < freqOffs) return false;
      }
         // combined mask, which basically means that a 0 in either
         // mask is a wildcard.
      int64_t cmask = mcodeMask & right.mcodeMask;
      return ((mcode & cmask) < (right.mcode & cmask));
   }


   void ObsID ::
   makeWild()
   {
      type = ObservationType::Any;
      band = CarrierBand::Any;
      code = TrackingCode::Any;
      freqOffsWild = true;
      mcodeMask = 0;
   }


   bool ObsID ::
   isWild() const
   {
      return ((type == ObservationType::Any) ||
              (band == CarrierBand::Any) ||
              (code == TrackingCode::Any) ||
              (mcodeMask != -1) ||
              freqOffsWild);
   }


   namespace StringUtils
   {
      // convert this object to a string representation
      std::string asString(const ObsID& p)
      {
         std::ostringstream oss;
         p.dump(oss);
         return oss.str();
      }
   }


   // stream output for ObsID
   std::ostream& operator<<(std::ostream& s, const ObsID& p)
   {
      p.dump(s);
      return s;
   }
}
