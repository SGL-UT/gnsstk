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
#ifndef GPSTK_PNBGALINAVDATAFACTORY_HPP
#define GPSTK_PNBGALINAVDATAFACTORY_HPP

#include "PNBNavDataFactory.hpp"
#include "GalINavAlm.hpp"
#include "GalNavHealth.hpp"

namespace gpstk
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
          * @param[in] navIn The PackedNavBits data to process.
          * @param[out] navOut Any resulting NavData objects that were
          *   completed, usually as a result of adding navIn to the
          *   set of data.
          * @param[in] cadence This value is ignored in this class.
          * @return false on error. */
      bool addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut,
                   double cadence = -1)
         override;

         /** Process word types 1-5.  When a complete ephemeris of
          * word types 1,2,3,4,5 and consistent IODnav are accumulated
          * in ephAcc, that ephemeris is placed in navOut, along with
          * a GST-UTC time offset object, if such processing is
          * enabled.
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
          * @param[in] wordType The word type (7-10) pertaining to navIn.
          * @param[in] navIn The as-broadcast ephemeris bits.
          * @param[out] navOut If an ephemeris is completed, this will
          *   contain a GalINavEph object.
          * @return false on error. */
      bool processAlm(unsigned wordType, const PackedNavBitsPtr& navIn,
                      NavDataPtrList& navOut);

      bool processAlmOrb(const std::vector<PackedNavBitsPtr>& almWord,
                         GalINavAlm *alm, GalNavHealth *hp1, GalNavHealth *hp2,
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

} // namespace gpstk

#endif // GPSTK_PNBGALINAVDATAFACTORY_HPP
