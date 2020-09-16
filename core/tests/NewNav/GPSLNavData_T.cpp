#include "GPSLNavData.hpp"
#include "TestUtil.hpp"

/// Make a non-abstract child class of GPSLNavData for testing
class TestClass : public gpstk::GPSLNavData
{
public:
   gpstk::CommonTime getUserTime() const override
   { return gpstk::CommonTime::BEGINNING_OF_TIME; }
};


class GPSLNavData_T
{
public:
      /// Make sure constructor behavior remains unchanged.
   unsigned constructorTest();
   unsigned validateTest();
};


unsigned GPSLNavData_T ::
constructorTest()
{
   TUDEF("GPSLNavData", "GPSLNavData");
   TestClass obj;
   TUASSERTE(uint32_t, 0, obj.pre);
   TUASSERTE(uint32_t, 0, obj.tlm);
   TUASSERTE(bool, false, obj.asFlag);
   TUASSERTE(bool, false, obj.alert);
   TURETURN();
}


unsigned GPSLNavData_T ::
validateTest()
{
   TUDEF("GPSLNavData", "validate");
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
   GPSLNavData_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.validateTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
