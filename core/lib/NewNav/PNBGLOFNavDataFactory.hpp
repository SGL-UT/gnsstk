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
#ifndef GNSSTK_PNBGLOFNAVDATAFACTORY_HPP
#define GNSSTK_PNBGLOFNAVDATAFACTORY_HPP

#include "PNBNavDataFactory.hpp"
#include "BasicTimeSystemConverter.hpp"
#include "ValidType.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** This class provides a factory that processes PackedNavBits
       * data containing GLONASS FDMA civil nav data as defined in the
       * GLONASS (FDMA) ICD.  The results of the addData method (in
       * the navOut argument) may contain any number of GLOFNavAlm,
       * GLOFNavEph, GLOFNavHealth, GLOFNavISC, GLOFNavTimeOffset or
       * GLOFNavUT1TimeOffset objects, according to what data is fed
       * to the method and what data is requested via the validity and
       * type filters (see PNBNavDataFactory). */
   class PNBGLOFNavDataFactory : public PNBNavDataFactory
   {
   public:
         /// Initialize data members.
      PNBGLOFNavDataFactory();

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

         /** Reset the state of the data accumulator.  Most
          * PNBNavDataFactory child classes will maintain some state
          * to assemble data prior to processing.  This method is
          * intended to be used to clear out that intermediate data to
          * start from a fresh state, e.g. if you're loading
          * discontinuous data. */
      void resetState() override;

         /** Process strings 1-4.  When a complete ephemeris of
          * strings 1-4 with consecutive time stamps is accumulated in
          * ephAcc, that ephemeris is placed in navOut.  An ISC and/or
          * health object may also be added to navOut when available
          * and such processing is enabled. 
          * @param[in] stringID The string ID (1-4) pertaining to navIn.
          * @param[in] navIn The as-broadcast ephemeris bits.
          * @param[out] navOut If an ephemeris is completed and
          *   requested, this will contain a GLOFNavEph object.  If
          *   navIn contains string 4 and ISC data is requested,
          *   navOut will contain a GLOFNavISC object.  If an
          *   ephemeris is completed and health data is requested,
          *   navOut will contain a GLOFNavHealth object.
          * @return false on error. */
      bool processEph(unsigned long stringID, const PackedNavBitsPtr& navIn,
                      NavDataPtrList& navOut);

         /** Process strings 6-15.  When a complete set of almanac
          * orbital elements (e.g. page 6+7) with consecutive time
          * stamps is accumulated in almAcc, that almanac is placed in
          * navOut.  If requested, health will also be added to
          * navOut. 
          * @note This method stores data in almAcc.
          * @param[in] stringID The string ID (6-15) pertaining to navIn.
          * @param[in] navIn The as-broadcast almanac bits.
          * @param[out] navOut If an almanac is completed and the data
          *   is requested, this will contain a GLOFNavAlm object.  If
          *   an almanac is completed and health data is requested, it
          *   will contain a GLOFNavHealth object.
          * @return false on error. */
      bool processAlm(unsigned long stringID, const PackedNavBitsPtr& navIn,
                      NavDataPtrList& navOut);

         /** Process strings 6-15.  When a complete set of almanac
          * orbital elements (e.g. page 6+7) with consecutive time
          * stamps is accumulated in almAcc, that almanac is placed in
          * navOut.  If requested, health will also be added to
          * navOut. 
          * @note This method operates only on data already present in almAcc.
          * @param[in] key An unique identifier for the transmitting
          *   signal, used as a hash for almAcc.
          * @param[in] almIdx The array index into the almAcc
          *   collection for this satellite/signal (expected to be
          *   even numbers from 0..8).
          * @param[out] navOut If an almanac is completed and the data
          *   is requested, this will contain a GLOFNavAlm object.  If
          *   an almanac is completed and health data is requested, it
          *   will contain a GLOFNavHealth object.
          * @return false on error. */
      bool processAlmOrb(const NavSatelliteID& key, unsigned almIdx,
                         NavDataPtrList& navOut);

         /** Process string 5.  If requested, any of health, almanac,
          * or time offset data may be placed in navOut.
          * @param[in] navIn The as-broadcast string 5 bits.
          * @param[out] navOut If an almanac is completed and the data
          *   is requested, this will contain a GLOFNavAlm object.  If
          *   health data is requested, it will contain a
          *   GLOFNavHealth object.  If time offsets are requested, it
          *   will contain a GLOFNavTimeOffset object.
          * @return false on error. */
      bool processTime(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

         /** Process string 14, frame 5.  If requested, UT1 time
          * offset data will be placed in navOut.
          * @note This method does not check the string or frame
          *   numbers.  That must be done previously by the caller.
          * @param[in] navIn The as-broadcast string 14, frame 5 bits.
          * @param[out] navOut If time offset data is requested and
          *   the almanac reference time from string 5 has been
          *   loaded, this will contain a GLOFNavUT1TimeOffset object.
          * @return false on error. */
      bool processTimeUT1(const PackedNavBitsPtr& navIn,
                          NavDataPtrList& navOut);

         /** Compute the frame number given the transmit time of a string.
          * @param[in] xmitTime The transmit time of a nav string.
          * @return the decoded frame number (1-5). 
          * @throw AssertionFailure if unable to convert the time
          *   system of xmitTime to GLO.*/
      unsigned getFrameNum(const CommonTime& xmitTime);

         /// Return true if the PNB is 85 or 96 bits.
      inline static bool validSize(const PackedNavBitsPtr& navIn)
      {
         size_t sz = navIn->getNumBits();
         return ((sz == 85) || (sz == 96));
      }

   protected:
         /// Class to store fields used to turn GLONASS into UT1.
      class TimeMeta
      {
      public:
            /// Cast to NavDataPtr (specifically GLOFNavTimeOffset).
         operator NavDataPtr() const;
            /// Set the value of NT (from string 4).
         void setNT(unsigned v);
            /// Set the value of NA (from string 5).
         void setNA(unsigned v);
            /// Return true if all data fields are valid.
         bool isValid();
         vdouble tauc; ///< GLONASS time scale correction to UTC(SU) time.
         vdouble B1;   ///< GLONASS-UT1 offset.
         vdouble B2;   ///< GLONASS-UT1 drift.
         vuint KP;     ///< ??
      private:
            // keep these private so they can't be changed without
            // resetting B1/B2, particularly B2 which depends on the
            // values of NT and NA.
         vuint NT;     ///< Current calendar number of day w/in 4-year interval.
         vuint NA;     ///< NT but for almanac instead of ephemeris.
      };
         /** Map GLONASS transmit satellite ID to TimeMeta to
          * accumulate complete GLONASS to UT1 time corrections. */
      std::map<NavSatelliteID, TimeMeta> timeAcc;
         /** Map GLONASS satellite ID to a vector of PackedNavBits for
          * accumulating ephemeris data, where index 0 is string 1
          * and so on. */
      std::map<NavSatelliteID, std::vector<PackedNavBitsPtr> > ephAcc;
         /** Map GLONASS satellite ID to a vector of PackedNavBits for
          * accumulating almanac data, where index 0 is string 6
          * and so on. */
      std::map<NavSatelliteID, std::vector<PackedNavBitsPtr> > almAcc;

         // Some or all of these are likely going to get put into a
         // time offset class.

         /// Used to convert time stamps to and from GLONASS.
      BasicTimeSystemConverter btsc;
         /// Used to set the reference almanac time.
      CommonTime almDOY;

         /** Set to true if there are pending almanacs waiting on
          * almDOY to be set. */
      bool pendingAlms;
   }; // class PNBGLOFNavDataFactory

      //@}

} // namespace gnsstk

#endif // GNSSTK_PNBGLOFNAVDATAFACTORY_HPP
