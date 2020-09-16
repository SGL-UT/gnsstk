#ifndef GPSTK_GPSLNAVEPH_HPP
#define GPSTK_GPSLNAVEPH_HPP

#include "GPSLNavData.hpp"

namespace gpstk
{
      /// Class to distinguish between Almanac data and Ephemeris data.
   class GPSLNavEph : public GPSLNavData
   {
   public:
         /// Codes on L2 channel, per IS-GPS-200 20.3.3.3.1.2
      enum class L2Codes
      {
         Invalid1 = 0,
         Pcode = 1,
         CAcode = 2,
         Invalid2 = 3
      };

         /// Sets the nav message type and all other data members to 0.
      GPSLNavEph();

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
          * @return transmit time + 18s, which is the time required for sf1-3.
          */
      CommonTime getUserTime() const override
      { return timeStamp + 18.0; }

         /** Fill the beginFit and endFit values for this object.
          * @pre Toe, iodc, fitIntFlag and xmitTime must all be set. */
      void fixFit();

         /** Dump SV status information (e.g. health).
          * @param[in,out] s The stream to write the data to. */
      void dumpSVStatus(std::ostream& s) override;

      uint32_t pre2;      ///< The TLM preamble from word 1 of subframe 2.
      uint32_t pre3;      ///< The TLM preamble from word 1 of subframe 3.
      uint32_t tlm2;      ///< The TLM message from word 1 of subframe 2.
      uint32_t tlm3;      ///< The TLM message from word 1 of subframe 3.
      uint16_t iodc;      ///< Issue Of Data-Clock for the ephemeris.
      uint16_t iode;      ///< Issue Of Data-Ephemeris.
      uint8_t fitIntFlag; ///< Fit interval flag from subframe 2.
      uint8_t healthBits; ///< 6 SV health bits from subframe 1, word 3.
      uint8_t uraIndex;   ///< 4-bit URA index from subframe 1, word 3.
      double tgd;         ///< Ionospheric group delay in seconds.
      bool asFlag2;       ///< Anti-spoof flag from SF2 HOW.
      bool asFlag3;       ///< Anti-spoof flag from SF3 HOW.
      bool alert2;        ///< Alert flag from SF2 HOW
      bool alert3;        ///< Alert flag from SF3 HOW
      L2Codes codesL2;    ///< Code on L2 in-phase component.
         /** L2 P data flag from subframe 1, word 3.
          * @note This retains the behavior as described in
          *   IS-GPS-200K 20.3.3.3.1.6, in that true (1) indicates the
          *   LNAV data stream was commanded OFF on the P-code of the
          *   in-phase component of the L2 channel */
      bool L2Pdata;
   };


   namespace StringUtils
   {
         /// Convert L2Codes to a printable string for dump().
      std::string asString(GPSLNavEph::L2Codes e);
   }
}

#endif // GPSTK_GPSLNAVEPH_HPP
