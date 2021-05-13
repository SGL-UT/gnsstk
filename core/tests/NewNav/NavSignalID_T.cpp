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
#include <list>
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


/// gain access to order method
class TestClass : public gpstk::NavSignalID
{
public:
   TestClass(gpstk::SatelliteSystem sys, gpstk::CarrierBand car,
             gpstk::TrackingCode track, gpstk::NavType nmt)
         : NavSignalID(sys,car,track,nmt)
   {}
   TestClass(gpstk::SatelliteSystem sys, const gpstk::ObsID& oid,
             gpstk::NavType nmt)
         : NavSignalID(sys,oid,nmt)
   {}
   int order(const TestClass& right) const
   {
      return NavSignalID::order(right);
   }
};


class NavSignalID_T
{
public:
   unsigned constructorTest();
   unsigned orderTest();
   unsigned orderTestFast();
};


unsigned NavSignalID_T ::
constructorTest()
{
   TUDEF("NavSignalID", "NavSignalID");
      // test default constructor first
   gpstk::NavSignalID def;
   TUASSERTE(gpstk::SatelliteSystem,gpstk::SatelliteSystem::Unknown,def.system);
   TUASSERTE(gpstk::CarrierBand,gpstk::CarrierBand::Unknown,def.obs.band);
   TUASSERTE(gpstk::TrackingCode,gpstk::TrackingCode::Unknown,def.obs.code);
   TUASSERTE(gpstk::NavType,gpstk::NavType::Unknown,def.nav);
      // test the fill-in-everything constructor
   gpstk::NavSignalID test2(gpstk::SatelliteSystem::GPS,
                            gpstk::CarrierBand::L1,
                            gpstk::TrackingCode::Y,
                            gpstk::NavType::GPSLNAV);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS, test2.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1, test2.obs.band);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::Y, test2.obs.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, test2.nav);
      // test ObsID constructor
   gpstk::ObsID oid(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::L1,
                    gpstk::TrackingCode::Y);
   oid.freqOffs = -7;
   oid.freqOffsWild = false;
   oid.setMcodeBits(0x12345678, 0xffffffff);
   gpstk::NavSignalID uut3(gpstk::SatelliteSystem::GPS, oid,
                           gpstk::NavType::GPSLNAV);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::GPS, uut3.system);
   TUASSERTE(gpstk::CarrierBand, gpstk::CarrierBand::L1, uut3.obs.band);
   TUASSERTE(gpstk::TrackingCode, gpstk::TrackingCode::Y, uut3.obs.code);
   TUASSERTE(gpstk::NavType, gpstk::NavType::GPSLNAV, uut3.nav);
   TUASSERTE(gpstk::ObsID, oid, uut3.obs);
   TURETURN();
}


unsigned NavSignalID_T ::
orderTest()
{
   TUDEF("NavSignalID", "order");
      // Create a bunch of test objects in the order they should be in
      // when sorting, and verify their ordering and the commutative
      // properties of ordering.
   using SS = gpstk::SatelliteSystem;
   using CB = gpstk::CarrierBand;
   using TC = gpstk::TrackingCode;
   using NT = gpstk::NavType;
   std::list<TestClass> uut;
      // yes this will generate a lot of completely absurd, invalid
      // code combinations but that's not the point.
   for (gpstk::SatelliteSystem ss : gpstk::SatelliteSystemIterator())
   {
      for (gpstk::CarrierBand cb : gpstk::CarrierBandIterator())
      {
         if (cb == gpstk::CarrierBand::Any)
            continue; // don't test wildcards
         for (gpstk::TrackingCode tc : gpstk::TrackingCodeIterator())
         {
            if (tc == gpstk::TrackingCode::Any)
               continue; // don't test wildcards
            for (gpstk::NavType nt : gpstk::NavTypeIterator())
            {
               if (nt == gpstk::NavType::Any)
                  continue; // don't test wildcards
               uut.push_back(TestClass(ss,cb,tc,nt));
            }
         }
      }
   }
   bool failed = false;
   for (auto i1 = uut.begin(); i1 != uut.end(); i1++)
   {
      for (auto i2 = std::next(i1); i2 != uut.end(); i2++)
      {
         if (!((*i1) < (*i2)))
         {
            std::ostringstream s;
            s << (*i1) << " < " << (*i2);
            TUFAIL(s.str());
            failed = true;
         }
      }
   }
   if (!failed)
      TUPASS("operator<");
   TURETURN();
}


unsigned NavSignalID_T ::
orderTestFast()
{
   TUDEF("NavSignalID", "order");
      // test GLONASS metadata ordering
   gpstk::ObsID oid1(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::G1,
                     gpstk::TrackingCode::Standard, -3);
   gpstk::ObsID oid2(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::G1,
                     gpstk::TrackingCode::Standard, 7);
   TestClass uut1(gpstk::SatelliteSystem::Glonass, oid1,
                  gpstk::NavType::GloCivilF);
   TestClass uut2(gpstk::SatelliteSystem::Glonass, oid2,
                  gpstk::NavType::GloCivilF);
   TestClass uut3(gpstk::SatelliteSystem::Glonass, oid1,
                  gpstk::NavType::GloCivilF);
   TUASSERTE(int, -1, uut1.order(uut2));
   TUASSERTE(int, 1, uut2.order(uut1));
   TUASSERTE(int, 0, uut1.order(uut3));
      // test mcode metadata ordering
   gpstk::ObsID oid4(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::L1,
                     gpstk::TrackingCode::MDP);
   gpstk::ObsID oid5(gpstk::ObservationType::NavMsg, gpstk::CarrierBand::L1,
                     gpstk::TrackingCode::MDP);
   oid4.setMcodeBits(0x12345678);
   oid5.setMcodeBits(0x87654321);
   TestClass uut4(gpstk::SatelliteSystem::GPS, oid4, gpstk::NavType::GPSMNAV);
   TestClass uut5(gpstk::SatelliteSystem::GPS, oid5, gpstk::NavType::GPSMNAV);
   TestClass uut6(gpstk::SatelliteSystem::GPS, oid4, gpstk::NavType::GPSMNAV);
   TUASSERTE(int, -1, uut4.order(uut5));
   TUASSERTE(int, 1, uut5.order(uut4));
   TUASSERTE(int, 0, uut4.order(uut6));
   TURETURN();
}


int main()
{
   NavSignalID_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
      // This test takes a long time, so don't run it as part of
      // automated testing.
      //errorTotal += testClass.orderTest();
   errorTotal += testClass.orderTestFast();
   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
