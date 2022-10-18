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
#ifndef GNSSTK_BDSD1NAVTIMEOFFSET_HPP
#define GNSSTK_BDSD1NAVTIMEOFFSET_HPP

#include "StdNavTimeOffset.hpp"
#include "TimeSystem.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Defines the class that provide the ability to convert
       * between BeiDou time (BDT) and a supported time system (UTC,
       * GPS, GLONASS, Galileo), using data extracted from BDS
       * navigation messages.
       * @note The ICD isn't clear on the topic of reference times.
       *   While GPS LNAV has an explicit reference time in its time
       *   offset data, BeiDou does not.  As such, when
       *   PNBBDSD1NavDataFactory decodes this data, it sets the
       *   reference time to our "best guess", which is the start of
       *   the transmit week (based on how t<sub>E</sub> is used).
       * @note This class is used to encapsulate the data from
       *   subframe 5, pages 9 and 10.  Page 10 is for UTC conversion
       *   and has additional factors not present in page 9 (see the
       *   data fields).  Additionally, the parameter subscripts
       *   differ depending on the target time system, however
       *   mathematically they are identical terms and generic names
       *   are used in this class to represent the data. */
   class BDSD1NavTimeOffset : public StdNavTimeOffset
   {
   public:
         /// Initialize all data to 0.
      BDSD1NavTimeOffset();
         /// Create a deep copy of this object.
      NavDataPtr clone() const override
      { return std::make_shared<BDSD1NavTimeOffset>(*this); }

         /** Return the number of seconds prior to the effectivity
          * time (2/3 of a day) during which the standard delta t
          * formula is used. */
      virtual double effStart() const
      { return 57600.0; }
         /** Return the number of seconds after the effectivity time
          * (5/4 of a day) during which the standard delta t formula
          * is used. */
      virtual double effEnd() const
      { return 108000.0; }

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;
   };

      //@}

}

#endif // GNSSTK_BDSD1NAVUTCTIMEOFFSET_HPP
