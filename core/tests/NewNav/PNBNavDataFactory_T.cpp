#include "PNBNavDataFactory.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


/// Give access to protected members for testing.
class TestClass : public gpstk::PNBNavDataFactory
{
private:
   friend class PNBNavDataFactory_T;
   bool addData(const gpstk::PackedNavBitsPtr& navIn,
                gpstk::NavDataPtrList& navOut)
      override
   { return false; }
   void resetState() override {}
};

class PNBNavDataFactory_T
{
public:
   unsigned constructorTest();
   unsigned setValidityFilterTest();
   unsigned setTypeFilterTest();
};


unsigned PNBNavDataFactory_T ::
constructorTest()
{
   TUDEF("PNBNavDataFactory", "PNBNavDataFactory()");
   TestClass uut;
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::All,
             uut.navValidity);
   TUASSERTE(bool, true, uut.processEph);
   TUASSERTE(bool, true, uut.processAlm);
   TUASSERTE(bool, true, uut.processHea);
   TUASSERTE(bool, true, uut.processTim);
   TURETURN();
}


unsigned PNBNavDataFactory_T ::
setValidityFilterTest()
{
   TUDEF("PNBNavDataFactory", "satValidityFilter()");
   TestClass uut;
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::All,
             uut.navValidity);
   for (gpstk::NavValidityType i : gpstk::NavValidityTypeIterator())
   {
      uut.setValidityFilter(i);
      TUASSERTE(gpstk::NavValidityType, i, uut.navValidity);
   }
   TURETURN();
}


unsigned PNBNavDataFactory_T ::
setTypeFilterTest()
{
   TUDEF("PNBNavDataFactory", "setTypeFilter()");
   TestClass uut;
   TUASSERTE(bool, true,  uut.processEph);
   TUASSERTE(bool, true,  uut.processAlm);
   TUASSERTE(bool, true,  uut.processHea);
   TUASSERTE(bool, true,  uut.processTim);
   uut.setTypeFilter({gpstk::NavMessageType::Almanac});
   TUASSERTE(bool, false, uut.processEph);
   TUASSERTE(bool, true,  uut.processAlm);
   TUASSERTE(bool, false, uut.processHea);
   TUASSERTE(bool, false, uut.processTim);
   uut.setTypeFilter({gpstk::NavMessageType::Ephemeris,
                      gpstk::NavMessageType::TimeOffset});
   TUASSERTE(bool, true,  uut.processEph);
   TUASSERTE(bool, false, uut.processAlm);
   TUASSERTE(bool, false, uut.processHea);
   TUASSERTE(bool, true,  uut.processTim);
   uut.setTypeFilter({gpstk::NavMessageType::Health,
                      gpstk::NavMessageType::Clock});
   TUASSERTE(bool, false, uut.processEph);
   TUASSERTE(bool, false, uut.processAlm);
   TUASSERTE(bool, true,  uut.processHea);
   TUASSERTE(bool, false, uut.processTim);
   uut.setTypeFilter({gpstk::NavMessageType::Clock});
   TUASSERTE(bool, false, uut.processEph);
   TUASSERTE(bool, false, uut.processAlm);
   TUASSERTE(bool, false, uut.processHea);
   TUASSERTE(bool, false, uut.processTim);
   TURETURN();
}


int main()
{
   PNBNavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.setValidityFilterTest();
   errorTotal += testClass.setTypeFilterTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
