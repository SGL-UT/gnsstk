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
#ifndef GNSSTK_BDSD2NAVEPH_HPP
#define GNSSTK_BDSD2NAVEPH_HPP

#include "BDSD2NavData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements unique to BDS D2NAV ephemerides.
       * @note The standard xmitTime data member (in OrbitDataKepler)
       *   corresponds to the transmit time of subframe 1 page 1.
       * @note Unlike BDSD1NavEph we don't store the preamble, SOW or
       *   reserved bits from the other (8-9) subframes.  This is
       *   simply a choice - if someone down the road decides to add
       *   it in, there's no reason not to at this point other than
       *   cost/benefit. */
   class BDSD2NavEph : public BDSD2NavData
   {
   public:
         /// Sets the nav message type and all other data members to 0.
      BDSD2NavEph();

         /** Compute the satellites position and velocity at a time.
          * @note This overrides the default OrbitDataKepler
          *   implementation for GEO satellites, MEO still use that
          *   implementation.
          * @param[in] when The time at which to compute the xvt.
          * @param[out] xvt The resulting computed position/velocity.
          * @return true if successful, false if required nav data was
          *   unavailable. */
      bool getXvt(const CommonTime& when, Xvt& xvt) override;

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

         /// @note The health flag is true if unhealthy.
      bool satH1;         ///< Autonomous satellite health flag.
      uint8_t aodc;       ///< Age of data - clock.
      uint8_t aode;       ///< Age of data - ephemeris.
         /** @note The BeiDou D2 URA indices match the GPS URA
          * index->URA mappings, which are defined in
          * SV_ACCURACY_GPS_MIN_INDEX and
          * SV_ACCURACY_GPS_MAX_INDEX. */
      uint8_t uraIndex;   ///< 4-bit URA index from subframe 1.
      double tgd1;        ///< Group delay differential on B1I.
      double tgd2;        ///< Group delay differential on B2I.
   };

      //@}

}

#endif // GNSSTK_BDSD2NAVEPH_HPP
