#include "NavDataFactoryWithStore.hpp"
#include "GPSWeekSecond.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavTimeOffset.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
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
class TestClass : public gpstk::NavDataFactoryWithStore
{
public:
      /// Grant access to protected data.
   gpstk::NavMessageMap& getData()
   { return data; }
   bool addDataSource(const std::string& source) override
   { return false; }
};


class NavDataFactoryWithStore_T
{
public:
      /// Initialize time objects used for stamping fake nav data.
   NavDataFactoryWithStore_T();

   unsigned addNavDataTest();
   unsigned findTest();
   unsigned getOffsetTest();
   unsigned editTest();
   unsigned clearTest();

      /// Fill fact with test data
   void fillFactory(gpstk::TestUtil& testFramework, TestClass& fact);
      /// Add a single NavData with the given parameters
   void addData(gpstk::TestUtil& testFramework, TestClass& fact,
                const gpstk::CommonTime& ct, unsigned long sat,
                unsigned long xmitSat,
                gpstk::SatelliteSystem sys = gpstk::SatelliteSystem::GPS,
                gpstk::CarrierBand car = gpstk::CarrierBand::L1,
                gpstk::TrackingCode code = gpstk::TrackingCode::CA,
                gpstk::NavType nav = gpstk::NavType::GPSLNAV);
   void fillSignal(gpstk::NavSignalID& signal,
                   gpstk::SatelliteSystem sys = gpstk::SatelliteSystem::GPS,
                   gpstk::CarrierBand car = gpstk::CarrierBand::L1,
                   gpstk::TrackingCode code = gpstk::TrackingCode::CA,
                   gpstk::NavType nav = gpstk::NavType::GPSLNAV);
   void fillSat(gpstk::NavSatelliteID& satellite,
                unsigned long sat,
                unsigned long xmitSat,
                gpstk::SatelliteSystem sys = gpstk::SatelliteSystem::GPS,
                gpstk::CarrierBand car = gpstk::CarrierBand::L1,
                gpstk::TrackingCode code = gpstk::TrackingCode::CA,
                gpstk::NavType nav = gpstk::NavType::GPSLNAV);
      /// Check to make sure there are no empty maps in fact.
   void checkForEmpty(gpstk::TestUtil& testFramework, TestClass& fact);

   gpstk::GPSWeekSecond gws;
   gpstk::CommonTime ct;
};


NavDataFactoryWithStore_T ::
NavDataFactoryWithStore_T()
      : gws(2101,3450.0),
        ct(gws)
{
}


unsigned NavDataFactoryWithStore_T ::
addNavDataTest()
{
   TUDEF("NavDataFactoryWithStore", "addNavData");

   TestClass fact;

      // NavData is abstract so we instantiate a GPSLNavEph instead
   gpstk::SatID subjID(23,gpstk::SatelliteSystem::GPS);
   gpstk::SatID xmitID(32,gpstk::SatelliteSystem::GPS);
   gpstk::NavDataPtr navOut = std::make_shared<gpstk::GPSLNavEph>();
   navOut->timeStamp = ct;
   navOut->signal.messageType = gpstk::NavMessageType::Ephemeris;
   navOut->signal.sat = subjID;
   navOut->signal.xmitSat = xmitID;
   navOut->signal.system = gpstk::SatelliteSystem::GPS;
   navOut->signal.carrier = gpstk::CarrierBand::L1;
   navOut->signal.code = gpstk::TrackingCode::CA;
   navOut->signal.nav = gpstk::NavType::GPSLNAV;

   TUASSERT(fact.addNavData(navOut));
   TUASSERTE(size_t, 1, fact.size());

   gpstk::NavMessageMap &nmm(fact.getData());
   for (auto& nmmi : nmm)
   {
      for (auto& nsatmi : nmmi.second)
      {
         TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
                   nsatmi.first.system);
         TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1,
                   nsatmi.first.carrier);
         TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::CA,
                   nsatmi.first.code);
         TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, nsatmi.first.nav);
         TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
                   nsatmi.first.system);
         TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1,
                   nsatmi.first.carrier);
         TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::CA,
                   nsatmi.first.code);
         TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV,
                   nsatmi.first.nav);
         TUASSERTE(gpstk::SatID, subjID, nsatmi.first.sat);
         TUASSERTE(gpstk::SatID, xmitID, nsatmi.first.xmitSat);
         for (auto& nmi : nsatmi.second)
         {
            TUASSERTE(gpstk::CommonTime, ct+18, nmi.first);
         }
      }
   }

   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
