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
#ifndef GPSTK_PNBBDSD1NAVDATAFACTORY_HPP
#define GPSTK_PNBBDSD1NAVDATAFACTORY_HPP

#include "PNBNavDataFactory.hpp"
#include "BDSD1NavAlm.hpp"
#include "BDSWeekSecond.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** This class provides a factory that processes PackedNavBits
       * data containing BeiDou D1 NAV data as defined in the BeiDou
       * ICD.  The results of the addData method (in the navOut
       * argument) may contain any number of BDSD1NavAlm, BDSD1NavEph,
       * BDSD1NavHealth or BDSD1NavTimeOffset objects, according to
       * what data is fed to the method and what data is requested via
       * the validity and type filters (see PNBNavDataFactory).
       *
       * The D1 NAV data structure is set up to provide almanac and
       * health data for PRNs >30 by flagging it in other almanac
       * pages using the AmEpID.  My interpretation of the ICD, and
       * data seems to back this up, is that AmEpID is only meaningful
       * when it is equal to 3 (b11) in subframe 4, pages 11-24.  In
       * this case, the subframe 5 page (11-24) in that same frame
       * will correspond to the information in sections 5.2.4.14,
       * 5.2.4.15 and 5.2.4.17.  AmEpID can be set to 3 for other
       * pages, but it doesn't seem to carry a meaning in that
       * context.
       *
       * With respect to validity checking, the following checks are performed:
       *   \li Default almanac page data (data is all zeroes)
       *
       * The following data are discarded and never returned due to
       * the fact that attempting to provide the data to users would
       * result in exceptions and other software internal errors:
       *   \li Almanac pages with no matching toa in a subframe 5, page 8,
       *       needed for the WNa
       * @note This class does not enforce sequential messages when
       *   assembling almanac or ephemeris data.
       */
   class PNBBDSD1NavDataFactory : public PNBNavDataFactory
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

         /** Process almanac messages.  An almanac is placed in
          * navOut, if such processing is enabled.
          * @param[in] sfid Subframe ID (4 or 5).
          * @param[in] pnum The page number (1-24) pertaining to navIn.
          * @param[in] navIn The as-broadcast almanac bits.
          * @param[out] navOut If an almanac is completed and the data
          *   is requested, this will contain a BDSD1NavAlm object.
          * @return false on error. */
      bool processAlm(unsigned long sfid, unsigned pnum,
                      const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

         /** Process ephemeris messages.  A combination of ephemeris,
          * iono and ISC objects are placed in navOut, depending on
          * the type filter settings and input subframe.
          * @param[in] sfid Subframe ID (1-3).
          * @param[in] navIn The as-broadcast ephemeris bits.
          * @param[out] navOut If an ephemeris is completed and the
          *   data is requested, this will contain a BDSD1NavEph
          *   object.  If the input is subframe 1 and data is
          *   requested, this will contain BDSD1NavIono and/or
          *   BDSD1NavISC objects.
          * @return false on error. */
      bool processEph(unsigned long sfid,
                      const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

      bool processSF5Pg7(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);
      bool processSF5Pg8(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);
      bool processSF5Pg9(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);
      bool processSF5Pg10(const PackedNavBitsPtr& navIn,NavDataPtrList& navOut);
      bool processSF5Pg24(const PackedNavBitsPtr& navIn,NavDataPtrList& navOut);

      bool isAlmDefault(const PackedNavBitsPtr& navIn);

         /** Reset the state of the data accumulator.  Most
          * PNBNavDataFactory child classes will maintain some state
          * to assemble data prior to processing.  This method is
          * intended to be used to clear out that intermediate data to
          * start from a fresh state, e.g. if you're loading
          * discontinuous data. */
      void resetState() override;

         /** For debugging purposes, dump the sizes of the accumulator maps.
          * @param[in,out] s The stream to write the debug output to. */
      void dumpState(std::ostream& s) const;

         // test code, remove me before submitting.
      void bunk();
         // other test code, remove me before submitting.
      void bunk2();

   protected:
         /// Store transmit time and corresponding AmEpID for amEpIDMap.
      class AmEpID
      {
      public:
            /// Set the time system to Any to avoid exceptions.
         AmEpID()
         { t.setTimeSystem(TimeSystem::Any); }
            /// Return true if the AmEpID field indicates expanded almanac.
         bool isExpanded() const
         { return amEpID == 0x03; }

         CommonTime t;   ///< Transmit time of subframe containing AmEpID.
         uint8_t amEpID; ///< The two-bit AmEpID value.
      };
      using AlmPtr = std::shared_ptr<BDSD1NavAlm>;
      using AlmPtrList = std::list<AlmPtr>;
         /** Map transmit PRN to fully qualified week/second
          * (WNa/toa).  This is set by sf 5 pg 8. */
      std::map<unsigned, BDSWeekSecond> fullWNaMap;
         /** Accumulate almanac orbital element pages (sf 4 pg 1-24,
          * sf 5 pg 1-6) until a page sf 5 pg 8 becomes available to
          * properly set the WNa.  Key is the transmitting PRN (only
          * xmitSat is set, sat is ignored). */
      std::map<NavSatelliteID, AlmPtrList> almAcc;
         /** Keep track of the most recent AmEpID for a given
          * NavSatelliteID.  This allows us to determine whether or
          * not to emit an almanac for subframe 5, pages 11-23, or
          * health for subframe 5, page 24. */
      std::map<NavSatelliteID, AmEpID> amEpIDMap;
         /** Map PRN to a vector of PackedNavBits for accumulating
          * ephemeris data, where index 0 is subframe 1 and so on. */
      std::map<NavSatelliteID, std::vector<PackedNavBitsPtr> > ephAcc;
         /** Map PRN to health status to use in almanac objects.  The
          * data are pulled from sf 5, pg 7-8.  Unfortunately, only pg
          * 8 has toa in it, so really about the best we can do is
          * store the most recent health bits and use them. */
      std::map<NavSatelliteID, uint16_t> heaAcc;

      void makeHealth(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
                      unsigned long subjID,
                      unsigned startBit1, unsigned numBits1,
                      unsigned startBit2=0, unsigned numBits2=0);

      void finishAlm(bool fromWNa, const NavSatelliteID& key,
                     NavDataPtrList& navOut);
      bool finishAlm(AlmPtr& alm, bool fromWNa, const NavSatelliteID& key,
                     NavDataPtrList& navOut);
   }; // class PNBBDSD1NavDataFactory

      //@}

} // namespace gpstk

#endif // GPSTK_PNBBDSD1NAVDATAFACTORY_HPP
