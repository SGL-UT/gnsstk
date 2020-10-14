#ifndef GPSTK_GPSCNAVALM_HPP
#define GPSTK_GPSCNAVALM_HPP

#include "GPSCNavData.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /// Class containing data elements unique to GPS CNav almanac pages.
   class GPSCNavAlm : public GPSCNavData
   {
   public:
         /// Sets the nav message type.
      GPSCNavAlm();

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
          * @return transmit time + 12s or 6s, depending on whether
          *   this is CNAV on L5 or on L2.
          */
      CommonTime getUserTime() const override;

         /** This is just a method for making the dump output say
          * "almanac" vs "ephemeris" when appropriate. */
      std::string getDataType() const override
      { return "Almanac"; }
         /** Override dumpHarmonics to hide them in output since GPS
          * CNav almanacs don't contain this data. */
      void dumpHarmonics(std::ostream& s)
      {}

         /// Fill the beginFit and endFit values for this object.
      void fixFit();

         /// @note The health flags are true if unhealthy.
      bool healthL1;      ///< L1 signal health from message type 10.
      bool healthL2;      ///< L2 signal health from message type 10.
      bool healthL5;      ///< L5 signal health from message type 10.
      double deltai;      ///< Inclination in rad relative to 0.3*pi rad.
      unsigned wna;       ///< Reference week for toa.
      double toa;         ///< Convenience storage of unqualified toa.
   };

      //@}

}

#endif // GPSTK_GPSCNAVALM_HPP
