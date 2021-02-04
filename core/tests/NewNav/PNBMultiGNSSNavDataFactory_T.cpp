//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//  
//  This software was developed by Applied Research Laboratories at the 
//  University of Texas at Austin.
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
//
//==============================================================================


//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the 
//  University of Texas at Austin, under contract to an agency or agencies 
//  within the U.S. Department of Defense. The U.S. Government retains all 
//  rights to use, duplicate, distribute, disclose, or release this software. 
//
//  Pursuant to DoD Directive 523024 
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public 
//                            release, distribution is unlimited.
//
//==============================================================================
#include "PNBMultiGNSSNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "PNBGPSLNavDataFactory.hpp"
#include "GPSLNavTimeOffset.hpp"
#include "GPSLNavHealth.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavAlm.hpp"
#include "PNBGPSCNavDataFactory.hpp"
#include "PNBGPSCNav2DataFactory.hpp"
#include "GPSCNavTimeOffset.hpp"
#include "GPSCNavHealth.hpp"
#include "GPSCNavEph.hpp"
#include "GPSCNavAlm.hpp"
#include "GPSCNavRedAlm.hpp"

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
   { return factories(); }
};
class PNBTestFactory : public gpstk::PNBNavDataFactory
{
public:
   bool addData(const gpstk::PackedNavBitsPtr& navIn,
                gpstk::NavDataPtrList& navOut, double cadence = -1) override
   { return false; }
   gpstk::NavValidityType getValidity() const { return navValidity; }
   bool getProcEph() const { return processEph; }
   bool getProcAlm() const { return processAlm; }
   bool getProcHea() const { return processHea; }
   bool getProcTim() const { return processTim; }
   void resetState() override {}
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

#include "LNavTestDataDecl.hpp"
#include "CNavTestDataDecl.hpp"

      /// Counts of messages, set by countResults.
   unsigned almCount, ephCount, toCount, heaCount, otherCount;
};


PNBMultiGNSSNavDataFactory_T ::
PNBMultiGNSSNavDataFactory_T()
      : almCount(0), ephCount(0), toCount(0), heaCount(0), otherCount(0)
{
#include "LNavTestDataDef.hpp"
#include "CNavTestDataDef.hpp"
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
   bool foundGPSCNav = false;
   bool foundGPSCNav2 = false;
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
      else if (dynamic_cast<gpstk::PNBGPSCNavDataFactory*>(p) != nullptr)
      {
         foundGPSCNav = true;
      }
      else if (dynamic_cast<gpstk::PNBGPSCNav2DataFactory*>(p) != nullptr)
      {
         foundGPSCNav2 = true;
      }
      else if (dynamic_cast<PNBTestFactory*>(p) != nullptr)
      {
         foundTest = true;
      }
   }
   TUASSERT(foundGPSLNav);
   TUASSERT(foundGPSCNav);
   TUASSERT(foundGPSCNav2);
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
   TUASSERTE(gpstk::NavValidityType, gpstk::NavValidityType::Any,
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
   uut.setValidityFilter(gpstk::NavValidityType::Any);
      //
      // Start with some GPS LNAV data.
      //
      // Add subframe 1, expect only health initially.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF1, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // add subframe 2, expect nothing yet
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add subframe 3, expect the completed ephemeris
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF3, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   navOut.clear();
      // add almanac sv id 25, expect health
   TUASSERTE(bool, true, uut.addData(almLNAVGPS25, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // add almanac sv id 26, expect health
   TUASSERTE(bool, true, uut.addData(almLNAVGPS26, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // add page 56, expect time offset
   TUASSERTE(bool, true, uut.addData(pg56LNAVGPS, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, toCount);
   navOut.clear();
      // add page 63, expect 8 health
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   TUASSERTE(size_t, 8, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 8, heaCount);
   navOut.clear();
      // add page 51, expect 24 health and 2 almanacs
   TUASSERTE(bool, true, uut.addData(pg51LNAVGPS, navOut));
   TUASSERTE(size_t, 26, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 24, heaCount);
   TUASSERTE(unsigned, 2, almCount);
   navOut.clear();
      //
      // GPS CNav data (L2)
      //
      // get 3 health from ephemeris 1
   TUASSERTE(bool, true, uut.addData(msg10CNAVGPSL2, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // clock data completes the ephemeris
   TUASSERTE(bool, true, uut.addData(msg30CNAVGPSL2, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   navOut.clear();
      // nothing in message type 32 that we care about (not completing
      // an ephemeris)
   TUASSERTE(bool, true, uut.addData(msg32CNAVGPSL2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // expecting a time offset record from 33
   TUASSERTE(bool, true, uut.addData(msg33CNAVGPSL2, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, toCount);
   navOut.clear();
      //
      // QZSS CNav data (L5)
      //
      // get 3 health from ephemeris 1
   TUASSERTE(bool, true, uut.addData(msg10CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
      // nothing from ephemeris 2 (incomplete ephemeris)
   TUASSERTE(bool, true, uut.addData(msg11CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // clock data completes the ephemeris
   TUASSERTE(bool, true, uut.addData(msg30CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   navOut.clear();
      // nothing in message type 32 that we care about (not completing
      // an ephemeris)
   TUASSERTE(bool, true, uut.addData(msg32CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // expecting an almanac and 3 health from message type 37
   TUASSERTE(bool, true, uut.addData(msg37CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 4, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, almCount);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
      // expecting 4 almanacs and 12 health from message type 31
   TUASSERTE(bool, true, uut.addData(msg31CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 16, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 4, almCount);
   TUASSERTE(unsigned, 12, heaCount);
   navOut.clear();
      // expecting nothing from message type 12 because it's empty.
   TUASSERTE(bool, true, uut.addData(msg12CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // expecting a time offset record from 35
   TUASSERTE(bool, true, uut.addData(msg35CNAVQZSSL5, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, toCount);
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
      else if (dynamic_cast<gpstk::GPSCNavAlm*>(i.get()) != nullptr)
      {
         almCount++;
      }
      else if (dynamic_cast<gpstk::GPSCNavEph*>(i.get()) != nullptr)
      {
         ephCount++;
      }
      else if (dynamic_cast<gpstk::GPSCNavTimeOffset*>(i.get()) != nullptr)
      {
         toCount++;
      }
      else if (dynamic_cast<gpstk::GPSCNavHealth*>(i.get()) != nullptr)
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
