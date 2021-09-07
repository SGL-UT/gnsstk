//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
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
   unsigned copyXVTest();
   unsigned copyTTest();
   unsigned getUserTimeTest();
   unsigned getXvtTest();

   gnsstk::CivilTime civ;
   gnsstk::CommonTime ct;
};


OrbitDataSP3_T ::
OrbitDataSP3_T()
      : civ(2015,7,19,2,0,0.0,gnsstk::TimeSystem::GPS),
        ct(civ)
{
}


unsigned OrbitDataSP3_T ::
constructorTest()
{
   TUDEF("OrbitDataSP3", "OrbitDataSP3");
   gnsstk::OrbitDataSP3 uut;
   TUASSERTE(gnsstk::CommonTime, gnsstk::CommonTime::BEGINNING_OF_TIME,
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
copyXVTest()
{
   TUDEF("OrbitDataSP3", "copyXV");
   gnsstk::OrbitDataSP3 uut, from;
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
   unsigned i = 1;
   from.pos[0] = i++;
   from.pos[1] = i++;
   from.pos[2] = i++;
   from.posSig[0] = i++;
   from.posSig[1] = i++;
   from.posSig[2] = i++;
   from.vel[0] = i++;
   from.vel[1] = i++;
   from.vel[2] = i++;
   from.velSig[0] = i++;
   from.velSig[1] = i++;
   from.velSig[2] = i++;
   from.acc[0] = i++;
   from.acc[1] = i++;
   from.acc[2] = i++;
   from.accSig[0] = i++;
   from.accSig[1] = i++;
   from.accSig[2] = i++;
   from.clkBias = i++;
   from.biasSig = i++;
   from.clkDrift = i++;
   from.driftSig = i++;
   from.clkDrRate = i++;
   from.drRateSig = i++;
   TUCATCH(uut.copyXV(from));
      // make sure uut has been updated and from has not.
   i = 1;
   TUASSERTFE((double)i, uut.pos[0]);
   TUASSERTFE((double)i++, from.pos[0]);
   TUASSERTFE((double)i, uut.pos[1]);
   TUASSERTFE((double)i++, from.pos[1]);
   TUASSERTFE((double)i, uut.pos[2]);
   TUASSERTFE((double)i++, from.pos[2]);
   TUASSERTFE((double)i, uut.posSig[0]);
   TUASSERTFE((double)i++, from.posSig[0]);
   TUASSERTFE((double)i, uut.posSig[1]);
   TUASSERTFE((double)i++, from.posSig[1]);
   TUASSERTFE((double)i, uut.posSig[2]);
   TUASSERTFE((double)i++, from.posSig[2]);
   TUASSERTFE((double)i, uut.vel[0]);
   TUASSERTFE((double)i++, from.vel[0]);
   TUASSERTFE((double)i, uut.vel[1]);
   TUASSERTFE((double)i++, from.vel[1]);
   TUASSERTFE((double)i, uut.vel[2]);
   TUASSERTFE((double)i++, from.vel[2]);
   TUASSERTFE((double)i, uut.velSig[0]);
   TUASSERTFE((double)i++, from.velSig[0]);
   TUASSERTFE((double)i, uut.velSig[1]);
   TUASSERTFE((double)i++, from.velSig[1]);
   TUASSERTFE((double)i, uut.velSig[2]);
   TUASSERTFE((double)i++, from.velSig[2]);
   TUASSERTFE((double)i, uut.acc[0]);
   TUASSERTFE((double)i++, from.acc[0]);
   TUASSERTFE((double)i, uut.acc[1]);
   TUASSERTFE((double)i++, from.acc[1]);
   TUASSERTFE((double)i, uut.acc[2]);
   TUASSERTFE((double)i++, from.acc[2]);
   TUASSERTFE((double)i, uut.accSig[0]);
   TUASSERTFE((double)i++, from.accSig[0]);
   TUASSERTFE((double)i, uut.accSig[1]);
   TUASSERTFE((double)i++, from.accSig[1]);
   TUASSERTFE((double)i, uut.accSig[2]);
   TUASSERTFE((double)i++, from.accSig[2]);
   TUASSERTFE(0.0, uut.clkBias);
   TUASSERTFE((double)i++, from.clkBias);
   TUASSERTFE(0.0, uut.biasSig);
   TUASSERTFE((double)i++, from.biasSig);
   TUASSERTFE(0.0, uut.clkDrift);
   TUASSERTFE((double)i++, from.clkDrift);
   TUASSERTFE(0.0, uut.driftSig);
   TUASSERTFE((double)i++, from.driftSig);
   TUASSERTFE(0.0, uut.clkDrRate);
   TUASSERTFE((double)i++, from.clkDrRate);
   TUASSERTFE(0.0, uut.drRateSig);
   TUASSERTFE((double)i++, from.drRateSig);
   TURETURN();
}


unsigned OrbitDataSP3_T ::
copyTTest()
{
   TUDEF("OrbitDataSP3", "copyT");
   gnsstk::OrbitDataSP3 uut, from;
   TUASSERTFE(0.0, uut.clkBias);
   TUASSERTFE(0.0, uut.biasSig);
   TUASSERTFE(0.0, uut.clkDrift);
   TUASSERTFE(0.0, uut.driftSig);
   TUASSERTFE(0.0, uut.clkDrRate);
   TUASSERTFE(0.0, uut.drRateSig);
   unsigned i = 1;
   from.pos[0] = i++;
   from.pos[1] = i++;
   from.pos[2] = i++;
   from.posSig[0] = i++;
   from.posSig[1] = i++;
   from.posSig[2] = i++;
   from.vel[0] = i++;
   from.vel[1] = i++;
   from.vel[2] = i++;
   from.velSig[0] = i++;
   from.velSig[1] = i++;
   from.velSig[2] = i++;
   from.acc[0] = i++;
   from.acc[1] = i++;
   from.acc[2] = i++;
   from.accSig[0] = i++;
   from.accSig[1] = i++;
   from.accSig[2] = i++;
   from.clkBias = i++;
   from.biasSig = i++;
   from.clkDrift = i++;
   from.driftSig = i++;
   from.clkDrRate = i++;
   from.drRateSig = i++;
   TUCATCH(uut.copyT(from));
      // make sure uut has been updated and from has not.
   i = 1;
   TUASSERTFE(0.0, uut.pos[0]);
   TUASSERTFE((double)i++, from.pos[0]);
   TUASSERTFE(0.0, uut.pos[1]);
   TUASSERTFE((double)i++, from.pos[1]);
   TUASSERTFE(0.0, uut.pos[2]);
   TUASSERTFE((double)i++, from.pos[2]);
   TUASSERTFE(0.0, uut.posSig[0]);
   TUASSERTFE((double)i++, from.posSig[0]);
   TUASSERTFE(0.0, uut.posSig[1]);
   TUASSERTFE((double)i++, from.posSig[1]);
   TUASSERTFE(0.0, uut.posSig[2]);
   TUASSERTFE((double)i++, from.posSig[2]);
   TUASSERTFE(0.0, uut.vel[0]);
   TUASSERTFE((double)i++, from.vel[0]);
   TUASSERTFE(0.0, uut.vel[1]);
   TUASSERTFE((double)i++, from.vel[1]);
   TUASSERTFE(0.0, uut.vel[2]);
   TUASSERTFE((double)i++, from.vel[2]);
   TUASSERTFE(0.0, uut.velSig[0]);
   TUASSERTFE((double)i++, from.velSig[0]);
   TUASSERTFE(0.0, uut.velSig[1]);
   TUASSERTFE((double)i++, from.velSig[1]);
   TUASSERTFE(0.0, uut.velSig[2]);
   TUASSERTFE((double)i++, from.velSig[2]);
   TUASSERTFE(0.0, uut.acc[0]);
   TUASSERTFE((double)i++, from.acc[0]);
   TUASSERTFE(0.0, uut.acc[1]);
   TUASSERTFE((double)i++, from.acc[1]);
   TUASSERTFE(0.0, uut.acc[2]);
   TUASSERTFE((double)i++, from.acc[2]);
   TUASSERTFE(0.0, uut.accSig[0]);
   TUASSERTFE((double)i++, from.accSig[0]);
   TUASSERTFE(0.0, uut.accSig[1]);
   TUASSERTFE((double)i++, from.accSig[1]);
   TUASSERTFE(0.0, uut.accSig[2]);
   TUASSERTFE((double)i++, from.accSig[2]);
   TUASSERTFE((double)i, uut.clkBias);
   TUASSERTFE((double)i++, from.clkBias);
   TUASSERTFE((double)i, uut.biasSig);
   TUASSERTFE((double)i++, from.biasSig);
   TUASSERTFE((double)i, uut.clkDrift);
   TUASSERTFE((double)i++, from.clkDrift);
   TUASSERTFE((double)i, uut.driftSig);
   TUASSERTFE((double)i++, from.driftSig);
   TUASSERTFE((double)i, uut.clkDrRate);
   TUASSERTFE((double)i++, from.clkDrRate);
   TUASSERTFE((double)i, uut.drRateSig);
   TUASSERTFE((double)i++, from.drRateSig);
   TURETURN();
}


unsigned OrbitDataSP3_T ::
getUserTimeTest()
{
   TUDEF("OrbitDataSP3", "getUserTime");
   gnsstk::OrbitDataSP3 uut;
   gnsstk::CommonTime t = gnsstk::CivilTime(2020,1,2,3,4,5);
   uut.timeStamp = t;
   TUASSERTE(gnsstk::CommonTime, t, uut.getUserTime());
   TURETURN();
}


unsigned OrbitDataSP3_T ::
getXvtTest()
{
   TUDEF("OrbitDataSP3", "getXvt");
   gnsstk::OrbitDataSP3 uut;
   gnsstk::CommonTime t = gnsstk::CivilTime(2020,1,2,3,4,5);
   uut.timeStamp = t;
   uut.pos[0] = -15381.810046000001;
   uut.pos[1] = -15574.915217;
   uut.pos[2] = -15688.632718000001;
   uut.vel[0] = -15381.810046000001;
   uut.vel[1] = -15574.915217;
   uut.vel[2] = -15688.632718000001;
   uut.clkBias = 123.456789;
   uut.clkDrift = 234.56789;
      // make sure the scales are correct.
   gnsstk::Xvt xvt;
   TUASSERTE(bool, true, uut.getXvt(t, xvt));
   TUASSERTFE(-15381810.046000001, xvt.x[0]);
   TUASSERTFE(-15574915.217, xvt.x[1]);
   TUASSERTFE(-15688632.718000001, xvt.x[2]);
   TUASSERTFE(-1538.1810046000001, xvt.v[0]);
   TUASSERTFE(-1557.4915217, xvt.v[1]);
   TUASSERTFE(-1568.8632718000001, xvt.v[2]);
   TUASSERTFE(0.000123456789, xvt.clkbias);
   TUASSERTFE(0.00023456789, xvt.clkdrift);
   TURETURN();
}


int main()
{
   OrbitDataSP3_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.copyXVTest();
   errorTotal += testClass.copyTTest();
   errorTotal += testClass.getUserTimeTest();
   errorTotal += testClass.getXvtTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}

