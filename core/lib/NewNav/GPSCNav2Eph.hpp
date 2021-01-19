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
#ifndef GPSTK_GPSCNAV2EPH_HPP
#define GPSTK_GPSCNAV2EPH_HPP

#include "OrbitDataKepler.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to GPS CNav2 ephemerides.
   class GPSCNav2Eph : public OrbitDataKepler
   {
   public:
         /// Aref value defined in IS-GPS-800 (meters).
      static const double refAGPS;
         /// OMEGA dot reference value defined in IS-GPS-800 (radians).
      static const double refOMEGAdotGPS;

         /// Sets the nav message type and all other data members to 0.
      GPSCNav2Eph();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Fill the beginFit and endFit values for this object.
          * @pre xmitTime, xmit11, xmitClk, Toe must all be set. */
      void fixFit();

         /** Dump SV status information (e.g. health).
          * @param[in,out] s The stream to write the data to. */
      void dumpSVStatus(std::ostream& s) const override;

      uint8_t itow;       ///< Interval time of week.
         /// @note The health flags are true if unhealthy.
      bool healthL1C;     ///< L1C signal health.
      int8_t uraED;       ///< 5-bit URA index from message type 10.
      int8_t uraNED0;     ///< non-elevation dependent URA from clock message.
      uint8_t uraNED1;    ///< non-elevation dependent URA from clock message.
      uint8_t uraNED2;    ///< non-elevation dependent URA from clock message.
      bool integStat;     ///< Integrity status flag.
      double deltaA;      ///< Semi-major axis relative to reference (Aref).
      double dOMEGAdot;   ///< Rate of right ascension relative to -2.6e-9*pi.
      CommonTime top;     ///< Time of prediction.
      double tgd;         ///< Ionospheric group delay in seconds. NaN=invalid.
      double iscL1CP;     ///< Inter-signal correction for L1CP.
      double iscL1CD;     ///< Inter-signal correction for L1CD.
   };

      //@}

}

#endif // GPSTK_GPSCNAV2EPH_HPP
