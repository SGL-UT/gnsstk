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
#ifndef GNSSTK_SEMNAVDATAFACTORY_HPP
#define GNSSTK_SEMNAVDATAFACTORY_HPP

#include "NavData.hpp"
#include "NavDataFactoryWithStoreFile.hpp"
#include "SEMData.hpp"
#include "GPSLNavAlm.hpp"

namespace gnsstk
{
      /** Factory class for loading SEM nav data.
       * @note While in theory Y code and P code LNav are supported,
       *   SEM nav does not support the identification of the codes
       *   contained in the files.  As such, this factory only
       *   "produces" L1 C/A tagged data for LNav.
       */
   class SEMNavDataFactory : public NavDataFactoryWithStoreFile
   {
   public:
         /** Fill supportedSignals.
          * @note Only GPS nav is supported so only that will be added
          *   to supportedSignals.
          */
      SEMNavDataFactory();

         /// Clean up.
      virtual ~SEMNavDataFactory()
      {
      }

         /** Load SEM nav data into a map.
          * @param[in] filename The path of the file to load.
          * @param[out] navMap The map to store the loaded data in.
          * @param[out] navNearMap The map to store the loaded data in
          *   for use by "Nearest" (as opposed to "User") searches.
          * @param[out] ofsMap The map to load TimeOffsetData into.
          * @return true on succes, false on failure. */
      bool loadIntoMap(const std::string& filename,
                       NavMessageMap& navMap,
                       NavNearMessageMap& navNearMap,
                       OffsetCvtMap& ofsMap) override;

         /// @copydoc NavDataFactoryWithStoreFile::process(const std::string&,NavDataFactoryCallback&)
      bool process(const std::string& filename,
                   NavDataFactoryCallback& cb) override;

         /// Return a comma-separated list of formats supported by this factory.
      std::string getFactoryFormats() const override;

         /** Convert SEM nav data to a system/code-appropriate
          * OrbitData object.
          * @param[in] navIn The SEM nav message data to convert.
          * @param[out] navOut The OrbitData object to be added to the
          *   factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToOrbit(const SEMData& navIn,
                                 NavDataPtr& navOut);

         /** Convert SEM nav data to a system/code-appropriate
          * NavHealthData object.
          * @param[in] navIn The SEM nav message data to convert.
          * @param[out] healthOut The NavHealthData object to be added
          *   to the factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToHealth(const SEMData& navIn,
                                  NavDataPtr& healthOut);

         /** Convert SEM nav data to a system/code-appropriate
          * SystemNavData object.
          * @param[in] navIn The SEM nav message data to convert.
          * @param[out] healthOut The NavHealthData object to be added
          *   to the factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToSystem(const SEMData& navIn,
                                  NavDataPtr& systemOut);

         /** Fill the high level (NavData and above) data for an
          * object using information from a SEM nav record.
          * @param[in] navIn The SEM nav message data to convert.
          * @param[in,out] navOut A NavDataPtr that has already been
          *   allocated for the appropriate leaf node class
          *   (e.g. GPSLNavEph, GPSLNavHealth, etc.)
          * @post If navIn contains valid, supported data, the
          *   timeStamp and signal data members will be filled in.
          */
      static void fillNavData(const SEMData& navIn, NavDataPtr& navOut);
   };
}

#endif // GNSSTK_SEMNAVDATAFACTORY_HPP