findTest()
{
   TUDEF("NavDataFactoryWithStore", "find");

   TestClass fact1;
   gpstk::NavMessageID nmid1a;
   gpstk::NavDataPtr result;
   TUCATCH(fillFactory(testFramework, fact1));
   TUCATCH(fillSat(nmid1a, 23, 32));
   nmid1a.messageType = gpstk::NavMessageType::Ephemeris;
      // check basic time stamps that should work
   TUASSERT(fact1.find(nmid1a, ct+35, result, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::All,
                       gpstk::NavSearchOrder::User));
   TUASSERT(fact1.find(nmid1a, ct+30, result, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::All,
                       gpstk::NavSearchOrder::User));
   TUASSERT(fact1.find(nmid1a, ct+60, result, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::All,
                       gpstk::NavSearchOrder::User));
      // check time stamps that don't work
   TUASSERT(!fact1.find(nmid1a, ct, result, gpstk::SVHealth::Any,
                        gpstk::NavValidityType::All,
                        gpstk::NavSearchOrder::User));
      // time edge cases.
   TUASSERT(!fact1.find(nmid1a, ct+17, result, gpstk::SVHealth::Any,
                        gpstk::NavValidityType::All,
                        gpstk::NavSearchOrder::User));
   TUASSERT(fact1.find(nmid1a, ct+18, result, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::All,
                       gpstk::NavSearchOrder::User));
      // test validity flags
   TUASSERT(fact1.find(nmid1a, ct+30, result, gpstk::SVHealth::Any,
                       gpstk::NavValidityType::ValidOnly,
                       gpstk::NavSearchOrder::User));
   TUASSERT(!fact1.find(nmid1a, ct+30, result, gpstk::SVHealth::Any,
                        gpstk::NavValidityType::InvalidOnly,
                        gpstk::NavSearchOrder::User));

   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
getOffsetTest()
{
   TUDEF("NavDataFactoryWithStore", "getOffset");
   TestClass fact1;

   gpstk::NavDataPtr navOut = std::make_shared<gpstk::GPSLNavTimeOffset>();
   navOut->timeStamp = ct;
   navOut->signal.messageType = gpstk::NavMessageType::TimeOffset;
   gpstk::GPSLNavTimeOffset *toptr = dynamic_cast<gpstk::GPSLNavTimeOffset*>(
      navOut.get());
   fillSat(navOut->signal, 23, 23);
   toptr->deltatLS = 23; // set a simple, easy to verify value.
   TUASSERT(fact1.addNavData(navOut));
   double result;
   TUASSERT(fact1.getOffset(gpstk::TimeSystem::GPS, gpstk::TimeSystem::UTC,
                            ct+35, result, gpstk::SVHealth::Any,
                            gpstk::NavValidityType::All,
                            gpstk::NavSearchOrder::User));
   TUASSERTFE(23.0, result);
      // reverse the conversion and expect negative.
   TUASSERT(fact1.getOffset(gpstk::TimeSystem::UTC, gpstk::TimeSystem::GPS,
                            ct+35, result, gpstk::SVHealth::Any,
                            gpstk::NavValidityType::All,
                            gpstk::NavSearchOrder::User));
   TUASSERTFE(-23.0, result);
      // expect this to not work
   TUASSERT(!fact1.getOffset(gpstk::TimeSystem::UTC, gpstk::TimeSystem::BDT,
                             ct+35, result, gpstk::SVHealth::Any,
                             gpstk::NavValidityType::All,
                             gpstk::NavSearchOrder::User));

   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
editTest()
{
   TUDEF("NavDataFactoryWithStore", "edit");

      // First check the plain edit
   TestClass fact1;
   TUCATCH(fillFactory(testFramework, fact1));
      // verify initial conditions
   TUASSERTE(size_t, 8, fact1.size());
   TUASSERTE(size_t, 2, fact1.numSignals());
   TUASSERTE(size_t, 3, fact1.numSatellites());
      // remove nothing
   TUCATCH(fact1.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct));
   TUASSERTE(size_t, 8, fact1.size());
   TUASSERTE(size_t, 2, fact1.numSignals());
   TUASSERTE(size_t, 3, fact1.numSatellites());
   checkForEmpty(testFramework, fact1);
      // remove messages at ct
   TUCATCH(fact1.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct+30));
   TUASSERTE(size_t, 5, fact1.size());
   TUASSERTE(size_t, 2, fact1.numSignals());
   TUASSERTE(size_t, 3, fact1.numSatellites());
   checkForEmpty(testFramework, fact1);
      // remove messages at ct+30
   TUCATCH(fact1.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct+60));
   TUASSERTE(size_t, 2, fact1.size());
   TUASSERTE(size_t, 1, fact1.numSignals());
   TUASSERTE(size_t, 1, fact1.numSatellites());
   checkForEmpty(testFramework, fact1);
      // remove messages at ct+60
   TUCATCH(fact1.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct+90));
   TUASSERTE(size_t, 1, fact1.size());
   TUASSERTE(size_t, 1, fact1.numSignals());
   TUASSERTE(size_t, 1, fact1.numSatellites());
   checkForEmpty(testFramework, fact1);
      // remove messages at ct+90
   TUCATCH(fact1.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct+120));
   TUASSERTE(size_t, 0, fact1.size());
   TUASSERTE(size_t, 0, fact1.numSignals());
   TUASSERTE(size_t, 0, fact1.numSatellites());
   checkForEmpty(testFramework, fact1);

      // Now check satellite editing
   TestClass fact2;
   gpstk::NavSatelliteID satID2a, satID2b, satID2c, satID2d, satID2e;
   TUCATCH(fillFactory(testFramework, fact2));
   TUCATCH(fillSat(satID2a, 7, 7));
   TUCATCH(fillSat(satID2b, 99, 99));
   TUCATCH(fillSat(satID2c, 7, 7, gpstk::SatelliteSystem::GPS,
                   gpstk::CarrierBand::L5));
   TUCATCH(fillSat(satID2d, 11, 11, gpstk::SatelliteSystem::GPS,
                   gpstk::CarrierBand::L1, gpstk::TrackingCode::Y));
   TUCATCH(fillSat(satID2e, 23, 32));
      // verify initial conditions
   TUASSERTE(size_t, 8, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 3, fact2.numSatellites());
   checkForEmpty(testFramework, fact2);
      // remove nothing
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct, satID2a));
   TUASSERTE(size_t, 8, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 3, fact2.numSatellites());
   checkForEmpty(testFramework, fact2);
      // remove nothing, this time because the satellite ID isn't present
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, satID2b));
   TUASSERTE(size_t, 8, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 3, fact2.numSatellites());
   checkForEmpty(testFramework, fact2);
      // remove nothing, this time because the signal isn't present
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, satID2c));
   TUASSERTE(size_t, 8, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 3, fact2.numSatellites());
   checkForEmpty(testFramework, fact2);
      // remove all sat 7 data
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, satID2a));
   TUASSERTE(size_t, 6, fact2.size());
   TUASSERTE(size_t, 2, fact2.numSignals());
   TUASSERTE(size_t, 2, fact2.numSatellites());
   checkForEmpty(testFramework, fact2);
      // remove all sat 11 data
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, satID2d));
   TUASSERTE(size_t, 4, fact2.size());
   TUASSERTE(size_t, 1, fact2.numSignals());
   TUASSERTE(size_t, 1, fact2.numSatellites());
   checkForEmpty(testFramework, fact2);
      // remove some of sat 23 data
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct+30, satID2e));
   TUASSERTE(size_t, 3, fact2.size());
   TUASSERTE(size_t, 1, fact2.numSignals());
   TUASSERTE(size_t, 1, fact2.numSatellites());
   checkForEmpty(testFramework, fact2);
      // remove the rest of sat 23 data
   TUCATCH(fact2.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, satID2e));
   TUASSERTE(size_t, 0, fact2.size());
   TUASSERTE(size_t, 0, fact2.numSignals());
   TUASSERTE(size_t, 0, fact2.numSatellites());
   checkForEmpty(testFramework, fact2);

      // finally, check editing by signal
   TestClass fact3;
   gpstk::NavSignalID sig3a, sig3b, sig3c;
   TUCATCH(fillFactory(testFramework, fact3));
   TUCATCH(fillSignal(sig3a));
   TUCATCH(fillSignal(sig3b, gpstk::SatelliteSystem::Glonass));
   TUCATCH(fillSignal(sig3c, gpstk::SatelliteSystem::GPS,
                      gpstk::CarrierBand::L1, gpstk::TrackingCode::Y));
      // verify initial conditions
   TUASSERTE(size_t, 8, fact3.size());
   TUASSERTE(size_t, 2, fact3.numSignals());
   TUASSERTE(size_t, 3, fact3.numSatellites());
   checkForEmpty(testFramework, fact3);
      // remove nothing
   TUCATCH(fact3.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct, sig3a));
   TUASSERTE(size_t, 8, fact3.size());
   TUASSERTE(size_t, 2, fact3.numSignals());
   TUASSERTE(size_t, 3, fact3.numSatellites());
   checkForEmpty(testFramework, fact3);
      // remove nothing, this time because the signal isn't present
   TUCATCH(fact3.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, sig3b));
   TUASSERTE(size_t, 8, fact3.size());
   TUASSERTE(size_t, 2, fact3.numSignals());
   TUASSERTE(size_t, 3, fact3.numSatellites());
   checkForEmpty(testFramework, fact3);
      // remove all L1-Y data
   TUCATCH(fact3.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, sig3c));
   TUASSERTE(size_t, 6, fact3.size());
   TUASSERTE(size_t, 1, fact3.numSignals());
   TUASSERTE(size_t, 2, fact3.numSatellites());
   checkForEmpty(testFramework, fact3);
      // remove some of L1-CA data
   TUCATCH(fact3.edit(gpstk::CommonTime::BEGINNING_OF_TIME, ct+30, sig3a));
   TUASSERTE(size_t, 4, fact3.size());
   TUASSERTE(size_t, 1, fact3.numSignals());
   TUASSERTE(size_t, 2, fact3.numSatellites());
   checkForEmpty(testFramework, fact3);
      // remove the rest of L1-CA data
   TUCATCH(fact3.edit(gpstk::CommonTime::BEGINNING_OF_TIME,
                      gpstk::CommonTime::END_OF_TIME, sig3a));
   TUASSERTE(size_t, 0, fact3.size());
   TUASSERTE(size_t, 0, fact3.numSignals());
   TUASSERTE(size_t, 0, fact3.numSatellites());
   checkForEmpty(testFramework, fact3);

   TURETURN();
}


