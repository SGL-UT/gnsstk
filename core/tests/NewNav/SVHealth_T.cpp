#include "SVHealth.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::SVHealth e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class SVHealth_T
{
public:
   unsigned convertTest();
};


unsigned SVHealth_T ::
convertTest()
{
   TUDEF("SVHealth", "asString");
      // This effectively tests SVHealthIterator, asString and
      // asSVHealth all at once.
   for (gpstk::SVHealth e : gpstk::SVHealthIterator())
   {
      TUCSM("asString");
      std::string s(gpstk::StringUtils::asString(e));
      TUASSERT(!s.empty());
      TUASSERT(s != "???");
      TUCSM("asSVHealth");
      gpstk::SVHealth e2 = gpstk::StringUtils::asSVHealth(s);
      TUASSERTE(gpstk::SVHealth, e, e2);
   }
   TURETURN();
}


int main()
{
   SVHealth_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.convertTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
