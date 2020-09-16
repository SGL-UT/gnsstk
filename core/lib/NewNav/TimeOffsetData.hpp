#ifndef GPSTK_TIMEOFFSETDATA_HPP
#define GPSTK_TIMEOFFSETDATA_HPP

#include "NavData.hpp"
#include "TimeSystem.hpp"
#include "CommonTime.hpp"

namespace gpstk
{
      /** Defines the interface for classes that provide the ability
       * to convert between time systems, using data extracted from
       * GNSS navigation messages. */
   class TimeOffsetData : public NavData
   {
   public:
         /// Set the messageType
      TimeOffsetData()
      { signal.messageType = NavMessageType::TimeOffset; }

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
          * @return true if an offset is available, false if not. */
      virtual bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                             const CommonTime& when, double& offset) = 0;
   };
}

#endif // GPSTK_TIMEOFFSETDATA_HPP
