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

#ifndef GPSTK_ANGLE_HPP
#define GPSTK_ANGLE_HPP

#include "AngleReduced.hpp"

namespace gpstk
{
      /// @ingroup Geodetic
      //@{

      /** Wrap data for an angle, including the angle in degrees,
       * radians, and the sine and cosine of the angle.  This is done
       * for convenience when doing geometric computations that
       * require the use of a lot of trigonometry. */
   class Angle : public AngleReduced
   {
   public:
         /// Initialize all data to NaN.
      Angle();

         /** Initialize from a single value, filling out the rest.
          * @param[in] v The value to set.
          * @param[in] t The type of datum contained in v.
          * @post rad, deg, sin and cos are all set. */
      Angle(double v, Type t)
      { setValue(v, t); }

         /** Initialize from a pair of sin/cos values, filling out the rest.
          * @param[in] s The sine of the angle.
          * @param[in] c The cosine of the angle.
          * @post rad, deg, sin and cos are all set. */
      Angle(double s, double c);

         /** Set all values from a single angle datum.
          * @param[in] v The value to set.
          * @param[in] t The type of datum contained in v.
          * @post rad, deg, sin and cos are all set. */
      void setValue(double v, Type t);

         /** Basic difference.
          * @param[in] right The angle to subtract from this one.
          * @return The resulting angle when differenced with this one. */
      Angle operator-(const Angle& right) const;

         /** Numeric negation of angle. Changes radians to -radians
          * then recomputes the rest. */
      Angle operator-() const
      { return Angle(-radians, Rad); }

         /** Basic addition.
          * @param[in] right The angle to add to this one.
          * @return The resulting angle when added to this one. */
      Angle operator+(const Angle& right) const;

         /// Get the angle in radians.
      inline double rad() const
      { return radians; }

         /// Get the angle in degrees.
      inline double deg() const
      { return degrees; }

         /** Return a string containing the data separated by commas
          * (rad,deg,sin,cos). */
      std::string asString() const;

   protected:
      double radians; ///< The angle in radians.
      double degrees; ///< The angle in degrees.
   }; // class Angle


   inline std::ostream& operator<<(std::ostream& s, const Angle& a)
   {
      s << std::setprecision(20) << a.rad() << "," << a.deg() << "," << a.sin() << ","
        << a.cos();
      return s;
   }

      //@}

} // namespace gpstk

namespace std
{
   inline double sin(gpstk::Angle x)
   { return x.sin(); }
   inline double cos(gpstk::Angle x)
   { return x.cos(); }
}

#endif // GPSTK_ANGLE_HPP
