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
#ifndef GNSSTK_PNBGPSCNAV2DATAFACTORY_HPP
#define GNSSTK_PNBGPSCNAV2DATAFACTORY_HPP

#include "NavData.hpp"
#include "PNBNavDataFactory.hpp"
#include "GPSWeekSecond.hpp"
#include "GPSCNav2ISC.hpp"
#include "PackedNavBits.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** This class provides a factory that processes PackedNavBits
       * data containing GPS CNav2 (Civil Nav on L1C) data as defined
       * in IS-GPS-800 section 3.5.  The results of the addData method
       * (in the navOut argument) may contain any number of
       * GPSCNav2Alm, GPSCNav2Eph, GPSCNav2Health or
       * GPSCNav2TimeOffset objects, according to what data is fed to
       * the method and what data is requested via the validity and
       * type filters (see PNBNavDataFactory).
       * @note Currently validity is not checked in any way in this class. */
   class PNBGPSCNav2DataFactory : public PNBNavDataFactory
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

         /** Process ephemeris messages.
          * @param[in] navIn The as-broadcast ephemeris message bits.
          * @param[out] navOut This will contain a GPSCNav2Eph object.
          * @param[in] offset The bit offset for the start of the ephemeris.
          * @return false on error. */
      bool processEph(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
                      unsigned offset = 0);

         /** Process subframe 3, any page.
          * @param[in] navIn The PackedNavBits data containing the message.
          * @param[out] navOut The GPSCNav2Alm, GPSCNav2Health, GPSNavConfig,
          *   and/or GPSCNav2TimeOffset objects generated from navIn.
          * @param[in] offset The bit offset for the start of subframe 3.
          * @return true if successful (navOut may still be empty). */
      bool processSF3(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
                      unsigned offset = 0);

         /** Process subframe 3 page 4 (midi almanac).
          * @param[in] navIn The PackedNavBits data containing the message.
          * @param[out] navOut The GPSCNav2Alm and/or GPSCNav2Health
          *   objects generated from navIn.
          * @param[in] offset The bit offset for the start of the almanac.
          * @return true if successful (navOut may still be empty). */
      bool processAlmOrb(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
                         unsigned offset = 0);

         /** Process subframe 3 page 1 (UTC & IONO).
          * @param[in] navIn The PackedNavBits data containing the message.
          * @param[out] navOut The GPSCNav2TimeOffset objects
          *   generated from navIn.
          * @param[in] offset The bit offset for the start of the time offset.
          * @return true if successful (navOut may still be empty). */
      bool processUTCIono(const PackedNavBitsPtr& navIn,
                          NavDataPtrList& navOut, unsigned offset = 0);

         /** Process subframe 3 page 2 (GGTO & EOP).
          * @param[in] navIn The PackedNavBits data containing the message.
          * @param[out] navOut The GPSCNav2TimeOffset objects
          *   generated from navIn.
          * @param[in] offset The bit offset for the start of the time offset.
          * @return true if successful (navOut may still be empty). */
      bool processGGTOEOP(const PackedNavBitsPtr& navIn,
                          NavDataPtrList& navOut, unsigned offset = 0);

#if false /// @todo enable this if the SV config page is ever observed
         /** Process subframe 3 page 7 (SV config).
          * @param[in] navIn The PackedNavBits data containing the message.
          * @param[out] navOut The GPSNavConfig objects generated from navIn.
          * @param[in] offset The bit offset for the start of the SV config.
          * @return true if successful (navOut may still be empty). */
      bool processSVConfig(const PackedNavBitsPtr& navIn,
                           NavDataPtrList& navOut, unsigned offset = 0);
#endif

         /**
          * @note If processISC is given a sequence of nothing but
          * subframe 2, the last subframe 2 won't produce an ISC
          * object. */
      bool processISC(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

         /// Reset the state of the data accumulator (for ISCs).
      void resetState() override
      { iscAcc.clear(); }

         /** Adjust a timestamp so it matches the transmit time of
          * subframe 2, which is going to be offset by 0.52 seconds */
      static CommonTime getSF2Time(const CommonTime& timestamp);

         /** Adjust a timestamp so it matches the transmit time of
          * subframe 3, which is going to be offset by 12.52 seconds */
      static CommonTime getSF3Time(const CommonTime& timestamp);

   protected:
         /// Quick alias for a shared_ptr to GPSCNav2ISC.
      typedef std::shared_ptr<GPSCNav2ISC> GPSCNav2ISCPtr;

         /// Map signal to ISC for accumulating data between SF2 and SF3.
      std::map<NavSatelliteID, GPSCNav2ISCPtr> iscAcc;

         /** Retrieve an existing GPSCNav2ISCPtr from iscAsc or create
          * and return a new one.
          * @param[in] navIn The PackedNavBits being used to construct
          *   the ISC data (for setting the time stamp).
          * @param[in] nsid The satellite/signal identifier for the
          *   data contained within navIn.
          * @return A GPSCNav2ISCPtr object corresponding to the
          *   specified signal. */
      GPSCNav2ISCPtr getISCObj(const PackedNavBitsPtr& navIn,
                               const NavSatelliteID& nsid);
   };

      //@}

} // namespace gnsstk

#endif // GNSSTK_PNBGPSCNAV2DATAFACTORY_HPP
