#ifndef GPSTK_NAVDATAFACTORYWITHSTOREFILE_HPP
#define GPSTK_NAVDATAFACTORYWITHSTOREFILE_HPP

#include "NavDataFactoryWithStore.hpp"

namespace gpstk
{
      /// Define an interface for loading nav data from a file
   class NavDataFactoryWithStoreFile : public NavDataFactoryWithStore
   {
   public:
         /** Load a file into the default map, NavDataFactoryWithStore::data.
          * @param[in] source The path to the file to load.
          * @return true on success, false on failure. */
      bool addDataSource(const std::string& source) override
      { return loadIntoMap(source, data); }

         /** Abstract method that should be overridden by specific
          * file-reading factory classes in order to load the data
          * into the map.
          * @note When implementing this method, make sure to handle
          *   validity checks!
          * @param[in] filename The path of the file to load.
          * @param[out] navMap The map to store the loaded data in.
          * @return true on success, false on failure. */
      virtual bool loadIntoMap(const std::string& filename,
                               NavMessageMap& navMap) = 0;
   };
}

#endif // GPSTK_NAVDATAFACTORYWITHSTOREFILE_HPP
