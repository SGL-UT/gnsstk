#include "SP3NavDataFactory.hpp"
#include "TestUtil.hpp"
#include "OrbitDataSP3.hpp"
#include "CivilTime.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::GPSLNavEph::L2Codes e)
   {
      s << static_cast<long>(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::SatelliteSystem e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::CarrierBand e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::TrackingCode e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::NavType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

   /** Implement a test class to expose protected members rather than
    * using friends. */
class TestClass : public gpstk::SP3NavDataFactory
{
public:
      /// Grant access to protected data.
   gpstk::NavMessageMap& getData()
   { return data; }
};

/// Automated tests for gpstk::SP3NavDataFactory
class SP3NavDataFactory_T
{
public:
      /// Make sure the constructor does what it's supposed to.
   unsigned constructorTest();
      /// Exercise loadIntoMap by loading data with different options in place.
   unsigned loadIntoMapTest();
      /// Test find where the time stamps exactly match epochs in source data.
   unsigned findExactTest();
      /// Test find where interpolation is required.
   unsigned findInterpTest();
      /** Use dynamic_cast to verify that the contents of nmm are the
       * right class.
       * @param[in] testFramework The test framework created by TUDEF,
       *   used by TUASSERT macros in this function.
       * @param[in] nmm The data map to check. */
   template <class NavClass>
   void verifyDataType(gpstk::TestUtil& testFramework,
                       gpstk::NavMessageMap& nmm);
};


unsigned SP3NavDataFactory_T ::
constructorTest()
{
   TUDEF("SP3NavDataFactory", "SP3NavDataFactory");
   gpstk::SP3NavDataFactory fact;
      // check for expected signal support
   gpstk::NavSignalID nsid1(gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::CA,
                            gpstk::NavType::GPSLNAV);
   TUASSERT(fact.supportedSignals.count(nsid1));
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
loadIntoMapTest()
{
   TUDEF("SP3NavDataFactory", "loadIntoMap");

      // test loading SP3a nav
   gpstk::SP3NavDataFactory f2;
   std::string f2name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3a.sp3";
      // this should implicitly load into the data map
   TUASSERT(f2.addDataSource(f2name));
   TUASSERTE(size_t, 232, f2.size());

      // test loading SP3b nav
   gpstk::SP3NavDataFactory f3;
   std::string f3name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3b.sp3";
      // this should implicitly load into the data map
   TUASSERT(f3.addDataSource(f3name));
   TUASSERTE(size_t, 232, f3.size());

   TestClass f4;
   std::string f4name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUCATCH(f4.setTypeFilter({gpstk::NavMessageType::Ephemeris}));
      // this should implicitly load into the data map
   TUASSERT(f4.addDataSource(f4name));
   TUASSERTE(size_t, 116, f4.size());
   gpstk::NavMessageMap &nmm4(f4.getData());
      // only one message type
   TUASSERTE(size_t, 1, nmm4.size());
      // and it's ephemeris.
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             nmm4.begin()->first);
   TUCSM("convertToOrbit");
   verifyDataType<gpstk::OrbitDataSP3>(testFramework, nmm4);
   TUCSM("loadIntoMap");

   TestClass f5;
   std::string f5name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUCATCH(f5.setTypeFilter({gpstk::NavMessageType::Health}));
      // this should implicitly load into the data map
   TUASSERT(f5.addDataSource(f5name));
   TUASSERTE(size_t, 0, f5.size());
   TUCSM("loadIntoMap");

   TestClass f6;
   std::string f6name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUCATCH(f6.setTypeFilter({gpstk::NavMessageType::Almanac}));
      // this should implicitly load into the data map
   TUASSERT(f6.addDataSource(f6name));
      // except there isn't any data, because SP3 NAV doesn't contain almanacs
   TUASSERTE(size_t, 0, f6.size());

      // test loading something that isn't SP3
   TestClass f7;
   std::string f7name = gpstk::getPathData() + gpstk::getFileSep() +
      "arlm2000.15n";
   TUASSERT(!f7.addDataSource(f7name));
   TUASSERTE(size_t, 0, f7.size());

   TURETURN();
}


unsigned SP3NavDataFactory_T ::
findExactTest()
{
   TUDEF("SP3NavDataFactory", "find");
   gpstk::SP3NavDataFactory fact;
   gpstk::NavSatelliteID satID1(7, 7, gpstk::SatelliteSystem::GPS,
                                 gpstk::CarrierBand::L1,
                                 gpstk::TrackingCode::CA,
                                 gpstk::NavType::GPSLNAV);
   gpstk::WildSatID expSat1(7, gpstk::SatelliteSystem::GPS);
   gpstk::NavMessageID nmid1(satID1, gpstk::NavMessageType::Ephemeris);
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUASSERT(fact.addDataSource(fname));
      // 116 ephemeris + 116 clock
   TUASSERTE(size_t, 232, fact.size());
      // test find with exact epoch at start of file
   gpstk::CivilTime civ1(2001, 7, 22, 0, 0, 0, gpstk::TimeSystem::GPS);
   gpstk::CommonTime ct1(civ1);
   gpstk::NavDataPtr nd1;
   TUASSERT(fact.find(nmid1, ct1, nd1, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   gpstk::OrbitDataSP3 *uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd1.get());
                // NavData
   TUASSERTE(gpstk::CommonTime, ct1, uut->timeStamp);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gpstk::WildSatID, expSat1, uut->signal.sat);
   TUASSERTE(gpstk::WildSatID, expSat1, uut->signal.xmitSat);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
             uut->signal.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1, uut->signal.carrier);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::CA, uut->signal.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, uut->signal.nav);
      // OrbitData
      // OrbitDataSP3
      // Epsilon specified because of rounding errors and because of
      // what precision is available in the source data.
   TUASSERTFEPS(-18707084.879, uut->pos[0], 0.001);
   TUASSERTFEPS( 16766780.691, uut->pos[1], 0.001);
   TUASSERTFEPS(  8582072.924, uut->pos[2], 0.001);
   TUASSERTFEPS(468.885781, uut->clkBias, 0.000001);
   TUASSERTFEPS(-107758.25536, uut->vel[0], 0.00001);
   TUASSERTFEPS(  20775.83886, uut->vel[1], 0.00001);
   TUASSERTFEPS(-289749.34472, uut->vel[2], 0.00001);
   TUASSERTFEPS(-.252832, uut->clkDrift, 0.000001);

      // test find with exact epoch at end of file
   gpstk::CivilTime civ2(2001, 7, 22, 0, 45, 0, gpstk::TimeSystem::GPS);
   gpstk::CommonTime ct2(civ2);
   gpstk::NavDataPtr nd2;
   gpstk::NavSatelliteID satID2(6, 6, gpstk::SatelliteSystem::GPS,
                                 gpstk::CarrierBand::L1,
                                 gpstk::TrackingCode::CA,
                                 gpstk::NavType::GPSLNAV);
   gpstk::WildSatID expSat2(6, gpstk::SatelliteSystem::GPS);
   gpstk::NavMessageID nmid2(satID2, gpstk::NavMessageType::Ephemeris);
   TUASSERT(fact.find(nmid2, ct2, nd2, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd2.get());
                // NavData
   TUASSERTE(gpstk::CommonTime, ct2, uut->timeStamp);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gpstk::WildSatID, expSat2, uut->signal.sat);
   TUASSERTE(gpstk::WildSatID, expSat2, uut->signal.xmitSat);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
             uut->signal.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1, uut->signal.carrier);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::CA, uut->signal.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, uut->signal.nav);
      // OrbitData
      // OrbitDataSP3
      // Epsilon specified because of rounding errors and because of
      // what precision is available in the source data.
   TUASSERTFEPS(  21107001.443, uut->pos[0], 0.001);
   TUASSERTFEPS(  -1108815.431, uut->pos[1], 0.001);
   TUASSERTFEPS(  16383326.664, uut->pos[2], 0.001);
   TUASSERTFEPS(     -2.073849, uut->clkBias, 0.000001);
   TUASSERTFEPS( -150902.99480, uut->vel[0], 0.00001);
   TUASSERTFEPS(  139502.54271, uut->vel[1], 0.00001);
   TUASSERTFEPS(  203820.28718, uut->vel[2], 0.00001);
   TUASSERTFEPS(       .008499, uut->clkDrift, 0.000001);

      // test find with exact epoch in the middle of file
   gpstk::CivilTime civ3(2001, 7, 22, 0, 30, 0, gpstk::TimeSystem::GPS);
   gpstk::CommonTime ct3(civ3);
   gpstk::NavDataPtr nd3;
   gpstk::NavSatelliteID satID3(11, 11, gpstk::SatelliteSystem::GPS,
                                 gpstk::CarrierBand::L1,
                                 gpstk::TrackingCode::CA,
                                 gpstk::NavType::GPSLNAV);
   gpstk::WildSatID expSat3(11, gpstk::SatelliteSystem::GPS);
   gpstk::NavMessageID nmid3(satID3, gpstk::NavMessageType::Ephemeris);
   TUASSERT(fact.find(nmid3, ct3, nd3, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd3.get());
                // NavData
   TUASSERTE(gpstk::CommonTime, ct3, uut->timeStamp);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gpstk::WildSatID, expSat3, uut->signal.sat);
   TUASSERTE(gpstk::WildSatID, expSat3, uut->signal.xmitSat);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
             uut->signal.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1, uut->signal.carrier);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::CA, uut->signal.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, uut->signal.nav);
      // OrbitData
      // OrbitDataSP3
      // Epsilon specified because of rounding errors and because of
      // what precision is available in the source data.
   TUASSERTFEPS( -20916443.568, uut->pos[0], 0.001);
   TUASSERTFEPS( -14840885.192, uut->pos[1], 0.001);
   TUASSERTFEPS(  -6804522.988, uut->pos[2], 0.001);
   TUASSERTFEPS(      2.966233, uut->clkBias, 0.000001);
   TUASSERTFEPS(   95470.31126, uut->vel[0], 0.00001);
   TUASSERTFEPS(    -350.54953, uut->vel[1], 0.00001);
   TUASSERTFEPS( -292438.93155, uut->vel[2], 0.00001);
   TUASSERTFEPS(       .002133, uut->clkDrift, 0.000001);
   TURETURN();
}


