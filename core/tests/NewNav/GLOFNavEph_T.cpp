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
#include "GLOFNavEph.hpp"
#include "CivilTime.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, GLOFNavData::PCode e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}


class GLOFNavEph_T
{
public:
   unsigned constructorTest();
   unsigned validateTest();
   unsigned getXvtTest();
   unsigned getUserTimeTest();
   unsigned fixFitTest();
};


unsigned GLOFNavEph_T ::
constructorTest()
{
   TUDEF("GLOFNavEph", "GLOFNavEph()");
   gnsstk::GLOFNavEph uut;
   gnsstk::CommonTime exp;
   TUASSERTE(gnsstk::CommonTime, exp, uut.ref);
   TUASSERTE(gnsstk::CommonTime, exp, uut.xmit3);
   TUASSERTE(gnsstk::CommonTime, exp, uut.xmit4);
   TUASSERTE(size_t, 3, uut.pos.size());
   TUASSERTFE(0, uut.pos[0]);
   TUASSERTFE(0, uut.pos[1]);
   TUASSERTFE(0, uut.pos[2]);
   TUASSERTE(size_t, 3, uut.vel.size());
   TUASSERTFE(0, uut.vel[0]);
   TUASSERTFE(0, uut.vel[1]);
   TUASSERTFE(0, uut.vel[2]);
   TUASSERTE(size_t, 3, uut.acc.size());
   TUASSERTFE(0, uut.acc[0]);
   TUASSERTFE(0, uut.acc[1]);
   TUASSERTFE(0, uut.acc[2]);
   TUASSERT(isnan(uut.clkBias));
   TUASSERT(isnan(uut.freqBias));
   TUASSERTE(unsigned, (uint8_t)-1, uut.healthBits);
   TUASSERTE(unsigned, -1, uut.tb);
   TUASSERTE(unsigned, -1, uut.P1);
   TUASSERTE(unsigned, -1, uut.P2);
   TUASSERTE(unsigned, -1, uut.P3);
   TUASSERTE(unsigned, -1, uut.P4);
   TUASSERTE(unsigned, -1, uut.interval);
   TUASSERTE(gnsstk::GLOFNavData::PCode, gnsstk::GLOFNavData::PCode::Unknown,
             uut.opStatus);
   TUASSERT(isnan(uut.tauDelta));
   TUASSERTE(unsigned, -1, uut.aod);
   TUASSERTE(unsigned, -1, uut.accIndex);
   TUASSERTE(unsigned, -1, uut.dayCount);
   TUASSERTE(gnsstk::CommonTime, exp, uut.Toe);
   TUASSERTFE(1.0, uut.step);
   TURETURN();
}


unsigned GLOFNavEph_T ::
validateTest()
{
   TUDEF("GLOFNavEph", "validate()");
   gnsstk::GLOFNavEph uut;
      /// @todo Implement some testing when the function has some meat to it.
   TUASSERTE(bool, true, uut.validate());
   TURETURN();
}


