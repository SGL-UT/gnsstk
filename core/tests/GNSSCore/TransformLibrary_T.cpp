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
#include "TransformLibrary.hpp"
#include "HelmertTransformer.hpp"
#include "YDSTime.hpp"
#include "TimeString.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, const Transformer& t)
   {
      s << "Transformer " << StringUtils::asString(t.getFromFrame()) << " -> "
        << StringUtils::asString(t.getToFrame()) << " : "
        << printTime(t.getEpoch(), "%Y/%03j/%05s") << std::endl;
      return s;
   }
}


/// Something to put into a TransformLibrary without any overhead.
class FakeTransformer : public gnsstk::Transformer
{
public:
   FakeTransformer(gnsstk::RefFrameRlz fromRlz, gnsstk::RefFrameRlz toRlz)
   {
      fromFrame = gnsstk::RefFrame(fromRlz);
      toFrame = gnsstk::RefFrame(toRlz);
   }
   FakeTransformer(gnsstk::RefFrameRlz fromRlz, gnsstk::RefFrameRlz toRlz,
                   const gnsstk::CommonTime& start)
   {
      fromFrame = gnsstk::RefFrame(fromRlz);
      toFrame = gnsstk::RefFrame(toRlz);
      epoch = start;
   }
   bool transform(const gnsstk::Position& fromPos, gnsstk::Position& toPos)
      const noexcept override
   { return false; }
   bool transform(const gnsstk::Xvt& fromPos, gnsstk::Xvt& toPos)
      const noexcept override
   { return false; }
   bool transform(const gnsstk::Vector<double>& fromPos,
                  const gnsstk::RefFrame& srcFrame,
                  gnsstk::Vector<double>& toPos)
      const noexcept override
   { return false; }
   bool transform(const gnsstk::Triple& fromPos,
                  const gnsstk::RefFrame& srcFrame,
                  gnsstk::Triple& toPos)
      const noexcept override
   { return false; }
   bool transform(double fx, double fy, double fz,
                  const gnsstk::RefFrame& srcFrame,
                  double& tx, double& ty, double& tz)
      const noexcept override
   { return false; }
};

class TransformLibrary_T
{
public:
   TransformLibrary_T();
   unsigned constructorTest();
   unsigned getTransformTest();
   unsigned transformPositionTest();
   unsigned transformVectorTest();
   unsigned transformTripleTest();
   unsigned transformXvtTest();
   unsigned transformdoubleTest();

   gnsstk::YDSTime refTime1;
   gnsstk::RefFrame initialRF;
   gnsstk::RefFrame finalRF;
   std::shared_ptr<gnsstk::HelmertTransformer> xf1;
      /// initial positions and transformed positions
   std::vector<double> p1, x1;
};


TransformLibrary_T ::
TransformLibrary_T()
      : refTime1(2020,123,456,gnsstk::TimeSystem::UTC),
        initialRF(gnsstk::RefFrameSys::WGS84, refTime1),
        finalRF(gnsstk::RefFrameSys::CGCS2000, refTime1),
        p1({150, 150, 150}),
        x1({310, 310, 310}),      // 100% scale, 10m translation
        xf1(std::make_shared<gnsstk::HelmertTransformer>(
               initialRF, finalRF, 0, 0, 0, 10, 10, 10, 1, "hi there",
               refTime1))
{
}


unsigned TransformLibrary_T ::
constructorTest()
{
   TUDEF("TransformLibrary", "TransformLibrary");
   gnsstk::TransformLibrary uut;
   TUASSERTE(bool, false, uut.transformers.empty());
   TURETURN();
}


