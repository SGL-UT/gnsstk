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
#include "TropCorrector.hpp"
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


template <class Model>
class TestClass : public gnsstk::TropCorrector<Model>
{
public:
   TestClass()
   {}
   bool getUseDefault() const
   { return gnsstk::TropCorrector<Model>::useDefault; }
   double getDefTemp() const
   { return gnsstk::TropCorrector<Model>::defTemp; }
   double getDefPres() const
   { return gnsstk::TropCorrector<Model>::defPres; }
   double getDefHum() const
   { return gnsstk::TropCorrector<Model>::defHum; }
};


class TropCorrector_T
{
public:
   TropCorrector_T();
   unsigned constructorTest();
      // These test both getCorr and loadFile.
   unsigned getCorrTestPosition();
   unsigned getCorrTestXvt();
   std::string dataPath;
};


TropCorrector_T ::
TropCorrector_T()
{
   dataPath = gnsstk::getPathData() + gnsstk::getFileSep();
}


unsigned TropCorrector_T ::
constructorTest()
{
   TUDEF("TropCorrector", "TropCorrector");
   TestClass<gnsstk::ZeroTropModel> uut;
   TUASSERTE(gnsstk::CorrectorType, gnsstk::CorrectorType::Trop, uut.corrType);
   TUASSERTE(bool, true, std::isnan(uut.getDefTemp()));
   TUASSERTE(bool, true, std::isnan(uut.getDefPres()));
   TUASSERTE(bool, true, std::isnan(uut.getDefHum()));
   TUASSERTE(bool, false, uut.getUseDefault());
      // Just make sure the typedefs are still usable.
   gnsstk::ZeroTropCorrector uut2;
   gnsstk::SimpleTropCorrector uut3;
   gnsstk::SaasTropCorrector uut4;
   gnsstk::NBTropCorrector uut5;
   gnsstk::GGTropCorrector uut6;
   gnsstk::GGHeightTropCorrector uut7;
   gnsstk::NeillTropCorrector uut8;
   gnsstk::GlobalTropCorrector uut9;
   TURETURN();
}


unsigned TropCorrector_T ::
getCorrTestPosition()
{
   TUDEF("TropCorrector", "getCorr(Position)");
   TestClass<gnsstk::GlobalTropModel> uut;
   gnsstk::Position stnPos(-740290.01, -5457071.705, 3207245.599);
   gnsstk::Position svPos(-16208820.579, -207275.833, 21038422.516);
   gnsstk::CommonTime when(gnsstk::CivilTime(2015,7,19,4,30,0,
                                             gnsstk::TimeSystem::Any));
   gnsstk::SatID sat(27, gnsstk::SatelliteSystem::GPS);
   gnsstk::ObsID oid(gnsstk::ObservationType::Phase, gnsstk::CarrierBand::L1,
                     gnsstk::TrackingCode::CA);
   gnsstk::NavType nav = gnsstk::NavType::GPSLNAV;
   double corr = 0.0;
   TUCSM("loadFile");
   TUASSERTE(bool, true, uut.loadFile(dataPath+"arlm2000.15m"));
   TUCSM("getCorr(Position)");
   TUASSERTE(bool, true, uut.getCorr(stnPos, svPos, sat, oid, when, nav, corr));
   TUASSERTFE(9.775888009609917, corr);

      // test NBTropModel special handling
   TestClass<gnsstk::NBTropModel> uut2;
   TUASSERTE(bool, true,
             uut2.getCorr(stnPos, svPos, sat, oid, when, nav, corr));
      // epsilon so windows passes ...
   TUASSERTFEPS(9.4475001293154239335, corr, 1e-14);

   TestClass<gnsstk::GlobalTropModel> uut3;
   TUASSERTE(bool, false,
             uut3.getCorr(stnPos, svPos, sat, oid, when, nav, corr));

   TestClass<gnsstk::NBTropModel> uut4;
   TUCSM("setDefaultWx");
   uut4.setDefaultWx();
   TUASSERTE(bool, true,
             uut4.getCorr(stnPos, svPos, sat, oid, when, nav, corr));
      // Different value with the default weather compared without
      // implies that it's using those default weather values.
      // epsilon so windows passes ...
   TUASSERTFEPS(9.1431767236047711833, corr, 1e-14);
   TURETURN();
}


unsigned TropCorrector_T ::
getCorrTestXvt()
{
   TUDEF("TropCorrector", "getCorr(Xvt)");
   TestClass<gnsstk::GlobalTropModel> uut;
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
   double corr = 0.0;
   TUCSM("loadFile");
   TUASSERTE(bool, true, uut.loadFile(dataPath+"arlm2000.15m"));
   TUCSM("getCorr(Xvt)");
   TUASSERTE(bool, true, uut.getCorr(stnPos, svPos, sat, oid, when, nav, corr));
   TUASSERTFE(9.775888009609917, corr);
   TURETURN();
}


int main()
{
   unsigned errorTotal = 0;
   TropCorrector_T testClass;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getCorrTestPosition();
   errorTotal += testClass.getCorrTestXvt();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
