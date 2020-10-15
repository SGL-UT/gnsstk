/** @file NavStatic.cpp contains definitions of static data that need
 * to be initialized in a particular order. */

#include "NavMessageType.hpp"
#include "NavDataFactory.hpp"
#include "MultiFormatNavDataFactory.hpp"
#include "RinexNavDataFactory.hpp"
#include "SP3NavDataFactory.hpp"
#include "PNBMultiGNSSNavDataFactory.hpp"
#include "PNBGPSLNavDataFactory.hpp"
#include "PNBGPSCNavDataFactory.hpp"

namespace gpstk
{
      /** @note This must be initialized before instantiating
       * factories and adding them to MultiFormatNavDataFactory. */
   const NavMessageTypeSet allNavMessageTypes { NavMessageType::Almanac,
                                                NavMessageType::Ephemeris,
                                                NavMessageType::TimeOffset,
                                                NavMessageType::Health,
                                                NavMessageType::Clock };

      // Don't try and initialize this, you really need to use
      // addFactory to do it properly.
      /** @note This MUST be defined before the initializer otherwise
       * you'll run into static initialization problems. */
   NavDataFactoryMap MultiFormatNavDataFactory::factories;

      // Don't try and initialize this, you really need to use
      // addFactory to do it properly.
      /** @note This MUST be defined before the initializer otherwise
       * you'll run into static initialization problems. */
   PNBNavDataFactoryMap PNBMultiGNSSNavDataFactory::factories;

      /** This class is not for public use.  It is used to create a
       * singleton object to add supported formats to
       * MultiFormatNavDataFactory.
       * @todo Resolve the memory leak caused by the constructor here. */
   class GPSTKFormatInitializer
   {
   public:
      static bool initialized;
         /// Add gpstk factories to MultiFormatNavDataFactory
      GPSTKFormatInitializer()
      {
         // std::cerr << "starting GPSTKFormatInitializer() " << this << std::endl;
         if (initialized)
            return;
         // std::cerr << "continuing GPSTKFormatInitializer() " << this << std::endl;
         initialized = true;
            // create our factories
            /** @note The order that the factories are added defines
             * their precedence when doing a find in
             * MultiFormatNavDataFactory. */
         gpstk::NavDataFactoryPtr
            ndfp1(std::make_shared<gpstk::SP3NavDataFactory>());
         gpstk::NavDataFactoryPtr
            ndfp2(std::make_shared<gpstk::RinexNavDataFactory>());
            // ignore the return value
         MultiFormatNavDataFactory::addFactory(ndfp1);
         MultiFormatNavDataFactory::addFactory(ndfp2);

         gpstk::PNBNavDataFactoryPtr
            lnav(std::make_shared<gpstk::PNBGPSLNavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GPSLNAV,lnav);

         gpstk::PNBNavDataFactoryPtr
            cnavL2(std::make_shared<gpstk::PNBGPSCNavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GPSCNAVL2,cnavL2);

         gpstk::PNBNavDataFactoryPtr
            cnavL5(std::make_shared<gpstk::PNBGPSCNavDataFactory>());
         PNBMultiGNSSNavDataFactory::addFactory(NavType::GPSCNAVL5,cnavL5);
      }
   };

   bool GPSTKFormatInitializer::initialized = false;
   static GPSTKFormatInitializer gpstkMFNDFI;
}
