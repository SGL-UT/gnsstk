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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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
#ifndef GPSTK_GPSLNAVHEALTH_HPP
#define GPSTK_GPSLNAVHEALTH_HPP

#include "NavHealthData.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Wrapper for the 6-bit or 8-bit health status in GPS LNav
       * subframe 1, word 3, SV/Page ID 1-32, 51, and 63. */
   class GPSLNavHealth : public NavHealthData
   {
   public:
         /// Initialize to unhealthy using a value typically not seen in health.
      GPSLNavHealth()
            : svHealth(0x80)
      {}

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true always as there is nothing to check in this class.
          */
      bool validate() const override
      { return true; }

         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.
          * @return transmit time + 6s.  In all cases in LNav, the
          *   health bits represented by this class are in a single
          *   subframe.
          */
      CommonTime getUserTime() const override
      { return timeStamp + 6.0; }

         /** Print the contents of this object in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, Detail dl) const override;

         /** Defines the status of NavData::signal, specifically sat
          * (not xmitSat).
          * @return Healthy if no health bits are set. */
      SVHealth getHealth() const override
      { return (svHealth == 0) ? SVHealth::Healthy : SVHealth::Unhealthy; }

         /// 6-bit or 8-bit health.
      uint8_t svHealth;
   };

      //@}

}

#endif // GPSTK_GPSLNAVHEALTH_HPP
