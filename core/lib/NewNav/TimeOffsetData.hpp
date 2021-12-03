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
#ifndef GNSSTK_TIMEOFFSETDATA_HPP
#define GNSSTK_TIMEOFFSETDATA_HPP

#include "NavData.hpp"
#include "TimeSystem.hpp"
#include "CommonTime.hpp"

namespace gnsstk
{
      /// @ingroup NavFactory
      //@{

      /** Define a pair of TimeSystems where first=convert from and
       * second=convert to. */
   typedef std::pair<TimeSystem,TimeSystem> TimeCvtKey;
      /// Define a unique set of time system conversions.
   typedef std::set<TimeCvtKey> TimeCvtSet;

      /** Defines the interface for classes that provide the ability
       * to convert between time systems, using data extracted from
       * GNSS navigation messages. */
   class TimeOffsetData : public NavData
   {
   public:
         /// Set the messageType
      TimeOffsetData()
      { signal.messageType = NavMessageType::TimeOffset; }

         /// Obligatory virtual destructor.
      virtual ~TimeOffsetData()
      {}

         /** Returns the time for the data to be used when searching
          * in "Nearest" mode. */
      CommonTime getNearTime() const override
      { return timeStamp; }

         /** Get the offset, in seconds, to apply to times when
          * converting them from fromSys to toSys.
          * @param[in] fromSys The time system to convert from.
          * @param[in] toSys The time system to convert to.
          * @param[in] when The time being converted, usually in the
          *   time system appropriate for a given nav message source.
          *   The details of what time system this should be in and
          *   any other restrictions will be documented in each leaf
          *   class, e.g. GPSLNavTimeOffset.
          * @param[out] offset The offset in seconds where
          *   when(toSys)=when(fromSys)-offset.
          * @return true if an offset is available, false if not. */
      virtual bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                             const CommonTime& when, double& offset) const = 0;

         /** The set of time system conversions this class is capable of making.
          * @note This method should avoid returning bidirectional
          *   conversions, e.g. either TimeCvtKey(GPS,UTC) or
          *   TimeCvtKey(UTC,GPS) but not both.  The process of
          *   reversing the conversion pair will be implemented in the
          *   NavDataFactory.
          * @return a set of supported time system conversion to/from pairs. */
      virtual TimeCvtSet getConversions() const = 0;

         /// @copydoc NavData::isSameData
      bool isSameData(const NavDataPtr& right) const override
      { GNSSTK_THROW(Exception("Unimplemented function")); }
         /// @copydoc NavData::compare
      std::list<std::string> compare(const NavDataPtr& right)
         const override
      { GNSSTK_THROW(Exception("Unimplemented function")); }
   };

      //@}

}

#endif // GNSSTK_TIMEOFFSETDATA_HPP
