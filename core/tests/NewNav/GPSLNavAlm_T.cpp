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
};


unsigned GPSLNavAlm_T ::
constructorTest()
{
   TUDEF("GPSLNavAlm", "GPSLNavAlm");
   gpstk::GPSLNavAlm obj;
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Almanac,
             obj.signal.messageType);
   TURETURN();
}


unsigned GPSLNavAlm_T ::
getUserTimeTest()
{
   TUDEF("GPSLNavAlm", "getUserTime");
   gpstk::GPSLNavAlm obj;
   obj.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   gpstk::CommonTime exp(gpstk::GPSWeekSecond(2100,135.0));
      // almanac = 1 subframes * 6 seconds
   exp = exp + 6.0;
   TUASSERTE(gpstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}


int main()
{
   GPSLNavAlm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
