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
         /// Aref value defined in IS-GPS-200 (meters).
      static const double refAGPS;
         /// OMEGA dot reference value defined in IS-GPS-200 (radians).
      static const double refOMEGAdotGPS;
         /// inclination offset, this + delta i = i0, defined in IS-GPS-200.
      static const double refioffsetGPS;
         /// delta i reference value defined in IS-GPS-200 (radians).
      static const double refdeltaiGPS;
         /// Argument of perigee reference value (radians, guessed at).
      static const double refwGPS;
         /// Reference eccentricity for reduced almanac in IS-GPS-200.
      static const double refEccGPS;

         /// Aref value defined in IS-QZSS (meters).
      static const double refAQZSS;
         /// OMEGA dot reference value for QZSS (radians).
      static const double refOMEGAdotQZSS;
         /// inclination offset, this + delta i = i0, defined in IS-QZSS.
      static const double refioffsetQZSS;
         /// delta i reference value defined in IS-QZSS (radians).
      static const double refdeltaiQZSS;
         /// Argument of perigee reference value in IS-QZSS (radians).
      static const double refwQZSS;
         /// Reference eccentricity for reduced almanac in IS-QZSS-200.
      static const double refEccQZSS;

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