unsigned NavDataFactoryWithStore_T ::
clearTest()
{
   TUDEF("NavDataFactoryWithStore", "clear");

   TestClass fact;

      // NavData is abstract so we instantiate a GPSLNavEph instead
   gpstk::NavDataPtr navOut = std::make_shared<gpstk::GPSLNavEph>();
   navOut->timeStamp = ct;
   navOut->signal.messageType = gpstk::NavMessageType::Ephemeris;
   navOut->signal.sat = gpstk::SatID(23,gpstk::SatelliteSystem::GPS);
   navOut->signal.xmitSat = gpstk::SatID(32,gpstk::SatelliteSystem::GPS);
   navOut->signal.system = gpstk::SatelliteSystem::GPS;
   navOut->signal.carrier = gpstk::CarrierBand::L1;
   navOut->signal.code = gpstk::TrackingCode::CA;
   navOut->signal.nav = gpstk::NavType::GPSLNAV;

   TUASSERT(fact.addNavData(navOut));
   TUASSERTE(size_t, 1, fact.size());

   fact.clear();
   TUASSERTE(size_t, 0, fact.size());

   TURETURN();
}


void NavDataFactoryWithStore_T ::
fillFactory(gpstk::TestUtil& testFramework, TestClass& fact)
{
   addData(testFramework, fact, ct+0,  23, 32);
   addData(testFramework, fact, ct+0,   7, 7);
   addData(testFramework, fact, ct+0,  11, 11, gpstk::SatelliteSystem::GPS,
           gpstk::CarrierBand::L1, gpstk::TrackingCode::Y);
   addData(testFramework, fact, ct+30, 23, 32);
   addData(testFramework, fact, ct+30,  7, 7);
   addData(testFramework, fact, ct+30, 11, 11, gpstk::SatelliteSystem::GPS,
           gpstk::CarrierBand::L1, gpstk::TrackingCode::Y);
   addData(testFramework, fact, ct+60, 23, 32);
   addData(testFramework, fact, ct+90, 23, 32);
}


