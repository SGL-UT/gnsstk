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

#ifndef GNSSTK_SATMETADATASTORE_HPP
#define GNSSTK_SATMETADATASTORE_HPP

#include <map>
#include <set>
#include "SatMetaData.hpp"
#include "SatMetaDataSort.hpp"
#include "NavID.hpp"

namespace gnsstk
{
      /** Provide a class for reading satellite metadata from a
       * comma-separated values (CSV) file and provide methods for
       * looking up information in that file.
       *
       * There are five different record formats, each starting with a
       * string literal that is one of "SAT", "NORAD", "LAUNCH",
       * "CLOCK" or "SIG".
       *
       * Satellite status record:
       *   \li SAT (literal)
       *   \li GNSS name (see below)
       *   \li svn (string containing system-defined unique satellite ID)
       *   \li prn (0 if n/a)
       *   \li FDMA channel (0 if n/a)
       *   \li FDMA slot ID (0 if n/a)
       *   \li start time: year
       *   \li start time: day of year
       *   \li start time: seconds of day
       *   \li end time: year
       *   \li end time: day of year
       *   \li end time: seconds of day
       *   \li orbital plane (empty string if not applicable or unknown)
       *   \li orbital slot (empty string if not applicable or unknown)
       *   \li signal set name (see the SIG record below)
       *   \li satellite status (see below)
       *   \li active clock number (255 = unknown)
       *
       * satellite status may be one of the following case sensitive options
       *   \li Operational
       *   \li Decommissioned
       *   \li Test
       *   \li Unknown
       *
       * Mapping system satellite number to NORAD identifier:
       *   \li NORAD (literal)
       *   \li GNSS name (see below)
       *   \li svn (string containing system-defined unique satellite ID)
       *   \li NORAD ID
       *
       * Satellite launch time:
       *   \li LAUNCH (literal)
       *   \li GNSS name (see below)
       *   \li svn (string containing system-defined unique satellite ID)
       *   \li launch time: year
       *   \li launch time: day of year
       *   \li launch time: seconds of day
       *   \li satellite block/type (arbitrary system-defined string)
       *   \li mission number (arbitrary system-defined string)
       *
       * Clock configuration (optional record, defaults to all unknown):
       *   \li CLOCK (literal)
       *   \li GNSS name (see below)
       *   \li satellite block/type (arbitrary system-defined string)
       *   \li clock type 1
       *   \li clock type 2
       *   \li clock type 3
       *   \li clock type 4
       *
       * clock type may be one of the following case sensitive options
       *   \li Crystal
       *   \li Cesium
       *   \li Rubidium
       *   \li Hydrogen
       *   \li USNO
       *   \li Unknown (which doubles as "no clock")
       *
       * Signal sets are defined using multiple SIG records as follows
       *   \li SIG (literal)
       *   \li signal set name (unique arbitrary string)
       *   \li carrier band name (see gnsstk::CarrierBand)
       *   \li tracking code name (see gnsstk::TrackingCode)
       *   \li navigation code name (see gnsstk::NavType)
       *
       * GNSS name may be one of the following case sensitive options
       *   \li GPS
       *   \li Galileo
       *   \li GLONASS
       *   \li Geostationary
       *   \li LEO
       *   \li Transit
       *   \li BeiDou
       *   \li QZSS
       *   \li IRNSS
       *   \li Mixed (technically not useful in this context)
       *   \li UserDefined (technically not useful in this context)
       *   \li Unknown
       *
       */
   class SatMetaDataStore
   {
   public:
         /// Specifies a single GNSS signal.
      struct Signal
      {
            /// Initialize everything to unknown.
         Signal()
               : carrier(CarrierBand::Unknown), code(TrackingCode::Unknown),
                 nav(NavType::Unknown)
         {}
            /// Initialize using complete set of specified values.
         Signal(CarrierBand c, TrackingCode r, NavType n)
               : carrier(c), code(r), nav(n)
         {}
            /// Ordering for template/container.
         inline bool operator<(const Signal& right) const;
            /// Comparison operator that allows "Any" to be a wildcard.
         inline bool operator==(const Signal& right) const;
            /// Comparison operator that allows "Any" to be a wildcard.
         inline bool operator!=(const Signal& right) const
         { return !operator==(right); }
         CarrierBand carrier; ///< Carrier frequency.
         TrackingCode code;   ///< Tracking code.
         NavType nav;         ///< Navigation code.
      };
         /// Key of GNSS and satellite block
      class SystemBlock
      {
      public:
         inline bool operator<(const SystemBlock& right) const;
         SatelliteSystem sys;
         std::string blk;
      };
         /// Like SatID but for SVN which is a string
      class SVNID
      {
      public:
         SVNID();
         SVNID(SatelliteSystem sys, const std::string& svn);
         SatelliteSystem system;
         std::string id;
         bool operator<(const SVNID& right) const;
      };
         /// Launch configuration
      class LaunchConfig
      {
      public:
         SVNID svn;
         gnsstk::CommonTime launchTime;
         std::string type;             ///< Typically block number.
         std::string mission;          ///< Mission number.
      };
         /// Set of signals that may be transmitted by a satellite.
      typedef std::set<Signal> SignalSet;
         /// Map of signal set name to signal set.
      typedef std::map<std::string, SignalSet> SignalMap;
         /// Set of satellites ordered by PRN or channel/slotID.
      typedef std::multiset<SatMetaData, SatMetaDataSort> SatSet;
         /// Satellites grouped by system.
      typedef std::map<SatelliteSystem, SatSet> SatMetaMap;
         /// Types of clocks on a satellite (hardware-specific positional idx).
      typedef std::vector<SatMetaData::ClockType> ClockVec;
         /// Clock configuration information
      typedef std::map<SystemBlock, ClockVec> ClockConfigMap;
         /// Map SVN to launch time.
      typedef std::map<SVNID, LaunchConfig> LaunchMap;
         /// Map SVN to NORAD ID.
      typedef std::map<SVNID, unsigned long> NORADMap;

