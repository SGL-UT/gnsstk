#include "PNBMultiGNSSNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "PNBGPSLNavDataFactory.hpp"
#include "GPSLNavTimeOffset.hpp"
#include "GPSLNavHealth.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavAlm.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class TestClass : public gpstk::PNBMultiGNSSNavDataFactory
{
public:
   static gpstk::PNBNavDataFactoryMap& getFactories()
   { return factories; }
};
class PNBTestFactory : public gpstk::PNBNavDataFactory
{
public:
   bool addData(const gpstk::PackedNavBitsPtr& navIn,
                gpstk::NavDataPtrList& navOut) override
   { return false; }
   gpstk::NavValidityType getValidity() const { return navValidity; }
   bool getProcEph() const { return processEph; }
   bool getProcAlm() const { return processAlm; }
   bool getProcHea() const { return processHea; }
   bool getProcTim() const { return processTim; }
};


class PNBMultiGNSSNavDataFactory_T
{
public:
   PNBMultiGNSSNavDataFactory_T();
      /// This must be executed first as it adds a factory that other tests use.
   unsigned addFactoryTest();
      /** Make sure that the PNBMultiGNSSNavDataFactory contains the
       * expected factory objects.  Also implicitly tests addFactory,
       * as the test would fail if addFactory didn't work. */
   unsigned constructorTest();
   unsigned setValidityFilterTest();
   unsigned setTypeFilterTest();
   unsigned addDataTest();

      /// Count the various types of messages present in navOut.
   void countResults(const gpstk::NavDataPtrList& navOut);

   gpstk::ObsID oidLNAV;
   gpstk::PackedNavBitsPtr ephLNAVSF1, ephLNAVSF2, ephLNAVSF3, almLNAV25,
      almLNAV26, pg51LNAV, pg56LNAV, pg63LNAV;
   gpstk::CommonTime ephLNAVSF1ct, ephLNAVSF2ct, ephLNAVSF3ct, almLNAV25ct,
      almLNAV26ct, pg51LNAVct, pg56LNAVct, pg63LNAVct;
   gpstk::SatID ephLNAVSid, almLNAVSid;

      /// Counts of messages, set by countResults.
   unsigned almCount, ephCount, toCount, heaCount, otherCount;
};


