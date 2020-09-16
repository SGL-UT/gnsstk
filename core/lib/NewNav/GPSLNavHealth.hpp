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
         /// Initialize to unhealthy.
      GPSLNavHealth()
            : svHealth(0x3f)
      {}

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override
      { return true; }

         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.
          * @return transmit time + 6s, which is the time required for sf1.
          */
      CommonTime getUserTime() const override
      { return timeStamp + 6.0; }

         /** Defines the status of NavData::signal, specifically sat
          * (not xmitSat). */
      SVHealth getHealth() const override
      { return (svHealth == 0) ? SVHealth::Healthy : SVHealth::Unhealthy; }

         /// 6-bit or 8-bit health.
      uint8_t svHealth;
   };
}

#endif // GPSTK_GPSLNAVHEALTH_HPP
