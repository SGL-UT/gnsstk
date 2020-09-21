#ifndef GPSTK_NAVMESSAGEID_HPP
#define GPSTK_NAVMESSAGEID_HPP

#include <iostream>
#include "NavSatelliteID.hpp"
#include "NavMessageType.hpp"

namespace gpstk
{
      /// Class used to identify/categorize navigation message data.
   class NavMessageID : public NavSatelliteID
   {
   public:
         /// Initialize message type to Unknown
      NavMessageID()
            : messageType(NavMessageType::Unknown)
      {}
         /// Convenience constructor from NavSatelliteID
      NavMessageID(const NavSatelliteID& sat, NavMessageType nmt)
            : NavSatelliteID(sat), messageType(nmt)
      {}
         /// Comparison operator, obv.
      bool operator==(const NavMessageID& right) const
      {
         return ((messageType == right.messageType) &&
                 NavSatelliteID::operator==(right));
      }
         /// Implicit != not available
      bool operator!=(const NavMessageID& right) const
      {
         return ((messageType != right.messageType) ||
                 NavSatelliteID::operator!=(right));
      }
         /** Indicates whether a nav message is orbital elements,
          * health data or time offset information. */
      NavMessageType messageType;
   };


   inline std::ostream& operator<<(std::ostream& s, const NavMessageID& nmid)
   {
      s << StringUtils::asString(nmid.messageType) << " "
        << static_cast<NavSatelliteID>(nmid);
      return s;
   }
}

#endif // GPSTK_NAVMESSAGEID_HPP
