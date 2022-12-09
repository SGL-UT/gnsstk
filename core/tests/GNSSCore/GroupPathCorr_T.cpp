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
#include "BCISCorrector.hpp"
#include "BCIonoCorrector.hpp"
#include "TropCorrector.hpp"
#include "GroupPathCorr.hpp"
#include "NavLibrary.hpp"
#include "MultiFormatNavDataFactory.hpp"
#include "CivilTime.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, CorrectorType t)
   {
      s << StringUtils::asString(t);
      return s;
   }
}


class TestISCorrector1 : public gnsstk::GroupPathCorrector
{
public:
   TestISCorrector1()
   { corrType = gnsstk::CorrectorType::ISC; }
   bool getCorr(const gnsstk::Position& rxPos, const gnsstk::Position& svPos,
                const gnsstk::SatID& sat, const gnsstk::ObsID& obs,
                const gnsstk::CommonTime& when, gnsstk::NavType nav,
                double& corrOut) override
   {
      corrOut = 2.0;
      return true;
   }
   bool getCorr(const gnsstk::Position& rxPos, const gnsstk::Xvt& svPos,
                const gnsstk::SatID& sat, const gnsstk::ObsID& obs,
                const gnsstk::CommonTime& when, gnsstk::NavType nav,
                double& corrOut) override
   {
      corrOut = 2.0;
      return true;
   }
};


class TestISCorrector2 : public gnsstk::GroupPathCorrector
{
public:
   TestISCorrector2()
   { corrType = gnsstk::CorrectorType::ISC; }
   bool getCorr(const gnsstk::Position& rxPos, const gnsstk::Position& svPos,
                const gnsstk::SatID& sat, const gnsstk::ObsID& obs,
                const gnsstk::CommonTime& when, gnsstk::NavType nav,
                double& corrOut) override
   {
      corrOut = 4.0;
      return true;
   }
   bool getCorr(const gnsstk::Position& rxPos, const gnsstk::Xvt& svPos,
                const gnsstk::SatID& sat, const gnsstk::ObsID& obs,
                const gnsstk::CommonTime& when, gnsstk::NavType nav,
                double& corrOut) override
   {
      corrOut = 4.0;
      return true;
   }
};


class TestTropCorrector : public gnsstk::GroupPathCorrector
{
public:
   TestTropCorrector()
   { corrType = gnsstk::CorrectorType::Trop; }
   bool getCorr(const gnsstk::Position& rxPos, const gnsstk::Position& svPos,
                const gnsstk::SatID& sat, const gnsstk::ObsID& obs,
                const gnsstk::CommonTime& when, gnsstk::NavType nav,
                double& corrOut) override
   {
      corrOut = 10.0;
      return true;
   }
   bool getCorr(const gnsstk::Position& rxPos, const gnsstk::Xvt& svPos,
                const gnsstk::SatID& sat, const gnsstk::ObsID& obs,
                const gnsstk::CommonTime& when, gnsstk::NavType nav,
                double& corrOut) override
   {
      corrOut = 10.0;
      return true;
   }
};


class TestIonoCorrector : public gnsstk::GroupPathCorrector
{
public:
   TestIonoCorrector()
   { corrType = gnsstk::CorrectorType::Iono; }
   bool getCorr(const gnsstk::Position& rxPos, const gnsstk::Position& svPos,
                const gnsstk::SatID& sat, const gnsstk::ObsID& obs,
                const gnsstk::CommonTime& when, gnsstk::NavType nav,
                double& corrOut) override
   {
      corrOut = 20.0;
      return true;
   }
   bool getCorr(const gnsstk::Position& rxPos, const gnsstk::Xvt& svPos,
                const gnsstk::SatID& sat, const gnsstk::ObsID& obs,
                const gnsstk::CommonTime& when, gnsstk::NavType nav,
                double& corrOut) override
   {
      corrOut = 20.0;
      return true;
   }
};



class GroupPathCorr_T
{
public:
   GroupPathCorr_T();
   unsigned constructorTest();
   unsigned getCorrTestPositionDouble();
   unsigned getCorrTestXvtDouble();
   unsigned getCorrTestPositionResults();
   unsigned getCorrTestXvtResults();
   unsigned initTest();
   unsigned initGlobalTest();
   unsigned initNBTest();
   std::string dataPath;
};


GroupPathCorr_T ::
GroupPathCorr_T()
{
   dataPath = gnsstk::getPathData() + gnsstk::getFileSep();
}


