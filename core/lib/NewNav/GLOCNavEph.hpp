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
#ifndef GNSSTK_GLOCNAVEPH_HPP
#define GNSSTK_GLOCNAVEPH_HPP

#include "GLOCNavData.hpp"
#include "GLOCSatType.hpp"
#include "GLOCRegime.hpp"
#include "GLOCNavLTDMP.hpp"
#include "gnsstk_export.h"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements unique to GLONASS Civil C-Nav
       * ephemerides.
       * @see PNBGLOCNavDataFactory for additional details on the
       *   long-term dynamic model parameters. */
   class GLOCNavEph : public GLOCNavData
   {
   public:
         /** Angular velocity of Earth in rad/s.  This differs from
          * the PZ90Ellipsoid value in that it has more precision.
          * Based on almanac tests, it's clear that the ICD authors
          * use the truncated value for almanacs, so we're just going
          * to define and use the different value here. */
      GNSSTK_EXPORT static const double we;

         /// Sets the nav message type and all other data members to 0.
      GLOCNavEph();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @note This does not check the LTDMP, as the ephemeris can
          *   still be used for short-term solutions in the case the
          *   long-term parameters are unusable.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Compute the satellites position and velocity at a time.
          * @note There are three algorithms in Appendix J of
          *   ICD-GLONASS-CDMA General Edition.  We follow the
          *   recommendation therein of using the simplified algorithm
          *   for prediction intervals <=30 minutes, and the long-term
          *   algorithm for prediction intervals between 30 minutes
          *   and 4 hours.  The long-term algorithm requires the data
          *   from the LTDMP strings (31-32), so if those are absent
          *   for a long-term request, getXvt will indicate failure.
          * @param[in] when The time at which to compute the xvt.
          * @param[out] xvt The resulting computed position/velocity.
          * @param[in] oid Value is ignored - GLONASS does not have
          *   distinct transmitters.
          * @return true if successful, false if required nav data was
          *   unavailable. */
      bool getXvt(const CommonTime& when, Xvt& xvt,
                  const ObsID& = ObsID()) override;

         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.
          * @return most recent transmit time + 3s.
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

         /** Convert the coded factor values as described in ICD
          * section 5.2.2.9 into a pseudorange error (sigma). 
          * @param[in] factor The accuracy factor as broadcast.
          * @return the sigma value in meters, or NaN if factor is invalid. */
      static double factorToSigma(int8_t factor);

         /** @return true if this object has Long-Term Dynamic Model
          * Parameters, needed for best results for prediction
          * intervals > 30 minutes. */
      bool haveLTDMP() const
      { return ltdmp.isSVID(header11.svid); }

      CommonTime Toe;   ///< Reference time, combining N4, NT and tb.
      uint8_t N4;       ///< Number of leap years since 1996.
      uint16_t NT;      ///< Day within four-year interval N4.
      GLOCSatType Mj;   ///< What satellite j is and what it transmits.
      uint8_t PS;       ///< Number of strings from this type 10 to the next.
      unsigned long tb; ///< Instant in Moscow time this data relates to.
      uint8_t EjE;      ///< Age of ephemeris (6-hour intervals).
      uint8_t EjT;      ///< Age of clock (6-hour intervals).
      GLOCRegime RjE;   ///< Regime for generation of ephemeris data.
      GLOCRegime RjT;   ///< Regime for generation of clock data.
      int8_t FjE;       ///< Accuracy factors dependent on ephemeris errors.
      int8_t FjT;       ///< Accuracy factors dependent on clock errors.
      double clkBias;   ///< Satellite clock bias in sec (tau^j).
      double freqBias;  ///< Satellite relative frequency bias (gamma^j).
      double driftRate; ///< Half rate of relative deviation of carrier freq.
      double tauc;      ///< Correction for GLONASS to Moscow time.
      double taucdot;   ///< Rate of correction for GLONASS to Moscow time.

      Triple pos;       ///< Satellite position at tb in km.
      Triple vel;       ///< Satellite velocity at tb in km/s.
      Triple acc;       ///< Satellite acceleration at tb in km/s**2.
         /** @todo implement support for this, see ICD-GLONASS-CDMA
          * General Edition Appendix J, and weep. */
      Triple apcOffset; ///< L3OC APC offset from center of mass.
      double tauDelta;  ///< Offset of L3OCP time to L3OCD time.
      double tauGPS;    ///< Fractional part of offset from GPS to GLONASS time.
      GLOCNavLTDMP ltdmp; ///< Long-term dynamic model parameters.

      GLOCNavHeader header11; ///< Header (incl xmit time) data from string 11.
      GLOCNavHeader header12; ///< Header (incl xmit time) data from string 12.

         /** Integration step for Runge-Kutta algorithm (1 minute by default).
          * @note The default value is suggested in ICD-GLONASS-CDMA
          *   General Edition Appendix J. */
      double step;

   private:
         /** Function implementing the derivative of GLONASS orbital model.
          * @see ICD GLONASS CDMA General Description Appendix J.2.1.
          * @param[in] inState The input state vector consisting of
          *   [x, x', y, y', z, z'].
          * @param[in] accel The acceleration values, [x'', y'', z''].
          * @param[in] lt The long-term corrections [ax,ay,az].  Only
          *   used when simplified==false.
          * @param[in] simplified If true, use the simplified
          *   algorithm from appendix J.2.1.  If false, use the
          *   long-term algorithm from J.3.1.
          * @return A vector containing [x',Vx',Y',Vy',Z',Vz']. */
      Vector<double> derivative(const Vector<double>& inState,
                                const Vector<double>& accel,
                                const Vector<double>& lt,
                                bool simplified) const;
   };

      //@}

}

#endif // GNSSTK_GLOCNAVEPH_HPP
