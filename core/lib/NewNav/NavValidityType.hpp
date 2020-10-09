#ifndef GPSTK_NAVVALIDITYTYPE_HPP
#define GPSTK_NAVVALIDITYTYPE_HPP

#include <string>
#include "EnumIterator.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Enumeration used to determine how to handle valid and
       * invalid navigation data.  Validity is defined in the
       * individual ICDs, where ranges and such are defined. */
   enum class NavValidityType
   {
      Unknown,     ///< Only used as an initial value.
      ValidOnly,   ///< Only load/find nav messages that pass validity checks.
      InvalidOnly, ///< Only load/find nav messages that fail validity checks.
      All,         ///< Load/find nav messages regardless of validity checks.
      Last         ///< Used to create an iterator.
   };

      /** Define an iterator so C++11 can do things like
       * for (NavValidityType i : NavValidityTypeIterator()) */
   typedef EnumIterator<NavValidityType, NavValidityType::Unknown, NavValidityType::Last> NavValidityTypeIterator;

   namespace StringUtils
   {
         /// Convert a NavValidityType to a whitespace-free string name.
      std::string asString(NavValidityType e) throw();
         /// Convert a string name to an NavValidityType
      NavValidityType asNavValidityType(const std::string& s) throw();
   }

      //@}

}

#endif // GPSTK_NAVVALIDITYTYPE_HPP
