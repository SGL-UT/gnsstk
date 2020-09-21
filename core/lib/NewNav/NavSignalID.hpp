#ifndef GPSTK_NAVSIGNALID_HPP
#define GPSTK_NAVSIGNALID_HPP

#include <iostream>
#include <set>
#include "SatelliteSystem.hpp"
#include "CarrierBand.hpp"
#include "TrackingCode.hpp"
#include "NavType.hpp"

namespace gpstk
{
      /// Class used to identify navigation data signal types.
   class NavSignalID
   {
   public:
         /// Set all data members to "Unknown"
      NavSignalID();

         /** Initialize all data to specified values.
          * @param[in] sys The GNSS this signal originates from.
          * @param[in] car The carrier band of this signal.
          * @param[in] track The tracking code of this signal.
          * @param[in] nmt The navigation message format of this signal. */
      NavSignalID(SatelliteSystem sys, CarrierBand car, TrackingCode track,
                  NavType nmt);

         /// Sorting so we can use this class as a map key
      bool operator<(const NavSignalID& right) const
      { return (order(right) < 0); }
         /// Equality check (all data members)
      bool operator==(const NavSignalID& right) const
      { return (order(right) == 0); }
         /// Inequality check (all data members)
      bool operator!=(const NavSignalID& right) const
      { return (order(right) != 0); }

      SatelliteSystem system; ///< GNSS for this signal.
      CarrierBand carrier;    ///< Carrier frequency for this signal.
      TrackingCode code;      ///< Tracking/ranging code for this signal.
      NavType nav;            ///< Navigation message structure of this signal.

   protected:
         /** Generic comparison function that will tell us < = >
          * @param[in] right The object to compare with this.
          * @return <0 for this < right, == 0 for this == right, >0
          *   for this > right. */
      int order(const NavSignalID& right) const;
   };

      /// Set of nav data signal identifiers.
   using NavSignalSet = std::set<NavSignalID>;


   inline std::ostream& operator<<(std::ostream& s, const NavSignalID& nsid)
   {
      s << StringUtils::asString(nsid.system) << " "
        << StringUtils::asString(nsid.carrier) << " "
        << StringUtils::asString(nsid.code) << " "
        << StringUtils::asString(nsid.nav);
      return s;
   }
}

#endif // GPSTK_NAVSIGNALID_HPP