PNBMultiGNSSNavDataFactory_T ::
PNBMultiGNSSNavDataFactory_T()
      : oidLNAV(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::L1,
            gpstk::TrackingCode::CA),
        ephLNAVSF1ct(gpstk::GPSWeekSecond(1869,6.0)),
        ephLNAVSF2ct(gpstk::GPSWeekSecond(1869,12.0)),
        ephLNAVSF3ct(gpstk::GPSWeekSecond(1869,18.0)),
        almLNAV25ct(gpstk::GPSWeekSecond(1869,54.0)),
        almLNAV26ct(gpstk::GPSWeekSecond(1869,84.0)),
        pg51LNAVct(gpstk::GPSWeekSecond(1869,750.0)),
        pg56LNAVct(gpstk::GPSWeekSecond(1869,528.0)),
        pg63LNAVct(gpstk::GPSWeekSecond(1869,744.0)),
        ephLNAVSid(4,gpstk::SatelliteSystem::GPS),
        almLNAVSid(1,gpstk::SatelliteSystem::GPS),
        almCount(0), ephCount(0), toCount(0), heaCount(0), otherCount(0)
{
   ephLNAVSF1 = std::make_shared<gpstk::PackedNavBits>(ephLNAVSid,oidLNAV,
                                                       ephLNAVSF1ct);
   ephLNAVSF1->setNavID(gpstk::NavType::GPSLNAV);
   ephLNAVSF1->addUnsignedLong(0x22C34D21,30,1);
   ephLNAVSF1->addUnsignedLong(0x000029D4,30,1);
   ephLNAVSF1->addUnsignedLong(0x34D44000,30,1);
   ephLNAVSF1->addUnsignedLong(0x091B1DE7,30,1);
   ephLNAVSF1->addUnsignedLong(0x1C33746E,30,1);
   ephLNAVSF1->addUnsignedLong(0x2F701369,30,1);
   ephLNAVSF1->addUnsignedLong(0x39F53CB5,30,1);
   ephLNAVSF1->addUnsignedLong(0x128070A8,30,1);
   ephLNAVSF1->addUnsignedLong(0x003FF454,30,1);
   ephLNAVSF1->addUnsignedLong(0x3EAFC2F0,30,1);
   ephLNAVSF2 = std::make_shared<gpstk::PackedNavBits>(ephLNAVSid,oidLNAV,
                                                       ephLNAVSF2ct);
   ephLNAVSF2->setNavID(gpstk::NavType::GPSLNAV);
   ephLNAVSF2->addUnsignedLong(0x22C34D21,30,1);
   ephLNAVSF2->addUnsignedLong(0x00004A44,30,1);
   ephLNAVSF2->addUnsignedLong(0x12BFCB3A,30,1);
   ephLNAVSF2->addUnsignedLong(0x0D7A9094,30,1);
   ephLNAVSF2->addUnsignedLong(0x3B99FBAF,30,1);
   ephLNAVSF2->addUnsignedLong(0x3FC081B8,30,1);
   ephLNAVSF2->addUnsignedLong(0x09D171E1,30,1);
   ephLNAVSF2->addUnsignedLong(0x04B0A847,30,1);
   ephLNAVSF2->addUnsignedLong(0x03497656,30,1);
   ephLNAVSF2->addUnsignedLong(0x00709FA0,30,1);
   ephLNAVSF3 = std::make_shared<gpstk::PackedNavBits>(ephLNAVSid,oidLNAV,
                                                       ephLNAVSF3ct);
   ephLNAVSF3->setNavID(gpstk::NavType::GPSLNAV);
   ephLNAVSF3->addUnsignedLong(0x22C34D21,30,1);
   ephLNAVSF3->addUnsignedLong(0x00006BCC,30,1);
   ephLNAVSF3->addUnsignedLong(0x3FE14ED4,30,1);
   ephLNAVSF3->addUnsignedLong(0x05ABBB58,30,1);
   ephLNAVSF3->addUnsignedLong(0x3FE3498B,30,1);
   ephLNAVSF3->addUnsignedLong(0x145EE03A,30,1);
   ephLNAVSF3->addUnsignedLong(0x062ECB6F,30,1);
   ephLNAVSF3->addUnsignedLong(0x1C48068F,30,1);
   ephLNAVSF3->addUnsignedLong(0x3FE95E1E,30,1);
   ephLNAVSF3->addUnsignedLong(0x12844624,30,1);
   almLNAV25 = std::make_shared<gpstk::PackedNavBits>(almLNAVSid,oidLNAV,
                                                      almLNAV25ct);
   almLNAV25->setNavID(gpstk::NavType::GPSLNAV);
   almLNAV25->addUnsignedLong(0x22C34D21,30,1);
   almLNAV25->addUnsignedLong(0x00012C68,30,1);
   almLNAV25->addUnsignedLong(0x16499366,30,1);
   almLNAV25->addUnsignedLong(0x0905E5C5,30,1);
   almLNAV25->addUnsignedLong(0x3F52C035,30,1);
   almLNAV25->addUnsignedLong(0x28431EEE,30,1);
   almLNAV25->addUnsignedLong(0x39A2E96D,30,1);
   almLNAV25->addUnsignedLong(0x075C3A2E,30,1);
   almLNAV25->addUnsignedLong(0x117733C7,30,1);
   almLNAV25->addUnsignedLong(0x3E3FF848,30,1);
   almLNAV26 = std::make_shared<gpstk::PackedNavBits>(almLNAVSid,oidLNAV,
                                                      almLNAV26ct);
   almLNAV26->setNavID(gpstk::NavType::GPSLNAV);
   almLNAV26->addUnsignedLong(0x22C34D21,30,1);
   almLNAV26->addUnsignedLong(0x0001CC0C,30,1);
   almLNAV26->addUnsignedLong(0x16809922,30,1);
   almLNAV26->addUnsignedLong(0x0902F90F,30,1);
   almLNAV26->addUnsignedLong(0x3F4EC036,30,1);
   almLNAV26->addUnsignedLong(0x28433740,30,1);
   almLNAV26->addUnsignedLong(0x3994EFA9,30,1);
   almLNAV26->addUnsignedLong(0x3D38AA68,30,1);
   almLNAV26->addUnsignedLong(0x0E3E305F,30,1);
   almLNAV26->addUnsignedLong(0x3CBFE0A0,30,1);
   pg51LNAV = std::make_shared<gpstk::PackedNavBits>(almLNAVSid,oidLNAV,
                                                     pg51LNAVct);
   pg51LNAV->setNavID(gpstk::NavType::GPSLNAV);
   pg51LNAV->addUnsignedLong(0x22C34D21,30,1);
   pg51LNAV->addUnsignedLong(0x000FAD48,30,1);
   pg51LNAV->addUnsignedLong(0x1CC91377,30,1);
   pg51LNAV->addUnsignedLong(0x0000003F,30,1);
   pg51LNAV->addUnsignedLong(0x0000003F,30,1);
   pg51LNAV->addUnsignedLong(0x00FC0005,30,1);
   pg51LNAV->addUnsignedLong(0x00000016,30,1);
   pg51LNAV->addUnsignedLong(0x00000029,30,1);
   pg51LNAV->addUnsignedLong(0x00000016,30,1);
   pg51LNAV->addUnsignedLong(0x0000008C,30,1);
   pg63LNAV = std::make_shared<gpstk::PackedNavBits>(almLNAVSid,oidLNAV,
                                                     pg63LNAVct);
   pg63LNAV->setNavID(gpstk::NavType::GPSLNAV);
   pg63LNAV->addUnsignedLong(0x22C34D21,30,1);
   pg63LNAV->addUnsignedLong(0x000F8CC0,30,1);
   pg63LNAV->addUnsignedLong(0x1FEE6E77,30,1);
   pg63LNAV->addUnsignedLong(0x2AEAEEFF,30,1);
   pg63LNAV->addUnsignedLong(0x26A66A4A,30,1);
   pg63LNAV->addUnsignedLong(0x2A666659,30,1);
   pg63LNAV->addUnsignedLong(0x26EEEE6C,30,1);
   pg63LNAV->addUnsignedLong(0x2AEA402C,30,1);
   pg63LNAV->addUnsignedLong(0x00000000,30,1);
   pg63LNAV->addUnsignedLong(0x00000000,30,1);
   pg56LNAV = std::make_shared<gpstk::PackedNavBits>(almLNAVSid,oidLNAV,
                                                     pg56LNAVct);
   pg56LNAV->setNavID(gpstk::NavType::GPSLNAV);
   pg56LNAV->addUnsignedLong(0x22C34D21,30,1);
   pg56LNAV->addUnsignedLong(0x000B2C64,30,1);
   pg56LNAV->addUnsignedLong(0x1E05FFE1,30,1);
   pg56LNAV->addUnsignedLong(0x3F808FEC,30,1);
   pg56LNAV->addUnsignedLong(0x3F7F405F,30,1);
   pg56LNAV->addUnsignedLong(0x00000413,30,1);
   pg56LNAV->addUnsignedLong(0x0000003F,30,1);
   pg56LNAV->addUnsignedLong(0x01491376,30,1);
   pg56LNAV->addUnsignedLong(0x044EC0EB,30,1);
   pg56LNAV->addUnsignedLong(0x044000D8,30,1);
}


