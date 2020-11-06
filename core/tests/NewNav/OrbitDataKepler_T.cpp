#include "OrbitDataKepler.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"
#include "CivilTime.hpp"
#include <iomanip>

namespace gpstk
{
   std::ostream& operator<<(std::ostream& s, SVHealth h)
   {
      s << StringUtils::asString(h);
      return s;
   }
}

/// Allow us to test an abstract class
class TestClass : public gpstk::OrbitDataKepler
{
public:
   bool validate() const override
   { return true; }
   gpstk::CommonTime getUserTime() const override
   { return gpstk::CommonTime::BEGINNING_OF_TIME; }
};


class OrbitDataKepler_T
{
public:
   OrbitDataKepler_T();

   unsigned constructorTest();
   unsigned getXvtTest();
   unsigned svRelativityTest();
   unsigned svClockBiasTest();
   unsigned svClockDriftTest();

      /// Set the fields in TestClass/OrbitDataKepler for testing
   void fillTestClass(TestClass& uut);

   gpstk::CivilTime civ;
   gpstk::CommonTime ct;
};


OrbitDataKepler_T ::
OrbitDataKepler_T()
      : civ(2015,7,19,2,0,0.0,gpstk::TimeSystem::GPS),
        ct(civ)
{
}


unsigned OrbitDataKepler_T ::
constructorTest()
{
   TUDEF("OrbitDataKepler", "OrbitDataKepler");
   TestClass uut;
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime::BEGINNING_OF_TIME,
             uut.xmitTime);
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime::BEGINNING_OF_TIME,
             uut.Toe);
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime::BEGINNING_OF_TIME,
             uut.Toc);
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime::BEGINNING_OF_TIME,
             uut.beginFit);
   TUASSERTE(gpstk::CommonTime, gpstk::CommonTime::BEGINNING_OF_TIME,
             uut.endFit);
   TUASSERTE(gpstk::SVHealth, gpstk::SVHealth::Unknown, uut.health);
   TUASSERTFE(0.0, uut.Cuc);
   TUASSERTFE(0.0, uut.Cus);
   TUASSERTFE(0.0, uut.Crc);
   TUASSERTFE(0.0, uut.Crs);
   TUASSERTFE(0.0, uut.Cic);
   TUASSERTFE(0.0, uut.Cis);
   TUASSERTFE(0.0, uut.M0);
   TUASSERTFE(0.0, uut.dn);
   TUASSERTFE(0.0, uut.dndot);
   TUASSERTFE(0.0, uut.ecc);
   TUASSERTFE(0.0, uut.A);
   TUASSERTFE(0.0, uut.Ahalf);
   TUASSERTFE(0.0, uut.Adot);
   TUASSERTFE(0.0, uut.OMEGA0);
   TUASSERTFE(0.0, uut.i0);
   TUASSERTFE(0.0, uut.w);
   TUASSERTFE(0.0, uut.OMEGAdot);
   TUASSERTFE(0.0, uut.idot);
   TUASSERTFE(0.0, uut.af0);
   TUASSERTFE(0.0, uut.af1);
   TUASSERTFE(0.0, uut.af2);
   TURETURN();
}


unsigned OrbitDataKepler_T ::
getXvtTest()
{
   TUDEF("OrbitDataKepler", "getXvt");
   TestClass uut;
   gpstk::Xvt xvt;
   fillTestClass(uut);
   TUASSERT(uut.getXvt(ct+35, xvt));
      // TUASSERTE is not good for this check as we're testing a bunch
      // of floating point numbers, so we use TUSSERTFE instead for
      // each field.
      // @note These were checked against results provided by wheresat
      // while it was still using OrbElemStore.
   TUASSERTFE(  9345531.5274733770639, xvt.x[0]);
   TUASSERTFE(-12408177.088141856715,  xvt.x[1]);
   TUASSERTFE( 21486320.848036296666,  xvt.x[2]);
   TUASSERTFE(2081.276961058104007,    xvt.v[0]);
   TUASSERTFE(1792.4445008638492709,   xvt.v[1]);
   TUASSERTFE( 148.29209115082824155,  xvt.v[2]);
   TUASSERTFE(-0.00021641018042870913346, xvt.clkbias);
   TUASSERTFE(4.3200998334200003381e-12, xvt.clkdrift);
   TUASSERTFE(-8.8197758101551758427e-09, xvt.relcorr);
   TUASSERTE(gpstk::Xvt::HealthStatus, gpstk::Xvt::Healthy, xvt.health);
   TURETURN();
}


unsigned OrbitDataKepler_T ::
svRelativityTest()
{
   TUDEF("OrbitDataKepler", "svRelativity");
   TestClass uut;
   fillTestClass(uut);
   TUASSERTFE(-8.7994080166185110758e-09, uut.svRelativity(ct));
   TUASSERTFE(-8.8197758101551758427e-09, uut.svRelativity(ct+35));
   TURETURN();
}


unsigned OrbitDataKepler_T ::
svClockBiasTest()
{
   TUDEF("OrbitDataKepler", "svClockBias");
   TestClass uut;
   fillTestClass(uut);
   TUASSERTFE(-0.00021641018042870913346, uut.svClockBias(ct+35));
   TURETURN();
}


unsigned OrbitDataKepler_T ::
svClockDriftTest()
{
   TUDEF("OrbitDataKepler", "svClockDrift");
   TestClass uut;
   fillTestClass(uut);
   TUASSERTFE(4.3200998334200003381e-12, uut.svClockDrift(ct+35));
   TURETURN();
}


void OrbitDataKepler_T ::
fillTestClass(TestClass& uut)
{
   uut.xmitTime = gpstk::GPSWeekSecond(1854, .720000000000e+04);
   uut.Toe = gpstk::GPSWeekSecond(1854, .143840000000e+05);
   uut.Toc = gpstk::CivilTime(2015,7,19,3,59,44.0,gpstk::TimeSystem::GPS);
   uut.health = gpstk::SVHealth::Healthy;
   uut.Cuc = .200793147087e-05;
   uut.Cus = .823289155960e-05;
   uut.Crc = .214593750000e+03;
   uut.Crs = .369375000000e+02;
   uut.Cic = -.175088644028e-06;
   uut.Cis = .335276126862e-07;
   uut.M0 = .218771233916e+01;
   uut.dn = .511592738462e-08;
      // dndot
   uut.ecc = .422249664553e-02;
   uut.Ahalf =.515360180473e+04;
   uut.A = uut.Ahalf * uut.Ahalf;
      // Adot
   uut.OMEGA0 = -.189462874179e+01;
   uut.i0 = .946122987969e+00;
   uut.w = .374892043461e+00;
   uut.OMEGAdot = -.823034282681e-08;
   uut.idot = .492877673191e-09;
   uut.af0 = -.216379296035e-03;
   uut.af1 = .432009983342e-11;
   uut.af2 = .000000000000e+00;
      //uut.iode = .190000000000e+02;
      // uut.codes = .100000000000e+01
      //uut.week = .185400000000e+04
      //uut.l2p = .000000000000e+00
      //uut.accuracy = .240000000000e+01
      //uut.tgd = -.107102096081e-07
      //uut.iodc = .190000000000e+02
      //uut.fitint = .400000000000e+01;
}


int main()
{
   OrbitDataKepler_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getXvtTest();
   errorTotal += testClass.svRelativityTest();
   errorTotal += testClass.svClockBiasTest();
   errorTotal += testClass.svClockDriftTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}

