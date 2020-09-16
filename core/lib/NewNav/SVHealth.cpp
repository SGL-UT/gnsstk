#include "SVHealth.hpp"

namespace gpstk
{
   namespace StringUtils
   {
      std::string asString(SVHealth e) throw()
      {
         switch (e)
         {
            case SVHealth::Unknown:    return "Unknown";
            case SVHealth::Healthy:    return "Healthy";
            case SVHealth::Unhealthy:  return "Unhealthy";
            case SVHealth::Degraded:   return "Degraded";
            default:                   return "???";
         } // switch (e)
      } // asString(SVHealth)


      SVHealth asSVHealth(const std::string& s) throw()
      {
         if (s == "Unknown")
            return SVHealth::Unknown;
         if (s == "Healthy")
            return SVHealth::Healthy;
         if (s == "Unhealthy")
            return SVHealth::Unhealthy;
         if (s == "Degraded")
            return SVHealth::Degraded;
         return SVHealth::Unknown;
      } // asSVHealth(string)
   } // namespace StringUtils
} // namespace gpstk
