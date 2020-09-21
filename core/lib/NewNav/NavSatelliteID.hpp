#ifndef GPSTK_NAVSATELLITEID_HPP
#define GPSTK_NAVSATELLITEID_HPP

#include <iostream>
#include "NavSignalID.hpp"
#include "WildSatID.hpp"

namespace gpstk
{
      /** Class for identifying the transmitting satellite of a
       * navigation message, as well as the "subject" satellite, in
       * the case of almanac data where the navigation data may be for
       * a different satellite than the one transmitting. */
   class NavSatelliteID : public NavSignalID
   {
   public:
         /// Sets the IDs to 0.
      NavSatelliteID();

         /** Initialize the parent class data while setting the sat
          * and xmitSat to wildcard values.
          * @param[in] sig The signal to initialize from. */
      NavSatelliteID(const NavSignalID& right);

         /** Initialize all data to specific values.
          * @param[in] subj The ID of the satellite that nav data pertains to.
          * @param[in] xmit The ID of the satellite that transmitted
          *   the nav data.
          * @param[in] sys The GNSS this signal originates from.
          * @param[in] car The carrier band of this signal.
          * @param[in] track The tracking code of this signal.
          * @param[in] nmt The navigation message format of this signal. */
      NavSatelliteID(unsigned long subj, unsigned long xmit,
                     SatelliteSystem sys, CarrierBand car, TrackingCode track,
                     NavType nmt);

         /** Initialize system, sat to the given value, and the
          * remaining data members to wildcards.
          * @param[in] subj The satellite that nav data pertains to. */
      NavSatelliteID(const WildSatID& subj);

         /// Sorting so we can use this class as a map key
      bool operator<(const NavSatelliteID& right) const;
         /// Comparison, including wildcards.
      bool operator==(const NavSatelliteID& right) const;
         /// Implicit != not available
      bool operator!=(const NavSatelliteID& right) const
      { return !(operator==(right)); }

      WildSatID sat;     ///< ID of satellite to which the nav data applies.
      WildSatID xmitSat; ///< ID of the satellite transmitting the nav data.
   };


   inline std::ostream& operator<<(std::ostream& s, const NavSatelliteID& nsid)
   {
      s << "subj:" << nsid.sat << " xmit:" << nsid.xmitSat << " "
        << static_cast<NavSignalID>(nsid);
      return s;
   }
}

#endif // GPSTK_NAVSATELLITEID_HPP
