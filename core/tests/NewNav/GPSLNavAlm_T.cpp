#include "GPSLNavAlm.hpp"
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

class GPSLNavAlm_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
};


unsigned GPSLNavAlm_T ::
constructorTest()
{
   TUDEF("GPSLNavAlm", "GPSLNavAlm");
   gpstk::GPSLNavAlm uut;
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Almanac,
             uut.signal.messageType);
   TUASSERTE(uint8_t, 0xff, uut.healthBits);
   TURETURN();
}


unsigned GPSLNavAlm_T ::
getUserTimeTest()
{
   TUDEF("GPSLNavAlm", "getUserTime");
   gpstk::GPSLNavAlm uut;
   uut.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime exp(gpstk::GPSWeekSecond(2100,135.0));
      // almanac = 1 subframes * 6 seconds
   exp = exp + 6.0;
   TUASSERTE(gpstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GPSLNavAlm_T ::
fixFitTest()
{
   TUDEF("GPSLNavAlm", "fixFit");
   gpstk::CommonTime toa = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime expBegin = toa - (70.0 * 3600.0);
   gpstk::CommonTime expEnd   = toa + (74.0 * 3600.0);
   gpstk::GPSLNavAlm uut;
   uut.Toe = toa;
   TUCATCH(uut.fixFit());
   TUASSERTE(gpstk::CommonTime, expBegin, uut.beginFit);
   TUASSERTE(gpstk::CommonTime, expEnd, uut.endFit);
   TURETURN();
}


int main()
{
   GPSLNavAlm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
