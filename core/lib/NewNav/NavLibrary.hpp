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
#ifndef GPSTK_NAVLIBRARY_HPP
#define GPSTK_NAVLIBRARY_HPP

#include "NavDataFactory.hpp"
#include "Xvt.hpp"
#include "SVHealth.hpp"

namespace gpstk
{
      /** @defgroup NavFactory Navigation Message Decoding and Finding
       * Classes for providing an generalized (GNSS-agnostic)
       * interface to navigation data.
       *
       * @note Please make an effort to keep this documentation in
       * order of difficulty/usefulness, from simplest to hardest/most
       * esoteric.
       *
       * @section NavFactoryGettingStarted Getting Started
       *
       * The NavFactory classes provide an interface for searching for
       * navigation message data, including ephemeris, almanac, health
       * and time offset data, where
       *   \li Ephemeris is the more precise orbital information where
       *       the subject and transmitting satellite are the same.
       *   \li Almanac is less precise orbit information where the
       *       subject and transmitting satellite are not necessarily
       *       the same.
       *   \li Health data indicates whether a given satellite/signal
       *       is in a usable state.
       *   \li Time offset data provides the necessary data for
       *       converting between time systems, e.g. GPS and UTC.
       *
       * The simplest portion of this interface is defined in the
       * NavLibrary class, which provides an interface for computing
       * the satellite XVT at a given time.  Refer to that class'
       * documentation for a simple example of its use.  The
       * NavLibrary class also provides an interface for retrieving
       * health status information and time offset information.
       *
       * @section NavFactoryHowTo NavFactory HOW-TO
       *
       * \li Specify what to search for...
       *   * When using NavLibrary::find() and related methods, you
       *     must specify a NavMessageID (NavSatelliteID for the
       *     related methods).  This tells find() what type of message
       *     to search for (for NavMessageID), which satellites you're
       *     interested in (NavSatelliteID) and which signals you're
       *     interested in (NavSignalID).
       *   * NavSatelliteID allows the use of wildcards when
       *     specifying the subject satellite and/or transmitting
       *     satellite.  This means you can search for almanacs
       *     transmitted by GPS PRN 1, for example, or almanac orbital
       *     elements describing the orbit of GPS PRN 1, or almanacs
       *     for GPS PRN 7 transmitted by GPS PRN 3.  This also
       *     applies to health and ephemeris data, but bear in mind
       *     that ephemeris data will have the same subject and
       *     transmit satellite.  In most cases, you'll want to
       *     specify the subject satellite and leave the transmitting
       *     satellite as a wildcard.  See SatID for more information.
       *   * NavSignalID also allows the use of wildcards for the
       *     CarrierBand, TrackingCode, and NavType, any of which can
       *     be set to the "Any" enumeration to be treated as a
       *     wildcard.  This allows you to do things like get
       *     ephemerides for GPS PRN 1 without caring if the signal
       *     was L1 C/A, L1 P/Y or L2 P/Y (by setting
       *     NavSignalID::carrier and NavSignalID::code to Any and
       *     NavSignalID::nav to NavType::GPSLNAV).  You can also even
       *     set NavSignalID::nav to NavType::Any if you don't care
       *     whether the orbit data came from LNAV or CNAV or CNAV2.
       *
       * \li Get satellite XVT/health/time offset from arbitrary input...
       *   * Use the NavLibrary::getXvt(), NavLibrary::getHealth() or
       *     NavLibrary::getOffset() method, as appropriate.  Refer to
       *     the NavLibrary class and example.
       *
       * \li Get Ephemeris/Almanac/Health at a given time...
       *   * Use the NavLibrary::find() method.  The results will
       *     include any system-specific data, if you know how to use
       *     it (see NavData).
       *
       * \li Only process/load data of a certain type...
       *   * Use the NavLibrary::setTypeFilter() method, or if using a
       *     NavDataFactory class directly, use
       *     NavDataFactory::setTypeFilter().
       *   * This allows you to limit which data types (almanac,
       *     ephemeris, health, time offset) are processed and
       *     potentially stored internally, improving performance and
       *     reducing resource utilization by ignoring data that is
       *     not needed for the task.
       *
       * \li Only process/load valid data...
       *   * Each ICD generally will specify valid ranges of data when
       *     applicable.  This can be used to determine whether a
       *     given navigation message can/should be used or not.  The
       *     NavLibrary::setValidityFilter() method or
       *     NavDataFactory::setValidityFilter() method can be used to
       *     tell the factory/factories to only process/load valid
       *     data, invalid data, or all data.  Like setTypeFilter(),
       *     this can improve performance and reduce resource
       *     utilization under the right circumstances.
       *
       * \li Only use data from healthy satellites...
       *   * The NavLibrary::getXvt(), NavLibrary::getHealth(),
       *     NavLibrary::getOffset(), NavLibrary::find() and
       *     NavDataFactory::find() methods all support the
       *     specification of the desired health status of the
       *     transmitting satellite (in the form of the SVHealth
       *     enumeration).  By default, the health of the transmitting
       *     satellite is ignored, which is the fastest option as it
       *     does not engender searches for or checking of health
       *     status.  Specifying a specific desired health status to
       *     getHealth() is not the most useful option, however it can
       *     give you the ability to find out when an unhealthy
       *     satellite was last marked healthy, for example.
       *
       * \li Only use/retrieve valid or invalid data...
       *   * Like the transmit satellite health, you can also specify
       *     a validity to match as described above.
       *
       * \li Only support a specific input format...
       *   * If you want the high-level NavLibrary::getXvt() etc.
       *     interface, use that, of course, in conjunction with the
       *     NavLibrary::addFactory() method to add the specific
       *     format factory for processing the input data.
       *   * If you don't need the high-level interface and just want
       *     to retrieve the NavData-derived objects, use the
       *     appropriate NavDataFactory::find() object/method.
       *
       * \li Get the health of the subject satellite...
       *   * For orbit data derived from OrbitDataKepler (exceptions:
       *     GLONASS, OrbitDataSP3), the health status of the subject
       *     satellite is in the "health" field, which can be examined
       *     when using the NavLibrary::find() or
       *     NavDataFactory::find() method.
       *   * For health data, the NavHealthData::getHealth() method
       *     can be used.  This will of course yield the health status
       *     of the subject satellite if available.
       *   * TimeOffsetData does not have a subject satellite by
       *     nature, but the transmitting satellite health can be
       *     filtered according to the instructions above.
       *   * If NavLibrary::getXvt() is used, the health status is
       *     stored in the health field of the Xvt object.
       *
       * \li Get the valid time range for orbital data...
       *   * At present, this can only be done by getting the orbital
       *     data using the NavLibrary::find() or
       *     NavDataFactory::find() methods.  The data is stored in
       *     the OrbitDataKepler::beginFit and OrbitDataKepler::endFit
       *     fields, which means it is not available for OrbitDataSP3.
       *
       * \li Get the satellite Xvt when I've already obtained orbit data...
       *   * The OrbitData::getXvt() method can be used to compute the Xvt.
       *   * Note that it is recommended to do a new find() call for
       *     each orbit computation, particularly if there's any
       *     possibility of using interpolated orbits, as in
       *     OrbitDataSP3.  This is your best bet at having an
       *     appropriate set of orbital data for a given time.
       *
       * \li Get the URA for a given data set...
       *   * The URA/SISA data is system-specific and thus stored in
       *     the leaf nodes of the NavData tree (e.g. GPSLNavEph).
       *     You will need to do a dynamic_cast to the appropriate
       *     type and then check, e.g. \code{.cpp}
       * NavDataPtr result;
       * navlib.find(nmid, when, result, SVHealth::Any, NavValidityType::Any,
       *             NavSearchOrder::User);
       * GPSLNavEph *eph = dynamic_cast<GPSLNavEph*>(result.get());
       * if (eph != nullptr)
       * {
       *    cout << "URA index = " << (unsigned)eph->uraIndex << endl;
       * }
       * \endcode
       *
       * \li Compute Xvt over a time range using a single data set...
       *   * As noted above, it is recommended to do a new find() call
       *     for each orbital computation, however there is at least
       *     one use case where this is valid, when you're attempting
       *     to use empirical data to determine fit intervals.  To do
       *     as described use the NavLibrary::find() or
       *     NavDataFactory::find() method to get your orbital data
       *     (ephemeris or almanac), then simply use the getXvt()
       *     method with different times.
       *
       * @todo GLONASS probably has health information in its
       * non-Keplerian ephemeris data, but SP3 does not.  The GLONASS
       * data have not been implemented yet in this library but at
       * some point a decision will need to be made.  It might make
       * sense to move the health field up to the OrbitData class and
       * simply set it to "Unknown" for SP3.
       *
       * @todo Determine if OrbitDataKepler::beginFit and
       * OrbitDataKepler::endFit need to be moved up to OrbitData.
       * This mostly (if not exclusively) affects GLONASS orbits and
       * OrbitDataSP3.  If so, NavDataLibraryWithStore needs to
       * reflect this change.
       *
       * @todo Determine what, if anything, should be done in
       * NavLibrary::getXvt() with beginFit/endFit.
       *
       * @todo Determine if a URA in meters should be added to
       * OrbitDataKepler or OrbitData.
       *
       * @section NavFactoryDesignOverview NavFactory Design Overview
       *
       * The NavFactory classes are designed with several goals in
       * mind (in no particular order):
       * \li Provide a simple interface for common nav data tasks.
       * \li Support multiple GNSSes using a common interface.
       * \li Support storage formats that are implemented in the gpstk.
       * \li Allow for dynamic run-time extension of supported formats.
       * \li Support raw nav data bit decoding as needed by input formats.
       * \li Allow for dynamic run-time extension of raw nav data decoding.
       *
       * The end result is a design that includes the following:
       * \li NavLibrary class which provides a high-level interface
       *     for getting satellite XVT, health and time system offset
       *     data.
       * \li A tree of classes with NavDataFactory as the base, which,
       *     given a request for navigation data, will create and
       *     return the requested data if possible.  This tree of
       *     classes is where support for various input formats (most
       *     prominently, files) is implemented.
       * \li A tree of classes with NavData as the base, which
       *     implements the storage for orbit data (OrbitData), health
       *     data (NavHealthData) and time offset data
       *     (TimeOffsetData).  The GNSS-specific implementations
       *     derive from these three classes to provide the code and
       *     data needed for that GNSS.
       * \li A tree of classes with PNBNavDataFactory as the base,
       *     which provides the ability to decode the raw navigation
       *     message bits into their engineering units counterparts
       *     that are derived from NavData.  In short, the
       *     PNBNavDataFactory classes take a PackedNavBits object as
       *     input and produce 0 or more NavData-derived objects as
       *     output.  These classes are typically used internally by
       *     NavDataFactory classes.
       *
       * @subsection NavFactoryExtension NavFactory Extendability Design
       *
       * The NavFactory classes are designed to be extensible, as
       * noted above.  This is done using two classes:
       * MultiFormatNavDataFactory and PNBMultiGNSSNavDataFactory.
       *
       * @subsubsection MultiFormatNavDataFactory MultiFormatNavDataFactory Design
       *
       * The MultiFormatNavDataFactory class is a child of the
       * NavDataFactoryWithStoreFile class that keeps track of other
       * NavDataFactoryWithStoreFile classes.  It works by "farming
       * out" requests to factories that are capable of handling them.
       * The determination of whether a factory is capable of handling
       * a given request is used by mapping each of the signals
       * supported by the factories to the factory itself, therefore
       * when a find request is made to MultiFormatNavDataFactory, it
       * looks up the requested signal in the map of factories and
       * tries matching factories until one succeeds (giving up if
       * none succeed).
       *
       * The MultiFormatNavDataFactory may be added as a single
       * factory to the NavLibrary.
       *
       * The reason for having this particular factory rather than
       * adding each individual NavDataFactory to a NavLibrary (which
       * is also still possible) is where the extensibility comes in.
       * The MultiFormatNavDataFactory::factories data member is
       * declared static and is initialized at run time, which not
       * only allows for dynamic configuration of factory support, but
       * also allows for other libraries to add support for factories
       * that do not exist in the gpstk.
       *
       * As an example, support for RINEX NAV and SP3 is implemented
       * in the core gpstk, but support for Yuma and SEM format files
       * is only present in the ext code.  While in this particular
       * case, using compiler directives would have worked to decide
       * whether to add support for Yuma and SEM, that wouldn't have
       * worked for external formats.  You may have proprietary
       * formats you wish to support, file or real-time.  These can be
       * added as described in \ref NavFactoryAdd.
       *
       * The end result of all of this is that if you have a tool that
       * is written to use NavLibrary with MultiFormatNavDataFactory,
       * as long as you're not digging deep into the detailed
       * specifics of the factory or data classes, all that is
       * required to add support for additional formats is to link the
       * library implementing the format to that executable.
       *
       * @note The fact that the factories data member is static means
       * that certain methods will appear to behave in strange ways.
       * Please read the warning messages in
       * MultiFormatNavDataFactory.
       *
       * @subsubsection PNBMultiGNSSNavDataFactory PNBMultiGNSSNavDataFactory Design
       *
       * The PNBMultiGNSSNavDataFactory class design is much like the
       * MultiFormatNavDataFactory class.  It also has a static data
       * member PNBMultiGNSSNavDataFactory::factories where classes
       * derived from PNBNavDataFactory can be added to support the
       * decoding of additional signals.  This is also initialized by
       * libraries at run-time.
       *
       * There is no NavLibrary equivalent for PNBNavDataFactory.
       * Instead, a NavDataFactory-derived class that is expected to
       * process the raw navigation messages from multiple systems
       * would use an instance of this class to decode the raw
       * navigation messages in the form of PackedNavBits.  Data goes
       * through the translation process using
       * PNBNavDataFactory::addData(), the results of which can be
       * stored or used as appropriate.
       *
       * There are currently no factories in ext that are being added
       * this way, however instructions for doing so can be found in
       * \ref PNBNavFactoryAdd.
       *
       * @note The fact that the factories data member is static means
       * that certain methods will appear to behave in strange ways.
       * Please read the warning messages in
       * PNBMultiGNSSNavDataFactory.
       *
       * @section NavFactoryAdd Adding Custom NavData Factories
       * 
       * If you wish to add support for additional file formats to
       * MultiFormatNavDataFactory, you will need to do the following:
       *
       *   1. Define a class whose constructor instantiates the
       *      factory and adds it using
       *      MultiFormatNavDataFactory::addFactory().  This
       *      constructor should check a static boolean data member to
       *      make sure it only does this once.
       *   2. Define a static instance of said class so that the
       *      constructor is called at run time, thereby adding the
       *      custom factory to MultiFormatNavDataFactory whenever the
       *      code is used.
       *
       * As an example of how this is done, refer to ExtFactoryInitializer.cpp.
       *
       * @warning It is possible that even with this implementation,
       * the static initialization may not happen properly, a problem
       * we have seen under Windows at the very least.  It is strongly
       * recommended that if you chose to implement an addition to the
       * MultiFormatNavDataFactory you also implement a test in your
       * code to make sure it actually is adding the factory properly.
       *
       * @section PNBNavFactoryAdd Adding Custom PNBNavData Factories
       *
       * Adding support for additional nav message data decoders can
       * be added to PNBMultiGNSSNavDataFactory.  You will need to do
       * the following:
       *
       *   1. Define a class whose constructor instantiates the
       *      factory and adds it using
       *      PNBMultiGNSSNavDataFactory::addFactory().  This
       *      constructor should check a static boolean data member to
       *      make sure it only does this once.
       *   2. Define a static instance of said class so that the
       *      constructor is called at run time, thereby adding the
       *      custom factory to MultiFormatNavDataFactory whenever the
       *      code is used.
       *
       * There are no examples of this outside the gpstk core, however
       * one may look at core/lib/NewNav/NavStatic.cpp to see how this
       * is done for the core PNBNavDataFactory classes.  There should
       * be little difference for outside code.
       *
       * @warning It is possible that even with this implementation,
       * the static initialization may not happen properly, a problem
       * we have seen under Windows at the very least.  It is strongly
       * recommended that if you chose to implement an addition to the
       * PNBMultiGNSSNavDataFactory you also implement a test in your
       * code to make sure it actually is adding the factory properly.
       */

