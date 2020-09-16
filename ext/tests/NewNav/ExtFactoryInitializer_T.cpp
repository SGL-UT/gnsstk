#include "MultiFormatNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "SEMNavDataFactory.hpp"
#include "YumaNavDataFactory.hpp"

class TestClass : public gpstk::MultiFormatNavDataFactory
{
public:
   static gpstk::NavDataFactoryMap& getFactories()
   { return factories; }
};


class ExtFactoryInitializer_T
{
public:
      /** Make sure that the MultiFormatNavDataFactory contains the
       * expected factory objects. */
   unsigned constructorTest();
};


unsigned ExtFactoryInitializer_T ::
constructorTest()
{
   TUDEF("ExtFactoryInitializer", "ExtFactoryInitializer");
   bool foundYuma = false;
   bool foundSEM = false;
      // Iterate over the factories and try to find the expected
      // factory objects.  The factories map may also contain ext
      // factories, so we ignore anything else.
   for (auto& i : TestClass::getFactories())
   {
      gpstk::NavDataFactory *p = i.second.get();
      if (dynamic_cast<gpstk::YumaNavDataFactory*>(p) != nullptr)
      {
         foundYuma = true;
      }
      else if (dynamic_cast<gpstk::SEMNavDataFactory*>(p) != nullptr)
      {
         foundSEM = true;
      }
   }
   TUASSERT(foundYuma);
   TUASSERT(foundSEM);
   TURETURN();
}


int main()
{
   ExtFactoryInitializer_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
