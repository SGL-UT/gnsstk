#include "NavSignalID.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::SatelliteSystem e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::CarrierBand e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::TrackingCode e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::NavType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class NavSignalID_T
{
public:
   unsigned constructorTest();
};


unsigned NavSignalID_T ::
constructorTest()
{
   TUDEF("NavSignalID", "NavSignalID");
      // test default constructor first
   gpstk::NavSignalID def;
   TUASSERTE(gpstk::SatelliteSystem,gpstk::SatelliteSystem::Unknown,def.system);
   TUASSERTE(gpstk::CarrierBand,gpstk::CarrierBand::Unknown,def.carrier);
   TUASSERTE(gpstk::TrackingCode,gpstk::TrackingCode::Unknown,def.code);
   TUASSERTE(gpstk::NavType,gpstk::NavType::Unknown,def.nav);
      // test the fill-in-everything constructor
   gpstk::NavSignalID test2(gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::Y,
                            gpstk::NavType::GPSLNAV);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS, test2.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1, test2.carrier);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::Y, test2.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, test2.nav);
   TURETURN();
}


int main()
{
   NavSignalID_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
