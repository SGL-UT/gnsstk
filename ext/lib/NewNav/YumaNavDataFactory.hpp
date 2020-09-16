#ifndef GPSTK_YUMANAVDATAFACTORY_HPP
#define GPSTK_YUMANAVDATAFACTORY_HPP

#include "NavDataFactoryWithStoreFile.hpp"
#include "YumaData.hpp"
#include "GPSLNavAlm.hpp"

namespace gpstk
{
      /** Factory class for loading Yuma nav data.
       * @note While in theory Y code and P code LNav are supported,
       *   Yuma nav does not support the identification of the codes
       *   contained in the files.  As such, this factory only
       *   "produces" L1 C/A tagged data for LNav.
       */
   class YumaNavDataFactory : public NavDataFactoryWithStoreFile
   {
   public:
         /** Fill supportedSignals.
          * @note Only GPS nav is supported so only that will be added
          *   to supportedSignals.
          */
      YumaNavDataFactory();

         /** Load Yuma nav data into a map.
          * @param[in] filename The path of the file to load.
          * @param[out] navMap The map to store the loaded data in.
          * @return true on succes, false on failure. */
      bool loadIntoMap(const std::string& filename,
                       NavMessageMap& navMap) override;

         /** Convert Yuma nav data to a system/code-appropriate
          * OrbitData object.
          * @param[in] navIn The Yuma nav message data to convert.
          * @param[out] navOut The OrbitData object to be added to the
          *   factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToOrbit(const YumaData& navIn,
                                 NavDataPtr& navOut);

         /** Convert Yuma nav data to a system/code-appropriate
          * NavHealthData object.
          * @param[in] navIn The Yuma nav message data to convert.
          * @param[out] healthOut The NavHealthData object to be added
          *   to the factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToHealth(const YumaData& navIn,
                                  NavDataPtr& healthOut);

         /** Fill the high level (NavData and above) data for an
          * object using information from a Yuma nav record.
          * @param[in] navIn The Yuma nav message data to convert.
          * @param[in,out] navOut A NavDataPtr that has already been
          *   allocated for the appropriate leaf node class
          *   (e.g. GPSLNavEph, GPSLNavHealth, etc.)
          * @post If navIn contains valid, supported data, the
          *   timeStamp and signal data members will be filled in.
          */
      static void fillNavData(const YumaData& navIn, NavDataPtr& navOut);
   };
}

#endif // GPSTK_YUMANAVDATAFACTORY_HPP
