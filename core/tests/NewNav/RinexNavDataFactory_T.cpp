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
   std::ostream& operator<<(std::ostream& s, GPSLNavEph::L2Codes e)
   {
      s << static_cast<int>(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
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
      /// Exercise loadIntoMap with QZSS data.
   unsigned loadIntoMapQZSSTest();
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
   TUASSERT(fact.supportedSignals.count(nsid1));
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

   TURETURN();
}


unsigned RinexNavDataFactory_T ::
loadIntoMapQZSSTest()
{
   TUDEF("RinexNavDataFactory", "loadIntoMap(QZSS)");
   TestClass uut;
   std::string fname = gpstk::getPathData() + gpstk::getFileSep() +
      "qzsssampl_U_20141330729_01D_RN.rnx";
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(193, 193, gpstk::SatelliteSystem::QZSS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                            gpstk::NavType::GPSLNAV),
      gpstk::NavMessageType::Health);
      /// @todo should these be in QZSS time? They come out of RINEX as GPS.
   gpstk::CommonTime expTS = gpstk::CivilTime(2014,5,13,7,15,0,
                                              gpstk::TimeSystem::QZS);
   gpstk::CommonTime expXT = gpstk::CivilTime(2014,5,13,7,15,0,
                                               gpstk::TimeSystem::QZS);
   gpstk::CommonTime expti2 = gpstk::CivilTime(2014,5,13,7,15,0,
                                               gpstk::TimeSystem::QZS);
   gpstk::CommonTime exptf2 = gpstk::CivilTime(2014,5,13,10,30,0,
                                               gpstk::TimeSystem::QZS);
   gpstk::CommonTime toeExp = gpstk::GPSWeekSecond(1792,202512,
                                                   gpstk::TimeSystem::QZS);
   gpstk::CommonTime beginExp = gpstk::GPSWeekSecond(1792, 198900,
                                                     gpstk::TimeSystem::QZS);
   gpstk::CommonTime endExp = gpstk::GPSWeekSecond(1792, 210600,
                                                   gpstk::TimeSystem::QZS);
   TUASSERT(uut.addDataSource(fname));
   TUASSERTE(size_t, 2, uut.size());
   gpstk::NavMessageMap &nmm(uut.getData());
   uut.dump(std::cerr, gpstk::DumpDetail::Full);
   TUASSERTE(gpstk::CommonTime, expti2, uut.getInitialTime());
   TUASSERTE(gpstk::CommonTime, exptf2, uut.getFinalTime());
   gpstk::GPSLNavEph *eph;
   gpstk::GPSLNavHealth *hea;
   unsigned heaCount = 0, ephCount = 0, otherCount = 0;
   for (const auto& nmti : nmm)
   {
      for (const auto& sati : nmti.second)
      {
         for (const auto& ti : sati.second)
         {
            if ((eph = dynamic_cast<gpstk::GPSLNavEph*>(ti.second.get()))
                != nullptr)
            {
               ephCount++;
               nmidExp.messageType = gpstk::NavMessageType::Ephemeris;
                  // NavData fields
               TUASSERTE(gpstk::CommonTime, expTS, eph->timeStamp);
               TUASSERTE(gpstk::NavMessageID, nmidExp, eph->signal);
                  // OrbitData has no fields
                  // OrbitDataKepler fields
               TUASSERTE(gpstk::CommonTime, expXT, eph->xmitTime);
               TUASSERTE(gpstk::CommonTime, toeExp, eph->Toe);
                  // same value as toe
               TUASSERTE(gpstk::CommonTime, toeExp, eph->Toc);
               TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unhealthy,
                         eph->health);
               TUASSERTFE(-1.654587686062e-05, eph->Cuc);
               TUASSERTFE( 1.197867095470e-05, eph->Cus);
               TUASSERTFE(-1.558437500000e+02, eph->Crc);
               TUASSERTFE(-4.927812500000e+02, eph->Crs);
               TUASSERTFE(-2.041459083557e-06, eph->Cis);
               TUASSERTFE(-8.381903171539e-07, eph->Cic);
               TUASSERTFE( 7.641996743610e-01, eph->M0);
               TUASSERTFE( 2.222949737636e-09, eph->dn);
               TUASSERTFE(0, eph->dndot);
               TUASSERTFE( 7.542252133135e-02, eph->ecc);
               TUASSERTFE( 6.492895933151e+03, eph->Ahalf);
               TUASSERTFE( 6.492895933151e+03 *  6.492895933151e+03, eph->A);
               TUASSERTFE(0, eph->Adot);
               TUASSERTFE(-9.211997910060e-01, eph->OMEGA0);
               TUASSERTFE( 7.082252892260e-01, eph->i0);
               TUASSERTFE(-1.575843337115e+00, eph->w);
               TUASSERTFE(-2.349740733276e-09, eph->OMEGAdot);
               TUASSERTFE(-6.793140104410e-10, eph->idot);
               TUASSERTFE( 3.323303535581e-04, eph->af0);
               TUASSERTFE(-1.818989403546e-11, eph->af1);
               TUASSERTFE(0, eph->af2);
               TUASSERTE(gpstk::CommonTime, beginExp, eph->beginFit);
               TUASSERTE(gpstk::CommonTime, endExp, eph->endFit);
                  // GPSLNavData fields
               TUASSERTE(uint32_t, 0, eph->pre);
               TUASSERTE(uint32_t, 0, eph->tlm);
               TUASSERTE(bool, false, eph->alert);
               TUASSERTE(bool, false, eph->asFlag);
                  // GPSLNavEph fields
               TUASSERTE(uint32_t, 0, eph->pre2);
               TUASSERTE(uint32_t, 0, eph->tlm2);
               TUASSERTE(uint32_t, 0, eph->pre3);
               TUASSERTE(uint32_t, 0, eph->tlm3);
               TUASSERTE(uint16_t, 69, eph->iodc);
               TUASSERTE(uint16_t, 69, eph->iode);
               TUASSERTE(unsigned, 0, eph->fitIntFlag);
               TUASSERTE(unsigned, 1, eph->healthBits);
               TUASSERTE(unsigned, 0, eph->uraIndex);
               TUASSERTFE(-4.656612873077e-09, eph->tgd);
               TUASSERTE(bool, false, eph->alert2);
               TUASSERTE(bool, false, eph->alert3);
               TUASSERTE(bool, false, eph->asFlag2);
               TUASSERTE(bool, false, eph->asFlag3);
               TUASSERTE(gpstk::GPSLNavEph::L2Codes,
                         gpstk::GPSLNavEph::L2Codes::CAcode, eph->codesL2);
               TUASSERTE(bool, true, eph->L2Pdata);
            }
            else if ((hea=dynamic_cast<gpstk::GPSLNavHealth*>(ti.second.get()))
                     != nullptr)
            {
               heaCount++;
            }
            else
            {
               otherCount++;
            }
         }
      }
   }
   TUASSERTE(unsigned, 1, ephCount);
   TUASSERTE(unsigned, 1, heaCount);
   TUASSERTE(unsigned, 0, otherCount);
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
   errorTotal += testClass.loadIntoMapQZSSTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
