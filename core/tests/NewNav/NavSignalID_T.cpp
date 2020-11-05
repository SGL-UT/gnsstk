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
