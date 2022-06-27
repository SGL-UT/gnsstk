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
#ifndef GNSSTK_FACTORYCONTROL_HPP
#define GNSSTK_FACTORYCONTROL_HPP

#include "TimeOffsetFilter.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class to use for configuring NavDataFactory and
       * PNBNavDataFactory objects.  The parameters may not always
       * apply to any given factory. */
   class FactoryControl
   {
   public:
         /// Initialize data to reasonable defaults.
      FactoryControl()
            : bdsTimeZZfilt(false), timeOffsFilt(TimeOffsetFilter::NoFilt)
      {}

         /** If true, ignore BeiDou time offsets with A0 and A1 terms
          * both zero. */
      bool bdsTimeZZfilt;

         /** Specify how NavDataFactoryWithStore::addNavData() should
          * process TimeOffsetData objects. 
          * @note This only applies to classes derived from
          *   StdNavTimeOffset, which currently excludes
          *   RinexTimeOffset and GLOFNavUT1TimeOffset.
          * @todo Determine if RinexTimeOffset or GLOFNavUT1TimeOffset
          *   can be refactored so they derive from StdNavTimeOffset,
          *   or at the very least can/should be filtered via
          *   TimeOffsetUnique in NavDataFactoryWithStore. */
      TimeOffsetFilter timeOffsFilt;
   };

      //@}

} // namespace gnsstk

#endif // GNSSTK_FACTORYCONTROL_HPP
