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
#ifndef GPSTK_PNBGALFNAVDATAFACTORY_HPP
#define GPSTK_PNBGALFNAVDATAFACTORY_HPP

#include "PNBNavDataFactory.hpp"
#include "GalFNavAlm.hpp"
#include "GalNavHealth.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** This class provides a factory that processes PackedNavBits
       * data containing Galileo F/NAV data as defined in Galileo
       * OS-SIS-ICD.  The results of the addData method (in the navOut
       * argument) may contain any number of GalFNavAlm, GalFNavEph,
       * GalNavHealth or GalFNavTimeOffset objects, according to what
       * data is fed to the method and what data is requested via the
       * validity and type filters (see PNBNavDataFactory).
       * @note Currently validity is not checked in any way in this class.
       * @note This class does not enforce sequential messages when
       *   assembling almanac or ephemeris data.
       */
   class PNBGalFNavDataFactory : public PNBNavDataFactory
   {
   public:
         /** Process a PackedNavBits object, producing NavData objects
          * as appropriate.
          * @param[in] navIn The PackedNavBits data to process.
          * @param[out] navOut Any resulting NavData objects that were
          *   completed, usually as a result of adding navIn to the
          *   set of data.
          * @param[in] cadence This value is ignored in this class.
          * @return false on error. */
      bool addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
                   double cadence = -1)
         override;

         /** Process page types 1-4.  When a complete ephemeris of
          * page types 1,2,3,4 and consistent IODnav are accumulated
          * in ephAcc, that ephemeris is placed in navOut, along with
          * a GST-UTC and GST-GPS time offset object and health
          * information object, if such processing is enabled.
          * @param[in] pageType The page type (1-4) pertaining to navIn.
          * @param[in] navIn The as-broadcast ephemeris bits.
          * @param[out] navOut If an ephemeris is completed, this will
          *   contain a GalFNavEph object.  If navIn contains page
          *   type 1 and health data is requested, navOut will contain
          *   a GalNavHealth object.  If navIn contains page type 4
          *   and time offset data is requested, navOut will contain
          *   two GalFNavTimeOffset objects.
          * @return false on error. */
      bool processEph(unsigned pageType, const PackedNavBitsPtr& navIn,
                      NavDataPtrList& navOut);

         /** Process page types 5-6.  When a complete almanac of page
          * types 5,6 and consistent IODa are accumulated in almAcc,
          * that almanac is placed in navOut, along with health
          * information offset object, if such processing is enabled.
          * @param[in] pageType The page type (5-6) pertaining to navIn.
          * @param[in] navIn The as-broadcast almanac bits.
          * @param[out] navOut If an almanac is completed and the data
          *   is requested, this will contain a GalFNavAlm object,
          *   and/or if health data is requested, this will contain a
          *   GalNavHealth object
          * @return false on error. */
      bool processAlm(unsigned pageType, const PackedNavBitsPtr& navIn,
                      NavDataPtrList& navOut);


         /** Decode an almanac and/or health from a vector of Galileo
          * almanac words.
          * @param[in] almWord An vector of size 4 of Galileo word types 7-10.
          * @param[in,out] alm The pre-allocated almanac object to store
          *   the decoded data in.
          * @param[in,out] hp1 The pre-allocated health object to
          *   store E5b health status data in.
          * @param[in,out] hp2 The pre-allocated health object to
          *   store E1B health status data in.
          * @param[in] wtA The index into almWord of the first word
          *   type of the pair of word types being decoded to make an
          *   almanac.
          * @param[in] wtB The index into almWord of the second word
          *   type of the pair of word types being decoded to make an
          *   almanac.
          * @param[in] asiWNa The index into almWord of the word type
          *   that contains the WNa value.
          * @param[in] asit0a The index into almWord of the word type
          *   that contains the t0a value.
          * @param[in] asidAhalf The index into almWord of the word
          *   type that contains the delta A**1/2 value.
          * @param[in] asbdAhalf The first bit of the word type that
          *   contains the delta A**1/2 value.
          * @param[in] asiEcc The index into almWord of the word
          *   type that contains the eccentricity value.
          * @param[in] asbEcc The first bit of the word type that
          *   contains the eccentricity value.
          * @param[in] asiw The index into almWord of the word
          *   type that contains the argument of perigee.
          * @param[in] asbw The first bit of the word type that
          *   contains the argument of perigee.
          * @param[in] asidi The index into almWord of the word
          *   type that contains the delta inclination.
          * @param[in] asbdi The first bit of the word type that
          *   contains the delta inclination value.
          * @param[in] asiOMEGA0 The index into almWord of the word
          *   type that contains the right ascension reference value.
          * @param[in] asbOMEGA0 The first bit of the word type that
          *   contains the right ascension reference value.
          * @param[in] asiOMEGAdot The index into almWord of the word
          *   type that contains the rate of right ascension value.
          * @param[in] asbOMEGAdot The first bit of the word type that
          *   contains the rate of right ascension value.
          * @param[in] asiM0 The index into almWord of the word
          *   type that contains the mean anomaly reference value.
          * @param[in] asbM0 The first bit of the word type that
          *   contains the mean anomaly reference value.
          * @param[in] asiaf0 The index into almWord of the word
          *   type that contains the clock bias value.
          * @param[in] asbaf0 The first bit of the word type that
          *   contains the clock bias value.
          * @param[in] asiaf1 The index into almWord of the word
          *   type that contains the clock drift value.
          * @param[in] asbaf1 The first bit of the word type that
          *   contains the clock drift value.
          * @param[in] asiE5bhs The index into almWord of the word
          *   type that contains the E5b health status value.
          * @param[in] asbE5bhs The first bit of the word type that
          *   contains the E5b health status value.
          * @param[in] asiE1Bhs The index into almWord of the word
          *   type that contains the E1B health status value.
          * @param[in] asbE1Bhs The first bit of the word type that
          *   contains the E1B health status value.
          * @note alm, hp1 and hp2 need only have been created,
          *   typically using std::make_shared, prior to calling this
          *   method.
          * @return true if an almanac was decoded, false if the
          *   specified almanac contained a zero SVID.
          */
      bool processAlmOrb(const std::vector<PackedNavBitsPtr>& almPage,
                         GalFNavAlm *alm, GalNavHealth *hp1,
                         int ptA, int ptB,
                         int asiSVID, int asbSVID,
                         int asidAhalf, int asbdAhalf,
                         int asiEcc, int asbEcc,
                         int asiw, int asbw,
                         int asidi, int asbdi,
                         int asiOMEGAdot, int asbOMEGAdot,
                         int asiM0, int asbM0,
                         int asiaf0, int asbaf0,
                         int asiaf1, int asbaf1,
                         int asiE5ahs, int asbE5ahs);

         /** Reset the state of the data accumulator.  Most
          * PNBNavDataFactory child classes will maintain some state
          * to assemble data prior to processing.  This method is
          * intended to be used to clear out that intermediate data to
          * start from a fresh state, e.g. if you're loading
          * discontinuous data. */
      void resetState() override;
#if 0

         /** For debugging purposes, dump the sizes of the accumulator maps.
          * @param[in,out] s The stream to write the debug output to. */
      void dumpState(std::ostream& s) const;
#endif

   protected:
         /** Map transmitting satellite ID to a vector of
          * PackedNavBits for accumulating almanac data, where index 0
          * is page type 5 and so on. */
      std::map<NavSatelliteID, std::vector<PackedNavBitsPtr> > almAcc;
         /** Map satellite ID to a vector of PackedNavBits for accumulating
          * ephemeris data, where index 0 is page type 1 and so on. */
      std::map<NavSatelliteID, std::vector<PackedNavBitsPtr> > ephAcc;
   };

      //@}

} // namespace gpstk

#endif // GPSTK_PNBGALFNAVDATAFACTORY_HPP
