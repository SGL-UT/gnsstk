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

/* This is where all platform specific includes, defines and crud should go.
   Unless, of course, it has to go somewhere else. :-)
*/

#ifndef GNSSTK_GNSSTKPLATFORM_H
#define GNSSTK_GNSSTKPLATFORM_H

#ifdef _MSC_VER

#include <cstdlib>

#define HAVE_STRING_H 1
#define STDC_HEADERS  1

#include <stdint.h>

//#include <sys/types.h>
#include <sys/timeb.h>

#elif defined __SUNPRO_CC

#include <sys/types.h>
#include <sys/timeb.h>

#else

#include <stdint.h>

#endif  // _MSC_VER

#endif  // GNSSTK_GNSSTKPLATFORM_H
