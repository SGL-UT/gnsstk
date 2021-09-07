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
#ifndef GNSSTK_GPSCNAVTIMEOFFSET_HPP
#define GNSSTK_GPSCNAVTIMEOFFSET_HPP

#include "StdNavTimeOffset.hpp"
#include "TimeSystem.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Defines the class that provide the ability to convert
       * between GPS and a supported time system (UTC, GLONASS,
       * Galileo), using data extracted from GPS civil navigation
       * messages.
       * @note This class is used to encapsulate the data from message
       *   types 33 and 35.  Message type 33 is for UTC conversion and
       *   has additional factors not present in message type 35 (see
       *   the data fields).  Additionally, the parameter subscripts
       *   are different between the two, e.g. A<sub>0-n</sub> in
       *   message type 33 vs A<sub>0GGTO</sub> in message type 35,
       *   however mathematically they are identical terms and generic
       *   names are used in this class to represent the data. */
   class GPSCNavTimeOffset : public StdNavTimeOffset
   {
   public:
         /// Initialize all data to 0.
      GPSCNavTimeOffset();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.
          * @return transmit time + 12s or 6s, depending on whether
          *   this is CNAV on L5 or on L2.
          */
      CommonTime getUserTime() const override;
   };

      //@}

}

#endif // GNSSTK_GPSCNAVUTCTIMEOFFSET_HPP
