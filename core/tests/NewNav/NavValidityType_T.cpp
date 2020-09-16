#include "NavValidityType.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class NavValidityType_T
{
public:
   unsigned convertTest();
};


unsigned NavValidityType_T ::
convertTest()
{
   TUDEF("NavValidityType", "asString");
      // This effectively tests NavValidityTypeIterator, asString and
      // asNavValidityType all at once.
   for (gpstk::NavValidityType e : gpstk::NavValidityTypeIterator())
   {
      TUCSM("asString");
      std::string s(gpstk::StringUtils::asString(e));
      TUASSERT(!s.empty());
      TUASSERT(s != "???");
      TUCSM("asValidityType");
      gpstk::NavValidityType e2 = gpstk::StringUtils::asNavValidityType(s);
      TUASSERTE(gpstk::NavValidityType, e, e2);
   }
   TURETURN();
}


int main()
{
   NavValidityType_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.convertTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
