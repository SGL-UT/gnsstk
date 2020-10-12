#ifndef GPSTK_GPSCNAVDATA_HPP
#define GPSTK_GPSCNAVDATA_HPP

#include "OrbitDataKepler.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Class that defines nav message data information that is
       * shared between GPS CNav almanac and ephemeris messages. */
   class GPSCNavData : public OrbitDataKepler
   {
   public:
         /// Initialize data members.
      GPSCNavData();

         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      bool validate() const override;

      uint32_t pre; ///< The preamble from the start of the subframe.
      bool alert;   ///< Alert flag
   };

      //@}

}

#endif // GPSTK_GPSCNAVDATA_HPP
