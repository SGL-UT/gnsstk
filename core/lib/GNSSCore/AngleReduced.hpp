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

#ifndef GNSSTK_ANGLEREDUCED_HPP
#define GNSSTK_ANGLEREDUCED_HPP

#include <limits>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace gnsstk
{
      /// @ingroup Geodetic
      //@{

      /** Wrap data for just the sine and cosine of an angle.
       * Intended to be used in geometry where the actual angle isn't
       * used as a term itself, but only sine and/or cosine are.  This
       * saves a bit of computing time by storing the sin and cos of
       * an angle when such values are used repeatedly in geometric
       * equations, rather than recomputing the value multiple times
       * over the course of a method/function. */
   class AngleReduced
   {
   public:
         /** Because the angle can be initialized via a variety of
          * different values that are the same type, we use this enum
          * to indicate in the constructor what type is being
          * passed. */
      enum Type
      {
         Rad,  ///< Value is in radians.
         Deg,  ///< Value is in degrees.
         Sin,  ///< Value is the sine of the angle.
         Cos   ///< Value is the cosine of the angle.
      };

         /// Initialize all data to NaN.
      AngleReduced();

         /** Initialize using an angular value.  The sine and cosine
          * will be derived from each other using the Pythagorean
          * identity sin**2+cos**2=1.
          * @param[in] v The value to set.
          * @param[in] t The type of datum contained in v.
          * @post sin and cos are set. */
      AngleReduced(double v, Type t)
      { setValue(v, t); }

         /** Initialize using the sine and cosine values.
          * @param[in] s The sine value of the angle being represented.
          * @param[in] c The cosine value of the angle being represented.
          * @post sin and cos are set. */
      AngleReduced(double s, double c)
            : sine(s), cosine(c)
      {}

         /// Standard equality operator.
      inline bool operator==(const AngleReduced& right) const
      { return ((sine == right.sine) && (cosine == right.cosine)); }

         /** Set all values from a single angle datum.
          * @param[in] v The value to set.
          * @param[in] t The type of datum contained in v.
          * @post sin and cos are set. */
      void setValue(double v, Type t);

         /// Get the sine of this angle.
      inline double sin() const
      { return sine; }

         /// Get the cosine of this angle.
      inline double cos() const
      { return cosine; }

         /// Return a string containing the data separated by commas (sin,cos).
      inline std::string asString() const;

   protected:
      double sine;   ///< The sine of the angle.
      double cosine; ///< The cosine of the angle.
   }; // class AngleReduced


   inline std::ostream& operator<<(std::ostream& s, const AngleReduced& a)
   {
      s << std::setprecision(20) << a.sin() << "," << a.cos();
      return s;
   }


   std::string AngleReduced ::
   asString() const
   {
      std::ostringstream s;
      s << *this;
      return s.str();
   }

} // namespace gnsstk

namespace std
{
   inline double sin(gnsstk::AngleReduced x)
   { return x.sin(); }
   inline double cos(gnsstk::AngleReduced x)
   { return x.cos(); }
}

#endif // GNSSTK_ANGLEREDUCED_HPP
