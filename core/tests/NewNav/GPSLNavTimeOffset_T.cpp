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
   unsigned validateTest();
   unsigned getUserTimeTest();
   unsigned getOffsetTest();
   unsigned getConversionsTest();
};


unsigned GPSLNavTimeOffset_T ::
constructorTest()
{
   TUDEF("GPSLNavTimeOffset", "GPSLNavTimeOffset");
   gpstk::GPSLNavTimeOffset obj;
   TUASSERTFE(0.0, obj.deltatLS);
   TUASSERTFE(0.0, obj.a0);
   TUASSERTFE(0.0, obj.a1);
   TUASSERTFE(0.0, obj.tot);
//   TUASSERTE(unsigned, 0, obj.wn);
   TUASSERTE(unsigned, 0, obj.wnt);
   TUASSERTE(unsigned, 0, obj.wnLSF);
   TUASSERTE(unsigned, 0, obj.dn);
   TUASSERTFE(0.0, obj.deltatLSF);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::TimeOffset,
             obj.signal.messageType);
   TURETURN();
}


unsigned GPSLNavTimeOffset_T ::
validateTest()
{
   TUDEF("GPSLNavTimeOffset", "validate");
   gpstk::GPSLNavTimeOffset offs;
   TUASSERTE(bool, true, offs.validate());
   offs.tot = 602112.0;
   TUASSERTE(bool, true, offs.validate());
   offs.dn = 7;
   TUASSERTE(bool, true, offs.validate());
   offs.dn = 8;
   TUASSERTE(bool, false, offs.validate());
   offs.dn = 7;
   offs.tot = -0.001;
   TUASSERTE(bool, false, offs.validate());
   offs.tot = 602112.1;
   TUASSERTE(bool, false, offs.validate());
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


unsigned GPSLNavTimeOffset_T ::
getConversionsTest()
{
   TUDEF("GPSLNavTimeOffset", "getConversions");
   gpstk::TimeOffsetData::TimeCvtSet convs;
   gpstk::GPSLNavTimeOffset offs;
   gpstk::TimeOffsetData::TimeCvtKey key1(gpstk::TimeSystem::GPS,
                                          gpstk::TimeSystem::UTC);
   gpstk::TimeOffsetData::TimeCvtKey key2(gpstk::TimeSystem::UTC,
                                          gpstk::TimeSystem::GPS);
   TUCATCH(convs = offs.getConversions());
   TUASSERTE(size_t, 2, convs.size());
   TUASSERTE(size_t, 1, convs.count(key1));
   TUASSERTE(size_t, 1, convs.count(key2));
   TURETURN();
}


int main()
{
   GPSLNavTimeOffset_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.validateTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getOffsetTest();
   errorTotal += testClass.getConversionsTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
