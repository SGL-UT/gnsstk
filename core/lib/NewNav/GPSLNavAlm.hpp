#ifndef GPSTK_GPSLNAVALM_HPP
#define GPSTK_GPSLNAVALM_HPP

#include "GPSLNavData.hpp"

namespace gpstk
{
      /// Class to distinguish between Almanac data and Almanac data.
   class GPSLNavAlm : public GPSLNavData
   {
   public:
         /// Sets the nav message type.
      GPSLNavAlm();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @todo implement some checking.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode.
          * @return transmit time + 6s, as only one 6s subframe is used.
          */
      CommonTime getUserTime() const override;

         /** Fill the beginFit and endFit values for this object.
          * @todo Implement GPSLNavAlm::fixFit */
      void fixFit();

      uint8_t healthBits; ///< 8 SV health bits.
   };
}

#endif // GPSTK_GPSLNAVALM_HPP