      /// @ingroup NavFactory
      //@{

      /** Provide a high-level interface for finding navigation data.
       * It is recommended that you use MultiFormatNavDataFactory when
       * the intent of the program is to process navigation data from
       * files, as this provides support for a wide variety of input
       * file formats and GNSSes with no additional effort on your
       * part.
       *
       * Simplified example:
       * \code
       * NavLibrary navLib;
       * NavDataFactoryPtr ndfp(std::make_shared<MultiFormatNavDataFactory>());
       * navLib.addFactory(ndfp);
       * if (!ndfp->addDataSource(filename))
       *    die("could not load " + filename);
       * NavSatelliteID sat(...);
       * CommonTime when(...);
       * Xvt xvt;
       * if (navLib.getXvt(sat,when,xvt))
       *    doSomething(xvt);
       * \endcode
       */
   class NavLibrary
   {
   public:
         /** Get the position and velocity of a satellite at a specific time.
          * @param[in] sat Satellite to get the position/velocity for.
          * @param[in] when The time that the position should be computed for.
          * @param[out] xvt The computed position and velocity at when.
          * @param[in] useAlm If true, search for and use almanac
          *   orbital elements.  If false, search for and use
          *   ephemeris data instead.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful, false if no nav data was found
          *   to compute the Xvt. */
      bool getXvt(const NavSatelliteID& sat, const CommonTime& when,
                  Xvt& xvt, bool useAlm, SVHealth xmitHealth = SVHealth::Any,
                  NavValidityType valid = NavValidityType::ValidOnly,
                  NavSearchOrder order = NavSearchOrder::User);

