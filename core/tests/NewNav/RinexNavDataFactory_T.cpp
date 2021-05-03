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
#include "GalINavEph.hpp"
#include "GalFNavEph.hpp"
#include "GalINavHealth.hpp"
#include "RinexTimeOffset.hpp"
#include "GALWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::GPSLNavEph::L2Codes e)
   {
      s << static_cast<int>(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, GalHealthStatus e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, GalDataValid e)
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
      // count INAV, FNAV, and LNAV data
   unsigned ephICount = 0, ephFCount = 0, ephLCount = 0, heaICount = 0,
      heaLCount = 0, otherCount = 0;
   for (auto& nmti : f8.getData())
   {
      for (auto& sati : nmti.second)
      {
         for (auto& ti : sati.second)
         {
               // Count each data type and spot check the first one of each.
            gpstk::GalINavEph *ephI;
            gpstk::GalFnavEph *ephF;
            gpstk::GalINavHealth *heaI;
            gpstk::GPSLNavHealth *heaL;
            gpstk::GPSLNavEph *ephL;
            if ((ephI = dynamic_cast<gpstk::GalINavEph*>(ti.second.get()))
                != nullptr)
            {
               if (ephICount == 0)
               {
                  static const gpstk::CommonTime expToc =
                     gpstk::CivilTime(2020,5,29,0,10,0,gpstk::TimeSystem::GAL);
                     // yes, these are supposed to be GPS, see
                     // RinexNavDataFactory::fixTimeGalileo.
                  static const gpstk::CommonTime expToe =
                     gpstk::GPSWeekSecond(2107,432600);
                  static const gpstk::CommonTime expXmit1 =
                     gpstk::GPSWeekSecond(2107,433714);
                  static const gpstk::CommonTime expXmit2 = expXmit1+2.0;
                  static const gpstk::CommonTime expXmit3 = expXmit2+2.0;
                  static const gpstk::CommonTime expXmit4 = expXmit3+2.0;
                  static const gpstk::CommonTime expXmit5 = expXmit4+2.0;
                  static const gpstk::CommonTime expEnd = expToe + (3600.0*4.0);
                  static const gpstk::NavMessageID expNMID(
                     gpstk::NavSatelliteID(1, 1,
                                           gpstk::SatelliteSystem::Galileo,
                                           gpstk::CarrierBand::E5b,
                                           gpstk::TrackingCode::E5bI,
                                           gpstk::NavType::GalINAV),
                     gpstk::NavMessageType::Ephemeris);
                     // NavData
                  TUASSERTE(gpstk::CommonTime, expXmit1, ephI->timeStamp);
                  TUASSERTE(gpstk::NavMessageID, expNMID, ephI->signal);
                     // OrbitData doesn't actually have data.
                     // OrbitDataKepler
                  TUASSERTE(gpstk::CommonTime, expXmit1, ephI->xmitTime);
                  TUASSERTE(gpstk::CommonTime, expToe, ephI->Toe);
                  TUASSERTE(gpstk::CommonTime, expToc, ephI->Toc);
                  TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy,
                            ephI->health);
                  TUASSERTFE( 1.287087798119e-06, ephI->Cuc);
                  TUASSERTFE( 8.314847946167e-06, ephI->Cus);
                  TUASSERTFE( 1.744062500000e+02, ephI->Crc);
                  TUASSERTFE( 2.890625000000e+01, ephI->Crs);
                  TUASSERTFE( 3.911554813385e-08, ephI->Cic);
                  TUASSERTFE(-1.229345798492e-07, ephI->Cis);
                  TUASSERTFE(-4.933573954815e-01, ephI->M0);
                  TUASSERTFE( 2.787616115400e-09, ephI->dn);
                  TUASSERTFE(0, ephI->dndot);
                  TUASSERTFE( 6.772601045668e-05, ephI->ecc);
                  TUASSERTFE( 5.440601449966e+03*5.440601449966e+03, ephI->A);
                  TUASSERTFE( 5.440601449966e+03, ephI->Ahalf);
                  TUASSERTFE(0, ephI->Adot);
                  TUASSERTFE( 7.066878003606e-01, ephI->OMEGA0);
                  TUASSERTFE( 9.830061444145e-01, ephI->i0);
                  TUASSERTFE( 2.279243758668e+00, ephI->w);
                  TUASSERTFE(-5.394510417298e-09, ephI->OMEGAdot);
                  TUASSERTFE(-5.064496670900e-10, ephI->idot);
                  TUASSERTFE(-8.662177133374e-04, ephI->af0);
                  TUASSERTFE(-7.943867785798e-12, ephI->af1);
                  TUASSERTFE( 0.000000000000e+00, ephI->af2);
                  TUASSERTE(gpstk::CommonTime, expXmit1, ephI->beginFit);
                  TUASSERTE(gpstk::CommonTime, expEnd, ephI->endFit);
                     // GalINavEph
                  TUASSERTFE(-1.862645149231e-09, ephI->bgdE5aE1);
                  TUASSERTFE(-2.095475792885e-09, ephI->bgdE5bE1);
                  TUASSERTE(unsigned, 107, ephI->sisaIndex);
                  TUASSERTE(unsigned, 1, ephI->svid);
                  TUASSERTE(gpstk::CommonTime, expXmit2, ephI->xmit2);
                  TUASSERTE(gpstk::CommonTime, expXmit3, ephI->xmit3);
                  TUASSERTE(gpstk::CommonTime, expXmit4, ephI->xmit4);
                  TUASSERTE(gpstk::CommonTime, expXmit5, ephI->xmit5);
                  TUASSERTE(uint16_t, 81, ephI->iodnav1);
                  TUASSERTE(uint16_t, 81, ephI->iodnav2);
                  TUASSERTE(uint16_t, 81, ephI->iodnav3);
                  TUASSERTE(uint16_t, 81, ephI->iodnav4);
                  TUASSERTE(gpstk::GalHealthStatus,
                            gpstk::GalHealthStatus::OK, ephI->hsE5b);
                  TUASSERTE(gpstk::GalHealthStatus,
                            gpstk::GalHealthStatus::OK, ephI->hsE1B);
                  TUASSERTE(gpstk::GalDataValid, gpstk::GalDataValid::Valid,
                            ephI->dvsE5b);
                  TUASSERTE(gpstk::GalDataValid, gpstk::GalDataValid::Valid,
                            ephI->dvsE1B);
               }
               ephICount++;
            }
            else if ((ephF = dynamic_cast<gpstk::GalFnavEph*>(ti.second.get()))
                != nullptr)
            {
               ephFCount++;
                  /** @todo check these data when we have a fuller
                   * F/NAV implementation */
            }
            else if ((ephL = dynamic_cast<gpstk::GPSLNavEph*>(ti.second.get()))
                != nullptr)
            {
               if (ephLCount == 0)
               {
                  static const gpstk::CommonTime expToc =
                     gpstk::CivilTime(2020,5,29,2,0,0,gpstk::TimeSystem::GPS);
                  static const gpstk::CommonTime expToe =
                     gpstk::GPSWeekSecond(2107,4.392000000000e+05);
                  static const gpstk::CommonTime expXmit1 =
                     gpstk::GPSWeekSecond(2107,4.320000000000e+05);
                  static const gpstk::CommonTime expTS =
                     gpstk::GPSWeekSecond(2107,4.320180000000e+05);
                  static const gpstk::CommonTime expEnd = expToe + (3600.0*2.0);
                  static const gpstk::NavMessageID expNMID(
                     gpstk::NavSatelliteID(1, 1,
                                           gpstk::SatelliteSystem::GPS,
                                           gpstk::CarrierBand::L1,
                                           gpstk::TrackingCode::CA,
                                           gpstk::NavType::GPSLNAV),
                     gpstk::NavMessageType::Ephemeris);
                     // NavData
                  TUASSERTE(gpstk::CommonTime, expTS, ephL->timeStamp);
                  TUASSERTE(gpstk::NavMessageID, expNMID, ephL->signal);
                     // OrbitData doesn't actually have data.
                     // OrbitDataKepler
                  TUASSERTE(gpstk::CommonTime, expXmit1, ephL->xmitTime);
                  TUASSERTE(gpstk::CommonTime, expToe, ephL->Toe);
                  TUASSERTE(gpstk::CommonTime, expToc, ephL->Toc);
                  TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy,
                            ephL->health);
                  TUASSERTFE(-1.315027475357e-06, ephL->Cuc);
                  TUASSERTFE( 3.149732947350e-06, ephL->Cus);
                  TUASSERTFE( 3.250937500000e+02, ephL->Crc);
                  TUASSERTFE(-2.712500000000e+01, ephL->Crs);
                  TUASSERTFE( 2.365559339523e-07, ephL->Cic);
                  TUASSERTFE( 1.490116119385e-08, ephL->Cis);
                  TUASSERTFE(-1.365560667506e+00, ephL->M0);
                  TUASSERTFE( 4.307322274405e-09, ephL->dn);
                  TUASSERTFE(0, ephL->dndot);
                  TUASSERTFE( 9.911696310155e-03, ephL->ecc);
                  TUASSERTFE( 5.153623668671e+03*5.153623668671e+03, ephL->A);
                  TUASSERTFE( 5.153623668671e+03, ephL->Ahalf);
                  TUASSERTFE(0, ephL->Adot);
                  TUASSERTFE( 3.072919475362e+00, ephL->OMEGA0);
                  TUASSERTFE( 9.804866571591e-01, ephL->i0);
                  TUASSERTFE( 7.817788821256e-01, ephL->w);
                  TUASSERTFE(-8.079265105250e-09, ephL->OMEGAdot);
                  TUASSERTFE( 1.810789712374e-10, ephL->idot);
                  TUASSERTFE(-3.862413577735e-04, ephL->af0);
                  TUASSERTFE(-3.637978807092e-12, ephL->af1);
                  TUASSERTFE( 0.000000000000e+00, ephL->af2);
                  TUASSERTE(gpstk::CommonTime, expXmit1, ephL->beginFit);
                  TUASSERTE(gpstk::CommonTime, expEnd, ephL->endFit);
                     // GPSLNavData
                  TUASSERTE(uint32_t, 0, ephL->pre);
                  TUASSERTE(uint32_t, 0, ephL->tlm);
                  TUASSERTE(bool, false, ephL->alert);
                  TUASSERTE(bool, true, ephL->asFlag);
                     // GPSLNavEph
                  TUASSERTE(uint32_t, 0, ephL->pre2);
                  TUASSERTE(uint32_t, 0, ephL->tlm2);
                  TUASSERTE(bool, false, ephL->alert2);
                  TUASSERTE(bool, true, ephL->asFlag2);
                  TUASSERTE(uint32_t, 0, ephL->pre3);
                  TUASSERTE(uint32_t, 0, ephL->tlm3);
                  TUASSERTE(bool, false, ephL->alert3);
                  TUASSERTE(bool, true, ephL->asFlag3);
                  TUASSERTE(uint16_t, 48, ephL->iodc);
                  TUASSERTE(uint16_t, 48, ephL->iode);
                  TUASSERTE(unsigned, 0, ephL->fitIntFlag);
                  TUASSERTE(unsigned, 0, ephL->healthBits);
                  TUASSERTE(unsigned, 0, ephL->uraIndex); // 2m
                  TUASSERTFE( 5.122274160385e-09, ephL->tgd);
                  TUASSERTE(gpstk::GPSLNavEph::L2Codes,
                            gpstk::GPSLNavEph::L2Codes::Pcode, ephL->codesL2);
                  TUASSERTE(bool, false, ephL->L2Pdata);
               }
               ephLCount++;
            }
            else if ((heaI=dynamic_cast<gpstk::GalINavHealth*>(ti.second.get()))
                != nullptr)
            {
               if (heaICount == 0)
               {
                  static const gpstk::CommonTime expXmit1 =
                     gpstk::GPSWeekSecond(2107,433714);
                     /// @note this may change when we fix F/NAV support
                  static const gpstk::NavMessageID expNMID(
                     gpstk::NavSatelliteID(1, 1,
                                           gpstk::SatelliteSystem::Galileo,
                                           gpstk::CarrierBand::L1,
                                           gpstk::TrackingCode::E1B,
                                           gpstk::NavType::GalINAV),
                     gpstk::NavMessageType::Health);
                     // NavData
                  TUASSERTE(gpstk::CommonTime, expXmit1, heaI->timeStamp);
                  TUASSERTE(gpstk::NavMessageID, expNMID, heaI->signal);
                     // NavHealthData has nothing.
                     // GalINavHealth
                  TUASSERTE(gpstk::GalHealthStatus,
                            gpstk::GalHealthStatus::OK, heaI->sigHealthStatus);
                  TUASSERTE(gpstk::GalDataValid, gpstk::GalDataValid::Valid,
                            heaI->dataValidityStatus);
                     /** @todo This is probably due to our handling of
                      * F/NAV and I/NAV health under the assumption
                      * that both are valid in a given record.  See
                      * RinexNavDataFactory::convertToHealth. */
                  TUASSERTE(unsigned, 255, heaI->sisaIndex);
               }
               heaICount++;
            }
            else if ((heaL=dynamic_cast<gpstk::GPSLNavHealth*>(ti.second.get()))
                != nullptr)
            {
               if (heaLCount == 0)
               {
                  static const gpstk::CommonTime expTS =
                     gpstk::GPSWeekSecond(2107,4.320180000000e+05);
                  static const gpstk::NavMessageID expNMID(
                     gpstk::NavSatelliteID(1, 1,
                                           gpstk::SatelliteSystem::GPS,
                                           gpstk::CarrierBand::L1,
                                           gpstk::TrackingCode::CA,
                                           gpstk::NavType::GPSLNAV),
                     gpstk::NavMessageType::Health);
                     // NavData
                  TUASSERTE(gpstk::CommonTime, expTS, heaL->timeStamp);
                  TUASSERTE(gpstk::NavMessageID, expNMID, heaL->signal);
                     // NavHealthData has nothing.
                     // GPSLNavHealth
                  TUASSERTE(unsigned, 0, heaL->svHealth);
               }
               heaLCount++;
            }
            else
            {
               otherCount++;
            }
         }
      }
   }
   TUASSERTE(unsigned, 36, ephICount);
   TUASSERTE(unsigned, 3, ephFCount);
   TUASSERTE(unsigned, 1, ephLCount);
      /// @bug are we producing "I/NAV" health containing data from F/NAV?
   TUASSERTE(unsigned, 39*3, heaICount);
   TUASSERTE(unsigned, 1, heaLCount);
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

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