         /// Nothin doin.
      SatMetaDataStore() = default;

         /** Attempt to load satellite metadata from the store.
          * @param[in] sourceName The path to the input CSV-format file.
          * @return true if successful, false on error.
          */
      virtual bool loadData(const std::string& sourceName);

         /** Find a satellite in the map by searching by PRN.
          * @param[in] sys The GNSS of the desired satellite.
          * @param[in] prn The pseudo-random number identifying the
          *   desired satellite.
          * @param[in] when The time of interest of the desired satellite.
          * @param[out] sat If found the satellite's metadata.
          * @return true if the requested satellite mapping was found.
          */
      bool findSat(SatelliteSystem sys, uint32_t prn,
                   const gnsstk::CommonTime& when,
                   SatMetaData& sat)
         const;

         /** Find a satellite in the map by searching by PRN.
          * @param[in] prn The satellite to find, identified by PRN
          *   (i.e. not FDMA channel/slot).
          * @param[in] when The time of interest of the desired satellite.
          * @param[out] sat If found the satellite's metadata.
          * @return true if the requested satellite mapping was found.
          */
      bool findSat(const SatID& prn,
                   const gnsstk::CommonTime& when,
                   SatMetaData& sat)
         const
      { return findSat(prn.system, prn.id, when, sat); }

         /** Get the space vehicle number of a satellite in the map by
          * searching by PRN.
          * @param[in] sys The GNSS of the desired satellite.
          * @param[in] prn The pseudo-random number identifying the
          *   desired satellite.
          * @param[in] when The time of interest of the desired satellite.
          * @param[out] svn If found the satellite's vehicle number.
          * @return true if the requested satellite mapping was found.
          */
      bool getSVN(SatelliteSystem sys, uint32_t prn,
                  const gnsstk::CommonTime& when,
                  std::string& svn)
         const;

         /** Get the space vehicle number of a satellite in the map by
          * searching by PRN.
          * @param[in] sat The ID of the desired satellite.
          * @param[in] when The time of interest of the desired satellite.
          * @param[out] svn If found the satellite's vehicle number.
          * @return true if the requested satellite mapping was found.
          */
      bool getSVN(const SatID& sat, const gnsstk::CommonTime& when,
                  std::string& svn)
         const
      { return getSVN(sat.system, sat.id, when, svn); }

         /** Get the NORAD number of a satellite in the map by
          * searching by PRN.
          * @param[in] sys The GNSS of the desired satellite.
          * @param[in] prn The pseudo-random number identifying the
          *   desired satellite.
          * @param[in] when The time of interest of the desired satellite.
          * @param[out] norad If found the satellite's NORAD number.
          * @return true if the requested satellite mapping was found.
          */
      bool getNORAD(SatelliteSystem sys, uint32_t prn,
                    const gnsstk::CommonTime& when,
                    int32_t& norad)
         const;

