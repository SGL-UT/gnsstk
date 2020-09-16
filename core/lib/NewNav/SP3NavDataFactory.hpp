#ifndef GPSTK_SP3NAVDATAFACTORY_HPP
#define GPSTK_SP3NAVDATAFACTORY_HPP

#include "NavDataFactoryWithStoreFile.hpp"
#include "SP3Data.hpp"
#include "GPSLNavEph.hpp"

namespace gpstk
{
      /** Factory class for loading SP3 data.
       * @note While in theory Y code and P code LNav are supported,
       *   SP3 does not support the identification of the codes
       *   contained in the files.  As such, this factory only
       *   "produces" L1 C/A tagged data for LNav.
       * @note SP3 does not contain health information.
       */
   class SP3NavDataFactory : public NavDataFactoryWithStoreFile
   {
   public:
         /** Fill supportedSignals.
          * @note Only GPS nav is supported so only that
          *   will be added to supportedSignals.
          */
      SP3NavDataFactory();

         /** Search the store to find the navigation message that
          * meets the specified criteria.  SP3 is only capable of
          * containing ephemeris data.  This implementation of the
          * find method works by finding existing data in the
          * position/velocity tables and returning an OrbitDataSP3
          * that is already interpolated to get the appropriate values
          * at the desired time.
          * @param[in] nmid Specify the message type, satellite and
          *   codes to match.
          * @param[in] when The time of interest to search for data.
          * @param[out] navData The resulting navigation message.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful.  If false, navData will be untouched. */
      bool find(const NavMessageID& nmid, const CommonTime& when,
                NavDataPtr& navData, NavValidityType valid,
                NavSearchOrder order) override;

         /** Load SP3 nav data into a map.
          * @param[in] filename The path of the file to load.
          * @param[out] navMap The map to store the loaded data in.
          * @return true on succes, false on failure. */
      bool loadIntoMap(const std::string& filename,
                       NavMessageMap& navMap) override;

         /** Convert SP3 nav data to a system/code-appropriate
          * OrbitData object.
          * @param[in] navIn The SP3 nav message data to convert.
          * @param[out] navOut The OrbitData object to be added to the
          *   factory data map.
          * @return true if the conversion is valid, false if the
          *   input data is unsupported.
          */
      static bool convertToOrbit(const SP3Data& navIn,
                                 NavDataPtr& navOut);
   private:
      bool store(bool processEph, NavDataPtr& eph);
   };
}

#endif // GPSTK_SP3NAVDATAFACTORY_HPP
