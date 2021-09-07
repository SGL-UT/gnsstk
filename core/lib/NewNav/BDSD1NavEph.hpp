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
#ifndef GNSSTK_BDSD1NAVEPH_HPP
#define GNSSTK_BDSD1NAVEPH_HPP

#include "BDSD1NavData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements unique to BDS D1NAV ephemerides.
       * @note The standard xmitTime data member (in OrbitDataKepler)
       *   corresponds to the transmit time of subframe 1. */
   class BDSD1NavEph : public BDSD1NavData
   {
   public:
         /// Sets the nav message type and all other data members to 0.
      BDSD1NavEph();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.
          * @return most recent transmit time + 6s.
          */
      CommonTime getUserTime() const override;

         /** Fill the beginFit and endFit values for this object.
          * The fit interval isn't specified in the ICD, so we chose
          * to set the fit interval based on empirical data, which is
          * +/- 2 hours for a URE of 2m \cite johnson
          * @pre Toe and xmitTime must be set.
          * @post beginFit and endFit are set. */
      void fixFit();

         /** Dump SV status information (e.g. health).
          * @param[in,out] s The stream to write the data to. */
      void dumpSVStatus(std::ostream& s) const override;

         /** Get age of data in hours from AODC.
          * @param[in] aod The AODC or AODE value from the subframe.
          * @return The AOD in hours.  (unsigned)-1 is returned if the
          *   input aod is >=32 which is beyond the valid range of the
          *   navigation data. */
      static unsigned getAOD(uint8_t aod);

      uint32_t pre2;      ///< The preamble from the start of subframe 2.
      uint32_t pre3;      ///< The preamble from the start of subframe 3.
      uint32_t rev2;      ///< The rev field from word 1 of subframe 2.
      uint32_t rev3;      ///< The rev field from word 1 of subframe 3.
      uint32_t sow2;      ///< Seconds of week from word 1-2 of subframe 2.
      uint32_t sow3;      ///< Seconds of week from word 1-2 of subframe 3.
         /// @note The health flag is true if unhealthy.
      bool satH1;         ///< Autonomous satellite health flag.
      uint8_t aodc;       ///< Age of data - clock.
      uint8_t aode;       ///< Age of data - ephemeris.
         /** @note The BeiDou D1 URA indices match the GPS URA
          * index->URA mappings, which are defined in
          * SV_ACCURACY_GPS_MIN_INDEX and
          * SV_ACCURACY_GPS_MAX_INDEX. */
      uint8_t uraIndex;   ///< 4-bit URA index from subframe 1.
      CommonTime xmit2;   ///< Transmit time for subframe 2.
      CommonTime xmit3;   ///< Transmit time for subframe 3.
      double tgd1;        ///< Group delay differential on B1I.
      double tgd2;        ///< Group delay differential on B2I.
   };

      //@}

}

#endif // GNSSTK_BDSD1NAVEPH_HPP
