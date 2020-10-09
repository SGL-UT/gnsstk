#ifndef GPSTK_NAVLIBRARY_HPP
#define GPSTK_NAVLIBRARY_HPP

#include "NavDataFactory.hpp"
#include "Xvt.hpp"
#include "SVHealth.hpp"

namespace gpstk
{
      /** @defgroup NavFactory Navigation Message Decoding and Finding
       * @todo write something!
       */

      /// @ingroup NavFactory
      //@{

      /** Provide a high-level interface for finding navigation data.
       * Simplified example:
       * \code
       * NavLibrary navLib;
       * NavDataFactoryPtr ndfp(std::make_shared<MultiFormatNavDataFactory>());
       * navLib.addFactory(ndfp);
       * if (!ndfp->addDataSource(filename))
       *    die("could not load " + filename);
       * NavSatelliteID sat(...);
       * CommonTime when(...);
       * Xvt xvt;
       * if (navLib.getXvt(sat,when,xvt))
       *    doSomething(xvt);
       * \endcode
       */
   class NavLibrary
   {
   public:
         /** Get the position and velocity of a satellite at a specific time.
          * @param[in] sat Satellite to get the position/velocity for.
          * @param[in] when The time that the position should be computed for.
          * @param[out] xvt The computed position and velocity at when.
          * @param[in] useAlm If true, search for and use almanac
          *   orbital elements.  If false, search for and use
          *   ephemeris data instead.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful, false if no nav data was found
          *   to compute the Xvt. */
      bool getXvt(const NavSatelliteID& sat, const CommonTime& when,
                  Xvt& xvt, bool useAlm, SVHealth xmitHealth = SVHealth::Any,
                  NavValidityType valid = NavValidityType::ValidOnly,
                  NavSearchOrder order = NavSearchOrder::User);

         /** Get the position and velocity of a satellite at a
          * specific time.  This implementation differs from the other
          * getXvt method in that it will first attempt to find a
          * matching ephemeris, and if that fails, it will attempt to
          * search for a matching almanac, as opposed to one or the other.
          * @param[in] sat Satellite to get the position/velocity for.
          * @param[in] when The time that the position should be computed for.
          * @param[out] xvt The computed position and velocity at when.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful, false if no nav data was found
          *   to compute the Xvt. */
      bool getXvt(const NavSatelliteID& sat, const CommonTime& when,
                  Xvt& xvt, SVHealth xmitHealth = SVHealth::Any,
                  NavValidityType valid = NavValidityType::ValidOnly,
                  NavSearchOrder order = NavSearchOrder::User);

         /** Get the health status of a satellite at a specific time.
          * @param[in] sat Satellite to get the health status for.
          * @param[in] when The time that the health should be retrieved.
          * @param[out] health The health status at when.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful, false if no nav data was found
          *   containing health status. */
      bool getHealth(const NavSatelliteID& sat, const CommonTime& when,
                     SVHealth& health, SVHealth xmitHealth = SVHealth::Any,
                     NavValidityType valid = NavValidityType::ValidOnly,
                     NavSearchOrder order = NavSearchOrder::User);

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
      bool getOffset(TimeSystem fromSys, TimeSystem toSys,
                     const CommonTime& when, double& offset,
                     SVHealth xmitHealth = SVHealth::Any,
                     NavValidityType valid = NavValidityType::ValidOnly,
                     NavSearchOrder order = NavSearchOrder::User);

         /** Search factories to find the navigation message that meets
          * the specified criteria.
          * @note returns the first successful match from factories.
          * @param[in] nmid Specify the message type, satellite and
          *   codes to match.
          * @param[in] when The time of interest to search for data.
          * @param[out] navData The resulting navigation message.
          * @param[in] xmitHealth The desired health status of the
          *   transmitting satellite.
          * @param[in] valid Specify whether to search only for valid
          *   or invalid messages, or both.
          * @param[in] order Specify whether to search by receiver
          *   behavior or by nearest to when in time. 
          * @return true if successful.  If false, navData will be untouched. */
      bool find(const NavMessageID& nmid, const CommonTime& when,
                NavDataPtr& navData, SVHealth xmitHealth, NavValidityType valid,
                NavSearchOrder order);

         /** Set the factories' handling of valid and invalid
          * navigation data.  This should be called before any find()
          * calls.
          * @param[in] nvt The new nav data loading filter method. */
      void setValidityFilter(NavValidityType nvt);

         /** Indicate what nav message types the factories should be
          * loading.  This should be called before the factories
          * acquire any data.
          * @param[in] nmts The set of nav message types to be
          *   processed by the factories. */
      void setTypeFilter(const NavMessageTypeSet& nmts);

         /** Add a new factory to the library.
          * @param[in] fact The NavDataFactory object to add to the library.
          */
      void addFactory(NavDataFactoryPtr& fact);

         /** Print the contents of all factories in a human-readable
          * format.
          * @param[in,out] s The stream to write the data to.
          * @param[in] dl The level of detail the output should contain. */
      void dump(std::ostream& s, NavData::Detail dl) const;

   protected:
         /** Known nav data factories, organized by signal to make
          * searches simpler and/or quicker. */
      NavDataFactoryMap factories;
   };

      //@}

}

#endif // GPSTK_NAVLIBRARY_HPP
