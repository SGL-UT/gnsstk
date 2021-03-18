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
#ifndef GPSTK_GPSCNAV2HEALTH_HPP
#define GPSTK_GPSCNAV2HEALTH_HPP

#include "NavHealthData.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Wrapper for the 1-bit health status in GPS CNav2 subframe 2 or 3.
   class GPSCNav2Health : public NavHealthData
   {
   public:
         /// Initialize to unhealthy.
      GPSCNav2Health();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true always as there is nothing to check in this class.
          */
      bool validate() const override
      { return true; }

         /** Print the contents of this object in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;

         /** Defines the status of NavData::signal, specifically sat
          * (not xmitSat).
          * @return Healthy if no health bits are set. */
      SVHealth getHealth() const override
      { return (health ? SVHealth::Unhealthy : SVHealth::Healthy); }

         /** Alter the message length depending on whether this health
          * data as having originated from an ephemeris or almanac
          * message.
          * @param[in] e If true, this health was derived from ephemeris data.
          */
      void setEph(bool e);

         /// 1-bit health.
      bool health;
   };

      //@}

}

#endif // GPSTK_GPSCNAV2HEALTH_HPP
