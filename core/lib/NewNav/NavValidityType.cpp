#include "NavValidityType.hpp"

namespace gpstk
{
   namespace StringUtils
   {
      std::string asString(NavValidityType e) throw()
      {
         switch (e)
         {
            case NavValidityType::Unknown:     return "Unknown";
            case NavValidityType::ValidOnly:   return "ValidOnly";
            case NavValidityType::InvalidOnly: return "InvalidOnly";
            case NavValidityType::Any:         return "Any";
            default:                           return "???";
         } // switch (e)
      } // asString(NavValidityType)


      NavValidityType asNavValidityType(const std::string& s) throw()
      {
         if (s == "Unknown")
            return NavValidityType::Unknown;
         if (s == "ValidOnly")
            return NavValidityType::ValidOnly;
         if (s == "InvalidOnly")
            return NavValidityType::InvalidOnly;
         if (s == "Any")
            return NavValidityType::Any;
         return NavValidityType::Unknown;
      } // asNavValidityType(string)
   } // namespace StringUtils
} // namespace gpstk
