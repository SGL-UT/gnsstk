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
#include <list>
#include "NavSignalID.hpp"
#include "TestUtil.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::SatelliteSystem e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gnsstk::CarrierBand e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gnsstk::TrackingCode e)
   {
      s << StringUtils::asString(e);
      return s;
   }

   std::ostream& operator<<(std::ostream& s, gnsstk::NavType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


/// gain access to order method
class TestClass : public gnsstk::NavSignalID
{
public:
   TestClass(gnsstk::SatelliteSystem sys, gnsstk::CarrierBand car,
             gnsstk::TrackingCode track, gnsstk::NavType nmt)
         : NavSignalID(sys,car,track,nmt)
   {}
   TestClass(gnsstk::SatelliteSystem sys, const gnsstk::ObsID& oid,
             gnsstk::NavType nmt)
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
   gnsstk::NavSignalID def;
   TUASSERTE(gnsstk::SatelliteSystem,gnsstk::SatelliteSystem::Unknown,def.system);
   TUASSERTE(gnsstk::CarrierBand,gnsstk::CarrierBand::Unknown,def.obs.band);
   TUASSERTE(gnsstk::TrackingCode,gnsstk::TrackingCode::Unknown,def.obs.code);
   TUASSERTE(gnsstk::NavType,gnsstk::NavType::Unknown,def.nav);
      // test the fill-in-everything constructor
   gnsstk::NavSignalID test2(gnsstk::SatelliteSystem::GPS,
                            gnsstk::CarrierBand::L1,
                            gnsstk::TrackingCode::Y,
                            gnsstk::NavType::GPSLNAV);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS, test2.system);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, test2.obs.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::Y, test2.obs.code);
   TUASSERTE(gnsstk::NavType, gnsstk::NavType::GPSLNAV, test2.nav);
      // test ObsID constructor
   gnsstk::ObsID oid(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::L1,
                    gnsstk::TrackingCode::Y);
   oid.freqOffs = -7;
   oid.freqOffsWild = false;
   oid.setMcodeBits(0x12345678, 0xffffffff);
   gnsstk::NavSignalID uut3(gnsstk::SatelliteSystem::GPS, oid,
                           gnsstk::NavType::GPSLNAV);
   TUASSERTE(gnsstk::SatelliteSystem, gnsstk::SatelliteSystem::GPS, uut3.system);
   TUASSERTE(gnsstk::CarrierBand, gnsstk::CarrierBand::L1, uut3.obs.band);
   TUASSERTE(gnsstk::TrackingCode, gnsstk::TrackingCode::Y, uut3.obs.code);
   TUASSERTE(gnsstk::NavType, gnsstk::NavType::GPSLNAV, uut3.nav);
   TUASSERTE(gnsstk::ObsID, oid, uut3.obs);
   TURETURN();
}


unsigned NavSignalID_T ::
orderTest()
{
   TUDEF("NavSignalID", "order");
      // Create a bunch of test objects in the order they should be in
      // when sorting, and verify their ordering and the commutative
      // properties of ordering.
   using SS = gnsstk::SatelliteSystem;
   using CB = gnsstk::CarrierBand;
   using TC = gnsstk::TrackingCode;
   using NT = gnsstk::NavType;
   std::list<TestClass> uut;
      // yes this will generate a lot of completely absurd, invalid
      // code combinations but that's not the point.
   for (gnsstk::SatelliteSystem ss : gnsstk::SatelliteSystemIterator())
   {
      for (gnsstk::CarrierBand cb : gnsstk::CarrierBandIterator())
      {
         if (cb == gnsstk::CarrierBand::Any)
            continue; // don't test wildcards
         for (gnsstk::TrackingCode tc : gnsstk::TrackingCodeIterator())
         {
            if (tc == gnsstk::TrackingCode::Any)
               continue; // don't test wildcards
            for (gnsstk::NavType nt : gnsstk::NavTypeIterator())
            {
               if (nt == gnsstk::NavType::Any)
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
   gnsstk::ObsID oid1(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::G1,
                     gnsstk::TrackingCode::Standard, -3);
   gnsstk::ObsID oid2(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::G1,
                     gnsstk::TrackingCode::Standard, 7);
   TestClass uut1(gnsstk::SatelliteSystem::Glonass, oid1,
                  gnsstk::NavType::GloCivilF);
   TestClass uut2(gnsstk::SatelliteSystem::Glonass, oid2,
                  gnsstk::NavType::GloCivilF);
   TestClass uut3(gnsstk::SatelliteSystem::Glonass, oid1,
                  gnsstk::NavType::GloCivilF);
   TUASSERTE(int, -1, uut1.order(uut2));
   TUASSERTE(int, 1, uut2.order(uut1));
   TUASSERTE(int, 0, uut1.order(uut3));
      // test mcode metadata ordering
   gnsstk::ObsID oid4(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::L1,
                     gnsstk::TrackingCode::MDP);
   gnsstk::ObsID oid5(gnsstk::ObservationType::NavMsg, gnsstk::CarrierBand::L1,
                     gnsstk::TrackingCode::MDP);
   oid4.setMcodeBits(0x12345678);
   oid5.setMcodeBits(0x87654321);
   TestClass uut4(gnsstk::SatelliteSystem::GPS, oid4, gnsstk::NavType::GPSMNAV);
   TestClass uut5(gnsstk::SatelliteSystem::GPS, oid5, gnsstk::NavType::GPSMNAV);
   TestClass uut6(gnsstk::SatelliteSystem::GPS, oid4, gnsstk::NavType::GPSMNAV);
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
