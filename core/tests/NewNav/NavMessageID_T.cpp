//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
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

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gnsstk::SatelliteSystem e)
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
   gnsstk::NavMessageID uut1;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Unknown,
             uut1.messageType);
      // constructor 2 test
   gnsstk::SatID satID(7,gnsstk::SatelliteSystem::GPS);
   gnsstk::SatID satID3(3,gnsstk::SatelliteSystem::GPS);
   gnsstk::NavID navID(gnsstk::NavType::GPSLNAV);
   gnsstk::ObsID obsID(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::L1,
                      gnsstk::TrackingCode::CA);
   obsID.freqOffs = -7;
   obsID.freqOffsWild = false;
   obsID.setMcodeBits(0x12345678, 0xffffffff);
   gnsstk::NavSatelliteID sat(3, satID, obsID, navID);
   gnsstk::NavMessageID uut2(sat, gnsstk::NavMessageType::Almanac);
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Almanac,
             uut2.messageType);
   TUASSERTE(gnsstk::SatID, satID3, uut2.sat);
   TUASSERTE(gnsstk::SatID, satID, uut2.xmitSat);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS, uut2.system);
   TUASSERTE(gnsstk::ObsID, obsID, uut2.obs);
   TUASSERTE(gnsstk::NavType, gnsstk::NavType::GPSLNAV, uut2.nav);
   TURETURN();
}


int main()
{
   NavMessageID_T testClass;
   unsigned errorTotal = 0;

      // make sure we can see the details in the event of miscompare
   gnsstk::ObsID::verbose = true;
   errorTotal += testClass.constructorTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
