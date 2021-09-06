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

#ifndef GPSTK_POSITIONRECORD_HPP
#define GPSTK_POSITIONRECORD_HPP

#include <iostream>
// This include is to make sure dependencies are established correctly
// for SWIG, since this class is used as a template argument.
#include "TabularSatStore.hpp"

namespace gpstk
{
      /// @ingroup GNSSEph
      //@{

      /** Data record for storing position data. See note on units in
       * class PositionStore. */
   class PositionRecord
   {
   public:
      Triple Pos, sigPos;  ///< position (ECEF Cartesian) and sigmas
      Triple Vel, sigVel;  ///< velocity and sigmas
      Triple Acc, sigAcc;  ///< acceleration and sigmas
   };

      /// Output stream operator is used by dump() in TabularSatStore
   std::ostream& operator<<(std::ostream& os, const PositionRecord& cdr)
      throw();

      //@}

}  // namespace gpstk

#endif // GPSTK_POSITIONRECORD_HPP
