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

/// @file format.hpp Simple class to encapsulate output format

#ifndef CLASS_FORMAT_HELPER_INCLUDE
#define CLASS_FORMAT_HELPER_INCLUDE

#include <ostream>

namespace gnsstk
{
      /**
       Class format holds 3 flags for format, width and precision; use with
       std ostream operator << */
   class format
   {
      int form;   /// 0=general,1=float,2=scientific, default is 1
      int wide;   ///< width in char
      int prec;   ///< precision in char

   public:
         /// Constructor
      explicit format(int w, int p, int f = 1) : form(f), wide(w), prec(p) {}

         /// Set format to scientific
      format& scientific()
      {
         form = 2;
         return *this;
      }

         /// Set format to scientific
      format& sci()
      {
         form = 2;
         return *this;
      }

         /// Set format to fixed
      format& fixed()
      {
         form = 1;
         return *this;
      }

         /// Set format to fixed
      format& fix()
      {
         form = 1;
         return *this;
      }

         /// Set format to general
      format& general()
      {
         form = 0;
         return *this;
      }

         /// Set format to general
      format& gen()
      {
         form = 0;
         return *this;
      }

         /// Set width to w characters
      format& width(int w)
      {
         wide = w;
         return *this;
      }

         /// Set width to w characters
      format& w(int w)
      {
         wide = w;
         return *this;
      }

         /// Set precision to p digits
      format& precision(int p)
      {
         prec = p;
         return *this;
      }

         /// Set precision to p digits
      format& p(int p)
      {
         prec = p;
         return *this;
      }

         /// Set both width and precision
      format& wp(int w, int p)
      {
         prec = p;
         wide = w;
         return *this;
      }

         /// ostream operator for format
      friend std::ostream& operator<<(std::ostream& os, const format& f);

   }; // end class format

   //---------------------------------------------------------------------------------
} // namespace gnsstk
#endif
