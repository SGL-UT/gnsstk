#include "WildSatID.hpp"
#include "TestUtil.hpp"

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, gpstk::SatelliteSystem e)
   {
      s << gpstk::StringUtils::asString(e);
      return s;
   }
}


class WildSatID_T
{
public:
   unsigned constructorTest();
   unsigned equalTest();
};


unsigned WildSatID_T ::
constructorTest()
{
   TUDEF("WildSatID", "WildSatID");
   gpstk::WildSatID ws1;
   TUASSERTE(bool, true, ws1.wildSat);
   TUASSERTE(bool, true, ws1.wildSys);
   gpstk::WildSatID ws2(99);
   TUASSERTE(int, 99, ws2.id);
   TUASSERTE(bool, false, ws2.wildSat);
   TUASSERTE(bool, true, ws2.wildSys);
   gpstk::WildSatID ws3(gpstk::SatelliteSystem::Mixed);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::Mixed, ws3.system);
   TUASSERTE(bool, true, ws3.wildSat);
   TUASSERTE(bool, false, ws3.wildSys);
   gpstk::WildSatID ws4(-65, gpstk::SatelliteSystem::LEO);
   TUASSERTE(int, -65, ws4.id);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::LEO, ws4.system);
   TUASSERTE(bool, false, ws4.wildSat);
   TUASSERTE(bool, false, ws4.wildSys);
   gpstk::SatID foo(25, gpstk::SatelliteSystem::QZSS);
   gpstk::WildSatID ws5(foo);
   TUASSERTE(int, 25, ws5.id);
   TUASSERTE(gpstk::SatelliteSystem, gpstk::SatelliteSystem::QZSS, ws5.system);
   TUASSERTE(bool, false, ws5.wildSat);
   TUASSERTE(bool, false, ws5.wildSys);
   TURETURN();
}


unsigned WildSatID_T ::
equalTest()
{
   TUDEF("WildSatID", "operator==");
   TURETURN();
}


int main()
{
   WildSatID_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.equalTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
