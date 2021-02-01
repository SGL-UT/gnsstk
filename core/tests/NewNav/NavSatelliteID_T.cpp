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
      // test default constructor
   gpstk::NavSatelliteID nsid1;
   gpstk::SatID exp1a(0,gpstk::SatelliteSystem::Unknown);
   gpstk::SatID exp1b(0,gpstk::SatelliteSystem::GPS);
      // test for exact match
   TUASSERTE(gpstk::SatID, exp1a, nsid1.sat);
   TUASSERTE(gpstk::SatID, exp1a, nsid1.xmitSat);
      // test for expected mismatch
   TUASSERT(nsid1.sat != exp1b);
   TUASSERT(nsid1.xmitSat != exp1b);
      // test constructor with integer satellite IDs
   gpstk::NavSatelliteID nsid2(23, 32, gpstk::SatelliteSystem::Transit,
                               gpstk::CarrierBand::L5, gpstk::TrackingCode::Y,
                               gpstk::NavType::GPSMNAV);
   gpstk::SatID exps2a(23,gpstk::SatelliteSystem::Transit);
   gpstk::SatID expx2a(32,gpstk::SatelliteSystem::Transit);
   gpstk::SatID exps2b(23,gpstk::SatelliteSystem::GPS);
   gpstk::SatID expx2b(32,gpstk::SatelliteSystem::GPS);
   TUASSERTE(gpstk::SatID, exps2a, nsid2.sat);
   TUASSERTE(gpstk::SatID, expx2a, nsid2.xmitSat);
   TUASSERT(nsid2.sat != exps2b);
   TUASSERT(nsid2.xmitSat != expx2b);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::Transit,
             nsid2.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L5, nsid2.obs.band);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::Y, nsid2.obs.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSMNAV, nsid2.nav);
      // test constructor with wildcard transmit satellite
   gpstk::NavSatelliteID nsid7(23, gpstk::SatelliteSystem::Transit,
                               gpstk::CarrierBand::L5, gpstk::TrackingCode::Y,
                               gpstk::NavType::GPSMNAV);
   gpstk::SatID expx7a;
   expx7a.makeWild();
   TUASSERTE(gpstk::SatID, exps2a, nsid7.sat);
   TUASSERTE(gpstk::SatID, expx7a, nsid7.xmitSat);
   TUASSERT(nsid7.sat != exps2b);
      // should match because wildcard.
   TUASSERT(nsid7.xmitSat == expx2b);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::Transit,
             nsid7.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L5, nsid7.obs.band);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::Y, nsid7.obs.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSMNAV, nsid7.nav);
      // test "subframe 5 page 25" constructor
   gpstk::SatID exps3a(17,gpstk::SatelliteSystem::GPS);
   gpstk::SatID exps3b(23,gpstk::SatelliteSystem::GPS);
   gpstk::ObsID oid3a(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::L5,
                      gpstk::TrackingCode::Y);
   gpstk::NavID nid3a(gpstk::NavType::GPSMNAV);
   oid3a.freqOffs = -7;
   oid3a.freqOffsWild = false;
   oid3a.mcode = 0x12345678;
   oid3a.mcodeMask = 0xffffffff;
   gpstk::NavSatelliteID nsid3(17, exps3b, oid3a, nid3a);
   TUASSERTE(gpstk::SatID, exps3a, nsid3.sat);
   TUASSERTE(gpstk::SatID, exps3b, nsid3.xmitSat);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
             nsid3.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L5, nsid3.obs.band);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::Y, nsid3.obs.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSMNAV, nsid3.nav);
   TUASSERTE(gpstk::ObsID, oid3a, nsid3.obs);
      // test "QZSS CNAV" constructor
   gpstk::SatID exps4a(17,gpstk::SatelliteSystem::GPS);
   gpstk::SatID exps4b(23,gpstk::SatelliteSystem::QZSS);
   gpstk::NavSatelliteID nsid4(exps4a, exps4b, oid3a, nid3a);
   TUASSERTE(gpstk::SatID, exps4a, nsid4.sat);
   TUASSERTE(gpstk::SatID, exps4b, nsid4.xmitSat);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::QZSS,
             nsid4.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L5, nsid4.obs.band);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::Y, nsid4.obs.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSMNAV, nsid4.nav);
   TUASSERTE(gpstk::ObsID, oid3a, nsid3.obs);
      // test yet another constructor
   gpstk::SatID exps5a(17,gpstk::SatelliteSystem::GPS);
   gpstk::SatID exps5b(23,gpstk::SatelliteSystem::GPS);
   gpstk::NavSatelliteID nsid5(exps5a, exps5b, gpstk::CarrierBand::L5,
                               gpstk::TrackingCode::Y, gpstk::NavType::GPSMNAV);
   TUASSERTE(gpstk::SatID, exps5a, nsid5.sat);
   TUASSERTE(gpstk::SatID, exps5b, nsid5.xmitSat);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
             nsid5.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L5, nsid5.obs.band);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::Y, nsid5.obs.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSMNAV, nsid5.nav);
      // test the SatID constructor
   gpstk::SatID exps6a(17,gpstk::SatelliteSystem::GPS);
   gpstk::NavSatelliteID nsid6(exps6a);
   TUASSERTE(gpstk::SatID, exps6a, nsid6.sat);
   TUASSERTE(bool, true, nsid6.xmitSat.wildId);
   TUASSERTE(bool, true, nsid6.xmitSat.wildSys);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS,
             nsid6.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::Any, nsid6.obs.band);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::Any, nsid6.obs.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::Any, nsid6.nav);
   
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
   nsid12.sat = gpstk::SatID(1); // wildcard system
   nsid13.sat = gpstk::SatID(2); // wildcard system
   nsid14.sat = gpstk::SatID(gpstk::SatelliteSystem::GPS);  // wildcard sat
   nsid15.sat = gpstk::SatID(gpstk::SatelliteSystem::QZSS); // wildcard sat
   nsid16.xmitSat = gpstk::SatID(1); // wildcard system
   nsid17.xmitSat = gpstk::SatID(2); // wildcard system
   nsid18.xmitSat = gpstk::SatID(gpstk::SatelliteSystem::GPS);  // wild sat
   nsid19.xmitSat = gpstk::SatID(gpstk::SatelliteSystem::QZSS); // wild sat
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
   TUCSM("operator!=");
   TUASSERTE(bool, false, nsid01 != nsid02);
   TUASSERTE(bool, true,  nsid01 != nsid03);
   TUASSERTE(bool, true,  nsid01 != nsid04);
   TUASSERTE(bool, true,  nsid01 != nsid05);
   TUASSERTE(bool, true,  nsid01 != nsid06);
   TUASSERTE(bool, true,  nsid01 != nsid07);
   TUASSERTE(bool, true,  nsid01 != nsid08);
   TUASSERTE(bool, false, nsid01 != nsid09);
   TUASSERTE(bool, false, nsid01 != nsid10);
   TUASSERTE(bool, false, nsid01 != nsid11);
   TUASSERTE(bool, false, nsid01 != nsid12);
   TUASSERTE(bool, true,  nsid01 != nsid13);
   TUASSERTE(bool, false, nsid01 != nsid14);
   TUASSERTE(bool, true,  nsid01 != nsid15);
   TUASSERTE(bool, true,  nsid01 != nsid16);
   TUASSERTE(bool, false, nsid01 != nsid17);
   TUASSERTE(bool, false, nsid01 != nsid18);
   TUASSERTE(bool, true,  nsid01 != nsid19);
   TURETURN();
}


