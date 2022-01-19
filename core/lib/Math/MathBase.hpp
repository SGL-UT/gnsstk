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

/// @file MathBase.hpp
/// Basic math \#defines (ABS, SQRT, etc)

#ifndef GNSSTK_MATH_BASE_HPP
#define GNSSTK_MATH_BASE_HPP

//@fix MSVC doesnt like std::sqrt or std::abs, and disabling
//extensions allows abs(double) to be used instead of fabs()
#ifdef _MSC_VER
#undef _MSC_EXTENSIONS
#endif
#include <cmath>
#ifdef _MSC_VER
#define _MSC_EXTENSIONS
#endif

#ifdef ABS
#undef ABS
#endif
#ifdef SQRT
#undef SQRT
#endif
#ifdef MAX
#undef MAX
#endif

namespace gnsstk
{
// do Doxygen elsewhere
#ifdef _MSC_VER
#define ABS(x)  ::abs(x)
#define SQRT(x) ::sqrt(x)
#define MAX(x,y) std::max(x,y)
#else
#define ABS(x)  std::abs(x)
#define SQRT(x) std::sqrt(x)
#define MAX(x,y) std::max(x,y)
#endif

}  // namespace gnsstk
#endif