unsigned TransformLibrary_T ::
getTransformTest()
{
   TUDEF("TransformLibrary", "getTransform");
   gnsstk::TransformLibrary uut;
   gnsstk::TransformerPtr optimus;
   gnsstk::TimeSystem ts(gnsstk::TimeSystem::UTC);
   uut.transformers.clear();
   TUCATCH(uut.addTransform(std::make_shared<FakeTransformer>(
                               gnsstk::RefFrameRlz::WGS84G1150,
                               gnsstk::RefFrameRlz::PZ90KGS)));
   TUCATCH(uut.addTransform(std::make_shared<FakeTransformer>(
                               gnsstk::RefFrameRlz::WGS84G1150,
                               gnsstk::RefFrameRlz::ITRF2000)));
   TUCATCH(uut.addTransform(std::make_shared<FakeTransformer>(
                               gnsstk::RefFrameRlz::ITRF2020,
                               gnsstk::RefFrameRlz::PZ90Y2007,
                               gnsstk::YDSTime(2020,5,0,ts))));
   TUCATCH(uut.addTransform(std::make_shared<FakeTransformer>(
                               gnsstk::RefFrameRlz::ITRF2020,
                               gnsstk::RefFrameRlz::PZ90Y2007,
                               gnsstk::YDSTime(2020,15,0,ts))));
   TUCATCH(uut.addTransform(std::make_shared<FakeTransformer>(
                               gnsstk::RefFrameRlz::ITRF2020,
                               gnsstk::RefFrameRlz::PZ90Y2007,
                               gnsstk::YDSTime(2020,95,0,ts))));
      // forward transformation search test
   TUASSERTE(bool, true,
             uut.getTransform(gnsstk::RefFrameRlz::WGS84G1150,
                              gnsstk::RefFrameRlz::ITRF2000,
                              optimus));
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::WGS84G1150),
             optimus->getFromFrame());
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF2000),
             optimus->getToFrame());
      // transformation unavailable test
   TUASSERTE(bool, false,
             uut.getTransform(gnsstk::RefFrameRlz::WGS84G1150,
                              gnsstk::RefFrameRlz::ITRF2020,
                              optimus));
      // reverse transformation search test
   TUASSERTE(bool, true,
             uut.getTransform(gnsstk::RefFrameRlz::ITRF2000,
                              gnsstk::RefFrameRlz::WGS84G1150,
                              optimus));
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::WGS84G1150),
             optimus->getFromFrame());
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF2000),
             optimus->getToFrame());
      // time history search test (too early)
   TUASSERTE(bool, false,
             uut.getTransform(gnsstk::RefFrameRlz::ITRF2020,
                              gnsstk::RefFrameRlz::PZ90Y2007,
                              optimus,
                              gnsstk::YDSTime(1,1,0,ts)));
      // time history search test (spot on)
   TUASSERTE(bool, true,
             uut.getTransform(gnsstk::RefFrameRlz::ITRF2020,
                              gnsstk::RefFrameRlz::PZ90Y2007,
                              optimus,
                              gnsstk::YDSTime(2020,5,0,ts)));
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF2020),
             optimus->getFromFrame());
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::PZ90Y2007),
             optimus->getToFrame());
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2020,5,0,ts),
             optimus->getEpoch());
      // time history search test (in between)
   TUASSERTE(bool, true,
             uut.getTransform(gnsstk::RefFrameRlz::ITRF2020,
                              gnsstk::RefFrameRlz::PZ90Y2007,
                              optimus,
                              gnsstk::YDSTime(2020,7,0,ts)));
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF2020),
             optimus->getFromFrame());
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::PZ90Y2007),
             optimus->getToFrame());
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2020,5,0,ts),
             optimus->getEpoch());
      // time history search test (late)
   TUASSERTE(bool, true,
             uut.getTransform(gnsstk::RefFrameRlz::ITRF2020,
                              gnsstk::RefFrameRlz::PZ90Y2007,
                              optimus,
                              gnsstk::YDSTime(2029,1,0,ts)));
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::ITRF2020),
             optimus->getFromFrame());
   TUASSERTE(gnsstk::RefFrame,
             gnsstk::RefFrame(gnsstk::RefFrameRlz::PZ90Y2007),
             optimus->getToFrame());
   TUASSERTE(gnsstk::CommonTime, gnsstk::YDSTime(2020,95,0,ts),
             optimus->getEpoch());
   TURETURN();
}


