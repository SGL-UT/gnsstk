//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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
#ifndef GNSSTK_PNBGPSCNAVDATAFACTORY_HPP
#define GNSSTK_PNBGPSCNAVDATAFACTORY_HPP

#include "PNBNavDataFactory.hpp"
#include "GPSWeekSecond.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** This class provides a factory that processes PackedNavBits
       * data containing GPS CNav (Civil Nav, aka data ID 4) data as
       * defined in IS-GPS-200 section 30.  The results of the addData
       * method (in the navOut argument) may contain any number of
       * GPSCNavAlm, GPSCNavEph, GPSCNavHealth or GPSCNavTimeOffset
       * objects, according to what data is fed to the method and what
       * data is requested via the validity and type filters (see
       * PNBNavDataFactory).
       * @note Currently validity is not checked in any way in this class.
       * @note Currently only message types 10, 11, 30, 32 and 33 are
       *   being broadcast by the constellation.  As such, no further
       *   effort is going to be spent at this time on decoding
       *   messages outside this set.  GPSCNavAlm data structure and
       *   decoding was already implemented by this time, but the data
       *   isn't being transmitted and has not been tested. */
   class PNBGPSCNavDataFactory : public PNBNavDataFactory
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

         /** Process ephemeris messages 1-2.  When a complete and
          * consistent ephemeris is accumulated in ephAcc, that
          * ephemeris is placed in navOut.
          * @param[in] msgType The CNAV message type (10-11).
          * @param[in] navIn The as-broadcast ephemeris message bits.
          * @param[out] navOut If an ephemeris is completed, this will
          *   contain a GPSCNavEph object.
          * @return false on error. */
      bool processEph(unsigned msgType, const PackedNavBitsPtr& navIn,
                      NavDataPtrList& navOut);

         /** Process message type 37 (GPS, QZSS) or 53 (QZSS).
          * @param[in] msgType The CNAV message type.
          * @param[in] navIn The PackedNavBits data containing the message.
          * @param[out] navOut The GPSCNavAlm and/or GPSCNavHealth
          *   objects generated from navIn.
          * @return true if successful (navOut may still be empty). */
      bool processAlmOrb(unsigned msgType, const PackedNavBitsPtr& navIn,
                         NavDataPtrList& navOut);

         /** Process message type 12.  This includes reduced almanac data.
          * @param[in] msgType Type of the message that contains the
          *   packet being decoded.  Used to determine the subject
          *   satellite in QZSS.
          * @param[in] navIn The PackedNavBits data containing the message.
          * @param[out] navOut The GPSCNavTimeOffset object generated from
          *   navIn.
          * @return true if successful. */
      bool process12(unsigned msgType, const PackedNavBitsPtr& navIn,
                     NavDataPtrList& navOut);

         /** Process message type 30.  This includes ionospheric
          * correction information for single-frequency users.
          * @param[in] navIn The PackedNavBits data containing the message.
          * @param[out] navOut The GPSCNavIono object generated from
          *   navIn.
          * @return true if successful. */
      bool process30(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

         /** Process message type 31.  This includes reduced almanac data.
          * @param[in] msgType Type of the message that contains the
          *   packet being decoded.  Used to determine the subject
          *   satellite in QZSS.
          * @param[in] navIn The PackedNavBits data containing the message.
          * @param[out] navOut The GPSCNavTimeOffset object generated from
          *   navIn.
          * @return true if successful. */
      bool process31(unsigned msgType, const PackedNavBitsPtr& navIn,
                     NavDataPtrList& navOut);

         /** Process a single reduced almanac packed from either
          * message type 12 or 31 (or, for QZSS, including message
          * types 28 and 47).
          * @param[in] msgType Type of the message that contains the
          *   packet being decoded.  Used to determine the subject
          *   satellite in QZSS.
          * @param[in] offset The position of the first bit of the
          *   reduced almanac packet within navIn.
          * @param[in] pre The 8-bit preamble taken from the message start.
          * @param[in] alert The 1-bit alert flag taken from the message start.
          * @param[in] wna The WNa (almanac week number) as extracted
          *   from the message.  This value is shared across the
          *   message and applies to all packets in a given message.
          * @param[in] toa The almanac reference time in seconds of
          *   week (see wna).
          * @param[in] navIn The PackedNavBits data containing the message.
          * @param[out] navOut The GPSCNavRedAlm object generated from
          *   navIn.
          * @return true if successful. */
      bool processRedAlmOrb(unsigned msgType, unsigned offset, unsigned pre,
                            bool alert, unsigned wna, double toa,
                            const PackedNavBitsPtr& navIn,
                            NavDataPtrList& navOut);

         /** Process message type 33.  This includes GPS-UTC time offset data.
          * @param[in] navIn The PackedNavBits data containing the message.
          * @param[out] navOut The GPSCNavTimeOffset object generated from
          *   navIn.
          * @return true if successful. */
      bool process33(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

         /** Process message type 35.  This includes GPS-GNSS time offset data.
          * @param[in] navIn The PackedNavBits data containing the message.
          * @param[out] navOut The GPSCNavTimeOffset object generated from
          *   navIn.
          * @return true if successful. */
      bool process35(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

         /** Reset the state of the data accumulator.  This method is
          * intended to be used to clear out that intermediate data to
          * start from a fresh state, e.g. if you're loading
          * discontinuous data. */
      void resetState() override
      { ephAcc.clear(); }

         /** For debugging purposes, dump the sizes of the accumulator maps.
          * @param[in,out] s The stream to write the debug output to. */
      void dumpState(std::ostream& s) const;

   protected:
         /** Map GPS PRN to a vector of PackedNavBits for accumulating
          * ephemeris data, where index 0 is message 10 and so on. */
      std::map<unsigned, std::vector<PackedNavBitsPtr> > ephAcc;
   };

      //@}

} // namespace gnsstk

#endif // GNSSTK_PNBGPSCNAVDATAFACTORY_HPP
