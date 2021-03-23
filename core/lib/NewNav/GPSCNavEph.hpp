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
#ifndef GPSTK_GPSCNAVEPH_HPP
#define GPSTK_GPSCNAVEPH_HPP

#include "GPSCNavData.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements unique to GPS CNav ephemerides.
       * @note The standard xmitTime data member (in OrbitDataKepler)
       *   corresponds to the transmit time of message type 10. */
   class GPSCNavEph : public GPSCNavData
   {
   public:
         /// Sets the nav message type and all other data members to 0.
      GPSCNavEph();

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
          * @return most recent transmit time + 12s or 6s, depending
          *   on whether this is CNAV on L5 or on L2.
          */
      CommonTime getUserTime() const override;

         /** Fill the beginFit and endFit values for this object.
          * @pre xmitTime, xmit11, xmitClk, Toe must all be set. */
      void fixFit();

         /** Dump SV status information (e.g. health).
          * @param[in,out] s The stream to write the data to. */
      void dumpSVStatus(std::ostream& s) const override;

      uint32_t pre11;     ///< The preamble from the start of message type 11.
      uint32_t preClk;    ///< The preamble from the start of the clock message.
         /// @note The health flags are true if unhealthy.
      bool healthL1;      ///< L1 signal health from message type 10.
      bool healthL2;      ///< L2 signal health from message type 10.
      bool healthL5;      ///< L5 signal health from message type 10.
      int8_t uraED;       ///< 5-bit URA index from message type 10.
      int8_t uraNED0;     ///< non-elevation dependent URA from clock message.
      uint8_t uraNED1;    ///< non-elevation dependent URA from clock message.
      uint8_t uraNED2;    ///< non-elevation dependent URA from clock message.
      bool alert11;       ///< Alert flag from message type 11.
      bool alertClk;      ///< Alert flag from the clock message.
      bool integStat;     ///< Integrity status flag.
      bool phasingL2C;    ///< L2C phasing
      double deltaA;      ///< Semi-major axis relative to reference (Aref).
      double dOMEGAdot;   ///< Rate of right ascension relative to -2.6e-9*pi.
      CommonTime top;     ///< Time of prediction.
      CommonTime xmit11;  ///< Transmit time for message 11.
      CommonTime xmitClk; ///< Transmit time for the clock message.
   };

      //@}

}

#endif // GPSTK_GPSCNAVEPH_HPP
