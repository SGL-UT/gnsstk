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
#ifndef GPSTK_NEQUICKIONODATA_HPP
#define GPSTK_NEQUICKIONODATA_HPP

#include "IonoData.hpp"
#include "CivilTime.hpp"
#include "CCIR.hpp"
#include "MODIP.hpp"
#include "GalileoIonoEllipsoid.hpp"
#include "DebugTrace.hpp"

/*
 * ALL EQUATION AND SECTION REFERENCES ARE TO THE DOCUMENT 
 * "Ionospheric Correction Algorithm for Galileo Single Frequency Users"
 * aka "Galileo Ionospheric Model"
 * UNLESS OTHERWISE STATED
 */

// forward declaration of test class
class NeQuickIonoData_T;

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements unique to NeQuick ionospheric model.
       * Design criteria:
       *   * The code was written to match the equations in \cite galileo:iono.
       *   * In particular, the code uses variable names that match the
       *     document, where in other situations, more descriptive
       *     names would have been used.
       *   * Implementation of functions may more closely match the EU's
       *     implementation, in order to match their input and truth
       *     data for testing.
       *   * Magic numbers abound, though they have been replaced when a
       *     useful name could be found.
       *   * No, I don't know why A1=F2 amplitude, etc.
       * 
       * References:
       * \cite galileo:iono
       * \cite itur:iono
       */
   class NeQuickIonoData : public IonoData
   {
   public:
         /// Number of degrees longitude per hour.
      static constexpr double DEGREE_PER_HOUR = 15.0;

         /// Initialize internal data.
      NeQuickIonoData();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override
      { return true; }

         /** Print the contents of this NeQuickIonoData object in a
          * human-readable format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;

         /** Get the ionospheric correction in meters.
          * @param[in] when The time of the observation to correct.
          * @param[in] rxgeo The receiver's geodetic position.
          * @param[in] svgeo The observed satellite's geodetic position.
          * @param[in] band The carrier band of the signal being corrected.
          * @return The ionospheric delay, in meters, on band. */
      double getCorrection(const CommonTime& when,
                           const Position& rxgeo,
                           const Position& svgeo,
                           CarrierBand band) const override;

      double getTEC(const CommonTime& when,
                    const Position& rxgeo,
                    const Position& svgeo,
                    CarrierBand band) const;

         /** a<sub>i</sub> terms of NeQuick model in solar flux units,
          * solar flux units/degree, solar flux
          * units/degree<sup>2</sup>.  Refer to Galileo-OS-SIS-ICD. */
      double ai[3];
         // These are technically Galileo-specific terms, and they
         // seem to be as yet unused according to the ICD (section
         // 5.1.6, "reserved for future use"), however until such time
         // as we have a situation where we use an NeQuick model that
         // does NOT use this data, I'm leaving it here so it only
         // needs to be defined once.
      bool idf[5]; ///< Ionospheric disturbance flag for regions 1-5 (0-4).

         /** Similar to standard exp() function, but with the exponent
          * clipped to +/- 80, per F2.1.2.3 \cite galileo:iono 
          * @param[in] x The exponent to raise e to.
          * @param[in] e^x. */
      static double neExp(double x);

   protected:
         /** @note I'm not dead set on making these protected or
          * private.  Someone may have a use for them as public
          * methods down the road and move them out, but to avoid
          * confusion by users looking through a bunch of functions
          * they don't need, I currently only expose the top-level
          * method. */

         /** Get the effective ionization level in solar flux units.
          * @param[in] modip_u Modified dip latitude of observer.
          * @return The effective ionization level Az in solar flux units. */
      double getEffIonoLevel(double modip_u) const;

         /// Aggregate the model parameters as defined in section 2.5.5
      class ModelParameters
      {
      public:
            /** Compute the various NeQuickG model parameters.
             * @param[in] modip_u Modified dip latitude in degrees.
             * @param[in] pos The geodetic position of the observer.
             * @param[in] az The effective ionization level in solar flux units
             *   (NOT azimuth).
             * @param[in] ccirData A CCIR object to be used for looking up
             *   ionospheric model data.
             * @param[in] when The time of the observation being
             *   modeled (month and hour of day are used).
             * @post fAzr, ffoE, fNmE, ffoF1, fNmF1, fNmF2 are set. */
         ModelParameters(double modip_u, const Position& pos, double az,
                         CCIR& ccirData, const CivilTime& when);

            /** Compute the sine and cosine of the solar
             * declination. (sec 2.5.4.6)
             * @param[in] when The time at which to compute the solar
             *   declination.
             * @return The sine and cosine of the solar declination. */
         static AngleReduced solarDeclination(const CivilTime& when);

            /** Compute the solar zenith angle.
             * @param[in] pos The geodetic position of the observer.
             * @param[in] when The time at which to compute the solar zenith. */
         static Angle solarZenithAngle(const Position& pos,
                                       const CivilTime& when);

            /** Compute the effective solar zenith angle.
             * @param[in] pos The geodetic position of the observer.
             * @param[in] when The time at which to compute the solar zenith. */
         static Angle effSolarZenithAngle(const Position& pos,
                                          const CivilTime& when);

            /** Compute foF2 and M(3000)F2 by Legendre calculation.
             * @param[in] modip_u Modified dip latitude in degrees.
             * @param[in] pos The geodetic position of the observer.
             * @post ffoF2, fM3000F2 are set.
             */
         void legendre(double modip_u, const CivilTime& when,
                       const Position& pos);

            /** Compute hmF2 and hmF1 (maximum density height).
             * @pre ffoE, ffoF2, fM3000F2 must be set.
             * @post fhmF2 and fhmF1 are set. */
         void height();

            /** Compute thickness parameters.
             * @pre fNmF2, ffoF2, fM3000F2, fhmF1, fhmF2 must be set. 
             * @post fB2bot, fB1top, fB1bot, fBEtop are set. */
         void thickness();

            /** Compute the top-side thickness parameter.
             * @pre fAzr, fhmF2, fB2bot, fNmF2 must be set. 
             * @param[in] month Month 1-12 for ionospheric model.
             * @post fH0 is set. */
         void exosphereAdjust(unsigned month);

            /** Compute peak amplitudes.
             * @pre fNmE, fNmF1, fhmF1, fNmF2, fhmF2, fBEtop, fB1bot,
             *   fB2bot, ffoF1 must be set.
             * @post fA1, fA2, fA3 are set.
             */
         void peakAmplitudes();

            /** Epstein function per 2.5.1.
             * @param[in] x peak amplitude.
             * @param[in] y peak height.
             * @param[in] z thickness around the peak.
             * @param[in] w height-dependent variable.
             * @return ????????????????????????????????????????????????????????
             */
         static inline double epstein(double x, double y, double z, double w);

            /** Using a formula akin to Eq.4, return a longitude from
             * the given time.
             * @note This function is implemented to match the
             *   NeQuickG C library, though its use in Eq.59 and Eq.60
             *   is not well documented (One is likely to infer that
             *   the lambda in galileo:iono refers to the current
             *   integration position).  It's not entirely clear what
             *   this is, but at a guess I'd say it's a rough
             *   longitude of the overhead sun.
             * @param[in] when The time at which to derive a longitude.
             * @return the solar longitude in radians. */
         static inline double solarGetLongitude(const CivilTime& when);

            /** Compute electron density.
             * @pre fhmF2, fhmF1, fB2bot, fB1top, fB1bot, fBEtop must be set.
             */
         double electronDensity(const Position& pos);
         double electronDensityTop(const Position& pos);
            /** @pre fA must be set. */
         double electronDensityBottom(const Position& pos);

         CCIR &ccir;      ///< Reference to iono model data.
         double fAzr;     ///< Effective sunspot number.
         double ffoE;     ///< E layer critical frequency in MHz.
         double fNmE;     ///< E layer maximum electron density in el m**-2.
         double fBEtop;   ///< E layer top-side thickness parameter in km.
         double fA[3];    ///< F2, F1, E layer peak amplitudes.
            // hmE and BEbot are constants.
         double ffoF1;    ///< F1 layer critical frequency in MHz.
         double fNmF1;    ///< F1 layer maximum electron density in el m**-2.
         double fhmF1;    ///< F1 layer maximum density height in km.
         double fB1top;   ///< F1 layer top-side thickness parameter in km.
         double fB1bot;   ///< F1 layer bottom-side thickness parameter in km.
         double ffoF2;    ///< F2 layer critical frequency in MHz.
         double fNmF2;    ///< F2 layer maximum electron density in el m**-2.
         double fhmF2;    ///< F2 layer maximum density height in km.
         double fB2bot;   ///< F2 layer bottom-side thickness parameter in km.
         double fM3000F2; ///< F2 layer transmission factor.
         double fH0;      ///< Top-side thickness parameter in km.
         Angle fXeff;     ///< Effective solar zenith angle.

      private:
            /// Constructor for testing only.
         ModelParameters(CCIR& ccirData);

         friend class ::NeQuickIonoData_T;
      };

         /// Class to contain data used when integrating TEC.
      class IntegrationParameters
      {
      public:
         IntegrationParameters(const Position& rx, const Position& sv,
                               const Position& Pp);
         double ip1; ///< Radius in km at first integration point.
         double ip2; ///< Radius in km at second integration point.
         double s1;  ///< Slant height in km of station.
         double sa;  ///< Slant height in km of first integration point.
         double sb;  ///< Slant height in km of second integration point.
         double s2;  ///< Slant height in km of satellite.
            /** A vector containing the set of relevant integration
             * intervals, which is essentially s1, s2, and sa, sb if
             * they're in between s1 and s2.  Put another way: the
             * subset of (s1,s2,sa,sb) that are >= s1 and <= s2.  This
             * gives us a set of intervals to use for integration that
             * obviates having functions for each different
             * condition. */
         std::vector<double> slantHeights;
            /** Integration thresholds used for slantHeights.
             * @note the size of intThresh will be slantHeights.size()-1. */
         std::vector<double> intThresh;
      };

         /// get slant total electron content
      double getSTEC(double dist, const Position& rxgeo, const Position& svgeo,
                     const MODIP& modip, CCIR& ccirData, const CivilTime& when,
                     double azu)
         const;
         /// get vertical total electron content
      double getVTEC()
         const;

      static double integrateGauss(double g1, double g2);

         /// @todo I think rxgeo/svgeo can be replaced by using Pp directly.
      double integrateGaussKronrod(double heightPt1, double heightPt2,
                                   const Position& rxgeo, const Position& svgeo,
                                   const MODIP& modip, CCIR& ccirData,
                                   const CivilTime& when, double azu,
                                   double tolerance, bool vertical,
                                   unsigned recursionLevel = 0)
         const;

         /** Galileo ellipsoid model for ionospheric modeling.  This
          * is as defined in galileo:iono.
          * @note This "ellipsoid" model is really a sphere,
          *   i.e. eccentricity of 0, and the equations reflect that
          *   fact.  Any attempts to use non-spherical models will
          *   result in incorrect results. */
      GalileoIonoEllipsoid elModel;

      friend class ::NeQuickIonoData_T;
   };

   double NeQuickIonoData::ModelParameters ::
   epstein(double x, double y, double z, double w)
   {
      DEBUGTRACE_FUNCTION();
      DEBUGTRACE("thickness_param=" << std::scientific << z);
      DEBUGTRACE("height_km=" << std::scientific << w);
      DEBUGTRACE("pPeak->height_km=" << std::scientific << y);
      double tmp = exp((w-y)/z);
      DEBUGTRACE("electron_density (1)=" << std::scientific << tmp);
      double rv = (x*tmp) / ((1+tmp)*(1+tmp));                          //eq.3
      DEBUGTRACE("electron_density (2)=" << std::scientific << rv);
      return rv;
   }

   double NeQuickIonoData::ModelParameters ::
   solarGetLongitude(const CivilTime& when)
   {
         // eq.4, sort of.
      return (DEGREE_PER_HOUR * when.getUTHour() - 180.0) * DEG2RAD;
   }

      //@}

}

#endif // GPSTK_NEQUICKIONODATA_HPP
