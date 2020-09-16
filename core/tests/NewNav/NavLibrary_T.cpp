#include "NavLibrary.hpp"
#include "RinexNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavHealth.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::SVHealth e)
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

/// Class to expose NavLibrary::factories rather than making a friend.
class TestClass : public gpstk::NavLibrary
{
public:
   gpstk::NavDataFactoryMap& getFactories()
   { return factories; }
};


/// Fake factory used for testing addFactory
class TestFactory : public gpstk::NavDataFactoryWithStore
{
public:
   TestFactory()
   {
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L1,
                                                 gpstk::TrackingCode::CA,
                                                 gpstk::NavType::GPSLNAV));
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L1,
                                                 gpstk::TrackingCode::P,
                                                 gpstk::NavType::GPSLNAV));
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L1,
                                                 gpstk::TrackingCode::Y,
                                                 gpstk::NavType::GPSLNAV));
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L2,
                                                 gpstk::TrackingCode::P,
                                                 gpstk::NavType::GPSLNAV));
      supportedSignals.insert(gpstk::NavSignalID(gpstk::SatelliteSystem::GPS,
                                                 gpstk::CarrierBand::L2,
                                                 gpstk::TrackingCode::Y,
                                                 gpstk::NavType::GPSLNAV));
   }
   gpstk::NavValidityType getValidityFilter() const
   { return navValidity; }
   gpstk::NavMessageTypeSet getTypeFilter() const
   { return procNavTypes; }
   bool addDataSource(const std::string& source) override
   { return false; }
};

class RinexTestFactory : public gpstk::RinexNavDataFactory
{
public:
   gpstk::NavValidityType getValidityFilter() const
   { return navValidity; }
   gpstk::NavMessageTypeSet getTypeFilter() const
   { return procNavTypes; }
};


/// Automated tests for gpstk::NavLibrary
class NavLibrary_T
{
public:
   NavLibrary_T();

      /** Make sure addFactory() puts the factory in each supported
       * signal in the factories map. */
   unsigned addFactoryTest();
      /** Make sure NavLibrary::setTypeFilter updates getTypeFilter() in
       * all factories. */
   unsigned setTypeFilterTest();
      /** Make sure NavLibrary::setValidityFilter updates navValidity
       * in all factories. */
   unsigned setValidityFilterTest();
      /** Make sure that NavLibrary::getXvt pulls the correct
       * ephemeris and computes the correct xvt. */
   unsigned getXvtTest();
   unsigned getHealthTest();

   gpstk::CivilTime civ;
   gpstk::CommonTime ct;
};


NavLibrary_T ::
NavLibrary_T()
      : civ(2015,7,19,2,0,0.0,gpstk::TimeSystem::GPS),
        ct(civ)
{
}


unsigned NavLibrary_T ::
addFactoryTest()
{
   TUDEF("NavLibrary", "addFactory");
   TestClass navLib;
   gpstk::NavDataFactoryPtr ndfp1(std::make_shared<TestFactory>());
   gpstk::NavDataFactoryPtr
      ndfp2(std::make_shared<RinexTestFactory>());
   TUCATCH(navLib.addFactory(ndfp1));
   TUCATCH(navLib.addFactory(ndfp2));
   TestFactory *tfp = dynamic_cast<TestFactory*>(ndfp1.get());
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp2.get());
   TUASSERT(tfp != nullptr);
   TUASSERT(rndfp != nullptr);
   gpstk::NavDataFactoryMap &fm(navLib.getFactories());
      // make sure the factories map has the same number of items
   size_t expTotal = tfp->supportedSignals.size() +
      rndfp->supportedSignals.size();
   TUASSERTE(size_t, expTotal, fm.size());
      // make sure the map has the same set of signals
   gpstk::NavSignalSet allSignals;
   std::set_union(tfp->supportedSignals.begin(), tfp->supportedSignals.end(),
                  rndfp->supportedSignals.begin(),rndfp->supportedSignals.end(),
                  std::inserter(allSignals, allSignals.end()));
   for (const auto& fi : fm)
   {
      TUASSERTE(size_t, 1, allSignals.count(fi.first));
   }
   
   TURETURN();
}


unsigned NavLibrary_T ::
setTypeFilterTest()
{
   TUDEF("NavLibrary", "setTypeFilter");
   gpstk::NavLibrary navLib;
   gpstk::NavDataFactoryPtr ndfp1(std::make_shared<TestFactory>());
   gpstk::NavDataFactoryPtr
      ndfp2(std::make_shared<RinexTestFactory>());
   TUCATCH(navLib.addFactory(ndfp1));
   TUCATCH(navLib.addFactory(ndfp2));
   TestFactory *tfp = dynamic_cast<TestFactory*>(ndfp1.get());
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp2.get());
   TUASSERT(tfp != nullptr);
   TUASSERT(rndfp != nullptr);
   gpstk::NavMessageTypeSet nmts1 { gpstk::NavMessageType::Unknown };
   gpstk::NavMessageTypeSet nmts2 { gpstk::NavMessageType::Ephemeris };
   navLib.setTypeFilter(nmts1);
   TUASSERTE(gpstk::NavMessageTypeSet, nmts1, tfp->getTypeFilter());
   TUASSERTE(gpstk::NavMessageTypeSet, nmts1, rndfp->getTypeFilter());
   navLib.setTypeFilter(nmts2);
   TUASSERTE(gpstk::NavMessageTypeSet, nmts2, tfp->getTypeFilter());
   TUASSERTE(gpstk::NavMessageTypeSet, nmts2, rndfp->getTypeFilter());
   navLib.setTypeFilter(gpstk::allNavMessageTypes);
   TUASSERTE(gpstk::NavMessageTypeSet, gpstk::allNavMessageTypes,
             tfp->getTypeFilter());
   TUASSERTE(gpstk::NavMessageTypeSet, gpstk::allNavMessageTypes,
             rndfp->getTypeFilter());
   TURETURN();
}


