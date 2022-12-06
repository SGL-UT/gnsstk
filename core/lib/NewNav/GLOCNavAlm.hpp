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
#ifndef GNSSTK_GLOCNAVALM_HPP
#define GNSSTK_GLOCNAVALM_HPP

#include "GLOCNavData.hpp"
#include "PZ90Ellipsoid.hpp"
#include "GLOCSatType.hpp"
#include "GLOCOrbitType.hpp"
#include "gnsstk_export.h"

// test class forward declaration for a friend.
class GLOCNavAlm_T;

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements unique to GLONASS Civil C-Nav
       * almanacs.
       * @note Apologies for not thoroughly documenting every little
       *   parameter in here, but neither did the GLONASS ICD.  In any
       *   case, many of them aren't visible to the end user. */
   class GLOCNavAlm : public GLOCNavData
   {
   public:
         /// Ellipsoid parameters used by GLONASS
      GNSSTK_EXPORT static const PZ90Ellipsoid ell;
         /// Gravitational constant.
      GNSSTK_EXPORT static const double GM;
         /// Equatorial radius of Earth in km.
      GNSSTK_EXPORT static const double ae;
         /// Earth's rotation rate (rad/s).
      GNSSTK_EXPORT static constexpr double omegaE = 0.7292115e-4;
         /// i<sub>ap</cp> as defined in ICD-GLONASS-CDMA-L3 section 5.3.2.12
      GNSSTK_EXPORT static const double iav;
         /// T<sub>av</cp> as defined in ICD-GLONASS-CDMA-L3 section 5.3.2.15
      GNSSTK_EXPORT static const double Tav;
         /// Second degree coefficient of normal potential, table 4.1.
      GNSSTK_EXPORT static const double J20;

         /// Sets the nav message type, and all other data members to 0.
      GLOCNavAlm();
         /// Create a deep copy of this object.
      NavDataPtr clone() const override
      { return std::make_shared<GLOCNavAlm>(*this); }

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Compute the satellites position and velocity at a time.
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
          * @return most recent transmit time + 6s.
          */
      CommonTime getUserTime() const override;

         /** Fill the beginFit and endFit values for this object.
          * @pre timeStamp must be set. */
      void fixFit();

         /** Print the contents of this NavData object in a (usually)
          * human-readable format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;

         /** Dump contents in terse format (DumpDetail==Terse)
          * @param[in,out] s The stream to write the data to. */
      void dumpTerse(std::ostream& s) const;

         /** @note the data members are being left in units of
          * semi-circles rather than converting to radians as typical,
          * so that the math in the ICD matches the math in the
          * code. */

      CommonTime Toa;      ///< Reference time for almanac.
      GLOCOrbitType orbitType; ///< Orbit type.
      unsigned numSVs;     ///< Number of SVs in complete almanac (NS).
      unsigned aoa;        ///< Age of almanac (E<sub>A</sub>).
      unsigned NA;         ///< Almanac reference time days since leap year.
      unsigned statusReg;  ///< Status register (SR<sub>A</sub> 5.3.2.7).
      GLOCSatType satType; ///< What satellite j is and what it transmits.
      double tau;          ///< Time correction from L3OCd to GLONASS.
      double lambda;       ///< Longitude of first ascending node (semicirc).
      double tLambda;      ///< Instant in Moscow time when passing lambda.
      double deltai;       ///< Inclination offset from 64.8 degrees (semicirc).
      double ecc;          ///< Eccentricity at t<sub>lambda<sub>A</sub></sub>.
      double omega;        ///< Almanac parameter for argument of perigee (semicirc).
      double deltaT;       ///< Draconic orbital period offset.
      double deltaTdot;    ///< Draconic orbital period rate.

   private:
         /** Collect Keplerian and quasi-Keplerian parameters that are
          * used in a variety of contexts. */
      class Perturbations
      {
      public:
            /// Initialize everything to NaN.
         Perturbations();
            /// Allow this class to be differenced.
         Perturbations operator-(const Perturbations& right) const;

         inline double geta() const { return a; }
         inline double geth() const { return h; }
         inline double getl() const { return l; }
         inline double getlambda() const { return lambda; }
         inline double getomega() const { return omega; }
         inline double geti() const { return i; }
         inline double getLk() const { return Lk; }
      protected:
         double a;      ///< Semi-major axis
         double h;      ///< Quasi-Keplerian thing (true latitude?).
         double l;      ///< Quasi-Keplerian thing (true longitude?).
         double lambda; ///< longitude of ascending node
         double omega;  ///< argument of perigee
         double i;      ///< inclination
         double Lk;     ///< Mean longitude
         friend class ::GLOCNavAlm_T;
      };
         /** Provide methods for computing the uncorrected Keplerian
          * parameters.
          * @note The Uncorrected glass does not utilize the Lk data
          *   member, because there are two such parameters for k=1,2
          *   while the remaining parameters do not have a subscript.
          *   Instead, L1 and L are stored within NumberCruncher.
          */
      class Uncorrected : public Perturbations
      {
      public:
            /// Initialize data to NaN.
         Uncorrected();
            /** Set all the delta (correction) parameters as defined
             * in stages 3-9 of ICD GLONASS CDMA General Description,
             * appendix M.1.
             * @param[in] alm The almanac containing the orbital elements.
             * @param[in] Tdr The mean draconic period in orbit as
             *   defined in stage 4.
             * @param[in] n The computed mean motion as defined in stage 4.
             * @param[in] Deltatpr The interval of prediction (in s)
             *   as defined in stage 1.
             * @post i, a, p, lambda, omega, h and l are all set. */
         inline void setData(const GLOCNavAlm& alm, double Tdr, double n,
                             double Deltatpr);
      private:
         double p; ///< Orbit radius at perigee (?)
            /** Set the inclination from the supplied data.
             * @param[in] DeltaiA The Delta inclination as extracted
             *   from the almanac (in semi-circles).
             * @post i is set. */
         inline void seti(double DeltaiA);
            /** Set the semi-major axis from the supplied data.
             * @param[in] epsilonA Eccentricity from almanac.
             * @param[in] omegaA Arg of perigee from almanac.
             * @param[in] Tdr The mean draconic period in orbit as
             *   defined in stage 4.
             * @pre i must be set.
             * @post a is set. */
         inline void seta(double epsilonA, double omegaA, double Tdr);
            /** Set the orbit radius at perigee from the supplied data.
             * @param[in] epsilonA Eccentricity from almanac.
             * @pre a must be set.
             * @post p is set. */
         inline void setp(double epsilonA);
            /** @pre i must be set
             * @pre p must be set */
         inline void setlambda(double lambdaA, double n, double Deltatpr);
            /** @pre i must be set
             * @pre p must be set */
         inline void setomega(double omegaA, double n, double Deltatpr);
            /// @pre omega must be set
         inline void sethl(double epsilonA);
         friend class ::GLOCNavAlm_T;
      };
         /** Yet more abstraction, as these data get computed multiple
          * times. 
          * @note Perturbations::omega is never used in this context
          *   and will remain set to NaN. */
      class Deltas : public Perturbations
      {
      public:
         Deltas() {}
            /** Set all the delta (correction) parameters as defined
             * on page 67 of ICD GLONASS CDMA General Description.
             * @param[in] B The precomputed B parameter as defined on
             *   this ICD page.
             * @param[in] Lk The mean longitude for this set of deltas
             *   (k=1 or 2). 
             * @param[in] uncor The uncorrected parameters used to
             *   compute the corrections.
             * @post a, h, l, lambda, i, Lk are all set. */
         inline void setData(double B, double Lk, const Uncorrected& uncor);
      private:
            /** Set the a parameter from the supplied data.
             * @copydetails setData() */
         inline void setdeltaa_a(double B, double Lk, const Uncorrected& uncor);
            /** Set the h parameter from the supplied data.
             * @copydetails setData() */
         inline void setdeltah(double B, double Lk, const Uncorrected& uncor);
            /** Set the l parameter from the supplied data.
             * @copydetails setData() */
         inline void setdeltal(double B, double Lk, const Uncorrected& uncor);
            /** Set the lambda parameter from the supplied data.
             * @copydetails setData() */
         inline void setdeltalambda(double B, double Lk,
                                    const Uncorrected& uncor);
            /** Set the i parameter from the supplied data.
             * @copydetails setData() */
         inline void setdeltai(double B, double Lk, const Uncorrected& uncor);
            /** Set the Lk parameter from the supplied data.
             * @copydetails setData() */
         inline void setdeltaLk(double B, double Lk, const Uncorrected& uncor);
         friend class ::GLOCNavAlm_T;
      };
         /// Provide methods for computing the corrected Keplerian parameters.
      class Corrected : public Perturbations
      {
      public:
            /// Initialize data to NaN.
         Corrected();
            /** Set all the delta (correction) parameters as defined
             * in stage 9 of ICD GLONASS CDMA General Description
             * appendex M.1.
             * @param[in] L The mean longitude for k=2 (aka L2).
             * @param[in] uncor The uncorrected parameters used to
             *   compute the corrections.
             * @param[in] delta1 The computed delta parameters for k=1.
             * @param[in] delta2 The computed delta parameters for k=2.
             * @post a, h, l, epsilon, i, lambda, omega, Lk, p, E, nu,
             *   u, r, vr, vu are all set. */
         inline void setData(double L, const Uncorrected& uncor,
                             const Deltas& delta1, const Deltas& delta2);
            /** Get the Xvt using the existing data.
             * @note clkbias and clkdrift will be 0 as they are unavailble.
             * @pre r must be set
             * @pre lambda must be set
             * @pre u must be set
             * @pre i must be set
             * @param[in] toi The time at which the Xvt is being computed. */
         inline Xvt getXvt(const gnsstk::CommonTime& toi);
      private:
         double E;       ///< Eccentric anomaly
         double epsilon; ///< epsilon prime, eccentricity.
         double nu;      ///< True anomaly
         double p;       ///< Orbit radius at perigee (?)
         double u;
         double r;       ///< Radial velocity
         double vu;
         double vr;
            /** @pre epsilon must be set
             * @pre a must be set
             * @post p is set */
         inline void setp();
            /** @pre Lk must be set
             * @pre omega must be set
             * @pre epsilon must be set
             * @post E is set */
         inline void setE(double L);
            /** @pre epsilon must be set
             * @pre E must be set 
             * @post nu is set */
         inline void setnu();
            /** @pre nu must be set
             * @pre omega must be set 
             * @post u is set */
         inline void setu();
            /** @pre nu must be set
             * @pre epsilon must be set 
             * @pre p must be set 
             * @post r is set */
         inline void setr();
            /** @pre p must be set
             * @pre epsilon must be set
             * @pre nu must be set 
             * @post vr, vu are set */
         inline void setvrvu();
         friend class ::GLOCNavAlm_T;
      };
         /// Class to assist in doing all the math to get the XVT.
      class NumberCruncher
      {
      public:
         NumberCruncher();
         inline Xvt getXvt(const GLOCNavAlm& alm,
                           const gnsstk::CommonTime& toi);
      private:
         Deltas k1;              ///< Delta values for k=1
         Deltas k2;              ///< Delta values for k=2
         Uncorrected uncorrected;///< Uncorrected parameters (stage 1-9-ish).
         Corrected corrected;    ///< Corrected parameters (stage 9).
         double Deltatpr;        ///< Time diff in seconds from ref to interest.
         int W;                  ///< Number of whole orbits on prediction intvl
         double Tdr;             ///< Mean draconic period in orbit W+1.
         double n;               ///< Mean motion
         double E0;              ///< Eccentric anomaly at t_A (?).
         double L1;              ///< Mean longitude at ascending node.
         double L;               ///< Current mean longitude of SV.
         double B;               ///< Correction scale factor.

            /** Set Delta t_pr from the given data.
             * @param[in] almTime The almanac reference time.
             * @param[in] ti The time of interest, i.e. when to compute xvt.
             * @post Deltatpr is set. */
         inline void setDeltatpr(const gnsstk::CommonTime& almTime,
                                 const gnsstk::CommonTime& ti);
            /** Set W from the given data.
             * @param[in] DeltaTA The &Delta;T<sub>A</sub> from the almanac.
             * @pre Deltatpr must be set.
             * @post W is set. */
         inline void setW(double DeltaTA);
            /** @pre W must be set.
             * @post Tdr is set. */
         inline void setTdr(double DeltaTA, double DeltaTdotA);
            /** @pre Tdr must be set.
             * @post n is set. */
         inline void setn();
            /** @pre uncorrected.omega must be set.
             * @post E0 is set. */
         inline void setE0(double epsilonA);
            /** @pre E0 must be set.
             * @pre uncorrected.omega must be set.
             * @post L1 is set. */
         inline void setL1(double epsilonA);
            /** @pre W must be set.
             * @pre n must be set.
             * @pre L1 must be set.
             * @pre Deltatpr must be set. 
             * @post L is set. */
         inline void setL(double DeltaTA, double DeltaTdotA);
            /** @pre uncorrected.a must be set.
             * @post B is set. */
         inline void setB();

         friend class ::GLOCNavAlm_T;
      };

      NumberCruncher math;///< Retain as much computed data as possible.

         // Need access to private data.
      friend class ::GLOCNavAlm_T;
   };

      //@}

}

// inline method definitions, in separate files for cleanliness.
#include "GLOCNavAlmUncorrected.hpp"
#include "GLOCNavAlmCorrected.hpp"
#include "GLOCNavAlmDeltas.hpp"
#include "GLOCNavAlmNumberCruncher.hpp"

#endif // GNSSTK_GLOCNAVALM_HPP
