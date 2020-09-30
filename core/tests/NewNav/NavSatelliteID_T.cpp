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
   unsigned equalTest();
   unsigned lessThanTest();
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


unsigned NavSatelliteID_T ::
equalTest()
{
   TUDEF("NavSatelliteID", "operator==");
   gpstk::NavSatelliteID nsid01(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid02(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid03(1, 1, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid04(2, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid05(1, 2, gpstk::SatelliteSystem::QZSS,
                                gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid06(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L2, gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid07(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1, gpstk::TrackingCode::Y,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid08(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1, gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSCNAV2);
   gpstk::NavSatelliteID nsid09(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::Any,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid10(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::Any,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid11(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::Any);
   gpstk::NavSatelliteID nsid12(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid13(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid14(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid15(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid16(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid17(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid18(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   gpstk::NavSatelliteID nsid19(1, 2, gpstk::SatelliteSystem::GPS,
                                gpstk::CarrierBand::L1,
                                gpstk::TrackingCode::CA,
                                gpstk::NavType::GPSLNAV);
   nsid12.sat = gpstk::WildSatID(1); // wildcard system
   nsid13.sat = gpstk::WildSatID(2); // wildcard system
   nsid14.sat = gpstk::WildSatID(gpstk::SatelliteSystem::GPS);  // wildcard sat
   nsid15.sat = gpstk::WildSatID(gpstk::SatelliteSystem::QZSS); // wildcard sat
   nsid16.xmitSat = gpstk::WildSatID(1); // wildcard system
   nsid17.xmitSat = gpstk::WildSatID(2); // wildcard system
   nsid18.xmitSat = gpstk::WildSatID(gpstk::SatelliteSystem::GPS);  // wild sat
   nsid19.xmitSat = gpstk::WildSatID(gpstk::SatelliteSystem::QZSS); // wild sat
   TUASSERTE(bool, true,  nsid01 == nsid02);
   TUASSERTE(bool, false, nsid01 == nsid03);
   TUASSERTE(bool, false, nsid01 == nsid04);
   TUASSERTE(bool, false, nsid01 == nsid05);
   TUASSERTE(bool, false, nsid01 == nsid06);
   TUASSERTE(bool, false, nsid01 == nsid07);
   TUASSERTE(bool, false, nsid01 == nsid08);
   TUASSERTE(bool, true,  nsid01 == nsid09);
   TUASSERTE(bool, true,  nsid01 == nsid10);
   TUASSERTE(bool, true,  nsid01 == nsid11);
   TUASSERTE(bool, true,  nsid01 == nsid12);
   TUASSERTE(bool, false, nsid01 == nsid13);
   TUASSERTE(bool, true,  nsid01 == nsid14);
   TUASSERTE(bool, false, nsid01 == nsid15);
   TUASSERTE(bool, false, nsid01 == nsid16);
   TUASSERTE(bool, true,  nsid01 == nsid17);
   TUASSERTE(bool, true,  nsid01 == nsid18);
   TUASSERTE(bool, false, nsid01 == nsid19);
TURETURN();
}


unsigned NavSatelliteID_T ::
lessThanTest()
{
   TUDEF("NavSatelliteID", "operator<");
   TURETURN();
}


int main()
{
   NavSatelliteID_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.equalTest();
   errorTotal += testClass.lessThanTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}

