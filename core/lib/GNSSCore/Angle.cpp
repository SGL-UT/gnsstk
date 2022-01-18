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

#include <limits>
#include <math.h>
#include "Angle.hpp"
#include "GNSSconstants.hpp"

namespace gnsstk
{
   Angle ::
   Angle()
         : radians(std::numeric_limits<double>::quiet_NaN()),
           degrees(std::numeric_limits<double>::quiet_NaN()),
           tangent(std::numeric_limits<double>::quiet_NaN()),
           semicircles(std::numeric_limits<double>::quiet_NaN())
   {
   }


   Angle ::
   Angle(double s, double c)
         : AngleReduced(s,c)
   {
      radians = atan2(s,c);
      degrees = radians * RAD2DEG;
      semicircles = radians / PI;
      tangent = ::tan(radians);
   }


   void Angle ::
   setValue(double v, AngleType t)
   {
      switch (t)
      {
         case AngleType::Rad:
            radians = v;
            degrees = v * RAD2DEG;
            semicircles = v / PI;
            sine = ::sin(radians);
            cosine = ::cos(radians);
            tangent = ::tan(radians);
            break;
         case AngleType::Deg:
            radians = v * DEG2RAD;
            degrees = v;
            semicircles = v / 180.0;
            sine = ::sin(radians);
            cosine = ::cos(radians);
            tangent = ::tan(radians);
            break;
         case AngleType::SemiCircle:
            radians = v * PI;
            degrees = v * 180.0;
            semicircles = v;
            sine = ::sin(radians);
            cosine = ::cos(radians);
            tangent = ::tan(radians);
            break;
         case AngleType::Sin:
            radians = asin(v);
            degrees = radians * RAD2DEG;
            semicircles = radians / PI;
            sine = v;
            cosine = ::sqrt(1-v*v);
            tangent = ::tan(radians);
            break;
         case AngleType::Cos:
            radians = acos(v);
            degrees = radians * RAD2DEG;
            semicircles = radians / PI;
            sine = ::sqrt(1-v*v);
            cosine = v;
            tangent = ::tan(radians);
            break;
         default:
            GNSSTK_THROW(Exception("Invalid type in setValue"));
            break;
      }
   } // setValue


   Angle Angle ::
   operator-(const Angle& right) const
   {
      double newrad = radians - right.radians;
      Angle rv(newrad, AngleType::Rad);
      return rv;
   }


   Angle Angle ::
   operator+(const Angle& right) const
   {
      double newrad = radians + right.radians;
      Angle rv(newrad, AngleType::Rad);
      return rv;
   }


   std::string Angle ::
   asString() const
   {
      std::ostringstream s;
      s << *this;
      return s.str();
   }

} // namespace gnsstk
