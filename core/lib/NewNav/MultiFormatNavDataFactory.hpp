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
#ifndef GNSSTK_MULTIFORMATNAVDATAFACTORY_HPP
#define GNSSTK_MULTIFORMATNAVDATAFACTORY_HPP

#include "gnsstk_export.h"
#include "NavDataFactoryWithStoreFile.hpp"
#include "NDFUniqIterator.hpp"

namespace gnsstk
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
       *   class due to the static data.
       * @warning Instantiating more than one of this class at any
       *   time will likely have unexpected results due to the shared
       *   (static) data stored internally.  DON'T DO IT.
       */
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

         /** Determine the earliest time for which this object can successfully
          * determine the Xvt for any object.
          * @note In the case that data from multiple systems is
          *   stored, the result may be in the UTC time system.
          * @return The initial time, or CommonTime::END_OF_TIME if no
          *   data is available. */
      CommonTime getInitialTime() const override;

         /** Determine the latest time for which this object can successfully
          * determine the Xvt for any object.
          * @note In the case that data from multiple systems is
          *   stored, the result may be in the UTC time system.
          * @return The initial time, or CommonTime::BEGINNING_OF_TIME if no
          *   data is available. */
      CommonTime getFinalTime() const override;

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
                                       const CommonTime& toTime)
         const override;

         /** Determine if a given message/satellite/signal is
          * available in the factory.
          * @param[in] nmid The message/satellite/signal to search for.
          * @param[in] fromTime The earliest time for which a matching
          *   message should be available.
          * @param[in] toTime The latest time for which a matching
          *   message should be available.
          * @return true if the given satellite/signal is has data in
          *   the given time span.
          * @note We specifically require the time range parameters to
          *   try to avoid making assumptions about the size of the
          *   data set (i.e. assuming the data is going to be a day's
          *   worth when it's actually several years).
          * @note This method iterates over the given time span until
          *   it finds a match.  As such, it is strongly recommended
          *   that you not use BEGINNING_OF_TIME or END_OF_TIME, as it
          *   takes several minutes to iterate over that time span. */
      bool isPresent(const NavMessageID& nmid,
                     const CommonTime& fromTime,
                     const CommonTime& toTime) override;

         /// Return a comma-separated list of formats supported by the factories
      std::string getFactoryFormats() const override;

         /// Return the number of nav messages in all factories.
      size_t size() const override;
         /// @copydoc NavDataFactoryWithStore::count(const NavMessageID&) const
      size_t count(const NavMessageID& nmid) const override;
         /// @copydoc NavDataFactoryWithStore::count(SatelliteSystem,NavMessageType) const
      size_t count(SatelliteSystem sys,
                   NavMessageType nmt = NavMessageType::Unknown)
         const override
      { return NavDataFactoryWithStore::count(sys,nmt); }
         /// @copydoc NavDataFactoryWithStore::count(const SatID&,NavMessageType) const
      size_t count(const SatID& satID,
                   NavMessageType nmt = NavMessageType::Unknown)
         const override
      { return NavDataFactoryWithStore::count(satID,nmt); }
         /// @copydoc NavDataFactoryWithStore::count(gnsstk::NavMessageType) const
      size_t count(NavMessageType nmt) const override
      { return NavDataFactoryWithStore::count(nmt); }
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
          * @warning You MUST include Health data in the type filter
          *   if you intend to look up data by health status other
          *   than "Any" (exceptions: if you're ONLY looking up orbit
          *   data that has self-contained health status).
          * @param[in] nmts The set of nav message types to be
          *   processed by the factories. */
      void setTypeFilter(const NavMessageTypeSet& nmts) override;

         /** Clear the type filters of each of the factories.  This
          * should be used prior to loading data, and prior to using
          * addTypeFilter(), if that API is going to be used instead
          * of setTypeFilter(). */
      void clearTypeFilter() override;

         /** Add a NavMessageType to be processed to each of the
          * factories.  This should be used prior to loading data and
          * as an alternate approach to setTypeFilter().
          * @param[in] nmt The NavMessageType to be processed on the
          *   next load. */
      void addTypeFilter(NavMessageType nmt) override;

         /** Method for loading data.  This will iterate over the
          * available factories, calling their load method until one
          * succeeds, since failure typically indicates an invalid
          * input.
          * @param[in] source The path of the file to load.
          * @return true on success, false if none of the available
          *   factories succeeded. */
      bool addDataSource(const std::string& source) override;

         /// @copydoc NavDataFactoryWithStoreFile::process(const std::string&,NavDataFactoryCallback&)
      bool process(const std::string& filename,
                   NavDataFactoryCallback& cb) override;

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
      void dump(std::ostream& s, DumpDetail dl) const;

         /** Get the instance of a given factory type, specified by
          * the template argument.  This allows you to get a specific
          * factory if for some reason you need to tweak its
          * settings. */
      template <class Fact>
      std::shared_ptr<Fact> getFactory();

         /** Set the configuration parameters for this and any child
          * NavDataFactory or PNBNavDataFactory objects.
          * @param[in] ctrl The configuration for the factory/factories.
          */
      void setControl(const FactoryControl& ctrl) override;

   protected:
         /** Known nav data factories, organized by signal to make
          * searches simpler and/or quicker.  Declared static so that
          * other libraries can transparently add factories. */
      static std::shared_ptr<NavDataFactoryMap> factories();
     
         /** Keep a cached copy of the shared_ptr to the static
          * NavDataFactoryMap so that windows doesn't destroy it before
          * destroying this. */
      std::shared_ptr<NavDataFactoryMap> myFactories;

   private:
         /** This method makes no sense in this context, because we
          * don't want to load, e.g. RINEX and SP3 into the same
          * NavMessageMap, because SP3's find method performs
          * interpolation. */
      bool loadIntoMap(const std::string& filename,
                       NavMessageMap& navMap,
                       NavNearMessageMap& navNearMap,
                       OffsetCvtMap& ofsMap) override
      { return false; }
   };


   template <class Fact>
   std::shared_ptr<Fact> MultiFormatNavDataFactory ::
   getFactory()
   {
      std::shared_ptr<Fact> rv;
      for (auto& fi : NDFUniqIterator<NavDataFactoryMap>(factories()))
      {
         rv = std::dynamic_pointer_cast<Fact>(fi.second);
         if (rv)
            return rv;
      }
      return rv;
   }

      //@}

}

#endif // GNSSTK_MULTIFORMATNAVDATAFACTORY_HPP