unsigned GroupPathCorr_T ::
constructorTest()
{
   TUDEF("GroupPathCorr", "GroupPathCorr");
   gnsstk::GroupPathCorr uut;
   TUASSERTE(bool, true, uut.calcs.empty());
   TURETURN();
}


unsigned GroupPathCorr_T ::
getCorrTestPositionDouble()
{
   TUDEF("GroupPathCorr", "getCorr(Position,double)");
   gnsstk::BCISCorrector *isc;
   gnsstk::BCIonoCorrector *iono;
   gnsstk::GlobalTropCorrector *trop;
   gnsstk::GroupPathCorr uut;
   std::shared_ptr<gnsstk::NavLibrary> navLib;
   gnsstk::NavDataFactoryPtr ndf;
   gnsstk::Position stnPos(-740290.01, -5457071.705, 3207245.599);
   gnsstk::Position svPos(-16208820.579, -207275.833, 21038422.516);
   gnsstk::CommonTime when(gnsstk::CivilTime(2015,7,19,4,30,0,
                                             gnsstk::TimeSystem::Any));
   gnsstk::SatID sat(2, gnsstk::SatelliteSystem::GPS);
   gnsstk::ObsID oid(gnsstk::ObservationType::Phase, gnsstk::CarrierBand::L1,
                     gnsstk::TrackingCode::CA);
   gnsstk::NavType nav = gnsstk::NavType::GPSLNAV;
   ndf = std::make_shared<gnsstk::MultiFormatNavDataFactory>();
   gnsstk::MultiFormatNavDataFactory *mfndf =
      dynamic_cast<gnsstk::MultiFormatNavDataFactory*>(ndf.get());
   TUASSERTE(bool, true, mfndf->addDataSource(dataPath + "arlm2000.15n"));
   navLib = std::make_shared<gnsstk::NavLibrary>();
   TUCATCH(navLib->addFactory(ndf));
   double corr = 0.0;
   gnsstk::GroupPathCorrectorPtr ec1, ec2, ec3;
   ec1 = std::make_shared<gnsstk::BCISCorrector>();
   ec2 = std::make_shared<gnsstk::BCIonoCorrector>();
   ec3 = std::make_shared<gnsstk::GlobalTropCorrector>();
   isc = dynamic_cast<gnsstk::BCISCorrector*>(ec1.get());
   iono = dynamic_cast<gnsstk::BCIonoCorrector*>(ec2.get());
   trop = dynamic_cast<gnsstk::GlobalTropCorrector*>(ec3.get());
   TUASSERTE(bool, true, trop->loadFile(dataPath+"arlm2000.15m"));
   isc->navLib = navLib;
   iono->navLib = navLib;
   uut.calcs.push_back(ec1);
   uut.calcs.push_back(ec2);
   uut.calcs.push_back(ec3);
   TUASSERTE(bool, true, uut.getCorr(stnPos, svPos, sat, oid, when, nav, corr));
   TUASSERTFE(13.399231057432754, corr);
   TURETURN();
}


unsigned GroupPathCorr_T ::
getCorrTestXvtDouble()
{
   TUDEF("GroupPathCorr", "getCorr(Xvt,double)");
   gnsstk::BCISCorrector *isc;
   gnsstk::BCIonoCorrector *iono;
   gnsstk::GlobalTropCorrector *trop;
   gnsstk::GroupPathCorr uut;
   std::shared_ptr<gnsstk::NavLibrary> navLib;
   gnsstk::NavDataFactoryPtr ndf;
   gnsstk::Position stnPos(-740290.01, -5457071.705, 3207245.599);
   gnsstk::Xvt svPos;
   svPos.x = gnsstk::Triple(-16208820.579, -207275.833, 21038422.516);
      // The rest are just bunk with the intent that if the algorithm
      // is changed to include the data somehow, the assertion fails.
   svPos.v = gnsstk::Triple(123,456,789);
   svPos.clkbias = 234;
   svPos.clkdrift = 345;
   svPos.relcorr = 456;
   gnsstk::CommonTime when(gnsstk::CivilTime(2015,7,19,4,30,0,
                                             gnsstk::TimeSystem::Any));
   gnsstk::SatID sat(2, gnsstk::SatelliteSystem::GPS);
   gnsstk::ObsID oid(gnsstk::ObservationType::Phase, gnsstk::CarrierBand::L1,
                     gnsstk::TrackingCode::CA);
   gnsstk::NavType nav = gnsstk::NavType::GPSLNAV;
   ndf = std::make_shared<gnsstk::MultiFormatNavDataFactory>();
   gnsstk::MultiFormatNavDataFactory *mfndf =
      dynamic_cast<gnsstk::MultiFormatNavDataFactory*>(ndf.get());
   TUASSERTE(bool, true, mfndf->addDataSource(dataPath + "arlm2000.15n"));
   navLib = std::make_shared<gnsstk::NavLibrary>();
   TUCATCH(navLib->addFactory(ndf));
   double corr = 0.0;
   gnsstk::GroupPathCorrectorPtr ec1, ec2, ec3;
   ec1 = std::make_shared<gnsstk::BCISCorrector>();
   ec2 = std::make_shared<gnsstk::BCIonoCorrector>();
   ec3 = std::make_shared<gnsstk::GlobalTropCorrector>();
   isc = dynamic_cast<gnsstk::BCISCorrector*>(ec1.get());
   iono = dynamic_cast<gnsstk::BCIonoCorrector*>(ec2.get());
   trop = dynamic_cast<gnsstk::GlobalTropCorrector*>(ec3.get());
   TUASSERTE(bool, true, trop->loadFile(dataPath+"arlm2000.15m"));
   isc->navLib = navLib;
   iono->navLib = navLib;
   uut.calcs.push_back(ec1);
   uut.calcs.push_back(ec2);
   uut.calcs.push_back(ec3);
   TUASSERTE(bool, true, uut.getCorr(stnPos, svPos, sat, oid, when, nav, corr));
   TUASSERTFE(13.399231057432754, corr);
   TURETURN();
}


