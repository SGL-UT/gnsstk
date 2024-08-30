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
//  Copyright 2004-2023, The Board of Regents of The University of Texas System
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
#ifndef GNSSTK_GPSLNAVNMCT_HPP
#define GNSSTK_GPSLNAVNMCT_HPP

#include <memory>  // make_shared
#include <ostream>

#include "DumpDetail.hpp"
#include "GPSNMCTAI.hpp"
#include "SystemNavData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Wrapper for entries of the Navigation Message Correction Table (NMCT) 
       * in subframe 4, page 13. 
       * 
       * Also contains the Age of Data Offset (AODO) and Toe as provided in subframe 2.
       * The IS-GPS-200 specifies: "If the AODO term is 27900 seconds (i.e., binary 11111), 
       * then the NMCT currently available from the transmitting SV is invalid and shall not be used."
       * It is currently up to users to check the AODO value and assess whether the ERD can be used. 
       * 
       * 
       * For reference, from the IS-GPS-200:
       *   * Figure 20-1. Data Format (sheet 10 of 11)
       *   * 20.3.3.4.4 NMCT Validity Time
       *   * 20.3.3.5.1.9 NMCT
       */
   class GPSLNavNMC : public SystemNavData
   {
   public:
      GPSLNavNMC();

         /// Create a deep copy of this object.
      NavDataPtr clone() const override
      {
         return std::make_shared<GPSLNavNMC>(*this);
      }

         /// @copydoc NavData::validate
      bool validate() const override;

         /// @copydoc NavData::dump
      void dump(std::ostream &s, DumpDetail dl) const override;

         /*! @copydoc NavDat::isSameData()
          *
          * Additional Checks at the GPSLNavNMC Level
          *
          *    <table>
          *     <tr><td>aodo<td>AODO in seconds from subframe 2
          *     <tr><td>Toe<td>Orbit epoch (Time of Ephemeris) from subframe 2
          *     <tr><td>availabilityIndicator<td>Availability Indicator from subframe 4 page 13.
          *     <tr><td>erd<td>Estimated Range Deviation (ERD) for the given subject satellite.
          *     <tr><td>Tnmct<td>Validity time of the NMCT
          *    </table>
          */
      bool isSameData(const NavDataPtr& right, bool ignore_timestamp = false) const override;

         /// Update Tnmct using Toe and AODO.
         ///
         /// Defined by section "20.3.3.4.4 NMCT Validity Time"
         /// of the IS-GPS-200
      void updateTNMCT();

         /// Age of Data Offset (AODO) in seconds provided by subframe 2.
      long aodo = -1;

         /// Time of Ephemeris (Toe) provided by subframe 2
      CommonTime Toe = CommonTime::END_OF_TIME;      ///< Orbit epoch

         /// Provides information on whether the NMCT is avaible and encrypted.
         /// Defaults to NotAvailable
      GPSNMCTAI availabilityIndicator = GPSNMCTAI::NotAvailable;

         /// The Estimated Range Deviation (ERD) for the given subject satellite.
      double erd = std::numeric_limits<double>::quiet_NaN();

         /// The validity time for the NMCT
      CommonTime Tnmct = CommonTime::END_OF_TIME;

   };

      //@}
} // namespace gnsstk

#endif // GNSSTK_GPSLNAVNMCT_HPP
