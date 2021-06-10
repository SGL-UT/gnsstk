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
             *   modeled (month and hour of day are used). */
         ModelParameters(double modip_u, const Position& pos, double az,
                         CCIR& ccirData, const CivilTime& when);

            /** Compute the sine and cosine of the solar
             * declination. (sec 2.5.4.6)
             * @param[in] when The time at which to compute the solar
             *   declination.
             * @param[out] sdsin The sine of the solar declination.
             * @param[out] sdcos The cosine of the solar declination. */
         static void solarDeclination(const CivilTime& when, double& sdsin,
                                      double& sdcos);

            /** Compute the solar zenith angle IN DEGREES.
             * @param[in] pos The geodetic position of the observer.
             * @param[in] when The time at which to compute the solar zenith. */
         static double solarZenithAngle(const Position& pos,
                                        const CivilTime& when);

            /** Compute the effective solar zenith angle IN DEGREES.
             * @param[in] pos The geodetic position of the observer.
             * @param[in] when The time at which to compute the solar zenith. */
         static double effSolarZenithAngle(const Position& pos,
                                           const CivilTime& when);

            /** Compute foF2 and M(3000)F2 by Legendre calculation.
             * @param[in] modip_u Modified dip latitude in degrees.
             * @param[in] pos The geodetic position of the observer.
             * @post ffoF2, fM3000F2 are set.
             */
         void legendre(double modip_u, const Position& pos);

            /** Compute hmF2 and hmF1 (maximum density height).
             * @pre ffoE, ffoF2, fM3000F2 must be set.
             * @post fhmF2 and fhmF1 are set. */
         void height();

            /** Compute thickness parameters.
             * @pre fNmF2, ffoF2, fM3000F2, hmF1, hmF2 must be set. 
             * @post fB2bot, fB1top, fB1bot, fBEtop are set. */
         void thickness();

            /** Compute the top-side thickness parameter.
             * @pre fAzr, fhmF2, fB2bot, fNmF2 must be set. 
             * @param[in] month Month 1-12 for ionospheric model.
             * @return The shape parameter "k". */
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
             */
         static inline double epstein(double x, double y, double z, double w);

         CCIR &ccir;      ///< Reference to iono model data.
         double fAzr;     ///< Effective sunspot number.
         double ffoE;     ///< E layer critical frequency in MHz.
         double fNmE;     ///< E layer maximum electron density in el m**-2.
         double fBEtop;   ///< E layer top-side thickness parameter in km.
         double fA3;      ///< E layer amplitude.
            // hmE and BEbot are constants.
         double ffoF1;    ///< F1 layer critical frequency in MHz.
         double fNmF1;    ///< F1 layer maximum electron density in el m**-2.
         double fhmF1;    ///< F1 layer maximum density height in km.
         double fB1top;   ///< F1 layer top-side thickness parameter in km.
         double fB1bot;   ///< F1 layer bottom-side thickness parameter in km.
         double fA2;      ///< F1 layer amplitude.
         double ffoF2;    ///< F2 layer critical frequency in MHz.
         double fNmF2;    ///< F2 layer maximum electron density in el m**-2.
         double fhmF2;    ///< F2 layer maximum density height in km.
         double fB2bot;   ///< F2 layer bottom-side thickness parameter in km.
         double fA1;      ///< F2 layer amplitude.
         double fM3000F2; ///< F2 layer transmission factor.
         double fH0;      ///< Top-side thickness parameter in km.
         double fXeff;    ///< Effective solar zenith angle in degrees.

      private:
            /// Constructor for testing only.
         ModelParameters(CCIR& ccirData);

         friend class ::NeQuickIonoData_T;
      };

         /// get slant total electron content
      static double getSTEC();
         /// get vertical total electron content
      static double getVTEC();

      static double integrateGauss(double g1, double g2);

      friend class ::NeQuickIonoData_T;
   };

   double NeQuickIonoData::ModelParameters ::
   epstein(double x, double y, double z, double w)
   {
      double tmp = exp((w-y)/z);
      return (x*tmp) / ((1+tmp)*(1+tmp));                               //eq.3
   }

      //@}

}

#endif // GPSTK_NEQUICKIONODATA_HPP
