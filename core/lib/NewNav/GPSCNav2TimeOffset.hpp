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
#ifndef GPSTK_GPSCNAV2TIMEOFFSET_HPP
#define GPSTK_GPSCNAV2TIMEOFFSET_HPP

#include "StdNavTimeOffset.hpp"
#include "TimeSystem.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Defines the class that provide the ability to convert
       * between GPS and a supported time system (UTC, GLONASS,
       * Galileo), using data extracted from GPS civil navigation
       * messages.
       * @note This class is used to encapsulate the data from pages 1
       *   and 2.  Page 1 is for UTC conversion and has additional
       *   factors not present in page 2 (see the data fields).
       *   Additionally, the parameter subscripts are different
       *   between the two, e.g. A<sub>0-n</sub> in page 1 vs
       *   A<sub>0-GGTO</sub> in page 2, however mathematically they
       *   are identical terms and generic names are used in this
       *   class to represent the data. */
   class GPSCNav2TimeOffset : public StdNavTimeOffset
   {
   public:
         /// Initialize all data to 0.
      GPSCNav2TimeOffset();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;
   };

      //@}

}

#endif // GPSTK_GPSCNAV2UTCTIMEOFFSET_HPP
