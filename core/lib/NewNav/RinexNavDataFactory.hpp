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
#ifndef GNSSTK_RINEXNAVDATAFACTORY_HPP
#define GNSSTK_RINEXNAVDATAFACTORY_HPP

#include "NavDataFactoryWithStoreFile.hpp"
#include "Rinex3NavData.hpp"
#include "GPSLNavEph.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Factory class for loading RINEX NAV (2, 3) data.
       * @note While in theory Y code and P code LNav are supported,
       *   RINEX NAV does not support the identification of the codes
       *   contained in the files.  As such, this factory only
       *   "produces" L1 C/A tagged data for LNav.
       */
   class RinexNavDataFactory : public NavDataFactoryWithStoreFile
   {
   public:
         /** Fill supportedSignals.
          * @note Currently only GPS nav is supported so only that
          *   will be added to supportedSignals.
          */
      RinexNavDataFactory();

         /// Clean up.
      virtual ~RinexNavDataFactory()
      {
      }

         /** Load RINEX NAV data into a map.
          * @param[in] filename The path of the file to load.
          * @param[out] navMap The map to store the loaded data in.
          * @param[out] navNearMap The map to store the loaded data in
          *   for use by "Nearest" (as opposed to "User") searches.
          * @param[out] ofsMap The map to load TimeOffsetData into.
          * @return true on success, false on failure. */
      bool loadIntoMap(const std::string& filename,
                       NavMessageMap& navMap,
                       NavNearMessageMap& navNearMap,
                       OffsetCvtMap& ofsMap) override;

         /// Return a comma-separated list of formats supported by this factory.
      std::string getFactoryFormats() const override;

         /** Convert RINEX nav data to a system/code-appropriate
          * OrbitData object.
          * @param[in] navIn The RINEX nav message data to convert.
          * @param[out] navOut The OrbitData object to be added to the
          *   factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToOrbit(const Rinex3NavData& navIn,
                                 NavDataPtr& navOut);

         /** Copy common elements from Rinex3NavData to OrbitDataKepler.
          * @param[in] navIn The RINEX nav message data to convert.
          * @param[in,out] navOut The pre-allocated OrbitDataKepler
          *   object to be updated from the RINEX nav data.
          */
      static void convertToOrbitDataKepler(const Rinex3NavData& navIn,
                                           OrbitDataKepler* navOut);

         /** Convert RINEX nav data to a system/code-appropriate
          * NavHealthData object.
          * @param[in] navIn The RINEX nav message data to convert.
          * @param[out] healthOut The NavHealthData object(s) to be added
          *   to the factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToHealth(const Rinex3NavData& navIn,
                                  NavDataPtrList& healthOut);

         /** Convert RINEX nav header data to a TimeOffsetData object.
          * @param[in] navIn The RINEX nav header to convert.
          * @param[out] navOut The GPSCNavTimeOffset object generated from
          *   navIn.
          * @return true if successful.
          */
      static bool convertToOffset(const Rinex3NavHeader& navIn,
                                  NavDataPtrList& navOut);

         /** Convert RINEX nav header data into a IonoData object.
          * @param[in] when A timestamp to use for the IonoData, since
          *   the RINEX nav header doesn't include time information on
          *   its own (usually a timestamp pulled from the data).
          * @param[in] navIn The RINEX nav header to convert.
          * @param[out] navOut A list (possibly empty) of the
          *   resulting converted data.  This may include
          *   NavHealthData which is necessary due to the minimalist
          *   storage used by RINEX vs the implementation of
          *   NavLibrary::getIonoCorr().
          * @return true if successful. */
      static bool convertToIono(const CommonTime& when,
                                const Rinex3NavHeader& navIn,
                                NavDataPtrList& navOut);

         /** Convert RINEX nav data to a InterSigCorr object.
          * @param[in] navIn The RINEX nav message data to convert.
          * @param[out] navOut The OrbitData object to be added to the
          *   factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported. */
      static bool convertToISC(const Rinex3NavData& navIn,
                               NavDataPtr& navOut);

         /** Fill the high level (NavData and above) data for an
          * object using information from a RINEX nav record.
          * @param[in] navIn The RINEX nav message data to convert.
          * @param[in,out] navOut A NavDataPtr that has already been
          *   allocated for the appropriate leaf node class
          *   (e.g. GPSLNavEph, GPSLNavHealth, etc.)
          * @post If navIn contains valid, supported data, the
          *   timeStamp and signal data members will be filled in.
          */
      static void fillNavData(const Rinex3NavData& navIn, NavDataPtr& navOut);

         /** Set the xmitTime, Toc and Toe fields in navOut according
          * to the appropriate data in navIn.
          * @param[in] navIn A GPS LNav record in RINEX format.
          * @param[in,out] navOut The GPSLNavEph object whose time
          *   stamps are to be set. */
      static void fixTimeGPS(const Rinex3NavData& navIn, GPSLNavEph& navOut);

         /** Set the xmitTime field in navOut according to the
          * appropriate data in navIn.
          * @param[in] navIn A Galileo I/Nav or F/Nav record in RINEX format.
          * @param[in,out] navOut The GalINavEph or GalFNavEph object whose time
          *   stamps are to be set. */
      static void fixTimeGalileo(const Rinex3NavData& navIn,
                                 OrbitDataKepler& navOut);

         /** Set the xmitTime field in navOut according to the
          * appropriate data in navIn.
          * @param[in] navIn A BeiDou D1Nav or D2Nav record in RINEX format.
          * @param[in,out] navOut The BDSD1NavEph or BDSD2NavEph
          *   object whose time stamps are to be set. */
      static void fixTimeBeiDou(const Rinex3NavData& navIn,
                                OrbitDataKepler& navOut);

         /** Convert accuracy in meters into a Galileo Signal In Space
          * Accuracy index.
          * @note This uses RINEX conventions (naturally) of using -1
          *   to mean No Accuracy Predicition Available.
          * @param[in] accuracy The signal accuracy in meters. */
      static uint8_t decodeSISA(double accuracy);
   };

      //@}

}

#endif // GNSSTK_RINEXNAVDATAFACTORY_HPP
