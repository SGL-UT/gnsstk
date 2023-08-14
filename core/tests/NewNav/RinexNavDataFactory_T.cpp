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
#include <math.h>
#include "RinexNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavHealth.hpp"
#include "GPSLNavIono.hpp"
#include "GPSLNavISC.hpp"
#include "GalINavEph.hpp"
#include "GalINavIono.hpp"
#include "GalINavISC.hpp"
#include "GalFNavEph.hpp"
#include "GalINavHealth.hpp"
#include "GalFNavHealth.hpp"
#include "BDSD1NavEph.hpp"
#include "BDSD1NavHealth.hpp"
#include "BDSD1NavIono.hpp"
#include "BDSD1NavISC.hpp"
#include "BDSD2NavEph.hpp"
#include "BDSD2NavHealth.hpp"
#include "BDSD2NavISC.hpp"
#include "BDSWeekSecond.hpp"
#include "GLOFNavEph.hpp"
#include "GLOFNavHealth.hpp"
#include "GLOFNavISC.hpp"
#include "RinexTimeOffset.hpp"
#include "GALWeekSecond.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::GPSLNavL2Codes e)
   {
      s << static_cast<int>(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
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
   std::ostream& operator<<(std::ostream& s, GLOFNavPCode e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, GLOFNavSatType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

   /** Implement a test class to expose protected members rather than
    * using friends. */
class TestClass : public gnsstk::RinexNavDataFactory
{
public:
      /// Grant access to protected data.
   gnsstk::NavMessageMap& getData()
   { return data; }
};

/// Automated tests for gnsstk::RinexNavDataFactory
class RinexNavDataFactory_T
{
public:
      /// Make sure the constructor does what it's supposed to.
   unsigned constructorTest();
      /// Exercise loadIntoMap by loading data with different options in place.
   unsigned loadIntoMapTest();
      /// Exercise loadIntoMap with QZSS data.
   unsigned loadIntoMapQZSSTest();
   unsigned decodeSISATest();
   unsigned encodeSISATest();
      /** Use dynamic_cast to verify that the contents of nmm are the
       * right class.
       * @param[in] testFramework The test framework created by TUDEF,
       *   used by TUASSERT macros in this function.
       * @param[in] nmm The data map to check. */
   template <class NavClass>
   void verifyDataType(gnsstk::TestUtil& testFramework,
                       gnsstk::NavMessageMap& nmm);
};


unsigned RinexNavDataFactory_T ::
constructorTest()
{
   TUDEF("RinexNavDataFactory", "RinexNavDataFactory");
   gnsstk::RinexNavDataFactory fact;
      // check for expected signal support
   gnsstk::NavSignalID nsid1(gnsstk::SatelliteSystem::GPS,
                             gnsstk::CarrierBand::L1,
                             gnsstk::TrackingCode::CA,
                             gnsstk::NavType::GPSLNAV);
   gnsstk::NavSignalID nsid2(gnsstk::SatelliteSystem::Galileo,
                             gnsstk::CarrierBand::L1,
                             gnsstk::TrackingCode::E1B,
                             gnsstk::NavType::GalINAV);
   gnsstk::NavSignalID nsid3(gnsstk::SatelliteSystem::Galileo,
                             gnsstk::CarrierBand::E5b,
                             gnsstk::TrackingCode::E5bI,
                             gnsstk::NavType::GalINAV);
   gnsstk::NavSignalID nsid4(gnsstk::SatelliteSystem::Galileo,
                             gnsstk::CarrierBand::L5,
                             gnsstk::TrackingCode::E5aI,
                             gnsstk::NavType::GalFNAV);
   gnsstk::NavSignalID nsid5(gnsstk::SatelliteSystem::QZSS,
                             gnsstk::CarrierBand::L1,
                             gnsstk::TrackingCode::CA,
                             gnsstk::NavType::GPSLNAV);
   gnsstk::NavSignalID nsid6(gnsstk::SatelliteSystem::BeiDou,
                             gnsstk::CarrierBand::B1,
                             gnsstk::TrackingCode::B1I,
                             gnsstk::NavType::BeiDou_D1);
   gnsstk::NavSignalID nsid7(gnsstk::SatelliteSystem::BeiDou,
                             gnsstk::CarrierBand::B1,
                             gnsstk::TrackingCode::B1I,
                             gnsstk::NavType::BeiDou_D2);
   TUASSERT(fact.supportedSignals.count(nsid1));
   TUASSERT(fact.supportedSignals.count(nsid2));
   TUASSERT(fact.supportedSignals.count(nsid3));
   TUASSERT(fact.supportedSignals.count(nsid4));
   TUASSERT(fact.supportedSignals.count(nsid5));
   TUASSERT(fact.supportedSignals.count(nsid6));
   TUASSERT(fact.supportedSignals.count(nsid7));
   TURETURN();
}


unsigned RinexNavDataFactory_T ::
loadIntoMapTest()
{
   TUDEF("RinexNavDataFactory", "loadIntoMap");

      // test loading RINEX 2 nav
   gnsstk::RinexNavDataFactory f2;
   std::string f2name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "arlm2000.15n";
      // this should implicitly load into the data map
   TUASSERT(f2.addDataSource(f2name));
   TUASSERTE(size_t, 507, f2.size());
   gnsstk::CommonTime expti2 = gnsstk::CivilTime(2015,7,19,0,0,0,
                                                 gnsstk::TimeSystem::GPS);
   gnsstk::CommonTime exptf2 = gnsstk::CivilTime(2015,7,20,2,0,0,
                                                 gnsstk::TimeSystem::GPS);
   TUASSERTE(gnsstk::CommonTime, expti2, f2.getInitialTime());
   TUASSERTE(gnsstk::CommonTime, exptf2, f2.getFinalTime());

   TUCSM("convertToOffset");
   gnsstk::NavDataPtr navPtr;
   double offsVal = 666;
   gnsstk::CommonTime refTime = gnsstk::GPSWeekSecond(1854,233472);
   TUASSERTE(bool, true, f2.getOffset(gnsstk::TimeSystem::GPS,
                                      gnsstk::TimeSystem::UTC, refTime, navPtr));
   gnsstk::RinexTimeOffset *rto = dynamic_cast<gnsstk::RinexTimeOffset*>(
      navPtr.get());
   TUASSERTE(bool, true, rto->getOffset(gnsstk::TimeSystem::GPS,
                                        gnsstk::TimeSystem::UTC, refTime,
                                        offsVal));
      //         123456789012345678
      //                  931322575 (test 1 and test 2)
      //                       3553 (test 2 only)
   TUASSERTFE(17.000000000931322575, offsVal);
      // add one second to ref time to force the A1 term to come into play
   refTime += 1.0;
   TUASSERTE(bool, true, rto->getOffset(gnsstk::TimeSystem::GPS,
                                        gnsstk::TimeSystem::UTC, refTime,
                                        offsVal));
   TUASSERTFE(17.000000000931326128, offsVal);
   TUCSM("loadIntoMap");

   gnsstk::RinexNavDataFactory f3;
   std::string f3name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_rinex3_76193040.14n";
      // this should implicitly load into the data map
   TUASSERT(f3.addDataSource(f3name));
   TUASSERTE(size_t, 44, f3.size());

   TestClass f4;
   std::string f4name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_rinex3_76193040.14n";
   TUCATCH(f4.setTypeFilter({gnsstk::NavMessageType::Ephemeris}));
      // this should implicitly load into the data map
   TUASSERT(f4.addDataSource(f4name));
   TUASSERTE(size_t, 13, f4.size());
   gnsstk::NavMessageMap &nmm4(f4.getData());
      // only one message type
   TUASSERTE(size_t, 1, nmm4.size());
      // and it's ephemeris.
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Ephemeris,
             nmm4.begin()->first);
   TUCSM("convertToOrbit/fillNavData");
   verifyDataType<gnsstk::GPSLNavEph>(testFramework, nmm4);
   TUCSM("loadIntoMap");

   TestClass f5;
   std::string f5name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_rinex3_76193040.14n";
   TUCATCH(f5.setTypeFilter({gnsstk::NavMessageType::Health}));
      // this should implicitly load into the data map
   TUASSERT(f5.addDataSource(f5name));
   TUASSERTE(size_t, 13, f5.size());
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
      "test_input_rinex3_76193040.14n";
   TUCATCH(f6.setTypeFilter({gnsstk::NavMessageType::Almanac}));
      // this should implicitly load into the data map
   TUASSERT(f6.addDataSource(f6name));
      // except there isn't any data, because RINEX NAV doesn't contain almanacs
   TUASSERTE(size_t, 0, f6.size());

      // test loading something that isn't rinex
   TestClass f7;
   std::string f7name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_SP3a.sp3";
   TUASSERT(!f7.addDataSource(f7name));
   TUASSERTE(size_t, 0, f7.size());

      // test RINEX 3 Galileo
   TestClass f8;
   std::string f8name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_rinex3_nav_gal.20n";
      // this should implicitly load into the data map
   DEBUGTRACE_ENABLE();
   TUASSERT(f8.addDataSource(f8name));
   DEBUGTRACE_DISABLE();
      // x 4 time offset
      // x 1 GPS ephemeris
      // x 1 GPS health
      // x 1 GPS ISC
      // x 39 Galileo ephemerides
      // x 39 Galileo ISC
      // x 90 Galileo health
      // x 1 Galileo kludge health for iono corrections
      // x 1 Galileo iono correction
   TUASSERTE(size_t, 177, f8.size());
      // count INAV, FNAV, and LNAV data
   unsigned ephICount = 0, ephFCount = 0, ephLCount = 0, heaICount = 0,
      heaLCount = 0, ionoCount = 0, iscLCount = 0, iscICount = 0,
      otherCount = 0, heaFCount = 0, iscFCount = 0, timeRCount = 0,
      heaGalFCount = 0;
   gnsstk::RinexTimeOffset *timeR;
   for (auto& nmti : f8.getData())
   {
      for (auto& sati : nmti.second)
      {
         for (auto& ti : sati.second)
         {
               // Count each data type and spot check the first one of each.
            gnsstk::GalINavEph *ephI;
            gnsstk::GalFNavEph *ephF;
            gnsstk::GalFNavHealth *heaGalF;
            gnsstk::GalINavHealth *heaI;
            gnsstk::GalINavIono *iono;
            gnsstk::GalINavISC *iscI;
            gnsstk::GPSLNavHealth *heaL;
            gnsstk::GPSLNavEph *ephL;
            gnsstk::GPSLNavISC *iscL;
            if ((ephI = dynamic_cast<gnsstk::GalINavEph*>(ti.second.get()))
                != nullptr)
            {
               if (ephICount == 0)
               {
                  static const gnsstk::CommonTime expToc =
                     gnsstk::CivilTime(2020,5,29,0,10,0,gnsstk::TimeSystem::GAL);
                     // yes, these are supposed to be GPS, see
                     // RinexNavDataFactory::fixTimeGalileo.
                  static const gnsstk::CommonTime expToe =
                     gnsstk::GPSWeekSecond(2107,432600);
                  static const gnsstk::CommonTime expXmit1 =
                     gnsstk::GPSWeekSecond(2107,433714);
                  static const gnsstk::CommonTime expXmit2 = expXmit1+2.0;
                  static const gnsstk::CommonTime expXmit3 = expXmit2+2.0;
                  static const gnsstk::CommonTime expXmit4 = expXmit3+2.0;
                  static const gnsstk::CommonTime expXmit5 = expXmit4+2.0;
                  static const gnsstk::CommonTime expEnd = expToe + (3600.0*4.0);
                  static const gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(1, 1,
                                            gnsstk::SatelliteSystem::Galileo,
                                            gnsstk::CarrierBand::E5b,
                                            gnsstk::TrackingCode::E5bI,
                                            gnsstk::NavType::GalINAV),
                     gnsstk::NavMessageType::Ephemeris);
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expXmit1, ephI->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, ephI->signal);
                     // OrbitData doesn't actually have data.
                     // OrbitDataKepler
                  TUASSERTE(gnsstk::CommonTime, expXmit1, ephI->xmitTime);
                  TUASSERTE(gnsstk::CommonTime, expToe, ephI->Toe);
                  TUASSERTE(gnsstk::CommonTime, expToc, ephI->Toc);
                  TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy,
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
                  TUASSERTE(gnsstk::CommonTime, expXmit1, ephI->beginFit);
                  TUASSERTE(gnsstk::CommonTime, expEnd, ephI->endFit);
                     // GalINavEph
                  TUASSERTFE(-1.862645149231e-09, ephI->bgdE5aE1);
                  TUASSERTFE(-2.095475792885e-09, ephI->bgdE5bE1);
                  TUASSERTE(unsigned, 107, ephI->sisaIndex);
                  TUASSERTE(unsigned, 1, ephI->svid);
                  TUASSERTE(gnsstk::CommonTime, expXmit2, ephI->xmit2);
                  TUASSERTE(gnsstk::CommonTime, expXmit3, ephI->xmit3);
                  TUASSERTE(gnsstk::CommonTime, expXmit4, ephI->xmit4);
                  TUASSERTE(gnsstk::CommonTime, expXmit5, ephI->xmit5);
                  TUASSERTE(uint16_t, 81, ephI->iodnav1);
                  TUASSERTE(uint16_t, 81, ephI->iodnav2);
                  TUASSERTE(uint16_t, 81, ephI->iodnav3);
                  TUASSERTE(uint16_t, 81, ephI->iodnav4);
                  TUASSERTE(gnsstk::GalHealthStatus,
                            gnsstk::GalHealthStatus::OK, ephI->hsE5b);
                  TUASSERTE(gnsstk::GalHealthStatus,
                            gnsstk::GalHealthStatus::OK, ephI->hsE1B);
                  TUASSERTE(gnsstk::GalDataValid, gnsstk::GalDataValid::Valid,
                            ephI->dvsE5b);
                  TUASSERTE(gnsstk::GalDataValid, gnsstk::GalDataValid::Valid,
                            ephI->dvsE1B);
               }
               ephICount++;
            }
            else if ((ephF = dynamic_cast<gnsstk::GalFNavEph*>(ti.second.get()))
                     != nullptr)
            {
               if (ephFCount == 0)
               {
                  static const gnsstk::CommonTime expToc =
                     gnsstk::CivilTime(2020,5,29,0,10,0,gnsstk::TimeSystem::GAL);
                     // yes, these are supposed to be GPS, see
                     // RinexNavDataFactory::fixTimeGalileo.
                  static const gnsstk::CommonTime expToe =
                     gnsstk::GPSWeekSecond(2107,432600);
                  static const gnsstk::CommonTime expXmit1 =
                     gnsstk::GPSWeekSecond(2107,433264);
                  static const gnsstk::CommonTime expXmit2 = expXmit1+10.0;
                  static const gnsstk::CommonTime expXmit3 = expXmit2+10.0;
                  static const gnsstk::CommonTime expXmit4 = expXmit3+10.0;
                  static const gnsstk::CommonTime expEnd = expToe + (3600.0*4.0);
                  static const gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(10, 10,
                                            gnsstk::SatelliteSystem::Galileo,
                                            gnsstk::CarrierBand::L5,
                                            gnsstk::TrackingCode::E5aI,
                                            gnsstk::NavType::GalFNAV),
                     gnsstk::NavMessageType::Ephemeris);
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expXmit1, ephF->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, ephF->signal);
                     // OrbitData doesn't actually have data.
                     // OrbitDataKepler
                  TUASSERTE(gnsstk::CommonTime, expXmit1, ephF->xmitTime);
                  TUASSERTE(gnsstk::CommonTime, expToe, ephF->Toe);
                  TUASSERTE(gnsstk::CommonTime, expToc, ephF->Toc);
                  TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy,
                            ephF->health);
                  TUASSERTFE(-4.507601261139e-07, ephF->Cuc);
                  TUASSERTFE( 2.101063728333e-06, ephF->Cus);
                  TUASSERTFE( 3.041875000000e+02, ephF->Crc);
                  TUASSERTFE(-5.968750000000e+00, ephF->Crs);
                  TUASSERTFE( 6.519258022308e-08, ephF->Cic);
                  TUASSERTFE( 5.774199962616e-08, ephF->Cis);
                  TUASSERTFE(-2.375150555491e+00, ephF->M0);
                  TUASSERTFE( 2.890120385115e-09, ephF->dn);
                  TUASSERTFE(0, ephF->dndot);
                  TUASSERTFE( 4.586749710143e-04, ephF->ecc);
                  TUASSERTFE( 5.440611391068e+03*5.440611391068e+03, ephF->A);
                  TUASSERTFE( 5.440611391068e+03, ephF->Ahalf);
                  TUASSERTFE(0, ephF->Adot);
                  TUASSERTFE( 2.789971923884e+00, ephF->OMEGA0);
                  TUASSERTFE( 9.883643686117e-01, ephF->i0);
                  TUASSERTFE( 5.035283685377e-01, ephF->w);
                  TUASSERTFE(-5.772026142343e-09, ephF->OMEGAdot);
                  TUASSERTFE( 2.103659054415e-10, ephF->idot);
                  TUASSERTFE( 3.116308012977e-03, ephF->af0);
                  TUASSERTFE( 2.346354222027e-10, ephF->af1);
                  TUASSERTFE( 3.469446951954e-18, ephF->af2);
                  TUASSERTE(gnsstk::CommonTime, expXmit1, ephF->beginFit);
                  TUASSERTE(gnsstk::CommonTime, expEnd, ephF->endFit);
                     // GalFNavEph
                  TUASSERTFE(-1.559965312481e-08, ephF->bgdE5aE1);
                  TUASSERTE(unsigned, 107, ephF->sisaIndex);
                  TUASSERTE(unsigned, 10, ephF->svid);
                  TUASSERTE(gnsstk::CommonTime, expXmit2, ephF->xmit2);
                  TUASSERTE(gnsstk::CommonTime, expXmit3, ephF->xmit3);
                  TUASSERTE(gnsstk::CommonTime, expXmit4, ephF->xmit4);
                  TUASSERTE(uint16_t, 81, ephF->iodnav1);
                  TUASSERTE(uint16_t, 81, ephF->iodnav2);
                  TUASSERTE(uint16_t, 81, ephF->iodnav3);
                  TUASSERTE(uint16_t, 81, ephF->iodnav4);
                  TUASSERTE(gnsstk::GalHealthStatus,
                            gnsstk::GalHealthStatus::OK, ephF->hsE5a);
                  TUASSERTE(gnsstk::GalDataValid, gnsstk::GalDataValid::Valid,
                            ephF->dvsE5a);
               }
               ephFCount++;
            }
            else if ((ephL = dynamic_cast<gnsstk::GPSLNavEph*>(ti.second.get()))
                     != nullptr)
            {
               if (ephLCount == 0)
               {
                  static const gnsstk::CommonTime expToc =
                     gnsstk::CivilTime(2020,5,29,2,0,0,gnsstk::TimeSystem::GPS);
                  static const gnsstk::CommonTime expToe =
                     gnsstk::GPSWeekSecond(2107,4.392000000000e+05);
                  static const gnsstk::CommonTime expXmit1 =
                     gnsstk::GPSWeekSecond(2107,4.320000000000e+05);
                  static const gnsstk::CommonTime expTS =
                     gnsstk::GPSWeekSecond(2107,4.320180000000e+05);
                  static const gnsstk::CommonTime expEnd = expToe + (3600.0*2.0);
                  static const gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(1, 1,
                                            gnsstk::SatelliteSystem::GPS,
                                            gnsstk::CarrierBand::L1,
                                            gnsstk::TrackingCode::CA,
                                            gnsstk::NavType::GPSLNAV),
                     gnsstk::NavMessageType::Ephemeris);
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expTS, ephL->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, ephL->signal);
                     // OrbitData doesn't actually have data.
                     // OrbitDataKepler
                  TUASSERTE(gnsstk::CommonTime, expXmit1, ephL->xmitTime);
                  TUASSERTE(gnsstk::CommonTime, expToe, ephL->Toe);
                  TUASSERTE(gnsstk::CommonTime, expToc, ephL->Toc);
                  TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy,
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
                  TUASSERTE(gnsstk::CommonTime, expXmit1, ephL->beginFit);
                  TUASSERTE(gnsstk::CommonTime, expEnd, ephL->endFit);
                     // GPSLNavData
                  TUASSERTE(uint32_t, 0, ephL->pre);
                  TUASSERTE(uint32_t, 0, ephL->tlm);
                  TUASSERTE(bool, false, ephL->isf);
                  TUASSERTE(bool, false, ephL->alert);
                  TUASSERTE(bool, true, ephL->asFlag);
                     // GPSLNavEph
                  TUASSERTE(uint32_t, 0, ephL->pre2);
                  TUASSERTE(uint32_t, 0, ephL->tlm2);
                  TUASSERTE(bool, false, ephL->isf2);
                  TUASSERTE(bool, false, ephL->alert2);
                  TUASSERTE(bool, true, ephL->asFlag2);
                  TUASSERTE(uint32_t, 0, ephL->pre3);
                  TUASSERTE(uint32_t, 0, ephL->tlm3);
                  TUASSERTE(bool, false, ephL->isf3);
                  TUASSERTE(bool, false, ephL->alert3);
                  TUASSERTE(bool, true, ephL->asFlag3);
                  TUASSERTE(uint16_t, 48, ephL->iodc);
                  TUASSERTE(uint16_t, 48, ephL->iode);
                  TUASSERTE(unsigned, 0, ephL->fitIntFlag);
                  TUASSERTE(unsigned, 0, ephL->healthBits);
                  TUASSERTE(unsigned, 0, ephL->uraIndex); // 2m
                  TUASSERTFE( 5.122274160385e-09, ephL->tgd);
                  TUASSERTE(gnsstk::GPSLNavL2Codes,
                            gnsstk::GPSLNavL2Codes::Pcode, ephL->codesL2);
                  TUASSERTE(bool, false, ephL->L2Pdata);
               }
               ephLCount++;
            }
            else if ((heaI=dynamic_cast<gnsstk::GalINavHealth*>(ti.second.get()))
                     != nullptr)
            {
               if (heaICount == 1)
               {
                  static const gnsstk::CommonTime expXmit1 =
                     gnsstk::GPSWeekSecond(2107,433714);
                  static const gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(1, 1,
                                            gnsstk::SatelliteSystem::Galileo,
                                            gnsstk::CarrierBand::L5,
                                            gnsstk::TrackingCode::E5bI,
                                            gnsstk::NavType::GalINAV),
                     gnsstk::NavMessageType::Health);
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expXmit1, heaI->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, heaI->signal);
                     // NavHealthData has nothing.
                     // GalINavHealth
                  TUASSERTE(gnsstk::GalHealthStatus,
                            gnsstk::GalHealthStatus::OK, heaI->sigHealthStatus);
                  TUASSERTE(gnsstk::GalDataValid, gnsstk::GalDataValid::Valid,
                            heaI->dataValidityStatus);
                  TUASSERTE(unsigned, 107, heaI->sisaIndex);
               }
               heaICount++;
            }
            else if ((heaGalF = dynamic_cast<gnsstk::GalFNavHealth*>(
                         ti.second.get())) != nullptr)
            {
               if (heaGalFCount == 1)
               {
                  static const gnsstk::CommonTime expXmit1 =
                     gnsstk::GPSWeekSecond(2107,433864);
                  static const gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(1, 1,
                                            gnsstk::SatelliteSystem::Galileo,
                                            gnsstk::CarrierBand::L5,
                                            gnsstk::TrackingCode::E5aI,
                                            gnsstk::NavType::GalFNAV),
                     gnsstk::NavMessageType::Health);
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expXmit1, heaGalF->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, heaGalF->signal);
                     // NavHealthData has nothing.
                     // GalFNavHealth
                  TUASSERTE(gnsstk::GalHealthStatus,
                            gnsstk::GalHealthStatus::OK,
                            heaGalF->sigHealthStatus);
                  TUASSERTE(gnsstk::GalDataValid, gnsstk::GalDataValid::Valid,
                            heaGalF->dataValidityStatus);
                  TUASSERTE(unsigned, 255, heaGalF->sisaIndex);
               }
               heaGalFCount++;
            }
            else if ((heaL=dynamic_cast<gnsstk::GPSLNavHealth*>(ti.second.get()))
                     != nullptr)
            {
               if (heaLCount == 0)
               {
                  static const gnsstk::CommonTime expTS =
                     gnsstk::GPSWeekSecond(2107,4.320180000000e+05);
                  static const gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(1, 1,
                                            gnsstk::SatelliteSystem::GPS,
                                            gnsstk::CarrierBand::L1,
                                            gnsstk::TrackingCode::CA,
                                            gnsstk::NavType::GPSLNAV),
                     gnsstk::NavMessageType::Health);
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expTS, heaL->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, heaL->signal);
                     // NavHealthData has nothing.
                     // GPSLNavHealth
                  TUASSERTE(unsigned, 0, heaL->svHealth);
               }
               heaLCount++;
            }
            else if ((iono=dynamic_cast<gnsstk::GalINavIono*>(ti.second.get()))
                     != nullptr)
            {
               if (ionoCount == 0)
               {
                  static const gnsstk::CommonTime expTS =
                     gnsstk::GPSWeekSecond(2107,439200);
                  static const gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(0, 0,
                                            gnsstk::SatelliteSystem::Galileo,
                                            gnsstk::CarrierBand::L1,
                                            gnsstk::TrackingCode::E1B,
                                            gnsstk::NavType::GalINAV),
                     gnsstk::NavMessageType::Iono);
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expTS, iono->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, iono->signal);
                     // GalINavIono
                  TUASSERTFE(0.4575e+02, iono->ai[0]);
                  TUASSERTFE(0.1641e+00, iono->ai[1]);
                  TUASSERTFE(0.6714e-03, iono->ai[2]);
                  TUASSERTE(bool, false, iono->idf[0]);
                  TUASSERTE(bool, false, iono->idf[1]);
                  TUASSERTE(bool, false, iono->idf[2]);
                  TUASSERTE(bool, false, iono->idf[3]);
                  TUASSERTE(bool, false, iono->idf[4]);
               }
               ionoCount++;
            }
            else if ((iscL = dynamic_cast<gnsstk::GPSLNavISC*>(ti.second.get()))
                     != nullptr)
            {
               static const gnsstk::CommonTime expTS =
                  gnsstk::GPSWeekSecond(2107,4.320180000000e+05);
               static const gnsstk::NavMessageID expNMID(
                  gnsstk::NavSatelliteID(1, 1,
                                         gnsstk::SatelliteSystem::GPS,
                                         gnsstk::CarrierBand::L1,
                                         gnsstk::TrackingCode::CA,
                                         gnsstk::NavType::GPSLNAV),
                  gnsstk::NavMessageType::ISC);
                  // NavData
               TUASSERTE(gnsstk::CommonTime, expTS, iscL->timeStamp);
               TUASSERTE(gnsstk::NavMessageID, expNMID, iscL->signal);
                  // InterSigCorr fields
               TUASSERTFE(5.122274160385e-09, iscL->isc);
                  // GPSLNavISC fields
               TUASSERTE(uint32_t, 0, iscL->pre);
               TUASSERTE(uint32_t, 0, iscL->tlm);
               TUASSERTE(bool, false, iscL->isf);
               TUASSERTE(bool, false, iscL->alert);
               TUASSERTE(bool, true, iscL->asFlag);
               iscLCount++;
            }
            else if ((iscI = dynamic_cast<gnsstk::GalINavISC*>(ti.second.get()))
                     != nullptr)
            {
               if (iscICount == 0)
               {
                  static const gnsstk::CommonTime expTS =
                     gnsstk::GPSWeekSecond(2107,433714);
                  static const gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(1, 1,
                                            gnsstk::SatelliteSystem::Galileo,
                                            gnsstk::CarrierBand::E5b,
                                            gnsstk::TrackingCode::E5bI,
                                            gnsstk::NavType::GalINAV),
                     gnsstk::NavMessageType::ISC);
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expTS, iscI->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, iscI->signal);
                     // InterSigCorr fields
                  TUASSERTE(bool, true, std::isnan(iscI->isc));
                     // GalINavISC fields
                  TUASSERTFE(-1.862645149231e-09, iscI->bgdE1E5a);
                  TUASSERTFE(-2.095475792885e-09, iscI->bgdE1E5b);
               }
               iscICount++;
            }
            else if ((timeR = dynamic_cast<gnsstk::RinexTimeOffset*>(
                         ti.second.get())) != nullptr)
            {
               if (timeRCount == 0)
               {
                  static const gnsstk::CommonTime expTS =
                     gnsstk::GPSWeekSecond(2108, 61440);
                     // Have to make this non-const to set it to the
                     // right truth value.
                  gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(),
                     gnsstk::NavMessageType::TimeOffset);
                  expNMID.system = gnsstk::SatelliteSystem::GPS;
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expTS, timeR->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, timeR->signal);
                     // TimeOffsetData has no data of its own
                     // TimeSystemCorrection
                     // Note that the TIME SYSTEM CORR data doesn't
                     // come out in the same order it's in the file.
                     // Probably not good for round-trips.
                  TUASSERTE(gnsstk::TimeSystemCorrection::CorrType,
                            gnsstk::TimeSystemCorrection::GPUT,
                            timeR->type);
                  TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GPS,
                            timeR->frTS);
                  TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC,
                            timeR->toTS);
                  TUASSERTFE(-.931322574615e-09, timeR->A0);
                  TUASSERTFE(-.355271367880e-14, timeR->A1);
                  TUASSERTE(gnsstk::CommonTime, expTS, timeR->refTime);
                  TUASSERTE(std::string, "", timeR->geoProvider);
                  TUASSERTE(int, 0, timeR->geoUTCid);
                     // RinexTimeOffset
                  TUASSERTFE(18, timeR->deltatLS);
               }
               timeRCount++;
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
   TUASSERTE(unsigned, 52, heaICount);
   TUASSERTE(unsigned, 39, heaGalFCount);
   TUASSERTE(unsigned, 39, iscICount);
   TUASSERTE(unsigned, 1, heaLCount);
   TUASSERTE(unsigned, 1, ionoCount);
   TUASSERTE(unsigned, 1, iscLCount);
   TUASSERTE(unsigned, 4, timeRCount);
   TUASSERTE(unsigned, 0, otherCount);


      // test RINEX 3 BeiDou
   TestClass f9;
   std::string f9name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "test_input_rinex3_nav_BeiDou.14n";
      // this should implicitly load into the data map
   TUASSERT(f9.addDataSource(f9name));
      // 1 ephemeris
      // 1 health
      // 1 time offset
      // 1 ISC
   TUASSERTE(size_t, 4, f9.size());
   unsigned eph2Count = 0, hea2Count = 0, isc2Count = 0;
   timeRCount = 0;
   otherCount = 0;
   for (auto& nmti : f9.getData())
   {
      for (auto& sati : nmti.second)
      {
         for (auto& ti : sati.second)
         {
               // Count each data type and spot check the first one of each.
            gnsstk::BDSD2NavEph *eph2;
            gnsstk::BDSD2NavHealth *hea2;
            gnsstk::BDSD2NavISC *isc2;
            if ((eph2 = dynamic_cast<gnsstk::BDSD2NavEph*>(ti.second.get()))
                != nullptr)
            {
               if (eph2Count == 0)
               {
                  static const gnsstk::CommonTime expToc =
                     gnsstk::CivilTime(2014,5,10,0,0,0,gnsstk::TimeSystem::BDT);
                  static const gnsstk::CommonTime expToe =
                     gnsstk::BDSWeekSecond(435,5.184000000000e+05);
                  static const gnsstk::CommonTime expXmit1 =
                     gnsstk::BDSWeekSecond(435,5.184000000000e+05);
                  static const gnsstk::CommonTime expTS =
                     gnsstk::BDSWeekSecond(435,5.184000000000e+05);
                  static const gnsstk::CommonTime expBeg = expToe-(3600.0*2.0);
                  static const gnsstk::CommonTime expEnd = expXmit1+(3600.0*24.0+30.0);
                  static const gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(1, 1,
                                            gnsstk::SatelliteSystem::BeiDou,
                                            gnsstk::CarrierBand::B1,
                                            gnsstk::TrackingCode::B1I,
                                            gnsstk::NavType::BeiDou_D2),
                     gnsstk::NavMessageType::Ephemeris);
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expTS, eph2->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, eph2->signal);
                     // OrbitData doesn't actually have data.
                     // OrbitDataKepler
                  TUASSERTE(gnsstk::CommonTime, expXmit1, eph2->xmitTime);
                  TUASSERTE(gnsstk::CommonTime, expToe, eph2->Toe);
                  TUASSERTE(gnsstk::CommonTime, expToc, eph2->Toc);
                  TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy,
                            eph2->health);
                  TUASSERTFE( 1.447647809982e-05, eph2->Cuc);
                  TUASSERTFE( 8.092261850834e-06, eph2->Cus);
                  TUASSERTFE(-2.506406250000e+02, eph2->Crc);
                  TUASSERTFE( 4.365468750000e+02, eph2->Crs);
                  TUASSERTFE(-2.654269337654e-08, eph2->Cic);
                  TUASSERTFE(-3.864988684654e-08, eph2->Cis);
                  TUASSERTFE(-3.118148933476e+00, eph2->M0);
                  TUASSERTFE( 1.318269196918e-09, eph2->dn);
                  TUASSERTFE(0, eph2->dndot);
                  TUASSERTFE( 2.822051756084e-04, eph2->ecc);
                  TUASSERTFE( 6.493480609894e+03*6.493480609894e+03, eph2->A);
                  TUASSERTFE( 6.493480609894e+03, eph2->Ahalf);
                  TUASSERTFE(0, eph2->Adot);
                  TUASSERTFE( 3.076630958509e+00, eph2->OMEGA0);
                  TUASSERTFE( 1.103024081152e-01, eph2->i0);
                  TUASSERTFE( 2.587808789012e+00, eph2->w);
                  TUASSERTFE(-3.039412318009e-10, eph2->OMEGAdot);
                  TUASSERTFE( 2.389385241772e-10, eph2->idot);
                  TUASSERTFE( 2.969256602228e-04, eph2->af0);
                  TUASSERTFE( 2.196998138970e-11, eph2->af1);
                  TUASSERTFE( 0.000000000000e+00, eph2->af2);
                  TUASSERTE(gnsstk::CommonTime, expBeg, eph2->beginFit);
                  TUASSERTE(gnsstk::CommonTime, expEnd, eph2->endFit);
               }
               eph2Count++;
            }
            else if ((hea2 = dynamic_cast<gnsstk::BDSD2NavHealth*>(
                         ti.second.get())) != nullptr)
            {
               if (hea2Count == 0)
               {
                  static const gnsstk::CommonTime expTS =
                     gnsstk::BDSWeekSecond(435,5.184000000000e+05);
                  static const gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(1, 1,
                                            gnsstk::SatelliteSystem::BeiDou,
                                            gnsstk::CarrierBand::B1,
                                            gnsstk::TrackingCode::B1I,
                                            gnsstk::NavType::BeiDou_D2),
                     gnsstk::NavMessageType::Health);
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expTS, hea2->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, hea2->signal);
                     // NavHealthData has nothing.
                     // BDSD2NavHealth
                  TUASSERTE(bool, false, hea2->isAlmHealth);
                  TUASSERTE(bool, false, hea2->satH1);
                  TUASSERTE(unsigned, 255, hea2->svHealth);
               }
               hea2Count++;
            }
            else if ((isc2 = dynamic_cast<gnsstk::BDSD2NavISC*>(
                         ti.second.get())) != nullptr)
            {
               if (isc2Count == 0)
               {
                  static const gnsstk::CommonTime expTS =
                     gnsstk::BDSWeekSecond(435,5.184000000000e+05);
                  static const gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(1, 1,
                                            gnsstk::SatelliteSystem::BeiDou,
                                            gnsstk::CarrierBand::B1,
                                            gnsstk::TrackingCode::B1I,
                                            gnsstk::NavType::BeiDou_D2),
                     gnsstk::NavMessageType::ISC);
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expTS, isc2->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, isc2->signal);
                     // InterSigCorr fields
                  TUASSERTE(bool, true, std::isnan(isc2->isc));
                     // GalINavISC fields
                  TUASSERTFE( 1.420000000000e-08, isc2->tgd1);
                  TUASSERTFE(-1.040000000000e-08, isc2->tgd2);
               }
               isc2Count++;
            }
            else if ((timeR = dynamic_cast<gnsstk::RinexTimeOffset*>(
                         ti.second.get())) != nullptr)
            {
               if (timeRCount == 0)
               {
                  static const gnsstk::CommonTime expTS =
                     gnsstk::BDSWeekSecond(435, 14);
                     // Have to make this non-const to set it to the
                     // right truth value.
                  static gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(),
                     gnsstk::NavMessageType::TimeOffset);
                  expNMID.system = gnsstk::SatelliteSystem::BeiDou;
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expTS, timeR->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, timeR->signal);
                     // TimeOffsetData has no data of its own
                     // TimeSystemCorrection
                     // Note that the TIME SYSTEM CORR data doesn't
                     // come out in the same order it's in the file.
                     // Probably not good for round-trips.
                  TUASSERTE(gnsstk::TimeSystemCorrection::CorrType,
                            gnsstk::TimeSystemCorrection::BDUT,
                            timeR->type);
                  TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::BDT,
                            timeR->frTS);
                  TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC,
                            timeR->toTS);
                  TUASSERTFE(-9.3132257462e-10, timeR->A0);
                  TUASSERTFE(9.769962617e-15, timeR->A1);
                  TUASSERTE(gnsstk::CommonTime, expTS, timeR->refTime);
                  TUASSERTE(std::string, "", timeR->geoProvider);
                  TUASSERTE(int, 0, timeR->geoUTCid);
                     // RinexTimeOffset
                  TUASSERTFE(0, timeR->deltatLS);
               }
               timeRCount++;
            }
            else
            {
               otherCount++;
            }
         }
      }
   }
   TUASSERTE(unsigned, 1, eph2Count);
   TUASSERTE(unsigned, 1, hea2Count);
   TUASSERTE(unsigned, 1, isc2Count);
   TUASSERTE(unsigned, 1, timeRCount);
   TUASSERTE(unsigned, 0, otherCount);

      // Test RINEX 2 mixed (GLONASS in particular)
   TestClass f10;
   std::string f10name = gnsstk::getPathData() + gnsstk::getFileSep() +
      "mixed.06n";
      // this should implicitly load into the data map
   TUASSERT(f10.addDataSource(f10name));
      // 4 ephemerides
      // 5 health
      // 1 iono
      // 2 ISC
      // 2 time offset (2 duplicates)
   ephFCount = ephLCount = heaFCount = heaLCount = iscFCount = iscLCount = 0;
   unsigned ionoLCount = 0;
   timeRCount = 0;
   otherCount = 0;
   for (auto& nmti : f10.getData())
   {
      for (auto& sati : nmti.second)
      {
         for (auto& ti : sati.second)
         {
            gnsstk::GPSLNavEph *ephL;
            gnsstk::GPSLNavHealth *heaL;
            gnsstk::GPSLNavISC *iscL;
            gnsstk::GPSLNavIono *ionoL;
            gnsstk::GLOFNavEph *ephF;
            gnsstk::GLOFNavHealth *heaF;
            gnsstk::GLOFNavISC *iscF;
            if ((ephF = dynamic_cast<gnsstk::GLOFNavEph*>(ti.second.get()))
                != nullptr)
            {
               if (ephFCount == 0)
               {
                  static const gnsstk::CommonTime expToe =
                     gnsstk::CivilTime(2006, 10, 1, 0, 15, 0,
                                       gnsstk::TimeSystem::UTC);
                  static const gnsstk::CommonTime expTS =
                     gnsstk::CivilTime(2006, 10, 1, 0, 1, 30,
                                       gnsstk::TimeSystem::UTC);
                  static const gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(1, gnsstk::SatelliteSystem::Glonass,
                                            gnsstk::CarrierBand::G1,
                                            gnsstk::TrackingCode::Standard,
                                            gnsstk::XmitAnt::Standard, 7, false,
                                            gnsstk::NavType::GloCivilF),
                     gnsstk::NavMessageType::Ephemeris);
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expTS, ephF->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, ephF->signal);
                     // GLOFNavData
                  TUASSERTE(gnsstk::CommonTime, expTS+2.0, ephF->xmit2);
                  TUASSERTE(gnsstk::GLOFNavSatType,
                            gnsstk::GLOFNavSatType::Unknown,
                            ephF->satType);
                  TUASSERTE(unsigned, 1, ephF->slot);
                  TUASSERTE(bool, false, ephF->lhealth);
                  TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Healthy,
                            ephF->health);
                  TUASSERTE(gnsstk::CommonTime, expTS, ephF->beginFit);
                  TUASSERTE(gnsstk::CommonTime, expToe+930.0, ephF->endFit);
                     // GLOFNavEph
                  TUASSERTE(gnsstk::CommonTime, expTS, ephF->ref);
                  TUASSERTE(gnsstk::CommonTime, expTS+4.0, ephF->xmit3);
                  TUASSERTE(gnsstk::CommonTime, expTS+6.0, ephF->xmit4);
                     // 1 um tolerance.
                  TUASSERTFEPS( 0.157594921875E+05, ephF->pos[0], 1e-9);
                  TUASSERTFEPS(-0.813711474609E+04, ephF->pos[1], 1e-9);
                  TUASSERTFEPS( 0.183413398438E+05, ephF->pos[2], 1e-9);
                  TUASSERTFEPS(-0.145566368103E+01, ephF->vel[0], 1e-9);
                  TUASSERTFEPS( 0.205006790161E+01, ephF->vel[1], 1e-9);
                  TUASSERTFEPS( 0.215388488770E+01, ephF->vel[2], 1e-9);
                  TUASSERTFEPS( 0.000000000000E+00, ephF->acc[0], 1e-9);
                  TUASSERTFEPS( 0.931322574615E-09, ephF->acc[1], 1e-9);
                  TUASSERTFEPS(-0.186264514923E-08, ephF->acc[2], 1e-9);
                  TUASSERTFE(0.137668102980E-04, ephF->clkBias);
                  TUASSERTFE(-0.454747350886E-11, ephF->freqBias);
                  TUASSERTE(unsigned, 0, ephF->healthBits);
                  TUASSERTE(unsigned, -1, ephF->tb);
                  TUASSERTE(unsigned, -1, ephF->P1);
                  TUASSERTE(unsigned, -1, ephF->P2);
                  TUASSERTE(unsigned, -1, ephF->P3);
                  TUASSERTE(unsigned, -1, ephF->P4);
                  TUASSERTE(unsigned, 0, ephF->interval);
                  TUASSERTE(gnsstk::GLOFNavPCode,
                            gnsstk::GLOFNavPCode::Unknown,
                            ephF->opStatus);
                  TUASSERTE(int, 1, isnan(ephF->tauDelta));
                  TUASSERTE(unsigned, 1, ephF->aod);
                  TUASSERTE(unsigned, -1, ephF->accIndex);
                  TUASSERTE(unsigned, -1, ephF->dayCount);
                  TUASSERTE(gnsstk::CommonTime, expToe, ephF->Toe);
                  TUASSERTFE(60, ephF->step);
               }
               ephFCount++;
            }
            else if ((ephL = dynamic_cast<gnsstk::GPSLNavEph*>(ti.second.get()))
                     != nullptr)
            {
               ephLCount++;
            }
            else if ((heaF=dynamic_cast<gnsstk::GLOFNavHealth*>(ti.second.get()))
                     != nullptr)
            {
               if (heaFCount == 0)
               {
                  static const gnsstk::CommonTime expTS =
                     gnsstk::CivilTime(2006, 10, 1, 0, 1, 30,
                                       gnsstk::TimeSystem::UTC);
                  static const gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(1, gnsstk::SatelliteSystem::Glonass,
                                            gnsstk::CarrierBand::G1,
                                            gnsstk::TrackingCode::Standard,
                                            gnsstk::XmitAnt::Standard, 7, false,
                                            gnsstk::NavType::GloCivilF),
                     gnsstk::NavMessageType::Health);
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expTS, heaF->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, heaF->signal);
                     // GLOFNavHealth
                  TUASSERTE(bool, true, heaF->healthBits.is_valid());
                  TUASSERTE(bool, false, heaF->ln.is_valid());
                  TUASSERTE(bool, false, heaF->Cn.is_valid());
                  TUASSERTE(unsigned,0,static_cast<unsigned>(heaF->healthBits));
               }
               heaFCount++;
            }
            else if ((heaL=dynamic_cast<gnsstk::GPSLNavHealth*>(ti.second.get()))
                     != nullptr)
            {
               heaLCount++;
            }
            else if ((iscF = dynamic_cast<gnsstk::GLOFNavISC*>(ti.second.get()))
                     != nullptr)
            {
               iscFCount++;
            }
            else if ((iscL = dynamic_cast<gnsstk::GPSLNavISC*>(ti.second.get()))
                     != nullptr)
            {
               iscLCount++;
            }
            else if ((ionoL = dynamic_cast<gnsstk::GPSLNavIono*>(
                         ti.second.get())) != nullptr)
            {
               ionoLCount++;
            }
            else if ((timeR = dynamic_cast<gnsstk::RinexTimeOffset*>(
                         ti.second.get())) != nullptr)
            {
               if (timeRCount == 0)
               {
                  static const gnsstk::CommonTime expTS =
                     gnsstk::GPSWeekSecond(1395, 147456);
                     // Have to make this non-const to set it to the
                     // right truth value.
                  static gnsstk::NavMessageID expNMID(
                     gnsstk::NavSatelliteID(),
                     gnsstk::NavMessageType::TimeOffset);
                  expNMID.system = gnsstk::SatelliteSystem::GPS;
                     // NavData
                  TUASSERTE(gnsstk::CommonTime, expTS, timeR->timeStamp);
                  TUASSERTE(gnsstk::NavMessageID, expNMID, timeR->signal);
                     // TimeOffsetData has no data of its own
                     // TimeSystemCorrection
                     // Note that the TIME SYSTEM CORR data doesn't
                     // come out in the same order it's in the file.
                     // Probably not good for round-trips.
                  TUASSERTE(gnsstk::TimeSystemCorrection::CorrType,
                            gnsstk::TimeSystemCorrection::GPUT,
                            timeR->type);
                  TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::GPS,
                            timeR->frTS);
                  TUASSERTE(gnsstk::TimeSystem, gnsstk::TimeSystem::UTC,
                            timeR->toTS);
                  TUASSERTFE(0.2793967723E-08, timeR->A0);
                  TUASSERTFE(0, timeR->A1);
                  TUASSERTE(gnsstk::CommonTime, expTS, timeR->refTime);
                  TUASSERTE(std::string, "", timeR->geoProvider);
                  TUASSERTE(int, 0, timeR->geoUTCid);
                     // RinexTimeOffset
                  TUASSERTFE(14, timeR->deltatLS);
               }
               timeRCount++;
            }
            else
            {
               otherCount++;
               ti.second->dump(std::cerr,gnsstk::DumpDetail::Full);
            }
         }
      }
   }
   TUASSERTE(unsigned, 2, ephLCount);
   TUASSERTE(unsigned, 2, ephFCount);
   TUASSERTE(unsigned, 3, heaLCount);
   TUASSERTE(unsigned, 2, heaFCount);
   TUASSERTE(unsigned, 0, iscFCount);
   TUASSERTE(unsigned, 2, iscLCount);
   TUASSERTE(unsigned, 2, timeRCount);
   TUASSERTE(unsigned, 1, ionoLCount);
   TUASSERTE(unsigned, 0, otherCount);
   TUASSERTE(size_t, 14, f10.size());
   TURETURN();
}