unsigned SP3NavDataFactory_T ::
findInterpTest()
{
   TUDEF("SP3NavDataFactory", "find");
   gpstk::SP3NavDataFactory fact;
   gpstk::NavSatelliteID satID1(15, 15, gpstk::SatelliteSystem::GPS,
                                 gpstk::CarrierBand::L1,
                                 gpstk::TrackingCode::CA,
                                 gpstk::NavType::GPSLNAV);
   gpstk::WildSatID expSat1(15, gpstk::SatelliteSystem::GPS);
   gpstk::NavMessageID nmid1(satID1, gpstk::NavMessageType::Ephemeris);
   gpstk::CommonTime ct;
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_sp3_nav_ephemerisData.sp3";
   TUASSERT(fact.addDataSource(fname));
      // 2400 ephemeris + 2400 clock
   TUASSERTE(size_t, 4800, fact.size());
   ct = gpstk::CivilTime(1997,4,6,0,45,2,gpstk::TimeSystem::GPS);
   gpstk::NavDataPtr nd1;
   TUASSERT(!fact.find(nmid1, ct, nd1, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::ValidOnly,
                       gpstk::NavSearchOrder::User));

   ct = gpstk::CivilTime(1997,4,6,6,17,36,gpstk::TimeSystem::GPS);
   gpstk::NavDataPtr nd2;
   std::cerr << "the test starts here" << std::endl;
   TUASSERT(fact.find(nmid1, ct, nd2, gpstk::SVHealth::Any,
                      gpstk::NavValidityType::ValidOnly,
                      gpstk::NavSearchOrder::User));
   std::cerr << "the test ends here" << std::endl;
   gpstk::OrbitDataSP3 *uut = dynamic_cast<gpstk::OrbitDataSP3*>(nd2.get());
   if (uut == nullptr)
      TURETURN();
      // NavData
   TUASSERTE(gpstk::CommonTime, ct, uut->timeStamp);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             uut->signal.messageType);
   TUASSERTE(gpstk::WildSatID, expSat1, uut->signal.sat);
   TUASSERTE(gpstk::WildSatID, expSat1, uut->signal.xmitSat);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
             uut->signal.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1, uut->signal.carrier);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::CA, uut->signal.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, uut->signal.nav);
      // OrbitData
      // OrbitDataSP3
      // Commented out truth values are from SP3EphemerisStore test.
      // Note that discrepancies in data are on the order of
      // nanometers, and are due to the fact that
      // SP3EphemerisStore/PositionSatStore interpolates over km vs m.
      //TUASSERTFE(-15643515.779275318608, uut->pos[0]);
   TUASSERTFE(-15643515.779275322333, uut->pos[0]);
   std::cerr << std::setprecision(20) << uut->pos[0] << std::endl;
   TUASSERTFE(17046376.009584486485, uut->pos[1]);
   std::cerr << std::setprecision(20) << uut->pos[1] << std::endl;
   TUASSERTFE(12835522.993916222826, uut->pos[2]);
   std::cerr << std::setprecision(20) << uut->pos[2] << std::endl;
      //TUASSERTFE(411.87359147363480361, uut->vel[0]);
   TUASSERTFE(411.87359147363758893, uut->vel[0]);
   std::cerr << std::setprecision(20) << uut->vel[0] << std::endl;
      //TUASSERTFE(-1620.8344018608040642, uut->vel[1]);
   TUASSERTFE(-1620.8344018608056558, uut->vel[1]);
   std::cerr << std::setprecision(20) << uut->vel[1] << std::endl;
      //TUASSERTFE(2608.7591273316520528, uut->vel[2]);
   TUASSERTFE(2608.7591273316543266, uut->vel[2]);
   std::cerr << std::setprecision(20) << uut->vel[2] << std::endl;
      //TUASSERTFE(411.55797411176868201, uut->clkBias);
   TUASSERTFE(411.55797411176871492, uut->clkBias);
   std::cerr << std::setprecision(20) << uut->clkBias << std::endl;
   TUASSERTFE(2.29094726634170796e-6, uut->clkDrift);
   std::cerr << std::setprecision(20) << uut->clkDrift << std::endl;
   TURETURN();
}


template <class NavClass>
void SP3NavDataFactory_T ::
verifyDataType(gpstk::TestUtil& testFramework,
               gpstk::NavMessageMap& nmm)
{
   for (auto& nmti : nmm)
   {
      for (auto& sati : nmti.second)
      {
         for (auto& ti : sati.second)
         {
            TUASSERT(dynamic_cast<NavClass*>(ti.second.get()) != nullptr);
         }
      }
   }
}


int main()
{
   SP3NavDataFactory_T testClass;
   unsigned errorTotal = 0;

//   errorTotal += testClass.constructorTest();
//   errorTotal += testClass.loadIntoMapTest();
//   errorTotal += testClass.findExactTest();
   errorTotal += testClass.findInterpTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
