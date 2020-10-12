#include "PNBGPSCNavDataFactory.hpp"
#include "TestUtil.hpp"
// #include "GPSCNavTimeOffset.hpp"
// #include "GPSCNavHealth.hpp"
#include "GPSCNavEph.hpp"
// #include "GPSCNavAlm.hpp"
#include "TimeString.hpp"

using namespace std;

class PNBGPSCNavDataFactory_T
{
public:
   PNBGPSCNavDataFactory_T();

   unsigned addDataAllTest();

#include "CNavTestDataDecl.hpp"

      /// Count the various types of messages present in navOut.
   void countResults(const gpstk::NavDataPtrList& navOut);

      /// Counts of messages, set by countResults.
   unsigned almCount, ephCount, toCount, heaCount, otherCount;
};


PNBGPSCNavDataFactory_T ::
PNBGPSCNavDataFactory_T()
      : almCount(0), ephCount(0), toCount(0), heaCount(0), otherCount(0)
{
#include "CNavTestDataDef.hpp"
}


unsigned PNBGPSCNavDataFactory_T ::
addDataAllTest()
{
   TUDEF("PNBGPSCNavDataFactory", "addData");
   gpstk::PNBGPSCNavDataFactory uut;
   gpstk::SatID gloSid(1,gpstk::SatelliteSystem::Glonass);
   gpstk::ObsID gloid(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::G1,
                      gpstk::TrackingCode::Standard);
   gpstk::NavID gloNav(gpstk::NavType::GloCivilF);
   gpstk::PackedNavBitsPtr nonGPS =
      std::make_shared<gpstk::PackedNavBits>(gloSid,gloid,gloNav,"XX",
                                             msg10CNAVct);
   gpstk::NavDataPtrList navOut;
      // should refuse non-GPS data
   TUASSERTE(bool, false, uut.addData(nonGPS, navOut));
      /// @todo check that it also rejects GPS LNAV
   TUASSERTE(bool, true, uut.addData(msg10CNAV, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(msg11CNAV, navOut));
   TUASSERTE(size_t, 0, navOut.size());
   navOut.clear();
   TUASSERTE(bool, true, uut.addData(msg30CNAV, navOut));
   TUASSERTE(size_t, 1, navOut.size());
   countResults(navOut);
   TUASSERTE(unsigned, 1, ephCount);
   navOut.clear();   
   TURETURN();
}


void PNBGPSCNavDataFactory_T ::
countResults(const gpstk::NavDataPtrList& navOut)
{
   almCount = ephCount = toCount = heaCount = otherCount = 0;
   for (const auto& i : navOut)
   {
      // if (dynamic_cast<gpstk::GPSCNavAlm*>(i.get()) != nullptr)
      // {
      //    almCount++;
      // }
      // else
      if (dynamic_cast<gpstk::GPSCNavEph*>(i.get()) != nullptr)
      {
         ephCount++;
      }
      // else if (dynamic_cast<gpstk::GPSCNavTimeOffset*>(i.get()) != nullptr)
      // {
      //    toCount++;
      // }
      // else if (dynamic_cast<gpstk::GPSCNavHealth*>(i.get()) != nullptr)
      // {
      //    heaCount++;
      // }
      else
      {
         otherCount++;
      }
      i->dump(cerr, gpstk::NavData::Detail::Full);
   }
}


int main()
{
   PNBGPSCNavDataFactory_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.addDataAllTest();
   // errorTotal += testClass.addDataAlmanacTest();
   // errorTotal += testClass.addDataEphemerisTest();
   // errorTotal += testClass.addDataHealthTest();
   // errorTotal += testClass.addDataTimeTest();
   // errorTotal += testClass.addDataEphHealthTest();
   // errorTotal += testClass.processEphTest();
   // errorTotal += testClass.processAlmOrbTest();
   // errorTotal += testClass.processSVID51Test();
   // errorTotal += testClass.processSVID63Test();
   // errorTotal += testClass.processSVID56Test();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