unsigned RinexNavDataFactory_T ::
decodeSISATest()
{
   TUDEF("RinexNavDataFactory", "decodeSISA");
   TUASSERTE(unsigned, 255,
             gnsstk::RinexNavDataFactory::decodeSISA(-1.0));
      // test all SISA indices for which there's a SISA value in meters
   for (unsigned index = 0; index < 50; index++)
   {
      double accuracy = index / 100.0;
      TUASSERTE(unsigned, index,
                gnsstk::RinexNavDataFactory::decodeSISA(accuracy));
   }
   for (unsigned index = 50; index < 75; index++)
   {
      double accuracy = 0.5 + ((index-50) * 0.02);
      TUASSERTE(unsigned, index,
                gnsstk::RinexNavDataFactory::decodeSISA(accuracy));
   }
   for (unsigned index = 75; index < 100; index++)
   {
      double accuracy = 1.0 + ((index-75) * 0.04);
      TUASSERTE(unsigned, index,
                gnsstk::RinexNavDataFactory::decodeSISA(accuracy));
   }
   for (unsigned index = 100; index < 126; index++)
   {
      double accuracy = 2.0 + ((index-100) * 0.16);
         // std::cerr /*<< std::setprecision(20)*/ << "index=" << index << "  accuracy=" << accuracy << std::endl;
      TUASSERTE(unsigned, index,
                gnsstk::RinexNavDataFactory::decodeSISA(accuracy));
   }
   TURETURN();
}


