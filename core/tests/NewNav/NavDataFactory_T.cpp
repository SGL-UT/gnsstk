#include "NavDataFactory.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s,
                            const gpstk::NavMessageTypeSet& nmts)
   {
      s << "{";
      for (const auto& i : nmts)
      {
         s << " " << gpstk::StringUtils::asString(i);
      }
      s << " }";
      return s;
   }
}

/// Make a non-abstract class derived from NavDataFactory so we can test it
class TestClass : public gpstk::NavDataFactory
{
public:
   bool find(const gpstk::NavMessageID& nmid, const gpstk::CommonTime& when,
             gpstk::NavDataPtr& navData, gpstk::SVHealth xmitHealth,
             gpstk::NavValidityType valid, gpstk::NavSearchOrder order) override
   { return false; }
   bool getOffset(gpstk::TimeSystem fromSys, gpstk::TimeSystem toSys,
                  const gpstk::CommonTime& when, double& offset,
                  gpstk::SVHealth xmitHealth, gpstk::NavValidityType valid,
                  gpstk::NavSearchOrder order) override
   { return false; }
   bool addDataSource(const std::string& source) override
   { return false; }
   gpstk::NavValidityType getValidityFilter() const
   { return navValidity; }
   gpstk::NavMessageTypeSet getTypeFilter() const
   { return procNavTypes; }
};


class NavDataFactory_T
{
public:
   unsigned constructorTest();
   unsigned setValidityFilterTest();
   unsigned setTypeFilterTest();
};


unsigned NavDataFactory_T ::
constructorTest()
{
   TUDEF("NavDataFactory", "NavDataFactory");
   TestClass obj;
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::Any,
             obj.getValidityFilter());
   TUASSERTE(gpstk::NavMessageTypeSet, gpstk::allNavMessageTypes,
             obj.getTypeFilter());
      // nothing should be in supportedSignals at this level.
   TUASSERT(obj.supportedSignals.empty());
   TURETURN();
}


unsigned NavDataFactory_T ::
setValidityFilterTest()
{
   TUDEF("NavDataFactory", "setValidityFilter");
   TestClass obj;
   obj.setValidityFilter(gpstk::NavValidityType::ValidOnly);
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::ValidOnly,
             obj.getValidityFilter());
   obj.setValidityFilter(gpstk::NavValidityType::InvalidOnly);
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::InvalidOnly,
             obj.getValidityFilter());
   TURETURN();
}


unsigned NavDataFactory_T ::
setTypeFilterTest()
{
   TUDEF("NavDataFactory", "setTypeFilter");
   TestClass obj;
      // some random examples
   gpstk::NavMessageTypeSet s1 { gpstk::NavMessageType::Almanac,
                                 gpstk::NavMessageType::Health };
   gpstk::NavMessageTypeSet s2 { gpstk::NavMessageType::Ephemeris };
   gpstk::NavMessageTypeSet s3 { gpstk::NavMessageType::Ephemeris,
                                 gpstk::NavMessageType::TimeOffset,
                                 gpstk::NavMessageType::Health };
   obj.setTypeFilter(s1);
   TUASSERTE(gpstk::NavMessageTypeSet, s1, obj.getTypeFilter());
   obj.setTypeFilter(s2);
   TUASSERTE(gpstk::NavMessageTypeSet, s2, obj.getTypeFilter());
   obj.setTypeFilter(s3);
   TUASSERTE(gpstk::NavMessageTypeSet, s3, obj.getTypeFilter());
   TURETURN();
}


int main()
{
   NavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.setValidityFilterTest();
   errorTotal += testClass.setTypeFilterTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
