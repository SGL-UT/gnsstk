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
#include <math.h>
#include "TestUtil.hpp"
#include "GLOCNavAlm.hpp"
#include "CivilTime.hpp"
#include "GLONASSTime.hpp"
#include "TimeString.hpp"
#include "Angle.hpp"
#include "Position.hpp"
#include "DebugTrace.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gnsstk::GLOCSatType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
   std::ostream& operator<<(std::ostream& s, gnsstk::GLOCOrbitType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class GLOCNavAlm_T
{
public:
   unsigned constructorTest();
   unsigned constructorPerturbationsTest();
   unsigned constructorUncorrectedTest();
   unsigned constructorCorrectedTest();
   unsigned constructorNumberCruncherTest();
   unsigned validateTest();
   unsigned getXvtTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
      /// Not getting the same results. Why not?
   void reproduce();
   unsigned reproduce2();
   unsigned NumberCruncherTest();
   unsigned DeltasTest();
   unsigned UncorrectedTest();
   unsigned CorrectedTest();

      // ICD constants
   static const double Tav;
   static const double iav;
   static const double GM; // in km, not m
   static const double ae; // in km, not m
   static const double J20;
   static const double we; // rad/s
      // example data from M.2
   static const double NA;
   static const double tlambdaA;
   static const double DeltaTA;
   static const double DeltaTdotA;
   static const double lambdaA;
   static const double omegaA;
   static const double epsilonA;
   static const double DeltaiA;
   static const double ti;
   static const double N;
   static const gnsstk::GLONASSTime toi;
   static const gnsstk::GLONASSTime tA;
      // values pulled from ICD GLONASS CDMA General Edition appendix M.2
   static const double DeltatprExpected;
   static const int WExpected;
   static const double iExpected;
   static const double TdrExpected;
   static const double nExpected;
   static const double aExpected;
   static const double lambdaExpected;
   static const double omegaExpected;
   static const double E0Expected;
   static const double L1Expected;
   static const double LExpected;
   static const double aprimeExpected;
   static const double epsilonprimeExpected;
   static const double iprimeExpected;
   static const double lambdaprimeExpected;
   static const double omegaprimeExpected;
   static const double LprimeExpected;
   static const double hprimeExpected;
   static const double lprimeExpected;
   static const double pprimeExpected;
   static const double hExpected;
   static const double lExpected;
   static const double BExpected;
   static const double deltaa1_aExpected;
   static const double deltaa2_aExpected;
   static const double deltah1Expected;
   static const double deltah2Expected;
   static const double deltal1Expected;
   static const double deltal2Expected;
   static const double deltalambda1Expected;
   static const double deltalambda2Expected;
   static const double deltai1Expected;
   static const double deltai2Expected;
   static const double deltaL1Expected;
   static const double deltaL2Expected;
   static const double EExpected;
   static const double nuExpected;
   static const double uExpected;
      // not explicitly documented, but confirmed by other means
   static const double pExpected;
   static const double rExpected;
   static const double vrExpected;
   static const double vuExpected;
   static const double xExpected;
   static const double yExpected;
   static const double zExpected;
   static const double xdotExpected;
   static const double ydotExpected;
   static const double zdotExpected;
   static const double clkbiasExpected;
   static const double clkdriftExpected;
      /// @warning this value not vetted independently.
   static const double relcorrExpected;
};


const double GLOCNavAlm_T::Tav = 40544.0;
const double GLOCNavAlm_T::iav = 64.8;
const double GLOCNavAlm_T::GM = 398600441.8e-3; // in km, not m
const double GLOCNavAlm_T::ae = 6378136e-3; // in km, not m
const double GLOCNavAlm_T::J20 = 1082.62575e-6;
const double GLOCNavAlm_T::we = 7.2921150e-5; // rad/s
const double GLOCNavAlm_T::NA = 1452;
const double GLOCNavAlm_T::tlambdaA = 33571.625;
const double GLOCNavAlm_T::DeltaTA = 0.01953124999975;
const double GLOCNavAlm_T::DeltaTdotA = 6.103515625E-05;
const double GLOCNavAlm_T::lambdaA = -0.293967247009277;
const double GLOCNavAlm_T::omegaA = 0.57867431640625;
const double GLOCNavAlm_T::epsilonA = 0.000432968139648438;
const double GLOCNavAlm_T::DeltaiA = -0.00012947082519531;
const double GLOCNavAlm_T::ti = 51300;
const double GLOCNavAlm_T::N = 1453;
const gnsstk::GLONASSTime GLOCNavAlm_T::toi(0, N, ti);
const gnsstk::GLONASSTime GLOCNavAlm_T::tA(0, NA, tlambdaA);
const double GLOCNavAlm_T::DeltatprExpected = 104128.375;
const int GLOCNavAlm_T::WExpected = 2;
const double GLOCNavAlm_T::iExpected = 1.1305666106990377;
const double GLOCNavAlm_T::TdrExpected = 40544.019836425781;
const double GLOCNavAlm_T::nExpected = 0.00015497193747756143;
const double GLOCNavAlm_T::aExpected = 25509.694012650143;
const double GLOCNavAlm_T::lambdaExpected = -8.5173843140309469;
const double GLOCNavAlm_T::omegaExpected = 1.8178836298808301;
const double GLOCNavAlm_T::E0Expected = -1.8174637892065451;
const double GLOCNavAlm_T::L1Expected = 0.00083970352771615942;
const double GLOCNavAlm_T::LExpected = 3.5714451660610322;
const double GLOCNavAlm_T::aprimeExpected = 25508.9554310860556;
const double GLOCNavAlm_T::epsilonprimeExpected = 0.000424199178735691155;
const double GLOCNavAlm_T::iprimeExpected = 1.13055979412986857;
const double GLOCNavAlm_T::lambdaprimeExpected = -8.51736802279423523;
const double GLOCNavAlm_T::omegaprimeExpected = 1.96580151879618214;
const double GLOCNavAlm_T::LprimeExpected = 3.57148542469343516;
const double GLOCNavAlm_T::hprimeExpected = 0.00039153353705544239;
const double GLOCNavAlm_T::lprimeExpected = -0.00016323735050805419;
const double GLOCNavAlm_T::pprimeExpected = 25508.9508408785184;
const double GLOCNavAlm_T::hExpected = 0.00041981843111904164;
const double GLOCNavAlm_T::lExpected = -0.00010589567905904274;
const double GLOCNavAlm_T::BExpected = 0.00010151884398503961;
const double GLOCNavAlm_T::deltaa1_aExpected = 8.30615563565482404e-05;
const double GLOCNavAlm_T::deltaa2_aExpected = 5.41085801381285305e-05;
const double GLOCNavAlm_T::deltah1Expected = 1.69203051231874733e-08;
const double GLOCNavAlm_T::deltah2Expected = -2.82679737584760494e-05;
const double GLOCNavAlm_T::deltal1Expected = 4.61175901454555875e-05;
const double GLOCNavAlm_T::deltal2Expected = -1.12240813035558812e-05;
const double GLOCNavAlm_T::deltalambda1Expected = 6.05435071360537479e-08;
const double GLOCNavAlm_T::deltalambda2Expected = 1.63517802188070858e-05;
const double GLOCNavAlm_T::deltai1Expected = 1.95658219192184023e-05;
const double GLOCNavAlm_T::deltai2Expected = 1.27492527500887458e-05;
const double GLOCNavAlm_T::deltaL1Expected = 1.10334212545863083e-08;
const double GLOCNavAlm_T::deltaL2Expected = 4.02696658242106401e-05;
const double GLOCNavAlm_T::EExpected = 1.60610784063585167;
const double GLOCNavAlm_T::nuExpected = 1.60653177221277219;
const double GLOCNavAlm_T::uExpected = 3.57233329100895433;
const double GLOCNavAlm_T::pExpected = 25509.689230566949846;
const double GLOCNavAlm_T::rExpected = 25509.3374532650379;
const double GLOCNavAlm_T::vrExpected = 0.00167577247194655653;
const double GLOCNavAlm_T::vuExpected = 3.95290163460661152;
const double GLOCNavAlm_T::xExpected = 10697116.4874360654;
const double GLOCNavAlm_T::yExpected = 21058292.4241863210;
const double GLOCNavAlm_T::zExpected = -9635679.33963303405;
const double GLOCNavAlm_T::xdotExpected = -686.100809921691084;
const double GLOCNavAlm_T::ydotExpected = -1136.54864124521881;
const double GLOCNavAlm_T::zdotExpected = -3249.98587740305799;
const double GLOCNavAlm_T::clkbiasExpected = 0;
const double GLOCNavAlm_T::clkdriftExpected = 0;
const double GLOCNavAlm_T::relcorrExpected = -9.5126785342953703954e-10;

unsigned GLOCNavAlm_T ::
constructorTest()
{
   TUDEF("GLOCNavAlm", "GLOCNavAlm()");
   gnsstk::GLOCNavAlm uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Almanac,
             uut.signal.messageType);
   TUASSERTE(gnsstk::GLOCOrbitType, gnsstk::GLOCOrbitType::Unknown,
             uut.orbitType);
   TUASSERTE(unsigned, 0, uut.numSVs);
   TUASSERTE(unsigned, 0, uut.aoa);
   TUASSERTE(unsigned, 0, uut.NA);
   TUASSERTE(unsigned, 0, uut.statusReg);
   TUASSERTE(gnsstk::GLOCSatType, gnsstk::GLOCSatType::Unknown,
             uut.satType);
   TUASSERTE(int, 1, isnan(uut.tau));
   TUASSERTE(int, 1, isnan(uut.lambda));
   TUASSERTE(int, 1, isnan(uut.tLambda));
   TUASSERTE(int, 1, isnan(uut.deltai));
   TUASSERTE(int, 1, isnan(uut.ecc));
   TUASSERTE(int, 1, isnan(uut.omega));
   TUASSERTE(int, 1, isnan(uut.deltaT));
   TUASSERTE(int, 1, isnan(uut.deltaTdot));
   TURETURN();
}


