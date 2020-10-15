#include "PNBGPSLNavDataFactory.hpp"
#include "TestUtil.hpp"
#include "GPSLNavTimeOffset.hpp"
#include "GPSLNavHealth.hpp"
#include "GPSLNavEph.hpp"
#include "GPSLNavAlm.hpp"
#include "TimeString.hpp"

using namespace std;

namespace gpstk
{
   ostream& operator<<(ostream& s, GPSLNavEph::L2Codes e)
   {
      s << static_cast<int>(e);
      return s;
   }
}


class PNBGPSLNavDataFactory_T
{
public:
   PNBGPSLNavDataFactory_T();
      // addData tests for a variety of filter states
      /// Test addData with all message types enabled
   unsigned addDataAllTest();
      /// Test addData with almanac selected only
   unsigned addDataAlmanacTest();
      /// Test addData with ephemeris selected only
   unsigned addDataEphemerisTest();
      /// Test addData with health data selected only
   unsigned addDataHealthTest();
      /// Test addData with time offset data selected only
   unsigned addDataTimeTest();
      /// One additional combo test with no particular meaning behind selection.
   unsigned addDataEphHealthTest();
   unsigned processEphTest();
   unsigned processAlmOrbTest();
   unsigned processSVID51Test();
   unsigned processSVID63Test();
   unsigned processSVID56Test();

      /// Count the various types of messages present in navOut.
   void countResults(const gpstk::NavDataPtrList& navOut);

#include "LNavTestDataDecl.hpp"

      /// Counts of messages, set by countResults.
   unsigned almCount, ephCount, toCount, heaCount, otherCount;
};


PNBGPSLNavDataFactory_T ::
PNBGPSLNavDataFactory_T()
      : almCount(0), ephCount(0), toCount(0), heaCount(0), otherCount(0)
{
#include "LNavTestDataDef.hpp"
}


