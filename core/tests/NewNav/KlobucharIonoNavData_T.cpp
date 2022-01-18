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
#include "KlobucharIonoNavData.hpp"
#include "TestUtil.hpp"
#include "GPSWeekSecond.hpp"

namespace gnsstk
{
   std::ostream& operator<<(std::ostream& s, gnsstk::NavMessageType e)
   {
      s << StringUtils::asString(e);
      return s;
   }
}

class KlobucharIonoNavData_T
{
public:
      /// Make sure constructor initializes data members correctly.
   unsigned constructorTest();
   unsigned getIonoCorrTest();
};


unsigned KlobucharIonoNavData_T ::
constructorTest()
{
   TUDEF("KlobucharIonoNavData", "KlobucharIonoNavData");
   gnsstk::KlobucharIonoNavData uut;
   TUASSERTE(gnsstk::NavMessageType, gnsstk::NavMessageType::Iono,
             uut.signal.messageType);
   TUASSERTFE(0.0, uut.alpha[0]);
   TUASSERTFE(0.0, uut.alpha[1]);
   TUASSERTFE(0.0, uut.alpha[2]);
   TUASSERTFE(0.0, uut.alpha[3]);
   TUASSERTFE(0.0, uut.beta[0]);
   TUASSERTFE(0.0, uut.beta[1]);
   TUASSERTFE(0.0, uut.beta[2]);
   TUASSERTFE(0.0, uut.beta[3]);
   TURETURN();
}


unsigned KlobucharIonoNavData_T ::
getIonoCorrTest()
{
   TUDEF("KlobucharIonoNavData", "getIonoCorr");
   gnsstk::KlobucharIonoNavData uut;
   gnsstk::CommonTime when = gnsstk::GPSWeekSecond(2100,135.0);
   gnsstk::Position rx, sv;
   rx.setECEF(-1575232.0141,-4707872.2332, 3993198.4383);
   sv.setECEF(18217581.007, -14220522.580,  12707796.859);
   uut.alpha[0] =  1.11758709E-08;
   uut.alpha[1] = -2.37159346E-09;
   uut.alpha[2] = -6.03921316E-09;
   uut.alpha[3] =  3.84468251E-09;
   uut.beta[0]  =  1.16736000E+05;
   uut.beta[1]  = -7.30126485E+04;
   uut.beta[2]  = -1.32803702E+04;
   uut.beta[3]  =  3.38181850E+04;
   TUASSERTFE(13.174577965354167475,
              uut.getIonoCorr(when, rx, sv, gnsstk::CarrierBand::L2));
   TUASSERTFE(7.9994064218713107906,
              uut.getIonoCorr(when, rx, sv, gnsstk::CarrierBand::L1));
   TURETURN();
}


int main()
{
   KlobucharIonoNavData_T testClass;
   unsigned errorTotal = 0;

   errorTotal += testClass.constructorTest();
   errorTotal += testClass.getIonoCorrTest();

   std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal
             << std::endl;

   return errorTotal;
}
