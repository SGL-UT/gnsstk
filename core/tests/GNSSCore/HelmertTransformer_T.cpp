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

#include "TestUtil.hpp"
#include "HelmertTransformer.hpp"
#include "YDSTime.hpp"

/// Gain access to protected data.
class TestClass : public gnsstk::HelmertTransformer
{
public:
   using gnsstk::HelmertTransformer::HelmertTransformer;
   double getrx() const { return rotation(2,1); }
   double getry() const { return rotation(0,2); }
   double getrz() const { return rotation(1,0); }
   double gettx() const { return translation(0); }
   double getty() const { return translation(1); }
   double gettz() const { return translation(2); }
   double getscale() const { return scale; }
   std::string getdescription() const { return description; }
};


class HelmertTransformer_T
{
public:
   HelmertTransformer_T();
   unsigned constructorTest();
   unsigned transformPositionTest();
   unsigned transformVectorTest();
   unsigned transformTripleTest();
   unsigned transformXvtTest();
   unsigned transformdoubleTest();

   gnsstk::RefFrame initialRF;
   gnsstk::RefFrame finalRF;
   gnsstk::HelmertTransformer uut1, uut2;
      /// initial positions and transformed positions
   std::vector<double> p1, x1, p2, x2;
};


HelmertTransformer_T ::
HelmertTransformer_T()
      : initialRF(gnsstk::RefFrameSys::WGS84,
                  gnsstk::YDSTime(2020,123,456,gnsstk::TimeSystem::UTC)),
        finalRF(gnsstk::RefFrameSys::CGCS2000,
                gnsstk::YDSTime(1994,1,0,gnsstk::TimeSystem::UTC)),
        uut1(initialRF, finalRF, 0, 0, 0, 10, 10, 10, 1, "hi there",
             gnsstk::YDSTime(2020,123,456,gnsstk::TimeSystem::UTC)),
           // https://d28rz98at9flks.cloudfront.net/71433/71433.pdf
        uut2(initialRF, finalRF, -24.1665*gnsstk::DEG_PER_MAS,
             -20.9515*gnsstk::DEG_PER_MAS, -21.3961*gnsstk::DEG_PER_MAS,
             -42.7e-3, -17.06e-3, 28.81e-3, 11.474*gnsstk::PPB, "step 2",
             gnsstk::YDSTime(1994, 1, 0,gnsstk::TimeSystem::UTC)),
        p1({150, 150, 150}),
        x1({310, 310, 310}),      // 100% scale, 10m translation
        p2({-4052052.3678, 4212836.0411, -2545105.1089}),
        x2({-4052051.7615, 4212836.1945, -2545106.0145})
{
}


unsigned HelmertTransformer_T ::
constructorTest()
{
   TUDEF("HelmertTransformer", "HelmertTransformer");
   TestClass uut1;
   gnsstk::RefFrame exp;
   TUASSERTE(gnsstk::RefFrame, exp, uut1.getFromFrame());
   TUASSERTE(gnsstk::RefFrame, exp, uut1.getToFrame());
   TUASSERTE(int, 1, std::isnan(uut1.getscale()));

   TestClass uut2(gnsstk::RefFrame(gnsstk::RefFrameRlz::PZ90Y2007),
                  gnsstk::RefFrame(gnsstk::RefFrameRlz::WGS84G1150),
                  1e-4, 2e-4, 3e-4, 4, 5, 6, 7, "hi there",
                  gnsstk::YDSTime(2020,123,456,gnsstk::TimeSystem::UTC));
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::PZ90Y2007),
             uut2.getFromFrame());
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::WGS84G1150),
             uut2.getToFrame());
   TUASSERTFE(1e-4*gnsstk::DEG_TO_RAD, uut2.getrx());
   TUASSERTFE(2e-4*gnsstk::DEG_TO_RAD, uut2.getry());
   TUASSERTFE(3e-4*gnsstk::DEG_TO_RAD, uut2.getrz());
   TUASSERTFE(4, uut2.gettx());
   TUASSERTFE(5, uut2.getty());
   TUASSERTFE(6, uut2.gettz());
   TUASSERTFE(7, uut2.getscale());
   TUASSERTE(std::string, "hi there", uut2.getdescription());
   TUASSERTE(gnsstk::CommonTime,
             gnsstk::YDSTime(2020,123,456,gnsstk::TimeSystem::UTC),
             uut2.getEpoch());
   TURETURN();
}


