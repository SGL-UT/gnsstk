#ifndef GPSTK_NAVDATAFACTORY_HPP
#define GPSTK_NAVDATAFACTORY_HPP

#include <memory>
#include <map>
#include "NavSignalID.hpp"
#include "CommonTime.hpp"
#include "NavData.hpp"
#include "NavValidityType.hpp"
#include "NavMessageID.hpp"
#include "NavSearchOrder.hpp"
#include "SVHealth.hpp"

namespace gpstk
{
      /// @ingroup NavFactory
      //@{

      /** Abstract base class that defines the interface for searching
       * for navigation data. */
   class NavDataFactory
   {
   public:
         /** Initialize navValidity to All and procNavTypes to all
          * known message types. */
      NavDataFactory()
            : navValidity(NavValidityType::All),
              procNavTypes(allNavMessageTypes)
      {}

         /** Search the store as defined by each child class of
          * NavDataFactory to find the navigation message that meets
          * the specified criteria.
          * @param[in] nmid Specify the message type, satellite and
          *   codes to match.
          * @param[in] when The time of interest to search for data.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[out] navData The resulting navigation message.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful.  If false, navData will be untouched. */
      virtual bool find(const NavMessageID& nmid, const CommonTime& when,
                        NavDataPtr& navData, SVHealth xmitHealth,
                        NavValidityType valid, NavSearchOrder order) = 0;

         /** Get the offset, in seconds, to apply to times when
          * converting them from fromSys to toSys.
          * @param[in] fromSys The time system to convert from.
          * @param[in] toSys The time system to convert to.
          * @param[in] when The time being converted, usually in the
          *   time system appropriate for a given nav message source.
          *   The details of what time system this should be in and
          *   any other restrictions will be documented in each leaf
          *   class, e.g. GPSLNavTimeOffset.
          * @param[out] offset The offset when converting fromSys->toSys.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if an offset is available, false if not. */
      virtual bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                             const CommonTime& when, double& offset,
                             SVHealth xmitHealth = SVHealth::Any,
                             NavValidityType valid = NavValidityType::ValidOnly,
                             NavSearchOrder order = NavSearchOrder::User) = 0;

         /** Set the factory's handling of valid and invalid
          * navigation data.  This should be called before any find()
          * calls.
          * @param[in] nvt The new nav data loading filter method. */
      virtual void setValidityFilter(NavValidityType nvt)
      { navValidity = nvt; }

         /** Indicate what nav message types the factories should be
          * loading.  This should be called before the factories
          * acquire any data.
          * @param[in] nmts The set of nav message types to be
          *   processed by the factories. */
      virtual void setTypeFilter(const NavMessageTypeSet& nmts)
      { procNavTypes = nmts; }

         /** Add a data source for this factory.  This could be a file
          * name, an IP address and port, or something else entirely,
          * depending on the factory implementation.  This method is
          * declared here so that NavDataFactoryPtr objects can be set
          * up without having to do any casting of pointers.
          * @param[in] source A string describing the source of data
          *   for the factory.
          * @return true If the factory was able to process the data
          *   in source. */
      virtual bool addDataSource(const std::string& source) = 0;

         /** Print the contents of this factory in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      virtual void dump(std::ostream& s, NavData::Detail dl) const
      {}

         /** Define which signals this factory supports.  This will be
          * empty by default, which means that NavLibrary would not
          * use this factory, so it is up to the derived classes to
          * fill out the signals as appropriate. */
      NavSignalSet supportedSignals;

   protected:
         /** Determines how the factory should load nav data from the store.
          * @note There's nothing to prevent someone from doing
          *   something silly like setting this to "InvalidOnly" then
          *   doing a find for "ValidOnly" which of course will result
          *   in no results. */
      NavValidityType navValidity;

         /** Determines which types of navigation message data the
          * factory should be processing. */
      NavMessageTypeSet procNavTypes;
   };

      /// Managed pointer to NavDataFactory.
   using NavDataFactoryPtr = std::shared_ptr<NavDataFactory>;
      /// Map signal to a factory.
   using NavDataFactoryMap = std::multimap<NavSignalID, NavDataFactoryPtr>;

      //@}

}

#endif // GPSTK_NAVDATAFACTORY_HPP
