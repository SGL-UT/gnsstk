#include "GPSCNavData.hpp"
#include "TestUtil.hpp"

/// Make a non-abstract child class of GPSCNavData for testing
class TestClass : public gpstk::GPSCNavData
{
public:
   gpstk::CommonTime getUserTime() const override
   { return gpstk::CommonTime::BEGINNING_OF_TIME; }
};


class GPSCNavData_T
{
public:
      /// Make sure constructor behavior remains unchanged.
   unsigned constructorTest();
   unsigned validateTest();
};


unsigned GPSCNavData_T ::
constructorTest()
{
   TUDEF("GPSCNavData", "GPSCNavData");
   TestClass obj;
   TUASSERTE(uint32_t, 0, obj.pre);
   TUASSERTE(bool, false, obj.alert);
   TURETURN();
}


unsigned GPSCNavData_T ::
validateTest()
{
   TUDEF("GPSCNavData", "validate");
   TestClass obj;
   TUASSERTE(bool, true, obj.validate());
   obj.pre = 0x22c; // no this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.pre = 0x8b; // but this is
   TUASSERTE(bool, true, obj.validate());
   TURETURN();
}


int main()
{
   GPSCNavData_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.validateTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
