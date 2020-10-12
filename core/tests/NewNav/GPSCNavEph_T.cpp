#include "GPSCNavEph.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gpstk::GPSCNavEph::L2Codes e)
   {
      s << static_cast<long>(e);
      return s;
   }
}

class GPSCNavEph_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned validateTest();
};


unsigned GPSCNavEph_T ::
constructorTest()
{
   TUDEF("GPSCNavEph", "GPSCNavEph");
   gpstk::GPSCNavEph obj;
   TUASSERTE(uint32_t, 0, obj.pre11);
   TUASSERTE(uint32_t, 0, obj.preClk);
   TUASSERTE(bool, true, obj.healthL1);
   TUASSERTE(bool, true, obj.healthL2);
   TUASSERTE(bool, true, obj.healthL5);
   TUASSERTE(int8_t, -16, obj.uraED);
   TUASSERTE(int8_t, -16, obj.uraNED0);
   TUASSERTE(uint8_t, 0, obj.uraNED1);
   TUASSERTE(uint8_t, 0, obj.uraNED2);
   TUASSERTE(bool, false, obj.alert11);
   TUASSERTE(bool, false, obj.alertClk);
   TUASSERTE(bool, false, obj.integStat);
   TUASSERTE(bool, false, obj.phasingL2C);
   TUASSERTFE(0, obj.deltaA);
   TUASSERTFE(0, obj.dOMEGAdot);
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime::BEGINNING_OF_TIME, obj.top);
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime::BEGINNING_OF_TIME,
             obj.xmit11);
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime::BEGINNING_OF_TIME,
             obj.xmitClk);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             obj.signal.messageType);
   TURETURN();
}


unsigned GPSCNavEph_T ::
getUserTimeTest()
{
   TUDEF("GPSCNavEph", "getUserTime");
   gpstk::GPSCNavEph obj;
      // just using made-up numbers
   obj.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   obj.xmitTime = gpstk::GPSWeekSecond(2100,139.0);
   obj.xmit11 = gpstk::GPSWeekSecond(2100,200.0);
   obj.xmitClk = gpstk::GPSWeekSecond(2100,1.0);
   gpstk::CommonTime exp(gpstk::GPSWeekSecond(2100,212.0));
   TUASSERTE(gpstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}


unsigned GPSCNavEph_T ::
fixFitTest()
{
   TUDEF("GPSCNavEph", "fixFit");
   gpstk::GPSCNavEph obj;
   gpstk::GPSWeekSecond beginExpWS2(2059, 597600), endExpWS2(2060, 3600);
   gpstk::CommonTime beginExp2(beginExpWS2), endExp2(endExpWS2);
   obj.Toe = obj.Toc = gpstk::GPSWeekSecond(2059, 603000);
   obj.xmitTime = gpstk::GPSWeekSecond(2059,597600);
   TUCATCH(obj.fixFit());
   TUASSERTE(gpstk::CommonTime, beginExp2, obj.beginFit);
   TUASSERTE(gpstk::CommonTime, endExp2, obj.endFit);
   obj.dump(std::cerr, gpstk::OrbitDataKepler::Detail::Full);
   TURETURN();
}


unsigned GPSCNavEph_T ::
validateTest()
{
   TUDEF("GPSCNavData", "validate");
   gpstk::GPSCNavEph obj;
   TUASSERTE(bool, true, obj.validate());
   obj.pre = 0x22c; // this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.pre = 0x8b; // this is valid
   obj.pre11 = 0x22c; // this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.pre11 = 0x8b; // this is valid
   obj.preClk = 0x22c; // this is not valid
   TUASSERTE(bool, false, obj.validate());
   obj.preClk = 0x8b; // this is valid (all 3 preambles == 8b)
   TUASSERTE(bool, true, obj.validate());
   TURETURN();
}


int main()
{
   GPSCNavEph_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.validateTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