unsigned PNBMultiGNSSNavDataFactory_T ::
addFactoryTest()
{
   TUDEF("PNBMultiGNSSNavDataFactory", "addFactory()");
      // Add a test factory to the multi gnss set.
   gpstk::PNBNavDataFactoryPtr test(std::make_shared<PNBTestFactory>());
   TUASSERTE(bool, true,
             gpstk::PNBMultiGNSSNavDataFactory::addFactory(
                gpstk::NavType::Unknown, test));
   TURETURN();
}


unsigned PNBMultiGNSSNavDataFactory_T ::
constructorTest()
{
   TUDEF("PNBMultiGNSSNavDataFactory", "PNBMultiGNSSNavDataFactory()");
   bool foundGPSLNav = false;
   bool foundTest = false;
      // Iterate over the factories and try to find the expected
      // factory objects.  The factories map may also contain ext
      // factories, so we ignore anything else.
   for (auto& i : TestClass::getFactories())
   {
      gpstk::PNBNavDataFactory *p = i.second.get();
      if (dynamic_cast<gpstk::PNBGPSLNavDataFactory*>(p) != nullptr)
      {
         foundGPSLNav = true;
      }
      else if (dynamic_cast<PNBTestFactory*>(p) != nullptr)
      {
         foundTest = true;
      }
   }
   TUASSERT(foundGPSLNav);
   TUASSERT(foundTest);
   TURETURN();
}