unsigned GLOCNavAlm_T ::
constructorPerturbationsTest()
{
   TUDEF("GLOCNavAlm::Perturbations", "Perturbations()");
   gnsstk::GLOCNavAlm::Perturbations uut;
   TUASSERTE(int, 1, isnan(uut.a));
   TUASSERTE(int, 1, isnan(uut.h));
   TUASSERTE(int, 1, isnan(uut.l));
   TUASSERTE(int, 1, isnan(uut.lambda));
   TUASSERTE(int, 1, isnan(uut.omega));
   TUASSERTE(int, 1, isnan(uut.i));
   TUASSERTE(int, 1, isnan(uut.Lk));
   TURETURN();
}


unsigned GLOCNavAlm_T ::
constructorUncorrectedTest()
{
   TUDEF("GLOCNavAlm::Uncorrected", "Uncorrected()");
   gnsstk::GLOCNavAlm::Uncorrected uut;
   TUASSERTE(int, 1, isnan(uut.p));
   TURETURN();
}


unsigned GLOCNavAlm_T ::
constructorCorrectedTest()
{
   TUDEF("GLOCNavAlm::Corrected", "Corrected()");
   gnsstk::GLOCNavAlm::Corrected uut;
   TUASSERTE(int, 1, isnan(uut.E));
   TUASSERTE(int, 1, isnan(uut.epsilon));
   TUASSERTE(int, 1, isnan(uut.nu));
   TUASSERTE(int, 1, isnan(uut.p));
   TUASSERTE(int, 1, isnan(uut.u));
   TUASSERTE(int, 1, isnan(uut.r));
   TUASSERTE(int, 1, isnan(uut.vu));
   TUASSERTE(int, 1, isnan(uut.vr));
   TURETURN();
}

