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
#ifndef GNSSTK_GALINAVEPH_HPP
#define GNSSTK_GALINAVEPH_HPP

#include "OrbitDataKepler.hpp"
#include "GalHealthStatus.hpp"
#include "GalDataValid.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to GPS LNav ephemerides.
   class GalINavEph : public OrbitDataKepler
   {
   public:
         /// Sets the nav message type and all other data members to 0.
      GalINavEph();

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
          * @return transmit time of most recent message type + 2s.
          */
      CommonTime getUserTime() const override;

         /** Fill the beginFit and endFit values for this object.
          * @pre Toe, iodc, fitIntFlag and xmitTime must all be set. */
      void fixFit();

         /** Dump SV status information (e.g. health).
          * @param[in,out] s The stream to write the data to. */
      void dumpSVStatus(std::ostream& s) const override;

      double bgdE5aE1;   ///< Group delay in seconds between E5a and E1.
      double bgdE5bE1;   ///< Group delay in seconds between E5b and E1.
      uint8_t sisaIndex; ///< Signal in space accuracy index (OS-SIS-ICD tbl 76)
      uint8_t svid;      ///< SVID field from page type 1 (or PRN if not avail)
      CommonTime xmit2;  ///< Transmit time for word type 2.
      CommonTime xmit3;  ///< Transmit time for word type 3.
      CommonTime xmit4;  ///< Transmit time for word type 4.
      CommonTime xmit5;  ///< Transmit time for word type 5.
      uint16_t iodnav1;  ///< IODnav for word type 1.
      uint16_t iodnav2;  ///< IODnav for word type 2.
      uint16_t iodnav3;  ///< IODnav for word type 3.
      uint16_t iodnav4;  ///< IODnav for word type 4.
      GalHealthStatus hsE5b; ///< Health status for E5b.
      GalHealthStatus hsE1B; ///< Health status for E1B.
      GalDataValid dvsE5b;   ///< Data validity status for E5b.
      GalDataValid dvsE1B;   ///< Data validity status for E1B.
   };

      //@}

}

#endif // GNSSTK_GALINAVEPH_HPP
