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

#ifndef GNSSTK_CLOCKRECORD_HPP
#define GNSSTK_CLOCKRECORD_HPP

#include <iostream>
// This include is to make sure dependencies are established correctly
// for SWIG, since this class is used as a template argument.
#include "TabularSatStore.hpp"

namespace gnsstk
{
      /// @ingroup GNSSEph
      //@{

      /** Data record for storing clock data. See note on units in
       * class ClockSatStore. */
   class ClockRecord
   {
   public:
      double bias, sig_bias;     ///< clock bias and sigma
      double drift, sig_drift;   ///< clock drift and sigma
      double accel, sig_accel;   ///< clock acceleration and sigma
   };

      /// Output stream operator is used by dump() in TabularSatStore
   std::ostream& operator<<(std::ostream& os, const ClockRecord& rec) throw();

      //@}

}  // namespace gnsstk

#endif // GNSSTK_CLOCKRECORD_HPP
