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
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
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
#include "GPSNavConfig.hpp"
#include "SEMNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavHealth.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

   /** Implement a test class to expose protected members rather than
    * using friends. */
class TestClass : public gnsstk::SEMNavDataFactory
{
public:
      /// Grant access to protected data.
   gnsstk::NavMessageMap& getData()
   { return data; }
};

/// Automated tests for gnsstk::SEMNavDataFactory
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
   void verifyDataType(gnsstk::TestUtil& testFramework,
                       gnsstk::NavMessageMap& nmm);
};


unsigned SEMNavDataFactory_T ::
constructorTest()
{
   TUDEF("SEMNavDataFactory", "SEMNavDataFactory");
   gnsstk::SEMNavDataFactory fact;
      // check for expected signal support
   gnsstk::NavSignalID nsid1(gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::CA,
                            gnsstk::NavType::GPSLNAV);
   TUASSERT(fact.supportedSignals.count(nsid1));
   TURETURN();
}


unsigned SEMNavDataFactory_T ::
loadIntoMapTest()
{
   TUDEF("SEMNavDataFactory", "loadIntoMap");

      // test loading SEM 2 nav
   gnsstk::SEMNavDataFactory f2;
   std::string f2name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_sem387.txt";
      // this should implicitly load into the data map
   TUASSERT(f2.addDataSource(f2name));
   TUASSERTE(size_t, 60, f2.size());

   TestClass f4;
   std::string f4name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_sem387.txt";
   TUCATCH(f4.setTypeFilter({gnsstk::NavMessageType::Almanac}));
      // this should implicitly load into the data map
   TUASSERT(f4.addDataSource(f4name));
   TUASSERTE(size_t, 30, f4.size());
   gnsstk::NavMessageMap &nmm4(f4.getData());
      // only one message type
   TUASSERTE(size_t, 1, nmm4.size());
      // and it's almanac.
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Almanac,
             nmm4.begin()->first);
   TUCSM("convertToOrbit/fillNavData");
   verifyDataType<gnsstk::GPSLNavAlm>(testFramework, nmm4);
   TUCSM("loadIntoMap");

   TestClass f5;
   std::string f5name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_sem387.txt";
   TUCATCH(f5.setTypeFilter({gnsstk::NavMessageType::Health}));
      // this should implicitly load into the data map
   TUASSERT(f5.addDataSource(f5name));
   TUASSERTE(size_t, 30, f5.size());
   gnsstk::NavMessageMap &nmm5(f5.getData());
      // only one message type
   TUASSERTE(size_t, 1, nmm5.size());
      // and it's health.
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Health,
             nmm5.begin()->first);
   TUCSM("convertToHealth/fillNavData");
   verifyDataType<gnsstk::GPSLNavHealth>(testFramework, nmm5);
   TUCSM("loadIntoMap");

   TestClass f6;
   std::string f6name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_sem387.txt";
   TUCATCH(f6.setTypeFilter({gnsstk::NavMessageType::Ephemeris}));
      // this should implicitly load into the data map
   TUASSERT(f6.addDataSource(f6name));
      // except there isn't any data, because SEM nav doesn't contain ephemeris
   TUASSERTE(size_t, 0, f6.size());

      // test loading something that isn't SEM
   TestClass f7;
   std::string f7name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUASSERT(!f7.addDataSource(f7name));
   TUASSERTE(size_t, 0, f7.size());

   TestClass f8;
   std::string f8name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_sem387.txt";
   TUCATCH(f8.setTypeFilter({gnsstk::NavMessageType::System}));
      // this should implicitly load into the data map
   TUASSERT(f8.addDataSource(f8name));
   TUASSERTE(size_t, 30, f8.size());
   gnsstk::NavMessageMap &nmm8(f8.getData());
      // only one message type
   TUASSERTE(size_t, 1, nmm8.size());
      // and it's SV config.
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::System,
             nmm8.begin()->first);
   TUCSM("convertToHealth/fillNavData");
   verifyDataType<gnsstk::GPSNavConfig>(testFramework, nmm8);
   TUCSM("loadIntoMap");

   TURETURN();
}


template <class NavClass>
void SEMNavDataFactory_T ::
verifyDataType(gnsstk::TestUtil& testFramework,
               gnsstk::NavMessageMap& nmm)
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
   SEMNavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.loadIntoMapTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