         /** Get the position and velocity of a satellite at a
          * specific time.  This implementation differs from the other
          * getXvt method in that it will first attempt to find a
          * matching ephemeris, and if that fails, it will attempt to
          * search for a matching almanac, as opposed to one or the other.
          * @param[in] sat Satellite to get the position/velocity for.
          * @param[in] when The time that the position should be computed for.
          * @param[out] xvt The computed position and velocity at when.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful, false if no nav data was found
          *   to compute the Xvt. */
      bool getXvt(const NavSatelliteID& sat, const CommonTime& when,
                  Xvt& xvt, SVHealth xmitHealth = SVHealth::Any,
                  NavValidityType valid = NavValidityType::ValidOnly,
                  NavSearchOrder order = NavSearchOrder::User);

         /** Get the health status of a satellite at a specific time.
          * @param[in] sat Satellite to get the health status for.
          * @param[in] when The time that the health should be retrieved.
          * @param[out] health The health status at when.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful, false if no nav data was found
          *   containing health status. */
      bool getHealth(const NavSatelliteID& sat, const CommonTime& when,
                     SVHealth& health, SVHealth xmitHealth = SVHealth::Any,
                     NavValidityType valid = NavValidityType::ValidOnly,
                     NavSearchOrder order = NavSearchOrder::User);

