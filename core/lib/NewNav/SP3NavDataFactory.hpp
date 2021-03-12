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
#ifndef GPSTK_SP3NAVDATAFACTORY_HPP
#define GPSTK_SP3NAVDATAFACTORY_HPP

#include "NavDataFactoryWithStoreFile.hpp"
#include "SP3Data.hpp"
#include "SP3Header.hpp"
#include "GPSLNavEph.hpp"
#include "ClockSatStore.hpp"
#include "PositionSatStore.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Factory class for loading SP3 data.
       * @note While in theory Y code and P code LNav are supported,
       *   SP3 does not support the identification of the codes
       *   contained in the files.  As such, this factory only
       *   "produces" L1 C/A tagged data for LNav.
       * @note SP3 does not contain health information.
       */
   class SP3NavDataFactory : public NavDataFactoryWithStoreFile
   {
   public:
         /** Fill supportedSignals.
          * @note Only GPS nav is supported so only that
          *   will be added to supportedSignals.
          */
      SP3NavDataFactory();

         /// Clean up.
      virtual ~SP3NavDataFactory()
      {
      }

         /** Search the store to find the navigation message that
          * meets the specified criteria.  SP3 is only capable of
          * containing ephemeris data.  This implementation of the
          * find method works by finding existing data in the
          * position/velocity tables and returning an OrbitDataSP3
          * that is already interpolated to get the appropriate values
          * at the desired time.
          * @param[in] nmid Specify the message type, satellite and
          *   codes to match.
          * @param[in] when The time of interest to search for data.
          * @param[out] navData The resulting navigation message.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful.  If false, navData will be untouched. */
      bool find(const NavMessageID& nmid, const CommonTime& when,
                NavDataPtr& navData, SVHealth xmitHealth, NavValidityType valid,
                NavSearchOrder order) override;

         /** Load a file into internal store.
          * @param[in] source The path to the SP3 file to load.
          * @return true on success, false on failure. */
      bool addDataSource(const std::string& source) override;

         /// Return a comma-separated list of formats supported by this factory.
      std::string getFactoryFormats() const override;

         /** Convert SP3 nav data to a OrbitDataSP3 object with
          * position and velocity data.
          * @param[in] head The header from the SP3 file being converted.
          * @param[in] navIn The SP3 nav message data to convert.
          * @param[in] isC If true, the SP3Data comes from an SP3c file.
          * @param[out] navOut The OrbitDataSP3 object to be added to the
          *   factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToOrbit(const SP3Header& head, const SP3Data& navIn,
                                 bool isC, NavDataPtr& navOut);

         /** Convert SP3 nav data to a OrbitDataSP3 object with SV
          * clock offset data.
          * @param[in] head The header from the SP3 file being converted.
          * @param[in] navIn The SP3 nav message data to convert.
          * @param[in] isC If true, the SP3Data comes from an SP3c file.
          * @param[out] clkOut The OrbitDataSP3 object to be added to the
          *   factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToClock(const SP3Header& head, const SP3Data& navIn,
                                 bool isC, NavDataPtr& clkOut);

         /** Half of the interpolation order.  When interpolating SP3
          * records, 2x this number of records are used, centered on
          * the time of interest. */
      unsigned halfOrder;

   private:
      bool store(bool process, NavDataPtr& obj);

      bool findGeneric(NavMessageType nmt, const NavSatelliteID& nsid,
                       const CommonTime& when, NavDataPtr& navData);

         /** Interpolate the ephemeris data
          * (position/velocity/acceleration) from the data in the
          * sequence [ti1,ti3).
          * @param[in] ti1 The iterator marking the first OrbitDataSP3
          *   object in the internal store to be used in interpolation.
          * @param[in] ti3 The iterator marking the first OrbitDataSP3
          *   object in the internal store after ti1 to NOT be used
          *   interpolation (use like end() in typical iterator
          *   usage).
          * @param[in] when The time at which to interpolate the data.
          * @param[in,out] navData The pre-allocated NavDataPtr object
          *   that stores the interpolated OrbitDataSP3. */
      void interpolateEph(const NavMap::iterator& ti1,
                          const NavMap::iterator& ti3,
                          const CommonTime& when, NavDataPtr& navData);

         /** Interpolate the SV clock correction data
          * (bias/drift/drift rate) from the data in the sequence
          * [ti1,ti3).
          * @param[in] ti1 The iterator marking the first OrbitDataSP3
          *   object in the internal store to be used in interpolation.
          * @param[in] ti3 The iterator marking the first OrbitDataSP3
          *   object in the internal store after ti1 to NOT be used
          *   interpolation (use like end() in typical iterator
          *   usage).
          * @param[in] when The time at which to interpolate the data.
          * @param[in,out] navData The pre-allocated NavDataPtr object
          *   that stores the interpolated OrbitDataSP3. */
      void interpolateClk(const NavMap::iterator& ti1,
                          const NavMap::iterator& ti3,
                          const CommonTime& when, NavDataPtr& navData);

         /** Load SP3 nav data into a map.
          * @note This method is unused, in favor of overriding
          *   addDataSource directly and using its own store rather
          *   than the one in NavDataFactoryWithStore.
          * @param[in] filename The path of the file to load.
          * @param[out] navMap The map to store the loaded data in.
          * @return true on succes, false on failure. */
      bool loadIntoMap(const std::string& filename,
                       NavMessageMap& navMap) override
      { return false; }

         /** Used to make sure that we don't load SP3 data with
          * inconsistent time systems. */
      TimeSystem storeTimeSystem;
   };

      //@}

}

#endif // GPSTK_SP3NAVDATAFACTORY_HPP
