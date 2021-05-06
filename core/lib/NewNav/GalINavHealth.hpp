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
#ifndef GPSTK_GALINAVHEALTH_HPP
#define GPSTK_GALINAVHEALTH_HPP

#include "NavHealthData.hpp"
#include "GalHealthStatus.hpp"
#include "GalDataValid.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Wrapper for the combined Signal Health Status (SHS) and Data
       * Validity Status (DVS) associated with Galileo satellites. */
   class GalINavHealth : public NavHealthData
   {
   public:
         /// Initialize to unhealthy using a value typically not seen in health.
      GalINavHealth();

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
          * @note Galileo-OS-SDD refers to a SIS status as "Marginal",
          *   where we use the SVHealth enumeration name "Degraded".
          * @return Rolled-up health status based on internal data. */
      SVHealth getHealth() const override;

         /** Static version of getHealth for convenience.
          * @param[in] shs The Signal Health Status of a given signal.
          * @param[in] dvs The Data Validity Status of a given signal.
          * @param[in] sisa The Signal In Space Accuracy Index of a given signal
          * @note Galileo-OS-SDD refers to a SIS status as "Marginal",
          *   where we use the SVHealth enumeration name "Degraded".
          * @return Rolled-up health status based on internal data. */
      static SVHealth galHealth(GalHealthStatus shs, GalDataValid dvs,
                                uint8_t sisa);

         /// Signal health status (SHS)
      GalHealthStatus sigHealthStatus;
         /// Data Validity Status (DVS)
      GalDataValid dataValidityStatus;
         /// Signal in space accuracy index (OS-SIS-ICD tbl 76)
      uint8_t sisaIndex;
   };

      //@}

}

#endif // GPSTK_GALINAVHEALTH_HPP