unsigned NavLibrary_T ::
setValidityFilterTest()
{
   TUDEF("NavLibrary", "setValidityFilter");
   gpstk::NavLibrary navLib;
   gpstk::NavDataFactoryPtr ndfp1(std::make_shared<TestFactory>());
   gpstk::NavDataFactoryPtr
      ndfp2(std::make_shared<RinexTestFactory>());
   TUCATCH(navLib.addFactory(ndfp1));
   TUCATCH(navLib.addFactory(ndfp2));
   TestFactory *tfp = dynamic_cast<TestFactory*>(ndfp1.get());
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp2.get());
   TUASSERT(tfp != nullptr);
   TUASSERT(rndfp != nullptr);
   navLib.setValidityFilter(gpstk::NavValidityType::ValidOnly);
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::ValidOnly,
             tfp->getValidityFilter());
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::ValidOnly,
             rndfp->getValidityFilter());
   navLib.setValidityFilter(gpstk::NavValidityType::All);
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::All,
             tfp->getValidityFilter());
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::All,
             rndfp->getValidityFilter());
   TURETURN();
}


unsigned NavLibrary_T ::
getXvtTest()
{
   TUDEF("NavLibrary", "getXvt");
   gpstk::Xvt xvt;
   gpstk::NavLibrary navLib;
   gpstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "arlm2000.15n";
   TUCATCH(navLib.addFactory(ndfp));
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   gpstk::NavSatelliteID sat(5, 5, gpstk::SatelliteSystem::GPS,
                             gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                             gpstk::NavType::GPSLNAV);
   TUASSERT(navLib.getXvt(sat, ct+35, xvt));
      // TUASSERTE is not good for this check as we're testing a bunch
      // of floating point numbers, so we use TUSSERTFE instead for
      // each field.
      // @note These were checked against results provided by wheresat
      // while it was still using OrbElemStore.
   TUASSERTFE(  9345531.5274733770639, xvt.x[0]);
   TUASSERTFE(-12408177.088141856715,  xvt.x[1]);
   TUASSERTFE( 21486320.848036296666,  xvt.x[2]);
   TUASSERTFE(2081.276961058104007,    xvt.v[0]);
   TUASSERTFE(1792.4445008638492709,   xvt.v[1]);
   TUASSERTFE( 148.29209115082824155,  xvt.v[2]);
   TUASSERTFE(-0.00021641018042870913346, xvt.clkbias);
   TUASSERTFE(4.3200998334200003381e-12, xvt.clkdrift);
   TUASSERTFE(-8.8197758101551758427e-09, xvt.relcorr);

   TUASSERTE(gpstk::Xvt::HealthStatus, gpstk::Xvt::Healthy, xvt.health);
   TURETURN();
}


unsigned NavLibrary_T ::
getHealthTest()
{
   TUDEF("NavLibrary", "getHealth");
   gpstk::SVHealth health;
   gpstk::NavLibrary navLib;
   gpstk::NavDataFactoryPtr
      ndfp(std::make_shared<RinexTestFactory>());
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "arlm2000.15n";
   TUCATCH(navLib.addFactory(ndfp));
   RinexTestFactory *rndfp =
      dynamic_cast<RinexTestFactory*>(ndfp.get());
   TUASSERT(rndfp->addDataSource(fname));
   gpstk::NavSatelliteID sat(10, 10, gpstk::SatelliteSystem::GPS,
                             gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                             gpstk::NavType::GPSLNAV);
      // shouldn't have data at this time
   TUASSERT(!navLib.getHealth(sat, ct+35, health));
      // shouldn't have data at this time, either
      // Remember that the first line of a RINEX 2 nav like we use in
      // this test is the Toc, NOT the transmit time.
   gpstk::CivilTime civ2(2015,7,19,12,35,35.0,gpstk::TimeSystem::GPS);
   TUASSERT(!navLib.getHealth(sat, civ2, health));
      // should have data at this time, and it should be unhealthy
   gpstk::CivilTime civ3(2015,7,19,12,35,36.0,gpstk::TimeSystem::GPS);
   TUASSERT(navLib.getHealth(sat, civ3, health));
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unhealthy, health);
      // should have this data and healthy
   gpstk::NavSatelliteID sat4(2, 2, gpstk::SatelliteSystem::GPS,
                              gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                              gpstk::NavType::GPSLNAV);
   gpstk::CivilTime civ4(2015,7,19,2,0,0.0,gpstk::TimeSystem::GPS);
   TUASSERT(navLib.getHealth(sat4, civ4, health));
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, health);
   TURETURN();
}


int main()
{
   NavLibrary_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.addFactoryTest();
   errorTotal += testClass.setTypeFilterTest();
   errorTotal += testClass.setValidityFilterTest();
   errorTotal += testClass.getXvtTest();
   errorTotal += testClass.getHealthTest();
   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
