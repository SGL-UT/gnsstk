#ifndef GPSTK_GPSLNAVDATA_HPP
#define GPSTK_GPSLNAVDATA_HPP

#include "OrbitDataKepler.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Class that defines nav message data information that is
       * shared between GPS LNav almanac and ephemeris messages.
       * @note The data in this class should represent subframe 1's
       *   data in the case of an ephemeris, or the single subframe
       *   for almanac data.
       * @see GPSLNavEph for data from subframes 2 and 3. */
   class GPSLNavData : public OrbitDataKepler
   {
   public:
         /// Initialize data members.
      GPSLNavData();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

      uint32_t pre; ///< The TLM preamble from word 1 of the subframe.
      uint32_t tlm; ///< The TLM message from word 1 of the subframe.
      bool alert;   ///< Alert flag from HOW
      bool asFlag;  ///< Anti-spoof flag from HOW
   };

      //@}

}

#endif // GPSTK_GPSLNAVDATA_HPP
