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


int main()
{
   NavSignalID_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
      // This test takes a long time, so don't run it as part of
      // automated testing.
      //errorTotal += testClass.orderTest();
   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
