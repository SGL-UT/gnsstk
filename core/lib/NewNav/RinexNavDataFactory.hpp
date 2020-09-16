#ifndef GPSTK_RINEXNAVDATAFACTORY_HPP
#define GPSTK_RINEXNAVDATAFACTORY_HPP

#include "NavDataFactoryWithStoreFile.hpp"
#include "Rinex3NavData.hpp"
#include "GPSLNavEph.hpp"

namespace gpstk
{
      /** Factory class for loading RINEX NAV (2, 3) data.
       * @note While in theory Y code and P code LNav are supported,
       *   RINEX NAV does not support the identification of the codes
       *   contained in the files.  As such, this factory only
       *   "produces" L1 C/A tagged data for LNav.
       */
   class RinexNavDataFactory : public NavDataFactoryWithStoreFile
   {
   public:
         /** Fill supportedSignals.
          * @note Currently only GPS nav is supported so only that
          *   will be added to supportedSignals.
          */
      RinexNavDataFactory();

         /** Load RINEX NAV data into a map.
          * @param[in] filename The path of the file to load.
          * @param[out] navMap The map to store the loaded data in.
          * @return true on success, false on failure. */
      bool loadIntoMap(const std::string& filename,
                       NavMessageMap& navMap) override;

         /** Convert RINEX nav data to a system/code-appropriate
          * OrbitData object.
          * @param[in] navIn The RINEX nav message data to convert.
          * @param[out] navOut The OrbitData object to be added to the
          *   factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToOrbit(const Rinex3NavData& navIn,
                                 NavDataPtr& navOut);

         /** Convert RINEX nav data to a system/code-appropriate
          * NavHealthData object.
          * @param[in] navIn The RINEX nav message data to convert.
          * @param[out] healthOut The NavHealthData object to be added
          *   to the factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToHealth(const Rinex3NavData& navIn,
                                  NavDataPtr& healthOut);

         /** Fill the high level (NavData and above) data for an
          * object using information from a RINEX nav record.
          * @param[in] navIn The RINEX nav message data to convert.
          * @param[in,out] navOut A NavDataPtr that has already been
          *   allocated for the appropriate leaf node class
          *   (e.g. GPSLNavEph, GPSLNavHealth, etc.)
          * @post If navIn contains valid, supported data, the
          *   timeStamp and signal data members will be filled in.
          */
      static void fillNavData(const Rinex3NavData& navIn, NavDataPtr& navOut);

         /** Set the xmitTime, Toc and Toe fields in navOut according
          * to the appropriate data in navIn.
          * @param[in] navIn A GPS LNav record in RINEX format.
          * @param[in,out] navOut The GPSLNavEph object whose time
          *   stamps are to be set. */
      static void fixTimeGPS(const Rinex3NavData& navIn, GPSLNavEph& navOut);
   };
}

#endif // GPSTK_RINEXNAVDATAFACTORY_HPP
