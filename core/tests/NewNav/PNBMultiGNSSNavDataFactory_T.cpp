//==============================================================================
//
//  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
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
#include "GPSLNavIono.hpp"
#include "PNBGPSCNavDataFactory.hpp"
#include "PNBGPSCNav2DataFactory.hpp"
#include "GPSCNavTimeOffset.hpp"
#include "GPSCNavHealth.hpp"
#include "GPSCNavEph.hpp"
#include "GPSCNavAlm.hpp"
#include "GPSCNavRedAlm.hpp"
#include "GPSCNavIono.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavValidityType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class TestClass : public gnsstk::PNBMultiGNSSNavDataFactory
{
public:
   static gnsstk::PNBNavDataFactoryMap& getFactories()
   { return factories(); }
};
class PNBTestFactory : public gnsstk::PNBNavDataFactory
{
public:
   bool addData(const gnsstk::PackedNavBitsPtr& navIn,
                gnsstk::NavDataPtrList& navOut, double cadence = -1) override
   { return false; }
   gnsstk::NavValidityType getValidity() const { return navValidity; }
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
   void countResults(const gnsstk::NavDataPtrList& navOut);

#include "LNavTestDataDecl.hpp"
#include "CNavTestDataDecl.hpp"

   void resetCount()
   { almCount = ephCount = toCount = heaCount = ionoCount = otherCount = 0; }
      /// Counts of messages, set by countResults.
   unsigned almCount, ephCount, toCount, heaCount, ionoCount, otherCount;
};


PNBMultiGNSSNavDataFactory_T ::
PNBMultiGNSSNavDataFactory_T()
{
   resetCount();
#include "LNavTestDataDef.hpp"
#include "CNavTestDataDef.hpp"
}


