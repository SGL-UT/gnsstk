#include "NavMessageType.hpp"

namespace gpstk
{
   namespace StringUtils
   {
      std::string asString(NavMessageType e) throw()
      {
         switch (e)
         {
            case NavMessageType::Unknown:    return "Unknown";
            case NavMessageType::Almanac:    return "Almanac";
            case NavMessageType::Ephemeris:  return "Ephemeris";
            case NavMessageType::TimeOffset: return "TimeOffset";
            case NavMessageType::Health:     return "Health";
            case NavMessageType::Clock:      return "Clock";
            default:                         return "???";
         } // switch (e)
      } // asString(NavMessageType)


      NavMessageType asNavMessageType(const std::string& s) throw()
      {
         if (s == "Unknown")
            return NavMessageType::Unknown;
         if (s == "Almanac")
            return NavMessageType::Almanac;
         if (s == "Ephemeris")
            return NavMessageType::Ephemeris;
         if (s == "TimeOffset")
            return NavMessageType::TimeOffset;
         if (s == "Health")
            return NavMessageType::Health;
         if (s == "Clock")
            return NavMessageType::Clock;
         return NavMessageType::Unknown;
      } // asNavMessageType(string)
   } // namespace StringUtils
} // namespace gpstk