         /** Get the offset as a NavDataPtr that refers to a
          * TimeOffset object, to apply to times when converting them
          * from fromSys to toSys.
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
                     NavValidityType valid = NavValidityType::ValidOnly);

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
                     const CommonTime& when, double& offset,
                     SVHealth xmitHealth = SVHealth::Any,
                     NavValidityType valid = NavValidityType::ValidOnly);

         /** Search factories to find the navigation message that meets
          * the specified criteria.
          * @note returns the first successful match from factories.
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
                NavSearchOrder order);

         /** Set the factories' handling of valid and invalid
          * navigation data.  This should be called before any find()
          * calls.
          * @param[in] nvt The new nav data loading filter method. */
      void setValidityFilter(NavValidityType nvt);

         /** Indicate what nav message types the factories should be
          * loading.  This should be called before the factories
          * acquire any data.
          * @param[in] nmts The set of nav message types to be
          *   processed by the factories. */
      void setTypeFilter(const NavMessageTypeSet& nmts);

         /** Add a new factory to the library.
          * @param[in] fact The NavDataFactory object to add to the library.
          */
      void addFactory(NavDataFactoryPtr& fact);

         /** Print the contents of all factories in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, NavData::Detail dl) const;

         /** Remove all data from the library's factories in the time
          * span [fromTime,toTime).
          * @param[in] fromTime The earliest time to be removed.
          * @param[in] toTime The earliest time that will NOT be removed.
          */
      void edit(const CommonTime& fromTime, const CommonTime& toTime);

