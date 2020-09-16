#include "NavHealthData.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

/// make NavHealthData not abstract
class TestClass : public gpstk::NavHealthData
{
public:
   gpstk::SVHealth getHealth() const override
   { return gpstk::SVHealth::Unhealthy; }
   bool validate() const override
   { return false; }
   gpstk::CommonTime getUserTime() const override
   { return gpstk::CommonTime::END_OF_TIME; }
};


namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class NavHealthData_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
};


unsigned NavHealthData_T ::
constructorTest()
{
   TUDEF("NavHealthData", "NavHealthData");
   TestClass obj;
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Health,
             obj.signal.messageType);
   TURETURN();
}


int main()
{
   NavHealthData_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
