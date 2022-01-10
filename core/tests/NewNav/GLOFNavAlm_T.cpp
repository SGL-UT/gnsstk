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
#include <math.h>
#include "TestUtil.hpp"
#include "GLOFNavAlm.hpp"
#include "CivilTime.hpp"
#include "YDSTime.hpp"
#include "TimeString.hpp"
#include "GLOFNavEph.hpp"
#include "Angle.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GLOFNavAlm_T
{
public:
   unsigned constructorTest();
   unsigned validateTest();
   unsigned getXvtTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
   unsigned blahTest();
};


unsigned GLOFNavAlm_T ::
constructorTest()
{
   TUDEF("GLOFNavAlm", "GLOFNavAlm()");
   gnsstk::GLOFNavAlm uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Almanac,
             uut.signal.messageType);
   TUASSERTE(bool, false, uut.healthBits);
   TUASSERTE(int, 1, isnan(uut.tau));
   TUASSERTE(int, 1, isnan(uut.lambda));
   TUASSERTE(int, 1, isnan(uut.deltai));
   TUASSERTE(int, 1, isnan(uut.ecc));
   TUASSERTE(int, 1, isnan(uut.omega));
   TUASSERTE(int, 1, isnan(uut.tLambda));
   TUASSERTE(int, 1, isnan(uut.deltaT));
   TUASSERTE(int, 1, isnan(uut.deltaTdot));
   TUASSERTE(int, -1, uut.freq);
   TURETURN();
}


unsigned GLOFNavAlm_T ::
validateTest()
{
   TUDEF("GLOFNavAlm", "validate()");
   gnsstk::GLOFNavAlm uut;
      /// @todo Implement some testing when the function has some meat to it.
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}


unsigned GLOFNavAlm_T ::
getXvtTest()
{
   TUDEF("GLOFNavAlm", "getXvt()");
   gnsstk::GLOFNavAlm uut;
      // 2001/09/06
   gnsstk::YDSTime toi(2001, 249, 33300, gnsstk::TimeSystem::GLO);
      // taken from ICD appendix A.3.2.3
   uut.lambda = -0.189986229 * gnsstk::PI;
   uut.tLambda = 27122.09375;
   uut.deltai = 0.011929512 * gnsstk::PI;
   uut.deltaT = -2655.76171875;
   uut.deltaTdot = 0.000549316;
   uut.ecc = 0.001482010;
   uut.omega = 0.440277100 * gnsstk::PI;
   uut.Toa = gnsstk::YDSTime(2001, 249, uut.tLambda, gnsstk::TimeSystem::GLO);
      // Normally called by fixFit, but we don't care about the fit
      // interval for this test.
   uut.setSemiMajorAxisIncl();
   gnsstk::Xvt xvt;
   TUASSERTE(bool, true, uut.getXvt(toi, xvt));
   std::cout << "xvt = " << std::setprecision(17) << xvt << std::endl;
   gnsstk::CommonTime x(gnsstk::YDSTime(2001,249,0,gnsstk::TimeSystem::GLO));
   unsigned i = 0;
   double st = 0;
   while (st <= 6.02401539573)
   {
      gnsstk::CommonTime y = x+(double)i;
      st = (gnsstk::GLOFNavEph::getSiderealTime(y)*gnsstk::PI/12.0);
      std::cout << gnsstk::printTime(y, "%Y %j %s %m %d = ")
                << std::setprecision(15) << st << std::endl;
      i += 86400;
   }
   TURETURN();
}


unsigned GLOFNavAlm_T ::
getUserTimeTest()
{
   TUDEF("GLOFNavAlm", "getUserTime()");
   gnsstk::GLOFNavAlm uut;
   uut.timeStamp = gnsstk::CivilTime(2006, 10, 01, 0, 15, 0,
                                     gnsstk::TimeSystem::GLO);
   uut.xmit2 = gnsstk::CivilTime(2006, 10, 01, 0, 11, 0,
                                     gnsstk::TimeSystem::GLO);
   gnsstk::CommonTime exp(uut.timeStamp + 2.0);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GLOFNavAlm_T ::
fixFitTest()
{
   TUDEF("GLOFNavAlm", "fixFit()");
   gnsstk::GLOFNavAlm uut;
   uut.timeStamp = gnsstk::CivilTime(2006, 10, 01, 0, 15, 0,
                                     gnsstk::TimeSystem::GLO);
   gnsstk::CommonTime expBegin(uut.timeStamp),
      expEnd(gnsstk::CommonTime::END_OF_TIME);
   expEnd.setTimeSystem(gnsstk::TimeSystem::GLO);
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, expBegin, uut.beginFit);
      // This is not a good value but it's the only one we have for now
   TUASSERTE(gnsstk::CommonTime, expEnd, uut.endFit);
   TURETURN();
}


