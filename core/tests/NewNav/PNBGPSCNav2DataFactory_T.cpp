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
#include "PNBGPSCNav2DataFactory.hpp"
#include "TestUtil.hpp"
#include "GPSCNav2TimeOffset.hpp"
#include "GPSCNav2Health.hpp"
#include "GPSCNav2Eph.hpp"
#include "GPSCNav2Alm.hpp"
//#include "GPSCNav2RedAlm.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
}


class PNBGPSCNav2DataFactory_T
{
public:
   PNBGPSCNav2DataFactory_T();

#include "CNav2TestDataDecl.hpp"

   unsigned addDataAllTest();
      /// Test addData with ephemeris selected only
   unsigned addDataEphemerisTest();
      /// Test addData with almanac selected only
   unsigned addDataAlmanacTest();
      /// Test addData with health data selected only
   unsigned addDataHealthTest();
      /// Test addData with time offset data selected only
   unsigned addDataTimeTest();
      /// One additional combo test.
   unsigned addDataEphHealthTest();
      /// Another combo test that makes sure alm health is processed correctly.
   unsigned addDataAlmHealthTest();
   unsigned processEphTest();
   unsigned processAlmOrbTest();
   unsigned processUTCIonoTest();
   unsigned processGGTOEOPTest();

      /// Count the various types of messages present in navOut.
   void countResults(const gpstk::NavDataPtrList& navOut);

      /// Counts of messages, set by countResults.
   unsigned almCount, ephCount, toCount, heaCount, otherCount;
};


