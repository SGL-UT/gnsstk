#include "NavMessageID.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class NavMessageID_T
{
public:
   unsigned constructorTest();
};


unsigned NavMessageID_T ::
constructorTest()
{
   TUDEF("NavMessageID", "NavMessageID");
      // only one thing to check
   gpstk::NavMessageID obj;
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Unknown,
             obj.messageType);
   TURETURN();
}


int main()
{
   NavMessageID_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
