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
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
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
#include "NavMessageID.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gpstk::SatelliteSystem e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class NavMessageID_T
{
public:
   unsigned constructorTest();
};


unsigned NavMessageID_T ::
constructorTest()
{
   TUDEF("NavMessageID", "NavMessageID");
      // default constructor test
   gpstk::NavMessageID uut1;
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Unknown,
             uut1.messageType);
      // constructor 2 test
   gpstk::SatID satID(7,gpstk::SatelliteSystem::GPS);
   gpstk::SatID satID3(3,gpstk::SatelliteSystem::GPS);
   gpstk::NavID navID(gpstk::NavType::GPSLNAV);
   gpstk::ObsID obsID(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::L1,
                      gpstk::TrackingCode::CA);
   obsID.freqOffs = -7;
   obsID.freqOffsWild = false;
   obsID.mcode = 0x12345678;
   obsID.mcodeMask = 0xffffffff;
   gpstk::NavSatelliteID sat(3, satID, obsID, navID);
   gpstk::NavMessageID uut2(sat, gpstk::NavMessageType::Almanac);
   TUASSERTE(gpstk::NavMessageType, gpstk::NavMessageType::Almanac,
             uut2.messageType);
   TUASSERTE(gpstk::SatID, satID3, uut2.sat);
   TUASSERTE(gpstk::SatID, satID, uut2.xmitSat);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS, uut2.system);
   TUASSERTE(gpstk::ObsID, obsID, uut2.obs);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, uut2.nav);
   TURETURN();
}


int main()
{
   NavMessageID_T testClass;
   unsigned errorTotal = 0;

      // make sure we can see the details in the event of miscompare
   gpstk::ObsID::verbose = true;
   errorTotal += testClass.constructorTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