unsigned PNBMultiGNSSNavDataFactory_T ::
setValidityFilterTest()
{
   TUDEF("PNBMultiGNSSNavDataFactory", "setValidityFilter()");
   gpstk::PNBMultiGNSSNavDataFactory uut;
   PNBTestFactory *fact = nullptr;
      // find our test factory so we can verify setValidityFilter
   for (auto& i : TestClass::getFactories())
   {
      gpstk::PNBNavDataFactory *p = i.second.get();
      if ((fact = dynamic_cast<PNBTestFactory*>(p)) != nullptr)
      {
         break;
      }
   }
   TUASSERT(fact != nullptr);
   if (fact == nullptr)
   {
         // give up.
      TURETURN();
   }
      // check default
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::All,
             fact->getValidity());
      // check possible values
   for (gpstk::NavValidityType i : gpstk::NavValidityTypeIterator())
   {
      uut.setValidityFilter(i);
      TUASSERTE(gpstk::NavValidityType, i, fact->getValidity());
   }
   TURETURN();
}


unsigned PNBMultiGNSSNavDataFactory_T ::
setTypeFilterTest()
{
   TUDEF("PNBMultiGNSSNavDataFactory", "setTypeFilter()");
   gpstk::PNBMultiGNSSNavDataFactory uut;
   PNBTestFactory *fact = nullptr;
      // find our test factory so we can verify setTypeFilter
   for (auto& i : TestClass::getFactories())
   {
      gpstk::PNBNavDataFactory *p = i.second.get();
      if ((fact = dynamic_cast<PNBTestFactory*>(p)) != nullptr)
      {
         break;
      }
   }
   TUASSERT(fact != nullptr);
   if (fact == nullptr)
   {
         // give up.
      TURETURN();
   }
      // check defaults
   TUASSERTE(bool, true,  fact->getProcEph());
   TUASSERTE(bool, true,  fact->getProcAlm());
   TUASSERTE(bool, true,  fact->getProcHea());
   TUASSERTE(bool, true,  fact->getProcTim());
      // check individual flags
   uut.setTypeFilter({gpstk::NavMessageType::Ephemeris});
   TUASSERTE(bool, true,  fact->getProcEph());
   TUASSERTE(bool, false, fact->getProcAlm());
   TUASSERTE(bool, false, fact->getProcHea());
   TUASSERTE(bool, false, fact->getProcTim());
   uut.setTypeFilter({gpstk::NavMessageType::Almanac});
   TUASSERTE(bool, false, fact->getProcEph());
   TUASSERTE(bool, true,  fact->getProcAlm());
   TUASSERTE(bool, false, fact->getProcHea());
   TUASSERTE(bool, false, fact->getProcTim());
   uut.setTypeFilter({gpstk::NavMessageType::Health});
   TUASSERTE(bool, false, fact->getProcEph());
   TUASSERTE(bool, false, fact->getProcAlm());
   TUASSERTE(bool, true,  fact->getProcHea());
   TUASSERTE(bool, false, fact->getProcTim());
   uut.setTypeFilter({gpstk::NavMessageType::TimeOffset});
   TUASSERTE(bool, false, fact->getProcEph());
   TUASSERTE(bool, false, fact->getProcAlm());
   TUASSERTE(bool, false, fact->getProcHea());
   TUASSERTE(bool, true,  fact->getProcTim());
      // some subset tests.
   uut.setTypeFilter({gpstk::NavMessageType::TimeOffset,
                      gpstk::NavMessageType::Ephemeris});
   TUASSERTE(bool, true,  fact->getProcEph());
   TUASSERTE(bool, false, fact->getProcAlm());
   TUASSERTE(bool, false, fact->getProcHea());
   TUASSERTE(bool, true,  fact->getProcTim());
   uut.setTypeFilter({gpstk::NavMessageType::Almanac,
                      gpstk::NavMessageType::Health});
   TUASSERTE(bool, false, fact->getProcEph());
   TUASSERTE(bool, true,  fact->getProcAlm());
   TUASSERTE(bool, true,  fact->getProcHea());
   TUASSERTE(bool, false, fact->getProcTim());
   TURETURN();
}