unsigned GroupPathCorr_T ::
getCorrTestPositionResults()
{
   TUDEF("GroupPathCorr", "getCorr(Position,CorrectionResults)");
   gnsstk::GroupPathCorr uut;
   gnsstk::GroupPathCorrectorPtr ec1, ec2, ec3, ec4;
   gnsstk::Position stnPos(-740290.01, -5457071.705, 3207245.599);
   gnsstk::Position svPos(-16208820.579, -207275.833, 21038422.516);
   gnsstk::CommonTime when(gnsstk::CivilTime(2015,7,19,4,30,0,
                                             gnsstk::TimeSystem::Any));
   gnsstk::SatID sat(27, gnsstk::SatelliteSystem::GPS);
   gnsstk::ObsID oid(gnsstk::ObservationType::Phase, gnsstk::CarrierBand::L1,
                     gnsstk::TrackingCode::CA);
   gnsstk::NavType nav = gnsstk::NavType::GPSLNAV;
   gnsstk::CorrectionResults out;
   ec1 = std::make_shared<TestISCorrector1>();
   ec2 = std::make_shared<TestIonoCorrector>();
   ec3 = std::make_shared<TestISCorrector2>();
   ec4 = std::make_shared<TestTropCorrector>();
   uut.calcs.push_back(ec1);
   uut.calcs.push_back(ec2);
   uut.calcs.push_back(ec3);
   uut.calcs.push_back(ec4);
      // These assertions verify that the default dup behavior is
      // ComputeFirst and that the ComputeFirst behavior hasn't
      // changed.
   TUASSERTE(bool, true, uut.getCorr(stnPos, svPos, sat, oid, when, nav, out));
   const gnsstk::CorrectionResultList& results1 = out.getResults();
   TUASSERTE(size_t, 3, results1.size());
   TUASSERTFE(32.0, out.getCorrSum(gnsstk::CorrDupHandling::ComputeFirst));
   auto i1 = results1.begin();
   TUASSERTFE(2.0, i1->result);
   TUASSERT(i1->source == ec1);
   i1++;
   TUASSERTFE(20.0, i1->result);
   TUASSERT(i1->source == ec2);
   i1++;
   TUASSERTFE(10.0, i1->result);
   TUASSERT(i1->source == ec4);
      // check ComputeLast
      // note that this also tests that getCorr clears the results first
   TUASSERTE(bool, true, uut.getCorr(stnPos, svPos, sat, oid, when, nav, out,
                                     gnsstk::CorrDupHandling::ComputeLast));
   const gnsstk::CorrectionResultList& results2 = out.getResults();
   TUASSERTE(size_t, 4, results2.size());
   TUASSERTFE(34.0, out.getCorrSum(gnsstk::CorrDupHandling::ComputeLast));
   auto i2 = results2.begin();
   TUASSERTFE(2.0, i2->result);
   TUASSERT(i2->source == ec1);
   i2++;
   TUASSERTFE(20.0, i2->result);
   TUASSERT(i2->source == ec2);
   i2++;
   TUASSERTFE(4.0, i2->result);
   TUASSERT(i2->source == ec3);
   i2++;
   TUASSERTFE(10.0, i2->result);
   TUASSERT(i2->source == ec4);
      // check UseFirst
   TUASSERTE(bool, true, uut.getCorr(stnPos, svPos, sat, oid, when, nav, out,
                                     gnsstk::CorrDupHandling::UseFirst));
   const gnsstk::CorrectionResultList& results3 = out.getResults();
   TUASSERTE(size_t, 4, results3.size());
   TUASSERTFE(32.0, out.getCorrSum(gnsstk::CorrDupHandling::UseFirst));
   auto i3 = results3.begin();
   TUASSERTFE(2.0, i3->result);
   TUASSERT(i3->source == ec1);
   i3++;
   TUASSERTFE(20.0, i3->result);
   TUASSERT(i3->source == ec2);
   i3++;
   TUASSERTFE(4.0, i3->result);
   TUASSERT(i3->source == ec3);
   i3++;
   TUASSERTFE(10.0, i3->result);
   TUASSERT(i3->source == ec4);
   TURETURN();
}


