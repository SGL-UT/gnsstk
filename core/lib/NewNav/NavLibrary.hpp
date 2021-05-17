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
       */

      /** \page APIguide
       * - \subpage navlibrary Navigation Message Library
       * \page navlibrary Navigation Message Library
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
       * @section NavFactoryEntryPoints Entry Points
       *
       * The NavLibrary class, which provides the highest-level
       * interface, has four primary entry points for looking up
       * navigation message data:
       *   \li NavLibrary::getXvt() to get a satellite's position
       *       (referenced to antenna phase center in most cases, see
       *       below), velocity and clock offset.
       *   \li NavLibrary::getHealth() to get a satellite's health status.
       *   \li NavLibrary::getOffset() to get TimeSystem conversion information.
       *   \li NavLibrary::find() to get arbitrary navigation message
       *       data (excluding TimeOffset information).
       *
       * Regarding NavLibrary::getXvt() and antenna phase center
       * (APC), one exception to this rule is when using SP3 files as
       * input.  The data in an SP3 file can be referenced to the
       * center-of-mass, or to the antenna phase center.  There is no
       * programattic way to distinguish between the two, so it's
       * entirely up to the user in this case to know what they're
       * doing.
       *
       * @section NavFactoryUseCase Use Cases
       *
       * The table below describes what search parameters are expected
       * to be specified vs. may be wildcards ("Any") in that
       * situation.  In each case, the use of wildcards is given as an
       * option, i.e. specific values may still be specified if
       * desired.
       *
       * The following generalized use cases are defined:
       * \dictionary
       * \dicterm{Low-Precision}
       * \dicdef{Only a rough estimate of the satellite position is
       *         needed.  Typically used for applications where only
       *         azimuth and elevation measurements are needed to
       *         precision of maybe a 10th of a degree.  Almanac or
       *         Ephemeris data may be used in such cases.  Examples:
       *         visibility plots or any other plot against elevation
       *         and/or azimuth is used.}
       * \dicterm{High-Precision}
       * \dicdef{When a more precise satellite position is required.
       *         In this case you would use ephemeris data, but not
       *         almanac data.  Examples: observed range deviation
       *         (ORD) analysis, inter-signal correction estimation.}
       * \dicterm{Known-Source}
       * \dicdef{This situation is typically when you're analyzing
       *    signal health and you need to know the exact signal a
       *    given navigation message came from.  You might be looking
       *    at any given nav message type in this case, even almanac
       *    data.}
       * \enddictionary
       *
       * Parameter          | Low-Precision | High-Precision | Known-Source
       * ------------------ | ------------- | -------------- | ------------
       * sat.sat.id         | specified     | specified      | specified
       * sat.sat.system     | specified     | specified      | specified
       * sat.xmitSat.id     | wild          | wild           | specified
       * sat.xmitSat.system | wild          | wild           | specified
       * sat.system         | specified     | specified      | specified
       * sat.obs.type       | wild          | wild           | wild
       * sat.obs.band       | wild          | specified      | specified
       * sat.obs.code       | wild          | specified      | specified
       * sat.obs.xmitAnt    | wild          | specified      | specified
       * sat.obs.freqOffs   | GLONASS       | GLONASS        | GLONASS
       * sat.obs.mcode      | wild          | wild           | wild
       * sat.nav            | wild          | specified      | specified
       *
       * \note nmid is used the same as "sat" in the above table as it
       * extends the class with the additional messageType parameter.
       * This parameter is only used in the find() method.  The
       * nmid.messageType field must always be specified as there is
       * no wildcard value.
       *
       * \note sat.obs.type may be specified to either "Any" or
       * "NavMsg".  No other values will yield any results.
       *
       * \note sat.obs.freqOffset should be specified when supporting
       * GLONASS (Is this always true, even for low-precision?)
       *
       * @subsection NavFactoryExampleCode Code Examples
       *
       * In most cases, you'll want to use NavLibrary in conjunction
       * with MultiFormatNavDataFactory to access navigation messages.
       * This provides a relatively simple interface while
       * simultaneously supporting multiple input formats, with the
       * expansion of that set of supported input formats happening
       * without the need for additional code changes.
       *
       * The process for using NavLibrary in this fashion involves the
       * following steps:
       * -# Construct a NavLibrary object.
       * -# Construct a NavDataFactory object (typically
       *    MultiFormatNavDataFactory).
       * -# Add the NavDataFactory to the NavLibrary using addFactory().
       * -# Add input data (files) to the NavDataFactory using addDataSource().
       * -# Search the NavLibrary.
       *
       * @subsubsection NavDataFactoryLowPrecEx Low-Precision Example
       *
       * Here is an example code snippet of how one might use
       * NavLibrary for a low-precision use case.  The getXvt() call
       * in this example allows derivation of XVT data from either an
       * almanac or ephemeris message:
       *
       * \todo Update the ObsID stuff to match the decision about
       * using freqOffset in the table above.
       *
       * \code
       *    // Parameters used to construct sat will have been
       *    // specified on the command-line or via some other means.
       * gpstk::CommonTime when;
       * unsigned long subjID;
       * gpstk::SatelliteSystem sys;
       * int freqOffs;
       * bool freqOffsSpec; // true if specified
       *    // Output of search
       * gpstk::Xvt xvt;
       *    // Construct a NavLibrary object.
       * gpstk::NavLibrary navLib;
       *    // Construct a NavDataFactory object
       * gpstk::NavDataFactoryPtr ndfp(
       *    std::make_shared<gpstk::MultiFormatNavDataFactory>());
       *    // Add the NavDataFactory to the NavLibrary
       * navLib.addFactory(ndfp);
       *    // Add input data (files) to the NavDataFactory
       * if (!ndfp->addDataSource(inputFileName))
       * {
       *    cerr << "Unable to load \"" << inputFileName << "\"" << endl;
       *    return false;
       * }
       *    // Search the NavLibrary
       * gpstk::NavSatelliteID sat(subjID, system, gpstk::CarrierBand::Any,
       *                           gpstk::TrackingCode::Any,
       *                           gpstk::XmitAnt::Any, freqOffs,
       *                           !freqOffsSpec);
       * if (!navLib.getXvt(sat, when, xvt))
       * {
       *    cerr << "Unable to find XVT for " << sat << " @ " << when << endl;
       *    return false;
       * }
       * \endcode
       *
       * @subsubsection NavDataFactoryHighPrecEx High-Precision Example
       *
       * Here is a reduced example code snippet of how one might use
       * NavLibrary for a high-precision use case.  In this example,
       * the getXvt method that enforces the use of either almanac or
       * ephemeris is used, and is forced to use only ephemeris data.
       *
       * \code
       *    // Parameters used to construct sat will have been
       *    // specified on the command-line or via some other means.
       * gpstk::CommonTime when;
       * unsigned long subjID;
       * gpstk::SatelliteSystem sys;
       * gpstk::CarrierBand band;
       * gpstk::TrackingCode code;
       * gpstk::XmitAnt xmitAnt;
       * gpstk::NavType nav; // e.g. GPSLNAV
       * int freqOffs;
       * bool freqOffsSpec; // true if specified
       *    // Output of search
       * gpstk::Xvt xvt;
       *    // Construct a NavLibrary object.
       * gpstk::NavLibrary navLib;
       *    // Construct a NavDataFactory object
       * gpstk::NavDataFactoryPtr ndfp(
       *    std::make_shared<gpstk::MultiFormatNavDataFactory>());
       *    // Add the NavDataFactory to the NavLibrary
       * navLib.addFactory(ndfp);
       *    // Add input data (files) to the NavDataFactory
       * if (!ndfp->addDataSource(inputFileName))
       * {
       *    cerr << "Unable to load \"" << inputFileName << "\"" << endl;
       *    return false;
       * }
       *    // Search the NavLibrary
       * gpstk::NavSatelliteID sat(subjID, system, band, code, xmitAnt,
       *                           freqOffs, !freqOffsSpec, nav);
       * if (!navLib.getXvt(sat, when, xvt, false))
       * {
       *    cerr << "Unable to find XVT for " << sat << " @ " << when << endl;
       *    return false;
       * }
       * \endcode
       *
       * @subsubsection NavDataFactoryKnownSrcEx Known-Source Example
       *
       * Here is a reduced example code snippet of how one might use
       * NavLibrary for a known-source use case.  This is much the
       * same as the high-precision case, but with more details
       * specified for matching.
       *
       * \code
       *    // Parameters used to construct sat will have been
       *    // specified on the command-line or via some other means.
       * gpstk::CommonTime when;
       * unsigned long subjID;
       * gpstk::SatelliteSystem subjSys;
       * unsigned long xmitID;
       * gpstk::SatelliteSystem xmitSys;
       * gpstk::CarrierBand band;
       * gpstk::TrackingCode code;
       * gpstk::XmitAnt xmitAnt;
       * gpstk::NavType nav; // e.g. GPSLNAV
       * int freqOffs;
       * bool freqOffsSpec; // true if specified
       *    // Output of search
       * gpstk::Xvt xvt;
       *    // Construct a NavLibrary object.
       * gpstk::NavLibrary navLib;
       *    // Construct a NavDataFactory object
       * gpstk::NavDataFactoryPtr ndfp(
       *    std::make_shared<gpstk::MultiFormatNavDataFactory>());
       *    // Add the NavDataFactory to the NavLibrary
       * navLib.addFactory(ndfp);
       *    // Add input data (files) to the NavDataFactory
       * if (!ndfp->addDataSource(inputFileName))
       * {
       *    cerr << "Unable to load \"" << inputFileName << "\"" << endl;
       *    return false;
       * }
       *    // Search the NavLibrary
       * gpstk::ObsID oid;
       * if (freqOffsSpec)
       * {
       *    oid = gpstk::ObsID(gpstk::ObservationType::NavMsg, band, code,
       *                       freqOffs, xmitAnt);
       * }
       * else
       * {
       *    oid = gpstk::ObsID(gpstk::ObservationType::NavMsg, band, code,
       *                       xmitAnt);
       * }
       * gpstk::SatID subjSat(subjID, subjSys);
       * gpstk::SatID xmitSat(xmitID, xmitSys);
       * gpstk::NavSatelliteID sat(subjSat, xmitSat, oid, nav);
       * if (!navLib.getXvt(sat, when, xvt, false))
       * {
       *    cerr << "Unable to find XVT for " << sat << " @ " << when << endl;
       *    return false;
       * }
       * \endcode
       *
       * @section NavFactorySearchParams Search Parameters
       *
       * The NavLibrary entry points have a common set of parameters
       * that are specified when searching for data.  With the
       * exception of the find() method, defaults are used for these
       * parameters that were deemed the most likely values across
       * typical use cases and these are typically safe values to use
       * in a given situation.  Changing from the defaults is only
       * likely in the event of implementing an atypical use case.
       *
       * The common parameters are:
       * \dictionary
       * \dicterm{sat/nmid}
       * \dicdef{The full signal/message specification to search for.}
       * \dicterm{when}
       * \dicdef{The time you're interested in getting the health, time
       *         offset, Xvt, etc.}
       * \dicterm{xmitHealth}
       * \dicdef{Allows you to specify the desired health state of
       *         the satellite that transmitted a given nav message
       *         at the time of interest.  DEFAULT=Any}
       * \dicterm{valid}
       * \dicdef{Allows you to specify whether you want navigation
       *         messages that passed or failed validity checks.
       *         DEFAULT=ValidOnly}
       * \dicterm{order}
       * \dicdef{Allows you to specify whether you want the data that
       *         would have appeared in a near-real-time situation or
       *         if you want the data with the closest time stamp.
       *         DEFAULT=User}
       * \enddictionary
       *
       * Most of these parameters have a wildcard (aka "don't care",
       * aka "Any") value, meaning that any value for that parameter
       * will match.  The \b when and \b order parameters are
       * exceptions to this - they must always be specified as a
       * fixed, single value.
       *
       * @subsection NavFactorySatParam Parameter sat/nmid
       *
       * The \b sat and \b nmid parameters specify a great deal of
       * detail about the data you're interested in, and this is where
       * most of the use-case-specific tweaking will be done.
       *
       * The \b sat parameter is a NavSatelliteID object, while \b
       * nmid is a NavMessageID object.  NavMessageID inherits from
       * NavSatelliteID (click for class/inheritance diagram), and
       * adds the messageType parameter.
       *
       * The classes contain the following data:
       *
       * \dictionary
       * \dicterm{messageType}
       * \dicdef{(nmid/NavMessageID only) Specifies what type of navigation
       *         message is being stored or searched for,
       *         e.g. NavMessageType::Almanac,
       *         NavMessageType::Ephemeris, etc.}
       * \dicterm{sat}
       * \dicdef{The subject satellite ID (SatID).  This is the
       *         satellite to which the data applies.  This may be
       *         different from \b xmitSat, and typically is in the
       *         case of almanac data.  This is the satellite whose
       *         health or position you want.}
       * \dicterm{xmitSat}
       * \dicdef{The ID of the satellite that transmitted the
       *         navigation message.  In most cases is the same as
       *         sat, but may be different for almanac data.  Can be a
       *         wildcard value if, for example, you want almanac data
       *         but don't care what the transmitting satellite was.
       *         Ephemerides will have the same \b sat and \b xmitSat
       *         values and can be specified this way when searching.}
       * \dicterm{system}
       * \dicdef{This specifies the SatelliteSystem whose data is
       *         identified or being searched for.  This is distinct
       *         from the system in \b sat and \b xmitSat for two
       *         reasons, one is that some systems have been known to
       *         broadcast data about other systems (QZSS test
       *         broadcasting GPS navigation messages for example).
       *         Another is that the NavSignalID class that contains
       *         the information does not contain the satellite data,
       *         but is in some cases used independently of the
       *         satellite ID.}
       * \dicterm{obs}
       * \dicdef{This specifies the CarrierBand and TrackingCode and
       *         other relevant information describing a signal.
       *         These can be set to wildcard values as well when
       *         searching.}
       * \dicdef{nav}
       * \dicdef{This specifies the navigation message structure from
       *         which the data was derived, e.g. gpstk::NavType::GPSLNAV.
       *         This too can be specified as a wildcard using
       *         gpstk::NavType::Any.}
       * \enddictionary
       *
       * @todo try to fix the NavType links above.
       *
       * @subsubsection NavDataFactorySatID Details on SatID
       *
       * SatID data is used in two ways in the NavSatelliteID class.
       * One is to identify the transmitting satellite, and the other
       * is to identify the subject satellite.  These are distinct for
       * the case of almanac data, where each satellite in a given
       * constellation broadcasts low precision orbital elements
       * ("almanacs") for every satellite in the constellation.
       *
       * In most situations, the transmit satellite may be set to
       * wildcard values in the search parameters.
       *
       * The satellite is identified using a combination of the
       * satellite system and the system-specific identifier (e.g. PRN
       * for GPS).
       *
       * A SatID may be made into a wildcard by one of two ways,
       * either by calling the SatID::makeWild() method, or by setting
       * SatID::wildId and/or SatID::wildSys individually (the former
       * is recommended if the SatID is meant to match anything, as it
       * should be kept up-to-date with any internal data changes).
       *
       * @subsubsection NavDataFactoryObsID Details on ObsID
       *
       * The ObsID class identifies an observation of a signal from a
       * satellite.  This includes the observation type (in this case,
       * always navigation messages), the carrier band, tracking code,
       * transmitting antenna, and, in the case of GLONASS, the
       * frequency offset for the FDMA constellation.  Depending on
       * your use case (see \ref NavFactoryUseCase above), you may
       * wish to specify this data or not.
       *
       * Bit fields for M-Code data are also present in the ObsID
       * class for matching, but use cases for anything but wildcard
       * matches will be extremely rare.
       *
       * @subsection NavFactoryWhenParam Parameter when
       *
       * The \b when parameter specifies a time of interest.  There
       * aren't too many restrictions on this, except that the time
       * system should match the satellite system of interest (or time
       * system, when getting time offset information), or otherwise
       * be set to TimeSystem::Any.
       *
       * @subsection NavFactoryXmitHealthParam Parameter xmitHealth
       *
       * The \b xmitHealth parameter allows you to specify whether you
       * want your data to come from a satellite that is healthy, or
       * unhealthy, or if you don't care.
       *
       * If a default value is set in the method prototype at all, the
       * default is SVHealth::Any, which means you don't care whether
       * the transmitting satellite is healthy or not.
       *
       * If you only want data transmitted by healthy satellites, then
       * specify SVHealth::Healthy.
       *
       * If for some reason you only want data from unhealthy
       * satellites, then specify SVHealth::Unhealthy.
       *
       * One last option for health is SVHealth::Degraded, however
       * this is a very limited use case as currently it is only
       * utilized by the Galileo implementation.
       *
       * @subsection NavFactoryValidParam Parameter valid
       *
       * The \b valid parameter allows you to specify whether or not
       * you want to do validity checks on the navigation data before
       * using it.  When methods have a default for this parameter, it
       * is to perform validity checks.
       *
       * The validity checks are performed was the data is added to
       * the NavDataFactory.  The validity checks are all specific to
       * each navigation message, and usually consists of range checks
       * on decoded values.
       *
       * Other values are NavValidityType::InvalidOnly, which runs
       * validity checks and only accepts data that fails.  Finally is
       * the ubiquitous NavValidityType::Any value which skips the
       * validity checks altogether and doesn't reject any data.
       *
       * @subsection NavFactoryOrderParam Parameter order
       *
       * The \b order parameter has two possible values,
       * NavSearchOrder::User, which most closely resembles the
       * behavior a user would see when using a receiver in near
       * real-time, and NavSearchOrder::Nearest which looks for data
       * with the timestamp closest to \b when, forward or backward in
       * time.
       *
       * Most use cases will want to use NavSearchOrder::User, and
       * this is the default when one is specified.
       * NavSearchOrder::Nearest is only used in vary specific cases
       * that you probably already know if you're going to use it at
       * all.
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
       * \li Get ionospheric correction data...
       *   * Sadly, this is not available in an abstract fashion at this
       *     time, and if this data is needed, one will have to
       *     process the system-specific data.
       *
       * \li Get satellite XVT/health/time offset (aka clock
       *     correction) from arbitrary input...
       *   * Use the NavLibrary::getXvt(), NavLibrary::getHealth() or
       *     NavLibrary::getOffset() method, as appropriate.  Refer to
       *     the NavLibrary class and examples in \ref NavFactoryExampleCode.
       *   * If only the clock correction is desired, you may still want to use
       *     getXvt() but only look at the clock bias/clock drift data.
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
          * @param[out] healthOut The health status at when.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful, false if no nav data was found
          *   containing health status. */
      bool getHealth(const NavSatelliteID& sat, const CommonTime& when,
                     SVHealth& healthOut, SVHealth xmitHealth = SVHealth::Any,
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
          * @param[out] navOut The offset when converting fromSys->toSys.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @return true if an offset is available, false if not. */
      bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                     const CommonTime& when, NavDataPtr& navOut,
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
          * @param[out] navOut The resulting navigation message.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful.  If false, navData will be untouched. */
      bool find(const NavMessageID& nmid, const CommonTime& when,
                NavDataPtr& navOut, SVHealth xmitHealth, NavValidityType valid,
                NavSearchOrder order);

         /** Set the factories' handling of valid and invalid
          * navigation data.  This should be called before any find()
          * calls.
          * @param[in] nvt The new nav data loading filter method. */
      void setValidityFilter(NavValidityType nvt);

         /** Indicate what nav message types the factories should be
          * loading.  This should be called before the factories
          * acquire any data.
          * @warning In order to properly support SP3, if you want to
          *   include Ephemeris data, you should also include Clock
          *   data in the filter.
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
      void dump(std::ostream& s, DumpDetail dl) const;

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
      void clear();

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
                                         const CommonTime& toTime) const;

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
                                       const CommonTime& toTime) const;

         /** Determine if a given message/satellite/signal is
          * available in the factory.
          * @param[in] nmid The message/satellite/signal to search for.
          * @param[in] fromTime The earliest time for which any
          *   messages should be available.
          * @param[in] toTime The earliest time for which any
          *   messages should be NOT available.
          * @return true if the given satellite/signal is has data in
          *   the given time span.
          * @note We specifically require the time range parameters to
          *   try to avoid making assumptions about the size of the
          *   data set (i.e. assuming the data is going to be a day's
          *   worth when it's actually several years. */
      bool isPresent(const NavMessageID& nmid,
                     const CommonTime& fromTime,
                     const CommonTime& toTime);

         /** Determine if a given satellite/signal is available in the factory.
          * @param[in] nmt The navigation message type of interest.
          * @param[in] satID The satellite/signal to search for.
          * @param[in] fromTime The earliest time for which any
          *   messages should be available.
          * @param[in] toTime The earliest time for which any
          *   messages should be NOT available.
          * @return true if the given satellite/signal is has data in
          *   the given time span.
          * @note We specifically require the time range parameters to
          *   try to avoid making assumptions about the size of the
          *   data set (i.e. assuming the data is going to be a day's
          *   worth when it's actually several years. */
      bool isTypePresent(NavMessageType nmt,
                         const NavSatelliteID& satID,
                         const CommonTime& fromTime,
                         const CommonTime& toTime)
      { return isPresent(NavMessageID(satID,nmt),fromTime,toTime); }

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