PNBGPSCNav2DataFactory_T ::
PNBGPSCNav2DataFactory_T()
      : almCount(0), ephCount(0), toCount(0), heaCount(0), otherCount(0)
{
#include "CNav2TestDataDef.hpp"
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataAllTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   gpstk::PNBGPSCNav2DataFactory uut;
   gpstk::SatID gloSid(1,gpstk::SatelliteSystem::Glonass);
   gpstk::ObsID gloid(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::G1,
                      gpstk::TrackingCode::Standard);
   gpstk::NavID gloNav(gpstk::NavType::GloCivilF);
   gpstk::PackedNavBitsPtr nonGPS =
      std::make_shared<gpstk::PackedNavBits>(gloSid,gloid,gloNav,"XX",
                                             sf2CNAV2GPSct);
   gpstk::NavDataPtrList navOut;
      // should refuse non-GPS data
   TUASSERTE(bool, false, uut.addData(nonGPS, navOut));
   countResults(navOut);
      // get 1 health and 1 ephemeris from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   TUASSERTE(size_t, 2, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   TUASSERTE(unsigned, 1, ephCount);
   navOut.clear();
      // get 1 time offset from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, toCount);
   navOut.clear();
      // get 0 time offsets from subframe 3 page 2 (because no data available)
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get 3 health and 1 almanac from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   TUASSERTE(size_t, 4, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   TUASSERTE(unsigned, 1, almCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataEphemerisTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   gpstk::PNBGPSCNav2DataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Ephemeris}));
      // get 1 ephemeris from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   navOut.clear();
      // get nothing from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get nothing from subframe 3 page 2
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get nothing from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataAlmanacTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   gpstk::PNBGPSCNav2DataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Almanac}));
      // get nothing from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get nothing from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get nothing from subframe 3 page 2
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get 1 almanac from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, almCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataHealthTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   gpstk::PNBGPSCNav2DataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Health}));
      // get 1 health from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // get nothing from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get nothing from subframe 3 page 2
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get 3 health from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataTimeTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   gpstk::PNBGPSCNav2DataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::TimeOffset}));
      // get nothing from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get 1 time offset from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, toCount);
   navOut.clear();
      // get nothing from subframe 3 page 2
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get nothing from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataEphHealthTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   gpstk::PNBGPSCNav2DataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Ephemeris,
                              gpstk::NavMessageType::Health}));
      // get 1 ephemeris, 1 health from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   TUASSERTE(size_t, 2, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // get nothing from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get nothing from subframe 3 page 2
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get 3 health from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   TUASSERTE(size_t, 3, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
addDataAlmHealthTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "addData");
   gpstk::PNBGPSCNav2DataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Almanac,
                              gpstk::NavMessageType::Health}));
      // get 1 health from subframe 2
   TUASSERTE(bool, true, uut.addData(sf2CNAV2GPS, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // get nothing from subframe 3 page 1
   TUASSERTE(bool, true, uut.addData(sf3p1CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get nothing from subframe 3 page 2
   TUASSERTE(bool, true, uut.addData(sf3p2CNAV2GPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // get 1 almanac, 3 health from subframe 3 page 4
   TUASSERTE(bool, true, uut.addData(sf3p4CNAV2GPS, navOut));
   TUASSERTE(size_t, 4, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 3, heaCount);
   TUASSERTE(unsigned, 1, almCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
processEphTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "processEph");
   gpstk::PNBGPSCNav2DataFactory uut;
   gpstk::NavMessageID nmidExpHea(
      gpstk::NavSatelliteID(4, 4, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::L1CD,
                            gpstk::NavType::GPSCNAV2),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExpEph(
      gpstk::NavSatelliteID(4, 4, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::L1CD,
                            gpstk::NavType::GPSCNAV2),
      gpstk::NavMessageType::Ephemeris);
   gpstk::CommonTime toeExp = gpstk::GPSWeekSecond(2049,351000.0);
   gpstk::CommonTime topExp = gpstk::GPSWeekSecond(2049,321300.0);
   gpstk::CommonTime beginExp = gpstk::GPSWeekSecond(2049,345600.0);
   gpstk::CommonTime endExp = gpstk::GPSWeekSecond(2049,356400.0);
   gpstk::NavDataPtrList navOut;
   gpstk::GPSCNav2Health *hea;
   gpstk::GPSCNav2Eph *eph;
      // success, health and ephemeris
   TUASSERTE(bool, true, uut.processEph(sf2CNAV2GPS, navOut));
   TUASSERTE(size_t, 2, navOut.size());
   heaCount = 0, ephCount = 0, otherCount = 0;
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gpstk::GPSCNav2Health*>(i.get())) != nullptr)
      {
         heaCount++;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, sf2CNAV2GPSct, hea->timeStamp);
         TUASSERTE(gpstk::NavType, gpstk::NavType::GPSCNAV2, hea->signal.nav);
         TUASSERTE(gpstk::NavMessageID, nmidExpHea, hea->signal);
            // NavHealthData has no fields
            // GPSCNav2Health
         TUASSERTE(bool, true, hea->health);
      }
      else if ((eph = dynamic_cast<gpstk::GPSCNav2Eph*>(i.get())) != nullptr)
      {
         ephCount++;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, sf2CNAV2GPSct, eph->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExpEph, eph->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gpstk::CommonTime, sf2CNAV2GPSct, eph->xmitTime);
         TUASSERTE(gpstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(gpstk::CommonTime, toeExp, eph->Toc); // same value as toe
         TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unhealthy, eph->health);
         TUASSERTFE(1.52643770E-06, eph->Cuc);
         TUASSERTFE(1.529045403003692627E-05, eph->Cus);
         TUASSERTFE(8.210546875E+01, eph->Crc);
         TUASSERTFE(3.112890625E+01, eph->Crs);
         TUASSERTFE(3.16649675E-08, eph->Cic);
         TUASSERTFE(1.86264515E-08, eph->Cis);
         TUASSERTFE(1.9394493464900481872, eph->M0);
         TUASSERTFE(4.16410202E-09, eph->dn);
         TUASSERTFE(-1.14162296E-13, eph->dndot);
         TUASSERTFE(3.7740299012511968613E-04, eph->ecc);
         TUASSERTFE(::sqrt(2.656154014453125E+07), eph->Ahalf);
         TUASSERTFE(2.656154014453125E+07, eph->A);
         TUASSERTFE(1.1958599090576171875E-02, eph->Adot);
         TUASSERTFE(-1.0895206889670716599E-01, eph->OMEGA0);
         TUASSERTFE(9.6039230773739858726E-01, eph->i0);
         TUASSERTFE(-1.6173077678173934935E+00, eph->w);
         TUASSERTFE(-7.84259162E-09, eph->OMEGAdot);
         TUASSERTFE(-6.62527597E-11, eph->idot);
         TUASSERTFE(1.5038633137010037899E-04, eph->af0);
         TUASSERTFE(4.96669372E-12, eph->af1);
         TUASSERTFE(0, eph->af2);
         TUASSERTE(gpstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gpstk::CommonTime, endExp, eph->endFit);
            // GPSCNavEph fields
         TUASSERTE(bool, true, eph->healthL1C);
         TUASSERTE(int, -3, eph->uraED);
         TUASSERTE(int, -4, eph->uraNED0);
         TUASSERTE(unsigned, 2, eph->uraNED1);
         TUASSERTE(unsigned, 7, eph->uraNED2);
         TUASSERTE(bool, false, eph->integStat);
         TUASSERTFE(1830.14453125, eph->deltaA);
         TUASSERTFE(3.2554927471973048162e-10, eph->dOMEGAdot);
         TUASSERTE(gpstk::CommonTime, topExp, eph->top);
         TUASSERTFE(-8.7893567979335784912e-09, eph->tgd);
         TUASSERTFE(-7.85803422E-10, eph->iscL1CP);
         TUASSERTFE(-3.49245965E-10, eph->iscL1CD);
      }
      else
      {
         otherCount++;
      }
   }
   TUASSERTE(unsigned, 1, heaCount);
   TUASSERTE(unsigned, 1, ephCount);
   TUASSERTE(unsigned, 0, otherCount);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
processAlmOrbTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "processAlmOrb");
   gpstk::PNBGPSCNav2DataFactory uut;
   gpstk::NavMessageID nmidExpHeaL1(
      gpstk::NavSatelliteID(2, 4, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::L1CD,
                            gpstk::NavType::GPSCNAV2),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExpHeaL2(
      gpstk::NavSatelliteID(2, 4, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L2, gpstk::TrackingCode::L2CM,
                            gpstk::NavType::GPSCNAVL2),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExpHeaL5(
      gpstk::NavSatelliteID(2, 4, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::L5I,
                            gpstk::NavType::GPSCNAVL5),
      gpstk::NavMessageType::Health);
   gpstk::NavMessageID nmidExpAlm(
      gpstk::NavSatelliteID(2, 4, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::L1CD,
                            gpstk::NavType::GPSCNAV2),
      gpstk::NavMessageType::Almanac);
   gpstk::CommonTime toeExp = gpstk::GPSWeekSecond(2049,503808.0);
   gpstk::CommonTime beginExp = sf3p4CNAV2GPSct;
   gpstk::CommonTime endExp = gpstk::CommonTime::END_OF_TIME;
   gpstk::NavDataPtrList navOut;
   gpstk::GPSCNav2Health *hea;
   gpstk::GPSCNav2Alm *alm;
   endExp.setTimeSystem(gpstk::TimeSystem::GPS);
      // success, health and almanac
   TUASSERTE(bool, true, uut.processAlmOrb(sf3p4CNAV2GPS, navOut));
   TUASSERTE(size_t, 4, navOut.size());
   heaCount = 0, almCount = 0, otherCount = 0;
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gpstk::GPSCNav2Health*>(i.get())) != nullptr)
      {
         heaCount++;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, sf3p4CNAV2GPSct, hea->timeStamp);
         bool expHea;
         switch (hea->signal.nav)
         {
            case gpstk::NavType::GPSCNAV2:
               TUASSERTE(gpstk::NavMessageID, nmidExpHeaL1, hea->signal);
               expHea = false;
               break;
            case gpstk::NavType::GPSCNAVL2:
               TUASSERTE(gpstk::NavMessageID, nmidExpHeaL2, hea->signal);
               expHea = false;
               break;
            case gpstk::NavType::GPSCNAVL5:
               TUASSERTE(gpstk::NavMessageID, nmidExpHeaL5, hea->signal);
               expHea = true;
               break;
            default:
               TUFAIL("Unexpected nav type: " +
                      gpstk::StringUtils::asString(hea->signal.nav));
               break;
         }
            // NavHealthData has no fields
            // GPSCNav2Health
         TUASSERTE(bool, expHea, hea->health);
      }
      else if ((alm = dynamic_cast<gpstk::GPSCNav2Alm*>(i.get())) != nullptr)
      {
         almCount++;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, sf3p4CNAV2GPSct, alm->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExpAlm, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gpstk::CommonTime, sf3p4CNAV2GPSct, alm->xmitTime);
         TUASSERTE(gpstk::CommonTime, toeExp, alm->Toe);
         TUASSERTE(gpstk::CommonTime, toeExp, alm->Toc); // same value as toe
         TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, alm->health);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(1.0574880056486639368E-01, alm->M0);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(1.89056396484375E-02, alm->ecc);
         TUASSERTFE(5153.625, alm->Ahalf);
         TUASSERTFE(26559850.640625, alm->A);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(-2.2980949678511763956E+00, alm->OMEGA0);
         TUASSERTFE(0.95417439958456595761, alm->i0);
         TUASSERTFE(-1.7481628553941737092E+00, alm->w);
         TUASSERTFE(-8.0460494360E-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-1.926422119140625E-04, alm->af0);
         TUASSERTFE(-7.2759576142E-12, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gpstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gpstk::CommonTime, endExp, alm->endFit);
            // GPSCNavAlm fields
         TUASSERTE(bool, false, alm->healthL1);
         TUASSERTE(bool, false, alm->healthL2);
         TUASSERTE(bool, true, alm->healthL5);
         TUASSERTFE(1.1696603507628014137E-02, alm->deltai);
         TUASSERTE(unsigned, 2049, alm->wna);
         TUASSERTFE(503808, alm->toa);
      }
      else
      {
         otherCount++;
      }
   }
   TUASSERTE(unsigned, 3, heaCount);
   TUASSERTE(unsigned, 1, almCount);
   TUASSERTE(unsigned, 0, otherCount);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
processUTCIonoTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "processUTCIono");
   gpstk::PNBGPSCNav2DataFactory uut;
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(4, 4, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::L1CD,
                            gpstk::NavType::GPSCNAV2),
      gpstk::NavMessageType::TimeOffset);
   gpstk::NavDataPtrList navOut;
   gpstk::GPSCNav2TimeOffset *to;
      // success, time offset only
   TUASSERTE(bool, true, uut.processUTCIono(sf3p1CNAV2GPS, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   toCount = 0, otherCount = 0;
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gpstk::GPSCNav2TimeOffset*>(i.get())) != nullptr)
      {
         toCount++;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, sf3p1CNAV2GPSct, to->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExp, to->signal);
            // TimeOffsetData has no fields
            // GPSCNav2TimeOffset
         TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::UTC, to->tgt);
         TUASSERTFE(6.5192580223e-09, to->a0);
         TUASSERTFE(1.5099033135e-14, to->a1);
         TUASSERTFE(0, to->a2);
         TUASSERTFE(18, to->deltatLS);
         TUASSERTFE(503808, to->tot);
         TUASSERTE(unsigned, 2049, to->wnot);
         TUASSERTE(unsigned, 1929, to->wnLSF);
         TUASSERTE(unsigned, 7, to->dn);
         TUASSERTFE(18, to->deltatLSF);
      }
      else
      {
         otherCount++;
      }
   }
   TUASSERTE(unsigned, 1, toCount);
   TUASSERTE(unsigned, 0, otherCount);
   TURETURN();
}