unsigned GLOCNavAlm_T ::
constructorNumberCruncherTest()
{
   TUDEF("GLOCNavAlm::NumberCruncher", "NumberCruncher()");
   gnsstk::GLOCNavAlm::NumberCruncher uut;
   TUASSERTE(int, 1, isnan(uut.Deltatpr));
   TUASSERTE(int, -1, uut.W);
   TUASSERTE(int, 1, isnan(uut.Tdr));
   TUASSERTE(int, 1, isnan(uut.n));
   TUASSERTE(int, 1, isnan(uut.E0));
   TUASSERTE(int, 1, isnan(uut.L1));
   TUASSERTE(int, 1, isnan(uut.L));
   TUASSERTE(int, 1, isnan(uut.B));
   TURETURN();
}

unsigned GLOCNavAlm_T ::
validateTest()
{
   TUDEF("GLOCNavAlm", "validate()");
   gnsstk::GLOCNavAlm uut;
      /// @todo Implement some testing when the function has some meat to it.
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}


unsigned GLOCNavAlm_T ::
getXvtTest()
{
   TUDEF("GLOCNavAlm", "getXvt()");
   gnsstk::GLOCNavAlm uut;
      //uut.tau = not specified
   uut.lambda = lambdaA;
   uut.deltai = DeltaiA;
   uut.ecc = epsilonA;
   uut.omega = omegaA;
   uut.tLambda = tlambdaA;
   uut.deltaT = DeltaTA;
   uut.deltaTdot = DeltaTdotA;
   uut.Toa = gnsstk::GLONASSTime(0, NA, uut.tLambda, gnsstk::TimeSystem::GLO);
   gnsstk::RefFrame expRF(gnsstk::RefFrameRlz::PZ90KGS);

   gnsstk::Xvt xvt;
   TUASSERTE(bool, true, uut.getXvt(toi, xvt));
   TUASSERTFESMRT(xExpected, xvt.x[0]);
   TUASSERTFESMRT(yExpected, xvt.x[1]);
   TUASSERTFESMRT(zExpected, xvt.x[2]);
      // This is a bit of a cheat.  All I can figure is that numeric
      // error gets accumulated in the LSBs leading to v[0] being just
      // slightly out of spec.  That or the fact that it's negative
      // has an effect on the representable digits.  Either way, this
      // same code and its results are tested pretty thoroughly,
      // step-by-step, in other test methods.
   TUASSERTFEPS(xdotExpected, xvt.v[0], 1.5e-12);
   TUASSERTFESMRT(ydotExpected, xvt.v[1]);
   TUASSERTFESMRT(zdotExpected, xvt.v[2]);
   TUASSERTFESMRT(clkbiasExpected, xvt.clkbias);
   TUASSERTFESMRT(clkdriftExpected, xvt.clkdrift);
   TUASSERTFESMRT(relcorrExpected, xvt.relcorr);
   TUASSERTE(gnsstk::RefFrame,expRF,xvt.frame);

   TUASSERTFESMRT(DeltatprExpected, uut.math.Deltatpr);
   TUASSERTE(int, WExpected, uut.math.W);
   TUASSERTFESMRT(TdrExpected, uut.math.Tdr);
   TUASSERTFESMRT(nExpected, uut.math.n);
   TUASSERTFESMRT(E0Expected, uut.math.E0);
   TUASSERTFESMRT(L1Expected, uut.math.L1);
   TUASSERTFESMRT(LExpected, uut.math.L);
   TUASSERTFESMRT(BExpected, uut.math.B);

   TUASSERTFESMRT(aprimeExpected, uut.math.corrected.a);
   TUASSERTFESMRT(epsilonprimeExpected, uut.math.corrected.epsilon);
   TUASSERTFESMRT(iprimeExpected, uut.math.corrected.i);
   TUASSERTFESMRT(lambdaprimeExpected, uut.math.corrected.lambda);
   TUASSERTFESMRT(omegaprimeExpected, uut.math.corrected.omega);
   TUASSERTFESMRT(LprimeExpected, uut.math.corrected.Lk);
   TUASSERTFESMRT(hprimeExpected, uut.math.corrected.h);
   TUASSERTFESMRT(lprimeExpected, uut.math.corrected.l);
   TUASSERTFESMRT(pprimeExpected, uut.math.corrected.p);
   TUASSERTFESMRT(EExpected, uut.math.corrected.E);
   TUASSERTFESMRT(nuExpected, uut.math.corrected.nu);
   TUASSERTFESMRT(rExpected, uut.math.corrected.r);
   TUASSERTFESMRT(uExpected, uut.math.corrected.u);
   TUASSERTFESMRT(vuExpected, uut.math.corrected.vu);
   TUASSERTFESMRT(vrExpected, uut.math.corrected.vr);

   TUASSERTFESMRT(iExpected, uut.math.uncorrected.i);
   TUASSERTFESMRT(aExpected, uut.math.uncorrected.a);
   TUASSERTFESMRT(omegaExpected, uut.math.uncorrected.omega);
   TUASSERTFESMRT(lambdaExpected, uut.math.uncorrected.lambda);
   TUASSERTFESMRT(hExpected, uut.math.uncorrected.h);
   TUASSERTFESMRT(lExpected, uut.math.uncorrected.l);
   TUASSERTFESMRT(pExpected, uut.math.uncorrected.p);

   TUASSERTFESMRT(deltaa1_aExpected, uut.math.k1.a);
   TUASSERTFESMRT(deltah1Expected, uut.math.k1.h);
   TUASSERTFESMRT(deltal1Expected, uut.math.k1.l);
   TUASSERTFESMRT(deltalambda1Expected, uut.math.k1.lambda);
   TUASSERTFESMRT(deltai1Expected, uut.math.k1.i);
   TUASSERTFESMRT(deltaL1Expected, uut.math.k1.Lk);

   TUASSERTFESMRT(deltaa2_aExpected, uut.math.k2.a);
   TUASSERTFESMRT(deltah2Expected, uut.math.k2.h);
   TUASSERTFESMRT(deltal2Expected, uut.math.k2.l);
   TUASSERTFESMRT(deltalambda2Expected, uut.math.k2.lambda);
   TUASSERTFESMRT(deltai2Expected, uut.math.k2.i);
   TUASSERTFESMRT(deltaL2Expected, uut.math.k2.Lk);

   TURETURN();
}