void NavDataFactoryWithStore_T ::
addData(gpstk::TestUtil& testFramework, TestClass& fact,
        const gpstk::CommonTime& ct, unsigned long sat,
        unsigned long xmitSat, gpstk::SatelliteSystem sys,
        gpstk::CarrierBand car, gpstk::TrackingCode code, gpstk::NavType nav)
{
   gpstk::NavDataPtr navOut = std::make_shared<gpstk::GPSLNavEph>();
   navOut->timeStamp = ct;
   navOut->signal.messageType = gpstk::NavMessageType::Ephemeris;
   fillSat(navOut->signal, sat, xmitSat, sys, car, code, nav);
   TUASSERT(fact.addNavData(navOut));
}


void NavDataFactoryWithStore_T ::
fillSignal(gpstk::NavSignalID& signal, gpstk::SatelliteSystem sys,
           gpstk::CarrierBand car, gpstk::TrackingCode code, gpstk::NavType nav)
{
   signal.system = sys;
   signal.carrier = car;
   signal.code = code;
   signal.nav = nav;
}


void NavDataFactoryWithStore_T ::
fillSat(gpstk::NavSatelliteID& satellite,
        unsigned long sat, unsigned long xmitSat,
        gpstk::SatelliteSystem sys, gpstk::CarrierBand car,
        gpstk::TrackingCode code, gpstk::NavType nav)
{
   fillSignal(satellite, sys, car, code, nav);
   satellite.sat = gpstk::SatID(sat,sys);
   satellite.xmitSat = gpstk::SatID(xmitSat,sys);
}


void NavDataFactoryWithStore_T ::
checkForEmpty(gpstk::TestUtil& testFramework, TestClass& fact)
{
   gpstk::NavMessageMap &data(fact.getData());
   for (auto& nmmi : data)
   {
      TUASSERT(!nmmi.second.empty());
      for (auto& sati : nmmi.second)
      {
         TUASSERT(!sati.second.empty());
      }
   }
}


int main()
{
   NavDataFactoryWithStore_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.addNavDataTest();
   errorTotal += testClass.editTest();
   errorTotal += testClass.clearTest();
   errorTotal += testClass.findTest();
   errorTotal += testClass.getOffsetTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