         /** Remove data for a specific satellite signal from the
          * library's factories in the time span [fromTime,toTime).
          * @param[in] fromTime The earliest time to be removed.
          * @param[in] toTime The earliest time that will NOT be removed.
          * @param[in] satID The complete signal specification for the
          *   data to be removed (subject satellite, transmit
          *   satellite, system, carrier, code, nav).
          */
      void edit(const CommonTime& fromTime, const CommonTime& toTime,
                const NavSatelliteID& satID);

         /** Remove data for all satellites matching a specific signal
          * from the library's factories in the time span
          * [fromTime,toTime).
          * @param[in] fromTime The earliest time to be removed.
          * @param[in] toTime The earliest time that will NOT be removed.
          * @param[in] signal The signal for the data to be removed
          *   (system, carrier, code, nav).
          */
      void edit(const CommonTime& fromTime, const CommonTime& toTime,
                const NavSignalID& signal);

         /// Remove all data from the library's factories.
      virtual void clear();

         /** Determine the earliest time for which this object can successfully 
          * determine the Xvt for any object.
          * @return The initial time, or CommonTime::END_OF_TIME if no
          *   data is available. */
      CommonTime getInitialTime() const;

         /** Determine the latest time for which this object can successfully 
          * determine the Xvt for any object.
          * @return The initial time, or CommonTime::BEGINNING_OF_TIME if no
          *   data is available. */
      CommonTime getFinalTime() const;

         /// Return a comma-separated list of formats supported by the factories
      std::string getFactoryFormats() const;

   protected:
         /** Known nav data factories, organized by signal to make
          * searches simpler and/or quicker. */
      NavDataFactoryMap factories;
   };

      //@}

}

#endif // GPSTK_NAVLIBRARY_HPP
