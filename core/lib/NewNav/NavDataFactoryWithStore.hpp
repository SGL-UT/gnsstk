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
#ifndef GNSSTK_NAVDATAFACTORYWITHSTORE_HPP
#define GNSSTK_NAVDATAFACTORYWITHSTORE_HPP

#include "NavDataFactory.hpp"
#include "TimeOffsetData.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Define methods and data structures for internally storing
       * navigation message data internally as read from some
       * source.
       * @todo Currently it's possible for health messages from one
       *   signal to stomp health messages on another signal.
       *   Specific example: if you have a CNAV message with the three
       *   signal health bits that get split up into L1, L2 and L5,
       *   it's possible for the L1 signal from the CNAV message to
       *   overwrite the health status from an LNAV message for L1 and
       *   vice versa.  Since it's possible for the health bits to be
       *   different, we probably need to decide if we need to do
       *   something about this issue and if so, what. */
   class NavDataFactoryWithStore : public NavDataFactory
   {
   public:
         // Time offset information is organized differently due to the use case
         /** Map that will contain all TimeOffsetData objects with the
          * same conversion pair broadcast at a given time. */
      typedef std::map<NavSatelliteID, NavDataPtr> OffsetMap;
         /** Map from the timeStamp of a TimeOffsetData object to the
          * collection of TimeOffsetData objects. */
      typedef std::map<CommonTime, OffsetMap> OffsetEpochMap;
         /// Map from the time system conversion pair to the conversion objects.
      typedef std::map<TimeCvtKey, OffsetEpochMap> OffsetCvtMap;

         /// Initialize internal data.
      NavDataFactoryWithStore();

         /// Clean up.
      virtual ~NavDataFactoryWithStore()
      {
      }

         /** Search the store to find the navigation message that meets
          * the specified criteria.
          * @note In order for xmitHealth matching to occur, one must
          *   have loaded health information, meaning that health data
          *   must have been included in the type filter.  For formats
          *   like SP3, which contain no health information, you won't
          *   get any results if you specify "Healthy", "Unhealthy" or
          *   "Degraded".  Use this option with care.
          * @param[in] nmid Specify the message type, satellite and
          *   codes to match.
          * @param[in] when The time of interest to search for data.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[out] navOut The resulting navigation message.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time.
          * @return true if successful.  If false, navData will be untouched. */
      bool find(const NavMessageID& nmid, const CommonTime& when,
                NavDataPtr& navOut, SVHealth xmitHealth, NavValidityType valid,
                NavSearchOrder order) override;

         /// @copydoc NavDataFactory::getOffset()
      bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                     const CommonTime& when, NavDataPtr& offset,
                     SVHealth xmitHealth = SVHealth::Any,
                     NavValidityType valid = NavValidityType::ValidOnly)
         override;

         /** Remove all data from the internal storage in the time
          * span [fromTime,toTime).
          * @param[in] fromTime The earliest time to be removed.
          * @param[in] toTime The earliest time that will NOT be removed.
          */
      void edit(const CommonTime& fromTime, const CommonTime& toTime) override;

         /** Remove data for a specific satellite signal from the
          * internal storage in the time span [fromTime,toTime).
          * @param[in] fromTime The earliest time to be removed.
          * @param[in] toTime The earliest time that will NOT be removed.
          * @param[in] satID The complete signal specification for the
          *   data to be removed (subject satellite, transmit
          *   satellite, system, carrier, code, nav).
          */
      void edit(const CommonTime& fromTime, const CommonTime& toTime,
                const NavSatelliteID& satID) override;

         /** Remove data for all satellites matching a specific signal
          * from the internal storage in the time span
          * [fromTime,toTime).
          * @param[in] fromTime The earliest time to be removed.
          * @param[in] toTime The earliest time that will NOT be removed.
          * @param[in] signal The signal for the data to be removed
          *   (system, carrier, code, nav).
          */
      void edit(const CommonTime& fromTime, const CommonTime& toTime,
                const NavSignalID& signal) override;

         /// Remove all data from the internal store.
      void clear() override;

         /** Add a nav message to the internal store (data).
          * @param[in] nd The nav data to add.
          * @return true if successful. */
      bool addNavData(const NavDataPtr& nd)
      { return addNavData(nd, data, nearestData, offsetData); }

         /** Add a nav message to the given store.
          * @param[in] nd The nav data to add.
          * @param[out] navMap The map to load the data in.
          * @param[out] navNearMap The map to load the data in
          *   for use by "Nearest" (as opposed to "User") searches.
          * @param[out] ofsMap The map to load TimeOffsetData into.
          * @return true if successful. */
      bool addNavData(const NavDataPtr& nd, NavMessageMap& navMap,
                      NavNearMessageMap& navNearMap, OffsetCvtMap& ofsMap);

         /** Determine the earliest time for which this object can successfully
          * determine the Xvt for any object.
          * @return The initial time, or CommonTime::END_OF_TIME if no
          *   data is available. */
      CommonTime getInitialTime() const override
      { return initialTime; }

         /** Determine the latest time for which this object can successfully
          * determine the Xvt for any object.
          * @return The initial time, or CommonTime::BEGINNING_OF_TIME if no
          *   data is available. */
      CommonTime getFinalTime() const override
      { return finalTime; }

         /** Determine the timestamp of the oldest record in this
          * store for the given satellite.
          * @note This should not be confused with getInitialTime(),
	  *   which is the time of applicability, not the time stamp
	  *   of the contained data. */
      CommonTime getFirstTime(const SatID& sat) const;

      	 /** Determine the timestamp of the newest record in this
	  * store for the given satellite.
	  * @note This should not be confused with getFinalTime(),
	  *   which is the time of applicability, not the time stamp
	  *   of the contained data. */
      CommonTime getLastTime(const SatID& sat) const;

         /** Obtain a set of satellites for which we have data in the
          * given time span.
          * @param[in] fromTime The earliest time for which any
          *   messages should be available.
          * @param[in] toTime The earliest time for which any
          *   messages should be NOT available.
          * @return a set of satellites for which data is available
          *   from [fromTime,toTime).
          * @note We specifically require the time range parameters to
          *   try to avoid making assumptions about the size of the
          *   data set (i.e. assuming the data is going to be a day's
          *   worth when it's actually several years. */
      NavSatelliteIDSet getAvailableSats(const CommonTime& fromTime,
                                         const CommonTime& toTime)
         const override;

         /** Obtain a set of satellites for which we have data of a
          * specific message type in the given time span.
          * @param[in] nmt The navigation message type you're looking for.
          * @param[in] fromTime The earliest time for which any
          *   messages should be available.
          * @param[in] toTime The earliest time for which any
          *   messages should be NOT available.
          * @return a set of satellites for which data is available
          *   from [fromTime,toTime).
          * @note We specifically require the time range parameters to
          *   try to avoid making assumptions about the size of the
          *   data set (i.e. assuming the data is going to be a day's
          *   worth when it's actually several years. */
      NavSatelliteIDSet getAvailableSats(NavMessageType nmt,
                                         const CommonTime& fromTime,
                                         const CommonTime& toTime)
         const override;

         /** Similar to getAvailableSats() except it only returns the
          * basic subject satellite ID, making no further distinction
          * between codes.
          * @param[in] fromTime The earliest time for which any
          *   messages should be available.
          * @param[in] toTime The earliest time for which any
          *   messages should be NOT available.
          * @return a set of satellites for which data is available
          *   from [fromTime,toTime).
          * @note We specifically require the time range parameters to
          *   try to avoid making assumptions about the size of the
          *   data set (i.e. assuming the data is going to be a day's
          *   worth when it's actually several years. */
      std::set<SatID> getIndexSet(const CommonTime& fromTime,
                                  const CommonTime& toTime) const;

         /** Similar to getAvailableSats() except it only returns the
          * basic subject satellite ID, making no further distinction
          * between codes.
          * @param[in] nmt The navigation message type you're looking for.
          * @param[in] fromTime The earliest time for which any
          *   messages should be available.
          * @param[in] toTime The earliest time for which any
          *   messages should be NOT available.
          * @return a set of satellites for which data is available
          *   from [fromTime,toTime).
          * @note We specifically require the time range parameters to
          *   try to avoid making assumptions about the size of the
          *   data set (i.e. assuming the data is going to be a day's
          *   worth when it's actually several years. */
      std::set<SatID> getIndexSet(NavMessageType nmt,
                                  const CommonTime& fromTime,
                                  const CommonTime& toTime) const;

         /** Obtain a set of satellites+message types for which we
          * have data in the given time span.
          * @param[in] fromTime The earliest time for which any
          *   messages should be available.
          * @param[in] toTime The earliest time for which any
          *   messages should be NOT available.
          * @return a set of NavMessageID objects for which data is available
          *   from [fromTime,toTime).
          * @note We specifically require the time range parameters to
          *   try to avoid making assumptions about the size of the
          *   data set (i.e. assuming the data is going to be a day's
          *   worth when it's actually several years. */
      NavMessageIDSet getAvailableMsgs(const CommonTime& fromTime,
                                       const CommonTime& toTime) const override;

         /// Return the number of nav messages in data.
      virtual size_t size() const;
         /** Return a count of messages matching the given NavMessageID.
          * @param[in] nmid The NavMessageID to match.  Wildcards may
          *   be used. In addition to the standard wildcard support in
          *   SatID, ObsID and NavID, the signal field may be set to
          *   "Unknown" to be treated as a wildcard, as may the
          *   messageType field.  The following code snippet
          *   represents a NavMessageID with all wildcards (which will
          *   match the results from the size() method).
          *
          * \code{.cpp}
          *   NavMessageID key1(
          *      NavSatelliteID(SatID(SatelliteSystem::Unknown),
          *                     SatID(SatelliteSystem::Unknown),
          *                     ObsID(ObservationType::Any,
          *                           CarrierBand::Any,
          *                           TrackingCode::Any,
          *                           XmitAnt::Any),
          *                     NavID(NavType::Any)),
          *      NavMessageType::Unknown);
          *   key1.sat.makeWild();
          *   key1.xmitSat.makeWild();
          * \endcode
          *
          * The \a system field is initialized in the above
          * constructor from the SatID system, which is why we're
          * initializing it to "Unknown" (to make \a system a
          * wildcard).  But setting the SatID::system to Unknown
          * doesn't make SatID a wildcard, that requires the
          * SatID::makeWild() method calls.
          *
          * @note This method has a bit of overhead, though no more
          *   than find(). */
      virtual size_t count(const NavMessageID& nmid) const;
         /** Return a count of messages matching the given
          * SatelliteSystem and NavMessageType.
          * @param[in] sys The SatelliteSystem to match when counting.
          * @param[in] nmt The NavMessageType to match (default Unknown=all).
          * @note This method has a bit of overhead, though no more
          *   than find().
          * @note Only NavSignalID::system is checked, if you need to
          *   explicitly check NavSatelliteID::SatID::system, use the
          *   count(constNavMessageID&). */
      virtual size_t count(SatelliteSystem sys,
                           NavMessageType nmt = NavMessageType::Unknown)
         const;
         /** Return a count of messages where the SUBJECT satellite ID
          * matches the given SatID and message type.
          * @param[in] satID The subject satellite ID to match.
          * @param[in] nmt The NavMessageType to match (default Unknown=all).
          * @note This method has a bit of overhead, though no more
          *   than find(). */
      virtual size_t count(const SatID& satID,
                           NavMessageType nmt = NavMessageType::Unknown)
         const;
         /** Return a count of messages matching the given NavMessageType.
          * @note This method has a bit of overhead, though no more
          *   than find(). */
      virtual size_t count(NavMessageType nmt) const;
         /// Return the number of distinct signals (ignoring PRN) in the data.
      virtual size_t numSignals() const;
         /// Return the number of distinct signals including PRN, in the data.
      virtual size_t numSatellites() const;
         /** Print the contents of this store in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, DumpDetail dl) const override;
         /// Get read-only access to the nav data map (User priority).
      const NavMessageMap& getNavMessageMap() const
      { return data; }
         /// Get read-only access to the nav data map (Nearest priority).
      const NavNearMessageMap& getNavNearMessageMap() const
      { return nearestData; }
         /// Get read-only access to the time offset map.
      const OffsetCvtMap& getTimeOffsetMap() const
      { return offsetData; }
         /** Get read-only access to the nav data for a specific type/sat.
          * @param[in] nmid The nav message ID to obtain the map for.
          * @return The resulting NavMap if available or nullptr if not. */
      const NavMap* getNavMap(const NavMessageID& nmid) const;

   protected:
         /** Search the store to find the navigation message that meets
          * the specified criteria using User-oriented data.
          * @note In order for xmitHealth matching to occur, one must
          *   have loaded health information, meaning that health data
          *   must have been included in the type filter.  For formats
          *   like SP3, which contain no health information, you won't
          *   get any results if you specify "Healthy", "Unhealthy" or
          *   "Degraded".  Use this option with care.
          * @param[in] nmid Specify the message type, satellite and
          *   codes to match.
          * @param[in] when The time of interest to search for data.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[out] navData The resulting navigation message.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @return true if successful.  If false, navData will be untouched. */
      virtual bool findUser(const NavMessageID& nmid, const CommonTime& when,
                            NavDataPtr& navData, SVHealth xmitHealth,
                            NavValidityType valid);

         /** Search the store to find the navigation message that
          * meets the specified criteria using the nearest match in
          * time (e.g. nearest toe to when).
          * @note In order for xmitHealth matching to occur, one must
          *   have loaded health information, meaning that health data
          *   must have been included in the type filter.  For formats
          *   like SP3, which contain no health information, you won't
          *   get any results if you specify "Healthy", "Unhealthy" or
          *   "Degraded".  Use this option with care.
          * @param[in] nmid Specify the message type, satellite and
          *   codes to match.
          * @param[in] when The time of interest to search for data.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[out] navData The resulting navigation message.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @return true if successful.  If false, navData will be untouched. */
      virtual bool findNearest(const NavMessageID& nmid, const CommonTime& when,
                               NavDataPtr& navData, SVHealth xmitHealth,
                               NavValidityType valid);

         /** Performs an appropriate validity check based on the
          * desired validity.
          * @param[in] ti A container iterator pointing to the nav
          *   data to check.
          * @param[in] nm The NavMap that ti is iterating over.
          * @param[in] valid The desired validity for navigation data.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @return true if the validity of the nav data pointed to by
          *   ti matches the requested validity described by valid and
          *   the health status of the transmitting satellite matches
          *   xmitHealth. */
      bool validityCheck(const NavMap::iterator& ti,
                         NavMap& nm,
                         NavValidityType valid,
                         SVHealth xmitHealth,
                         const CommonTime& when);

         /** Performs an appropriate validity check based on the
          * desired validity.
          * @param[in] ndp The NavDataPtr object whose validity is to
          *   be checked.
          * @param[in] valid The desired validity for navigation data.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @return true if the validity of ndp matches the requested
          *   validity described by valid and the health status of the
          *   transmitting satellite matches xmitHealth. */
      bool validityCheck(const NavDataPtr& ndp,
                         NavValidityType valid,
                         SVHealth xmitHealth,
                         const CommonTime& when);

         /** Check the SV health status of the transmitting satellite
          * of a navigation message.
          * @param[in] ndp A pointer to the NavData to verify.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @return true if the health status of the satellite that
          *   transmitted ndp matches xmitHealth. */
      bool matchHealth(NavData *ndp, SVHealth xmitHealth);

         /** Update initialTime and finalTime according to a fit
          * interval or other timestamp.
          * For orbital elements with an actual fit interval, the
          * beginning and end of the fit interval are specified as
          * arguments.  For tabular orbital data (e.g. SP3), the time
          * stamp of the data is used for both parameters.
          * @param[in] begin The start of the fit interval of the
          *   orbital elements being processed.
          * @param[in] end The end of the fit interval of the
          *   orbital elements being processed.
          * @post initialTime and/or finalTime may be updated. */
      bool updateInitialFinal(const CommonTime& begin, const CommonTime& end);

         /// Internal storage of navigation data for User searches
      NavMessageMap data;
         /// Internal storage of navigation data for Nearest searches
      NavNearMessageMap nearestData;
         /** Store the time offset data separate from the other nav
          * data because searching is very different. */
      OffsetCvtMap offsetData;
         /// Store the earliest applicable orbit time here, by addNavData
      CommonTime initialTime;
         /// Store the latest applicable orbit time here, by addNavData
      CommonTime finalTime;
         /// Map subject satellite ID to time stamp pair (oldest,newest).
      std::map<SatID,std::pair<CommonTime,CommonTime> > firstLastMap;

         /// Grant access to MultiFormatNavDataFactory for various functions.
      friend class MultiFormatNavDataFactory;
   };

      //@}

}

#endif // GNSSTK_NAVDATAFACTORYWITHSTORE_HPP