         /** Get the space vehicle number of a satellite in the map by
          * searching by PRN.
          * @param[in] sat The ID of the desired satellite.
          * @param[in] when The time of interest of the desired satellite.
          * @param[out] norad If found the satellite's NORAD number.
          * @return true if the requested satellite mapping was found.
          */
      bool getNORAD(const SatID& sat, const gnsstk::CommonTime& when,
                    int32_t& norad)
         const
      { return getNORAD(sat.system, sat.id, when, norad); }

         /** Find a satellite in the map by searching by SVN.
          * @param[in] sys The GNSS of the desired satellite.
          * @param[in] svn The system-unique space vehicle number
          *   identifying the desired satellite.
          * @param[in] when The time of interest of the desired satellite.
          * @param[out] sat If found the satellite's metadata.
          * @return true if the requested satellite mapping was found.
          */
      bool findSatBySVN(SatelliteSystem sys, const std::string& svn,
                        const gnsstk::CommonTime& when,
                        SatMetaData& sat)
         const;

         /** Find a GLONASS satellite in the map by searching by
          *  its orbit slotID and FDMA channel.  To be a unique
          *  identification, both are necessary.
          *  This is only applicable to GLONASS FDMA SVs
          * @param[in] slotID The GLONASS orbit slot ID
          *   identifying the desired satellite.
          * @param[in] channel The FDMA channel
          *   identifying the desired satellite.
          * @param[in] when The time of interest of the desired satellite.
          * @param[out] sat If found the satellite's metadata.
          * @return true if the requested satellite mapping was found.
          */
      bool findSatBySlotFdma(uint32_t slotID,
                             int32_t channel,
                             const gnsstk::CommonTime& when,
                             SatMetaData& sat)
         const;

         /** Get the pseudo-random number of a satellite in the map by
          * searching by SVN.
          * @param[in] sys The GNSS of the desired satellite.
          * @param[in] svn The space vehicle number identifying the
          *   desired satellite.
          * @param[in] when The time of interest of the desired satellite.
          * @param[out] prn If found the satellite's pseudo-random number.
          * @return true if the requested satellite mapping was found.
          */
      bool getPRN(SatelliteSystem sys, const std::string& svn,
                  const gnsstk::CommonTime& when,
                  uint32_t& prn)
         const;

         /** Get the names of signal sets that include the specified signal.
          * The "Any" enum value may be used to match as a wildcard.
          * @param[in] sig The signal to match.
          * @return A set of the signal set names that include the
          *   specified signal. */
      std::set<std::string> getSignalSet(const Signal& sig)
         const;

         /** Get the names of signal sets that include ALL of the
          * specified signals.  The "Any" enum value may be used to
          * match as a wildcard.
          * @param[in] signals The signals to match.
          * @return A set of the signal set names that include all of the
          *   specified signals. */
      std::set<std::string> getSignalSet(const SignalSet& signals)
         const;

         /** Get the names of signal sets that include the specified signal.
          * The "Any" enum value may be used to match as a wildcard.
          * @param[in] band The carrier band to match.
          * @param[in] code The tracking code to match.
          * @param[in] nav The navigation message format to match.
          * @return A set of the signal set names that include the
          *   specified signal. */
      std::set<std::string> getSignalSet(CarrierBand band, TrackingCode code,
                                         NavType nav)
         const
      { return getSignalSet(Signal(band,code,nav)); }
      
         /** Get a set of all of the SAT records that match ALL of the
          * specified signals.
          * @param[in] signals The signals to match.
          * @return A set of the satellites that broadcast all of the
          *   specified signals. */
      SatSet getSatsBySignal(const SignalSet& signals);
      
         /** Get a set of all of the SAT records that match ALL of the
          * specified signals and the specified time is between the
          * start and end time for the record.
          * @param[in] signals The signals to match.
          * @param[in] when The time of interest of the desired satellites.
          * @return A set of the satellites that broadcast all of the
          *   specified signals at the time of interest. */
      SatSet getSatsBySignal(const SignalSet& signals, const CommonTime& when);

         /** Get a set of all of the SAT records that match ALL of the
          * specified signals and the specified status at any time
          * during the specified time range.  Specifically, if the
          * startTime and endTime of the SAT record overlap at all
          * with the specified beginTime and endTime, that is
          * considered a match.
          * @param[in] signals The signals to match.
          * @param[in] beginTime The earliest time for which a SAT
          *   record may match.
          * @param[in] endTime The latest time for which a SAT record may match.
          * @param[in] status The desired status values of the
          *   satellite at the time range of interest (default=all).
          * @return A set of the satellites that broadcast all of the
          *   specified signals during the time range of interest. */
      SatSet getSatsBySignal(
         const SignalSet& signals,
         const CommonTime& beginTime,
         const CommonTime& endTime,
         const std::set<SatMetaData::Status>& status = {
            SatMetaData::Status::Operational,
            SatMetaData::Status::Decommissioned,
            SatMetaData::Status::Test });