unsigned PNBMultiGNSSNavDataFactory_T ::
addDataTest()
{
   TUDEF("PNBMultiGNSSNavDataFactory", "addData()");
   gpstk::PNBMultiGNSSNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   uut.setTypeFilter(gpstk::allNavMessageTypes);
   uut.setValidityFilter(gpstk::NavValidityType::All);
      //
      // Start with some GPS data.
      //
      // Add subframe 1, expect only health initially.
   TUASSERTE(bool, true, uut.addData(ephLNAVSF1, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // add subframe 2, expect nothing yet
   TUASSERTE(bool, true, uut.addData(ephLNAVSF2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add subframe 3, expect the completed ephemeris
   TUASSERTE(bool, true, uut.addData(ephLNAVSF3, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   navOut.clear();
      // add almanac sv id 25, expect health
   TUASSERTE(bool, true, uut.addData(almLNAV25, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // add almanac sv id 26, expect health
   TUASSERTE(bool, true, uut.addData(almLNAV26, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // add page 56, expect time offset
   TUASSERTE(bool, true, uut.addData(pg56LNAV, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, toCount);
   navOut.clear();
      // add page 63, expect 8 health
   TUASSERTE(bool, true, uut.addData(pg63LNAV, navOut));
   TUASSERTE(size_t, 8, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 8, heaCount);
   navOut.clear();
      // add page 51, expect 24 health and 2 almanacs
   TUASSERTE(bool, true, uut.addData(pg51LNAV, navOut));
   TUASSERTE(size_t, 26, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 24, heaCount);
   TUASSERTE(unsigned, 2, almCount);
   navOut.clear();
      /// @todo Add additional signals as they are implemented.
   TURETURN();
}


void PNBMultiGNSSNavDataFactory_T ::
countResults(const gpstk::NavDataPtrList& navOut)
{
   almCount = ephCount = toCount = heaCount = otherCount = 0;
   for (const auto& i : navOut)
   {
      if (dynamic_cast<gpstk::GPSLNavAlm*>(i.get()) != nullptr)
      {
         almCount++;
      }
      else if (dynamic_cast<gpstk::GPSLNavEph*>(i.get()) != nullptr)
      {
         ephCount++;
      }
      else if (dynamic_cast<gpstk::GPSLNavTimeOffset*>(i.get()) != nullptr)
      {
         toCount++;
      }
      else if (dynamic_cast<gpstk::GPSLNavHealth*>(i.get()) != nullptr)
      {
         heaCount++;
      }
      else
      {
         otherCount++;
      }
   }
}


int main()
{
   PNBMultiGNSSNavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.addFactoryTest();
   errorTotal += testClass.constructorTest();
   errorTotal += testClass.setValidityFilterTest();
   errorTotal += testClass.setTypeFilterTest();
   errorTotal += testClass.addDataTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
