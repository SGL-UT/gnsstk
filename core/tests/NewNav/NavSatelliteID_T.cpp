#include "NavSatelliteID.hpp"
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


class NavSatelliteID_T
{
public:
   unsigned constructorTest();
};


unsigned NavSatelliteID_T ::
constructorTest()
{
   TUDEF("NavSatelliteID", "NavSatelliteID");
   gpstk::NavSatelliteID nsid1;
   gpstk::WildSatID exp1a(0,gpstk::SatelliteSystem::Unknown);
   gpstk::WildSatID exp1b(0,gpstk::SatelliteSystem::GPS);
      // test for exact match
   TUASSERTE(gpstk::WildSatID, exp1a, nsid1.sat);
   TUASSERTE(gpstk::WildSatID, exp1a, nsid1.xmitSat);
      // test for expected mismatch
   TUASSERT(nsid1.sat != exp1b);
   TUASSERT(nsid1.xmitSat != exp1b);
      // the other constructor...
   gpstk::NavSatelliteID nsid2(23, 32, gpstk::SatelliteSystem::Transit,
                               gpstk::CarrierBand::L5, gpstk::TrackingCode::Y,
                               gpstk::NavType::GPSMNAV);
   gpstk::WildSatID exps2a(23,gpstk::SatelliteSystem::Transit);
   gpstk::WildSatID expx2a(32,gpstk::SatelliteSystem::Transit);
   gpstk::WildSatID exps2b(23,gpstk::SatelliteSystem::GPS);
   gpstk::WildSatID expx2b(32,gpstk::SatelliteSystem::GPS);
   TUASSERTE(gpstk::WildSatID, exps2a, nsid2.sat);
   TUASSERTE(gpstk::WildSatID, expx2a, nsid2.xmitSat);
   TUASSERT(nsid2.sat != exps2b);
   TUASSERT(nsid2.xmitSat != expx2b);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::Transit, nsid2.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L5, nsid2.carrier);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::Y, nsid2.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSMNAV, nsid2.nav);
   TURETURN();
}


int main()
{
   NavSatelliteID_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}

