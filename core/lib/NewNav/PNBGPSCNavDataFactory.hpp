#ifndef GPSTK_PNBGPSCNAVDATAFACTORY_HPP
#define GPSTK_PNBGPSCNAVDATAFACTORY_HPP

#include "PNBNavDataFactory.hpp"
#include "GPSWeekSecond.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** This class provides a factory that processes PackedNavBits
       * data containing GPS CNav (Civil Nav, aka data ID 4) data as
       * defined in IS-GPS-200 section 30.  The results of the addData
       * method (in the navOut argument) may contain any number of
       * GPSCNavAlm, GPSCNavEph, GPSCNavHealth or GPSCNavTimeOffset
       * objects, according to what data is fed to the method and what
       * data is requested via the validity and type filters (see
       * PNBNavDataFactory).
       * @note Currently validity is not checked in any way in this class.
       * @note Currently only message types 10, 11, 30, 32 and 33 are
       *   being broadcast by the constellation.  As such, no further
       *   effort is going to be spent at this time on decoding
       *   messages outside this set.  GPSCNavAlm data structure and
       *   decoding was already implemented by this time, but the data
       *   isn't being transmitted and has not been tested. */
   class PNBGPSCNavDataFactory : public PNBNavDataFactory
   {
   public:
         /** Process a PackedNavBits object, producing NavData objects
          * as appropriate.
          * @param[in] navIn The PackedNavBits data to process.
          * @param[out] navOut Any resulting NavData objects that were
          *   completed, usually as a result of adding navIn to the
          *   set of data.
          * @return false on error. */
      bool addData(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut)
         override;

         /** Process ephemeris messages 1-2.  When a complete and
          * consistent ephemeris is accumulated in ephAcc, that
          * ephemeris is placed in navOut.
          * @param[in] msgType The CNAV message type (10-11).
          * @param[in] navIn The as-broadcast ephemeris subframe bits.
          * @param[out] navOut If an ephemeris is completed, this will
          *   contain a GPSCNavEph object.
          * @return false on error. */
      bool processEph(unsigned msgType, const PackedNavBitsPtr& navIn,
                      NavDataPtrList& navOut);

         /** Process message type 37.
          * @param[in] navIn The PackedNavBits data containing the subframe.
          * @param[in] navOut The GPSCNavAlm and/or GPSCNavHealth
          *   objects generated from navIn.
          * @return true if successful (navOut may still be empty). */
      bool processAlmOrb(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

         /** Process message type 33.  This includes GPS-GNSS time offset data.
          * @param[in] navIn The PackedNavBits data containing the subframe.
          * @param[in] navOut The GPSCNavTimeOffset object generated from
          *   navIn.
          * @return true if successful. */
      bool process33(const PackedNavBitsPtr& navIn, NavDataPtrList& navOut);

         /** For debugging purposes, dump the sizes of the accumulator maps.
          * @param[in,out] s The stream to write the debug output to. */
      void dumpState(std::ostream& s) const;

   protected:
         /** Map GPS PRN to a vector of PackedNavBits for accumulating
          * ephemeris data, where index 0 is subframe 1 and so on. */
      std::map<unsigned, std::vector<PackedNavBitsPtr> > ephAcc;
   };

      //@}

} // namespace gpstk

#endif // GPSTK_PNBGPSCNAVDATAFACTORY_HPP
