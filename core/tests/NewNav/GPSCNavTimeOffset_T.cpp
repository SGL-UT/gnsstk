#include "GPSCNavTimeOffset.hpp"
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


class GPSCNavTimeOffset_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned validateTest();
   unsigned getUserTimeTest();
   unsigned getOffsetTest();
   unsigned getConversionsTest();
};


unsigned GPSCNavTimeOffset_T ::
constructorTest()
{
   TUDEF("GPSCNavTimeOffset", "GPSCNavTimeOffset");
   gpstk::GPSCNavTimeOffset uut;
   TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::Unknown, uut.tgt);
   TUASSERTFE(0.0, uut.a0);
   TUASSERTFE(0.0, uut.a1);
   TUASSERTFE(0.0, uut.a2);
   TUASSERTFE(0.0, uut.deltatLS);
   TUASSERTFE(0.0, uut.tot);
   TUASSERTE(unsigned, 0, uut.wnot);
   TUASSERTE(unsigned, 0, uut.wnLSF);
   TUASSERTE(unsigned, 0, uut.dn);
   TUASSERTFE(0.0, uut.deltatLSF);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::TimeOffset,
             uut.signal.messageType);
   TURETURN();
}


unsigned GPSCNavTimeOffset_T ::
validateTest()
{
   TUDEF("GPSCNavTimeOffset", "validate");
   gpstk::GPSCNavTimeOffset offs;
   TUASSERTE(bool, true, offs.validate());
   offs.tot = 604784.0;
   TUASSERTE(bool, true, offs.validate());
   offs.dn = 7;
   TUASSERTE(bool, true, offs.validate());
   offs.dn = 8;
   TUASSERTE(bool, false, offs.validate());
   offs.dn = 7;
   offs.tot = -0.001;
   TUASSERTE(bool, false, offs.validate());
   offs.tot = 604784.1;
   TUASSERTE(bool, false, offs.validate());
   TURETURN();
}


unsigned GPSCNavTimeOffset_T ::
getUserTimeTest()
{
   TUDEF("GPSCNavTimeOffset", "getUserTime");
   gpstk::GPSCNavTimeOffset uut;
   uut.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime exp(gpstk::GPSWeekSecond(2100,135.0));
      // 1x 12s message
   exp = exp + 12.0;
   TUASSERTE(gpstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNavTimeOffset_T ::
getOffsetTest()
{
   TUDEF("GPSCNavTimeOffset", "getOffset");
   gpstk::GPSCNavTimeOffset uut;
   gpstk::GPSWeekSecond ws1(2060, 405504.0);
   gpstk::GPSWeekSecond ws2(2061, 405504.0);
   uut.tgt = gpstk::TimeSystem::UTC;
   uut.a0 = 1.9790604711E-09;
   uut.a1 = 7.5495165675E-15;
   uut.a2 = 0;
   uut.deltatLS = 18.0;
   uut.tot = 21600.0;
   uut.wnot = 2060;
   double offset;
      /// @todo Truth values here need to be verified.
   TUASSERT(uut.getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC, ws1,
                          offset));
   TUASSERTFE(18.000000004877350079, offset);
   TUASSERT(uut.getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC, ws2,
                          offset));
   TUASSERTFE(18.000000009443297699, offset);
   TURETURN();
}


unsigned GPSCNavTimeOffset_T ::
getConversionsTest()
{
   TUDEF("GPSCNavTimeOffset", "getConversions");
   gpstk::TimeOffsetData::TimeCvtSet convs;
   gpstk::GPSCNavTimeOffset offs;
      // This looks a bit weird, but basically getConversions is
      // expected to return a set containing one key pair for GPS to
      // the target time system, which by default is Unknown.
   gpstk::TimeOffsetData::TimeCvtKey key(gpstk::TimeSystem::GPS,
                                         gpstk::TimeSystem::Unknown);
   TUCATCH(convs = offs.getConversions());
   TUASSERTE(size_t, 1, convs.size());
   TUASSERTE(size_t, 1, convs.count(key));
   TURETURN();
}


int main()
{
   GPSCNavTimeOffset_T testClass;
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
