#include "GPSCNavHealth.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gpstk::SVHealth e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GPSCNavHealth_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned getHealthTest();
};


unsigned GPSCNavHealth_T ::
constructorTest()
{
   TUDEF("GPSCNavHealth", "GPSCNavHealth");
   gpstk::GPSCNavHealth uut;
   TUASSERTE(uint8_t, true, uut.health);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Health,
             uut.signal.messageType);
   TURETURN();
}


unsigned GPSCNavHealth_T ::
getUserTimeTest()
{
   TUDEF("GPSCNavHealth", "getUserTime");
   gpstk::GPSCNavHealth uut;
      // L2 has a 12s cadence, L5 has a 6s cadence
   gpstk::CommonTime expL2(gpstk::GPSWeekSecond(2100,147.0));
   gpstk::CommonTime expL5(gpstk::GPSWeekSecond(2100,141.0));
   uut.timeStamp = gpstk::GPSWeekSecond(2100,135.0);
   uut.signal = gpstk::NavMessageID(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L5, gpstk::TrackingCode::L5I,
                            gpstk::NavType::GPSCNAVL5),
      gpstk::NavMessageType::Health);
   TUASSERTE(gpstk::CommonTime, expL5, uut.getUserTime());
   uut.signal = gpstk::NavMessageID(
      gpstk::NavSatelliteID(1, 1, gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L2, gpstk::TrackingCode::L2CM,
                            gpstk::NavType::GPSCNAVL2),
      gpstk::NavMessageType::Health);
   TUASSERTE(gpstk::CommonTime, expL2, uut.getUserTime());
   TURETURN();
}


unsigned GPSCNavHealth_T ::
getHealthTest()
{
   TUDEF("GPSCNavHealth", "getHealth");
   gpstk::GPSCNavHealth uut;
      // default should be unhealthy
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unhealthy, uut.getHealth());
      // the one condition that should result in healthy
   uut.health = false;
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Healthy, uut.getHealth());
   TURETURN();
}


int main()
{
   GPSCNavHealth_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getHealthTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
