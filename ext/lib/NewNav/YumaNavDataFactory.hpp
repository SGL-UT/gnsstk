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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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
#ifndef GPSTK_YUMANAVDATAFACTORY_HPP
#define GPSTK_YUMANAVDATAFACTORY_HPP

#include "NavDataFactoryWithStoreFile.hpp"
#include "YumaData.hpp"
#include "GPSLNavAlm.hpp"

namespace gpstk
{
      /** Factory class for loading Yuma nav data.
       * @note While in theory Y code and P code LNav are supported,
       *   Yuma nav does not support the identification of the codes
       *   contained in the files.  As such, this factory only
       *   "produces" L1 C/A tagged data for LNav.
       */
   class YumaNavDataFactory : public NavDataFactoryWithStoreFile
   {
   public:
         /** Fill supportedSignals.
          * @note Only GPS nav is supported so only that will be added
          *   to supportedSignals.
          */
      YumaNavDataFactory();

         /// Clean up.
      virtual ~YumaNavDataFactory()
      {
      }

         /** Load Yuma nav data into a map.
          * @param[in] filename The path of the file to load.
          * @param[out] navMap The map to store the loaded data in.
          * @return true on succes, false on failure. */
      bool loadIntoMap(const std::string& filename,
                       NavMessageMap& navMap) override;

         /// Return a comma-separated list of formats supported by this factory.
      std::string getFactoryFormats() const override;

         /** Convert Yuma nav data to a system/code-appropriate
          * OrbitData object.
          * @param[in] navIn The Yuma nav message data to convert.
          * @param[out] navOut The OrbitData object to be added to the
          *   factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToOrbit(const YumaData& navIn,
                                 NavDataPtr& navOut);

         /** Convert Yuma nav data to a system/code-appropriate
          * NavHealthData object.
          * @param[in] navIn The Yuma nav message data to convert.
          * @param[out] healthOut The NavHealthData object to be added
          *   to the factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToHealth(const YumaData& navIn,
                                  NavDataPtr& healthOut);

         /** Fill the high level (NavData and above) data for an
          * object using information from a Yuma nav record.
          * @param[in] navIn The Yuma nav message data to convert.
          * @param[in,out] navOut A NavDataPtr that has already been
          *   allocated for the appropriate leaf node class
          *   (e.g. GPSLNavEph, GPSLNavHealth, etc.)
          * @post If navIn contains valid, supported data, the
          *   timeStamp and signal data members will be filled in.
          */
      static void fillNavData(const YumaData& navIn, NavDataPtr& navOut);
   };
}

#endif // GPSTK_YUMANAVDATAFACTORY_HPP
