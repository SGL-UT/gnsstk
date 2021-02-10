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
          * @return true if an offset is available, false if not. */
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

         /// Return a comma-separated list of formats supported by the factories
      std::string getFactoryFormats() const override;

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
      static NavDataFactoryMap& factories();

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
