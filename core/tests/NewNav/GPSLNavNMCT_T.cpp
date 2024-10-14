//==============================================================================
//
//  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
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
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
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
#include <cmath>

#include "CommonTime.hpp"
#include "GPSLNavNMCT.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"
#include "GPSNMCTAI.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::GPSNMCTAI e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GPSLNavNMCT_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getUserTimeTest();
   unsigned getERDTest();
   unsigned validateTest();
   unsigned updateTNMCTTest();
   unsigned cloneTest();
   unsigned isSameDataTest();
};


unsigned GPSLNavNMCT_T ::
constructorTest()
{
   TUDEF("GPSLNavNMC", "GPSLNavNMC");
   gnsstk::GPSLNavNMCT obj;
   TUASSERTE(long, -1, obj.aodo);
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::END_OF_TIME, obj.Toe);
   TUASSERTE(gnsstk::GPSNMCTAI, gnsstk::GPSNMCTAI::NotAvailable, obj.availabilityIndicator);
   TUASSERTE(bool, true, obj.erds.empty());
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::END_OF_TIME, obj.Tnmct);
   TURETURN();
}


unsigned GPSLNavNMCT_T ::
getUserTimeTest()
{
   TUDEF("GPSLNavNMC", "getUserTime");
   gnsstk::GPSLNavNMCT obj;
   obj.timeStamp = gnsstk::GPSWeekSecond(2100,253.0);
   gnsstk::CommonTime exp(gnsstk::GPSWeekSecond(2100,259.0));
   TUASSERTE(gnsstk::CommonTime, exp, obj.getUserTime());
   TURETURN();
}

unsigned GPSLNavNMCT_T ::
getERDTest()
{
   TUDEF("GPSLNavNMC", "getERD");
   gnsstk::GPSLNavNMCT obj;
   obj.signal.xmitSat = gnsstk::SatID(5, gnsstk::SatelliteSystem::GPS);
   obj.erds[4] = 040;
   obj.erds[6] = 077;

   double erd = 0.0;
   bool success = obj.getERD(5, erd);
   TUASSERTE(bool, false, success);

      // The ERD value should be identified as the invalid bits value
   success = obj.getERD(4, erd);
   TUASSERTE(bool, false, success);

   success = obj.getERD(6, erd);
   TUASSERTE(bool, true, success);
   TUASSERTFEPS(-9.3, erd, 0.1);
   TURETURN();
}


unsigned GPSLNavNMCT_T ::
validateTest()
{
   TUDEF("GPSLNavNMC", "validate");
   gnsstk::GPSLNavNMCT obj;
   obj.signal.xmitSat = gnsstk::SatID(31, gnsstk::SatelliteSystem::GPS);
   obj.aodo = 1;
   for (unsigned i = 1; i < 31; ++i)
   {
      obj.erds[i] = 4;
   }
   TUASSERTE(bool, true, obj.validate());
   
   obj.aodo = -1;
   TUASSERTE(bool, false, obj.validate());
   obj.aodo = 30000;
   TUASSERTE(bool, false, obj.validate());
   // reset aodo to valid and verify
   obj.aodo = 1;
   TUASSERTE(bool, true, obj.validate());

   obj.erds[500] = 4;
   TUASSERTE(bool, false, obj.validate());
   obj.erds.erase(500);
   TUASSERTE(bool, true, obj.validate());

   obj.erds[31] = 4;
   TUASSERTE(bool, false, obj.validate());
   obj.erds.erase(31);
   TUASSERTE(bool, true, obj.validate());

   obj.erds.erase(30);
   obj.erds[32] = 4;
   TUASSERTE(bool, false, obj.validate());
   obj.erds.erase(32);
   obj.erds[30] = 4;
   TUASSERTE(bool, true, obj.validate());

   obj.erds.erase(1);
   obj.erds.erase(2);
   obj.erds[31] = 4;
   obj.erds[32] = 4;
   TUASSERTE(bool, false, obj.validate());

   obj.erds.clear();
   TUASSERTE(bool, false, obj.validate())

   TURETURN();
}


unsigned GPSLNavNMCT_T ::
updateTNMCTTest()
{
   TUDEF("GPSLNavNMC", "updateTNMCT");
   gnsstk::GPSLNavNMCT uut;
   uut.Toe = gnsstk::GPSWeekSecond(2100,253.0);
   uut.aodo = 200;
   uut.updateTNMCT();
   gnsstk::CommonTime exp(gnsstk::GPSWeekSecond(2100, 7000));
   TUASSERTE(gnsstk::CommonTime, exp, uut.Tnmct);

   uut.Toe = gnsstk::GPSWeekSecond(2100,7200);
   uut.aodo = 200;
   uut.updateTNMCT();
   gnsstk::CommonTime exp2(gnsstk::GPSWeekSecond(2100, 7000));
   TUASSERTE(gnsstk::CommonTime, exp2, uut.Tnmct);
   
   TURETURN();
}

unsigned GPSLNavNMCT_T ::
cloneTest()
{
   TUDEF("GPSLNavNMC", "updateTNMCT");
   gnsstk::GPSLNavNMCT uut;
   uut.aodo = 300;

   gnsstk::NavDataPtr clone = uut.clone();
   std::shared_ptr<gnsstk::GPSLNavNMCT> clone2 = std::dynamic_pointer_cast<gnsstk::GPSLNavNMCT>(clone);
   TUASSERTE(bool, true, clone2 != nullptr);
   TUASSERTE(long, 300, clone2->aodo);
   clone2->aodo = 66;
   TUASSERTE(long, 66, clone2->aodo);
   TUASSERTE(long, 300, uut.aodo);
   
   TURETURN();
}

unsigned GPSLNavNMCT_T ::
isSameDataTest()
{
   TUDEF("GPSLNavNMC", "isSameData");
   gnsstk::GPSLNavNMCT uut;
   auto uut2 = std::make_shared<gnsstk::GPSLNavNMCT>();

   TUASSERTE(bool, true, uut.isSameData(uut2, true));
   uut.Tnmct = gnsstk::GPSWeekSecond(2100,253.0);
   TUASSERTE(bool, false, uut.isSameData(uut2, true));
   
   TURETURN();
}


int main()
{
   GPSLNavNMCT_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.validateTest();
   errorTotal += testClass.updateTNMCTTest();
   errorTotal += testClass.cloneTest();
   errorTotal += testClass.isSameDataTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
