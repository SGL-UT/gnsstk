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
#ifndef GNSSTK_PNBGLOCNAVDATAFACTORY_HPP
#define GNSSTK_PNBGLOCNAVDATAFACTORY_HPP

#include "PNBNavDataFactory.hpp"
#include "BasicTimeSystemConverter.hpp"
#include "ValidType.hpp"
#include "GLOCNavData.hpp"
#include "GLOCNavLTDMP.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** This class provides a factory that processes PackedNavBits
       * data containing GLONASS CDMA civil nav data as defined in the
       * GLONASS CDMA ICD.  The results of the addData method (in
       * the navOut argument) may contain any number of GLOCNavAlm,
       * GLOCNavEph, GLOCNavHealth, GLOCNavISC, GLOCNavTimeOffset or
       * GLOCNavUT1TimeOffset objects, according to what data is fed
       * to the method and what data is requested via the validity and
       * type filters (see PNBNavDataFactory).
       *
       * @note With respect to handling of strings 31 and 32, which
       * contain the long-term dynamic model parameters, these are
       * handled in time order.  This mimics how user equipment would
       * see it, but also avoids delaying the output of the decoded
       * ephemeris.  This does have the unfortunate side effect that
       * the first ephemeris for a given t_b will not have the LTDMP. */
   class PNBGLOCNavDataFactory : public PNBNavDataFactory
   {
   public:
         /// Initialize data members.
      PNBGLOCNavDataFactory();

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

         /** Process the header data in any given string.
          * @pre navIn must be a GloCivilC nav string of 300 bits.
          * @param[in] navIn The as-broadcast nav data string.
          * @param[in,out] navOut The object to decode the header into.
          * @return true if successful.
          */
      bool processHeader(const PackedNavBitsPtr& navIn, GLOCNavHeader& navOut);

         /** Process the health bits in the GLONASS CDMA Nav header.
          * @param[in] navIn The as-broadcast nav data string.
          * @param[out] navOut Any resulting NavData objects that were
          *   completed, usually as a result of adding navIn to the
          *   set of data.
          * @return true if successful.
          */
      bool processHealth(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

         /** Process Earth data, including iono and time offset.
          * @param[in] navIn The as-broadcast nav data string.
          * @param[out] navOut Any resulting NavData objects that were
          *   completed, usually as a sresult of adding navIno the set
          *   of data.
          * @return true if successful.
          */
      bool processEarth(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

         /** Process strings type 10-12.  When a complete ephemeris of
          * strings 10-12 with consecutive time stamps is accumulated in
          * ephAcc, that ephemeris is placed in navOut.  An ISC and/or
          * health object may also be added to navOut when available
          * and such processing is enabled. 
          * @param[in] stringID The string ID (10-12) pertaining to navIn.
          * @param[in] navIn The as-broadcast ephemeris bits.
          * @param[out] navOut If an ephemeris is completed and
          *   requested, this will contain a GLOCNavEph object.  If
          *   navIn contains string 4 and ISC data is requested,
          *   navOut will contain a GLOCNavISC object.  If an
          *   ephemeris is completed and health data is requested,
          *   navOut will contain a GLOCNavHealth object.
          * @return false on error. */
      bool processEph(unsigned long stringID, const PackedNavBitsPtr& navIn,
                      NavDataPtrList& navOut);

         /** Process string type 20.  That almanac is placed in
          * navOut.
          * @param[in] navIn The as-broadcast almanac bits.
          * @param[out] navOut If an almanac is completed and the data
          *   is requested, this will contain a GLOCNavAlm object.  If
          *   an almanac is completed and health data is requested, it
          *   will contain a GLOCNavHealth object.
          * @return false on error. */
      bool processAlm(const PackedNavBitsPtr& navIn,
                      NavDataPtrList& navOut);

         /** Process strings 31 and 32 which contain the long-term
          * dynamic model parameters.
          * @param[in] stringID The string ID (31 or 32) pertaining to navIn.
          * @param[in] navIn The as-broadcast string 31 or 32 bits.
          * @return false on error. */
      bool processLTDMP(unsigned long stringID,
                        const PackedNavBitsPtr& navIn);

   protected:
         /// Class to store fields used to turn GLONASS into UT1.
      class TimeMeta
      {
      public:
            /// Set the value of N4 (from string 10).
         void setN4(unsigned v);
            /// Return true if all data fields are valid.
         bool isValid();
         vuint N4; ///< Leap years since 1996.
      };
         /** Map GLONASS transmit satellite ID to TimeMeta to
          * accumulate complete GLONASS to UT1 time corrections. */
      std::map<NavSatelliteID, TimeMeta> timeAcc;
         /** Map GLONASS satellite ID to a vector of PackedNavBits for
          * accumulating ephemeris data, where index 0 is string 1
          * and so on. */
      std::map<NavSatelliteID, std::vector<PackedNavBitsPtr> > ephAcc;
         /** Map GLONASS satellite ID to a vector of GLOCNavLTDMP for
          * accumulating ephemeris data */
      std::map<NavSatelliteID, GLOCNavLTDMP> ltdmpAcc;
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
   }; // class PNBGLOCNavDataFactory

      //@}

} // namespace gnsstk

#endif // GNSSTK_PNBGLOCNAVDATAFACTORY_HPP
