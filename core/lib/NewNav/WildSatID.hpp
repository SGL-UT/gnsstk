#ifndef GPSTK_WILDSATID_HPP
#define GPSTK_WILDSATID_HPP

#include "SatID.hpp"

namespace gpstk
{
      /** Implement a satellite identifier that accepts wildcards for
       * system and/or satellite number. */
   class WildSatID : public SatID
   {
   public:
         /// Initialize with system and number wildcards.
      WildSatID();
         /** Initialize with a set number and a wild system (weird).
          * @note explicit keyword to prevent assignment with
          *   unexpected results.
          * @param[in] p The satellite number to specifically match.
          */
      explicit WildSatID(int p);
         /** Initialize with a set system and a wild satellite number.
          * @note explicit keyword to prevent assignment with
          *   unexpected results.
          * @param[in] s The satellite system to specifically match.
          */
      explicit WildSatID(SatelliteSystem s);
         /** Initialize with no wildcards.
          * @param[in] p The satellite number to specifically match.
          * @param[in] s The satellite system to specifically match.
          */
      WildSatID(int p, SatelliteSystem s);
         /** Initialize with no wildcards.
          * @param[in] sid The SatID to specifically match.
          */
      WildSatID(SatID sid);

         /// Match a WildSatID, taking wildcards into consideration.
      bool operator==(const WildSatID& right) const;
         /// Order WildSatIDs, taking wildcards into consideration.
      bool operator<(const WildSatID& right) const;

      bool wildSat; ///< If true, any satellite matches.
      bool wildSys; ///< If true, any system matches.
   };
}

#endif // GPSTK_WILDSATID_HPP