unsigned GroupPathCorr_T ::
getCorrTestXvtResults()
{
   TUDEF("GroupPathCorr", "getCorr(Xvt,CorrectionResults)");
   gnsstk::GroupPathCorr uut;
   gnsstk::GroupPathCorrectorPtr ec1, ec2, ec3, ec4;
   gnsstk::Position stnPos(-740290.01, -5457071.705, 3207245.599);
   gnsstk::Xvt svPos;
   svPos.x = gnsstk::Triple(-16208820.579, -207275.833, 21038422.516);
      // The rest are just bunk with the intent that if the algorithm
      // is changed to include the data somehow, the assertion fails.
   svPos.v = gnsstk::Triple(123,456,789);
   svPos.clkbias = 234;
   svPos.clkdrift = 345;
   svPos.relcorr = 456;
   gnsstk::CommonTime when(gnsstk::CivilTime(2015,7,19,4,30,0,
                                             gnsstk::TimeSystem::Any));
   gnsstk::SatID sat(27, gnsstk::SatelliteSystem::GPS);
   gnsstk::ObsID oid(gnsstk::ObservationType::Phase, gnsstk::CarrierBand::L1,
                     gnsstk::TrackingCode::CA);
   gnsstk::NavType nav = gnsstk::NavType::GPSLNAV;
   gnsstk::CorrectionResults out;
   ec1 = std::make_shared<TestISCorrector1>();
   ec2 = std::make_shared<TestIonoCorrector>();
   ec3 = std::make_shared<TestISCorrector2>();
   ec4 = std::make_shared<TestTropCorrector>();
   uut.calcs.push_back(ec1);
   uut.calcs.push_back(ec2);
   uut.calcs.push_back(ec3);
   uut.calcs.push_back(ec4);
      // These assertions verify that the default dup behavior is
      // ComputeFirst and that the ComputeFirst behavior hasn't
      // changed.
   TUASSERTE(bool, true, uut.getCorr(stnPos, svPos, sat, oid, when, nav, out));
   const gnsstk::CorrectionResultList& results1 = out.getResults();
   TUASSERTE(size_t, 3, results1.size());
   TUASSERTFE(32.0, out.getCorrSum(gnsstk::CorrDupHandling::ComputeFirst));
   auto i1 = results1.begin();
   TUASSERTFE(2.0, i1->result);
   TUASSERT(i1->source == ec1);
   i1++;
   TUASSERTFE(20.0, i1->result);
   TUASSERT(i1->source == ec2);
   i1++;
   TUASSERTFE(10.0, i1->result);
   TUASSERT(i1->source == ec4);
      // check ComputeLast
      // note that this also tests that getCorr clears the results first
   TUASSERTE(bool, true, uut.getCorr(stnPos, svPos, sat, oid, when, nav, out,
                                     gnsstk::CorrDupHandling::ComputeLast));
   const gnsstk::CorrectionResultList& results2 = out.getResults();
   TUASSERTE(size_t, 4, results2.size());
   TUASSERTFE(34.0, out.getCorrSum(gnsstk::CorrDupHandling::ComputeLast));
   auto i2 = results2.begin();
   TUASSERTFE(2.0, i2->result);
   TUASSERT(i2->source == ec1);
   i2++;
   TUASSERTFE(20.0, i2->result);
   TUASSERT(i2->source == ec2);
   i2++;
   TUASSERTFE(4.0, i2->result);
   TUASSERT(i2->source == ec3);
   i2++;
   TUASSERTFE(10.0, i2->result);
   TUASSERT(i2->source == ec4);
      // check UseFirst
   TUASSERTE(bool, true, uut.getCorr(stnPos, svPos, sat, oid, when, nav, out,
                                     gnsstk::CorrDupHandling::UseFirst));
   const gnsstk::CorrectionResultList& results3 = out.getResults();
   TUASSERTE(size_t, 4, results3.size());
   TUASSERTFE(32.0, out.getCorrSum(gnsstk::CorrDupHandling::UseFirst));
   auto i3 = results3.begin();
   TUASSERTFE(2.0, i3->result);
   TUASSERT(i3->source == ec1);
   i3++;
   TUASSERTFE(20.0, i3->result);
   TUASSERT(i3->source == ec2);
   i3++;
   TUASSERTFE(4.0, i3->result);
   TUASSERT(i3->source == ec3);
   i3++;
   TUASSERTFE(10.0, i3->result);
   TUASSERT(i3->source == ec4);
   TURETURN();
}