unsigned GLOFNavAlm_T ::
blahTest()
{
   TUDEF("GLOFNavAlm", "nothing");
   using namespace gnsstk;
   using namespace std;
      // system/model constants
   const double omega3 = 0.7292115e-4;
   const double mu = 398600.4418; // km**3/sec**2
   const double C20 = -1082.62575e-6;
   const double ae = 6378.136; // km
   const Angle icp(63.0, AngleType::Deg);
   const double Tcp = 43200; // seconds
      // elemnents from almanac
   const unsigned aNAj = 615;                // Date 06 09 2001
   const Angle alambdaj(-0.189986229, AngleType::SemiCircle);
   const double atlambdaj = 27122.09375;     // seconds
   const Angle aDeltaij(0.011929512, AngleType::SemiCircle);
   const double aDeltaTj = -2655.76171875;   // seconds
   const double aDeltaTPrimej = 0.000549316; // seconds/cycle**2
   const double aepsilonj = 0.001482010;     // unit
   const Angle aomegaj(0.440277100, AngleType::SemiCircle);
      // time of interest
   const unsigned cNAj = 615;                // Date 06 09 2001
   const double ctlambdaj = 33300.;          // seconds
   const Angle cS0(6.02401539573, AngleType::Rad);
      // expected values
   const double eXoi = 10947.021572;
   const double eYoi = 13078.978287;
   const double eZoi = 18922.063362;
   const double eVxoi = -3.375497;
   const double eVyoi = -0.161453;
   const double eVzoi = 2.060844;

      // dummies, aka my best guess at what values are supposed to be
      // plugged in since the ICD isn't 100% clear.
   unsigned N0 = aNAj;
   Angle lambda = alambdaj;
   double tlambda = atlambdaj;
   Angle Deltai = aDeltaij;
   double DeltaT = aDeltaTj;
   double DeltaTPrime = aDeltaTPrimej;
   double e = aepsilonj;
   Angle omega = aomegaj;
   Angle S0 = cS0;
   double ti = ctlambdaj;
   double NA = cNAj;
   

      // page 67 (russian)
   double Tdeltap = Tcp + DeltaT;
   Angle i = icp + Deltai;
      // a* are in km
   double a0 = ::pow((Tdeltap/(2*PI))*(Tdeltap/(2*PI))*mu, 1.0/3.0);
   cerr << "a(0) = " << a0 << endl;
   double an = a0;
   double anp1 = -9999999999999;
   Angle nu = -omega;
   unsigned n = 0;
   while (fabs(anp1-an) >= 1e-3)
   {
      double pn = an * (1-e*e);
      cerr << "p(" << n << ") = " << pn << endl;
      double Tocknp1 = Tdeltap /
         (1+(3/2)*C20*(ae/pn)*(ae/pn)*
          ((2-(5/2)*sin(i)*sin(i)) *
           (::pow(1-e*e,3.0/2.0) / ((1+e*cos(omega))*(1+e*cos(omega)))) +
           (((1+e*cos(nu))*(1+e*cos(nu))*(1+e*cos(nu))) / (1-e*e))));
      cerr << "Tock(" << (n+1) << ") = " << Tocknp1 << endl;
      if (n > 0)
         an = anp1;
      anp1 = ::pow((Tocknp1/(2*PI))*(Tocknp1/(2*PI))*mu, 1.0/3.0);
      cerr << "a(" << n << ") = " << an << endl
           << "a(" << (n+1) << ") = " << anp1 << endl;
      n++;
   }
   cerr << "a = " << anp1 << " km" << endl;
//   double Vzoi = Vri * sin(ui) * sin(ii) + Vui * sin(ui) * sin(ii);
   GLOFNavAlm::NumberCruncher nc;
   TURETURN();
}


int main()
{
   GLOFNavAlm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.validateTest();
   errorTotal += testClass.getXvtTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.blahTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
