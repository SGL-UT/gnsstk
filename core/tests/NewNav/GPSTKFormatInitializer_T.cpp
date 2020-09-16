#include "MultiFormatNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "SP3NavDataFactory.hpp"
#include "RinexNavDataFactory.hpp"

class TestClass : public gpstk::MultiFormatNavDataFactory
{
public:
   static gpstk::NavDataFactoryMap& getFactories()
   { return factories; }
};


class GPSTKFormatInitializer_T
{
public:
      /** Make sure that the MultiFormatNavDataFactory contains the
       * expected factory objects. */
   unsigned constructorTest();
};


unsigned GPSTKFormatInitializer_T ::
constructorTest()
{
   TUDEF("GPSTKFormatInitializer", "GPSTKFormatInitializer");
   bool foundRinex = false;
   bool foundSP3 = false;
      // Iterate over the factories and try to find the expected
      // factory objects.  The factories map may also contain ext
      // factories, so we ignore anything else.
   for (auto& i : TestClass::getFactories())
   {
      gpstk::NavDataFactory *p = i.second.get();
      if (dynamic_cast<gpstk::RinexNavDataFactory*>(p) != nullptr)
      {
         foundRinex = true;
      }
      else if (dynamic_cast<gpstk::SP3NavDataFactory*>(p) != nullptr)
      {
         foundSP3 = true;
      }
   }
   TUASSERT(foundRinex);
   TUASSERT(foundSP3);
   TURETURN();
}


int main()
{
   GPSTKFormatInitializer_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
