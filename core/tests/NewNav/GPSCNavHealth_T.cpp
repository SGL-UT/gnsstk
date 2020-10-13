#include "GPSCNavHealth.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gpstk::SVHealth e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GPSCNavHealth_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned getHealthTest();
};


unsigned GPSCNavHealth_T ::
constructorTest()
{
   TUDEF("GPSCNavHealth", "GPSCNavHealth");
   gpstk::GPSCNavHealth uut;
   TUASSERTE(uint8_t, true, uut.health);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Health,
             uut.signal.messageType);
   TURETURN();
}


unsigned GPSCNavHealth_T ::
getUserTimeTest()
{
   TUDEF("GPSCNavHealth", "getUserTime");
   gpstk::GPSCNavHealth uut;
   uut.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime exp(gpstk::GPSWeekSecond(2100,135.0));
      // 1x 12s message
   exp = exp + 12.0;
   TUASSERTE(gpstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNavHealth_T ::
getHealthTest()
{
   TUDEF("GPSCNavHealth", "getHealth");
   gpstk::GPSCNavHealth uut;
      // default should be unhealthy
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unhealthy, uut.getHealth());
      // the one condition that should result in healthy
   uut.health = false;
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, uut.getHealth());
   TURETURN();
}


int main()
{
   GPSCNavHealth_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getHealthTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