unsigned HelmertTransformer_T ::
transformPositionTest()
{
   TUDEF("HelmertTransformer", "transform(Position)");

   gnsstk::Position pos1(&p1[0], gnsstk::Position::Cartesian, nullptr,
                         initialRF);
   gnsstk::Position exp1(&x1[0], gnsstk::Position::Cartesian, nullptr,
                         finalRF);
   gnsstk::Position out1;
   out1.setReferenceFrame(finalRF);
   TUASSERTE(bool, true, uut1.transform(pos1, out1));
   TUASSERTFE(exp1.getX(), out1.getX());
   TUASSERTFE(exp1.getY(), out1.getY());
   TUASSERTFE(exp1.getZ(), out1.getZ());

   gnsstk::Position pos2(&p2[0], gnsstk::Position::Cartesian, nullptr,
                         initialRF);
   gnsstk::Position exp2(&x2[0], gnsstk::Position::Cartesian, nullptr, finalRF);
   gnsstk::Position out2;
   out2.setReferenceFrame(finalRF);
      // forward transform
   TUASSERTE(bool, true, uut2.transform(pos2, out2));
   TUASSERTFEPS(exp2.getX(), out2.getX(), 1e-4);
   TUASSERTFEPS(exp2.getY(), out2.getY(), 1e-4);
   TUASSERTFEPS(exp2.getZ(), out2.getZ(), 1e-4);
      // reverse transform
   out2.setReferenceFrame(initialRF);
   TUASSERTE(bool, true, uut2.transform(exp2, out2));
   TUASSERTFEPS(pos2.getX(), out2.getX(), 1e-4);
   TUASSERTFEPS(pos2.getY(), out2.getY(), 1e-4);
   TUASSERTFEPS(pos2.getZ(), out2.getZ(), 1e-4);

   TURETURN();
}


unsigned HelmertTransformer_T ::
transformVectorTest()
{
   TUDEF("HelmertTransformer", "transform(Vector)");

   gnsstk::Vector<double> pos1({p1[0], p1[1], p1[2]});
   gnsstk::Vector<double> exp1({x1[0], x1[1], x1[2]});
   gnsstk::Vector<double> out1;
   TUASSERTE(bool, true, uut1.transform(pos1, initialRF, out1));
   TUASSERTFE(exp1[0], out1[0]);
   TUASSERTFE(exp1[1], out1[1]);
   TUASSERTFE(exp1[2], out1[2]);

   gnsstk::Vector<double> pos2({p2[0], p2[1], p2[2]});
   gnsstk::Vector<double> exp2({x2[0], x2[1], x2[2]});
   gnsstk::Vector<double> out2;
      // forward transform
   TUASSERTE(bool, true, uut2.transform(pos2, initialRF, out2));
   TUASSERTFEPS(exp2[0], out2[0], 1e-4);
   TUASSERTFEPS(exp2[1], out2[1], 1e-4);
   TUASSERTFEPS(exp2[2], out2[2], 1e-4);
      // reverse transform
   TUASSERTE(bool, true, uut2.transform(exp2, finalRF, out2));
   TUASSERTFEPS(pos2[0], out2[0], 1e-4);
   TUASSERTFEPS(pos2[1], out2[1], 1e-4);
   TUASSERTFEPS(pos2[2], out2[2], 1e-4);

   TURETURN();
}


unsigned HelmertTransformer_T ::
transformTripleTest()
{
   TUDEF("HelmertTransformer", "transform(Triple)");

   gnsstk::Triple pos1({p1[0], p1[1], p1[2]});
   gnsstk::Triple exp1({x1[0], x1[1], x1[2]});
   gnsstk::Triple out1;
   TUASSERTE(bool, true, uut1.transform(pos1, initialRF, out1));
   TUASSERTFE(exp1[0], out1[0]);
   TUASSERTFE(exp1[1], out1[1]);
   TUASSERTFE(exp1[2], out1[2]);

   gnsstk::Triple pos2({p2[0], p2[1], p2[2]});
   gnsstk::Triple exp2({x2[0], x2[1], x2[2]});
   gnsstk::Triple out2;
      // forward transform
   TUASSERTE(bool, true, uut2.transform(pos2, initialRF, out2));
   TUASSERTFEPS(exp2[0], out2[0], 1e-4);
   TUASSERTFEPS(exp2[1], out2[1], 1e-4);
   TUASSERTFEPS(exp2[2], out2[2], 1e-4);
      // reverse transform
   TUASSERTE(bool, true, uut2.transform(exp2, finalRF, out2));
   TUASSERTFEPS(pos2[0], out2[0], 1e-4);
   TUASSERTFEPS(pos2[1], out2[1], 1e-4);
   TUASSERTFEPS(pos2[2], out2[2], 1e-4);

   TURETURN();
}


