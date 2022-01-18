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

/// @file random.hpp
/// Simple random number generator.

#ifndef GNSSTK_GEOMATICS_RANDOM_INCLUDE
#define GNSSTK_GEOMATICS_RANDOM_INCLUDE

namespace gnsstk
{
/// Generate random numbers uniformly distributed from 0 to 1.
double Rand(long seed=0);

/// Generate normally distributed random numbers, zero mean and
/// sqrt of variance sigma.  Uses Box-Muller and Rand().
double RandNorm(double sigma);

/// Return random integers between low and hi. Seed the generator by calling
/// Rand(seed) before this call.
int ARand(int low, int hi);

/// Return random doubles between low and hi. Seed the generator by calling
/// Rand(seed) before this call.
double ARand(double low, double hi);

/// Generate a random walk sequence, given sqrt variance sigma, time step dt
/// and previous point xlast.
double RandomWalk(double dt, double sigma, double xlast);

/// Generate exponentially correlated random numbers, given sqrt variance sigma,
/// time step dt, time constant T, and previous point xlast.
double RandExpCor(double dt, double sigma, double T, double xlast);
}
#endif