unsigned PNBMultiGNSSNavDataFactory_T ::
addFactoryTest()
{
   TUDEF("PNBMultiGNSSNavDataFactory", "addFactory()");
      // Add a test factory to the multi gnss set.
   gnsstk::PNBNavDataFactoryPtr test(std::make_shared<PNBTestFactory>());
   TUASSERTE(bool, true,
             gnsstk::PNBMultiGNSSNavDataFactory::addFactory(
                gnsstk::NavType::Unknown, test));
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
      gnsstk::PNBNavDataFactory *p = i.second.get();
      if (dynamic_cast<gnsstk::PNBGPSLNavDataFactory*>(p) != nullptr)
      {
         foundGPSLNav = true;
      }
      else if (dynamic_cast<gnsstk::PNBGPSCNavDataFactory*>(p) != nullptr)
      {
         foundGPSCNav = true;
      }
      else if (dynamic_cast<gnsstk::PNBGPSCNav2DataFactory*>(p) != nullptr)
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
   gnsstk::PNBMultiGNSSNavDataFactory uut;
   PNBTestFactory *fact = nullptr;
      // find our test factory so we can verify setValidityFilter
   for (auto& i : TestClass::getFactories())
   {
      gnsstk::PNBNavDataFactory *p = i.second.get();
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
   TUASSERTE(gnsstk::NavValidityType, gnsstk::NavValidityType::Any,
             fact->getValidity());
      // check possible values
   for (gnsstk::NavValidityType i : gnsstk::NavValidityTypeIterator())
   {
      uut.setValidityFilter(i);
      TUASSERTE(gnsstk::NavValidityType, i, fact->getValidity());
   }
   TURETURN();
}


unsigned PNBMultiGNSSNavDataFactory_T ::
setTypeFilterTest()
{
   TUDEF("PNBMultiGNSSNavDataFactory", "setTypeFilter()");
   gnsstk::PNBMultiGNSSNavDataFactory uut;
   PNBTestFactory *fact = nullptr;
      // find our test factory so we can verify setTypeFilter
   for (auto& i : TestClass::getFactories())
   {
      gnsstk::PNBNavDataFactory *p = i.second.get();
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
   uut.setTypeFilter({gnsstk::NavMessageType::Ephemeris});
   TUASSERTE(bool, true,  fact->getProcEph());
   TUASSERTE(bool, false, fact->getProcAlm());
   TUASSERTE(bool, false, fact->getProcHea());
   TUASSERTE(bool, false, fact->getProcTim());
   uut.setTypeFilter({gnsstk::NavMessageType::Almanac});
   TUASSERTE(bool, false, fact->getProcEph());
   TUASSERTE(bool, true,  fact->getProcAlm());
   TUASSERTE(bool, false, fact->getProcHea());
   TUASSERTE(bool, false, fact->getProcTim());
   uut.setTypeFilter({gnsstk::NavMessageType::Health});
   TUASSERTE(bool, false, fact->getProcEph());
   TUASSERTE(bool, false, fact->getProcAlm());
   TUASSERTE(bool, true,  fact->getProcHea());
   TUASSERTE(bool, false, fact->getProcTim());
   uut.setTypeFilter({gnsstk::NavMessageType::TimeOffset});
   TUASSERTE(bool, false, fact->getProcEph());
   TUASSERTE(bool, false, fact->getProcAlm());
   TUASSERTE(bool, false, fact->getProcHea());
   TUASSERTE(bool, true,  fact->getProcTim());
      // some subset tests.
   uut.setTypeFilter({gnsstk::NavMessageType::TimeOffset,
                      gnsstk::NavMessageType::Ephemeris});
   TUASSERTE(bool, true,  fact->getProcEph());
   TUASSERTE(bool, false, fact->getProcAlm());
   TUASSERTE(bool, false, fact->getProcHea());
   TUASSERTE(bool, true,  fact->getProcTim());
   uut.setTypeFilter({gnsstk::NavMessageType::Almanac,
                      gnsstk::NavMessageType::Health});
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
   gnsstk::PNBMultiGNSSNavDataFactory uut;
   gnsstk::NavDataPtrList navOut;
   uut.setTypeFilter(gnsstk::allNavMessageTypes);
   uut.setValidityFilter(gnsstk::NavValidityType::Any);
      //
      // Start with some GPS LNAV data.
      //
      // Add subframe 1, expect 1 health and 1 ISC.
      /// @todo Switch this test to use FactoryCounter and check for 1 ISC
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF1, navOut));
   TUASSERTE(size_t, 2, navOut.size());
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
   TUASSERTE(size_t, 2, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, toCount);
   TUASSERTE(unsigned, 1, ionoCount);
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
      /// @todo Switch this test to use FactoryCounter and check for 1 ISC
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   TUASSERTE(unsigned, 1, ionoCount);
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
      /// @todo Switch this test to use FactoryCounter and check for 1 ISC
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   TUASSERTE(unsigned, 1, ionoCount);
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
countResults(const gnsstk::NavDataPtrList& navOut)
{
   resetCount();
   for (const auto& i : navOut)
   {
      if (dynamic_cast<gnsstk::GPSLNavAlm*>(i.get()) != nullptr)
      {
         almCount++;
      }
      else if (dynamic_cast<gnsstk::GPSLNavEph*>(i.get()) != nullptr)
      {
         ephCount++;
      }
      else if (dynamic_cast<gnsstk::GPSLNavTimeOffset*>(i.get()) != nullptr)
      {
         toCount++;
      }
      else if (dynamic_cast<gnsstk::GPSLNavHealth*>(i.get()) != nullptr)
      {
         heaCount++;
      }
      else if (dynamic_cast<gnsstk::GPSLNavIono*>(i.get()) != nullptr)
      {
         ionoCount++;
      }
      else if (dynamic_cast<gnsstk::GPSCNavAlm*>(i.get()) != nullptr)
      {
         almCount++;
      }
      else if (dynamic_cast<gnsstk::GPSCNavEph*>(i.get()) != nullptr)
      {
         ephCount++;
      }
      else if (dynamic_cast<gnsstk::GPSCNavTimeOffset*>(i.get()) != nullptr)
      {
         toCount++;
      }
      else if (dynamic_cast<gnsstk::GPSCNavHealth*>(i.get()) != nullptr)
      {
         heaCount++;
      }
      else if (dynamic_cast<gnsstk::GPSCNavIono*>(i.get()) != nullptr)
      {
         ionoCount++;
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
