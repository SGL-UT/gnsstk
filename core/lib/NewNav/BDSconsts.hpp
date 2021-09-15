//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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

/** @file BDSconsts.hpp Define constants that are specific to BDS but
 * are not specific to one message type (ephemeris, etc). */

namespace gnsstk
{
   namespace bds
   {
         // use enum for integral constants and constexpr for floating
         // point to avoid having the linker allocate storage for the
         // data.

         /// Integer constants for BDS.
      enum BDSIntConsts
      {
         Preamble = 0x712,      ///< Nav message preamble value
         D1MinSF = 1,           ///< Minimum D1 subframe ID
         D1MinEphSF = 1,        ///< Minimum D1 subframe ID containing eph data
         D1MaxEphSF = 3,        ///< Maximum D1 subframe ID containing eph data
         D1AmEpIDSF = 4,        ///< Subframe containing the AmEpID values
         D1MinAlmSF = 4,        ///< Minimum D1 subframe ID containing alm data
         D1MaxAlmSF = 5,        ///< Maximum D1 subframe ID containing alm data
         D1MaxSF = 5,           ///< Maximum D1 subframe ID
         D1MinExpAlmPg = 11,    ///< Minimum D1 SF5 page containing expanded alm
         D1MaxExpAlmPg = 23,    ///< Maximum D1 SF5 page containing expanded alm
         D1ExpAlmHeaPg = 24,    ///< Page containing expanded almanac health

         D2MinSF = 1,           ///< Minimum D2 subframe ID
         D2EphSF = 1,           ///< D2 subframe ID containing ephemeris data
         D2AlmSF = 5,           ///< D2 subframe ID containing almanac data
         D2MaxSF = 5,           ///< Maximum D2 subframe ID
      };
   }
}
