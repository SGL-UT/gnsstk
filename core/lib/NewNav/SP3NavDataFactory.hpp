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
#ifndef GNSSTK_SP3NAVDATAFACTORY_HPP
#define GNSSTK_SP3NAVDATAFACTORY_HPP

#include "NavDataFactoryWithStoreFile.hpp"
#include "SP3Data.hpp"
#include "SP3Header.hpp"
#include "ClockSatStore.hpp"
#include "PositionSatStore.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Factory class for loading SP3 data.
       * @note In order to use SP3 data, both ephemeris and clock must
       *   be selected via setTypeFilter (by default all data is
       *   selected).
       * @note SP3 does not contain health information.
       */
   class SP3NavDataFactory : public NavDataFactoryWithStoreFile
   {
   public:
         /// Types of interpolation that can be used on the clock data
      enum class ClkInterpType
      {
         Linear,
         Lagrange,
         Derivative
      };
         /// Generic ObsIDs for each GNSS.
      static const ObsID oidGPS, oidGalileo, oidQZSS, oidGLONASS, oidBeiDou;
         /// Generic NavTypes for each GNSS.
      static const NavType ntGPS, ntGalileo, ntQZSS, ntGLONASS, ntBeiDou;

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
          * @param[out] navOut The resulting navigation message.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful.  If false, navOut will be untouched. */
      bool find(const NavMessageID& nmid, const CommonTime& when,
                NavDataPtr& navOut, SVHealth xmitHealth, NavValidityType valid,
                NavSearchOrder order) override;

         /** Load a file into internal store.
          * @post If RINEX clock data is successfully loaded, the
          *   factory will be automatically switched to use RINEX
          *   clock data in place of SP3 clock.  As a side effect, any
          *   SP3 clock data will be removed from the internal store
          *   and any subsequent attempts to load SP3 data will not
          *   include the clock data from those SP3 files.
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

         /** Because SP3 files don't identify signals (the data is
          * computed, not broadcast). we use this function to
          * translate from arbitrary NavMessageID objects into ones
          * that will match the internally stored data.  This is used
          * by find() but is public in case someone finds a use for
          * it. 
          * @note The system used for making the generic NavMessageID
          *   comes from the subject satellite.
          * @param[in] nmidIn The NavMessageID to translate to generic form.
          * @param[out] nmidOut The generic form of the NavMessageID.
          * @return true if successful, false if the system is unsupported. */
      static bool transNavMsgID(const NavMessageID& nmidIn,
                                NavMessageID& nmidOut);

         /** Clear the clock dataset only, meaning remove all clock
          * data from the internal store. */
      void clearClock()
      { data.erase(NavMessageType::Clock); }

         /** Choose to load the clock data tables from RINEX clock
          * files. This will clear the clock store if the state
          * changes (from using to not using or vice versa).  The
          * addDataSource() method should be called after this, to
          * load data into the clock store from RINEX clock files (or
          * SP3 if changing back to using SP3 clock files).
          * @note If this is used prior to calling addDataSource()
          *   with an SP3 file, the SP3 clock data will not be loaded.
          *   But if it's called after loading SP3 or RINEX clock
          *   data, that data will be cleared from the internal
          *   storage. */
      void useRinexClockData(bool useRC = true);

         /** Set rejectBadPosFlag; if true then all values in a record
          * are rejected when that record contains a bad position,
          * while adding data to the store. */
      void rejectBadPositions(bool flag)
      { rejectBadPosFlag = flag; }

         /** Set rejectBadClockFlag; if true then all values in a
          * record are rejected when that record contains a bad clock,
          * while adding data to the store. */
      void rejectBadClocks(bool flag)
      { rejectBadClockFlag = flag; }

         /** Set rejectPredPosFlag; if true then predicted position
          * values in a record are rejected when adding data to the
          * store. */
      void rejectPredPositions(bool flag)
      { rejectPredPosFlag = flag; }

         /** Set rejectPredClockFlag; if true then predicted clock
          * values are rejected when adding data to the store. */
      void rejectPredClocks(bool flag)
      { rejectPredClockFlag = flag; }

         /** Half of the interpolation order.  When interpolating SP3
          * records, 2x this number of records are used, centered on
          * the time of interest. */
      unsigned halfOrder;

   private:
         /** Load a RINEX clock file into internal store.
          * @post If RINEX clock data is successfully loaded, the
          *   factory will be automatically switched to use RINEX
          *   clock data in place of SP3 clock.
          * @param[in] source The path to the RINEX clock file to load.
          * @return true on success, false on failure. */
      bool addRinexClock(const std::string& source);
      
         /** Store the given NavDataPtr object internally, provided it
          * passes any requested valditity checking. 
          * @param[in] process If true, validate and store.  If false,
          *   no storing or validation is done.  This is done to
          *   obviate having to check whether ephemeris or clock data
          *   is being processed prior to calling this method. 
          * @param[in,out] obj The object to (potentially) be stored.
          *   Regardless of whether the object is stored, the pointer
          *   will be reset, freeing this particular use. 
          * @return false if a failed attempt was made to store the
          *   data, true otherwise. */
      bool store(bool process, NavDataPtr& obj);

         /** Implementation of the core of what goes on in the find()
          * method.  This is a separate function because the
          * user-accessible find() method needs to acquire both clock
          * and ephemeris data to return a complete record.
          * @param[in] nmt The type of message to find (Ephemeris or Clock).
          * @param[in] nsid The satellite and codes to search for.
          * @param[in] when The time for which the data should be
          *   retrieved (and interpolated, if appropriate).
          * @param[in,out] navData The navData object, in the form of
          *   an OrbitDataSP3, to contain the results.  If navData is
          *   not allocated, it will be.
          * @return true on success, false if unable to find data or
          *   interpolate. */
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

         /** Set the obs and nav identification for the given NavMessageID
          * object, using a satellite system only.  This is really
          * just a "best guess" for a given system. 
          * @param[in] sat The satellite identifier (incl GNSS) for the data.
          * @param[in,out] signal The NavMessageID object to update.
          * @return true if successful, false if the system is unsupported. */
      static bool setSignal(const SatID& sat, NavMessageID& signal);

         /** Used to make sure that we don't load SP3 data with
          * inconsistent time systems. */
      TimeSystem storeTimeSystem;

         /** Flag indicating whether the clock store contains data
          * from SP3 (true, the default) or RINEX clock (false)
          * files */
      bool useSP3clock;

         /** Flag to reject all data in a record when that record
          * contains a bad position, default true. */
      bool rejectBadPosFlag;

         /** Flag to reject all data in a record when that record
          * contains bad clocks, default true.
          * @note this flag has no effect if the clock store comes
          * from RINEX clock files. */
      bool rejectBadClockFlag;

         /** Flag to reject predicted position data, using
          * orbitPredFlag in SP3Data. */
      bool rejectPredPosFlag;

         /** Flag to reject predicted clock data, using clockPredFlag
          * in SP3Data.
          * @note this flag has no effect if the clock store comes
          * from RINEX clock files. */
      bool rejectPredClockFlag;

         /// Clock data interpolation method.
      ClkInterpType interpType;
   };

      //@}

}

#endif // GNSSTK_SP3NAVDATAFACTORY_HPP
