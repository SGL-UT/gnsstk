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
#ifndef GNSSTK_GLOFNAVALM_HPP
#define GNSSTK_GLOFNAVALM_HPP

#include "GLOFNavData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements unique to GLONASS Civil F-Nav
       * almanacs. */
   class GLOFNavAlm : public GLOFNavData
   {
   public:
         /// Sets the nav message type, and all other data members to 0.
      GLOFNavAlm();

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
          * @todo Document correct predconditions here.
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

      CommonTime Toa;     ///< Reference time for almanac.
      bool healthBits;    ///< Health flag (C_n, 1 = operable).
      double tau;         ///< Time offset to GLONASS time.
      double lambda;      ///< Longitude of ascending node.
      double deltai;      ///< Correction to mean inclination (mean=63 degrees).
      double ecc;         ///< Eccentricity (epsilon).
      double omega;       ///< Argument of perigee.
      double tEpoch;      ///< Time of ascending node crossing.
      double deltaT;      ///< Correction to mean value of Draconian period.
      double deltaTdot;   ///< Time derivative of deltaT.
      int freq;           ///< Frequency offset (H_n).
   };

      //@}

}

#endif // GNSSTK_GLOFNAVALM_HPP
