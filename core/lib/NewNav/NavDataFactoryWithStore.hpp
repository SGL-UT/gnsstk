#ifndef GPSTK_NAVDATAFACTORYWITHSTORE_HPP
#define GPSTK_NAVDATAFACTORYWITHSTORE_HPP

#include "NavDataFactory.hpp"
#include "TimeOffsetData.hpp"

namespace gpstk
{
      /** Define methods and data structures for internally storing
       * navigation message data internally as read from some
       * source. */
   class NavDataFactoryWithStore : public NavDataFactory
   {
   public:
         /** Search the store to find the navigation message that meets
          * the specified criteria.
          * @param[in] nmid Specify the message type, satellite and
          *   codes to match.
          * @param[in] when The time of interest to search for data.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[out] navData The resulting navigation message.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful.  If false, navData will be untouched. */
      bool find(const NavMessageID& nmid, const CommonTime& when,
                NavDataPtr& navData, SVHealth xmitHealth, NavValidityType valid,
                NavSearchOrder order) override;

         /** Get the offset, in seconds, to apply to times when
          * converting them from fromSys to toSys.
          * @param[in] fromSys The time system to convert from.
          * @param[in] toSys The time system to convert to.
          * @param[in] when The time being converted, usually in the
          *   time system appropriate for a given nav message source.
          *   The details of what time system this should be in and
          *   any other restrictions will be documented in each leaf
          *   class, e.g. GPSLNavTimeOffset.
          * @param[out] offset The offset when converting fromSys->toSys.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if an offset is available, false if not. */
      bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                     const CommonTime& when, double& offset,
                     SVHealth xmitHealth = SVHealth::Any,
                     NavValidityType valid = NavValidityType::ValidOnly,
                     NavSearchOrder order = NavSearchOrder::User) override;

         /** Remove all data from the internal storage in the time
          * span [fromTime,toTime).
          * @param[in] fromTime The earliest time to be removed.
          * @param[in] toTime The earliest time that will NOT be removed.
          */
      virtual void edit(const CommonTime& fromTime, const CommonTime& toTime);

         /** Remove data for a specific satellite signal from the
          * internal storage in the time span [fromTime,toTime).
          * @param[in] fromTime The earliest time to be removed.
          * @param[in] toTime The earliest time that will NOT be removed.
          * @param[in] satID The complete signal specification for the
          *   data to be removed (subject satellite, transmit
          *   satellite, system, carrier, code, nav).
          */
      virtual void edit(const CommonTime& fromTime, const CommonTime& toTime,
                        const NavSatelliteID& satID);

         /** Remove data for all satellites matching a specific signal
          * from the internal storage in the time span
          * [fromTime,toTime).
          * @param[in] fromTime The earliest time to be removed.
          * @param[in] toTime The earliest time that will NOT be removed.
          * @param[in] signal The signal for the data to be removed
          *   (system, carrier, code, nav).
          */
      virtual void edit(const CommonTime& fromTime, const CommonTime& toTime,
                        const NavSignalID& signal);

         /// Remove all data from the internal store.
      virtual void clear();

         /** Add a nav message to the internal store (data).
          * @param[in] nd The nav data to add.
          * @return true if successful. */
      bool addNavData(const NavDataPtr& nd);

         /// Return the number of nav messages in data.
      virtual size_t size() const;
         /// Return the number of distinct signals (ignoring PRN) in the data.
      virtual size_t numSignals() const;
         /// Return the number of distinct signals including PRN, in the data.
      virtual size_t numSatellites() const;
         /** Print the contents of this store in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
       void dump(std::ostream& s, NavData::Detail dl) const override;

   protected:
         /** Performs an appropriate validity check based on the
          * desired validity.
          * @param[in] ti A container iterator pointing to the nav
          *   data to check.
          * @param[in] nm The NavMap that ti is iterating over.
          * @param[in] valid The desired validity for navigation data.
          * @return true if the validity of the nav data pointed to by
          *   ti matches the requested validty described by valid.
          */
      static bool validityCheck(const NavMap::iterator& ti,
                                NavMap& nm,
                                NavValidityType valid);

         /// Internal storage of navigation data.
      NavMessageMap data;
         // Time offset information is organized differently due to the use case
         /** Map that will contain all TimeOffsetData objects with the
          * same conversion pair broadcast at a given time. */
      using OffsetMap = std::map<NavSatelliteID, NavDataPtr>;
         /** Map from the timeStamp of a TimeOffsetData object to the
          * collection of TimeOffsetData objects. */
      using OffsetEpochMap = std::map<CommonTime, OffsetMap>;
         /// Map from the time system conversion pair to the conversion objects.
      using OffsetCvtMap = std::map<TimeOffsetData::TimeCvtKey, OffsetEpochMap>;
         /** Store the time offset data separate from the other nav
          * data because searching is very different. */
      OffsetCvtMap offsetData;
         /// Grant access to MultiFormatNavDataFactory for various functions.
      friend class MultiFormatNavDataFactory;
   };
}

#endif // GPSTK_NAVDATAFACTORYWITHSTORE_HPP