unsigned PNBGPSCNav2DataFactory_T ::
processGGTOEOPTest()
{
   TUDEF("PNBGPSCNav2DataFactory", "processGGTOEOP");
   gpstk::PNBGPSCNav2DataFactory uut;
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(4, 4, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::L1CD,
                            gpstk::NavType::GPSCNAV2),
      gpstk::NavMessageType::TimeOffset);
   gpstk::NavDataPtrList navOut;
   gpstk::GPSCNav2TimeOffset *to;
      // make sure we have at least the right number of bits
   TUASSERTE(size_t, 274, sf3p2fakeCNAV2GPS->getNumBits());
      // success, time offset only
   TUASSERTE(bool, true, uut.processGGTOEOP(sf3p2fakeCNAV2GPS, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   toCount = 0, otherCount = 0;
   for (const auto& i : navOut)
   {
      if ((to = dynamic_cast<gpstk::GPSCNav2TimeOffset*>(i.get())) != nullptr)
      {
         toCount++;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, sf3p2fakeCNAV2GPSct, to->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExp, to->signal);
            // TimeOffsetData has no fields
            // GPSCNav2TimeOffset
         TUASSERTE(gpstk::TimeSystem, gpstk::TimeSystem::GLO, to->tgt);
         TUASSERTFE(6.5192580223e-09, to->a0);
         TUASSERTFE(1.5099033135e-14, to->a1);
         TUASSERTFE(0, to->a2);
         TUASSERTFE(0, to->deltatLS);
         TUASSERTFE(3456, to->tot);
         TUASSERTE(unsigned, 2044, to->wnot);
         TUASSERTE(unsigned, 0, to->wnLSF);
         TUASSERTE(unsigned, 0, to->dn);
         TUASSERTFE(0, to->deltatLSF);
      }
      else
      {
         otherCount++;
      }
   }
   TUASSERTE(unsigned, 1, toCount);
   TUASSERTE(unsigned, 0, otherCount);
   TURETURN();
}


void PNBGPSCNav2DataFactory_T ::
countResults(const gpstk::NavDataPtrList& navOut)
{
   almCount = ephCount = toCount = heaCount = otherCount = 0;
   for (const auto& i : navOut)
   {
      if (dynamic_cast<gpstk::GPSCNav2Alm*>(i.get()) != nullptr)
      {
         almCount++;
      }
      else if (dynamic_cast<gpstk::GPSCNav2Eph*>(i.get()) != nullptr)
      {
         ephCount++;
      }
      else if (dynamic_cast<gpstk::GPSCNav2TimeOffset*>(i.get()) != nullptr)
      {
         toCount++;
      }
      else if (dynamic_cast<gpstk::GPSCNav2Health*>(i.get()) != nullptr)
      {
         heaCount++;
      }
      else
      {
         otherCount++;
      }
         //i->dump(cerr, gpstk::DumpDetail::Full);
   }
}


int main()
{
   PNBGPSCNav2DataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.addDataAllTest();
   errorTotal += testClass.addDataAlmanacTest();
   errorTotal += testClass.addDataEphemerisTest();
   errorTotal += testClass.addDataHealthTest();
   errorTotal += testClass.addDataTimeTest();
   errorTotal += testClass.addDataEphHealthTest();
   errorTotal += testClass.addDataAlmHealthTest();
   errorTotal += testClass.processEphTest();
   errorTotal += testClass.processAlmOrbTest();
   errorTotal += testClass.processUTCIonoTest();
   errorTotal += testClass.processGGTOEOPTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