         /// Storage of all the satellite metadata.
      SatMetaMap satMap;
         /// Map signal set name to the actual signals.
      SignalMap sigMap;
         /// Map satellite block to clock types.
      ClockConfigMap clkMap;
         /// Launch time of satellites.
      LaunchMap launchMap;
         /// Map SVN to NORAD ID.
      NORADMap noradMap;

   protected:
         /** Convert a SAT record to a SatMetaData record and store it.
          * @param[in] vals SAT record in the form of an array of columns.
          * @param[in] lineNo The line number of the input file being processed.
          * @return true if successful, false on error.
          */
      bool addSat(const std::vector<std::string>& vals, unsigned long lineNo);
         /** Convert a SIG record to a Signal object and store it.
          * @param[in] vals SIG record in the form of an array of columns.
          * @param[in] lineNo The line number of the input file being processed.
          * @return true if successful, false on error.
          */
      bool addSignal(const std::vector<std::string>& vals,
                     unsigned long lineNo);
         /** Add a CLOCK record to clkMap.
          * @param[in] vals CLOCK record in the form of an array of columns.
          * @param[in] lineNo The line number of the input file being processed.
          * @return true if successful, false on error.
          */
      bool addClock(const std::vector<std::string>& vals, unsigned long lineNo);
         /** Add a LAUNCH record to launchMap.
          * @param[in] vals LAUNCH record in the form of an array of columns.
          * @param[in] lineNo The line number of the input file being processed.
          * @return true if successful, false on error.
          */
      bool addLaunch(const std::vector<std::string>& vals,
                     unsigned long lineNo);
         /** Add a NORAD record to noradMap.
          * @param[in] vals NORAD record in the form of an array of columns.
          * @param[in] lineNo The line number of the input file being processed.
          * @return true if successful, false on error.
          */
      bool addNORAD(const std::vector<std::string>& vals, unsigned long lineNo);
   }; // class SatMetaDataStore


   bool SatMetaDataStore::Signal ::
   operator<(const Signal& right)
      const
   {
      if (static_cast<int>(carrier) < static_cast<int>(right.carrier))
      {
         return true;
      }
      if (static_cast<int>(carrier) > static_cast<int>(right.carrier))
      {
         return false;
      }
      if (static_cast<int>(code) < static_cast<int>(right.code))
      {
         return true;
      }
      if (static_cast<int>(code) > static_cast<int>(right.code))
      {
         return false;
      }
      if (static_cast<int>(nav) < static_cast<int>(right.nav))
      {
         return true;
      }
      return false;
   }


   bool SatMetaDataStore::Signal ::
   operator==(const Signal& right)
      const
   {
      if ((carrier != CarrierBand::Any) &&
          (right.carrier != CarrierBand::Any) &&
          (carrier != right.carrier))
      {
         return false;
      }
      if ((code != TrackingCode::Any) &&
          (right.code != TrackingCode::Any) &&
          (code != right.code))
      {
         return false;
      }
      if ((nav != NavType::Any) &&
          (right.nav != NavType::Any) &&
          (nav != right.nav))
      {
         return false;
      }
      return true;
   }


   bool SatMetaDataStore::SystemBlock ::
   operator<(const SatMetaDataStore::SystemBlock& right)
      const
   {
      if (static_cast<int>(sys) < static_cast<int>(right.sys))
         return true;
      if (static_cast<int>(sys) > static_cast<int>(right.sys))
         return false;
      return blk < right.blk;
   }


   inline std::ostream& operator<<(std::ostream& s,
                                   const SatMetaDataStore::SystemBlock& sblk)
   {
      s << gnsstk::StringUtils::asString(sblk.sys) << " " << sblk.blk;
      return s;
   }


   inline std::ostream& operator<<(std::ostream& s,
                                   const SatMetaDataStore::SVNID& svn)
   {
      s << gnsstk::StringUtils::asString(svn.system) << " " << svn.id;
      return s;
   }

} // namespace gnsstk

#endif // GNSSTK_SATMETADATASTORE_HPP