unsigned HelmertTransformer_T ::
transformXvtTest()
{
   TUDEF("HelmertTransformer", "transform(Xvt)");

   gnsstk::Xvt pos1, exp1, out1;
   pos1.x = gnsstk::Triple({p1[0], p1[1], p1[2]});
   pos1.v = gnsstk::Triple({1, 2, 3});
   pos1.clkbias = 4;
   pos1.clkdrift = 5;
   pos1.relcorr = 6;
   pos1.frame = initialRF;
   pos1.health = gnsstk::Xvt::Degraded;
   out1.frame = finalRF;
   exp1.x = gnsstk::Triple({x1[0], x1[1], x1[2]});
   exp1.v = gnsstk::Triple({1, 2, 3});
   exp1.clkbias = 4;
   exp1.clkdrift = 5;
   exp1.relcorr = 6;
   exp1.frame = finalRF;
   exp1.health = gnsstk::Xvt::Degraded;
   TUASSERTE(bool, true, uut1.transform(pos1, out1));
   TUASSERTFE(exp1.x[0], out1.x[0]);
   TUASSERTFE(exp1.x[1], out1.x[1]);
   TUASSERTFE(exp1.x[2], out1.x[2]);
   TUASSERTFE(exp1.v[0], out1.v[0]);
   TUASSERTFE(exp1.v[1], out1.v[1]);
   TUASSERTFE(exp1.v[2], out1.v[2]);
   TUASSERTFE(exp1.clkbias, out1.clkbias);
   TUASSERTFE(exp1.clkdrift, out1.clkdrift);
   TUASSERTFE(exp1.relcorr, out1.relcorr);
   TUASSERTE(gnsstk::RefFrame, exp1.frame, out1.frame);
   TUASSERTE(gnsstk::Xvt::HealthStatus, exp1.health, out1.health);

   gnsstk::Xvt pos2, exp2, out2;
   pos2.x = gnsstk::Triple({p2[0], p2[1], p2[2]});
   pos2.v = gnsstk::Triple({1, 2, 3});
   pos2.clkbias = 4;
   pos2.clkdrift = 5;
   pos2.relcorr = 6;
   pos2.frame = initialRF;
   pos2.health = gnsstk::Xvt::Degraded;
   out2.frame = finalRF;
   exp2.x = gnsstk::Triple({x2[0], x2[1], x2[2]});
   exp2.v = gnsstk::Triple({1, 2, 3});
   exp2.clkbias = 4;
   exp2.clkdrift = 5;
   exp2.relcorr = 6;
   exp2.frame = finalRF;
   exp2.health = gnsstk::Xvt::Degraded;
      // forward transform
   TUASSERTE(bool, true, uut2.transform(pos2, out2));
   TUASSERTFEPS(exp2.x[0], out2.x[0], 1e-4);
   TUASSERTFEPS(exp2.x[1], out2.x[1], 1e-4);
   TUASSERTFEPS(exp2.x[2], out2.x[2], 1e-4);
   TUASSERTFEPS(exp2.v[0], out2.v[0], 1e-4);
   TUASSERTFEPS(exp2.v[1], out2.v[1], 1e-4);
   TUASSERTFEPS(exp2.v[2], out2.v[2], 1e-4);
   TUASSERTFE(exp2.clkbias, out2.clkbias);
   TUASSERTFE(exp2.clkdrift, out2.clkdrift);
   TUASSERTFE(exp2.relcorr, out2.relcorr);
   TUASSERTE(gnsstk::RefFrame, exp2.frame, out2.frame);
   TUASSERTE(gnsstk::Xvt::HealthStatus, exp2.health, out2.health);
      // reverse transform
   out2.frame = initialRF;
   TUASSERTE(bool, true, uut2.transform(exp2, out2));
   TUASSERTFEPS(pos2.x[0], out2.x[0], 1e-4);
   TUASSERTFEPS(pos2.x[1], out2.x[1], 1e-4);
   TUASSERTFEPS(pos2.x[2], out2.x[2], 1e-4);
   TUASSERTFEPS(pos2.v[0], out2.v[0], 1e-4);
   TUASSERTFEPS(pos2.v[1], out2.v[1], 1e-4);
   TUASSERTFEPS(pos2.v[2], out2.v[2], 1e-4);
   TUASSERTFE(pos2.clkbias, out2.clkbias);
   TUASSERTFE(pos2.clkdrift, out2.clkdrift);
   TUASSERTFE(pos2.relcorr, out2.relcorr);
   TUASSERTE(gnsstk::RefFrame, pos2.frame, out2.frame);
   TUASSERTE(gnsstk::Xvt::HealthStatus, pos2.health, out2.health);

   TURETURN();
}


unsigned HelmertTransformer_T ::
transformdoubleTest()
{
   TUDEF("HelmertTransformer", "transform(double)");

   double out1x, out1y, out1z;
   TUASSERTE(bool, true, uut1.transform(p1[0], p1[1], p1[2], initialRF, out1x, out1y, out1z));
   TUASSERTFE(x1[0], out1x);
   TUASSERTFE(x1[1], out1y);
   TUASSERTFE(x1[2], out1z);

   double out2x, out2y, out2z;
      // forward transform
   TUASSERTE(bool, true, uut2.transform(p2[0], p2[1], p2[2], initialRF, out2x, out2y, out2z));
   TUASSERTFEPS(x2[0], out2x, 1e-4);
   TUASSERTFEPS(x2[1], out2y, 1e-4);
   TUASSERTFEPS(x2[2], out2z, 1e-4);
      // reverse transform
   TUASSERTE(bool, true, uut2.transform(x2[0], x2[1], x2[2], finalRF, out2x, out2y, out2z));
   TUASSERTFEPS(p2[0], out2x, 1e-4);
   TUASSERTFEPS(p2[1], out2y, 1e-4);
   TUASSERTFEPS(p2[2], out2z, 1e-4);

   TURETURN();
}


int main()
{
   HelmertTransformer_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.transformPositionTest();
   errorTotal += testClass.transformVectorTest();
   errorTotal += testClass.transformTripleTest();
   errorTotal += testClass.transformXvtTest();
   errorTotal += testClass.transformdoubleTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
