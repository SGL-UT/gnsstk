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
#ifndef GNSSTK_GLOFNAVALM_HPP
#define GNSSTK_GLOFNAVALM_HPP

#include "GLOFNavData.hpp"
#include "PZ90Ellipsoid.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements unique to GLONASS Civil F-Nav
       * almanacs. */
   class GLOFNavAlm : public GLOFNavData
   {
   public:
         /// Ellipsoid parameters used by GLONASS
      static const PZ90Ellipsoid ell;
         /// Gravitational constant.
      static const double mu;
         /// Equatorial radius of Earth in km.
      static const double ae;
         /** Fatty acids? No, Earth's rotation rate.
          * @note This is incorrectly listed as .73... in one
          * location.  It is, in fact, .72... */
      static constexpr double omega3 = 0.7292115e-4;
         /// i<sub>cp</cp> as defined in GLONASS ICD appendix 3.2.1
      static const double icp;
         /// T<sub>cp</cp> as defined in GLONASS ICD appendix 3.2.1
      static const double Tcp;
         /// Second zonal harmonic of spherical harmonic expansion.
      static const double C20;
         /// Some constant or other related to C20.
      static const double J;
         /// Term used in computing orbit.
      static const double C20Term;

         /// Sets the nav message type, and all other data members to 0.
      GLOFNavAlm();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Compute the satellites position and velocity at a time.
          * @bug This method does not yield expected results and until
          *   it is fixed will always return false.
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

         /// Compute and set the semi-major axis (A) and inclination (i).
      void setSemiMajorAxisIncl();

         /// Class to assist in doing all the math to get the XVT.
      class NumberCruncher
      {
      public:
            /// Initialize stuff to NaN.
         NumberCruncher();

	    /** Set terms related to eccentricity and argument of perigee
	     * @param[in] ecc The satellite's orbital eccentricity.
	     * @param[in] omega The satellite's orbital argument of perigee.
	     * @post h and l are set.
	     */
	 void setEccArgOfPerigee(double ecc, double omega);
            /** Set terms related to the orbit semi-major axis and inclination.
             * @param[in] deltaT Delta T parameter from broadcast almanac.
             * @param[in] deltai Delta i parameter from broadcast almanac.
             * @param[in] omega omega (lower-case) parameter from
             *   broadcast almanac.
             * @param[in] ecc epsilon (lower-case) parameter from
             *   broadcast almanac (eccentricity). */
         void setSemiMajorAxisIncl(double deltaT, double deltai, double omega,
                                   double ecc);
            /** Set lambdaBar and related data members according to
             * the given values.
             * @param[in] M Computed mean anomaly, I think.
             * @param[in] omega Argument of perigee.
             * @param[in] tau Either 0 or time between t<sub>i</sub>
             *   and t<sub>&lambda;k</sub> in seconds (see the mess of
             *   math in the ICD or the implementation in setDeltas).
             * @param[in] n the value of n (2pi/Tdeltap) as computed
             *   by GLOFNavAlm::setSemiMajorAxisIncl.
             * @post lambdabar, coslambdaBar, sinlambdaBar,
             *   cos2lambdaBar, sin2lambdaBar, cos3lambdaBar,
             *   sin3lambdaBar, cos4lambdaBar, sin4lambdaBar are
             *   set. */
         void setLambdaBar(double M, double omega, double tau, double n);
            /** Set the deltas values.
             * @param[in] M Computed mean anomaly, I think.
             * @param[in] omega Argument of perigee.
             * @param[in] a Satellite orbit semi-major axis.
             * @param[in] dt The delta time in seconds,
             *   &tau;=t<sub>i</sub>-t<sub>&lambda;k</sub>
             *   named dt instead of tau here to avoid confusion with other tau.
             * @pre coslambdaBar, sinlambdaBar, cos2lambdaBar,
             *   sin2lambdaBar, cos3lambdaBar, sin3lambdaBar,
             *   cos4lambdaBar, sin4lambdaBar, tau, n, l, h, JTerm,
             *   JsinTerm, Jsini2Term, Jcosi2term must be set. */
         void setDeltas(double M, double omega, double a, double dt);
            /** Get the value for &omega;<sub>i</sub> given the
             * already computed values for h<sub>i</sub>,
             * l<sub>i</sub> and &epsilon;<sub>i</sub>.
             * @param[in] hi Value h<sub>i</sub> as computed by getXvt().
             * @param[in] li Value l<sub>i</sub> as computed by getXvt().
             * @param[in] epsi Value &epsilon;<sub>i</sub> as computed
             *   by getXvt().
             * @return The appropriate value for &omega;<sub>i</sub>
             *   (see Appendix A.3.2.2, probably the Russian-language
             *   version as the formatting in the English version is a
             *   mess). */
         static double getomegai(double hi, double li, double epsi);
            /** Numerically integrate to get E_i^(n).
             * @param[in] Mi Value M<sub>i</sub> as computed by getXvt().
             * @param[in] epsi Value &epsilon;<sub>i</sub> as computed
             *   by getXvt().
             * @return The integrated E_i^(n).
             */
         static double integrateEin(double Mi, double epsi);

         bool getXvt(const CommonTime& when, Xvt& xvt, const GLOFNavAlm& alm);
      private:
            /** Yet more abstraction, as these data get computed
             * multiple times.  What are they? *shrug*. */
         class Deltas
         {
         public:
               /// Initialize everything to NaN.
            Deltas();
               /** Set the delta values in this object.
                * @param[in] nc The NumberCruncher that has all the
                *   necessary data used to compute the deltas (which
                *   is most of the data fields in NumberCruncher).
                * @param[in] a The semi-major axis of the satellite's orbit.
                * @param[in] tau The value of tau, which should be 0
                *   for computing m=1, and
                *   t<sub>i-t<sub>&lambdak</sub> for m=2. 
                * @param[in] n the value of n (2pi/Tdeltap) as computed
                *   by GLOFNavAlm::setSemiMajorAxisIncl. */
            void setDeltas(const NumberCruncher& nc, double a, double tau,
                           double n);
            Deltas operator-(const Deltas& right) const;
            double deltaa;
            double deltah;
            double deltal;
            double deltaOMEGA;
            double deltai;
            double deltalambdaBar;
         };
            // These are all terms defined in the ICD, however I can't
            // say I know exactly what they represent, as the ICD
            // frequently doesn't, hence the absence of documentation.
            // A lot of these are values that are computed once and
            // reused several time, an optimization favoring
            // performance over memory use.
         double lambdaBar;
         double tau;
         double nu;
         double Tdeltap;        ///< deltaT + half day for some reason.
         double n;              ///< 2pi/Tdeltap.
         double a;              ///< Semi-major axis (m)
         double i;              ///< Inclination (rad)
         double coslambdaBar;   ///< cos(lambdaBar)
         double sinlambdaBar;   ///< sin(lambdaBar)
         double cos2lambdaBar;  ///< cos(2*lambdaBar)
         double sin2lambdaBar;  ///< sin(2*lambdaBar)
         double cos3lambdaBar;  ///< cos(3*lambdaBar)
         double sin3lambdaBar;  ///< sin(3*lambdaBar)
         double cos4lambdaBar;  ///< cos(4*lambdaBar)
         double sin4lambdaBar;  ///< sin(4*lambdaBar)
         double earthvs;        ///< (A_earth/A_SV)**2.
         double sini;           ///< sin(i)
         double sini2;          ///< sin(i)**2
         double cosi;           ///< cos(i)
         double cosi2;          ///< cos(i)**2
         double JTerm;          ///< J * earthvs
         double JsinTerm;       ///< JTerm * (1-TH*sini2)
         double Jsini2Term;     ///< JTerm * sini2
         double Jcosi2Term;     ///< JTerm * sini2
         double h;
         double l;
         double ecc2;           ///< ecc**2
         double ecc2obv;        ///< 1-ecc**2
         Deltas deltas;         ///< Final delta results, not m=1 or m=2.
         friend class Deltas;
      };

         // M_n^a is stored in GLOFNavData::satType
         // slot (n^A) is stored in the signal structure and GLOFNavData::slot
         // Note that in naming variables below, I've explicitly added
         // "nA" to match the ICD notation to the extent we can and to
         // hopefully reduce confusion due to overloaded variables.
      CommonTime Toa;     ///< Reference time for almanac.
      bool healthBits;    ///< Health flag (C_n, 1 = operable).
         // This is tau sub n sup A, not to be confused with tau used
         // to compute the orbit.
      double taunA;         ///< Time offset to GLONASS time (tau_n^A).
      double lambdanA;      ///< Longitude of ascending node (lambda_n^A).
      double deltainA;      ///< Correction to mean inclination (Delta i_n^A).
      double eccnA;         ///< Eccentricity (epsilon_n^A).
      double omeganA;       ///< Argument of perigee (omega_n^A).
      double tLambdanA;     ///< Time of ascending node crossing (t_lambda_n^A).
      double deltaTnA;      ///< Correction to mean value of Draconian period (Delta T_n^A).
      double deltaTdotnA;   ///< Time derivative of deltaT (Delta T'_n^A).
      int freqnA;           ///< Frequency offset (H_n^A).
         // These data members are computed by setSemiMajorAxisIncl,
         // which should be done immediately following filling in the
         // data above and calling fixFit() when decoding a GLONASS
         // almanac.
      NumberCruncher math;  ///< Retain as much computed data as possible.
   };

      //@}

}

#endif // GNSSTK_GLOFNAVALM_HPP
