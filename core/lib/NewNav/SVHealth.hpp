#ifndef GPSTK_SVHEALTH_HPP
#define GPSTK_SVHEALTH_HPP

#include <string>
#include "EnumIterator.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Identify different types of SV health states.
   enum class SVHealth
   {
      Unknown,    ///< Health is not known or is uninitialized.
      Any,        ///< Use in searches when you don't care about the SV health.
      Healthy,    ///< Satellite is in a healthy and useable state.
      Unhealthy,  ///< Satellite is unhealthy and should not be used.
      Degraded,   ///< Satellite is in a degraded state. Use at your own risk.
      Last        ///< Used to create an iterator.
   };

      /** Define an iterator so C++11 can do things like
       * for (SVHealth i : SVHealthIterator()) */
   typedef EnumIterator<SVHealth, SVHealth::Unknown, SVHealth::Last> SVHealthIterator;

   namespace StringUtils
   {
         /// Convert a SVHealth to a whitespace-free string name.
      std::string asString(SVHealth e) throw();
         /// Convert a string name to an SVHealth
      SVHealth asSVHealth(const std::string& s) throw();
   }

      //@}

}

#endif // GPSTK_SVHEALTH_HPP