unsigned GLOCNavAlm_T ::
getUserTimeTest()
{
   TUDEF("GLOCNavAlm", "getUserTime()");
   gnsstk::GLOCNavAlm uut;
   uut.timeStamp = gnsstk::CivilTime(2006, 10, 01, 0, 15, 0,
                                     gnsstk::TimeSystem::GLO);
   gnsstk::CommonTime exp(uut.timeStamp + 3.0);
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GLOCNavAlm_T ::
fixFitTest()
{
   TUDEF("GLOCNavAlm", "fixFit()");
   gnsstk::GLOCNavAlm uut;
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


unsigned GLOCNavAlm_T ::
UncorrectedTest()
{
   TUDEF("GLOCNavAlm::Uncorrected", "moo");
   gnsstk::GLOCNavAlm::Uncorrected uut;
   TUCSM("seti");
   TUCATCH(uut.seti(DeltaiA));
   TUASSERTFESMRT(iExpected, uut.i);
   TUCSM("seta");
   TUCATCH(uut.seta(epsilonA, omegaA, TdrExpected));
   TUASSERTFESMRT(aExpected, uut.a);
   TUCSM("setp");
   TUCATCH(uut.setp(epsilonA));
   TUASSERTFESMRT(pExpected, uut.p);
   TUCSM("setlambda");
   TUCATCH(uut.setlambda(lambdaA, nExpected, DeltatprExpected));
   TUASSERTFESMRT(lambdaExpected, uut.lambda);
   TUCSM("setomega");
   TUCATCH(uut.setomega(omegaA, nExpected, DeltatprExpected));
   TUASSERTFESMRT(omegaExpected, uut.omega);
   TUCSM("sethl");
   TUCATCH(uut.sethl(epsilonA));
   TUASSERTFESMRT(hExpected, uut.h);
   TUASSERTFESMRT(lExpected, uut.l);
   TURETURN();
}


unsigned GLOCNavAlm_T ::
CorrectedTest()
{
   TUDEF("GLOCNavAlm::Corrected", "setData");
   gnsstk::GLOCNavAlm::Corrected uut;
   gnsstk::GLOCNavAlm::Uncorrected uncor;
   gnsstk::GLOCNavAlm::Deltas delta1, delta2;
   TUCSM("setnu");
   uut = gnsstk::GLOCNavAlm::Corrected();
   uut.epsilon = epsilonprimeExpected;
   uut.E = EExpected;
   TUCATCH(uut.setnu());
   TUASSERTFESMRT(nuExpected, uut.nu);
   TUCSM("setE");
   uut = gnsstk::GLOCNavAlm::Corrected();
   uut.Lk = LprimeExpected;
   uut.omega = omegaprimeExpected;
   uut.epsilon = epsilonprimeExpected;
   TUCATCH(uut.setE(LExpected));
   TUASSERTFESMRT(EExpected, uut.E);
   TUCSM("setp");
   uut = gnsstk::GLOCNavAlm::Corrected();
   uut.epsilon = epsilonprimeExpected;
   uut.a = aprimeExpected;
   TUCATCH(uut.setp());
   TUASSERTFESMRT(pprimeExpected, uut.p);
   TUCSM("setu");
   uut = gnsstk::GLOCNavAlm::Corrected();
   uut.nu = nuExpected;
   uut.omega = omegaprimeExpected;
   TUCATCH(uut.setu());
   TUASSERTFESMRT(uExpected, uut.u);
   TUCSM("setr");
   uut = gnsstk::GLOCNavAlm::Corrected();
   uut.p = pprimeExpected;
   uut.epsilon = epsilonprimeExpected;
   uut.nu = nuExpected;
   TUCATCH(uut.setr());
   TUASSERTFESMRT(rExpected, uut.r);
   TUCSM("setvrvu");
   uut = gnsstk::GLOCNavAlm::Corrected();
   uut.p = pprimeExpected;
   uut.epsilon = epsilonprimeExpected;
   uut.nu = nuExpected;
   TUCATCH(uut.setvrvu());
   TUASSERTFESMRT(vrExpected, uut.vr);
   TUASSERTFESMRT(vuExpected, uut.vu);
   TUCSM("setData");
   uut = gnsstk::GLOCNavAlm::Corrected();
   uncor.a = aExpected;
   uncor.h = hExpected;
   uncor.l = lExpected;
   uncor.i = iExpected;
   uncor.lambda = lambdaExpected;
   delta1.a = deltaa1_aExpected;
   delta1.h = deltah1Expected;
   delta1.l = deltal1Expected;
   delta1.lambda = deltalambda1Expected;
   delta1.i = deltai1Expected;
   delta1.Lk = deltaL1Expected;
   delta2.a = deltaa2_aExpected;
   delta2.h = deltah2Expected;
   delta2.l = deltal2Expected;
   delta2.lambda = deltalambda2Expected;
   delta2.i = deltai2Expected;
   delta2.Lk = deltaL2Expected;
   TUCATCH(uut.setData(LExpected, uncor, delta1, delta2));
   TUCSM("setData(a')");
   TUASSERTFESMRT(aprimeExpected, uut.geta());
   TUCSM("setData(h')");
   TUASSERTFESMRT(hprimeExpected, uut.geth());
   TUCSM("setData(l')");
   TUASSERTFESMRT(lprimeExpected, uut.getl());
   TUCSM("setData(lambda')");
   TUASSERTFESMRT(lambdaprimeExpected, uut.getlambda());
   TUCSM("setData(epsilon')");
   TUASSERTFESMRT(epsilonprimeExpected, uut.epsilon);
   TUCSM("setData(omega')");
   TUASSERTFESMRT(omegaprimeExpected, uut.getomega());
   TUCSM("setData(i')");
   TUASSERTFESMRT(iprimeExpected, uut.geti());
   TUCSM("setData(L')");
   TUASSERTFESMRT(LprimeExpected, uut.getLk());
   TUCSM("setData(p)");
   TUASSERTFESMRT(pprimeExpected, uut.p);
   TUCSM("setData(u)");
   TUASSERTFESMRT(uExpected, uut.u);
   TUCSM("setData(r)");
   TUASSERTFESMRT(rExpected, uut.r);
   TUCSM("setData(vr)");
   TUASSERTFESMRT(vrExpected, uut.vr);
   TUCSM("setData(vu)");
   TUASSERTFESMRT(vuExpected, uut.vu);
      // time is only used to figure out the ref frame
   gnsstk::CivilTime dummy(2023,1,1,0,0,0,gnsstk::TimeSystem::UTC);
   gnsstk::Xvt got = uut.getXvt(dummy);
   TUCSM("getXvt(x)");
   TUASSERTFESMRT(xExpected, got.x[0]);
   TUCSM("getXvt(y)");
   TUASSERTFESMRT(yExpected, got.x[1]);
   TUCSM("getXvt(z)");
   TUASSERTFESMRT(zExpected, got.x[2]);
   TUCSM("getXvt(vx)");
   TUASSERTFESMRT(xdotExpected, got.v[0]);
   TUCSM("getXvt(vy)");
   TUASSERTFESMRT(ydotExpected, got.v[1]);
   TUCSM("getXvt(vz)");
   TUASSERTFESMRT(zdotExpected, got.v[2]);
   TURETURN();
}


unsigned GLOCNavAlm_T ::
NumberCruncherTest()
{
   TUDEF("GLOCNavAlm::NumberCruncher", "moo");
   gnsstk::GLOCNavAlm::NumberCruncher uut;
   TUCSM("setDeltatpr");
   TUCATCH(uut.setDeltatpr(tA, toi));
   TUASSERTFESMRT(DeltatprExpected, uut.Deltatpr);
   TUCSM("setW");
   TUCATCH(uut.setW(DeltaTA));
   TUASSERTE(int, WExpected, uut.W);
   TUCSM("setTdr");
   TUCATCH(uut.setTdr(DeltaTA, DeltaTdotA));
   TUASSERTFESMRT(TdrExpected, uut.Tdr);
   TUCSM("setn");
   TUCATCH(uut.setn());
   TUASSERTFESMRT(nExpected, uut.n);
   TUCSM("setE0");
   uut.uncorrected.omega = omegaExpected;
   TUCATCH(uut.setE0(epsilonA));
   TUASSERTFESMRT(E0Expected, uut.E0);
   TUCSM("setL1");
   TUCATCH(uut.setL1(epsilonA));
   TUASSERTFESMRT(L1Expected, uut.L1);
   TUCSM("setL");
   TUCATCH(uut.setL(DeltaTA, DeltaTdotA));
   TUASSERTFESMRT(LExpected, uut.L);
   TUCSM("setB");
   uut.uncorrected.a = aExpected;
   TUCATCH(uut.setB());
   TUASSERTFESMRT(BExpected, uut.B);
   TURETURN();
}


unsigned GLOCNavAlm_T ::
DeltasTest()
{
   TUDEF("GLOCNavAlm::NumberCruncher::Deltas", "moo");
   gnsstk::GLOCNavAlm::Deltas deltas;
   gnsstk::GLOCNavAlm::Uncorrected uncor;
   TUCSM("setdeltaa_a");
   uncor.h = hExpected;
   uncor.l = lExpected;
   uncor.a = aExpected;
   uncor.i = iExpected;
   deltas.setdeltaa_a(BExpected, L1Expected, uncor);
   TUASSERTFESMRT(deltaa1_aExpected, deltas.a);
   deltas.setdeltaa_a(BExpected, LExpected, uncor);
   TUASSERTFESMRT(deltaa2_aExpected, deltas.a);
   TUCSM("setdeltah");
   deltas.setdeltah(BExpected, L1Expected, uncor);
   TUASSERTFESMRT(deltah1Expected, deltas.h);
   deltas.setdeltah(BExpected, LExpected, uncor);
   TUASSERTFESMRT(deltah2Expected, deltas.h);
   TUCSM("setdeltal");
   deltas.setdeltal(BExpected, L1Expected, uncor);
   TUASSERTFESMRT(deltal1Expected, deltas.l);
   deltas.setdeltal(BExpected, LExpected, uncor);
   TUASSERTFESMRT(deltal2Expected, deltas.l);
   TUCSM("setdeltalambda");
   deltas.setdeltalambda(BExpected, L1Expected, uncor);
   TUASSERTFESMRT(deltalambda1Expected, deltas.lambda);
   deltas.setdeltalambda(BExpected, LExpected, uncor);
   TUASSERTFESMRT(deltalambda2Expected, deltas.lambda);
   TUCSM("setdeltai");
   deltas.setdeltai(BExpected, L1Expected, uncor);
   TUASSERTFESMRT(deltai1Expected, deltas.i);
   deltas.setdeltai(BExpected, LExpected, uncor);
   TUASSERTFESMRT(deltai2Expected, deltas.i);
   TUCSM("setdeltaLk");
   deltas.setdeltaLk(BExpected, L1Expected, uncor);
   TUASSERTFESMRT(deltaL1Expected, deltas.Lk);
   deltas.setdeltaLk(BExpected, LExpected, uncor);
   TUASSERTFESMRT(deltaL2Expected, deltas.Lk);
   TURETURN();
}


int main()
{
   GLOCNavAlm_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.constructorPerturbationsTest();
   errorTotal += testClass.constructorUncorrectedTest();
   errorTotal += testClass.constructorCorrectedTest();
   errorTotal += testClass.constructorNumberCruncherTest();
   errorTotal += testClass.validateTest();
   errorTotal += testClass.getXvtTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();
   errorTotal += testClass.NumberCruncherTest();
   errorTotal += testClass.DeltasTest();
   errorTotal += testClass.UncorrectedTest();
   errorTotal += testClass.CorrectedTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
