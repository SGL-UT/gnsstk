#include "GPSCNavEph.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GPSCNavEph_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned validateTest();
};


unsigned GPSCNavEph_T ::
constructorTest()
{
   TUDEF("GPSCNavEph", "GPSCNavEph");
   gpstk::GPSCNavEph uut;
   TUASSERTE(uint32_t, 0, uut.pre11);
   TUASSERTE(uint32_t, 0, uut.preClk);
   TUASSERTE(bool, true, uut.healthL1);
   TUASSERTE(bool, true, uut.healthL2);
   TUASSERTE(bool, true, uut.healthL5);
   TUASSERTE(int8_t, -16, uut.uraED);
   TUASSERTE(int8_t, -16, uut.uraNED0);
   TUASSERTE(uint8_t, 0, uut.uraNED1);
   TUASSERTE(uint8_t, 0, uut.uraNED2);
   TUASSERTE(bool, false, uut.alert11);
   TUASSERTE(bool, false, uut.alertClk);
   TUASSERTE(bool, false, uut.integStat);
   TUASSERTE(bool, false, uut.phasingL2C);
   TUASSERTFE(0, uut.deltaA);
   TUASSERTFE(0, uut.dOMEGAdot);
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime::BEGINNING_OF_TIME, uut.top);
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime::BEGINNING_OF_TIME,
             uut.xmit11);
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime::BEGINNING_OF_TIME,
             uut.xmitClk);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Ephemeris,
             uut.signal.messageType);
   TURETURN();
}


unsigned GPSCNavEph_T ::
getUserTimeTest()
{
   TUDEF("GPSCNavEph", "getUserTime");
   gpstk::GPSCNavEph uut;
      // just using made-up numbers
   uut.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   uut.xmitTime = gpstk::GPSWeekSecond(2100,139.0);
   uut.xmit11 = gpstk::GPSWeekSecond(2100,200.0);
   uut.xmitClk = gpstk::GPSWeekSecond(2100,1.0);
      // L2 has a 12s cadence, L5 has a 6s cadence
   gpstk::CommonTime expL2(gpstk::GPSWeekSecond(2100,212.0));
   gpstk::CommonTime expL5(gpstk::GPSWeekSecond(2100,206.0));
   uut.signal = gpstk::NavMessageID(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::L5I,
                            gpstk::NavType::GPSCNAVL5),
      gpstk::NavMessageType::Ephemeris);
   TUASSERTE(gpstk::CommonTime, expL5, uut.getUserTime());
   uut.signal = gpstk::NavMessageID(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L2, gpstk::TrackingCode::L2CM,
                            gpstk::NavType::GPSCNAVL2),
      gpstk::NavMessageType::Ephemeris);
   TUASSERTE(gpstk::CommonTime, expL2, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNavEph_T ::
fixFitTest()
{
   TUDEF("GPSCNavEph", "fixFit");
   gpstk::GPSCNavEph uut;
   gpstk::GPSWeekSecond beginExpWS2(2059, 597600), endExpWS2(2060, 3600);
   gpstk::CommonTime beginExp2(beginExpWS2), endExp2(endExpWS2);
   uut.Toe = gpstk::GPSWeekSecond(2059, 603000);
   uut.xmitTime = gpstk::GPSWeekSecond(2059,597600);
   uut.xmit11 = gpstk::GPSWeekSecond(2059,597612);
   uut.xmitClk = gpstk::GPSWeekSecond(2059,597624);
   TUCATCH(uut.fixFit());
   TUASSERTE(gpstk::CommonTime, beginExp2, uut.beginFit);
   TUASSERTE(gpstk::CommonTime, endExp2, uut.endFit);
      //uut.dump(std::cerr, gpstk::OrbitDataKepler::Detail::Full);
   TURETURN();
}


unsigned GPSCNavEph_T ::
validateTest()
{
   TUDEF("GPSCNavData", "validate");
   gpstk::GPSCNavEph uut;
   TUASSERTE(bool, true, uut.validate());
   uut.pre = 0x22c; // this is not valid
   TUASSERTE(bool, false, uut.validate());
   uut.pre = 0x8b; // this is valid
   uut.pre11 = 0x22c; // this is not valid
   TUASSERTE(bool, false, uut.validate());
   uut.pre11 = 0x8b; // this is valid
   uut.preClk = 0x22c; // this is not valid
   TUASSERTE(bool, false, uut.validate());
   uut.preClk = 0x8b; // this is valid (all 3 preambles == 8b)
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}


int main()
{
   GPSCNavEph_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.validateTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