unsigned GLOFNavEph_T ::
getXvtTest()
{
   TUDEF("GLOFNavEph", "getXvt()");
   gnsstk::GLOFNavEph uut;
   gnsstk::Xvt xvt, exp1, exp2;
   uut.pos[0] = 15553.6342773;
   uut.pos[1] = -19901.1298828;
   uut.pos[2] = 3553.3354492200001;
   uut.vel[0] = -0.41938495636000001;
   uut.vel[1] = 0.32419204711900002;
   uut.vel[2] = 3.5266609191899998;
   uut.acc[0] = 0;
   uut.acc[1] = -9.3132257461499999e-10;
   uut.acc[2] = -1.86264514923e-09;
   uut.clkBias = 5.0653703510800001e-05;
   uut.freqBias = 1.8189894035500001e-12;
   uut.health = gnsstk::SVHealth::Healthy;
   exp1.x[0] = 15553634.277300000191;
   exp1.x[1] = -19901129.882800001651;
   exp1.x[2] = 3553335.4492200003006;
   exp1.v[0] = -419.38495635999998967;
   exp1.v[1] = 324.19204711899999438;
   exp1.v[2] = 3526.6609191899997313;
   exp1.clkbias = 5.0643837887362983528e-05;
   exp1.clkdrift = 1.8189894035500000529e-12;
   exp1.relcorr = 9.8656234370161929753e-09;
   exp2.x[0] = 15414234.528789049014;
   exp2.x[1] = -19781497.388794392347;
   exp2.x[2] = 4628091.6838117558509;
   exp2.v[0] = -490.60674446407949745;
   exp2.v[1] = 458.15034224403763119;
   exp2.v[2] = 3496.5690971072995126;
   exp2.clkbias = 5.064440682959031175e-05;
   exp2.clkdrift = 1.8189894035500000529e-12;
   exp2.relcorr = 9.8532919671748554905e-09;
      // m_day=2454010, m_msod=900000, GLO
   uut.Toe = gnsstk::CivilTime(2006, 10, 01, 0, 15, 0, gnsstk::TimeSystem::GLO);
   TUASSERTE(bool, true, uut.getXvt(uut.Toe, xvt));
   TUASSERTE(gnsstk::Xvt::HealthStatus,
             gnsstk::Xvt::HealthStatus::Healthy, xvt.health);
   for (unsigned i = 0; i < 3; i++)
   {
      TUASSERTFE(exp1.x[i], xvt.x[i]);
      TUASSERTFE(exp1.v[i], xvt.v[i]);
   }
   TUASSERTFE(exp1.clkbias, xvt.clkbias);
   TUASSERTFE(exp1.clkdrift, xvt.clkdrift);
   TUASSERTFE(exp1.relcorr, xvt.relcorr);
      // same ephemeris, orbit 306 seconds in the future of Toe
   TUASSERTE(bool, true, uut.getXvt(uut.Toe + 306, xvt));
   TUASSERTE(gnsstk::Xvt::HealthStatus,
             gnsstk::Xvt::HealthStatus::Healthy, xvt.health);
   for (unsigned i = 0; i < 3; i++)
   {
      TUASSERTFE(exp2.x[i], xvt.x[i]);
      TUASSERTFE(exp2.v[i], xvt.v[i]);
   }
   TUASSERTFE(exp2.clkbias, xvt.clkbias);
   TUASSERTFE(exp2.clkdrift, xvt.clkdrift);
   TUASSERTFE(exp2.relcorr, xvt.relcorr);
   TURETURN();
}


unsigned GLOFNavEph_T ::
getUserTimeTest()
{
   TUDEF("GLOFNavEph", "getUserTime()");
   gnsstk::GLOFNavEph uut;
   uut.timeStamp = gnsstk::CivilTime(2021,5,19,0,1,13,gnsstk::TimeSystem::GLO);
   uut.xmit2 = gnsstk::CivilTime(2021,5,19,0,1,19,gnsstk::TimeSystem::GLO);
   uut.xmit3 = gnsstk::CivilTime(2021,5,19,0,1,15,gnsstk::TimeSystem::GLO);
   uut.xmit4 = gnsstk::CivilTime(2021,5,19,0,1,17,gnsstk::TimeSystem::GLO);
   gnsstk::CommonTime exp(gnsstk::CivilTime(2021,5,19,0,1,21,
                                            gnsstk::TimeSystem::GLO));
   TUASSERTE(gnsstk::CommonTime, exp, uut.getUserTime());
   TURETURN();
}


unsigned GLOFNavEph_T ::
fixFitTest()
{
   TUDEF("GLOFNavEph", "fixFit()");
   gnsstk::GLOFNavEph uut;
      // test each of the possible interval values (0, 30, 45, 60)
   gnsstk::CommonTime
      bexp(gnsstk::CivilTime(2021,5,19,0,1,13,gnsstk::TimeSystem::GLO)),
      eexp0(gnsstk::CivilTime(2021,5,19,0,45,30,gnsstk::TimeSystem::GLO)),
      eexp30(gnsstk::CivilTime(2021,5,19,0,45,30,gnsstk::TimeSystem::GLO)),
      eexp45(gnsstk::CivilTime(2021,5,19,0,53,0,gnsstk::TimeSystem::GLO)),
      eexp60(gnsstk::CivilTime(2021,5,19,1,0,30,gnsstk::TimeSystem::GLO));
   uut.timeStamp = gnsstk::CivilTime(2021,5,19,0,1,13,gnsstk::TimeSystem::GLO);
   uut.Toe = gnsstk::CivilTime(2021,5,19,0,30,0,gnsstk::TimeSystem::GLO);
   uut.interval = 0;
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, bexp, uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, eexp0, uut.endFit);
   uut.interval = 30;
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, bexp, uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, eexp30, uut.endFit);
   uut.interval = 45;
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, bexp, uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, eexp45, uut.endFit);
   uut.interval = 60;
   TUCATCH(uut.fixFit());
   TUASSERTE(gnsstk::CommonTime, bexp, uut.beginFit);
   TUASSERTE(gnsstk::CommonTime, eexp60, uut.endFit);
   TURETURN();
}


int main()
{
   GLOFNavEph_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.validateTest();
   errorTotal += testClass.getXvtTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.fixFitTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