unsigned PNBGPSLNavDataFactory_T ::
addDataAllTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData");
   gpstk::PNBGPSLNavDataFactory uut;
   gpstk::SatID gloSid(1,gpstk::SatelliteSystem::Glonass);
   gpstk::ObsID gloid(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::G1,
                      gpstk::TrackingCode::Standard);
   gpstk::NavID gloNav(gpstk::NavType::GloCivilF);
   gpstk::PackedNavBitsPtr nonGPS = std::make_shared<gpstk::PackedNavBits>(
      gloSid, gloid, gloNav, "XX", almLNAVGPS26ct);
   gpstk::NavDataPtrList navOut;
      // should refuse non-GPS data
   TUASSERTE(bool, false, uut.addData(nonGPS, navOut));
      // The rest is checking that we're processing the good data
      // appropriately, but I'm not bothering to check the detailed
      // contents as that is assumed to be tested in the process*
      // tests.
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
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
addDataAlmanacTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData");
   gpstk::PNBGPSLNavDataFactory uut;
   gpstk::SatID gloSid(1,gpstk::SatelliteSystem::Glonass);
   gpstk::ObsID gloid(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::G1,
                      gpstk::TrackingCode::Standard);
   gpstk::NavID gloNav(gpstk::NavType::GloCivilF);
   gpstk::PackedNavBitsPtr nonGPS = std::make_shared<gpstk::PackedNavBits>(
      gloSid, gloid, gloNav, "XX", almLNAVGPS26ct);
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Almanac}));
      // Add subframe 1, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF1, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add subframe 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add subframe 3, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF3, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add almanac sv id 25, expect nothing
   TUASSERTE(bool, true, uut.addData(almLNAVGPS25, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add almanac sv id 26, expect nothing
   TUASSERTE(bool, true, uut.addData(almLNAVGPS26, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add page 56, expect nothing
   TUASSERTE(bool, true, uut.addData(pg56LNAVGPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add page 63, expect nothing
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add page 51, expect 2 almanacs
   TUASSERTE(bool, true, uut.addData(pg51LNAVGPS, navOut));
   TUASSERTE(size_t, 2, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 2, almCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
addDataEphemerisTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData");
   gpstk::PNBGPSLNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Ephemeris}));
      // Add subframe 1, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF1, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add subframe 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add subframe 3, expect a completed ephemeris.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF3, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   navOut.clear();
      // add almanac sv id 25, expect nothing
   TUASSERTE(bool, true, uut.addData(almLNAVGPS25, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add almanac sv id 26, expect nothing
   TUASSERTE(bool, true, uut.addData(almLNAVGPS26, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add page 56, expect nothing
   TUASSERTE(bool, true, uut.addData(pg56LNAVGPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add page 63, expect nothing
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add page 51, expect nothing
   TUASSERTE(bool, true, uut.addData(pg51LNAVGPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
addDataHealthTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData");
   gpstk::PNBGPSLNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Health}));
      // Add subframe 1, expect 1 health object.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF1, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // add subframe 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add subframe 3, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF3, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add almanac sv id 25, expect 1 health
   TUASSERTE(bool, true, uut.addData(almLNAVGPS25, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // add almanac sv id 26, expect 1 health
   TUASSERTE(bool, true, uut.addData(almLNAVGPS26, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // add page 56, expect nothing
   TUASSERTE(bool, true, uut.addData(pg56LNAVGPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add page 63, expect 8 health
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   TUASSERTE(size_t, 8, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 8, heaCount);
   navOut.clear();
      // add page 51, expect 24 health
   TUASSERTE(bool, true, uut.addData(pg51LNAVGPS, navOut));
   TUASSERTE(size_t, 24, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 24, heaCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
addDataTimeTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData");
   gpstk::PNBGPSLNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::TimeOffset}));
      // Add subframe 1, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF1, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add subframe 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add subframe 3, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF3, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add almanac sv id 25, expect nothing.
   TUASSERTE(bool, true, uut.addData(almLNAVGPS25, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add almanac sv id 26, expect nothing.
   TUASSERTE(bool, true, uut.addData(almLNAVGPS26, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add page 56, expect time offset
   TUASSERTE(bool, true, uut.addData(pg56LNAVGPS, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, toCount);
   navOut.clear();
      // add page 63, expect nothing.
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add page 51, expect nothing.
   TUASSERTE(bool, true, uut.addData(pg51LNAVGPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
addDataEphHealthTest()
{
   TUDEF("PNBGPSLNavDataFactory", "addData");
   gpstk::PNBGPSLNavDataFactory uut;
   gpstk::NavDataPtrList navOut;
   TUCATCH(uut.setTypeFilter({gpstk::NavMessageType::Ephemeris,
                              gpstk::NavMessageType::Health}));
      // Add subframe 1, expect 1 health.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF1, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // add subframe 2, expect nothing.
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add subframe 3, expect the completed ephemeris
   TUASSERTE(bool, true, uut.addData(ephLNAVGPSSF3, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   navOut.clear();
      // add almanac sv id 25, expect 1 health
   TUASSERTE(bool, true, uut.addData(almLNAVGPS25, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // add almanac sv id 26, expect 1 health
   TUASSERTE(bool, true, uut.addData(almLNAVGPS26, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, heaCount);
   navOut.clear();
      // add page 56, expect nothing.
   TUASSERTE(bool, true, uut.addData(pg56LNAVGPS, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // add page 63, expect 8 health
   TUASSERTE(bool, true, uut.addData(pg63LNAVGPS, navOut));
   TUASSERTE(size_t, 8, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 8, heaCount);
   navOut.clear();
      // add page 51, expect 24 health
   TUASSERTE(bool, true, uut.addData(pg51LNAVGPS, navOut));
   TUASSERTE(size_t, 24, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 24, heaCount);
   navOut.clear();
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processEphTest()
{
   TUDEF("PNBGPSLNavDataFactory", "processEph");
   gpstk::PNBGPSLNavDataFactory uut;
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(4, 4, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                            gpstk::NavType::GPSLNAV),
      gpstk::NavMessageType::Health);
   gpstk::CommonTime toeExp = gpstk::GPSWeekSecond(1869,7200.0);
   gpstk::CommonTime beginExp = gpstk::GPSWeekSecond(1869, 0.0);
   gpstk::CommonTime endExp = gpstk::GPSWeekSecond(1869, 14400.0);
   gpstk::NavDataPtrList navOut;
   gpstk::GPSLNavHealth *hea;
   gpstk::GPSLNavEph *eph;
      // success, health data only
   TUASSERTE(bool, true, uut.processEph(1, ephLNAVGPSSF1, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   heaCount = 0, ephCount = 0, otherCount = 0;
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gpstk::GPSLNavHealth*>(i.get())) != nullptr)
      {
         heaCount++;
         nmidExp.messageType = gpstk::NavMessageType::Health;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, ephLNAVGPSSF1ct, hea->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // GPSLNavHealth
            // Stored as uint8_t but I want failures to print numbers,
            // not characters, so I use unsigned.
         TUASSERTE(unsigned, 0, hea->svHealth);
      }
      else
      {
         otherCount++;
      }
   }
   TUASSERTE(unsigned, 1, heaCount);
   TUASSERTE(unsigned, 0, otherCount);
   navOut.clear();
   heaCount = otherCount = 0;
   TUASSERTE(bool, true, uut.processEph(2, ephLNAVGPSSF2, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
      // success, and we have an ephemeris.
   TUASSERTE(bool, true, uut.processEph(3, ephLNAVGPSSF3, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   for (const auto& i : navOut)
   {
      if ((eph = dynamic_cast<gpstk::GPSLNavEph*>(i.get())) != nullptr)
      {
         ephCount++;
         nmidExp.messageType = gpstk::NavMessageType::Ephemeris;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, ephLNAVGPSSF1ct, eph->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExp, eph->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gpstk::CommonTime, ephLNAVGPSSF1ct, eph->xmitTime);
         TUASSERTE(gpstk::CommonTime, toeExp, eph->Toe);
         TUASSERTE(gpstk::CommonTime, toeExp, eph->Toc); // same value as toe
         TUASSERTE(bool, true, eph->healthy);
         TUASSERTFE(-4.73111868E-07, eph->Cuc);
         TUASSERTFE(8.9444220066070556641e-06, eph->Cus);
         TUASSERTFE(1.97843750E+02, eph->Crc);
         TUASSERTFE(-6.62500000E+00, eph->Crs);
         TUASSERTFE(-2.14204192E-07, eph->Cis);
         TUASSERTFE(-2.2910535335540771484e-07, eph->Cic);
         TUASSERTFE(1.6427405986254530745, eph->M0);
         TUASSERTFE(4.92949105E-09, eph->dn);
         TUASSERTFE(0, eph->dndot);
         TUASSERTFE(0.012018375913612544537, eph->ecc);
         TUASSERTFE(5153.6434803009033203, eph->Ahalf);
         TUASSERTFE(26560041.122048005462, eph->A);
         TUASSERTFE(0, eph->Adot);
         TUASSERTFE(1.4502525982818956773, eph->OMEGA0);
         TUASSERTFE(0.9404723483911350268, eph->i0);
         TUASSERTFE(1.1153119288528794417, eph->w);
         TUASSERTFE(-8.27748765E-09, eph->OMEGAdot);
         TUASSERTFE(3.90730561E-10, eph->idot);
         TUASSERTFE(-4.0083192288875579834e-05, eph->af0);
         TUASSERTFE(-5.34328137E-12, eph->af1);
         TUASSERTFE(0, eph->af2);
         TUASSERTE(gpstk::CommonTime, beginExp, eph->beginFit);
         TUASSERTE(gpstk::CommonTime, endExp, eph->endFit);
            // GPSLNavData fields
         TUASSERTE(uint32_t, 0x8b, eph->pre);
         TUASSERTE(uint32_t, 0x34d, eph->tlm);
         TUASSERTE(bool, false, eph->alert);
         TUASSERTE(bool, true, eph->asFlag);
            // GPSLNavEph fields
         TUASSERTE(uint32_t, 0x8b, eph->pre2);
         TUASSERTE(uint32_t, 0x34d, eph->tlm2);
         TUASSERTE(uint32_t, 0x8b, eph->pre3);
         TUASSERTE(uint32_t, 0x34d, eph->tlm3);
         TUASSERTE(uint16_t, 0x4a, eph->iodc);
         TUASSERTE(uint16_t, 0x4a, eph->iode);
         TUASSERTE(unsigned, 0, eph->fitIntFlag);
         TUASSERTE(unsigned, 0, eph->healthBits);
         TUASSERTE(unsigned, 1, eph->uraIndex);
         TUASSERTFE(-6.519258E-09, eph->tgd);
         TUASSERTE(bool, false, eph->alert2);
         TUASSERTE(bool, false, eph->alert3);
         TUASSERTE(bool, true, eph->asFlag2);
         TUASSERTE(bool, true, eph->asFlag3);
         TUASSERTE(gpstk::GPSLNavEph::L2Codes,
                   gpstk::GPSLNavEph::L2Codes::Pcode, eph->codesL2);
         TUASSERTE(bool, false, eph->L2Pdata);
      }
      else
      {
         otherCount++;
      }
   }
   TUASSERTE(unsigned, 1, ephCount);
   TUASSERTE(unsigned, 0, otherCount);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processAlmOrbTest()
{
   TUDEF("PNBGPSLNavDataFactory", "processAlmOrb");
   gpstk::PNBGPSLNavDataFactory uut;
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(25, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                            gpstk::NavType::GPSLNAV),
      gpstk::NavMessageType::Health);
   gpstk::CommonTime toaExp = gpstk::GPSWeekSecond(1869,147456.0);
   gpstk::CommonTime beginExp = gpstk::GPSWeekSecond(1868, 500256.0);
   gpstk::CommonTime endExp = gpstk::GPSWeekSecond(1869, 413856.0);
   gpstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.processAlmOrb(25, almLNAVGPS25, navOut));
   TUASSERTE(size_t, 1, navOut.size());
      // no page 51 yet so we only get the health data back
   gpstk::GPSLNavHealth *hea =
      dynamic_cast<gpstk::GPSLNavHealth*>(navOut.begin()->get());
   TUASSERT(hea != nullptr);
      // NavData fields
   TUASSERTE(gpstk::CommonTime, almLNAVGPS25ct, hea->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExp, hea->signal);
      // NavHealthData has no fields
      // GPSLNavHealth
   TUASSERTE(unsigned, 0, hea->svHealth);
   TUCSM("processSVID51()");
   navOut.clear();
   TUASSERTE(bool, true, uut.processSVID51(pg51LNAVGPS, navOut));
      // 1 GPSLNavAlm and 24 GPSLNavHealth.
   TUASSERTE(size_t, 25, navOut.size());
   heaCount = 0, almCount = 0;
      // sv/page ID 51 contains health information for 24 satellites,
      // starting with PRN 1.
   unsigned subjPRN = 1;
   gpstk::GPSLNavAlm *alm;
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gpstk::GPSLNavHealth*>(i.get())) != nullptr)
      {
         heaCount++;
         nmidExp.messageType = gpstk::NavMessageType::Health;
         nmidExp.sat.id = subjPRN++;
            // Yes this code can cause seg faults on failure, but that's ok.
         gpstk::GPSLNavHealth *hea =
            dynamic_cast<gpstk::GPSLNavHealth*>(i.get());
         TUASSERT(hea != nullptr);
            // NavData fields
         TUASSERTE(gpstk::CommonTime, pg51LNAVGPSct, hea->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // GPSLNavHealth
            // Stored as uint8_t but I want failures to print numbers,
            // not characters, so I use unsigned.
            // PRN 10 is unhealthy, but we've already incremented subjPRN
            // so compare it to 11.
         TUASSERTE(unsigned, (subjPRN == 11 ? 0x3f : 0), hea->svHealth);
      }
      else if ((alm = dynamic_cast<gpstk::GPSLNavAlm*>(i.get())) != nullptr)
      {
         almCount++;
         nmidExp.messageType = gpstk::NavMessageType::Almanac;
            // make sure to reset the expected sat ID to the
            // appropriate value for the almanac page.
         nmidExp.sat.id = 25;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, almLNAVGPS25ct, alm->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExp, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gpstk::CommonTime, almLNAVGPS25ct, alm->xmitTime);
         TUASSERTE(gpstk::CommonTime, toaExp, alm->Toe);
         TUASSERTE(gpstk::CommonTime, toaExp, alm->Toc); // same value as toe
         TUASSERTE(bool, true, alm->healthy);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(1.714684548614031856, alm->M0);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(0.004675388336181640625, alm->ecc);
         TUASSERTFE(5153.56005859375, alm->Ahalf);
         TUASSERTFE(5153.56005859375 * 5153.56005859375, alm->A);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(-0.62474775600600196412, alm->OMEGA0);
         TUASSERTFE(0.97866416317866211649, alm->i0);
         TUASSERTFE(0.72259183672470084225, alm->w);
         TUASSERTFE(-7.92032991E-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-6.103515625e-05, alm->af0);
         TUASSERTFE(-3.63797881E-12, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gpstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gpstk::CommonTime, endExp, alm->endFit);
            // GPSLNavData fields
         TUASSERTE(uint32_t, 0x8b, alm->pre);
         TUASSERTE(uint32_t, 0x34d, alm->tlm);
         TUASSERTE(bool, false, alm->alert);
         TUASSERTE(bool, true, alm->asFlag);
            // GPSLNavAlm fields
         TUASSERTE(unsigned, 0, alm->healthBits);
         TUASSERTFE(0.036186367101724167816, alm->deltai);
         TUASSERTFE(147456, alm->toa);
      }
   }
   TUASSERTE(unsigned, 24, heaCount);
   TUASSERTE(unsigned, 1, almCount);
      // Now that a page 51 has been loaded, we should immediately get
      // both the almanac and health objects in return.
   navOut.clear();
   TUCSM("processAlmOrb");
   TUASSERTE(bool, true, uut.processAlmOrb(26, almLNAVGPS26, navOut));
   TUASSERTE(size_t, 2, navOut.size());
   nmidExp.sat.id = 26;
   heaCount = 0, almCount = 0;
   for (const auto& i : navOut)
   {
      if ((hea = dynamic_cast<gpstk::GPSLNavHealth*>(i.get())) != nullptr)
      {
         heaCount++;
         nmidExp.messageType = gpstk::NavMessageType::Health;
            // Yes this code can cause seg faults on failure, but that's ok.
         gpstk::GPSLNavHealth *hea =
            dynamic_cast<gpstk::GPSLNavHealth*>(i.get());
         TUASSERT(hea != nullptr);
            // NavData fields
         TUASSERTE(gpstk::CommonTime, almLNAVGPS26ct, hea->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExp, hea->signal);
            // NavHealthData has no fields
            // GPSLNavHealth
            // Stored as uint8_t but I want failures to print numbers,
            // not characters, so I use unsigned.
         TUASSERTE(unsigned, 0, hea->svHealth);
      }
      else if ((alm = dynamic_cast<gpstk::GPSLNavAlm*>(i.get())) != nullptr)
      {
         almCount++;
         nmidExp.messageType = gpstk::NavMessageType::Almanac;
            // NavData fields
         TUASSERTE(gpstk::CommonTime, almLNAVGPS26ct, alm->timeStamp);
         TUASSERTE(gpstk::NavMessageID, nmidExp, alm->signal);
            // OrbitData has no fields
            // OrbitDataKepler fields
         TUASSERTE(gpstk::CommonTime, almLNAVGPS26ct, alm->xmitTime);
         TUASSERTE(gpstk::CommonTime, toaExp, alm->Toe);
         TUASSERTE(gpstk::CommonTime, toaExp, alm->Toc); // same value as toe
         TUASSERTE(bool, true, alm->healthy);
         TUASSERTFE(0, alm->Cuc);
         TUASSERTFE(0, alm->Cus);
         TUASSERTFE(0, alm->Crc);
         TUASSERTFE(0, alm->Crs);
         TUASSERTFE(0, alm->Cis);
         TUASSERTFE(0, alm->Cic);
         TUASSERTFE(1.3982957680142222845, alm->M0);
         TUASSERTFE(0, alm->dn);
         TUASSERTFE(0, alm->dndot);
         TUASSERTFE(0.0002918243408203125, alm->ecc);
         TUASSERTFE(5153.60791015625, alm->Ahalf);
         TUASSERTFE(5153.60791015625 * 5153.60791015625, alm->A);
         TUASSERTFE(0, alm->Adot);
         TUASSERTFE(-0.63010732608789443709, alm->OMEGA0);
         TUASSERTFE(0.96071778638289062613, alm->i0);
         TUASSERTFE(-0.27279354095737701202, alm->w);
         TUASSERTFE(-8.1031946734407515747e-09, alm->OMEGAdot);
         TUASSERTFE(0, alm->idot);
         TUASSERTFE(-0.0001068115234375, alm->af0);
         TUASSERTFE(-1.4551915228366851807e-11, alm->af1);
         TUASSERTFE(0, alm->af2);
         TUASSERTE(gpstk::CommonTime, beginExp, alm->beginFit);
         TUASSERTE(gpstk::CommonTime, endExp, alm->endFit);
            // GPSLNavData fields
         TUASSERTE(uint32_t, 0x8b, alm->pre);
         TUASSERTE(uint32_t, 0x34d, alm->tlm);
         TUASSERTE(bool, false, alm->alert);
         TUASSERTE(bool, true, alm->asFlag);
            // GPSLNavAlm fields
         TUASSERTE(unsigned, 0, alm->healthBits);
         TUASSERTFE(0.018239990305952701738, alm->deltai);
         TUASSERTFE(147456, alm->toa);
      }
   }
   TUASSERTE(unsigned, 1, heaCount);
   TUASSERTE(unsigned, 1, almCount);
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processSVID51Test()
{
   TUDEF("PNBGPSLNavDataFactory", "processSVID51");
   gpstk::PNBGPSLNavDataFactory uut;
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                            gpstk::NavType::GPSLNAV),
      gpstk::NavMessageType::Health);
   gpstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.processSVID51(pg51LNAVGPS, navOut));
   TUASSERTE(size_t, 24, navOut.size());
      // sv/page ID 51 contains health information for 24 satellites,
      // starting with PRN 1.
   unsigned subjPRN = 1;
   for (const auto& i : navOut)
   {
      nmidExp.sat.id = subjPRN++;
         // Yes this code can cause seg faults on failure, but that's ok.
      gpstk::GPSLNavHealth *hea =
         dynamic_cast<gpstk::GPSLNavHealth*>(i.get());
      TUASSERT(hea != nullptr);
         // NavData fields
      TUASSERTE(gpstk::CommonTime, pg51LNAVGPSct, hea->timeStamp);
      TUASSERTE(gpstk::NavMessageID, nmidExp, hea->signal);
         // NavHealthData has no fields
         // GPSLNavHealth
         // Stored as uint8_t but I want failures to print numbers,
         // not characters, so I use unsigned.
         // PRN 10 is unhealthy, but we've already incremented subjPRN
         // so compare it to 11.
      TUASSERTE(unsigned, (subjPRN == 11 ? 0x3f : 0), hea->svHealth);
   }
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processSVID63Test()
{
   TUDEF("PNBGPSLNavDataFactory", "processSVID63");
   gpstk::PNBGPSLNavDataFactory uut;
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                            gpstk::NavType::GPSLNAV),
      gpstk::NavMessageType::Health);
   gpstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.processSVID63(pg63LNAVGPS, navOut));
   TUASSERTE(size_t, 8, navOut.size());
      // sv/page ID 63 contains health information for 8 satellites,
      // starting with PRN 25.
   unsigned subjPRN = 25;
   for (const auto& i : navOut)
   {
      nmidExp.sat.id = subjPRN++;
         // Yes this code can cause seg faults on failure, but that's ok.
      gpstk::GPSLNavHealth *hea =
         dynamic_cast<gpstk::GPSLNavHealth*>(i.get());
      TUASSERT(hea != nullptr);
         // NavData fields
      TUASSERTE(gpstk::CommonTime, pg63LNAVGPSct, hea->timeStamp);
      TUASSERTE(gpstk::NavMessageID, nmidExp, hea->signal);
         // NavHealthData has no fields
         // GPSLNavHealth
         // Stored as uint8_t but I want failures to print numbers,
         // not characters, so I use unsigned.
         /** @todo Add a test for SVID63 where there's actually an
          * unhealthy satellite. */
      TUASSERTE(unsigned, 0, hea->svHealth);
   }
   TURETURN();
}


unsigned PNBGPSLNavDataFactory_T ::
processSVID56Test()
{
   TUDEF("PNBGPSLNavDataFactory", "processSVID56");
   gpstk::PNBGPSLNavDataFactory uut;
   gpstk::NavMessageID nmidExp(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                            gpstk::NavType::GPSLNAV),
      gpstk::NavMessageType::TimeOffset);
   gpstk::NavDataPtrList navOut;
   TUASSERTE(bool, true, uut.processSVID56(pg56LNAVGPS, navOut));
   TUASSERTE(size_t, 1, navOut.size());
      // Yes this code can cause seg faults on failure, but that's ok.
   gpstk::GPSLNavTimeOffset *to =
      dynamic_cast<gpstk::GPSLNavTimeOffset*>(navOut.begin()->get());
   TUASSERT(to != nullptr);
      // NavData fields
   TUASSERTE(gpstk::CommonTime, pg56LNAVGPSct, to->timeStamp);
   TUASSERTE(gpstk::NavMessageID, nmidExp, to->signal);
      // TimeOffsetData has no fields
      // GPSLNavTimeOffset fields
   TUASSERTFE(17, to->deltatLS);
   TUASSERTFE(4.65661287E-09, to->a0);
   TUASSERTFE(1.42108547E-14, to->a1);
   TUASSERTFE(147456, to->tot);
   TUASSERTE(unsigned, 1869, to->wnt);
   TUASSERTE(unsigned, 1851, to->wnLSF);
   TUASSERTE(unsigned, 3, to->dn);
   TUASSERTFE(17, to->deltatLSF);
   TURETURN();
}


void PNBGPSLNavDataFactory_T ::
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
   PNBGPSLNavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.addDataAllTest();
   errorTotal += testClass.addDataAlmanacTest();
   errorTotal += testClass.addDataEphemerisTest();
   errorTotal += testClass.addDataHealthTest();
   errorTotal += testClass.addDataTimeTest();
   errorTotal += testClass.addDataEphHealthTest();
   errorTotal += testClass.processEphTest();
   errorTotal += testClass.processAlmOrbTest();
   errorTotal += testClass.processSVID51Test();
   errorTotal += testClass.processSVID63Test();
   errorTotal += testClass.processSVID56Test();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
