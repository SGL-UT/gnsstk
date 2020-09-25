#include "OrbitDataSP3.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"
#include "CivilTime.hpp"
#include <iomanip>


class OrbitDataSP3_T
{
public:
   OrbitDataSP3_T();

   unsigned constructorTest();
   unsigned getXvtTest();

   gpstk::CivilTime civ;
   gpstk::CommonTime ct;
};


OrbitDataSP3_T ::
OrbitDataSP3_T()
      : civ(2015,7,19,2,0,0.0,gpstk::TimeSystem::GPS),
        ct(civ)
{
}


unsigned OrbitDataSP3_T ::
constructorTest()
{
   TUDEF("OrbitDataSP3", "OrbitDataSP3");
   gpstk::OrbitDataSP3 uut;
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime::BEGINNING_OF_TIME,
             uut.timeStamp);
   TUASSERTFE(0.0, uut.pos[0]);
   TUASSERTFE(0.0, uut.pos[1]);
   TUASSERTFE(0.0, uut.pos[2]);
   TUASSERTFE(0.0, uut.posSig[0]);
   TUASSERTFE(0.0, uut.posSig[1]);
   TUASSERTFE(0.0, uut.posSig[2]);
   TUASSERTFE(0.0, uut.vel[0]);
   TUASSERTFE(0.0, uut.vel[1]);
   TUASSERTFE(0.0, uut.vel[2]);
   TUASSERTFE(0.0, uut.velSig[0]);
   TUASSERTFE(0.0, uut.velSig[1]);
   TUASSERTFE(0.0, uut.velSig[2]);
   TUASSERTFE(0.0, uut.acc[0]);
   TUASSERTFE(0.0, uut.acc[1]);
   TUASSERTFE(0.0, uut.acc[2]);
   TUASSERTFE(0.0, uut.accSig[0]);
   TUASSERTFE(0.0, uut.accSig[1]);
   TUASSERTFE(0.0, uut.accSig[2]);
   TUASSERTFE(0.0, uut.clkBias);
   TUASSERTFE(0.0, uut.biasSig);
   TUASSERTFE(0.0, uut.clkDrift);
   TUASSERTFE(0.0, uut.driftSig);
   TUASSERTFE(0.0, uut.clkDrRate);
   TUASSERTFE(0.0, uut.drRateSig);
   TURETURN();
}


unsigned OrbitDataSP3_T ::
getXvtTest()
{
   TUDEF("OrbitDataSP3", "getXvt");
   TURETURN();
}


int main()
{
   OrbitDataSP3_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getXvtTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}

