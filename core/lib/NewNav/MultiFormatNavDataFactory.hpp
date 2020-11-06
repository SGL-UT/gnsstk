#ifndef GPSTK_MULTIFORMATNAVDATAFACTORY_HPP
#define GPSTK_MULTIFORMATNAVDATAFACTORY_HPP

#include "NavDataFactoryWithStoreFile.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Define a class that allows you to use a single factory
       * object that can read data from a variety of formats.  The
       * supported formats depend on what libraries you link against,
       * as the libraries themselves add factories to the supported
       * list at run-time.
       * This class shares a lot of similarities with NavLibrary, but
       * NavLibrary implements high-level routines such as getXvt(),
       * while this class is set up to automatically include support
       * for available factory types.
       * @note This class is intended to support all known factory
       *   types implemented in libraries linked by the application,
       *   which is why factories and addFactory() are declared
       *   static.  You still must instantiate this class in order to
       *   use it.  Other methods, such as setValidityFilter() and
       *   setTypeFilter() are not declared static as they need to be
       *   implemented using polymorphism so that this class behaves
       *   correctly when used in conjunction with NavLibrary.
       * @warning Overridden methods affect every instance of this
       *   class due to the static data. */
   class MultiFormatNavDataFactory : public NavDataFactoryWithStoreFile
   {
   public:
         /// Initialize supportedSignals from factories.
      MultiFormatNavDataFactory();

         /** Clear all associated factories so as to avoid surprises
          * if you ever instantiate more than one
          * MultiFormatNavDataFactory in a session. */
      virtual ~MultiFormatNavDataFactory();

         /** Search the store of each factory in factories to find the
          * navigation message that meets the specified criteria.  The
          * first successful match is returned.
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

         /// Return the number of nav messages in all factories.
      size_t size() const override;
         /// Return the number of distinct signals (ignoring PRN) in factories.
      size_t numSignals() const override;
         /// Return the number of distinct signals including PRN, in factories.
      size_t numSatellites() const override;

         /** Set all the contained factories' handling of valid and
          * invalid navigation data.  This should be called before any
          * find() calls.
          * @warning affects all factories in the static data.
          * @param[in] nvt The new nav data loading filter method. */
      void setValidityFilter(NavValidityType nvt) override;

         /** Indicate what nav message types the factories should be
          * loading.  This should be called before the factories
          * acquire any data.
          * @warning affects all factories in the static data.
          * @param[in] nmts The set of nav message types to be
          *   processed by the factories. */
      void setTypeFilter(const NavMessageTypeSet& nmts) override;

         /** Method for loading data.  This will iterate over the
          * available factories, calling their load method until one
          * succeeds, since failure typically indicates an invalid
          * input.
          * @param[in] source The path of the file to load.
          * @return true on success, false if none of the available
          *   factories succeeded. */
      bool addDataSource(const std::string& source) override;

         /** Add a new factory to the library.  The factory must be
          * derived from NavDataFactoryWithStoreFile but not a
          * MultiFormatNavDataFactory.
          * @param[in] fact The NavDataFactory object to add to the library.
          * @return false if fact is not a valid factory class.
          */
      static bool addFactory(NavDataFactoryPtr& fact);

         /** Print the contents of all factories in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, NavData::Detail dl) const;

   protected:
         /** Known nav data factories, organized by signal to make
          * searches simpler and/or quicker.  Declared static so that
          * other libraries can transparently add factories. */
      static NavDataFactoryMap factories;

   private:
         /** This method makes no sense in this context, because we
          * don't want to load, e.g. RINEX and SP3 into the same
          * NavMessageMap, because SP3's find method performs
          * interpolation. */
      bool loadIntoMap(const std::string& filename,
                       NavMessageMap& navMap) override
      { return false; }
   };

      //@}

}

#endif // GPSTK_MULTIFORMATNAVDATAFACTORY_HPP
