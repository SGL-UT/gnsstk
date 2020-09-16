#ifndef GPSTK_NAVDATA_HPP
#define GPSTK_NAVDATA_HPP

#include <memory>
#include <map>
#include "CommonTime.hpp"
#include "NavSignalID.hpp"
#include "NavMessageID.hpp"

namespace gpstk
{
      /** This is an abstract base class for decoded navigation
       * message data, including orbit information, health data and
       * time offsets. */
   class NavData
   {
   public:
         /// Specify level of detail for dump output.
      enum class Detail
      {
         OneLine, ///< Limit output to minimal information on a single line.
         Brief,   ///< Limit output to <= 5 lines of minimal information.
         Full     ///< Include all detailed information.
      };
         /** Checks the contents of this message against known
          * validity rules as defined in the appropriate ICD.
          * @return true if this message is valid according to ICD criteria.
          */
      virtual bool validate() const = 0;
         /** Returns the time when the navigation message would have
          * first been available to the user equipment, i.e. the time
          * at which the final bit of a given broadcast navigation
          * message is received.  This is used by
          * NavDataFactoryWithStore::find() in User mode. */
      virtual CommonTime getUserTime() const = 0;
         /** Print the contents of this NavData object in a
          * human-readable format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      virtual void dump(std::ostream& s, Detail dl);
         /** Time stamp used to sort the data.  This should be the
          * appropriate time stamp used when attempting to find the
          * data, usually the transmit time. */
      CommonTime timeStamp;
         /// Source signal identification for this navigation message data.
      NavMessageID signal;
   };

      /// Factories instantiate these in response to find() requests
   using NavDataPtr = std::shared_ptr<NavData>;
      /// Map nav message transmit time to nav message.
   using NavMap = std::map<CommonTime, NavDataPtr>;
      /// Map satellite to nav data.
   using NavSatMap = std::map<NavSatelliteID, NavMap>;
      /// Map signal to satellite navigation data.
   using NavSignalMap = std::map<NavSignalID, NavSatMap>;
      /// Map nav message type to the rest of the storage.
   using NavMessageMap = std::map<NavMessageType, NavSignalMap>;
}

#endif // GPSTK_NAVDATA_HPP
