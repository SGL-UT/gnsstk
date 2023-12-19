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
#ifndef GNSSTK_PNBGALINAVDATAFACTORY_HPP
#define GNSSTK_PNBGALINAVDATAFACTORY_HPP

#include "PNBNavDataFactory.hpp"
#include "GalINavAlm.hpp"
#include "GalINavHealth.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** This class provides a factory that processes PackedNavBits
       * data containing Galileo I/NAV data as defined in Galileo
       * OS-SIS-ICD.  The results of the addData method (in the navOut
       * argument) may contain any number of GalINavAlm, GalINavEph,
       * GalINavHealth or GalINavTimeOffset objects, according to what
       * data is fed to the method and what data is requested via the
       * validity and type filters (see PNBNavDataFactory).
       * @note Currently validity is not checked in any way in this class.
       * @note This class does not enforce sequential messages when
       *   assembling almanac or ephemeris data.
       */
   class PNBGalINavDataFactory : public PNBNavDataFactory
   {
   public:
         /** Process a PackedNavBits object, producing NavData objects
          * as appropriate.
          * Accepts data words or full page pairs.
          * @param[in] navIn The PackedNavBits data to process.
          * @param[out] navOut Any resulting NavData objects that were
          *   completed, usually as a result of adding navIn to the
          *   set of data.
          * @param[in] cadence This value is ignored in this class.
          * @return false on error. */
      bool addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
                   double cadence = -1)
         override;
     
         /** Given a pair of I/NAV pages, assemble them into a word 1-10, 63.
          * See Galileo OS-SIS-ICD 4.3.2.3 for more info on page pairs.
          * @param[in] navIn The page pair of PackedNavBits to process.
          * @param[out] navOut the resulting data word, if successful.
          * @return false on error.
          */
      static bool wordFromPagePair(const PackedNavBits& navIn,
                                   PackedNavBits& navOut);

         /** Process word types 1-5.  When a complete ephemeris of
          * word types 1,2,3,4,5 and consistent IODnav are accumulated
          * in ephAcc, that ephemeris is placed in navOut, along with
          * a GST-UTC time offset object, if such processing is
          * enabled.
          * @note This function only accepts a data word. To extract
          *   a data word from a page pair use wordFromPagePair().
          * @param[in] wordType The word type (1-5) pertaining to navIn.
          * @param[in] navIn The as-broadcast ephemeris bits.
          * @param[out] navOut If an ephemeris is completed, this will
          *   contain a GalINavEph object.
          * @return false on error. */
      bool processEph(unsigned wordType, const PackedNavBitsPtr& navIn,
                      NavDataPtrList& navOut);

         /** Process word types 7-10.  When a complete almanac of word
          * types 7,8,9,10 and consistent IODa are accumulated in
          * almAcc, that almanac is placed in navOut, along with a
          * GST-GPS time offset object, if such processing is enabled.
          * @note This function only accepts a data word. To extract
          *   a data word from a page pair use wordFromPagePair().
          * @param[in] wordType The word type (7-10) pertaining to navIn.
          * @param[in] navIn The as-broadcast almanac bits.
          * @param[out] navOut If an almanac is completed and the data
          *   is requested, this will contain a GalINavAlm object.
          * @return false on error. */
      bool processAlm(unsigned wordType, const PackedNavBitsPtr& navIn,
                      NavDataPtrList& navOut);

         /** Decode an almanac and/or health from a vector of Galileo
          * almanac words.
          * @note This function only accepts data words. To extract
          *   a data word from a page pair use wordFromPagePair().
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
      bool processAlmOrb(const std::vector<PackedNavBitsPtr>& almWord,
                         GalINavAlm *alm,
                         GalINavHealth *hp1, GalINavHealth *hp2,
                         int wtA, int wtB,
                         int asiWNa, int asit0a,
                         int asiSVID, int asbSVID,
                         int asidAhalf, int asbdAhalf,
                         int asiEcc, int asbEcc,
                         int asiw, int asbw,
                         int asidi, int asbdi,
                         int asiOMEGA0, int asbOMEGA0,
                         int asiOMEGAdot, int asbOMEGAdot,
                         int asiM0, int asbM0,
                         int asiaf0, int asbaf0,
                         int asiaf1, int asbaf1,
                         int asiE5bhs, int asbE5bhs,
                         int asiE1Bhs, int asbE1Bhs);

         /** Process word type 6.  This includes GST-UTC time offset data.
          * @note This function only accepts a data word. To extract
          *   a data word from a page pair use wordFromPagePair().
          * @param[in] navIn The PackedNavBits data containing word type 6.
          * @param[out] navOut The GalINavTimeOffset object generated from
          *   navIn.
          * @return true if successful. */
      bool processOffset(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

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
          * is word type 7 and so on. */
      std::map<NavSatelliteID, std::vector<PackedNavBitsPtr> > almAcc;
         /** Map satellite ID to a vector of PackedNavBits for accumulating
          * ephemeris data, where index 0 is word type 1 and so on. */
      std::map<NavSatelliteID, std::vector<PackedNavBitsPtr> > ephAcc;
   };

      //@}

} // namespace gnsstk

#endif // GNSSTK_PNBGALINAVDATAFACTORY_HPP
