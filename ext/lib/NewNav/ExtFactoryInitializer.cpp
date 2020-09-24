#include "MultiFormatNavDataFactory.hpp"
#include "YumaNavDataFactory.hpp"
#include "SEMNavDataFactory.hpp"

namespace gpstk
{
      /** This class is not for public use and thus does not appear in
       * any include file.  It is used to create a singleton object to
       * add supported formats to MultiFormatNavDataFactory.
       * @todo Resolve the memory leak caused by the constructor here. */
   class ExtFactoryInitializer
   {
   public:
      static bool initialized;
         /// Add gpstk ext factories to MultiFormatNavDataFactory
      ExtFactoryInitializer()
      {
         if (initialized)
            return;
         std::cerr << "ExtFactoryInitializer() " << this << std::endl;
         initialized = true;
            // create our factories
         gpstk::NavDataFactoryPtr
            ndfp1(std::make_shared<gpstk::YumaNavDataFactory>());
         gpstk::NavDataFactoryPtr
            ndfp2(std::make_shared<gpstk::SEMNavDataFactory>());
            // ignore the return value
         MultiFormatNavDataFactory::addFactory(ndfp1);
         MultiFormatNavDataFactory::addFactory(ndfp2);
      }
   };

   bool ExtFactoryInitializer::initialized = false;
   static ExtFactoryInitializer gpstkEFI;
}
