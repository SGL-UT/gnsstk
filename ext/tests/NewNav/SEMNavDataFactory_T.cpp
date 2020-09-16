#include "SEMNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavHealth.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

   /** Implement a test class to expose protected members rather than
    * using friends. */
class TestClass : public gpstk::SEMNavDataFactory
{
public:
      /// Grant access to protected data.
   gpstk::NavMessageMap& getData()
   { return data; }
};

/// Automated tests for gpstk::SEMNavDataFactory
class SEMNavDataFactory_T
{
public:
      /// Make sure the constructor does what it's supposed to.
   unsigned constructorTest();
      /// Exercise loadIntoMap by loading data with different options in place.
   unsigned loadIntoMapTest();
      /** Use dynamic_cast to verify that the contents of nmm are the
       * right class.
       * @param[in] testFramework The test framework created by TUDEF,
       *   used by TUASSERT macros in this function.
       * @param[in] nmm The data map to check. */
   template <class NavClass>
   void verifyDataType(gpstk::TestUtil& testFramework,
                       gpstk::NavMessageMap& nmm);
};


unsigned SEMNavDataFactory_T ::
constructorTest()
{
   TUDEF("SEMNavDataFactory", "SEMNavDataFactory");
   gpstk::SEMNavDataFactory fact;
      // check for expected signal support
   gpstk::NavSignalID nsid1(gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::CA,
                            gpstk::NavType::GPSLNAV);
   TUASSERT(fact.supportedSignals.count(nsid1));
   TURETURN();
}


unsigned SEMNavDataFactory_T ::
loadIntoMapTest()
{
   TUDEF("SEMNavDataFactory", "loadIntoMap");

      // test loading SEM 2 nav
   gpstk::SEMNavDataFactory f2;
   std::string f2name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_sem387.txt";
      // this should implicitly load into the data map
   TUASSERT(f2.addDataSource(f2name));
   TUASSERTE(size_t, 60, f2.size());

   TestClass f4;
   std::string f4name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_sem387.txt";
   TUCATCH(f4.setTypeFilter({gpstk::NavMessageType::Almanac}));
      // this should implicitly load into the data map
   TUASSERT(f4.addDataSource(f4name));
   TUASSERTE(size_t, 30, f4.size());
   gpstk::NavMessageMap &nmm4(f4.getData());
      // only one message type
   TUASSERTE(size_t, 1, nmm4.size());
      // and it's almanac.
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Almanac,
             nmm4.begin()->first);
   TUCSM("convertToOrbit/fillNavData");
   verifyDataType<gpstk::GPSLNavAlm>(testFramework, nmm4);
   TUCSM("loadIntoMap");

   TestClass f5;
   std::string f5name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_sem387.txt";
   TUCATCH(f5.setTypeFilter({gpstk::NavMessageType::Health}));
      // this should implicitly load into the data map
   TUASSERT(f5.addDataSource(f5name));
   TUASSERTE(size_t, 30, f5.size());
   gpstk::NavMessageMap &nmm5(f5.getData());
      // only one message type
   TUASSERTE(size_t, 1, nmm5.size());
      // and it's health.
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Health,
             nmm5.begin()->first);
   TUCSM("convertToHealth/fillNavData");
   verifyDataType<gpstk::GPSLNavHealth>(testFramework, nmm5);
   TUCSM("loadIntoMap");

   TestClass f6;
   std::string f6name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_sem387.txt";
   TUCATCH(f6.setTypeFilter({gpstk::NavMessageType::Ephemeris}));
      // this should implicitly load into the data map
   TUASSERT(f6.addDataSource(f6name));
      // except there isn't any data, because SEM nav doesn't contain ephemeris
   TUASSERTE(size_t, 0, f6.size());

      // test loading something that isn't SEM
   TestClass f7;
   std::string f7name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUASSERT(!f7.addDataSource(f7name));
   TUASSERTE(size_t, 0, f7.size());

   TURETURN();
}


template <class NavClass>
void SEMNavDataFactory_T ::
verifyDataType(gpstk::TestUtil& testFramework,
               gpstk::NavMessageMap& nmm)
{
   for (auto& nmti : nmm)
   {
      for (auto& sigi : nmti.second)
      {
         for (auto& sati : sigi.second)
         {
            for (auto& ti : sati.second)
            {
               TUASSERT(dynamic_cast<NavClass*>(ti.second.get()) != nullptr);
            }
         }
      }
   }
}


int main()
{
   SEMNavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.loadIntoMapTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