unsigned NavSatelliteID_T ::
lessThanTest()
{
   TUDEF("NavSatelliteID", "operator<");
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
   nsid12.sat = gpstk::SatID(1); // wildcard system
   nsid13.sat = gpstk::SatID(2); // wildcard system
   nsid14.sat = gpstk::SatID(gpstk::SatelliteSystem::GPS);  // wildcard sat
   nsid15.sat = gpstk::SatID(gpstk::SatelliteSystem::QZSS); // wildcard sat
   nsid16.xmitSat = gpstk::SatID(1); // wildcard system
   nsid17.xmitSat = gpstk::SatID(2); // wildcard system
   nsid18.xmitSat = gpstk::SatID(gpstk::SatelliteSystem::GPS);  // wild sat
   nsid19.xmitSat = gpstk::SatID(gpstk::SatelliteSystem::QZSS); // wild sat
   TUASSERTE(bool, false, nsid01 < nsid02);
   TUASSERTE(bool, false, nsid02 < nsid01);
   TUASSERTE(bool, false, nsid01 < nsid03);
   TUASSERTE(bool, true,  nsid03 < nsid01);
   TUASSERTE(bool, true,  nsid01 < nsid04);
   TUASSERTE(bool, false, nsid04 < nsid01);
   TUASSERTE(bool, true,  nsid01 < nsid05);
   TUASSERTE(bool, false, nsid05 < nsid01);
   TUASSERTE(bool, true,  nsid01 < nsid06);
   TUASSERTE(bool, false, nsid06 < nsid01);
   TUASSERTE(bool, true,  nsid01 < nsid07);
   TUASSERTE(bool, false, nsid07 < nsid01);
   TUASSERTE(bool, true,  nsid01 < nsid08);
   TUASSERTE(bool, false, nsid08 < nsid01);
   TUASSERTE(bool, false, nsid01 < nsid09);
   TUASSERTE(bool, false, nsid09 < nsid01);
   TUASSERTE(bool, false, nsid01 < nsid10);
   TUASSERTE(bool, false, nsid10 < nsid01);
   TUASSERTE(bool, false, nsid01 < nsid11);
   TUASSERTE(bool, false, nsid11 < nsid01);
   TUASSERTE(bool, false, nsid01 < nsid12);
   TUASSERTE(bool, false, nsid12 < nsid01);
   TUASSERTE(bool, true,  nsid01 < nsid13);
   TUASSERTE(bool, false, nsid13 < nsid01);
   TUASSERTE(bool, false, nsid01 < nsid14);
   TUASSERTE(bool, false, nsid14 < nsid01);
   TUASSERTE(bool, true,  nsid01 < nsid15);
   TUASSERTE(bool, false, nsid15 < nsid01);
   TUASSERTE(bool, false, nsid01 < nsid16);
   TUASSERTE(bool, true,  nsid16 < nsid01);
   TUASSERTE(bool, false, nsid01 < nsid17);
   TUASSERTE(bool, false, nsid17 < nsid01);
   TUASSERTE(bool, false, nsid01 < nsid18);
   TUASSERTE(bool, false, nsid18 < nsid01);
   TUASSERTE(bool, true,  nsid01 < nsid19);
   TUASSERTE(bool, false, nsid19 < nsid01);
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

