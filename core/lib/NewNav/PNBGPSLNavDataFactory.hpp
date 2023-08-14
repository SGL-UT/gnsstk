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
#ifndef GNSSTK_PNBGPSLNAVDATAFACTORY_HPP
#define GNSSTK_PNBGPSLNAVDATAFACTORY_HPP

#include "PNBNavDataFactory.hpp"
#include "GPSWeekSecond.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** This class provides a factory that processes PackedNavBits
       * data containing GPS LNav (Legacy Nav, aka data ID 2) data as
       * defined in IS-GPS-200 section 20.  The results of the addData
       * method (in the navOut argument) may contain any number of
       * GPSLNavAlm, GPSLNavEph, GPSLNavHealth or GPSLNavTimeOffset
       * objects, according to what data is fed to the method and what
       * data is requested via the validity and type filters (see
       * PNBNavDataFactory).
       * @note Currently validity is not checked in any way in this class.
       * @note This class does not enforce sequential subframes in ephemerides.
       * @todo Currently this code enforces strict matching of toa
       *   between SVID 1-32 and 51.  While this helps ensure that the
       *   time stamp of the almanac orbital elements has the correct
       *   week number, it does mean that if you're missing the page
       *   51 for a particular toa, you won't get those almanacs.
       *   This is probably not ideal and likely needs to be enhanced
       *   without significantly increasing the risk of using an
       *   incorrect WNa.
       */
   class PNBGPSLNavDataFactory : public PNBNavDataFactory
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

         /** Process subframes 1-3.  When a complete ephemeris of
          * subframes 1,2,3 and consistent IODE/IODC are accumulated
          * in ephAcc, that ephemeris is placed in navOut.
          * @param[in] sfid The subframe ID (1-3) pertaining to navIn.
          * @param[in] navIn The as-broadcast ephemeris subframe bits.
          * @param[out] navOut If an ephemeris is completed, this will
          *   contain a GPSLNavEph object.
          * @return false on error. */
      bool processEph(unsigned sfid, const PackedNavBitsPtr& navIn,
                      NavDataPtrList& navOut);

         /** Process SV/page ID 1-32.  In order for GPSLNavAlm data to
          * be produced, SV/page ID 51 must have been processed.  This is
          * required in order to get the almanac week (WNa).  If page
          * 51 has not yet been processed, the almanac orbital
          * elements are stored in almAcc until such time as a page 51
          * is processed.
          * @param[in] prn The SV/page ID which represents the subject PRN.
          * @param[in] navIn The PackedNavBits data containing the subframe.
          * @param[out] navOut The GPSLNavAlm and/or GPSLNavHealth
          *   objects generated from navIn.
          * @return true if successful (navOut may still be empty). */
      bool processAlmOrb(unsigned long prn, const PackedNavBitsPtr& navIn,
                         NavDataPtrList& navOut);

         /** Process SV/page ID 51.  This includes health data for
          * PRNs 1-24 as well as the WNa.  This will likely result in
          * the generation of both GPSLNavHealth (24 of them) as well
          * as a set of GPSLNavAlm objects as the almanac structure
          * means page 51 is always last.
          * @param[in] navIn The PackedNavBits data containing the subframe.
          * @param[out] navOut The GPSLNavAlm and/or GPSLNavHealth
          *   objects generated from navIn.
          * @return true if successful (navOut may still be empty). */
      bool processSVID51(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

      /** Process SV/page ID 63.  This includes health data for
       * PRNs 25-32 and SV config for PRNs 1-32.
       * @param[in] navIn The PackedNavBits data containing the subframe.
       * @param[out] navOut The GPSLNavHealth and/or GPSNavConfig objects
       *   generated from navIn.
       * @return true if successful. */
      bool processSVID63(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

         /** Process SV/page ID 56.  This includes GPS-UTC time offset
          * data and ionospheric corrections.
          * @param[in] navIn The PackedNavBits data containing the subframe.
          * @param[out] navOut The GPSLNavTimeOffset object generated from
          *   navIn.
          * @return true if successful. */
      bool processSVID56(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

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

   protected:
         /** Map GPS transmit PRN to fully qualified week/second
          * (WNa/toa).  This is set by SV/page ID 51. */
      std::map<unsigned, GPSWeekSecond> fullWNaMap;
         /** Accumulate almanac orbital element pages (page ID 1-32)
          * until a page ID 51 becomes available to properly set the
          * WNa.  Key is the transmitting PRN (only xmitSat is set,
          * sat is ignored). */
      std::map<NavSatelliteID, NavDataPtrList> almAcc;
         /** Map GPS PRN to a vector of PackedNavBits for accumulating
          * ephemeris data, where index 0 is subframe 1 and so on. */
      std::map<NavSatelliteID, std::vector<PackedNavBitsPtr> > ephAcc;
   };

      //@}

} // namespace gnsstk

#endif // GNSSTK_PNBGPSLNAVDATAFACTORY_HPP
