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
#ifndef GNSSTK_NAVDATA_HPP
#define GNSSTK_NAVDATA_HPP

#include <memory>
#include <map>
#include <list>
#include "gnsstk_export.h"
#include "CommonTime.hpp"
#include "NavSignalID.hpp"
#include "NavMessageID.hpp"
#include "DumpDetail.hpp"
#include "SatMetaDataStore.hpp"

#ifdef SWIG
// make sure SWIG doesn't generate broken accessor methods to this data
%immutable gnsstk::NavData::satMetaDataStore;
#endif

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

   class NavData; // forward declaration

      /// Factories instantiate these in response to find() requests
   typedef std::shared_ptr<NavData> NavDataPtr;
      /// Map nav message transmit time to nav message.
   typedef std::map<CommonTime, NavDataPtr> NavMap;
      /// Map satellite to nav data.
   typedef std::map<NavSatelliteID, NavMap> NavSatMap;
      /// Map nav message type to the rest of the storage.
   typedef std::map<NavMessageType, NavSatMap> NavMessageMap;
      /** List of NavDataPtr, typically used when converting from
       * source data to NavDataPtr when multiple objects can be
       * created from a single input. */
   typedef std::list<NavDataPtr> NavDataPtrList;
      /// Map from "nearest" time reference (e.g. toe) to list of NavDataPtr.
   typedef std::map<CommonTime, NavDataPtrList> NavNearMap;
      /// Map satellite to nearest map.
   typedef std::map<NavSatelliteID, NavNearMap> NavNearSatMap;
      /// Map nav message type to the rest of the storage.
   typedef std::map<NavMessageType, NavNearSatMap> NavNearMessageMap;

      /** This is an abstract base class for decoded navigation
       * message data, including orbit information, health data and
       * time offsets. */
   class NavData
   {
   public:
         /// Time format used for the dump method (Full).
      GNSSTK_EXPORT static const std::string dumpTimeFmt;
         /// Time format used for the dump method (Brief).
      GNSSTK_EXPORT static const std::string dumpTimeFmtBrief;
         /// Initialize internal data fields.
      NavData();
         /// Create a deep copy of this object, whatever it truly is.
      virtual NavDataPtr clone() const = 0;
         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      virtual bool validate() const = 0;
         /** Returns the fully-qualified class name.  Required for
          * SWIG, at least until a better solution is found. */
      virtual std::string getClassName() const;
         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode. */
      virtual CommonTime getUserTime() const
      { return timeStamp + msgLenSec; }
         /** Returns the time for the data to be used when searching
          * in "Nearest" mode.  Example: toe for ephemeris data.  This
          * is used by NavDataFactoryWithStore::find(). */
      virtual CommonTime getNearTime() const
      { return timeStamp; }
         /// Const accessor for msgLenSec (total message transmit time).
      double getMsgLenSec() const
      { return msgLenSec; }
         /** Returns true if this two objects are 
          *   1. same concrete type, and
          *   2. same data contents.
          * This is intended as a "data uniqueness test" to allow
          * detection of successive transmissions of same data
          * and avoid duplicate storage.  The exact rules for 
          * uniqueness will vary by descendent class. 
          * @note We use shared_ptr to allow for casting without
          *   risking memory leaks. */
      virtual bool isSameData(const NavDataPtr& right) const;
         /** Compare two NavData descendent objects.
          *  Any differences are summarized and returned as a list of
          *  readable text.
          * @param[in] right The data to compare against.
          * @return a text list of mismatched data.
          * @note We use shared_ptr to allow for casting without
          *   risking memory leaks. */
      virtual std::list<std::string> compare(const NavDataPtr& right) const;
         /** Print the contents of this NavData object in a
          * human-readable format.
          * @note for dump methods to properly map from PRN to SVN,
          *   the "global" satMetaDataStore pointer must be set to a
          *   store that has data loaded.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      virtual void dump(std::ostream& s, DumpDetail dl) const;
         /** Shortcut to SatMetaDataStore::getSVN() that obviates
          * having to check the pointer for null.
          * @param[in] sat The ID of the desired satellite.
          * @param[in] when The time of interest of the desired satellite.
          * @param[out] svn If found the satellite's vehicle number.
          * @return true if the requested satellite mapping was found.
          */
      bool getSVN(const SatID& sat, const gnsstk::CommonTime& when,
                  std::string& svn)
         const
      {
         return ((satMetaDataStore != nullptr) &&
                 satMetaDataStore->getSVN(sat,when,svn));
      }
         /** Return lines of text containing the sufficient
          * specification of the current nav message signal. */
      std::string getSignalString() const;
         /** Return an appropriate header label for the time format in dump().
          * @param[in] dl The detail level for the time string to be dumped. 
          * @return A string labeling the columns of the time format
          *   (may be blank for some detail levels). */
      std::string getDumpTimeHdr(DumpDetail dl) const;
         /** Format a time appropriately for dump().
          * @param[in] dl The detail level for the time string to be dumped. 
          * @param[in] t The time to format for dumping.
          * @return A string containing the formatted time. */
      std::string getDumpTime(DumpDetail dl, const CommonTime& t) const;
         /** Time stamp used to sort the data.  This should be the
          * appropriate time stamp used when attempting to find the
          * data, usually the transmit time. */
      CommonTime timeStamp;
         /// Source signal identification for this navigation message data.
      NavMessageID signal;

         /// Set this to a valid store to get PRN->SVN translations in dump().
      GNSSTK_EXPORT static gnsstk::SatMetaDataStore *satMetaDataStore;
         /// Accessor for python
      static void setSatMetaDataStore(gnsstk::SatMetaDataStore *smds)
      { satMetaDataStore = smds; }
         /// Accessor for python
      static gnsstk::SatMetaDataStore* getSatMetaDataStore()
      { return satMetaDataStore; }

         /** Format string for printing week in dump().  This defaults
          * to "%4F(%4G)" which is the GPS full and short week, and
          * other GNSSes should use the same width, but different
          * format tokens (see TimeString.hpp).  Systems that don't
          * have a week number, e.g. GLONASS, should set this to an
          * empty string, and dump() methods should ignore it
          * accordingly. */
      std::string weekFmt;
   protected:
         /** Navigation message length in seconds.  This is used by
          * getUserTime() by default, though it is possible to
          * override getUserTime() to ignore this value (as in
          * CNav). */
      double msgLenSec;
         /// Allow RinexNavDataFactory access to msgLenSec
      friend class RinexNavDataFactory;
   };

      //@}

}

#endif // GNSSTK_NAVDATA_HPP