unsigned RinexNavDataFactory_T ::
encodeSISATest()
{
   TUDEF("RinexNavDataFactory", "encodeSISA");
   for (unsigned idx = 0; idx <= 125; idx++)
   {
      double accuracy;
      unsigned sisa;
      TUCATCH(accuracy = gnsstk::RinexNavDataFactory::encodeSISA(idx));
      TUCATCH(sisa = gnsstk::RinexNavDataFactory::decodeSISA(accuracy));
      TUASSERTE(unsigned, idx, sisa);
   }
      // 126-254 are spare
   TUASSERTFE(-1.0, gnsstk::RinexNavDataFactory::encodeSISA(255));
   TURETURN();
}


unsigned RinexNavDataFactory_T ::
loadIntoMapQZSSTest()
{
   TUDEF("RinexNavDataFactory", "loadIntoMap(QZSS)");
   TestClass uut;
   std::string fname = gnsstk::getPathData() + gnsstk::getFileSep() +
      "qzsssampl_U_20141330729_01D_RN.rnx";
   gnsstk::NavMessageID nmidExp(
      gnsstk::NavSatelliteID(193, 193, gnsstk::SatelliteSystem::QZSS,
                             gnsstk::CarrierBand::L1, gnsstk::TrackingCode::CA,
                             gnsstk::NavType::GPSLNAV),
      gnsstk::NavMessageType::Health);
   gnsstk::CommonTime expTS = gnsstk::CivilTime(2014,5,13,7,15,0,
                                                gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime expXT = gnsstk::CivilTime(2014,5,13,7,15,0,
                                                gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime expti2 = gnsstk::CivilTime(2014,5,13,7,15,0,
                                                 gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime exptf2 = gnsstk::CivilTime(2014,5,13,10,30,0,
                                                 gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime toeExp = gnsstk::GPSWeekSecond(1792,202512,
                                                     gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime beginExp = gnsstk::GPSWeekSecond(1792, 198900,
                                                       gnsstk::TimeSystem::QZS);
   gnsstk::CommonTime endExp = gnsstk::GPSWeekSecond(1792, 210600,
                                                     gnsstk::TimeSystem::QZS);
   TUASSERT(uut.addDataSource(fname));
   TUASSERTE(size_t, 3, uut.size());
   gnsstk::NavMessageMap &nmm(uut.getData());
      // uut.dump(std::cerr, gnsstk::DumpDetail::Full);
   TUASSERTE(gnsstk::CommonTime, expti2, uut.getInitialTime());
   TUASSERTE(gnsstk::CommonTime, exptf2, uut.getFinalTime());
   gnsstk::GPSLNavEph *eph;
   gnsstk::GPSLNavHealth *hea;
   gnsstk::GPSLNavISC *isc;
   unsigned heaCount = 0, ephCount = 0, iscCount = 0, otherCount = 0;
   for (const auto& nmti : nmm)
   {
      for (const auto& sati : nmti.second)
      {
         for (const auto& ti : sati.second)
         {
            if ((eph = dynamic_cast<gnsstk::GPSLNavEph*>(ti.second.get()))
                != nullptr)
            {
               ephCount++;
               nmidExp.messageType = gnsstk::NavMessageType::Ephemeris;
                  // NavData fields
               TUASSERTE(gnsstk::CommonTime, expTS, eph->timeStamp);
               TUASSERTE(gnsstk::NavMessageID, nmidExp, eph->signal);
                  // OrbitData has no fields
                  // OrbitDataKepler fields
               TUASSERTE(gnsstk::CommonTime, expXT, eph->xmitTime);
               TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toe);
                  // same value as toe
               TUASSERTE(gnsstk::CommonTime, toeExp, eph->Toc);
               TUASSERTE(gnsstk::SVHealth, gnsstk::SVHealth::Unhealthy,
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
               TUASSERTE(gnsstk::CommonTime, beginExp, eph->beginFit);
               TUASSERTE(gnsstk::CommonTime, endExp, eph->endFit);
                  // GPSLNavData fields
               TUASSERTE(uint32_t, 0, eph->pre);
               TUASSERTE(uint32_t, 0, eph->tlm);
               TUASSERTE(bool, false, eph->isf);
               TUASSERTE(bool, false, eph->alert);
               TUASSERTE(bool, false, eph->asFlag);
                  // GPSLNavEph fields
               TUASSERTE(uint32_t, 0, eph->pre2);
               TUASSERTE(uint32_t, 0, eph->tlm2);
               TUASSERTE(bool, false, eph->isf2);
               TUASSERTE(uint32_t, 0, eph->pre3);
               TUASSERTE(uint32_t, 0, eph->tlm3);
               TUASSERTE(bool, false, eph->isf3);
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
               TUASSERTE(gnsstk::GPSLNavL2Codes,
                         gnsstk::GPSLNavL2Codes::CAcode, eph->codesL2);
               TUASSERTE(bool, true, eph->L2Pdata);
            }
            else if ((hea=dynamic_cast<gnsstk::GPSLNavHealth*>(ti.second.get()))
                     != nullptr)
            {
               heaCount++;
            }
            else if ((isc = dynamic_cast<gnsstk::GPSLNavISC*>(ti.second.get()))
                     != nullptr)
            {
               iscCount++;
               nmidExp.messageType = gnsstk::NavMessageType::ISC;
                  // NavData fields
               TUASSERTE(gnsstk::CommonTime, expTS, isc->timeStamp);
               TUASSERTE(gnsstk::NavMessageID, nmidExp, isc->signal);
                  // InterSigCorr fields
               TUASSERTFE(-4.656612873077e-09, isc->isc);
                  // GPSLNavISC fields
               TUASSERTE(uint32_t, 0, isc->pre);
               TUASSERTE(uint32_t, 0, isc->tlm);
               TUASSERTE(bool, false, isc->isf);
               TUASSERTE(bool, false, isc->alert);
               TUASSERTE(bool, false, isc->asFlag);
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
   TUASSERTE(unsigned, 1, iscCount);
   TUASSERTE(unsigned, 0, otherCount);
   TURETURN();
}


template <class NavClass>
void RinexNavDataFactory_T ::
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
   RinexNavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.loadIntoMapTest();
   errorTotal += testClass.loadIntoMapQZSSTest();
   errorTotal += testClass.decodeSISATest();
   errorTotal += testClass.encodeSISATest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