unsigned GroupPathCorr_T ::
initTest()
{
   TUDEF("GroupPathCorr", "init");
   gnsstk::GroupPathCorr uut;
   auto navLib = std::make_shared<gnsstk::NavLibrary>();
   TUASSERTE(bool, true, uut.init(navLib));
      // While the order isn't important, we assume it hasn't changed.
   auto i = uut.calcs.begin();
   TUASSERT(dynamic_cast<gnsstk::BCISCorrector*>(i->get()) != nullptr);
   TUASSERT(dynamic_cast<gnsstk::BCISCorrector*>(i->get())->navLib == navLib);
   i++;
   TUASSERT(dynamic_cast<gnsstk::BCIonoCorrector*>(i->get()) != nullptr);
   TUASSERT(dynamic_cast<gnsstk::BCIonoCorrector*>(i->get())->navLib == navLib);
   TURETURN();
}


unsigned GroupPathCorr_T ::
initGlobalTest()
{
   TUDEF("GroupPathCorr", "initGlobal");
   gnsstk::GroupPathCorr uut;
   auto navLib = std::make_shared<gnsstk::NavLibrary>();
   TUASSERTE(bool, true, uut.initGlobal(navLib, dataPath+"arlm2000.15m"));
      // While the order isn't important, we assume it hasn't changed.
   auto i = uut.calcs.begin();
   TUASSERT(dynamic_cast<gnsstk::BCISCorrector*>(i->get()) != nullptr);
   TUASSERT(dynamic_cast<gnsstk::BCISCorrector*>(i->get())->navLib == navLib);
   i++;
   TUASSERT(dynamic_cast<gnsstk::BCIonoCorrector*>(i->get()) != nullptr);
   TUASSERT(dynamic_cast<gnsstk::BCIonoCorrector*>(i->get())->navLib == navLib);
   i++;
   TUASSERT(dynamic_cast<gnsstk::GlobalTropCorrector*>(i->get()) != nullptr);
   TUASSERTE(bool, false,
             dynamic_cast<gnsstk::GlobalTropCorrector*>(i->get())->wxData.wx.obs.empty());
   TURETURN();
}


unsigned GroupPathCorr_T ::
initNBTest()
{
   TUDEF("GroupPathCorr", "initNB");
   gnsstk::GroupPathCorr uut;
   auto navLib = std::make_shared<gnsstk::NavLibrary>();
   TUASSERTE(bool, true, uut.initNB(navLib, dataPath+"arlm2000.15m"));
      // While the order isn't important, we assume it hasn't changed.
   auto i = uut.calcs.begin();
   TUASSERT(dynamic_cast<gnsstk::BCISCorrector*>(i->get()) != nullptr);
   TUASSERT(dynamic_cast<gnsstk::BCISCorrector*>(i->get())->navLib == navLib);
   i++;
   TUASSERT(dynamic_cast<gnsstk::BCIonoCorrector*>(i->get()) != nullptr);
   TUASSERT(dynamic_cast<gnsstk::BCIonoCorrector*>(i->get())->navLib == navLib);
   i++;
   TUASSERT(dynamic_cast<gnsstk::NBTropCorrector*>(i->get()) != nullptr);
   TUASSERTE(bool, false,
             dynamic_cast<gnsstk::NBTropCorrector*>(i->get())->wxData.wx.obs.empty());
   TURETURN();
}


int main()
{
   unsigned errorTotal = 0;
   GroupPathCorr_T testClass;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getCorrTestPositionDouble();
   errorTotal += testClass.getCorrTestXvtDouble();
   errorTotal += testClass.getCorrTestPositionResults();
   errorTotal += testClass.getCorrTestXvtResults();
   errorTotal += testClass.initTest();
   errorTotal += testClass.initGlobalTest();
   errorTotal += testClass.initNBTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
