#include "GPSLNavTimeOffset.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class GPSLNavTimeOffset_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned getOffsetTest();
};


unsigned GPSLNavTimeOffset_T ::
constructorTest()
{
   TUDEF("GPSLNavTimeOffset", "GPSLNavTimeOffset");
   gpstk::GPSLNavTimeOffset obj;
   TUASSERTE(double, 0.0, obj.deltatLS);
   TUASSERTE(double, 0.0, obj.a0);
   TUASSERTE(double, 0.0, obj.a1);
   TUASSERTE(double, 0.0, obj.tot);
//   TUASSERTE(unsigned, 0, obj.wn);
   TUASSERTE(unsigned, 0, obj.wnt);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::TimeOffset,
             obj.signal.messageType);
   TURETURN();
}


unsigned GPSLNavTimeOffset_T ::
getUserTimeTest()
{
   TUDEF("GPSLNavTimeOffset", "getUserTime");
   gpstk::GPSLNavTimeOffset obj;
   obj.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime exp(gpstk::GPSWeekSecond(2100,135.0));
      // 1x 6s subframe
   exp = exp + 6.0;
   TUASSERTE(gpstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}


unsigned GPSLNavTimeOffset_T ::
getOffsetTest()
{
   TUDEF("GPSLNavTimeOffset", "getOffset");
   gpstk::GPSLNavTimeOffset obj;
   gpstk::GPSWeekSecond ws1(2092, 405504.0);
   gpstk::GPSWeekSecond ws2(2093, 405504.0);
   obj.deltatLS = 18.0;
   obj.a0 = -2.79396772E-09;
   obj.a1 = -7.10542736E-15;
   obj.tot = 405504.0;
   obj.wnt = 2092;
   double offset;
      /// @todo Truth values here need to be verified.
   TUASSERT(obj.getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC, ws1,
                          offset));
   TUASSERTFE(17.99999999720603227615, offset);
   TUASSERT(obj.getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC, ws2,
                          offset));
   TUASSERTFE(17.99999999290866981028, offset);
   TURETURN();
}


int main()
{
   GPSLNavTimeOffset_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getOffsetTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
