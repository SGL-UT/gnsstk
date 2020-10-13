#ifndef GPSTK_GPSCNAVEPH_HPP
#define GPSTK_GPSCNAVEPH_HPP

#include "GPSCNavData.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Class containing data elements unique to GPS CNav ephemerides.
       * @note The standard xmitTime data member (in OrbitDataKepler)
       *   corresponds to the transmit time of message type 10. */
   class GPSCNavEph : public GPSCNavData
   {
   public:
         /// Sets the nav message type and all other data members to 0.
      GPSCNavEph();

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
          * @return most recent transmit time + 12s, which is the time required
          *   for a message.
          * @todo If this class ends up being used for L5 CNAV or
          *   CNAV2 make sure it's the right interval.
          */
      CommonTime getUserTime() const override
      { return std::max({xmitTime, xmit11, xmitClk}) + 12.0; }

         /** Fill the beginFit and endFit values for this object.
          * @pre xmitTime, xmit11, xmitClk, Toe must all be set. */
      void fixFit();

         /** Dump SV status information (e.g. health).
          * @param[in,out] s The stream to write the data to. */
      void dumpSVStatus(std::ostream& s) const override;

      uint32_t pre11;     ///< The preamble from the start of message type 11.
      uint32_t preClk;    ///< The preamble from the start of the clock message.
         /// @note The health flags are true if unhealthy.
      bool healthL1;      ///< L1 signal health from message type 10.
      bool healthL2;      ///< L2 signal health from message type 10.
      bool healthL5;      ///< L5 signal health from message type 10.
      int8_t uraED;       ///< 5-bit URA index from message type 10.
      int8_t uraNED0;     ///< non-elevation dependent URA from clock message.
      uint8_t uraNED1;    ///< non-elevation dependent URA from clock message.
      uint8_t uraNED2;    ///< non-elevation dependent URA from clock message.
      bool alert11;       ///< Alert flag from message type 11.
      bool alertClk;      ///< Alert flag from the clock message.
      bool integStat;     ///< Integrity status flag.
      bool phasingL2C;    ///< L2C phasing
      double deltaA;      ///< Semi-major axis relative to reference (Aref).
      double dOMEGAdot;   ///< Rate of right ascension relative to -2.6e-9*pi.
      CommonTime top;     ///< Time of prediction.
      CommonTime xmit11;  ///< Transmit time for message 11.
      CommonTime xmitClk; ///< Transmit time for the clock message.
   };

      //@}

}

#endif // GPSTK_GPSCNAVEPH_HPP
