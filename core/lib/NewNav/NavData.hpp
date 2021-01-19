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
#ifndef GPSTK_NAVDATA_HPP
#define GPSTK_NAVDATA_HPP

#include <memory>
#include <map>
#include <list>
#include "CommonTime.hpp"
#include "NavSignalID.hpp"
#include "NavMessageID.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** This is an abstract base class for decoded navigation
       * message data, including orbit information, health data and
       * time offsets. */
   class NavData
   {
   public:
         /// Specify level of detail for dump output.
      enum class Detail
      {
         OneLine, ///< Limit output to minimal information on a single line.
         Brief,   ///< Limit output to <= 5 lines of minimal information.
         Full     ///< Include all detailed information.
      };
         /// Initialize internal data fields.
      NavData();
         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      virtual bool validate() const = 0;
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
         /** Print the contents of this NavData object in a
          * human-readable format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      virtual void dump(std::ostream& s, Detail dl) const;
         /** Time stamp used to sort the data.  This should be the
          * appropriate time stamp used when attempting to find the
          * data, usually the transmit time. */
      CommonTime timeStamp;
         /// Source signal identification for this navigation message data.
      NavMessageID signal;
   protected:
         /** Navigation message length in seconds.  This is used by
          * getUserTime() by default, though it is possible to
          * override getUserTime() to ignore this value (as in
          * CNav). */
      double msgLenSec;
   };

      /// Factories instantiate these in response to find() requests
   using NavDataPtr = std::shared_ptr<NavData>;
      /// Map nav message transmit time to nav message.
   using NavMap = std::map<CommonTime, NavDataPtr>;
      /// Map satellite to nav data.
   using NavSatMap = std::map<NavSatelliteID, NavMap>;
      /// Map nav message type to the rest of the storage.
   using NavMessageMap = std::map<NavMessageType, NavSatMap>;
      /** List of NavDataPtr, typically used when converting from
       * source data to NavDataPtr when multiple objects can be
       * created from a single input. */
   using NavDataPtrList = std::list<NavDataPtr>;
      /// Map from "nearest" time reference (e.g. toe) to list of NavDataPtr.
   using NavNearMap = std::map<CommonTime, NavDataPtrList>;
      /// Map satellite to nearest map.
   using NavNearSatMap = std::map<NavSatelliteID, NavNearMap>;
      /// Map nav message type to the rest of the storage.
   using NavNearMessageMap = std::map<NavMessageType, NavNearSatMap>;

      //@}

}

#endif // GPSTK_NAVDATA_HPP
