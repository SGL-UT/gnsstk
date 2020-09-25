#ifndef GPSTK_NAVMESSAGETYPE_HPP
#define GPSTK_NAVMESSAGETYPE_HPP

#include <string>
#include <set>
#include "EnumIterator.hpp"

namespace gpstk
{
      /// Identify different types of navigation message data.
   enum class NavMessageType
   {
      Unknown,    ///< Message type is not known or is uninitialized.
      Almanac,    ///< Low-precision orbits for other than the transmitting SV.
      Ephemeris,  ///< Precision orbits for the transmitting SV.
      TimeOffset, ///< Message containing information about time system offsets
      Health,     ///< SV health status information message.
      Clock,      ///< SV Clock offset data.  Currently only used by SP3.
      Last        ///< Used to create an iterator.
   };

      /** Define an iterator so C++11 can do things like
       * for (NavMessageType i : NavMessageTypeIterator()) */
   typedef EnumIterator<NavMessageType, NavMessageType::Unknown, NavMessageType::Last> NavMessageTypeIterator;
      /// Set of message types, used by NavLibrary and NavDataFactory.
   typedef std::set<NavMessageType> NavMessageTypeSet;

      /** Convenient set of all valid (non-meta) message types.
       * @note This is defined in NavStatic.cpp */
   extern const NavMessageTypeSet allNavMessageTypes;

   namespace StringUtils
   {
         /// Convert a NavMessageType to a whitespace-free string name.
      std::string asString(NavMessageType e) throw();
         /// Convert a string name to an NavMessageType
      NavMessageType asNavMessageType(const std::string& s) throw();
   }
}

#endif // GPSTK_NAVMESSAGETYPE_HPP
