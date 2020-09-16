#include "NavMessageType.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class NavMessageType_T
{
public:
   unsigned convertTest();
};


unsigned NavMessageType_T ::
convertTest()
{
   TUDEF("NavMessageType", "asString");
      // This effectively tests NavMessageTypeIterator, asString and
      // asNavMessageType all at once.
   for (gpstk::NavMessageType e : gpstk::NavMessageTypeIterator())
   {
      TUCSM("asString");
      std::string s(gpstk::StringUtils::asString(e));
      TUASSERT(!s.empty());
      TUASSERT(s != "???");
      TUCSM("asNavMessageType");
      gpstk::NavMessageType e2 = gpstk::StringUtils::asNavMessageType(s);
      TUASSERTE(gpstk::NavMessageType, e, e2);
   }
   TURETURN();
}


int main()
{
   NavMessageType_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.convertTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