unsigned TransformLibrary_T ::
transformPositionTest()
{
   TUDEF("TransformLibrary", "transform(Position)");
   gnsstk::TransformLibrary uut;
   uut.transformers.clear();
   TUCATCH(uut.addTransform(xf1));
   gnsstk::Position pos1(&p1[0], gnsstk::Position::Cartesian, nullptr,
                         initialRF);
   gnsstk::Position exp1(&x1[0], gnsstk::Position::Cartesian, nullptr,
                         finalRF);
   gnsstk::Position out1;
   out1.setReferenceFrame(finalRF);
   TUASSERTE(bool, true, uut.transform(pos1, out1, refTime1));
   TUASSERTFE(exp1.getX(), out1.getX());
   TUASSERTFE(exp1.getY(), out1.getY());
   TUASSERTFE(exp1.getZ(), out1.getZ());

   TURETURN();
}

unsigned TransformLibrary_T ::
transformVectorTest()
{
   TUDEF("TransformLibrary", "transform(Vector)");
   gnsstk::TransformLibrary uut;
   uut.transformers.clear();
   TUCATCH(uut.addTransform(xf1));
   gnsstk::Vector<double> pos1({p1[0], p1[1], p1[2]});
   gnsstk::Vector<double> exp1({x1[0], x1[1], x1[2]});
   gnsstk::Vector<double> out1;
   TUASSERTE(bool, true, uut.transform(pos1,initialRF,out1,finalRF,refTime1));
   TUASSERTFE(exp1[0], out1[0]);
   TUASSERTFE(exp1[1], out1[1]);
   TUASSERTFE(exp1[2], out1[2]);
   TURETURN();
}


unsigned TransformLibrary_T ::
transformTripleTest()
{
   TUDEF("TransformLibrary", "transform(Triple)");
   gnsstk::TransformLibrary uut;
   uut.transformers.clear();
   TUCATCH(uut.addTransform(xf1));
   gnsstk::Triple pos1({p1[0], p1[1], p1[2]});
   gnsstk::Triple exp1({x1[0], x1[1], x1[2]});
   gnsstk::Triple out1;
   TUASSERTE(bool, true, uut.transform(pos1,initialRF,out1,finalRF,refTime1));
   TUASSERTFE(exp1[0], out1[0]);
   TUASSERTFE(exp1[1], out1[1]);
   TUASSERTFE(exp1[2], out1[2]);
   TURETURN();
}


unsigned TransformLibrary_T ::
transformXvtTest()
{
   TUDEF("TransformLibrary", "transform(Xvt)");
   gnsstk::TransformLibrary uut;
   uut.transformers.clear();
   TUCATCH(uut.addTransform(xf1));
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
   TUASSERTE(bool, true, uut.transform(pos1, out1));
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
   TURETURN();
}


unsigned TransformLibrary_T ::
transformdoubleTest()
{
   TUDEF("TransformLibrary", "transform(double)");
   gnsstk::TransformLibrary uut;
   uut.transformers.clear();
   TUCATCH(uut.addTransform(xf1));
   double out1x, out1y, out1z;
   TUASSERTE(bool, true, uut.transform(p1[0], p1[1], p1[2], initialRF,
                                       out1x, out1y, out1z, finalRF, refTime1));
   TUASSERTFE(x1[0], out1x);
   TUASSERTFE(x1[1], out1y);
   TUASSERTFE(x1[2], out1z);
   TURETURN();
}


int main()
{
   TransformLibrary_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getTransformTest();
   errorTotal += testClass.transformPositionTest();
   errorTotal += testClass.transformVectorTest();
   errorTotal += testClass.transformTripleTest();
   errorTotal += testClass.transformXvtTest();
   errorTotal += testClass.transformdoubleTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}

