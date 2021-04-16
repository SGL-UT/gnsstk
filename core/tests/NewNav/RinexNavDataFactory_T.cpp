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
#include "RinexNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavHealth.hpp"
#include "RinexTimeOffset.hpp"

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
class TestClass : public gpstk::RinexNavDataFactory
{
public:
      /// Grant access to protected data.
   gpstk::NavMessageMap& getData()
   { return data; }
};

/// Automated tests for gpstk::RinexNavDataFactory
class RinexNavDataFactory_T
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


unsigned RinexNavDataFactory_T ::
constructorTest()
{
   TUDEF("RinexNavDataFactory", "RinexNavDataFactory");
   gpstk::RinexNavDataFactory fact;
      // check for expected signal support
   gpstk::NavSignalID nsid1(gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::CA,
                            gpstk::NavType::GPSLNAV);
   gpstk::NavSignalID nsid2(gpstk::SatelliteSystem::Galileo,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::E1B,
                            gpstk::NavType::GalINAV);
   gpstk::NavSignalID nsid3(gpstk::SatelliteSystem::Galileo,
                            gpstk::CarrierBand::E5b,
                            gpstk::TrackingCode::E5bI,
                            gpstk::NavType::GalINAV);
   gpstk::NavSignalID nsid4(gpstk::SatelliteSystem::Galileo,
                            gpstk::CarrierBand::L5,
                            gpstk::TrackingCode::E5aI,
                            gpstk::NavType::GalFNAV);
   TUASSERT(fact.supportedSignals.count(nsid1));
   TUASSERT(fact.supportedSignals.count(nsid2));
   TUASSERT(fact.supportedSignals.count(nsid3));
   TUASSERT(fact.supportedSignals.count(nsid4));
   TURETURN();
}


unsigned RinexNavDataFactory_T ::
loadIntoMapTest()
{
   TUDEF("RinexNavDataFactory", "loadIntoMap");

      // test loading RINEX 2 nav
   gpstk::RinexNavDataFactory f2;
   std::string f2name = gpstk::getPathData() + gpstk::getFileSep() +
      "arlm2000.15n";
      // this should implicitly load into the data map
   TUASSERT(f2.addDataSource(f2name));
   TUASSERTE(size_t, 337, f2.size());
   gpstk::CommonTime expti2 = gpstk::CivilTime(2015,7,19,0,0,0,
                                               gpstk::TimeSystem::GPS);
   gpstk::CommonTime exptf2 = gpstk::CivilTime(2015,7,20,2,0,0,
                                               gpstk::TimeSystem::GPS);
   TUASSERTE(gpstk::CommonTime, expti2, f2.getInitialTime());
   TUASSERTE(gpstk::CommonTime, exptf2, f2.getFinalTime());

   TUCSM("convertToOffset");
   gpstk::NavDataPtr navPtr;
   double offsVal = 666;
   gpstk::CommonTime refTime = gpstk::GPSWeekSecond(1854,233472);
   TUASSERTE(bool, true, f2.getOffset(gpstk::TimeSystem::GPS,
                                      gpstk::TimeSystem::UTC, refTime, navPtr));
   gpstk::RinexTimeOffset *rto = dynamic_cast<gpstk::RinexTimeOffset*>(
      navPtr.get());
   TUASSERTE(bool, true, rto->getOffset(gpstk::TimeSystem::GPS,
                                        gpstk::TimeSystem::UTC, refTime,
                                        offsVal));
      //         123456789012345678
      //                  931322575 (test 1 and test 2)
      //                       3553 (test 2 only)
   TUASSERTFE(17.000000000931322575, offsVal);
      // add one second to ref time to force the A1 term to come into play
   refTime += 1.0;
   TUASSERTE(bool, true, rto->getOffset(gpstk::TimeSystem::GPS,
                                        gpstk::TimeSystem::UTC, refTime,
                                        offsVal));
   TUASSERTFE(17.000000000931326128, offsVal);
   TUCSM("loadIntoMap");

   gpstk::RinexNavDataFactory f3;
   std::string f3name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_rinex3_76193040.14n";
      // this should implicitly load into the data map
   TUASSERT(f3.addDataSource(f3name));
   TUASSERTE(size_t, 27, f3.size());

   TestClass f4;
   std::string f4name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_rinex3_76193040.14n";
   TUCATCH(f4.setTypeFilter({gpstk::NavMessageType::Ephemeris}));
      // this should implicitly load into the data map
   TUASSERT(f4.addDataSource(f4name));
   TUASSERTE(size_t, 13, f4.size());
   gpstk::NavMessageMap &nmm4(f4.getData());
      // only one message type
   TUASSERTE(size_t, 1, nmm4.size());
      // and it's ephemeris.
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             nmm4.begin()->first);
   TUCSM("convertToOrbit/fillNavData");
   verifyDataType<gpstk::GPSLNavEph>(testFramework, nmm4);
   TUCSM("loadIntoMap");

   TestClass f5;
   std::string f5name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_rinex3_76193040.14n";
   TUCATCH(f5.setTypeFilter({gpstk::NavMessageType::Health}));
      // this should implicitly load into the data map
   TUASSERT(f5.addDataSource(f5name));
   TUASSERTE(size_t, 13, f5.size());
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
      "test_input_rinex3_76193040.14n";
   TUCATCH(f6.setTypeFilter({gpstk::NavMessageType::Almanac}));
      // this should implicitly load into the data map
   TUASSERT(f6.addDataSource(f6name));
      // except there isn't any data, because RINEX NAV doesn't contain almanacs
   TUASSERTE(size_t, 0, f6.size());

      // test loading something that isn't rinex
   TestClass f7;
   std::string f7name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUASSERT(!f7.addDataSource(f7name));
   TUASSERTE(size_t, 0, f7.size());

      // test RINEX 3 Galileo
   TestClass f8;
   std::string f8name = gpstk::getPathData() + gpstk::getFileSep() +
      "test_input_rinex3_nav_gal.20n";
      // this should implicitly load into the data map
   TUASSERT(f8.addDataSource(f8name));
      // x 4 time offset
      // x 1 GPS ephemeris
      // x 1 GPS health
      // x 39 Galileo ephemerides
      // x 3*39 Galileo health
   TUASSERTE(size_t, 162, f8.size());
   TURETURN();
}


template <class NavClass>
void RinexNavDataFactory_T ::
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
   RinexNavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.loadIntoMapTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
