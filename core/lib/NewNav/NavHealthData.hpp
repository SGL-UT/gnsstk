#ifndef GPSTK_NAVHEALTHDATA_HPP
#define GPSTK_NAVHEALTHDATA_HPP

#include "NavData.hpp"
#include "SVHealth.hpp"

namespace gpstk
{
      /** Abstract base class for classes that provide satellite
       * health information. */
   class NavHealthData : public NavData
   {
   public:
         /// Set our message type to health so we don't have to in every leaf.
      NavHealthData()
      { signal.messageType = NavMessageType::Health; }

         /** Defines the status of NavData::signal, specifically sat
          * (not xmitSat). */
      virtual SVHealth getHealth() const = 0;
   };
}

#endif // GPSTK_NAVHEALTHDATA_HPP
