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
#ifndef GNSSTK_GLOFNAVEPH_HPP
#define GNSSTK_GLOFNAVEPH_HPP

#include "GLOFNavData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements unique to GLONASS Civil F-Nav
       * ephemerides. */
   class GLOFNavEph : public GLOFNavData
   {
   public:
         /// Sets the nav message type and all other data members to 0.
      GLOFNavEph();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Compute the satellites position and velocity at a time.
          * @todo Make sure this statement is either true or fixed/removed.
          * @note Defaults to using the GPS ellipsoid parameters.
          * @param[in] when The time at which to compute the xvt.
          * @param[out] xvt The resulting computed position/velocity.
          * @return true if successful, false if required nav data was
          *   unavailable. */
      bool getXvt(const CommonTime& when, Xvt& xvt) override;

         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.
          * @return most recent transmit time + 6s.
          */
      CommonTime getUserTime() const override;

         /** Fill the beginFit and endFit values for this object.
          * @pre Toe, interval and timeStamp must all be set. */
      void fixFit();

         /** Print the contents of this NavData object in a (usually)
          * human-readable format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;

         /** Dump contents in terse format (DumpDetail==Terse)
          * @param[in,out] s The stream to write the data to. */
      void dumpTerse(std::ostream& s) const;

         /// Returns the accuracy in meters as defined in Table 4.4 of the ICD.
      double getAccuracy() const;

      CommonTime ref;     ///< Reference time (t_k) for this ephemeris.
      CommonTime xmit3;   ///< Transmit time for string 3.
      CommonTime xmit4;   ///< Transmit time for string 4.
      Triple pos;         ///< Satellite position at tb.
      Triple vel;         ///< Satellite velocity at tb.
      Triple acc;         ///< Satellite acceleration at tb.
      double clkBias;     ///< Satellite clock bias in sec (tau_n).
      double freqBias;    ///< Satellite relative frequency bias (gamma_n).
      uint8_t healthBits; ///< The 3-bit B_n value (look at bit 2 not 0 or 1).
      unsigned tb;        ///< Epoch index with Moscow day.
      unsigned P1;        ///< Flag for interval between adjacent t_b.
      unsigned P2;        ///< Flag of oddness (=1) or evenness (=0) of t_b.
      unsigned P3;        ///< Flag 1=5 almanac sats in frame, 0=4 almanac sats.
      unsigned P4;        ///< Flag 1=ephemeris present/uploaded. 0=nope.
      unsigned interval;  ///< P1 interval (minutes, see PNBGLOFNavDataFactory).
      PCode opStatus;     ///< Operational status flag.
      double tauDelta;    ///< Inter-frequency bias.
      unsigned aod;       ///< Age of data in days (E_n).
      unsigned accIndex;  ///< User accuracy index (F_T).
      unsigned dayCount;  ///< Days since Jan 1 of most recent leap year (N_T).
      CommonTime Toe;     ///< Orbit epoch (t_b).
   };

      //@}

}

#endif // GNSSTK_GLOFNAVEPH_HPP
