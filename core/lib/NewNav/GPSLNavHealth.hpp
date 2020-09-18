#ifndef GPSTK_GPSLNAVHEALTH_HPP
#define GPSTK_GPSLNAVHEALTH_HPP

#include "NavHealthData.hpp"

namespace gpstk
{
      /** Wrapper for the 6-bit or 8-bit health status in GPS LNav
       * subframe 1, word 3, SV/Page ID 1-32, 51, and 63. */
   class GPSLNavHealth : public NavHealthData
   {
   public:
         /// Initialize to unhealthy using a value typically not seen in health.
      GPSLNavHealth()
            : svHealth(0x80)
      {}

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true always as there is nothing to check in this class.
          */
      bool validate() const override
      { return true; }

         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.
          * @return transmit time + 6s.  In all cases in LNav, the
          *   health bits represented by this class are in a single
          *   subframe.
          */
      CommonTime getUserTime() const override
      { return timeStamp + 6.0; }

         /** Defines the status of NavData::signal, specifically sat
          * (not xmitSat).
          * @return Healthy if no health bits are set. */
      SVHealth getHealth() const override
      { return (svHealth == 0) ? SVHealth::Healthy : SVHealth::Unhealthy; }

         /// 6-bit or 8-bit health.
      uint8_t svHealth;
   };
}

#endif // GPSTK_GPSLNAVHEALTH_HPP
